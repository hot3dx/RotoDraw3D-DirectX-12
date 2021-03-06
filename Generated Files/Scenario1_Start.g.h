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
                ref class Grid;
                ref class StackPanel;
                ref class Border;
                ref class TextBlock;
                ref class Slider;
                ref class ComboBox;
            }
        }
    }
}

namespace Hot3dxRotoDraw
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class Scenario1_Start : public ::Windows::UI::Xaml::Controls::Page, 
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
    
        class Scenario1_Start_obj1_Bindings;
    
        private: ::Windows::UI::Xaml::Controls::Grid^ RootGrid;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ HeaderPanel;
        private: ::Windows::UI::Xaml::Controls::Border^ ErrorBorder;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ StatusBlock;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ text6Block1;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ text6Block2;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ text6Block3;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ SwitchMenuSidesTextBlock;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlock2;
        private: ::Windows::UI::Xaml::Controls::Slider^ RotationSpeedSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlock1;
        private: ::Windows::UI::Xaml::Controls::ComboBox^ EnvironmentComboBox;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlock;
        private: ::Windows::UI::Xaml::Controls::Slider^ RadiusSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlock3;
        private: ::Windows::UI::Xaml::Controls::Slider^ HeightSlider;
    };
}

