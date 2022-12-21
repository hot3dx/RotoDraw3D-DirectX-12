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

#include "Generated Files\Scenario2_Normal.g.h"
#include "DirectXPage.xaml.h"
#include "CardioidSound.h"
#include "Common\DeviceResources.h"
#include "Hot3dxRotoDrawMain.h"
#include "Hot3dxRotoDrawConfiguration.h"

using namespace Windows::UI::Xaml;

typedef enum class PlayState { Stopped = 0, Playing } PlayState;

namespace Hot3dxRotoDraw
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Scenario2_Normal sealed
    {
    public:
        Scenario2_Normal();
        virtual ~Scenario2_Normal();

       
        Windows::UI::Xaml::Controls::TextBox^ GetPointSpaceTextBox() { return m_PointSpaceTextBox; }

        void SetTopLeftCheckBoxFalse() { IDC_TOP_OR_LEFT_CHECKBOX->IsChecked = false; }
        void SetBottomRightCheckBoxFalse() { IDC_BOTTOM_OR_RIGHT_CHECKBOX->IsChecked = false; }

        void SetPartialSlider();
       
    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
       
    private:
        
        void IDC_CLEAR_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SET_COLORS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_COPY_FLIP_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        // CheckBoxes
        void IDC_EXTERIOR_FACES_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_INTERIOR_FACES_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_FIRST_TO_LAST_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_AXIS_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_TOP_OR_LEFT_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_BOTTOM_OR_RIGHT_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        // TextBoxes
        VOID IDC_ROTATION_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        // Point Spacing Slider
        void IDC_SLIDER_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void PointSpacingTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        // Sets Partial Slider for objects that are not 360 degrees
        
        // Partial Rotation Slider
        void IDC_PARTIAL_ROTATE_SLIDER_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void PartialRotateTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void IDC_CLOSED_OR_OPEN_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void IDC_SET_POINTS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        
        Windows::UI::Xaml::Controls::CheckBox^ m_IDC_EXTERIOR_FACES_CHECKBOX;
        Windows::UI::Xaml::Controls::CheckBox^ m_IDC_INTERIOR_FACES_CHECKBOX;
        bool m_bDoFrontFaces;
        bool m_bDoBackFaces;
        bool m_bFirstToLast;
        bool m_bAxisChecked;
        bool m_bTopOrLeftChecked;
        bool m_bBottomOrRightChecked;
        Windows::UI::Color m_pointColor;
        DirectXPage^ _rootPage;
        Windows::UI::Xaml::Controls::TextBox^ m_PointSpaceTextBox;
        int m_iPointCount;
        unsigned int m_fPointGroupsCount; // Number of Slices or pointGroups
        float DenomOf360;
       
    internal:
        static Scenario2_Normal^ Current;
        void RasterDescComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void SamplIndexWrapComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void EffectDescComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
        void SetEffectDescComboBox(unsigned int val);

       unsigned int m_iRasterDescIndex;
       unsigned int m_iSamplWrapIndex;
       unsigned int m_iEffectDescSelectedIndex;
    private:
       
        
       
    };
}

