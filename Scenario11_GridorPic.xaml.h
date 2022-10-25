#pragma once

#include "Generated Files\Scenario11_GridorPic.g.h"
#include "DirectXPage.xaml.h"
#include "Hot3dxRotoDrawConfiguration.h"
#include "Content\Hot3dxRotoDrawVariables.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Storage;

namespace Hot3dxRotoDraw
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Scenario11_GridorPic sealed
    {
    public:
        Scenario11_GridorPic();
        virtual ~Scenario11_GridorPic();

    internal:
        static Scenario11_GridorPic^ Current;
        Platform::String^ wcfileName;

    private:

        Hot3dxRotoDraw::DirectXPage^ _rootPage;
        bool _initialized = false;

        void IDC_GRID_OR_PIC_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void GridorPicTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        void GridorPicFilesTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void DisplayResultGridPic(Windows::UI::Xaml::Controls::Image^ image,
            Windows::UI::Xaml::Controls::TextBox^ textBox,
            Platform::String^ thumbnailModeName, size_t size,
            Windows::Storage::IStorageItem^ item,
            Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, bool isGroup);
        void IDC_PIC_TEXTURE_IMAGE1_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SHOW_BACKGROUND_PIC_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        
        void LoadChosenImage1();

        bool m_GridOrPicImage;
        Windows::Storage::StorageFile^ fileImageGridPic;
        Windows::Storage::FileProperties::StorageItemThumbnail^ m_thumbnailGridPicDXP;
        Platform::String^ fileNameGridPic;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapImageGridPicDXP;

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

     };

    
}

