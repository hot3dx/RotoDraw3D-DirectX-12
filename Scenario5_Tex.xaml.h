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

#include "Generated Files\Scenario5_Tex.g.h"
#include "DirectXPage.xaml.h"

#include "Content\Hot3dxRotoDrawVariables.h"
#include "ContentDialog1.xaml.h"


using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace Hot3dxRotoDraw
{

    class Errors
    {
    public:
        static Platform::String^ NoExifThumbnail;
        static Platform::String^ NoThumbnail;
        static Platform::String^ NoAlbumArt;
        static Platform::String^ NoIcon;
        static Platform::String^ NoImages;
        static Platform::String^ FileGroupEmpty;
        static Platform::String^ FileGroupLocation;
        static Platform::String^ Cancel;
        static Platform::String^ InvalidSize;
    };

    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
        public ref class Scenario5_Tex sealed
    {


    public:
        Scenario5_Tex();
        virtual ~Scenario5_Tex();

        void SetIDC_GRID_OR_PIC_CHECKED(bool b) {
            if (this)
            {
                IDC_GRID_OR_PIC_CHECKBOX->IsChecked = b;

            }

        }
        void SetIDC_PIC_CHECKED(bool b) {
            if (this)
            {
                IDC_PIC_CHECKBOX->IsChecked = b;

            }
        }

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:

        DirectXPage^ _rootPage;
        bool                            _initialized = false;

        void scrollBar_Scroll(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::ScrollEventArgs^ e);

        void LoadDefaultImage1(Platform::String^ imagePath);
        void LoadChosenImage1();
        void LoadDefaultImage2(Platform::String^ imagePath);
        void LoadChosenImage2();
        void LoadDefaultImage3(Platform::String^ imagePath);
        void LoadChosenImage3();
        void LoadDefaultImage4(Platform::String^ imagePath);
        void LoadChosenImage4();
        void LoadDefaultImage5(Platform::String^ imagePath);
        void LoadChosenImage5();
        void LoadDefaultImage6(Platform::String^ imagePath);
        void LoadChosenImage6();
        void LoadDefaultImagePic(Platform::String^ imagePath);
        void LoadChosenImagePic();
        void LoadDefaultImageVideo(Platform::String^ imagePath);

        void LoadDefaultImageUri1(Platform::String^ imagePath);
        void LoadDefaultImageUri2(Platform::String^ imagePath);
        void LoadDefaultImageUri3(Platform::String^ imagePath);
        void LoadDefaultImageUri4(Platform::String^ imagePath);
        void LoadDefaultImageUri5(Platform::String^ imagePath);
        void LoadDefaultImageUri6(Platform::String^ imagePath);
        void LoadDefaultImageUriPic(Platform::String^ imagePath);
        void LoadDefaultImageUriVideo(Platform::String^ imagePath);

        void LoadChosenImageVideo();
        void LoadThumbnail(Platform::String^ filePath, Platform::String^ imageCtrlName);
       
        unsigned int imageIdNums;
       
        static void DisplayResult(Windows::UI::Xaml::Controls::Image^ image, Windows::UI::Xaml::Controls::TextBlock^ textBlock,
            Platform::String^ thumbnailModeName,
            Windows::Storage::IStorageItem^ item, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail,
            bool isGroup);
        void DisplayResultVideo(Windows::UI::Xaml::Controls::Image^ image,
            Windows::UI::Xaml::Controls::TextBlock^ textBlock,
            Platform::String^ thumbnailModeName,
            Windows::Storage::IStorageItem^ item,
            Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail,
            bool isGroup);

        void MySc5FilePath(Platform::String^ image) { m_sSc5DirPath = image; }
        Platform::String^ m_sSc5DirPath;
        Platform::String^ m_sPicDirPath;
        Platform::String^ m_sVideoDirPath;
        Platform::String^ m_sDefaultLoadFolderPath;
        bool m_firstOrSecondImage;

    internal:
        static Scenario5_Tex^ Current;


    private:
        void TextureImage1_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage2_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage3_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage4_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage5_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage6_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void PicTextureImage2_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void VideoTextureImage2_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void filePath1TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void filePath2TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void filePath3TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void filePath4TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void filePath5TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void filePath6TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void filePathPic1TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void filePathVideo2TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void IDC_D3DMATERIAL_DISPLAY_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);

        void TextureImage1_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void TextureImage2_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void TextureRMAImage3_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void TextureEMMisiveImage4_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void TextureRadianceImage5_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void TextureIrradianceImage6_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void PicTextureImage2_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void VideoTextureImage2_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);

        inline void SetDefaultFolderPath();

        Platform::String^ GetImagePath(Platform::String^ imagePath);
        Windows::Foundation::Uri^ GetAppImageUriFromString(Platform::String^ imagePath);
        void TextureImage1_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);
        void TextureImage2_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);
        void TextureRMAImage3_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);
        void TextureNormalImage4_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);
        void TextureRadianceImage5_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);
        void TextureIrradianceImage6_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);
        void PicTextureImage2_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);
        void VideoTextureImage2_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);

        void IDC_D3DMATERIAL_DISPLAY_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_PIC_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_GRID_OR_PIC_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SHOW_BKGRND_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void SetTexture1Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail,  Platform::String^ thumbnailModeName, Platform::String^ message);
        void SetTexture2Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message);
        void SetTexture3Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message);
        void SetTexture4Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message);
        void SetTexture5Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message);
        void SetTexture6Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message);
        void SetTexturePicParameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message);
        void SetTextureVideoParameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message);

     };
}