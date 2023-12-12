#include "pch.h"
#include "Scenario11_GridorPic.xaml.h"

using namespace Hot3dxRotoDraw;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace concurrency;



Scenario11_GridorPic^ Scenario11_GridorPic::Current = nullptr;


Hot3dxRotoDraw::Scenario11_GridorPic::Scenario11_GridorPic() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();
	Scenario11Vars^ vars = _rootPage->m_Scene11Vars;
	

	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);

	Scenario11_GridorPic::Current = this;
	_rootPage->m_Scene11Vars->SetScenario11Page(this);

	
}

Hot3dxRotoDraw::Scenario11_GridorPic::~Scenario11_GridorPic()
{
	
}

void Hot3dxRotoDraw::Scenario11_GridorPic::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	
}
