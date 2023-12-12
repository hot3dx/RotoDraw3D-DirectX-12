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


#include "pch.h"
#include "Scenario2_Normal.xaml.h"
#include "Content\Hot3dxRotoDrawVariables.h"

using namespace Hot3dxRotoDraw;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace concurrency;

Scenario2_Normal^ Scenario2_Normal::Current = nullptr;

Hot3dxRotoDraw::Scenario2_Normal::Scenario2_Normal() : _rootPage(DirectXPage::Current)
   // m_iEffectDescSelectedIndex(0),//L"Basic Effect"),
   // m_iRasterDescIndex(0),//L"CullNone"),
   // m_iSamplWrapIndex(0)//L"Anisotropic Wrap")
{
    InitializeComponent();
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    _rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
    SolidColorBrush^ brush = ref new SolidColorBrush(_rootPage->m_Scene2Vars->GetPointColor());
    IDC_CLIP_STATIC2->Fill = brush;
    _rootPage->SetPointColorDXP(brush->Color);
    vars->SetPointColor(brush->Color);
    IDC_ROTATION_EDIT->Text = L"10";
    m_pointColor = { 255,125,125,0 };
    IDC_POINT_SPACE_STATIC->Text = "0.300000f";
    PointSpacingTextBox->Text = "0.300000f";
    PartialRotateTextBox->Text = "0.0f";
    vars->SetPartialRotateAngle(0.0f);
    vars->SetOpenOrClosedChecked(true);
    m_bAxisChecked = true;
    vars->SetAxisChecked(m_bAxisChecked);
    m_bTopOrLeftChecked = false;
    vars->SetTopOrLeftChecked(m_bTopOrLeftChecked);
    m_bBottomOrRightChecked = false;
    vars->SetBottomOrRightChecked(false);
    m_bColorOnlyNoTexture = (false);
    vars->SetColorOnlyNoTexture(false);
    DenomOf360 = 10.0f;
    m_fPointGroupsCount = 36;
    SetPartialSlider();
    Scenario2_Normal::Current = this;
    _rootPage->m_Scene2Vars->SetScenario2Page(this);

    IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked::set(true);
    IDC_CLOSED_OR_OPEN_CHECKBOX->SetValue(IDC_CLOSED_OR_OPEN_CHECKBOX->IsCheckedProperty, 
        IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked);
    /*
    Platform::Object^ val = SamplIndexWrapComboBox->SelectedValue::get();
    RasterDescComboBox->SelectedValue::set(m_iRasterDescIndex);
    RasterDescComboBox->SetValue(RasterDescComboBox->SelectedIndexProperty, RasterDescComboBox->SelectedValue);
    SamplIndexWrapComboBox->SelectedValue::set(m_iSamplWrapIndex);
    SamplIndexWrapComboBox->SetValue(SamplIndexWrapComboBox->SelectedIndexProperty, SamplIndexWrapComboBox->SelectedValue);
    EffectDescComboBox->SelectedValue::set(m_iEffectDescSelectedIndex);
    EffectDescComboBox->SetValue(EffectDescComboBox->SelectedIndexProperty, EffectDescComboBox->SelectedValue);
    */
    _rootPage->SetEffectIndexRenderer(0);

    Scenario2_Normal::Current = this;
    _rootPage->m_Scene2Vars->SetScenario2Page(this);

}
    


Hot3dxRotoDraw::Scenario2_Normal::~Scenario2_Normal()
{
   
}

