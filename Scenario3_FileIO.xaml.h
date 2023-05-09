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

#include "Scenario3_FileIO.g.h"
#include "DirectXPage.xaml.h"


using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace Hot3dxRotoDraw
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Scenario3_FileIO sealed
    {
    public:
        Scenario3_FileIO();
        virtual ~Scenario3_FileIO();
    private:
        
        Hot3dxRotoDraw::DirectXPage^ _rootPage;
        bool                            _initialized = false;
        Windows::Storage::StorageFile^ mtlFilename;
        
        void New_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Open_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        Windows::Storage::Streams::IBuffer^ GetBufferFromString(Platform::String^ str);
        void Save_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Save_Obj_Material_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void SaveAs_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void SaveAll_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Save_Texture_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        
        void Save_Material_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}

