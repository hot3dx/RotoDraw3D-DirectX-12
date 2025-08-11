//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"
#include "OmnidirectionalSound.h"
#include "ContentDialog1.xaml.h"
#include <\DirectXToolKitXaml12\Graphics\ScreenGrabXaml12.h>
#include <guiddef.h>

using namespace Hot3dxRotoDraw;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Automation::Peers;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace concurrency;

DirectXPage^ DirectXPage::Current = nullptr;

Platform::Array<Hot3dxRotoDraw::Scenario>^ Hot3dxRotoDraw::DirectXPage::scenariosInner = ref new Platform::Array<Hot3dxRotoDraw::Scenario>
{
	{ "ColorPicker", "Hot3dxRotoDraw.Scenario6_ColorPicker" },
	{ "Textures", "Hot3dxRotoDraw.Scenario5_Tex" },
	{ "Materials", "Hot3dxRotoDraw.Scenario5_MatsTexs" },
	{ "Draw!", "Hot3dxRotoDraw.Scenario2_Normal" },
	{ "Scale Object", "Hot3dxRotoDraw.Scenario4_MySettings" },
	{ "Rotate Object", "Hot3dxRotoDraw.Scenario9_Rotate" },
	{ "Move Object", "Hot3dxRotoDraw.Scenario12_Translate" },
	{ "FileIO", "Hot3dxRotoDraw.Scenario3_FileIO" },
	{ "Settings", "Hot3dxRotoDraw.Scenario1_Start" },
	{ "SphereDraw", "Hot3dxRotoDraw.Scenario7_SphereDraw" },
	{ "Sculpt", "Hot3dxRotoDraw.Scenario10_Sculpt" },
	{ "Help", "Hot3dxRotoDraw.Scenario8_Help" },
	{ "Grid or Pic", "Hot3dxRotoDraw.Scenario11_GridorPic" },
	{ "ScreenGrab", "Hot3dxRotoDraw.Scenario13_ScreenGrab" },
	// TODO: Example of How To Add the next scenario
	//{ "Something3", "Hot3dxRotoDraw.Scenario_Something3" }
};

Hot3dxRotoDraw::DirectXPage::DirectXPage() :
	m_fPointSpacingDXP(0.7f),
	m_windowVisible(true),
	m_fullscreen(false),
	m_xScaleDrawnObject(1.0f),
	m_yScaleDrawnObject(1.0f),
	m_zScaleDrawnObject(1.0f),
	m_xTranslateDrawnObject(0.0f),
	m_yTranslateDrawnObject(0.0f),
	m_zTranslateDrawnObject(0.0f),
	m_xRotateDrawnObject(0.0f),
	m_yRotateDrawnObject(0.0f),
	m_zRotateDrawnObject(0.0f),
	_initializedDXP(false),
	_timerDXP(),
	_timerEventTokenDXP(),
	_radiusDXP(2.0f),
	_heightDXP(0.0f),
	_angularVelocityDXP(0.0f),
	m_bIfRightShiftKeyHeldDrawStraightLine(false),
	m_bIfLeftShiftKeyHeldDrawStraightLine(false),
	m_bIfLeftShiftKeyHeldDraw45Line(false),
	m_bIfRightShiftKeySphereRadius(false),
    m_backgroundMusic(ref new Platform::String(L"Assets//LiteSatGroovy.mp3")), // musicmono_adpcm.wav");Audiorezout_Entering.mp3")),//;// 
	m_coreInput(nullptr),
	m_bDirPathFound(false)
{
	InitializeComponent();

	DirectXPage::Current = this;

	m_deviceResources = std::make_shared<DeviceResources>();
	m_deviceResources->SetSwapChainPanel(swapChainPanel);
	m_main = std::unique_ptr<Hot3dxRotoDrawMain>(new Hot3dxRotoDrawMain(m_deviceResources));
	// MyFilePathDXP(L"MakerC1.ico"); works but there's something faster
	if (m_bDirPathFound == false)
	{
		m_sDirPathDXP = ref new Platform::String(GetProgramDirPathDXP()->Data());
		m_main->GetSceneRenderer()->m_hot3dxDirPath = ref new Platform::String(m_sDirPathDXP->Data());
		m_bDirPathFound = true;

	}
	
	m_main->StartRenderLoop();
	/*
	ErrorPopup->Width = 0;
	ErrorPopup->Height = 0;
	ErrorMessageStackPanel->Width = 0;
	ErrorMessageStackPanel->Height = 0;
	*/
	Hot3dxRotoDrawVariables^ m_rVars = ref new Hot3dxRotoDrawVariables();

	m_rVars->SetColor(true, 255, 0, 0, 255);
	m_rVars->SetColor(false, 0, 255, 0, 255);

	m_rVars->Setm_brushFrontFaceColorRV(m_rVars->SetColor(true, 255, 0, 0, 255));
	m_rVars->Setm_brushBackFaceColorRV(m_rVars->SetColor(false, 0, 255, 0, 255));
	m_fPointDrawGroupAngleDXP = 10;

	// Register event handlers for page lifecycle.
	CoreWindow^ window = Window::Current->CoreWindow;

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXPage::OnVisibilityChanged);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);

	swapChainPanel->CompositionScaleChanged +=
		ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &DirectXPage::OnCompositionScaleChanged);

	swapChainPanel->SizeChanged +=
		ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);

	window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyDown);

	window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyUp);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	//m_deviceResources = std::make_shared<DeviceResources>();
	//m_deviceResources->SetSwapChainPanel(swapChainPanel);
	//
	// Register our SwapChainPanel to get independent input pointer events
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction^)
	{
		// The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
		m_coreInput = swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
		);

		// Register for pointer events, which will be raised on the background thread.
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerReleased);
		m_coreInput->PointerWheelChanged += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerWheelChanged);
		m_coreInput->PointerEntered += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerEntered);
		m_coreInput->PointerExited += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerExited);
		m_coreInput->PointerCaptureLost += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerCaptureLost);

		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	});

	// Run task on a dedicated high priority background thread.
	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	AudioInitialize();
	/*
	m_main = std::unique_ptr<Hot3dxRotoDrawMain>(new Hot3dxRotoDrawMain(m_deviceResources));
	// MyFilePathDXP(L"MakerC1.ico"); works but there's something faster
	if (m_bDirPathFound == false)
	{
		m_sDirPathDXP = ref new Platform::String(GetProgramDirPathDXP()->Data());
		m_main->GetSceneRenderer()->m_hot3dxDirPath = ref new Platform::String(m_sDirPathDXP->Data());
		m_bDirPathFound = true;

	}
		
	m_main->StartRenderLoop();

	*/
}

	Hot3dxRotoDraw::DirectXPage::~DirectXPage()
{
	AudioStop();
	if (_timerEventTokenDXP.Value != 0)
	{
		_timerDXP->Tick -= _timerEventTokenDXP;
	}

	// Stop rendering and processing events on destruction.
	m_main->StopRenderLoop();
	m_coreInput->Dispatcher->StopProcessEvents();
}

