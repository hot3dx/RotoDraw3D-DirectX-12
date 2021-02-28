﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class SwapChainPanel;
                ref class StackPanel;
                ref class SplitView;
                ref class Image;
                ref class TextBlock;
                ref class FontIcon;
                ref class ListBox;
                ref class HyperlinkButton;
                ref class Frame;
                ref class Border;
            }
        }
    }
}

namespace Hot3dxRotoDraw
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class DirectXPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        void UnloadObject(::Windows::UI::Xaml::DependencyObject^ dependencyObject);
        void DisconnectUnloadedObject(int connectionId);
    
    private:
        bool _contentLoaded;
    
        class DirectXPage_obj17_Bindings;
        class DirectXPage_obj1_Bindings;
    
        private: ::Windows::UI::Xaml::Controls::SwapChainPanel^ swapChainPanel;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ HeaderPanel;
        private: ::Windows::UI::Xaml::Controls::SplitView^ Splitter;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ HeaderPanel1;
        private: ::Windows::UI::Xaml::Controls::Image^ Hot3dxLogo;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ Header;
        private: ::Windows::UI::Xaml::Controls::FontIcon^ Hamburger;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ Hot3dxRotoDraw3DX12;
        private: ::Windows::UI::Xaml::Controls::ListBox^ ScenarioControl;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ FooterPanel;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ Copyright;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ LinksPanel;
        private: ::Windows::UI::Xaml::Controls::HyperlinkButton^ PrivacyLink;
        private: ::Windows::UI::Xaml::Controls::Frame^ ScenarioFrame;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ StatusPanel;
        private: ::Windows::UI::Xaml::Controls::Border^ StatusBorder;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ StatusBlock;
    };
}