void Hot3dxRotoDraw::Scenario2_Normal::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
    Scenario2_Normal::Current = this;
    IDC_TOP_OR_LEFT_CHECKBOX->IsChecked = false;// (nullptr, false);
    IDC_BOTTOM_OR_RIGHT_CHECKBOX->IsChecked = false;
    //unsigned int val = EffectDescComboBox->SelectedIndex::get();
    m_iEffectDescSelectedIndex = _rootPage->GetEffectIndexDXP();
    //Platform::String^ s1 = ref new Platform::String(L"\nVAL is: ");
    //OutputDebugString(s1->Concat(s1, ref new Platform::String(std::to_wstring(m_iEffectDescSelectedIndex).c_str()))->ToString()->Data());
    Platform::Object^ s;
    switch (m_iEffectDescSelectedIndex)
    {
    case 0:
        s = ref new Platform::String(L"Basic Effect");
        break;
    case 1:
        s = ref new Platform::String(L"DualTexture Effect");
        break;
    case 2:
        s = ref new Platform::String(L"PBR Effect");
        break;
    }
    m_iEffectDescSelectedIndex = _rootPage->GetEffectIndexDXP();
    EffectDescComboBox->SelectedIndex = (m_iEffectDescSelectedIndex);
    EffectDescComboBox->SetValue(EffectDescComboBox->SelectedIndexProperty, EffectDescComboBox->SelectedIndex);
    
       _rootPage->NotifyUser(s->ToString()->Concat(L"\nEffect is: ", s->ToString()), NotifyType::StatusMessage);
    _rootPage->SetEffectIndexRenderer(m_iEffectDescSelectedIndex);
  
    _rootPage->m_Scene2Vars->SetScenario2Page(this);
    
}


void Scenario2_Normal::PointSpacingTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
    IDC_POINT_SPACE_STATIC->Text = ref new Platform::String(PointSpacingTextBox->Text->Data());
    Platform::String^ str = ref new Platform::String(PointSpacingTextBox->Text->Data());
    _rootPage->SetPointSpacingDXP(std::wcstof(str->Data(), nullptr));
}

// Sets Partial Slider for objects that are not 360 degrees
void Scenario2_Normal::SetPartialSlider()
{
    IDC_PARTIAL_ROTATE_STATIC->Text = ref new Platform::String(std::to_wstring(m_fPointGroupsCount).c_str());
    IDC_PARTIAL_ROTATE_SLIDER->Maximum::set(m_fPointGroupsCount);
    PartialRotateTextBox->Text = ref new Platform::String(std::to_wstring(m_fPointGroupsCount).c_str());
    IDC_PARTIAL_ROTATE_SLIDER->Maximum::set(m_fPointGroupsCount);
    IDC_PARTIAL_ROTATE_SLIDER->SetValue(IDC_PARTIAL_ROTATE_SLIDER->MaximumProperty, IDC_PARTIAL_ROTATE_SLIDER->Maximum);
    IDC_PARTIAL_ROTATE_SLIDER->Value::set(0);
    IDC_PARTIAL_ROTATE_SLIDER->SetValue(IDC_PARTIAL_ROTATE_SLIDER->ValueProperty, IDC_PARTIAL_ROTATE_SLIDER->Value);
    PartialRotateTextBox->Text = ref new Platform::String(std::to_wstring(0).c_str());
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_PARTIAL_ROTATE_SLIDER_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    unsigned int value = static_cast<unsigned int>(e->NewValue);
    IDC_PARTIAL_ROTATE_STATIC->Text = ref new Platform::String(PartialRotateTextBox->Text->Data());
    PartialRotateTextBox->Text = ref new Platform::String(std::to_wstring(value).c_str());
    _rootPage->SetPartialRotateAngleDXP(static_cast<float>(value));
}

