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
    /*
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
    */
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
        

        void SetMaterialTypesStrings();
        void SetSingleMaterialTypesStringListboxItem(Platform::Object^ sender, unsigned int i,
            MaterialList const& s, MaterialTypes m,
            Windows::UI::Xaml::Controls::TextBlock^ textBlock);
        void SetMaterialTypesDataStrings();


        void MaterialListControl_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
        void MaterialListControlData_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
        void MySc5FilePath(Platform::String^ image){ m_sSc5DirPath = image; }
        Platform::String^ m_sSc5DirPath;

        bool m_firstOrSecondImage;

        
        
    internal:
        static Scenario5_MatsTexs^ Current;

        
    private:

        
       
    };

}