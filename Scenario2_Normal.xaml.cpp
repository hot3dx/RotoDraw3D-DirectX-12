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

Scenario2_Normal::Scenario2_Normal() : _rootPage(DirectXPage::Current)
{
    InitializeComponent();
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    _rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
    IDC_ROTATION_EDIT->Text = (L"70");
    m_pointColor = { 255,125,125,0 };
    IDC_POINT_SPACE_STATIC->Text = ("0.300000f");
    PointSpacingTextBox->Text = "0.300000f";
    PartialRotateTextBox->Text = "0.0f";
    vars->SetPartialRotateAngle(0.0f);
    vars->SetOpenOrClosedChecked(true);
    m_bAxisChecked = true;
    vars->SetAxisChecked(m_bAxisChecked);
    m_bTopOrLeftChecked = false;
    vars->SetTopOrLeftChecked(m_bTopOrLeftChecked);
    m_bBottomOrRightChecked = false;
    m_bBackgroundDrawChecked = false;
    vars->SetBottomOrRightChecked(false);
   Scenario2_Normal::Current = this;
   _rootPage->m_Scene2Vars->SetScenario2Page(this);
    
}
    


Scenario2_Normal::~Scenario2_Normal()
{
   
}

void Hot3dxRotoDraw::Scenario2_Normal::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
    Scenario2_Normal::Current = this;
    IDC_AXIS_CHECKBOX->IsChecked = m_bAxisChecked;
    IDC_TOP_OR_LEFT_CHECKBOX->IsChecked = m_bTopOrLeftChecked;
    IDC_BOTTOM_OR_RIGHT_CHECKBOX->IsChecked = m_bBottomOrRightChecked;
    IDC_BACKGROUND_DRAW_CHECKBOX->IsChecked = m_bBackgroundDrawChecked;
    _rootPage->m_Scene2Vars->SetScenario2Page(this);
}


void Scenario2_Normal::PointSpacingTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
   
    //const wchar_t* str = PointSpacingTextBox->Text->Data();
    IDC_POINT_SPACE_STATIC->Text = ref new Platform::String(PointSpacingTextBox->Text->Data());
    Platform::String^ str = ref new Platform::String(PointSpacingTextBox->Text->Data());
    _rootPage->SetPointSpacingDXP(std::wcstof(str->Data(), nullptr));
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_PARTIAL_ROTATE_SLIDER_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    float value = static_cast<float>(e->NewValue);
    //value; *= 0.01f;
    IDC_PARTIAL_ROTATE_STATIC->Text = ref new Platform::String(PartialRotateTextBox->Text->Data());
    PartialRotateTextBox->Text = ref new Platform::String(std::to_wstring(value).c_str());
    _rootPage->SetPartialRotateAngleDXP(value);
}

void Hot3dxRotoDraw::Scenario2_Normal::PartialRotateTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
    IDC_PARTIAL_ROTATE_STATIC->Text = ref new Platform::String(PartialRotateTextBox->Text->Data());
    Platform::String^ str = ref new Platform::String(PartialRotateTextBox->Text->Data());
    _rootPage->SetPartialRotateAngleDXP(std::wcstof(str->Data(), nullptr));
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_CLOSED_OR_OPEN_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_CLOSED_OR_OPEN_CHECKBOX->IsChecked->Value)
    {
        vars->SetOpenOrClosedChecked(true);
        _rootPage->NotifyUser("Closed Partial Object Picked", NotifyType::StatusMessage);
    }
    else {
        vars->SetOpenOrClosedChecked(false);
        _rootPage->NotifyUser("Open Partial Object Picked", NotifyType::StatusMessage);
    }
    //_rootPage->SetDoFrontFacesDXP(m_bDoFrontFaces);
   

    
}

void Scenario2_Normal::IDC_CLEAR_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    _rootPage->NotifyUser("New Page", NotifyType::StatusMessage);
    IDC_ROTATION_EDIT->Text = L"70";
    m_pointColor = { 255,125,125,0 };
    IDC_POINT_SPACE_STATIC->Text = "0.300000f";
    PointSpacingTextBox->Text = "0.300000f";
    PartialRotateTextBox->Text = "0.0f";
    m_bAxisChecked = true;
    vars->SetAxisChecked(m_bAxisChecked);
    m_bTopOrLeftChecked = false;
    vars->SetTopOrLeftChecked(m_bTopOrLeftChecked);
    m_bBottomOrRightChecked = false;
    vars->SetBottomOrRightChecked(false);
    m_bBackgroundDrawChecked = false;
    vars->SetGridPicChecked(false);
    _rootPage->IDC_CLEAR_BUTTON_Click(sender, e);

}