// Saves the current state of the app for suspend and terminate events.
void Hot3dxRotoDraw::DirectXPage::SaveInternalState(IPropertySet^ state)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->Trim();

	// Stop rendering when the app is suspended.
	m_main->StopRenderLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void Hot3dxRotoDraw::DirectXPage::LoadInternalState(IPropertySet^ state)
{
	// Put code to load app state here.

	// Start rendering when the app is resumed.
	m_main->StartRenderLoop();
}

void Hot3dxRotoDraw::DirectXPage::AudioInitialize()
{

	auto hr = _startDXP.Initialize(static_cast<LPCWSTR>(m_backgroundMusic->Data())); //L"Assets//Audiorezout_Entering.mp3");// LiteSatGroovy.mp3");// musicmono_adpcm.wav");
	if (SUCCEEDED(hr))
	{
		_timerDXP = ref new DispatcherTimer();
		_timerEventTokenDXP = _timerDXP->Tick += ref new EventHandler<Platform::Object^>(this, &DirectXPage::OnTimerTickDXP);
		TimeSpan timespan{};
		timespan.Duration = 10000 / 30;
		_timerDXP->Interval = timespan;

		_startDXP.Start();
		_timerDXP->Start();
		NotifyUser("Playing", NotifyType::StatusMessage);
	}
	else
	{
		if (hr == E_NOTIMPL)
		{
			NotifyUser("HRTF API is not supported on this platform. Use X3DAudio API instead - https://code.msdn.microsoft.com/XAudio2-Win32-Samples-024b3933", NotifyType::ErrorMessage);
		}
		else
		{
			throw ref new COMException(hr);
		}
	}

	_initializedDXP = SUCCEEDED(hr);
}

void Hot3dxRotoDraw::DirectXPage::AudioStop()
{
	if (_initializedDXP)
	{
		_startDXP.Stop();
		_timerDXP->Stop();
		NotifyUser("Audio Stopped", NotifyType::StatusMessage);
	}
}

// Window event handlers.

void Hot3dxRotoDraw::DirectXPage::UpdateStatus(String^ strMessage, NotifyType type)
{
	switch (type)
	{
	case NotifyType::StatusMessage:
		StatusBorder->Background = ref new SolidColorBrush(Windows::UI::Colors::GreenYellow);
		break;
	case NotifyType::ErrorMessage:
		StatusBorder->Background = ref new SolidColorBrush(Windows::UI::Colors::Red);
		break;
	default:
		break;
	}

	StatusBlock->Text = strMessage;

	// Collapse the StatusBlock if it has no text to conserve real estate.
	if (StatusBlock->Text != "")
	{
		StatusBorder->Visibility = Windows::UI::Xaml::Visibility::Visible;
		StatusPanel->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}
	else
	{
		StatusBorder->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		StatusPanel->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}

	// Raise an event if necessary to enable a screen reader to announce the status update.
	auto peer = dynamic_cast<FrameworkElementAutomationPeer^>(FrameworkElementAutomationPeer::FromElement(StatusBlock));
	if (peer != nullptr)
	{
		peer->RaiseAutomationEvent(AutomationEvents::LiveRegionChanged);
	}
}

void Hot3dxRotoDraw::DirectXPage::FullScreen_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (m_fullscreen == false)
	{
		if (IsLeftMenuSwapChainPanel == true)
		{
			double Left = 0;
			double Top = 0;
			double Right = 0;
			double Bottom = 0;
			Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom);
			swapChainPanel->Margin::set(Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom));
			swapChainPanel->SetValue(swapChainPanel->MarginProperty, swapChainPanel->Margin);
		}
		else
		{
			double Left = 0;
			double Top = 0;
			double Right = 0;
			double Bottom = 0;
			Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom);
			swapChainPanel->Margin::set(Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom));
			swapChainPanel->SetValue(swapChainPanel->MarginProperty, swapChainPanel->Margin);
			HeaderPanel->Margin::set(Windows::UI::Xaml::Thickness::Thickness(swapChainPanel->ActualWidth, Top, 500, Bottom));
			HeaderPanel->SetValue(HeaderPanel->MarginProperty, HeaderPanel->Margin);
		} // EO if (IsLeftMenuSwapChainPanel == true)
		m_fullscreen = true;
		swapChainPanel->SizeChanged +=
			ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);
	} // EO if (m_fullscreen == false)
}

