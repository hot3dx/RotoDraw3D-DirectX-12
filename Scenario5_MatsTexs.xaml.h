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

#include "Generated Files\Scenario5_MatsTexs.g.h"
#include "DirectXPage.xaml.h"
#include "Scenario5_MatsTexsConfig.h"
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
    public ref class Scenario5_MatsTexs sealed
    {
       

    public:
        Scenario5_MatsTexs();
        virtual ~Scenario5_MatsTexs();

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:

        DirectXPage^ _rootPage;
        bool                            _initialized = false;

        void scrollBar_Scroll(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::ScrollEventArgs^ e);
        void IDC_TEXTURE_IMAGE1_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_TEXTURE_IMAGE2_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_TEXTURE_RMA_IMAGE3_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_TEXTURE_EMMISIVE_IMAGE4_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_TEXTURE_RADIANCE_IMAGE5_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_TEXTURE_IRRADIANCE_IMAGE6_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

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
        void LoadDefaultImages(unsigned int imageIdNum, 
            Platform::String^ imagePath
        );
        unsigned int imageIdNums;
        void LoadChosenImages();

        void Hot3dxRotoDraw::Scenario5_MatsTexs::SetMaterialTypesStrings();
        void SetSingleMaterialTypesStringListboxItem(Platform::Object^ sender, unsigned int i,
            MaterialList const& s, MaterialTypes m,
            Windows::UI::Xaml::Controls::TextBlock^ textBlock);
        void SetMaterialTypesDataStrings();


        static void DisplayResult(Windows::UI::Xaml::Controls::Image^ image, Windows::UI::Xaml::Controls::TextBlock^ textBlock,
            Platform::String^ thumbnailModeName, size_t size,
            Windows::Storage::IStorageItem^ item, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail,
            bool isGroup);

        void MaterialListControl_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
        void MaterialListControlData_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
        void MySc5FilePath(Platform::String^ image);
        Platform::String^ m_sSc5DirPath;

        bool m_firstOrSecondImage;

    internal:
        static Scenario5_MatsTexs^ Current;

        
    private:
        void TextureImage1_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage2_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage3_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage4_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage5_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void TextureImage6_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };

}