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
using namespace Windows::UI::Xaml;
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


Hot3dxRotoDraw::Scenario7_SphereDraw::Scenario7_SphereDraw() : _rootPage(DirectXPage::Current)
{
    InitializeComponent();
    
    _rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
    
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

    Platform::String^ str;
    
    str = Hot3dxRotoDraw::Scenario7_SphereDraw::m_SphereRadiusTextBox->Text;
    m_cameraradius = std::wcstof(str->Data(), nullptr);

    str = Hot3dxRotoDraw::Scenario7_SphereDraw::m_PointSpaceTextBox->Text;
    m_camerarotation = std::wcstof(str->Data(), nullptr);
}


void Hot3dxRotoDraw::Scenario7_SphereDraw::Back_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->Toggle_Button_Click(sender, e);
}

