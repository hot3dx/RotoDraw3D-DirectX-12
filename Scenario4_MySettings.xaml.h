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


#pragma once

#include "Scenario4_MySettings.g.h"
#include "DirectXPage.xaml.h"


using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace Hot3dxRotoDraw
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Scenario4_MySettings sealed
    {
    public:
        Scenario4_MySettings();
        virtual ~Scenario4_MySettings();

    private:

        Hot3dxRotoDraw::DirectXPage^ _rootPage;
        bool                            _initialized = false;

        void IDC_SetScale_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_ALL_SCALE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e); 
        void IDC_X_SCALE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void IDC_Y_SCALE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void IDC_Z_SCALE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void IDC_ALL_SCALE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_X_SCALE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_Y_SCALE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_Z_SCALE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}


