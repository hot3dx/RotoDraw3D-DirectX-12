﻿//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BINDING_DEBUG_OUTPUT
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

#include "Scenario10_Sculpt.xaml.h"

void ::Hot3dxRotoDraw::Scenario10_Sculpt::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///Scenario10_Sculpt.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::Hot3dxRotoDraw::Scenario10_Sculpt::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
    case 2:
        {
            this->RootGrid = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(__target);
        }
        break;
    case 3:
        {
            this->HeaderPanel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
        }
        break;
    case 4:
        {
            this->m_sculptText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 5:
        {
            this->checkBoxSculptBool = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(this->checkBoxSculptBool))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario10_Sculpt::checkBoxSculptBool_Checked);
        }
        break;
    case 6:
        {
            this->m_sculptMovementLock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 7:
        {
            this->checkBoxSculptXBool = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(this->checkBoxSculptXBool))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario10_Sculpt::checkBoxSculptXBool_Checked);
        }
        break;
    case 8:
        {
            this->checkBoxSculptYBool = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(this->checkBoxSculptYBool))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario10_Sculpt::checkBoxSculptYBool_Checked);
        }
        break;
    case 9:
        {
            this->checkBoxSculptZBool = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(this->checkBoxSculptZBool))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario10_Sculpt::checkBoxSculptZBool_Checked);
        }
        break;
    case 10:
        {
            this->m_sculptObjectData = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 11:
        {
            this->textBoxGroupCount = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->textBoxGroupCount))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::textBoxGroupCount_TextChanged);
        }
        break;
    case 12:
        {
            this->textBoxPointsPerGroup = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->textBoxPointsPerGroup))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::textBoxPointsPerGroup_TextChanged);
        }
        break;
    case 13:
        {
            this->textBoxPointCount = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->textBoxPointCount))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::textBoxPointCount_TextChanged);
        }
        break;
    case 14:
        {
            this->m_selectedSculptObjectData = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 15:
        {
            this->textBoxGroupSelected = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->textBoxGroupSelected))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::textBoxSelectedGroup_TextChanged);
        }
        break;
    case 16:
        {
            this->textBoxPointSelected = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->textBoxPointSelected))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::textBoxSelectedPoint_TextChanged);
        }
        break;
    case 17:
        {
            this->textBoxSelectedPointCount = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->textBoxSelectedPointCount))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::textBoxSelectedPointCount_TextChanged);
        }
        break;
    case 18:
        {
            this->textBoxSelectedPointX = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->textBoxSelectedPointX))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::textBoxXPointValue_TextChanged);
        }
        break;
    case 19:
        {
            this->textBoxSelectedPointY = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->textBoxSelectedPointY))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::textBoxYPointValue_TextChanged);
        }
        break;
    case 20:
        {
            this->textBoxSelectedPointZ = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->textBoxSelectedPointZ))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::textBoxZPointValue_TextChanged);
        }
        break;
    case 21:
        {
            this->m_selectedGroup = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 22:
        {
            this->slider_SelectedGroup = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->slider_SelectedGroup))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario10_Sculpt::SelectedGroupSlider_ValueChanged);
        }
        break;
    case 23:
        {
            this->m_selectedPoint = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 24:
        {
            this->slider_SelectedPoint = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->slider_SelectedPoint))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario10_Sculpt::SelectedPointSlider_ValueChanged);
        }
        break;
    case 25:
        {
            this->RotateXTextBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 26:
        {
            this->GRIDCAM_XROTATE_EDIT = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->GRIDCAM_XROTATE_EDIT))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::GRIDCAM_X_ROTATE_EDIT_TextChanged);
        }
        break;
    case 27:
        {
            this->slider_RotateXGridCam = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->slider_RotateXGridCam))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario10_Sculpt::ROTATE_X_GridCam_ValueChanged);
        }
        break;
    case 28:
        {
            this->RotateYTextBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 29:
        {
            this->GRIDCAM_YROTATE_EDIT = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->GRIDCAM_YROTATE_EDIT))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::GRIDCAM_Y_ROTATE_EDIT_TextChanged);
        }
        break;
    case 30:
        {
            this->slider_RotateYGridCam = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->slider_RotateYGridCam))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario10_Sculpt::ROTATE_Y_GridCam_ValueChanged);
        }
        break;
    case 31:
        {
            this->RotateZTextBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 32:
        {
            this->GRIDCAM_ZROTATE_EDIT = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->GRIDCAM_ZROTATE_EDIT))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::GRIDCAM_Z_ROTATE_EDIT_TextChanged);
        }
        break;
    case 33:
        {
            this->slider_RotateZGridCam = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->slider_RotateZGridCam))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario10_Sculpt::ROTATE_Z_GridCam_ValueChanged);
        }
        break;
    case 34:
        {
            this->m_sculptTextBox = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->m_sculptTextBox))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::SculptTextBox_TextChanged);
        }
        break;
    case 35:
        {
            this->m_sculptFilesText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 36:
        {
            this->m_sculptFilesTextBox = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->m_sculptFilesTextBox))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario10_Sculpt::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario10_Sculpt::SculptFilesTextBox_TextChanged);
        }
        break;
    case 37:
        {
            this->IDC_DESCRIPTION_STATIC = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::Hot3dxRotoDraw::Scenario10_Sculpt::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}

#pragma warning(pop)


