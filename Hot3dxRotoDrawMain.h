#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Hot3dxRotoDrawSceneRender.h"

// This path works because although Hot3dxRotoDraw and DirectXToolkitXaml12 are
// in a separate folder they are both within the next folder up!
// On your computer this and the /WHOLEARCHIVE folders below must match!!!
// The goal will be to put this library in Windows Kits in the appropriate 10.0.18362.0 folders!
// Or C://Hot3dx
// 

#include <AlignedNewXaml12.h>
#include <RenderTargetStateXaml12.h>
#include <EffectPipelineStateDescriptionXaml12.h>
#include <CommonStatesXaml12.h>
#include <GraphicsMemoryXaml12.h>
#include <DescriptorHeapXaml12.h>
#include <EffectCommonXaml12.h>
#include <VertexTypesXaml12.h>
#include <SimpleMathXaml12.h>
#include <ModelXaml12.h>
#include <PrimitiveBatchXaml12.h>
#include <GeometricPrimitiveXaml12.h>
#include <SpriteBatchXaml12.h>
#include <SpriteFontXaml12.h>
#include <Hot3dxCamera.h>
#include <Audio\AudioXaml12.h>
#include <Audio\MediaReaderXaml12.h>
#include <Hot3dxGeometry.h>

// Renders Direct2D and 3D content on the screen.
namespace Hot3dxRotoDraw
{
	class Hot3dxRotoDrawMain : public DX::IDeviceNotify
	{
	public:
		Hot3dxRotoDrawMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~Hot3dxRotoDrawMain();
		void CreateWindowSizeDependentResources();
		void CreateDeviceDependentResources();
		void Update();
		bool Render();
		void Clear();
		void OnSuspending();
		void OnWindowSizeChanged();
		void ValidateDevice();
		void OnResuming();
		void OnDeviceRemoved();
		void StartTracking() { m_sceneRenderer->StartTracking(); }
		void TrackingUpdate(float positionX, float positionY) { m_pointerLocationX = positionX; m_pointerLocationY = positionY; }
		void StopTracking() { m_sceneRenderer->StopTracking(); }
		bool IsTracking() { return m_sceneRenderer->IsTracking(); }
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		// Accessors

		RotoDrawSceneRender^ GetSceneRenderer() { return m_sceneRenderer; }
		void SetSceneRenderer() {
			m_sceneRenderer = ref new RotoDrawSceneRender(m_deviceResources);
		}
		
		void PauseRequested() {
			m_timer.Stop();
			m_pauseRequested = true;
		};
		void PressComplete() {// if (m_updateState == RotoDraw3DX12::UpdateEngineState::WaitingForPress) 
			m_pressComplete = true;
		};


		void WindowActivationChanged(Windows::UI::Core::CoreWindowActivationState activationState);

		void KeyDown(Windows::System::VirtualKey key);
		void KeyUp(Windows::System::VirtualKey key);

		DX::StepTimer GetTimer() { return m_timer; }

	private:
		// Process all input from the user before updating game state
		void ProcessInput();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		RotoDrawSceneRender^ m_sceneRenderer;
		
		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		///////////////////////////////////////////////////
		bool                                                m_pauseRequested;
		bool                                                m_pressComplete;
		bool                                                m_renderNeeded;
		bool                                                m_haveFocus;
		bool                                                m_visible;

		//Hot3dxController^ m_controller;

		///////////////////////////////////////////////////

		// Track current input pointer position.
		float m_pointerLocationX;
		float m_pointerLocationY;

		Windows::UI::Core::CoreWindowActivationState m_activationState;
		Windows::System::VirtualKey m_key;


	};
}