void Hot3dxRotoDraw::DirectXPage::ScreenGrabDXP()
{
	CoreWindow^ w = Window::Current->CoreWindow;
	auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
	auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
	auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
	auto isShifDown = shiftState != CoreVirtualKeyStates::None;
	isCtrlDown = true;
	isShifDown = true;
	if (isCtrlDown && isShifDown)
	{

		// loads default top image
		FileSavePicker^ pick = ref new  FileSavePicker();
		pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

		auto plainTextExtensions = ref new Platform::Collections::Vector<Platform::String^>();
		Platform::String^ type = ref new Platform::String(L"Windows Bitmap");
		plainTextExtensions->Append(".bmp");
		pick->FileTypeChoices->Insert(type, plainTextExtensions);

		auto plainTextExtensions1 = ref new Platform::Collections::Vector<Platform::String^>();
		Platform::String^ type1 = ref new Platform::String(L"PNG");
		plainTextExtensions1->Append(".png");
		pick->FileTypeChoices->Insert(type1, plainTextExtensions1);

		auto plainTextExtensions2 = ref new Platform::Collections::Vector<Platform::String^>();
		Platform::String^ type2 = ref new Platform::String(L"JPEG");
		plainTextExtensions2->Append(".jpg");
		pick->FileTypeChoices->Insert(type2, plainTextExtensions2);

		auto plainTextExtensions3 = ref new Platform::Collections::Vector<Platform::String^>();
		Platform::String^ type3 = ref new Platform::String(L"TIFF");
		plainTextExtensions3->Append(".tiff");
		pick->FileTypeChoices->Insert(type3, plainTextExtensions3);

		auto plainTextExtensions4 = ref new Platform::Collections::Vector<Platform::String^>();
		Platform::String^ type4 = ref new Platform::String(L"GIF");
		plainTextExtensions4->Append(".gif");
		pick->FileTypeChoices->Insert(type4, plainTextExtensions4);

		auto plainTextExtensions5 = ref new Platform::Collections::Vector<Platform::String^>();
		Platform::String^ type6 = ref new Platform::String(L"Direct Draw Surface");
		plainTextExtensions5->Append(".dds");
		pick->FileTypeChoices->Insert(type6, plainTextExtensions5);

		pick->SuggestedFileName = ref new Platform::String(L"RotoDrawScreenShot");
		
		m_bIsScreenGrabInProgress = true;
		create_task(pick->PickSaveFileAsync()).then([this](StorageFile^ file)
			{
				

				DirectX::DXTKXAML12::ScreenGrab^ grab = ref new DirectX::DXTKXAML12::ScreenGrab();
				HRESULT hr = S_OK;
				if (file != nullptr)
				{
					m_deviceResources->WaitForGpu();

					D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATE_RENDER_TARGET;
					D3D12_RESOURCE_STATES afterState = D3D12_RESOURCE_STATE_RENDER_TARGET;
					std::function<void(IPropertyBag2*)> setCustomProps = nullptr;
					GUID guidContainerFormat;
					bool forceSRGB = false;
					const GUID* targetFormat = (const GUID*)NULL;
					unsigned int guidFormat = m_Scene13Vars->GetGuidFormatForScreenGrabVars();
					switch (guidFormat)
					{
					case 0:
					{
						guidContainerFormat = GUID_ContainerFormatBmp;
						hr = grab->SaveWICTextureToFile(
							m_deviceResources->GetCommandQueue(),
							m_deviceResources->GetRenderTarget(),
							guidContainerFormat,
							file,
							beforeState,
							afterState,
							targetFormat,
							setCustomProps,
							forceSRGB
						);
						
					}break;
					case 1:
					{
						guidContainerFormat = GUID_ContainerFormatPng;
						//hr = grab->SaveTextureToPngFile(ID3D12CommandQueue * pCommandQ, ID3D12Resource * pSource, Windows::Storage::StorageFile ^ file, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
						hr = grab->SaveWICTextureToFile(
							m_deviceResources->GetCommandQueue(),
							m_deviceResources->GetRenderTarget(),
							guidContainerFormat,
							file,
							beforeState,
							afterState,
							targetFormat,
							setCustomProps,
							forceSRGB
						);
						
					}break;
					case 2:
					{
						guidContainerFormat = GUID_ContainerFormatIco;
						hr = grab->SaveWICTextureToFile(
							m_deviceResources->GetCommandQueue(),
							m_deviceResources->GetRenderTarget(),
							guidContainerFormat,
							file,
							beforeState,
							afterState,
							targetFormat,
							setCustomProps,
							forceSRGB
						);
						
					}break;
					case 3:
					{
						guidContainerFormat = GUID_ContainerFormatJpeg;
						hr = grab->SaveWICTextureToFile(
							m_deviceResources->GetCommandQueue(),
							m_deviceResources->GetRenderTarget(),
							guidContainerFormat,
							file,
							beforeState,
							afterState,
							targetFormat,
							setCustomProps,
							forceSRGB
						);
						
					}break;
					case 4:
					{
						forceSRGB = true;
						guidContainerFormat = GUID_ContainerFormatTiff;
						hr = grab->SaveWICTextureToFile(
							m_deviceResources->GetCommandQueue(),
							m_deviceResources->GetRenderTarget(),
							guidContainerFormat,
							file,
							beforeState,
							afterState,
							targetFormat,
							setCustomProps,
							forceSRGB
						);
						
						
					}break;
					case 5:
					{
						guidContainerFormat = GUID_ContainerFormatGif;
						hr = grab->SaveWICTextureToFile(
							m_deviceResources->GetCommandQueue(),
							m_deviceResources->GetRenderTarget(),
							guidContainerFormat,
							file,
							beforeState,
							afterState,
							targetFormat,
							setCustomProps,
							forceSRGB
						);
						
					}	break;
					case 6:
					{
						guidContainerFormat = GUID_ContainerFormatWmp;
						hr = grab->SaveWICTextureToFile(
							m_deviceResources->GetCommandQueue(),
							m_deviceResources->GetRenderTarget(),
							guidContainerFormat,
							file,
							beforeState,
							afterState,
							targetFormat,
							setCustomProps,
							forceSRGB);
						
					}break;
					default:
					{
						guidContainerFormat = GUID_ContainerFormatDds;
						hr = grab->SaveDDSTextureToFile(
							m_deviceResources->GetCommandQueue(),
							m_deviceResources->GetRenderTarget(),
							file,
							beforeState,
							afterState);
						
					}break;

						
					}
					/*
					if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
						HRESULT reason = m_deviceResources->GetD3DDevice()->GetDeviceRemovedReason();
						// Optionally log 'reason'
						m_deviceResources->HandleDeviceLost();
						NotifyUser(L"Device lost during screen grab. Resources recreated.", NotifyType::ErrorMessage);
						//return;
					}
					*/
					NotifyUser(L"ScreenGrab File Saved" + file->Path, NotifyType::StatusMessage);
				}// eo if create_task
				else
				{
					
					NotifyUser(L"ScreenGrab File Save Failure Error", NotifyType::StatusMessage);
					
				}
				grab->~ScreenGrab();
				m_Scene13Vars->GetScenario13Page()->SetAllUnChecked();
			}).then([this](task<void> previousTask) {
				
				OutputDebugString(L"\n\nMaybe it worked, maybe not.\n");
				this->m_bIsScreenGrabInProgress = false;
				
				
		});
			
			if (m_bIsScreenGrabInProgress)
			{
				return; // or queue the clear for later
			}
			else
			{
				m_main->GetSceneRenderer()->ClearDrawnObject();
				m_bIsScreenGrabInProgress = true;
			}
				//eo switch ctrlswitch
		/* ScreenGrab* grab = new ScreenGrab();
			grab->SaveDDSTextureToFile(
			m_deviceResources->GetCommandQueue(),
			m_deviceResources->GetRenderTarget(),
			file,
			fileName,
			beforeState,
			afterState);// noexcept

		grab->SaveWICTextureToFile(
			m_deviceResources->GetCommandQueue(),
			m_deviceResources->GetRenderTarget(),
			guidContainerFormat,
			file,
			fileNameW,
			beforeState,
			afterState,
			targetFormat,
			setCustomProps,
			forceSRGB
		);
		*/
	}
}