void Hot3dxRotoDraw::Scenario2_Normal::PartialRotateTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
    DenomOf360 = std::wcstof(IDC_ROTATION_EDIT->Text->Data(), nullptr);
    m_fPointGroupsCount = static_cast<unsigned int>(360.0f / DenomOf360);
    
    IDC_PARTIAL_ROTATE_STATIC->Text = ref new Platform::String(std::to_wstring(m_fPointGroupsCount).c_str());// PartialRotateTextBox->Text->Data());
    Platform::String^ str = ref new Platform::String(PartialRotateTextBox->Text->Data());
    _rootPage->SetPartialRotateAngleDXP(std::wcstof(str->Data(), nullptr));
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_CLOSED_OR_OPEN_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked->Value)
    {
        vars->SetOpenOrClosedChecked(true);
        IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked::set(true);
        IDC_CLOSED_OR_OPEN_CHECKBOX->SetValue(IDC_CLOSED_OR_OPEN_CHECKBOX->IsCheckedProperty,
            IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked);
        _rootPage->NotifyUser("Closed Partial Object Picked", NotifyType::StatusMessage);
    }
    else {
        vars->SetOpenOrClosedChecked(false);
        IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked::set(false);
        IDC_CLOSED_OR_OPEN_CHECKBOX->SetValue(IDC_CLOSED_OR_OPEN_CHECKBOX->IsCheckedProperty,
            IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked);
        _rootPage->NotifyUser("Open Partial Object Picked", NotifyType::StatusMessage);
    }
    //_rootPage->SetDoFrontFacesDXP(m_bDoFrontFaces);
   

    
}

void Scenario2_Normal::IDC_CLEAR_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    _rootPage->NotifyUser("New Page", NotifyType::StatusMessage);
    IDC_ROTATION_EDIT->Text = L"10";
    m_pointColor = { 255,125,125,0 };
    IDC_POINT_SPACE_STATIC->Text = "0.300000f";
    PointSpacingTextBox->Text = "0.300000f";
    PartialRotateTextBox->Text = "1.0f";
    m_bAxisChecked = true;
    vars->SetAxisChecked(m_bAxisChecked);
    m_bTopOrLeftChecked = false;
    vars->SetTopOrLeftChecked(m_bTopOrLeftChecked);
    m_bBottomOrRightChecked = false;
    vars->SetBottomOrRightChecked(false);
    DenomOf360 = 10.0f;
    m_fPointGroupsCount = 36;
    SetPartialSlider();
    IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked::set(true);
    IDC_CLOSED_OR_OPEN_CHECKBOX->SetValue(IDC_CLOSED_OR_OPEN_CHECKBOX->IsCheckedProperty,
        IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked);
    vars->SetOpenOrClosedChecked(true);
    _rootPage->IDC_CLEAR_BUTTON_Click(sender, e);    
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_SET_COLORS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    SolidColorBrush^ brush = ref new SolidColorBrush(_rootPage->m_Scene2Vars->GetPointColor());
     IDC_CLIP_STATIC2->Fill = brush;
     _rootPage->SetPointColorDXP(brush->Color);
     
     Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
     vars->SetPointColor(brush->Color);
     _rootPage->NotifyUser("Point Color Set", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_EXTERIOR_FACES_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_EXTERIOR_FACES_CHECKBOX->IsChecked->Value)
    {
        m_bDoFrontFaces = true;
    }
    else {
        m_bDoFrontFaces = false;
    }
    _rootPage->SetDoFrontFacesDXP(m_bDoFrontFaces);
    vars->SetDoFrontFaces(m_bDoFrontFaces);

    _rootPage->NotifyUser("Exterior Faces Picked", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_INTERIOR_FACES_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_INTERIOR_FACES_CHECKBOX->IsChecked->Value)
    {
        m_bDoBackFaces = true;
    }
    else {
        m_bDoBackFaces = false;
    }
    _rootPage->SetDoBackFacesDXP(m_bDoBackFaces);
    vars->SetDoBackFaces(m_bDoBackFaces);

    _rootPage->NotifyUser("Interior Faces Picked", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_FIRST_TO_LAST_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_FIRST_TO_LAST_CHECKBOX->IsChecked->Value)
    {
        m_bFirstToLast = true;
    }
    else {
        m_bFirstToLast = false;
    }
    _rootPage->SetFirstToLastDXP(m_bFirstToLast);
    vars->SetFirstToLast(m_bFirstToLast);
    _rootPage->NotifyUser("First To Last Picked", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_AXIS_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    //Platform::IBox<bool>^ tf = IDC_AXIS_CHECKBOX->IsChecked->Value;
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_AXIS_CHECKBOX->IsChecked->Value)
    {
        m_bAxisChecked = true;
    }
    else {
        m_bAxisChecked = false;
    }
    _rootPage->SetAxisCheckedDXP(m_bAxisChecked);
    vars->SetAxisChecked(m_bAxisChecked);
    _rootPage->NotifyUser("Axis Change Picked", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_TOP_OR_LEFT_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_TOP_OR_LEFT_CHECKBOX->IsChecked->Value)
    {
        m_bTopOrLeftChecked = true;
    }
    else {
        m_bTopOrLeftChecked = false;
    }
    _rootPage->SetTopOrLeftCheckedDXP(m_bTopOrLeftChecked);
    vars->SetTopOrLeftChecked(m_bTopOrLeftChecked);
    _rootPage->NotifyUser("Top or Left Change Picked", NotifyType::StatusMessage);

}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_BOTTOM_OR_RIGHT_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_BOTTOM_OR_RIGHT_CHECKBOX->IsChecked->Value)
    {
        m_bBottomOrRightChecked = true;
    }
    else {
        m_bBottomOrRightChecked = false;
    }
    _rootPage->SetBottomOrRightCheckedDXP(m_bBottomOrRightChecked);
    vars->SetBottomOrRightChecked(m_bBottomOrRightChecked);
    _rootPage->NotifyUser("Bottom or Right Change Picked", NotifyType::StatusMessage);
}

