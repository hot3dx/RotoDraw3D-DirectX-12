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
                ref class TextBox;
                ref class Slider;
                ref class CheckBox;
                ref class Button;
            }
        }
    }
}
namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Shapes {
                ref class Rectangle;
            }
        }
    }
}

namespace Hot3dxRotoDraw
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class Scenario2_Normal : public ::Windows::UI::Xaml::Controls::Page, 
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
    
        class Scenario2_Normal_obj1_Bindings;
    
        private: ::Windows::UI::Xaml::Controls::Grid^ RootGrid;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ HeaderPanel;
        private: ::Windows::UI::Xaml::Controls::Border^ ErrorBorder;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ StatusBlock;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ left;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ IDC_POINT_SPACE_STATIC;
        private: ::Windows::UI::Xaml::Controls::TextBox^ PointSpacingTextBox;
        private: ::Windows::UI::Xaml::Controls::Slider^ IDC_SLIDER;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ IDC_PARTIAL_ROTATE_STATIC;
        private: ::Windows::UI::Xaml::Controls::TextBox^ PartialRotateTextBox;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ IDC_CLOSED_OR_OPEN_CHECKBOX;
        private: ::Windows::UI::Xaml::Controls::Slider^ IDC_PARTIAL_ROTATE_SLIDER;
        private: ::Windows::UI::Xaml::Controls::Button^ IDC_SET_COLORS_BUTTON;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ IDC_FACES_STATIC;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ IDC_EXTERIOR_FACES_CHECKBOX;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ IDC_INTERIOR_FACES_CHECKBOX;
        private: ::Windows::UI::Xaml::Controls::Button^ IDC_COPY_FLIP_BUTTON;
        private: ::Windows::UI::Xaml::Controls::Button^ IDC_SET_POINTS_BUTTON;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ IDC_ROTATION_STATIC;
        private: ::Windows::UI::Xaml::Controls::TextBox^ IDC_ROTATION_EDIT;
        private: ::Windows::UI::Xaml::Shapes::Rectangle^ IDC_CLIP_STATIC2;
        private: ::Windows::UI::Xaml::Controls::Button^ IDC_CLEAR_BUTTON;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ IDC_Advanced_STATIC;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ IDC_FIRST_TO_LAST_CHECKBOX;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ IDC_X_OR_Y_AXIS_STATIC;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ IDC_AXIS_CHECKBOX;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ IDC_DRAW_LINE_FIRST_STATIC;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ IDC_TOP_OR_LEFT_CHECKBOX;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ IDC_BOTTOM_OR_RIGHT_CHECKBOX;
    };
}
