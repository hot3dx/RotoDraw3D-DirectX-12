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

#include "Scenario9_Rotate.xaml.h"

void ::Hot3dxRotoDraw::Scenario9_Rotate::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///Scenario9_Rotate.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::Hot3dxRotoDraw::Scenario9_Rotate::Connect(int __connectionId, ::Platform::Object^ __target)
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
            this->panel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
        }
        break;
    case 4:
        {
            this->IDC_DESCRIPTION4_STATIC = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 5:
        {
            this->IDC_XROTATE_STATIC = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 6:
        {
            this->IDC_XROTATE_EDIT = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->IDC_XROTATE_EDIT))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario9_Rotate::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario9_Rotate::IDC_X_ROTATE_EDIT_TextChanged);
        }
        break;
    case 7:
        {
            this->XRotateSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->XRotateSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario9_Rotate::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario9_Rotate::IDC_X_ROTATE_ValueChanged);
        }
        break;
    case 8:
        {
            this->IDC_YROTATE_STATIC = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 9:
        {
            this->IDC_YROTATE_EDIT = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->IDC_YROTATE_EDIT))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario9_Rotate::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario9_Rotate::IDC_Y_ROTATE_EDIT_TextChanged);
        }
        break;
    case 10:
        {
            this->YRotateSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->YRotateSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario9_Rotate::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario9_Rotate::IDC_Y_ROTATE_ValueChanged);
        }
        break;
    case 11:
        {
            this->IDC_ZROTATE_STATIC = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 12:
        {
            this->IDC_ZROTATE_EDIT = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->IDC_ZROTATE_EDIT))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario9_Rotate::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario9_Rotate::IDC_Z_ROTATE_EDIT_TextChanged);
        }
        break;
    case 13:
        {
            this->ZRotateSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->ZRotateSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario9_Rotate::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario9_Rotate::IDC_Z_ROTATE_ValueChanged);
        }
        break;
    case 14:
        {
            this->IDC_Settings_BUTTON1 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->IDC_Settings_BUTTON1))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario9_Rotate::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario9_Rotate::IDC_SetRotate_BUTTON_Click);
        }
        break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::Hot3dxRotoDraw::Scenario9_Rotate::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}

#pragma warning(pop)