void Hot3dxRotoDraw::DirectXPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	// Populate the ListBox with the scenarios as defined in SampleConfiguration.cpp.
	auto itemCollection = ref new Platform::Collections::Vector<Object^>();
	int i = 1;
	for (auto const& s : DirectXPage::Current->scenarios)
	{
		// Create a textBlock to hold the content and apply the ListItemTextStyle from Styles.xaml
		TextBlock^ textBlock = ref new TextBlock();
		ListBoxItem^ item = ref new ListBoxItem();
		auto style = App::Current->Resources->Lookup("ListItemTextStyle");

		textBlock->Text = (i++).ToString() + ") " + s.Title;
		textBlock->Style = safe_cast<Windows::UI::Xaml::Style^>(style);

		item->Name = s.ClassName;
		item->Content = textBlock;
		itemCollection->Append(item);
	}

	// Set the newly created itemCollection as the ListBox ItemSource.
	ScenarioControl->ItemsSource = itemCollection;
	int startingScenarioIndex;

	if (Window::Current->Bounds.Width < 640)
	{
		startingScenarioIndex = -1;
	}
	else
	{
		startingScenarioIndex = 0;
	}

	ScenarioControl->SelectedIndex = startingScenarioIndex;
	ScenarioControl->ScrollIntoView(ScenarioControl->SelectedItem);
}

void Hot3dxRotoDraw::DirectXPage::ScenarioControl_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	ListBox^ scenarioListBox = safe_cast<ListBox^>(sender); //as ListBox;
	ListBoxItem^ item = dynamic_cast<ListBoxItem^>(scenarioListBox->SelectedItem);
	if (item != nullptr)
	{
		// Clear the status block when changing scenarios
		NotifyUser("", NotifyType::StatusMessage);

		// Navigate to the selected scenario.
		TypeName scenarioType = { item->Name, TypeKind::Custom };
		ScenarioFrame->Navigate(scenarioType, this);

		if (Window::Current->Bounds.Width < 640)
		{
			Splitter->IsPaneOpen = false;
		}
	}
}

void DirectXPage::Footer_Click(Object^ sender, RoutedEventArgs^ e)
{
	auto uri = ref new Uri((String^)((HyperlinkButton^)sender)->Tag);
	Windows::System::Launcher::LaunchUriAsync(uri);
}

void DirectXPage::Button_Click(Object^ sender, RoutedEventArgs^ e)
{
	Splitter->IsPaneOpen = !Splitter->IsPaneOpen;
}

void Hot3dxRotoDraw::DirectXPage::SetLeftSwapChainPanel()
{
	double Left = 500;
	double Top = 0;
	double Right = 0;
	double Bottom = 0;
	Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom);
	swapChainPanel->Margin::set(Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom));
	swapChainPanel->SetValue(swapChainPanel->MarginProperty, swapChainPanel->Margin);
	Left = -500;
	Top = 0;
	Right = 0;
	Bottom = 0;
	Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom);
	HeaderPanel->Margin::set(Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom));
	HeaderPanel->SetValue(HeaderPanel->MarginProperty, HeaderPanel->Margin);
	HeaderPanel->HorizontalAlignment::set(Windows::UI::Xaml::HorizontalAlignment::Left);
	HeaderPanel->SetValue(HeaderPanel->HorizontalAlignmentProperty, Windows::UI::Xaml::HorizontalAlignment::Left);
	IsLeftMenuSwapChainPanel = true;
}

void Hot3dxRotoDraw::DirectXPage::SetRightSwapChainPanel()
{
	double Left = 0;
	double Top = 0;
	double Right = 500;
	double Bottom = 0;
	Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom);
	swapChainPanel->Margin::set(Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom));
	swapChainPanel->SetValue(swapChainPanel->MarginProperty, swapChainPanel->Margin);
	Left = swapChainPanel->ActualWidth;// 866;
	Top = 0;
	Right = -500;
	Bottom = 0;
	Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom);
	HeaderPanel->Margin::set(Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom));
	HeaderPanel->SetValue(HeaderPanel->MarginProperty, HeaderPanel->Margin);
	HeaderPanel->HorizontalAlignment::set(Windows::UI::Xaml::HorizontalAlignment::Right);
	HeaderPanel->SetValue(HeaderPanel->HorizontalAlignmentProperty, Windows::UI::Xaml::HorizontalAlignment::Right);
	IsLeftMenuSwapChainPanel = false;
}

void Hot3dxRotoDraw::DirectXPage::ErrorMsgButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e, Platform::String^ msgType, Platform::String^ message)
{
	Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([msgType, message, this]()
		{
			ContentDialog1^ dlg = ref new ContentDialog1();
			dlg->ContentDialog_SetTitle(msgType);
			dlg->ContentDialog_SetTextBlock(message);
			Windows::Foundation::IAsyncOperation<ContentDialogResult>^ result =
				dlg->ShowAsync();
			if (result->GetResults() == ContentDialogResult::Primary) {}
			if (result->GetResults() == ContentDialogResult::Secondary) {}
		}));
	NotifyUser(message, NotifyType::StatusMessage);
	
}

void Hot3dxRotoDraw::DirectXPage::SetErrorMessagePopup(Platform::String^ msgType, Platform::String^ message)
{
	
	NotifyUser(message, NotifyType::StatusMessage);
	Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([msgType, message, this]()
		{
			ContentDialog1^ dlg = ref new ContentDialog1();
			dlg->ContentDialog_SetTitle(msgType);
			dlg->ContentDialog_SetTextBlock(message);
			Windows::Foundation::IAsyncOperation<ContentDialogResult>^ result =
				dlg->ShowAsync();
			if (result->GetResults() == ContentDialogResult::Primary) {
				this->m_main->GetSceneRenderer()->ClearDrawnObject();
				//CreateDeviceDependentResources();
			}
			if (result->GetResults() == ContentDialogResult::Secondary) {
				this->m_main->GetSceneRenderer()->ClearDrawnObject();
			}
		}));
	// Raise an event if necessary to enable a screen reader to announce the status update.
	auto peer = dynamic_cast<FrameworkElementAutomationPeer^>(FrameworkElementAutomationPeer::FromElement(StatusBlock));
	if (peer != nullptr)
	{
		peer->RaiseAutomationEvent(AutomationEvents::LiveRegionChanged);

	}
	
}

void Hot3dxRotoDraw::DirectXPage::OnRendering(Platform::Object^ sender, Platform::Object^ args)
{
	// Raise an event if necessary to enable a screen reader to announce the status update.
	auto peer = dynamic_cast<FrameworkElementAutomationPeer^>(FrameworkElementAutomationPeer::FromElement(StatusBlock));
	if (peer != nullptr)
	{
		peer->RaiseAutomationEvent(AutomationEvents::LiveRegionChanged);
	}
}
// DirectXPage::OnVisibilityChanged
//
// m_deviceResources->m_isSwapPanelVisible
// allows 3D environment to show
// replaces start button_click
//
void DirectXPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
	if (m_windowVisible)
	{
		m_deviceResources->m_isSwapPanelVisible = true;
		m_main->StartRenderLoop();
	}
	else
	{
		m_deviceResources->m_isSwapPanelVisible = false;
		m_main->StopRenderLoop();
	}
}

