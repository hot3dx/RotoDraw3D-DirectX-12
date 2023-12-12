//*********************************************************
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************


#include "pch.h"
#include "Scenario7_SphereDraw.xaml.h"

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

Scenario7_SphereDraw^ Scenario7_SphereDraw::Current = nullptr;

Hot3dxRotoDraw::Scenario7_SphereDraw::Scenario7_SphereDraw() : _rootPage(DirectXPage::Current)
{
    InitializeComponent();
    
	Scenario7Vars^ vars = _rootPage->m_Scene7Vars;
	_rootPage->m_Scene7Vars->SetScenario7Page(this);

				
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);


	
		Scenario7_SphereDraw::Current = this;
	_rootPage->m_Scene7Vars->SetScenario7Page(this);
}

Hot3dxRotoDraw::Scenario7_SphereDraw::~Scenario7_SphereDraw()
{
    
}

/*
void Hot3dxRotoDraw::Scenario7_SphereDraw::SourcePositionX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    _x = static_cast<float>(e->NewValue);
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::SourcePositionY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    _y = static_cast<float>(e->NewValue);
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::SourcePositionZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    _z = static_cast<float>(e->NewValue);
}
*/
/////////////////
void Hot3dxRotoDraw::Scenario7_SphereDraw::SphereRadiusTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario7_SphereDraw::PointSpacingTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario7_SphereDraw::DrawSphereButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (_rootPage->GetPointCount() == 0)
    {
        Platform::String^ str;

        str = Hot3dxRotoDraw::Scenario7_SphereDraw::m_SphereRadiusTextBox->Text;
        m_cameraradius = std::wcstof(str->Data(), nullptr);

        str = Hot3dxRotoDraw::Scenario7_SphereDraw::m_PointSpaceTextBox->Text;
        m_camerarotation = std::wcstof(str->Data(), nullptr);

        _rootPage->SET_SPHERE_BUTTON_Click(sender, e, m_cameraradius, m_camerarotation);
        _rootPage->NotifyUser(L"Sphere Drawn", NotifyType::StatusMessage);
    }
    else
    {
        _rootPage->NotifyUser(L"Go To Draw Page and Click the Clear Button before drawing another sphere", NotifyType::StatusMessage);
    }
}


void Hot3dxRotoDraw::Scenario7_SphereDraw::Back_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->Button_Click(sender, e);
}


void Hot3dxRotoDraw::Scenario7_SphereDraw::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	Scenario7Vars^ vars = _rootPage->m_Scene7Vars;
	
    Scenario7_SphereDraw::Current = this;
	_rootPage->m_Scene7Vars->SetScenario7Page(this);
}

