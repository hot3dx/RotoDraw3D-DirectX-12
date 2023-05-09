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
                ref class TextBlock;
                ref class CheckBox;
                ref class TextBox;
                ref class Slider;
            }
        }
    }
}

namespace Hot3dxRotoDraw
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class Scenario10_Sculpt : public ::Windows::UI::Xaml::Controls::Page, 
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
    
        class Scenario10_Sculpt_obj1_Bindings;
    
        private: ::Windows::UI::Xaml::Controls::Grid^ RootGrid;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ HeaderPanel;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ m_sculptText;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ checkBoxSculptBool;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ m_sculptMovementLock;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ checkBoxSculptXBool;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ checkBoxSculptYBool;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ checkBoxSculptZBool;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ m_sculptObjectData;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxGroupCount;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxPointsPerGroup;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxPointCount;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ m_selectedSculptObjectData;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxGroupSelected;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxPointSelected;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxSelectedPointCount;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxSelectedPointX;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxSelectedPointY;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxSelectedPointZ;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ m_selectedGroup;
        private: ::Windows::UI::Xaml::Controls::Slider^ slider_SelectedGroup;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ m_selectedPoint;
        private: ::Windows::UI::Xaml::Controls::Slider^ slider_SelectedPoint;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ RotateXTextBlock;
        private: ::Windows::UI::Xaml::Controls::TextBox^ GRIDCAM_XROTATE_EDIT;
        private: ::Windows::UI::Xaml::Controls::Slider^ slider_RotateXGridCam;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ RotateYTextBlock;
        private: ::Windows::UI::Xaml::Controls::TextBox^ GRIDCAM_YROTATE_EDIT;
        private: ::Windows::UI::Xaml::Controls::Slider^ slider_RotateYGridCam;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ RotateZTextBlock;
        private: ::Windows::UI::Xaml::Controls::TextBox^ GRIDCAM_ZROTATE_EDIT;
        private: ::Windows::UI::Xaml::Controls::Slider^ slider_RotateZGridCam;
        private: ::Windows::UI::Xaml::Controls::TextBox^ m_sculptTextBox;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ m_sculptFilesText;
        private: ::Windows::UI::Xaml::Controls::TextBox^ m_sculptFilesTextBox;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ IDC_DESCRIPTION_STATIC;
    };
}