// DisplayInformation event handlers.

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->ValidateDevice();
}

// Called when the app bar button is clicked.
void DirectXPage::AppBarButton_Click(Object^ sender, RoutedEventArgs^ e)
{
	// Use the app bar if it is appropriate for your app. Design the app bar, 
	// then fill in event handlers (like this one).
}

void DirectXPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
	if (e->CurrentPoint->Properties->IsLeftButtonPressed) { m_main->GetSceneRenderer()->Setm_bLButtonDown(true);
	}
	if (e->CurrentPoint->Properties->IsMiddleButtonPressed) { m_main->GetSceneRenderer()->Setm_bMButtonDown(true);
	}
	if (e->CurrentPoint->Properties->IsRightButtonPressed) { m_main->GetSceneRenderer()->Setm_bRButtonDown(true); 
	}
	// When the pointer is pressed begin tracking the pointer movement.
	m_main->StartTracking();
}

void DirectXPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e)
{
	m_main->StartTracking();
	// Update the pointer tracking code.
	if (m_main->IsTracking())
	{
		m_main->TrackingUpdate(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	}
	if (e->CurrentPoint->Properties->IsLeftButtonPressed) { m_main->GetSceneRenderer()->Setm_bMouseMove(true); }
	if (e->CurrentPoint->Properties->IsMiddleButtonPressed) { m_main->GetSceneRenderer()->Setm_bMouseMove(true); }
	if (e->CurrentPoint->Properties->IsRightButtonPressed) { m_main->GetSceneRenderer()->Setm_bMouseMove(true); }
}

void DirectXPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e)
{
	// Stop tracking pointer movement when the pointer is released.
	if (!e->CurrentPoint->Properties->IsLeftButtonPressed) { m_main->GetSceneRenderer()->Setm_bLButtonDown(false);
	}
	if (!e->CurrentPoint->Properties->IsMiddleButtonPressed) { m_main->GetSceneRenderer()->Setm_bMButtonDown(false);
	}
	if (!e->CurrentPoint->Properties->IsRightButtonPressed) { m_main->GetSceneRenderer()->Setm_bRButtonDown(false);
	}

}

void DirectXPage::OnPointerWheelChanged(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	// negative delta wheel rolled backward - positive delta wheel rolled forward..
	int delta = e->CurrentPoint->Properties->MouseWheelDelta;
	m_mouseWheelDeltaDXP = delta;
}

void DirectXPage::OnPointerEntered(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	//
}

void DirectXPage::OnPointerExited(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	//
}

void DirectXPage::OnPointerCaptureLost(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	//
}

float DirectXPage::FrameChange()
{
	StepTimer timer = m_main->GetTimer();
	float frameTime = static_cast<float>(timer.GetElapsedSeconds());
	float frameChange = 2.0f * frameTime;
	return frameChange;
}
void DirectXPage::RotateYaw(float deg)
{
	float x = m_main->GetSceneRenderer()->GetEyeX();
	float y = m_main->GetSceneRenderer()->GetEyeY();
	float z = m_main->GetSceneRenderer()->GetEyeZ();
	float xx = m_main->GetSceneRenderer()->GetUpX();
	float yy = m_main->GetSceneRenderer()->GetUpY();
	float zz = m_main->GetSceneRenderer()->GetUpZ();
	XMVECTOR mEye = XMVectorSet(x, y, z, 0.0f);
	XMVECTOR mUp = XMVectorSet(xx, yy, zz, 0.0f);
	XMMATRIX rotation = XMMatrixRotationAxis(mUp, deg);

	mEye = XMVector3TransformCoord(mEye, rotation);
	m_main->GetSceneRenderer()->Setm_EyeX(XMVectorGetX(mEye));
	m_main->GetSceneRenderer()->Setm_EyeY(XMVectorGetY(mEye));
	m_main->GetSceneRenderer()->Setm_EyeZ(XMVectorGetZ(mEye));
}

void DirectXPage::RotatePitch(float deg)
{
	float x = m_main->GetSceneRenderer()->GetEyeX();
	float y = m_main->GetSceneRenderer()->GetEyeY();
	float z = m_main->GetSceneRenderer()->GetEyeZ();
	float xx = m_main->GetSceneRenderer()->GetUpX();
	float yy = m_main->GetSceneRenderer()->GetUpY();
	float zz = m_main->GetSceneRenderer()->GetUpZ();
	XMVECTOR mEye = XMVectorSet(x, y, z, 0.0f);
	XMVECTOR mUp = XMVectorSet(xx, yy, zz, 0.0f);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(mEye, mUp));
	XMMATRIX rotation = XMMatrixRotationAxis(right, deg);

	mEye = XMVector3TransformCoord(mEye, rotation);
	m_main->GetSceneRenderer()->Setm_EyeX(XMVectorGetX(mEye));
	m_main->GetSceneRenderer()->Setm_EyeY(XMVectorGetY(mEye));
	m_main->GetSceneRenderer()->Setm_EyeZ(XMVectorGetZ(mEye));
}

