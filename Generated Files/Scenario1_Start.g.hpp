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

#include "Scenario1_Start.xaml.h"

void ::Hot3dxRotoDraw::Scenario1_Start::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///Scenario1_Start.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::Hot3dxRotoDraw::Scenario1_Start::Connect(int __connectionId, ::Platform::Object^ __target)
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
            this->ErrorBorder = safe_cast<::Windows::UI::Xaml::Controls::Border^>(__target);
        }
        break;
    case 5:
        {
            this->StatusBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 6:
        {
            this->text6Block1 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 7:
        {
            this->text6Block2 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 8:
        {
            this->text6Block3 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 9:
        {
            this->SwitchMenuSidesTextBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 10:
        {
            this->textBlock2 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 11:
        {
            ::Windows::UI::Xaml::Controls::Button^ element11 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element11))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario1_Start::LeftMenusButton_Click);
        }
        break;
    case 12:
        {
            this->RotationSpeedSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->RotationSpeedSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario1_Start::RotationSpeedSlider_ValueChanged);
        }
        break;
    case 13:
        {
            ::Windows::UI::Xaml::Controls::Button^ element13 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element13))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario1_Start::RightMenusButton_Click);
        }
        break;
    case 14:
        {
            this->textBlock1 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 15:
        {
            this->EnvironmentComboBox = safe_cast<::Windows::UI::Xaml::Controls::ComboBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::ComboBox^>(this->EnvironmentComboBox))->SelectionChanged += ref new ::Windows::UI::Xaml::Controls::SelectionChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::SelectionChangedEventArgs^))&Scenario1_Start::EnvironmentComboBox_SelectionChanged);
        }
        break;
    case 16:
        {
            ::Windows::UI::Xaml::Controls::Button^ element16 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element16))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario1_Start::MenuViewButton_Click);
        }
        break;
    case 17:
        {
            ::Windows::UI::Xaml::Controls::Button^ element17 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element17))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario1_Start::FullViewButton_Click);
        }
        break;
    case 18:
        {
            this->textBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 19:
        {
            this->RadiusSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->RadiusSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario1_Start::RadiusSlider_ValueChanged);
        }
        break;
    case 20:
        {
            this->textBlock3 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 21:
        {
            this->HeightSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->HeightSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&Scenario1_Start::HeightSlider_ValueChanged);
        }
        break;
    case 22:
        {
            ::Windows::UI::Xaml::Controls::Button^ element22 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element22))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario1_Start::PlayButton_Click);
        }
        break;
    case 23:
        {
            ::Windows::UI::Xaml::Controls::Button^ element23 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element23))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario1_Start::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario1_Start::StopButton_Click);
        }
        break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::Hot3dxRotoDraw::Scenario1_Start::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}

#pragma warning(pop)


