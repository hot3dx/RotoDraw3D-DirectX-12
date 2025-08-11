#include "pch.h"
#include "Common\DeviceResources.h"
#include "Hot3dxRotoDrawMain.h"
#include "Content\Hot3dxRotoDrawSceneRender.h"
#include "Common\DirectXHelper.h"


using namespace Hot3dxRotoDraw;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
Hot3dxRotoDrawMain::Hot3dxRotoDrawMain(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources), m_pointerLocationX(0.0f), m_pointerLocationY(0.0f)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	SetSceneRenderer();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

Hot3dxRotoDrawMain::~Hot3dxRotoDrawMain() 
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

void Hot3dxRotoDrawMain::CreateDeviceDependentResources()
{
	m_sceneRenderer->CreateDeviceDependentResources();
}

// Updates application state when the window size changes (e.g. device orientation change)
void Hot3dxRotoDrawMain::CreateWindowSizeDependentResources() 
{
	if (m_sceneRenderer->GetLoadingComplete() == false)return;
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

void Hot3dxRotoDrawMain::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == Windows::Foundation::AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction^ action)
	{
		while (action->Status == Windows::Foundation::AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			////////////////////////////////////////////////////////////////////

			auto commandQueue = m_deviceResources->GetCommandQueue();
			PIXBeginEvent(commandQueue, 0, L"Update");
			{
				Update();
			}
			PIXEndEvent(commandQueue);

			PIXBeginEvent(commandQueue, 0, L"Render");
			{
				if (m_deviceResources->m_isSwapPanelVisible == true)
				{
					m_sceneRenderer->Render();
				}
			}
			PIXEndEvent(commandQueue);
		}
	});
	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void Hot3dxRotoDrawMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void Hot3dxRotoDrawMain::Update() 
{
	ProcessInput();
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
	});

	Render();
}


// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool Hot3dxRotoDrawMain::Render() 
{
	
	if (m_deviceResources->IsDeviceRemoved()) // included by ChatBot
	{
		m_deviceResources->HandleDeviceLost();
		return false;
	}

	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	if (!m_sceneRenderer->GetLoadingComplete())
	{
		return false;
	}

	//auto 
	ID3D12GraphicsCommandList* context = m_deviceResources->GetCommandList();// m_cd3d12GridRenderer->GetComList();// 

	if (context == 0x0000000000000000)
	{
		return m_sceneRenderer->Render();
	}

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	if (m_deviceResources->m_isSwapPanelVisible == true)
	{
		return m_sceneRenderer->Render();
	}

	return true;
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::Clear()
{
	m_sceneRenderer->Clear();
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::OnWindowSizeChanged()
{
	Size size = m_deviceResources->GetOutputSize();
	
	if (!m_deviceResources->WindowSizeChanged(
		(int)size.Width,
		(int)size.Height,
		m_deviceResources->GetRotation()))
		
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::ValidateDevice()
{
	m_deviceResources->ValidateDevice();
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::OnSuspending()
{
	// TODO: Replace this with your app's suspending logic.
    // Process lifetime management may terminate suspended apps at any time, so it is
    // good practice to save any state that will allow the app to restart where it left off.
	m_sceneRenderer->SaveState();
	// If your application uses video memory allocations that are easy to re-create,
	// consider releasing that memory to make it available to other applications.
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::OnResuming()
{
	m_timer.ResetElapsedTime();
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::OnDeviceRemoved()
{
	// TODO: Save any necessary application or renderer state and release the renderer
	// and its resources which are no longer valid.
	   m_sceneRenderer->SaveState();
	   m_sceneRenderer = nullptr;
}

// Notifies renderers that device resources need to be released.
void Hot3dxRotoDrawMain::OnDeviceLost()
{
	m_sceneRenderer->OnDeviceLost();
}

// Notifies renderers that device resources may now be recreated.
void Hot3dxRotoDrawMain::OnDeviceRestored()
{
	m_sceneRenderer->OnDeviceRestored();
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::WindowActivationChanged(Windows::UI::Core::CoreWindowActivationState activationState)
{  
	unsigned int val = static_cast<unsigned int>(activationState);
	switch (val)
	{
	case 0://Windows::UI::Core::CoreWindowActivationState::CodeActivated:
		break;
	case 1://Windows::UI::Core::CoreWindowActivationState::Deactivated:
		break;
	case 2://Windows::UI::Core::CoreWindowActivationState::PointerActivated:
		break;
	default:
		break;
	};
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::KeyDown(Windows::System::VirtualKey key)
{
	int Key = static_cast<int>(key);
	switch (Key)
	{
	case 20:
	default:
		break;
	};
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::KeyUp(Windows::System::VirtualKey key)
{
	int Key = static_cast<int>(key);
	switch (Key)
	{
	case 20:
	default:
		break;
	};
}

void Hot3dxRotoDraw::Hot3dxRotoDrawMain::ProcessInput()
{
	// TODO: Add per frame input handling here.
	m_sceneRenderer->TrackingUpdate(m_pointerLocationX, m_pointerLocationY);
}