void DirectXPage::OnKeyDown(Windows::UI::Core::CoreWindow^ window, Windows::UI::Core::KeyEventArgs^ args)
{
	auto Key = args->VirtualKey;
	switch (Key)
	{
	case VirtualKey::F4:
	{
		//m_windowClosed(true)
		
	}break;
	case VirtualKey::F5:
	{
		//m_windowClosed(true)
		
	}
	case VirtualKey::P:       // Pause
	{
		
		//m_pauseKeyActive(true)
	}break;
	case VirtualKey::Home:
	{
		
		//m_homeKeyActive(true)
	}break;
	case VirtualKey::F://Fullscreen toggle OFF
	{
		if (m_fullscreen == true)
		{
			if (IsLeftMenuSwapChainPanel == true)
			{
				SetLeftSwapChainPanel();
				IsLeftMenuSwapChainPanel = true;
			}
			else
			{
				double Left = 0;
				double Top = 0;
				double Right = 500;
				double Bottom = 0;
				Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom);
				swapChainPanel->Margin::set(Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom));
				swapChainPanel->SetValue(swapChainPanel->MarginProperty, swapChainPanel->Margin);
				Left = swapChainPanel->ActualWidth - 500;// 866;
				Top = 0;
				Right = -500;
				Bottom = 0;
				Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom);
				HeaderPanel->Margin::set(Windows::UI::Xaml::Thickness::Thickness(Left, Top, Right, Bottom));
				HeaderPanel->SetValue(HeaderPanel->MarginProperty, HeaderPanel->Margin);
				IsLeftMenuSwapChainPanel = false;
			}
			m_fullscreen = false;
		}
		//m_fullscreen = false;
	}break;
	case VirtualKey::G:
		if (m_fullscreen == false)
		{
			FullScreen_Click(nullptr, nullptr);
			m_deviceResources->WaitForGpu();
		}
	case VirtualKey::Q:
	{
		m_main->GetSceneRenderer()->Setm_EyeZ(1.0f);
	}break;
	case VirtualKey::W:
	{
		m_main->GetSceneRenderer()->Setm_EyeY(-1.0f);
		m_main->GetSceneRenderer()->Setm_LookAtY(-1.0f);
	}break;
	case VirtualKey::E:
	{
		m_main->GetSceneRenderer()->Setm_EyeZ(-1.0f);
	}break;
	case VirtualKey::A:
	{
		m_main->GetSceneRenderer()->Setm_EyeX(1.0f);
		m_main->GetSceneRenderer()->Setm_LookAtX(1.0f);
	}break;
	case VirtualKey::S:
	{
		m_main->GetSceneRenderer()->Setm_EyeY(1.0f);
		m_main->GetSceneRenderer()->Setm_LookAtY(1.0f);
	}break;
	case VirtualKey::D:
	{
		m_main->GetSceneRenderer()->Setm_EyeX(-1.0f);
		m_main->GetSceneRenderer()->Setm_LookAtX(-1.0f);

	}break;
	case VirtualKey::Z:
	{
		m_main->GetSceneRenderer()->SetMouseWidthRatio(-0.0001f);
		m_main->GetSceneRenderer()->MouseWidthHeightRatioOutput();
	}break;
	case VirtualKey::X:
	{
		m_main->GetSceneRenderer()->SetMouseWidthRatio(0.0001f);
		m_main->GetSceneRenderer()->MouseWidthHeightRatioOutput();
	}break;
	case VirtualKey::C:
	{
		m_main->GetSceneRenderer()->SetMouseHeightRatio(-0.0001f);
		m_main->GetSceneRenderer()->MouseWidthHeightRatioOutput();
	}break;
	case VirtualKey::V:
	{
		m_main->GetSceneRenderer()->SetMouseHeightRatio(0.0001f);
		m_main->GetSceneRenderer()->MouseWidthHeightRatioOutput();
	}break;
	case VirtualKey::Left:
	{
		m_main->GetSceneRenderer()->SetRotateKeyPressed(true);
		m_main->GetSceneRenderer()->RotateYaw(-XM_PIDIV4);

	}break;
	case VirtualKey::Right:
	{
		m_main->GetSceneRenderer()->SetRotateKeyPressed(true);
		m_main->GetSceneRenderer()->RotateYaw(XM_PIDIV4);

	}break;
	case VirtualKey::Up:
	{
		m_main->GetSceneRenderer()->SetRotateKeyPressed(true);
		m_main->GetSceneRenderer()->RotatePitch(XM_PIDIV4);


	}break;
	case VirtualKey::Down:
	{
		m_main->GetSceneRenderer()->SetRotateKeyPressed(true);
		m_main->GetSceneRenderer()->RotatePitch(-XM_PIDIV4);


	}break;
	case VirtualKey::N:
	{ /*/
		CoreWindow^ w = Window::Current->CoreWindow;
		auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
		auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
		auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
		auto isShiftDown = shiftState != CoreVirtualKeyStates::None;
		if (isCtrlDown && isShiftDown)
		{
			m_bIfRightShiftKeyHeldDrawStraightLine = true;
		}*/
	}break;
	case VirtualKey::M:
	{
		/*
		CoreWindow^ w = Window::Current->CoreWindow;
		auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
		auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
		auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
		auto isShiftDown = shiftState != CoreVirtualKeyStates::None;
		if (isCtrlDown && isShiftDown)
		{
			m_bIfLeftShiftKeyHeldDrawStraightLine = true;
		}*/
	}break;
	case VirtualKey::L:
	{
	/*
		CoreWindow^ w = Window::Current->CoreWindow;
		auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
		auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
		auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
		auto isShiftDown = shiftState != CoreVirtualKeyStates::None;
		if (isCtrlDown && isShiftDown)
		{
			m_bIfLeftShiftKeyHeldDraw45Line = true;
		}*/
	}break;
	case VirtualKey::K:
	{ /*
		CoreWindow^ w = Window::Current->CoreWindow;
		auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
		auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
		auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
		auto isShiftDown = shiftState != CoreVirtualKeyStates::None;
		if (isCtrlDown && isShiftDown)
		{
			m_bIfRightShiftKeySphereRadius = true;
		} */
	}break;
	case VirtualKey::B:
	{
		CoreWindow^ w = Window::Current->CoreWindow;
		auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
		auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
		auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
		auto isShifDown = shiftState != CoreVirtualKeyStates::None;
		if (isCtrlDown && isShifDown)
		{
			
				// loads default top image
				FileSavePicker^ pick = ref new  FileSavePicker();
				pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

				auto plainTextList = ref new Platform::Collections::Vector<Platform::String^>();
				Platform::String^ typeList = ref new Platform::String(L"Choose From: dds, bmp, png, jpg, tiff, gif");
				plainTextList->Append(".bmp");
				pick->FileTypeChoices->Insert(typeList, plainTextList);

				auto plainTextExtensions = ref new Platform::Collections::Vector<Platform::String^>();
				Platform::String^ type = ref new Platform::String(L"Windows Bitmap");
				plainTextExtensions->Append(".bmp");
				pick->FileTypeChoices->Insert(type, plainTextExtensions);

				auto plainTextExtensions1 = ref new Platform::Collections::Vector<Platform::String^>();
				Platform::String^ type1 = ref new Platform::String(L"PNG");
				plainTextExtensions1->Append(".png");
				pick->FileTypeChoices->Insert(type1, plainTextExtensions1);

				auto plainTextExtensions2 = ref new Platform::Collections::Vector<Platform::String^>();
				Platform::String^ type2 = ref new Platform::String(L"JPEG");
				plainTextExtensions2->Append(".jpg");
				pick->FileTypeChoices->Insert(type2, plainTextExtensions2);

				auto plainTextExtensions3 = ref new Platform::Collections::Vector<Platform::String^>();
				Platform::String^ type3 = ref new Platform::String(L"TIFF");
				plainTextExtensions3->Append(".tiff");
				pick->FileTypeChoices->Insert(type3, plainTextExtensions3);

				auto plainTextExtensions4 = ref new Platform::Collections::Vector<Platform::String^>();
				Platform::String^ type4 = ref new Platform::String(L"GIF");
				plainTextExtensions4->Append(".gif");
				pick->FileTypeChoices->Insert(type4, plainTextExtensions4);

				auto plainTextExtensions5 = ref new Platform::Collections::Vector<Platform::String^>();
				Platform::String^ type6 = ref new Platform::String(L"Direct Draw Surface");
				plainTextExtensions5->Append(".dds");
				pick->FileTypeChoices->Insert(type6, plainTextExtensions5);

				pick->SuggestedFileName = ref new Platform::String(L"RotoDrawScreenShot");
				Platform::String^ fileNameS = ref new Platform::String();
				Platform::String^ s = ref new Platform::String(L"C:\\Hot3dxRotoDraw\\x64\\Release\\Hot3dxRotoDraw\\AppX\\Assets\\Textures\\");
				//s = L"Assets\\Textures\\";
				Platform::String^ sfile = ref new Platform::String(s->Data());
				sfile = sfile->Concat(sfile, fileNameS);


				create_task(pick->PickSaveFileAsync()).then([this](StorageFile^ file)
					{
						if (file != nullptr)
						{
							D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATE_RENDER_TARGET;
							D3D12_RESOURCE_STATES afterState = D3D12_RESOURCE_STATE_RENDER_TARGET;
							std::function<void(IPropertyBag2*)> setCustomProps = nullptr;
							GUID guidContainerFormat;
							bool forceSRGB = false;
							const GUID* targetFormat = (const GUID*)NULL;
							unsigned int guidFormat = m_Scene13Vars->GetGuidFormatForScreenGrabVars();
							switch (guidFormat)
							{
							case 0:
							{
								guidContainerFormat = GUID_ContainerFormatBmp;
								SaveWICTextureToFile(
									m_deviceResources->GetCommandQueue(),
									m_deviceResources->GetRenderTarget(),
									guidContainerFormat,
									file->Path->Data(),
									beforeState,
									afterState,
									targetFormat,
									setCustomProps,
									forceSRGB
								);
							}break;
							case 1:
							{
								guidContainerFormat = GUID_ContainerFormatPng;
								SaveWICTextureToFile(
									m_deviceResources->GetCommandQueue(),
									m_deviceResources->GetRenderTarget(),
									guidContainerFormat,
									file->Path->Data(),
									beforeState,
									afterState,
									targetFormat,
									setCustomProps,
									forceSRGB
								);
							}break;
							case 2:
							{
								guidContainerFormat = GUID_ContainerFormatIco;
								SaveWICTextureToFile(
									m_deviceResources->GetCommandQueue(),
									m_deviceResources->GetRenderTarget(),
									guidContainerFormat,
									file->Path->Data(),
									beforeState,
									afterState,
									targetFormat,
									setCustomProps,
									forceSRGB
								);
							}break;
							case 3:
							{
								guidContainerFormat = GUID_ContainerFormatJpeg;
								SaveWICTextureToFile(
									m_deviceResources->GetCommandQueue(),
									m_deviceResources->GetRenderTarget(),
									guidContainerFormat,
									file->Path->Data(),
									beforeState,
									afterState,
									targetFormat,
									setCustomProps,
									forceSRGB
								);
							}break;
							case 4:
							{
								guidContainerFormat = GUID_ContainerFormatTiff;
								SaveWICTextureToFile(
									m_deviceResources->GetCommandQueue(),
									m_deviceResources->GetRenderTarget(),
									guidContainerFormat,
									file->Path->Data(),//fileNameW,
									beforeState,
									afterState,
									targetFormat,
									setCustomProps,
									forceSRGB
								);
								forceSRGB = true;
							}break;
							case 5:
							{
								//const wchar_t* fileNameW = L"C:\\Users\\hot3d\\Pictures\\ScreenShots\\ScreenGrab.gif";
								guidContainerFormat = GUID_ContainerFormatGif;
								SaveWICTextureToFile(
									m_deviceResources->GetCommandQueue(),
									m_deviceResources->GetRenderTarget(),
									guidContainerFormat,
									file->Path->Data(),//fileNameW,
									beforeState,
									afterState,
									targetFormat,
									setCustomProps,
									forceSRGB
								);
							}	break;
							case 6:
							{
								//const wchar_t* fileNameW = L"C:\\Users\\hot3d\\Pictures\\ScreenShots\\ScreenGrab.wmp";
								guidContainerFormat = GUID_ContainerFormatWmp;
								SaveWICTextureToFile(
									m_deviceResources->GetCommandQueue(),
									m_deviceResources->GetRenderTarget(),
									guidContainerFormat,
									file->Path->Data(),//fileNameW,
									beforeState,
									afterState,
									targetFormat,
									setCustomProps,
									forceSRGB
								);
							}break;
							default:
							{
								guidContainerFormat = GUID_ContainerFormatDds;
								DirectX::SaveDDSTextureToFile(
									m_deviceResources->GetCommandQueue(),
									m_deviceResources->GetRenderTarget(),
									file->Path->Data(),//fileNameW,fileName->Data(),
									beforeState,
									afterState);
							}break;
							}


							NotifyUser(L"ScreenGrab File Saved" + file->Path, NotifyType::StatusMessage);
						}// eo if create_task
						else
						{
							NotifyUser(L"ScreenGrab File Save Failure Error", NotifyType::StatusMessage);
						}
					});

				//eo switch ctrlswitch
	/* ScreenGrab* grab = new ScreenGrab();
		grab->SaveDDSTextureToFile(
		m_deviceResources->GetCommandQueue(),
		m_deviceResources->GetRenderTarget(),
		file,
		fileName,
		beforeState,
		afterState);// noexcept

	grab->SaveWICTextureToFile(
		m_deviceResources->GetCommandQueue(),
		m_deviceResources->GetRenderTarget(),
		guidContainerFormat,
		file,
		fileNameW,
		beforeState,
		afterState,
		targetFormat,
		setCustomProps,
		forceSRGB
	);
	*/
			}break; // eo VirtualKey O

		} //eo if (isCtrlDown && isShifDown)
	} // eo VirtualKey::B:
}

