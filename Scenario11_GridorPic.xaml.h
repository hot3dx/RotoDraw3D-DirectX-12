#pragma once

#include "Generated Files\Scenario11_GridorPic.g.h"
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
    public ref class Scenario11_GridorPic sealed
    {
    public:
        Scenario11_GridorPic();
        virtual ~Scenario11_GridorPic();
        
    internal:
        static Scenario11_GridorPic^ Current;
        Platform::String^ wcfileName;

    private:

        DirectXPage^ _rootPage;
        bool _initialized = false;

        Platform::String^ m_dirPath;
        Windows::Storage::StorageFile^ fileImage;
        Windows::Storage::FileProperties::StorageItemThumbnail^ m_thumbnail;
        Platform::String^ fileName;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapImage;

     protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        
     };

    
}