VOID Hot3dxRotoDraw::Scenario2_Normal::IDC_ROTATION_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
    if (std::wcstof(IDC_ROTATION_EDIT->Text->Data(), nullptr) < 1
        || std::wcstof(IDC_ROTATION_EDIT->Text->Data(), nullptr) > 180)
    {
        _rootPage->NotifyUser("Error: Angle out of range", NotifyType::StatusMessage);
    }
    else
    {
        _rootPage->SetPointDrawGroupAngleDXP(std::wcstof(IDC_ROTATION_EDIT->Text->Data(), nullptr));
        DenomOf360 = std::wcstof(IDC_ROTATION_EDIT->Text->Data(), nullptr);
        m_fPointGroupsCount = static_cast<unsigned int>(360.0f / DenomOf360) -2;
        SetPartialSlider();
    }

}


void Hot3dxRotoDraw::Scenario2_Normal::IDC_SLIDER_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    float value = static_cast<float>(e->NewValue);
    value *= 0.01f;
    IDC_POINT_SPACE_STATIC->Text = ref new Platform::String(PointSpacingTextBox->Text->Data());
    PointSpacingTextBox->Text = ref new Platform::String(std::to_wstring(value).c_str());
    _rootPage->SetPointSpacingDXP(value);
    
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_COPY_FLIP_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (m_bAxisChecked)
    {
        _rootPage->CopyFlipPointsXAxisDXP();
    }
    else
    {
        _rootPage->CopyFlipPointsYAxisDXP();
    }
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_SET_POINTS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->SET_POINTS_BUTTON_Click(sender, e);
}

void Hot3dxRotoDraw::Scenario2_Normal::RasterDescComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    unsigned int val = RasterDescComboBox->SelectedIndex;
     _rootPage->SetCullNoneToWireframeRenderer(val);
}

void Hot3dxRotoDraw::Scenario2_Normal::SamplIndexWrapComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    unsigned int val = SamplIndexWrapComboBox->SelectedIndex;
    _rootPage->SetSamplIndexWrapRenderer(val);
}