void DirectXPage::OnKeyUp(Windows::UI::Core::CoreWindow^ /*window*/, Windows::UI::Core::KeyEventArgs^ args)
{
	//auto Key = args->VirtualKey;
	/*
	switch (Key)
	{
	case VirtualKey::N:
	{
		CoreWindow^ w = Window::Current->CoreWindow;
		auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
		auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
		auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
		auto isShiftDown = shiftState != CoreVirtualKeyStates::None;
		if (isCtrlDown && isShiftDown)
		{
			m_bIfRightShiftKeyHeldDrawStraightLine = false;
		}
	}break;
	case VirtualKey::M:
	{
		CoreWindow^ w = Window::Current->CoreWindow;
		auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
		auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
		auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
		auto isShiftDown = shiftState != CoreVirtualKeyStates::None;
		if (isCtrlDown && isShiftDown)
		{
			m_bIfLeftShiftKeyHeldDrawStraightLine = false;
		}
	}break;
	case VirtualKey::L:
	{
		CoreWindow^ w = Window::Current->CoreWindow;
		auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
		auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
		auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
		auto isShiftDown = shiftState != CoreVirtualKeyStates::None;
		if (isCtrlDown && isShiftDown)
		{
			m_bIfLeftShiftKeyHeldDraw45Line = false;
		}
	}break;
	case VirtualKey::K:
	{
		CoreWindow^ w = Window::Current->CoreWindow;
		auto ctrlState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Control);
		auto isCtrlDown = ctrlState != CoreVirtualKeyStates::None;
		auto shiftState = w->GetForCurrentThread()->GetKeyState(VirtualKey::Shift);
		auto isShiftDown = shiftState != CoreVirtualKeyStates::None;
		if (isCtrlDown && isShiftDown)
		{
			m_bIfRightShiftKeySphereRadius = false;
		}
	}break;
	}// eo switch(Key
	*/
	if (static_cast<UINT>(args->VirtualKey) < 256)
	{
		m_main->GetSceneRenderer()->SetRotateKeyPressed(false);
	}
}

