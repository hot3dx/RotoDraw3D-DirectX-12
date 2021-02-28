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

#include "Scenario6_ColorPicker.g.h"
#include "DirectXPage.xaml.h"
#include "Hot3dxRotoDrawConfiguration.h"
#include <Generated Files\Scenario6_ColorPicker.g.h>

using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;


namespace Hot3dxRotoDraw
{
   
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Scenario6_ColorPicker sealed
    {
    public:
        Scenario6_ColorPicker();
        virtual ~Scenario6_ColorPicker();
    
    private:

        DirectXPage^ _rootPage;
        bool                            _initialized = false;

        void ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
        void ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
        
        void frontColor_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void backColor_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        
       
    };
}