/*
void Hot3dxRotoDraw::Scenario2_Normal::IDC_SAVE_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    // Clear previous returned file name, if it exists, between iterations of this scenario
    //IDC_WELCOME_STATIC->Text = "";//was used as a sample but needs to be changed; A writer function needs to be called with
    //the file data to be saved.

    FileSavePicker^ savePicker = ref new FileSavePicker();
    savePicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;

    auto plainTextExtensions = ref new Platform::Collections::Vector<String^>();
    plainTextExtensions->Append(".txt");
    savePicker->FileTypeChoices->Insert("Plain Text", plainTextExtensions);
    savePicker->SuggestedFileName = "New Document";


    create_task(savePicker->PickSaveFileAsync()).then([this](StorageFile^ file)
        {
            if (file != nullptr)
            {
                Hot3dxRotoDraw::Scenario2_Normal^ _this = this;
                // Prevent updates to the remote version of the file until we finish making changes and call CompleteUpdatesAsync.
                CachedFileManager::DeferUpdates(file);
                // write to file
                create_task(FileIO::WriteTextAsync(file, file->Name)).then([_this, file]()
                    {
                        // Let Windows know that we're finished changing the file so the other app can update the remote version of the file.
                        // Completing updates may require Windows to ask for user input.
                        create_task(CachedFileManager::CompleteUpdatesAsync(file)).then([_this, file](FileUpdateStatus status)
                            {
                                if (status == FileUpdateStatus::Complete)
                                {
                                    //IDC_WELCOME_STATIC->Text = "File " + file->Name + " was saved.";
                                }
                                else
                                {
                                    //this->IDC_WELCOME_STATIC->Text = "File " + file->Name + " couldn't be saved.";
                                }
                            });
                    });
            }
            else
            {
                //this->IDC_WELCOME_STATIC->Text = "Operation cancelled.";
            }
        });
}
*/

void Hot3dxRotoDraw::Scenario2_Normal::EffectDescComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
    Scenario2_Normal::Current = this;
    if (_rootPage->GetPointCount() == 0)
    {
        unsigned int val = m_iEffectDescSelectedIndex = EffectDescComboBox->SelectedIndex::get();
        _rootPage->SetEffectIndexRenderer(val);
        //Platform::String^ s1 = ref new Platform::String(L"\nVAL is: ");
        //OutputDebugString(s1->Concat(s1, ref new Platform::String(std::to_wstring(m_iEffectDescSelectedIndex).c_str()))->ToString()->Data());
        Platform::Object^ s;
        switch (m_iEffectDescSelectedIndex)
        {
        case 0:
            s = ref new Platform::String(L"Basic Effect");
            break;
        case 1:
            s = ref new Platform::String(L"DualTexture Effect");
            break;
        case 2:
            s = ref new Platform::String(L"PBR Effect");
            break;
        case 3:
            s = ref new Platform::String(L"VideoTexture Effect");
            break;
        }

       // EffectDescComboBox->SetValue(EffectDescComboBox->PlaceholderTextProperty, s);
       // EffectDescComboBox->SelectedIndex = (m_iEffectDescSelectedIndex);
        EffectDescComboBox->SetValue(EffectDescComboBox->SelectedIndexProperty, EffectDescComboBox->SelectedIndex);

        _rootPage->NotifyUser(s->ToString()->Concat(L"\nEffect is: ", s->ToString()), NotifyType::StatusMessage);
        _rootPage->SetEffectIndexRenderer(m_iEffectDescSelectedIndex);
        _rootPage->m_Scene2Vars->SetScenario2Page(this);
    }
    else {
        EffectDescComboBox->SelectedIndex = (m_iEffectDescSelectedIndex);
        EffectDescComboBox->SetValue(EffectDescComboBox->SelectedIndexProperty, EffectDescComboBox->SelectedIndex);
        _rootPage->NotifyUser("Effects Cannot be changed once the Set Points Button is pushed until the Clear button is pushed: Pt Count must be ZERO!", NotifyType::StatusMessage);
    }
}

