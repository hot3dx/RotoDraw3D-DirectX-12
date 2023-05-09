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

#include "Scenario7_SphereDraw.g.h"
#include "DirectXPage.xaml.h"



using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace Hot3dxRotoDraw
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Scenario7_SphereDraw sealed
    {
    public:
        Scenario7_SphereDraw();
        virtual ~Scenario7_SphereDraw();

        Windows::UI::Xaml::Controls::TextBox^ GetSphereRadiusTextBox() { return m_SphereRadiusTextBox; }
        Windows::UI::Xaml::Controls::TextBox^ GetPointSpaceTextBox() { return m_PointSpaceTextBox; }

    internal:
        static Scenario7_SphereDraw^ Current;
        Platform::String^ wcfileName;

    private:

        DirectXPage^ _rootPage;
        bool                            _initialized = false;


        void SphereRadiusTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        void PointSpacingTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        void DrawSphereButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void Back_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        float m_cameraradius;
        float m_camerarotation;
        float m_anglerotation;
        int m_iPointCount;

        ///////////////  video

        void IDC_VIDEO_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void VideoTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        void VideoFilesTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void DisplayResultVideo(Windows::UI::Xaml::Controls::Image^ image,
            Windows::UI::Xaml::Controls::TextBox^ textBox,
            Platform::String^ thumbnailModeName, size_t size,
            Windows::Storage::IStorageItem^ item,
            Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, bool isGroup);
        void IDC_VIDEO_TEXTURE_IMAGE1_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SHOW_VIDEO_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void LoadChosenImage1();
        void LoadDefaultVideoImage(Platform::String^ imagePath);

        bool m_VideoImage;
        Platform::String^ m_dirPath;
        Windows::Storage::StorageFile^ fileImageVideo;
        Windows::Storage::FileProperties::StorageItemThumbnail^ m_thumbnailVideoDXP;
        Platform::String^ fileNameVideo;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapImageVideoDXP;

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    };
}


