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

#include "DirectXPage.xaml.h"

void ::Hot3dxRotoDraw::DirectXPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///DirectXPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::Hot3dxRotoDraw::DirectXPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
    case 2:
        {
            this->swapChainPanel = safe_cast<::Windows::UI::Xaml::Controls::SwapChainPanel^>(__target);
        }
        break;
    case 3:
        {
            this->HeaderPanel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
        }
        break;
    case 4:
        {
            this->Splitter = safe_cast<::Windows::UI::Xaml::Controls::SplitView^>(__target);
        }
        break;
    case 5:
        {
            this->HeaderPanel1 = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
        }
        break;
    case 6:
        {
            this->Hot3dxLogo = safe_cast<::Windows::UI::Xaml::Controls::Image^>(__target);
        }
        break;
    case 7:
        {
            this->Header = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 8:
        {
            ::Windows::UI::Xaml::Controls::Primitives::ToggleButton^ element8 = safe_cast<::Windows::UI::Xaml::Controls::Primitives::ToggleButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Primitives::ToggleButton^>(element8))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::Button_Click);
        }
        break;
    case 9:
        {
            this->Hamburger = safe_cast<::Windows::UI::Xaml::Controls::FontIcon^>(__target);
        }
        break;
    case 10:
        {
            this->Kubed3DHot3dxMaker3DX12 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 11:
        {
            this->ScenarioControl = safe_cast<::Windows::UI::Xaml::Controls::ListBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::ListBox^>(this->ScenarioControl))->SelectionChanged += ref new ::Windows::UI::Xaml::Controls::SelectionChangedEventHandler(this, (void (::Hot3dxRotoDraw::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::SelectionChangedEventArgs^))&DirectXPage::ScenarioControl_SelectionChanged);
        }
        break;
    case 12:
        {
            this->FooterPanel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
        }
        break;
    case 13:
        {
            this->Copyright = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 14:
        {
            this->LinksPanel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
        }
        break;
    case 15:
        {
            ::Windows::UI::Xaml::Controls::HyperlinkButton^ element15 = safe_cast<::Windows::UI::Xaml::Controls::HyperlinkButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::HyperlinkButton^>(element15))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::Footer_Click);
        }
        break;
    case 16:
        {
            this->PrivacyLink = safe_cast<::Windows::UI::Xaml::Controls::HyperlinkButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::HyperlinkButton^>(this->PrivacyLink))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::Footer_Click);
        }
        break;
    case 18:
        {
            this->ScenarioFrame = safe_cast<::Windows::UI::Xaml::Controls::Frame^>(__target);
        }
        break;
    case 19:
        {
            this->StatusPanel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
        }
        break;
    case 20:
        {
            this->StatusBorder = safe_cast<::Windows::UI::Xaml::Controls::Border^>(__target);
        }
        break;
    case 21:
        {
            this->StatusBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::Hot3dxRotoDraw::DirectXPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}

#pragma warning(pop)