void Hot3dxRotoDraw::Scenario2_Normal::SetEffectDescComboBox(unsigned int val)
{
    m_iEffectDescSelectedIndex = val;
    EffectDescComboBox->SelectedIndex = val;
    EffectDescComboBox->SetValue(EffectDescComboBox->SelectedIndexProperty, EffectDescComboBox->SelectedIndex);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_COLOR_ONLY_NO_TEXTURE_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_COLOR_ONLY_NO_TEXTURE_CHECKBOX->IsChecked->Value)
    {
        m_bColorOnlyNoTexture = true;
    }
    else {
        m_bColorOnlyNoTexture = false;
    }

    vars->SetColorOnlyNoTexture(m_bColorOnlyNoTexture);
    _rootPage->NotifyUser("Color Only No Texture Draw Object Picked", NotifyType::StatusMessage);
}


void Hot3dxRotoDraw::Scenario2_Normal::IDC_TEXTURE_UV_SLIDER_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    _rootPage->SetUVPercentTextureDimensionDXP(static_cast<float>(e->NewValue));
}


void Hot3dxRotoDraw::Scenario2_Normal::IDC_HORIZONTAL_LINE_checkBox_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_HORIZONTAL_LINE_checkBox->IsChecked->Value == true)
    {
        _rootPage->SetIfRightShiftKeyHeldDrawStraightLine(true);
        _rootPage->NotifyUser("Draw Horizontal Straight Line Change to true", NotifyType::StatusMessage);
    }
    else {
        _rootPage->SetIfRightShiftKeyHeldDrawStraightLine(false);
        _rootPage->NotifyUser("Draw Horizontal Straight Line Change to false", NotifyType::StatusMessage);
    }
}


void Hot3dxRotoDraw::Scenario2_Normal::IDC_VERTICAL_LINE_checkBox_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_VERTICAL_LINE_checkBox->IsChecked->Value == true)
    {
        _rootPage->SetIfLeftShiftKeyHeldDrawStraightLine(true);
        _rootPage->NotifyUser("Draw Vertical Straight Line Change to true", NotifyType::StatusMessage);
    }
    else {
        _rootPage->SetIfLeftShiftKeyHeldDrawStraightLine(false);
        _rootPage->NotifyUser("Draw Vertical Straight Line Change to false", NotifyType::StatusMessage);
    }
}


void Hot3dxRotoDraw::Scenario2_Normal::IDC_45_DEGREE_LINE_Checkbox_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_45_DEGREE_LINE_Checkbox->IsChecked->Value == true)
    {
        _rootPage->SetIfLeftShiftKeyHeldDraw45Line(true);
        _rootPage->NotifyUser("Draw 45 deg Line Changed to true", NotifyType::StatusMessage);
    }
    else {
        _rootPage->SetIfLeftShiftKeyHeldDraw45Line(false);
        _rootPage->NotifyUser("Draw 45 deg Line Changed to false", NotifyType::StatusMessage);
    }
    _rootPage->NotifyUser("Draw Straight Line Change Picked", NotifyType::StatusMessage);
}


void Hot3dxRotoDraw::Scenario2_Normal::IDC_ARC_RADIUS_LINE_checkBox_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_ARC_RADIUS_LINE_checkBox->IsChecked->Value == true)
    {
        _rootPage->SetIfIfRightShiftKeySphereRadius(true);
        _rootPage->NotifyUser("Draw ARC Radius Line Change to true", NotifyType::StatusMessage);
    }
    else {
        _rootPage->SetIfIfRightShiftKeySphereRadius(false);
        _rootPage->NotifyUser("Draw ARC Radius Line Change to false", NotifyType::StatusMessage);
    }
}


void Hot3dxRotoDraw::Scenario2_Normal::SAVE_PTS_button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->CopyPointsXYAxisDXP();

}
void Hot3dxRotoDraw::Scenario2_Normal::PUT_COPY_PTS_button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->DrawPointsXYAxisDXP();
}


void Hot3dxRotoDraw::Scenario2_Normal::CLEAR_COPED_PTS_button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->ClearPointsXYAxisDXP();
}


void Hot3dxRotoDraw::Scenario2_Normal::CursorLockResetButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->DrawGridPicRectangleDXP();
}
