#pragma once

#pragma once

#include "Generated Files\Scenario13_ScreenGrab.g.h"
#include "DirectXPage.xaml.h"

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
    public ref class Scenario13_ScreenGrab sealed
    {
    public:
        Scenario13_ScreenGrab();
        virtual ~Scenario13_ScreenGrab();
       
    internal:
        static Scenario13_ScreenGrab^ Current;
        Platform::String^ wcfileName;

    private:

        DirectXPage^ _rootPage;
        bool _initialized = false;

        void DisplayResultScreenGrabSave(Windows::UI::Xaml::Controls::Image^ image,
            Windows::UI::Xaml::Controls::TextBox^ textBox,
            Platform::String^ thumbnailModeName, size_t size,
            Windows::Storage::IStorageItem^ item,
            Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, bool isGroup);
       
        void LoadChosenImage1();

        bool m_bscreenGrab;
        Platform::String^ m_dirPath;
        Windows::Storage::StorageFile^ fileImageScreenGrab;
        Windows::Storage::FileProperties::StorageItemThumbnail^ m_thumbnailScreenGrabDXP;
        Platform::String^ fileNameScreenGrab;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapImageScreenGrabDXP;

        void IDC_SCREENGRAB_DDS_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SCREENGRAB_BMP_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SCREENGRAB_PNG_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        //void IDC_SCREENGRAB_ICO_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SCREENGRAB_JPG_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SCREENGRAB_TIFF_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SCREENGRAB_GIF_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        //void IDC_SCREENGRAB_WMP_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;


    private:
        void ScreenGrabButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        
        void ScreenGrabFilesTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void ScreenGrabTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
    };


}