void Scenario2_Normal::IDC_SET_COLORS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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
        _rootPage->NotifyUser("Exterior Faces Picked: TRUE disabled", NotifyType::StatusMessage);
    }
    else {
        m_bDoFrontFaces = false;
        _rootPage->NotifyUser("Exterior Faces Picked: FALSE disabled", NotifyType::StatusMessage);
    }
    _rootPage->SetDoFrontFacesDXP(m_bDoFrontFaces);
    vars->SetDoFrontFaces(m_bDoFrontFaces);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_INTERIOR_FACES_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_INTERIOR_FACES_CHECKBOX->IsChecked->Value)
    {
        m_bDoBackFaces = true;
        _rootPage->NotifyUser("Interior Faces Picked: TRUE disabled", NotifyType::StatusMessage);
    }
    else {
        m_bDoBackFaces = false;
        _rootPage->NotifyUser("Interior Faces Picked: FALSE disabled", NotifyType::StatusMessage);
    }
    _rootPage->SetDoBackFacesDXP(m_bDoBackFaces);
    vars->SetDoBackFaces(m_bDoBackFaces);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_FIRST_TO_LAST_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_FIRST_TO_LAST_CHECKBOX->IsChecked->Value)
    {
        m_bFirstToLast = true;
        _rootPage->NotifyUser("First To Last Picked: TRUE disabled", NotifyType::StatusMessage);
    }
    else {
        m_bFirstToLast = false;
        _rootPage->NotifyUser("First To Last Picked: FALSE disabled", NotifyType::StatusMessage);
    }
    _rootPage->SetFirstToLastDXP(m_bFirstToLast);
    vars->SetFirstToLast(m_bFirstToLast);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_AXIS_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    //Platform::IBox<bool>^ tf = IDC_AXIS_CHECKBOX->IsChecked->Value;
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_AXIS_CHECKBOX->IsChecked->Value)
    {
        m_bAxisChecked = true;
        _rootPage->NotifyUser("Axis Change Picked: Y axis drawing vertical", NotifyType::StatusMessage);
    }
    else {
        m_bAxisChecked = false;
        _rootPage->NotifyUser("Axis Change Picked: X axis drawing horizontal", NotifyType::StatusMessage);
    }
    _rootPage->SetAxisCheckedDXP(m_bAxisChecked);
    vars->SetAxisChecked(m_bAxisChecked);
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_TOP_OR_LEFT_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (_rootPage->GetPointsCountDXP() > 1)
    {
        Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
        if (IDC_TOP_OR_LEFT_CHECKBOX->IsChecked->Value)
        {
            m_bTopOrLeftChecked = true;
            _rootPage->NotifyUser("Top or Left Change Picked: TRUE", NotifyType::StatusMessage);
        }
        else {
            m_bTopOrLeftChecked = false;
            _rootPage->NotifyUser("Top or Left Change Picked: FALSE", NotifyType::StatusMessage);
        }
        _rootPage->SetTopOrLeftCheckedDXP(m_bTopOrLeftChecked);
        vars->SetTopOrLeftChecked(m_bTopOrLeftChecked);
       
    }
    else
    {
        _rootPage->NotifyUser("Error: Point Count Zero;\n Draw At Least Two Points First!", NotifyType::StatusMessage);
    }


}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_BOTTOM_OR_RIGHT_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (_rootPage->GetPointsCountDXP() > 1)
    {
        Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
        if (IDC_BOTTOM_OR_RIGHT_CHECKBOX->IsChecked->Value)
        {
            m_bBottomOrRightChecked = true;
            _rootPage->NotifyUser("Bottom or Right Change Picked: TRUE", NotifyType::StatusMessage);
        }
        else {
            m_bBottomOrRightChecked = false;
            _rootPage->NotifyUser("Bottom or Right Change Picked: FALSE", NotifyType::StatusMessage);
        }
        _rootPage->SetBottomOrRightCheckedDXP(m_bBottomOrRightChecked);
        vars->SetBottomOrRightChecked(m_bBottomOrRightChecked);
    }
    else
    {
        _rootPage->NotifyUser("Error: Point Count Zero;\n Draw At Least Two Points First!", NotifyType::StatusMessage);
    }
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_BACKGROUND_DRAW_CHECKBOX_checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Scenario2Vars^ vars = _rootPage->m_Scene2Vars;
    if (IDC_BACKGROUND_DRAW_CHECKBOX->IsChecked->Value)
    {
        m_bBackgroundDrawChecked = true;
        _rootPage->NotifyUser("Background Draw Photo Picked: TRUE", NotifyType::StatusMessage);
    }
    else {
        m_bBackgroundDrawChecked = false;
        _rootPage->NotifyUser("Background Draw Photo Picked: FALSE", NotifyType::StatusMessage);
    }
    _rootPage->SetGridPicCheckedDXP(m_bBackgroundDrawChecked);
    vars->SetGridPicChecked(m_bBackgroundDrawChecked);
    _rootPage->SetDDS_WIC_FLAGGridPicFileDXP(m_bBackgroundDrawChecked);
}

VOID Hot3dxRotoDraw::Scenario2_Normal::IDC_ROTATION_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if(std::wcstof(IDC_ROTATION_EDIT->Text->Data(), nullptr) < 1
        || std::wcstof(IDC_ROTATION_EDIT->Text->Data(), nullptr) > 180)
    {  
        _rootPage->NotifyUser("Error: Angle out of range", NotifyType::StatusMessage);
    }
    else
    {
        _rootPage->SetPointDrawGroupAngleDXP(std::wcstof(IDC_ROTATION_EDIT->Text->Data(), nullptr));
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
        if (_rootPage->GetPointsCountDXP() > 0)
        {
             _rootPage->CopyFlipPointsXAxisDXP();
        }
        else
        {
            _rootPage->NotifyUser("Error: Point Count Zero;\n Draw At Least One Points First!", NotifyType::StatusMessage);
        }
    }
    else
    {
            if (_rootPage->GetPointsCountDXP() > 0)
            {
                _rootPage->CopyFlipPointsYAxisDXP();
            }
            else
            {
                _rootPage->NotifyUser("Error: Point Count Zero;\n Draw At Least One Points First!", NotifyType::StatusMessage);
            }
    }
}

void Hot3dxRotoDraw::Scenario2_Normal::IDC_SET_POINTS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (_rootPage->GetPointsCountDXP() > 1)
    {
        _rootPage->SET_POINTS_BUTTON_Click(sender, e);
    }
    else
    {
        _rootPage->NotifyUser("Error: Point Count Zero;\n Draw At Least Two Points First!", NotifyType::StatusMessage);
    }
}