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

#include "Scenario1_Start.g.h"
#include "DirectXPage.xaml.h"
#include "Content\Hot3dxRotoDrawVariables.h"
#include "ContentDialog1.xaml.h"

using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Microsoft::WRL;
using namespace Windows::Media::Devices;
using namespace Windows::Storage::Streams;

namespace Hot3dxRotoDraw
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Scenario1_Start sealed
    {
    public:
        Scenario1_Start();
        virtual ~Scenario1_Start();
    
        void StopButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void OnTimerTick(Object^ sender, Object^ e);
        void RadiusSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void EnvironmentComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
        void FullViewButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void PlayButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        //void AngularVelocitySlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void RotationSpeedSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void HeightSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);

    private:
        
        DirectXPage^ _rootPage;
        Windows::UI::Core::CoreDispatcher^ m_CoreDispatcher;
        Windows::Media::SystemMediaTransportControls^ m_SystemMediaControls;
        Windows::Foundation::EventRegistrationToken  m_deviceStateChangeToken;
        Windows::Foundation::EventRegistrationToken  m_SystemMediaControlsButtonToken;

        Platform::Boolean    m_IsMFLoaded;
        Windows::Storage::Streams::IRandomAccessStream^ m_ContentStream;
        //ContentType    m_ContentType;
        //DeviceStateChangedEvent^ m_StateChangedEvent;
       // ComPtr<WASAPIRenderer>  m_spRenderer;
        //Platform::Boolean    m_deviceSupportsRawMode;
        static void DisplayResult(Windows::UI::Xaml::Controls::Image^ image, Windows::UI::Xaml::Controls::TextBox^ textBox,
            Platform::String^ thumbnailModeName,
            Windows::Storage::IStorageItem^ item, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail,
            bool isGroup);

        float _height;
        void LeftMenusButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void RightMenusButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void textBlock2_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void LoadButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        Platform::String^ m_soundPath; 
        void LoadDefaultSound1(Platform::String^ soundPath);
        void LoadChosenSound1();
        

        void UpdateContentProps(Platform::String^ str);
    };
}