/// <summary>
/// Runs the Hot3dxSceneRenderer SetPoints() function to draw the object
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void Hot3dxRotoDraw::DirectXPage::IDC_SET_POINTS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_main->GetSceneRenderer()->SetPoints();
}

/// <summary>
/// Runs the Hot3dxSceneRenderer InitSphereVB2() function to draw the Latitudinal Sphere object
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
/// <param name="cameraradius"></param>
/// <param name="camerarotation"></param>
void Hot3dxRotoDraw::DirectXPage::SET_SPHERE_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e,
	float cameraradius, float camerarotation)
{
		m_main->GetSceneRenderer()->InitSphereVB2( cameraradius, camerarotation);
		if (m_main->GetSceneRenderer()->GetPointCount() == 0) {
			return;
		}
}

/// <summary>
/// Runs the Hot3dxSceneRenderer InitSphereGeo() function to draw the Geodesic Sphere object
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
/// <param name="diameter"></param>
/// <param name="tesselation"></param>
void  Hot3dxRotoDraw::DirectXPage::SET_SPHERE_GEODESIC_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e,
	float diameter, size_t tesselation)
{
	m_main->GetSceneRenderer()->InitSphereGeo(diameter, tesselation);
}

/// <summary>
/// Runs the Hot3dxSceneRenderer InitLatLongSphere() function to draw the Latitudinal Longitudinal Sphere object
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
/// <param name="diameter"></param>
/// <param name="tesselation"></param>
void  Hot3dxRotoDraw::DirectXPage::SET_SPHERE_LONG_LAT_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e,
	float diameter, size_t tesselation)
{
	m_main->GetSceneRenderer()->InitLatLongSphere(diameter, tesselation);
}

void DirectXPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetLogicalSize(e->NewSize);
	m_main->CreateWindowSizeDependentResources();
}

void Hot3dxRotoDraw::DirectXPage::IDC_WELCOME_STATIC_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
void Hot3dxRotoDraw::DirectXPage::IDC_NEW_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}

void Hot3dxRotoDraw::DirectXPage::IDC_CLEAR_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	AudioStop();
	if (m_main->GetSceneRenderer()->GetPointCount() == 0) { return; }
	m_main->GetSceneRenderer()->ClearDrawnObject();
	AudioInitialize();
}

void Hot3dxRotoDraw::DirectXPage::MyFilePathDXP(Platform::String^ image)
{
		Platform::String^ imageName = ref new Platform::String(image->Data());
		Platform::String^ aPath = ref new Platform::String(L"ms-appx:///Assets/");
		aPath = aPath->Concat(aPath, imageName);

		Uri^ uri = ref new Uri(aPath);
		create_task(StorageFile::GetFileFromApplicationUriAsync(uri)).then([this](StorageFile^ file)
			{
				Platform::String^ s = ref new Platform::String(file->Path->Data());

		this->m_sDirPathDXP = ref new Platform::String();
		int len = s->Length();
		const wchar_t* dir = s->Data();
		unsigned int cnt = len - 18;

		for (unsigned int i = 0; i < cnt; i++)
		{
			if (dir[i] == '\\')
			{
				this->m_sDirPathDXP = this->m_sDirPathDXP->Concat(this->m_sDirPathDXP, dir[i]);
				
			}
			else {
				this->m_sDirPathDXP = this->m_sDirPathDXP->Concat(this->m_sDirPathDXP, dir[i]);
			}
		}

		return file->OpenSequentialReadAsync();
			}).then(
				[this](task<Windows::Storage::Streams::IInputStream^> inputStream)
				{
					try {
				//this->currentChunk = 0;
				//this->ReadBytesOutput->Text = "";

				Windows::Storage::Streams::DataReader^ datareader = ref new Windows::Storage::Streams::DataReader(inputStream.get());

				//this->ReadLoop(dataReader);
			}
			catch (Exception^ e) {
				//this->ReadBytesOuput->Text = e->Message;
			}
				});
}

void Hot3dxRotoDraw::DirectXPage::NotifyUser(Platform::String^ strMessage, NotifyType type)
{
	if (Dispatcher->HasThreadAccess)
	{
		UpdateStatus(strMessage, type);
	}
	else
	{
		Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([strMessage, type, this]()
		{
			UpdateStatus(strMessage, type);
			
		}));
	}
}


void Hot3dxRotoDraw::DirectXPage::ScenarioFrame_Navigated(Platform::Object^ sender, Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{

}

void Hot3dxRotoDraw::DirectXPage::OnTimerTickDXP(Object^ sender, Object^ e)
{
	// Update the sound position on every dispatcher timer tick.
	_startDXP.OnUpdate(_angularVelocityDXP, _heightDXP, _radiusDXP);
}

///////////////////// Textures ///////////////////
void Hot3dxRotoDraw::DirectXPage::TEXTURE_IMAGE1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::DirectXPage::TEXTURE_IMAGE2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
