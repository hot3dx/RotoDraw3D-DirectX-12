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

#include "Scenario6_ColorPicker.xaml.h"

void ::Hot3dxRotoDraw::Scenario6_ColorPicker::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///Scenario6_ColorPicker.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}


/// <summary>
/// Auto generated class for compiled bindings.
/// </summary>
class Hot3dxRotoDraw::Scenario6_ColorPicker::Scenario6_ColorPicker_obj1_Bindings 
    : public ::XamlBindingInfo::ReferenceTypeXamlBindings<::Hot3dxRotoDraw::Scenario6_ColorPicker, ::XamlBindingInfo::XamlBindingTrackingBase>
    , public ::XamlBindingInfo::IXamlBindingTracking
{
public:
    Scenario6_ColorPicker_obj1_Bindings()
    {
        InitializeTracking(this);
    }

private:
    ~Scenario6_ColorPicker_obj1_Bindings()
    {
        ReleaseAllListeners();
    }

public:

    void Connect(int __connectionId, ::Platform::Object^ __target)
    {
        switch(__connectionId)
        {
            case 9: // Scenario6_ColorPicker.xaml line 78
                this->obj9 = safe_cast<::Windows::UI::Xaml::Media::SolidColorBrush^>(__target);
                break;
            case 10: // Scenario6_ColorPicker.xaml line 70
                this->obj10 = safe_cast<::Windows::UI::Xaml::Media::SolidColorBrush^>(__target);
                break;
            case 12: // Scenario6_ColorPicker.xaml line 57
                this->obj12 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                break;
            case 13: // Scenario6_ColorPicker.xaml line 59
                this->obj13 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                break;
            case 17: // Scenario6_ColorPicker.xaml line 29
                this->obj17 = safe_cast<::Windows::UI::Xaml::Controls::ColorPicker^>(__target);
                break;
            case 19: // Scenario6_ColorPicker.xaml line 43
                this->obj19 = safe_cast<::Windows::UI::Xaml::Media::SolidColorBrush^>(__target);
                break;
        }
    }

    void DisconnectUnloadedObject(int connectionId)
    {
        throw ref new ::Platform::InvalidArgumentException("No unloadable elements to disconnect.");
    }

private:
    // Fields for each control that has bindings.
    ::Windows::UI::Xaml::Media::SolidColorBrush^ obj9;
    ::Windows::UI::Xaml::Media::SolidColorBrush^ obj10;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj12;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj13;
    ::Windows::UI::Xaml::Controls::ColorPicker^ obj17;
    ::Windows::UI::Xaml::Media::SolidColorBrush^ obj19;
    
    // Fields for binding tracking.
    ::Windows::UI::Xaml::DependencyObject^ cacheDPC_colorPicker_Color = nullptr;
    ::Windows::UI::Xaml::DependencyObject^ cacheDPC_alpha_IsChecked = nullptr;
    ::Windows::UI::Xaml::DependencyObject^ cacheDPC_colorSlider_IsChecked = nullptr;
    ::Windows::UI::Xaml::DependencyObject^ cacheDPC_colorChannelInput_IsChecked = nullptr;
    ::Windows::UI::Xaml::DependencyObject^ cacheDPC_hexInput_IsChecked = nullptr;
    ::Windows::UI::Xaml::DependencyObject^ cacheDPC_alphaSlider_IsChecked = nullptr;
    ::Windows::UI::Xaml::DependencyObject^ cacheDPC_alphaTextInput_IsChecked = nullptr;
    __int64 tokenDPC_colorPicker_Color = 0;
    __int64 tokenDPC_alpha_IsChecked = 0;
    __int64 tokenDPC_colorSlider_IsChecked = 0;
    __int64 tokenDPC_colorChannelInput_IsChecked = 0;
    __int64 tokenDPC_hexInput_IsChecked = 0;
    __int64 tokenDPC_alphaSlider_IsChecked = 0;
    __int64 tokenDPC_alphaTextInput_IsChecked = 0;

    // Update methods for each path node used in binding steps.
    void Update_(::Hot3dxRotoDraw::Scenario6_ColorPicker^ obj, int phase)
    {
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_colorPicker(obj->colorPicker, phase);
                this->Update_alpha(obj->alpha, phase);
                this->Update_colorSlider(obj->colorSlider, phase);
                this->Update_colorChannelInput(obj->colorChannelInput, phase);
                this->Update_hexInput(obj->hexInput, phase);
                this->Update_alphaSlider(obj->alphaSlider, phase);
                this->Update_alphaTextInput(obj->alphaTextInput, phase);
            }
        }
    }
    void Update_colorPicker(::Windows::UI::Xaml::Controls::ColorPicker^ obj, int phase)
    {
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(obj, ::Windows::UI::Xaml::Controls::ColorPicker::ColorProperty, &this->cacheDPC_colorPicker_Color, &this->tokenDPC_colorPicker_Color);
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_colorPicker_Color(obj->Color, phase);
            }
        }
    }
    void Update_colorPicker_Color(::Windows::UI::Color obj, int phase)
    {
        if ((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // Scenario6_ColorPicker.xaml line 78
            Set_Windows_UI_Xaml_Media_SolidColorBrush_Color(this->obj9, obj);
            // Scenario6_ColorPicker.xaml line 70
            Set_Windows_UI_Xaml_Media_SolidColorBrush_Color(this->obj10, obj);
            // Scenario6_ColorPicker.xaml line 43
            Set_Windows_UI_Xaml_Media_SolidColorBrush_Color(this->obj19, obj);
        }
    }
    void Update_alpha(::Windows::UI::Xaml::Controls::CheckBox^ obj, int phase)
    {
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(obj, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_alpha_IsChecked, &this->tokenDPC_alpha_IsChecked);
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_alpha_IsChecked(obj->IsChecked, phase);
            }
        }
    }
    void Update_alpha_IsChecked(::Platform::IBox<::Platform::Boolean>^ obj, int phase)
    {
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_alpha_IsChecked_Value(obj->Value, phase);
            }
        }
    }
    void Update_alpha_IsChecked_Value(::Platform::Boolean obj, int phase)
    {
        if ((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // Scenario6_ColorPicker.xaml line 57
            Set_Windows_UI_Xaml_Controls_Control_IsEnabled(this->obj12, obj);
            // Scenario6_ColorPicker.xaml line 59
            Set_Windows_UI_Xaml_Controls_Control_IsEnabled(this->obj13, obj);
            // Scenario6_ColorPicker.xaml line 29
            Set_Windows_UI_Xaml_Controls_ColorPicker_IsAlphaEnabled(this->obj17, obj);
        }
    }
    void Update_colorSlider(::Windows::UI::Xaml::Controls::CheckBox^ obj, int phase)
    {
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(obj, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_colorSlider_IsChecked, &this->tokenDPC_colorSlider_IsChecked);
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_colorSlider_IsChecked(obj->IsChecked, phase);
            }
        }
    }
    void Update_colorSlider_IsChecked(::Platform::IBox<::Platform::Boolean>^ obj, int phase)
    {
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_colorSlider_IsChecked_Value(obj->Value, phase);
            }
        }
    }
    void Update_colorSlider_IsChecked_Value(::Platform::Boolean obj, int phase)
    {
        if ((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // Scenario6_ColorPicker.xaml line 29
            Set_Windows_UI_Xaml_Controls_ColorPicker_IsColorSliderVisible(this->obj17, obj);
        }
    }
    void Update_colorChannelInput(::Windows::UI::Xaml::Controls::CheckBox^ obj, int phase)
    {
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(obj, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_colorChannelInput_IsChecked, &this->tokenDPC_colorChannelInput_IsChecked);
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_colorChannelInput_IsChecked(obj->IsChecked, phase);
            }
        }
    }
    void Update_colorChannelInput_IsChecked(::Platform::IBox<::Platform::Boolean>^ obj, int phase)
    {
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_colorChannelInput_IsChecked_Value(obj->Value, phase);
            }
        }
    }
    void Update_colorChannelInput_IsChecked_Value(::Platform::Boolean obj, int phase)
    {
        if ((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // Scenario6_ColorPicker.xaml line 29
            Set_Windows_UI_Xaml_Controls_ColorPicker_IsColorChannelTextInputVisible(this->obj17, obj);
        }
    }
    void Update_hexInput(::Windows::UI::Xaml::Controls::CheckBox^ obj, int phase)
    {
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(obj, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_hexInput_IsChecked, &this->tokenDPC_hexInput_IsChecked);
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_hexInput_IsChecked(obj->IsChecked, phase);
            }
        }
    }
    void Update_hexInput_IsChecked(::Platform::IBox<::Platform::Boolean>^ obj, int phase)
    {
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_hexInput_IsChecked_Value(obj->Value, phase);
            }
        }
    }
    void Update_hexInput_IsChecked_Value(::Platform::Boolean obj, int phase)
    {
        if ((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // Scenario6_ColorPicker.xaml line 29
            Set_Windows_UI_Xaml_Controls_ColorPicker_IsHexInputVisible(this->obj17, obj);
        }
    }
    void Update_alphaSlider(::Windows::UI::Xaml::Controls::CheckBox^ obj, int phase)
    {
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(obj, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_alphaSlider_IsChecked, &this->tokenDPC_alphaSlider_IsChecked);
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_alphaSlider_IsChecked(obj->IsChecked, phase);
            }
        }
    }
    void Update_alphaSlider_IsChecked(::Platform::IBox<::Platform::Boolean>^ obj, int phase)
    {
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_alphaSlider_IsChecked_Value(obj->Value, phase);
            }
        }
    }
    void Update_alphaSlider_IsChecked_Value(::Platform::Boolean obj, int phase)
    {
        if ((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // Scenario6_ColorPicker.xaml line 29
            Set_Windows_UI_Xaml_Controls_ColorPicker_IsAlphaSliderVisible(this->obj17, obj);
        }
    }
    void Update_alphaTextInput(::Windows::UI::Xaml::Controls::CheckBox^ obj, int phase)
    {
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(obj, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_alphaTextInput_IsChecked, &this->tokenDPC_alphaTextInput_IsChecked);
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_alphaTextInput_IsChecked(obj->IsChecked, phase);
            }
        }
    }
    void Update_alphaTextInput_IsChecked(::Platform::IBox<::Platform::Boolean>^ obj, int phase)
    {
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_alphaTextInput_IsChecked_Value(obj->Value, phase);
            }
        }
    }
    void Update_alphaTextInput_IsChecked_Value(::Platform::Boolean obj, int phase)
    {
        if ((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // Scenario6_ColorPicker.xaml line 29
            Set_Windows_UI_Xaml_Controls_ColorPicker_IsAlphaTextInputVisible(this->obj17, obj);
        }
    }

    virtual void ReleaseAllListeners() override
    {
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(nullptr, ::Windows::UI::Xaml::Controls::ColorPicker::ColorProperty, &this->cacheDPC_colorPicker_Color, &this->tokenDPC_colorPicker_Color);
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(nullptr, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_alpha_IsChecked, &this->tokenDPC_alpha_IsChecked);
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(nullptr, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_colorSlider_IsChecked, &this->tokenDPC_colorSlider_IsChecked);
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(nullptr, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_colorChannelInput_IsChecked, &this->tokenDPC_colorChannelInput_IsChecked);
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(nullptr, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_hexInput_IsChecked, &this->tokenDPC_hexInput_IsChecked);
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(nullptr, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_alphaSlider_IsChecked, &this->tokenDPC_alphaSlider_IsChecked);
        this->_bindingsTracking->UpdateDependencyPropertyChangedListener(nullptr, ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty, &this->cacheDPC_alphaTextInput_IsChecked, &this->tokenDPC_alphaTextInput_IsChecked);
    }

    virtual void PropertyChanged(Platform::Object^ sender, ::Windows::UI::Xaml::Data::PropertyChangedEventArgs^ e) override
    {
    }

    void CollectionChanged(::Platform::Object^ sender, ::Windows::UI::Xaml::Interop::NotifyCollectionChangedEventArgs^ e)
    {
    }

    void VectorChanged(::Platform::Object^ sender, ::Windows::Foundation::Collections::IVectorChangedEventArgs^ e)
    {
    }

    void MapChanged(::Platform::Object^ sender, ::Windows::Foundation::Collections::IMapChangedEventArgs<::Platform::String^>^ e)
    {
    }

    void DependencyPropertyChanged(::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
    {
        if (sender != nullptr)
        {
            if (sender->Equals(this->cacheDPC_colorPicker_Color) && ::Windows::UI::Xaml::Controls::ColorPicker::ColorProperty->Equals(prop))
            {
                ::Windows::UI::Xaml::Controls::ColorPicker^ obj = safe_cast<::Windows::UI::Xaml::Controls::ColorPicker^>(sender);
                if (obj != nullptr)
                {
                    this->Update_colorPicker_Color(obj->Color, DATA_CHANGED);
                }
            }
            if (sender->Equals(this->cacheDPC_alpha_IsChecked) && ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty->Equals(prop))
            {
                ::Windows::UI::Xaml::Controls::CheckBox^ obj = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(sender);
                if (obj != nullptr)
                {
                    this->Update_alpha_IsChecked(obj->IsChecked, DATA_CHANGED);
                }
            }
            if (sender->Equals(this->cacheDPC_colorSlider_IsChecked) && ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty->Equals(prop))
            {
                ::Windows::UI::Xaml::Controls::CheckBox^ obj = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(sender);
                if (obj != nullptr)
                {
                    this->Update_colorSlider_IsChecked(obj->IsChecked, DATA_CHANGED);
                }
            }
            if (sender->Equals(this->cacheDPC_colorChannelInput_IsChecked) && ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty->Equals(prop))
            {
                ::Windows::UI::Xaml::Controls::CheckBox^ obj = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(sender);
                if (obj != nullptr)
                {
                    this->Update_colorChannelInput_IsChecked(obj->IsChecked, DATA_CHANGED);
                }
            }
            if (sender->Equals(this->cacheDPC_hexInput_IsChecked) && ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty->Equals(prop))
            {
                ::Windows::UI::Xaml::Controls::CheckBox^ obj = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(sender);
                if (obj != nullptr)
                {
                    this->Update_hexInput_IsChecked(obj->IsChecked, DATA_CHANGED);
                }
            }
            if (sender->Equals(this->cacheDPC_alphaSlider_IsChecked) && ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty->Equals(prop))
            {
                ::Windows::UI::Xaml::Controls::CheckBox^ obj = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(sender);
                if (obj != nullptr)
                {
                    this->Update_alphaSlider_IsChecked(obj->IsChecked, DATA_CHANGED);
                }
            }
            if (sender->Equals(this->cacheDPC_alphaTextInput_IsChecked) && ::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty->Equals(prop))
            {
                ::Windows::UI::Xaml::Controls::CheckBox^ obj = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(sender);
                if (obj != nullptr)
                {
                    this->Update_alphaTextInput_IsChecked(obj->IsChecked, DATA_CHANGED);
                }
            }
        }
    }

    static void Set_Windows_UI_Xaml_Media_SolidColorBrush_Color(::Windows::UI::Xaml::Media::SolidColorBrush^ obj, ::Windows::UI::Color value)
    {
        obj->Color = value;
    }
    static void Set_Windows_UI_Xaml_Controls_Control_IsEnabled(::Windows::UI::Xaml::Controls::Control^ obj, ::Platform::Boolean value)
    {
        obj->IsEnabled = value;
    }
    static void Set_Windows_UI_Xaml_Controls_ColorPicker_IsColorSliderVisible(::Windows::UI::Xaml::Controls::ColorPicker^ obj, ::Platform::Boolean value)
    {
        obj->IsColorSliderVisible = value;
    }
    static void Set_Windows_UI_Xaml_Controls_ColorPicker_IsColorChannelTextInputVisible(::Windows::UI::Xaml::Controls::ColorPicker^ obj, ::Platform::Boolean value)
    {
        obj->IsColorChannelTextInputVisible = value;
    }
    static void Set_Windows_UI_Xaml_Controls_ColorPicker_IsHexInputVisible(::Windows::UI::Xaml::Controls::ColorPicker^ obj, ::Platform::Boolean value)
    {
        obj->IsHexInputVisible = value;
    }
    static void Set_Windows_UI_Xaml_Controls_ColorPicker_IsAlphaEnabled(::Windows::UI::Xaml::Controls::ColorPicker^ obj, ::Platform::Boolean value)
    {
        obj->IsAlphaEnabled = value;
    }
    static void Set_Windows_UI_Xaml_Controls_ColorPicker_IsAlphaSliderVisible(::Windows::UI::Xaml::Controls::ColorPicker^ obj, ::Platform::Boolean value)
    {
        obj->IsAlphaSliderVisible = value;
    }
    static void Set_Windows_UI_Xaml_Controls_ColorPicker_IsAlphaTextInputVisible(::Windows::UI::Xaml::Controls::ColorPicker^ obj, ::Platform::Boolean value)
    {
        obj->IsAlphaTextInputVisible = value;
    }
};

void ::Hot3dxRotoDraw::Scenario6_ColorPicker::Connect(int __connectionId, ::Platform::Object^ __target)
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
            this->alpha = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
        }
        break;
    case 5:
        {
            this->m_frontColor = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(this->m_frontColor))->Checked += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario6_ColorPicker::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario6_ColorPicker::frontColor_Checked);
        }
        break;
    case 6:
        {
            this->ColorFrontRect = safe_cast<::Windows::UI::Xaml::Shapes::Rectangle^>(__target);
        }
        break;
    case 7:
        {
            this->m_backColor = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(this->m_backColor))->Checked += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Hot3dxRotoDraw::Scenario6_ColorPicker::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Scenario6_ColorPicker::backColor_Checked);
        }
        break;
    case 8:
        {
            this->ColorBackRect = safe_cast<::Windows::UI::Xaml::Shapes::Rectangle^>(__target);
        }
        break;
    case 11:
        {
            this->hexInput = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
        }
        break;
    case 12:
        {
            this->alphaTextInput = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
        }
        break;
    case 13:
        {
            this->alphaSlider = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
        }
        break;
    case 14:
        {
            this->moreBtn = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
        }
        break;
    case 15:
        {
            this->colorSlider = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
        }
        break;
    case 16:
        {
            this->colorChannelInput = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
        }
        break;
    case 17:
        {
            this->colorPicker = safe_cast<::Windows::UI::Xaml::Controls::ColorPicker^>(__target);
        }
        break;
    case 18:
        {
            this->previewRect = safe_cast<::Windows::UI::Xaml::Shapes::Rectangle^>(__target);
        }
        break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::Hot3dxRotoDraw::Scenario6_ColorPicker::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    ::XamlBindingInfo::XamlBindings^ bindings = nullptr;
    switch (__connectionId)
    {
        case 1: // Scenario6_ColorPicker.xaml line 13
            {
                ::Windows::UI::Xaml::Controls::Page^ element1 = safe_cast<::Windows::UI::Xaml::Controls::Page^>(__target);
                Scenario6_ColorPicker_obj1_Bindings* objBindings = new Scenario6_ColorPicker_obj1_Bindings();
                objBindings->SetDataRoot(this);
                bindings = ref new ::XamlBindingInfo::XamlBindings(objBindings);
                this->Bindings = bindings;
                element1->Loading += ref new ::Windows::Foundation::TypedEventHandler<::Windows::UI::Xaml::FrameworkElement^, ::Platform::Object^>(bindings, &::XamlBindingInfo::XamlBindings::Loading);
            }
            break;
    }
    return bindings;
}

#pragma warning(pop)

