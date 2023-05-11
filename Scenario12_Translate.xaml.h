//
// Scenario12_Translate.xaml.h
// Declaration of the Scenario12_Translate class
//

#pragma once

#include "Scenario12_Translate.g.h"
#include "DirectXPage.xaml.h"

using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace Hot3dxRotoDraw
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Scenario12_Translate sealed
    {
    public:
        Scenario12_Translate();
        virtual ~Scenario12_Translate();

    private:

        DirectXPage^ _rootPage;
        bool                            _initialized = false;

        void IDC_SetTranslate_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SetTranslate_BUTTON2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void IDC_X_TRANSLATE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void IDC_Y_TRANSLATE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void IDC_Z_TRANSLATE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void IDC_X_TRANSLATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_Y_TRANSLATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_Z_TRANSLATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SetTranslate_BUTTON3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}
