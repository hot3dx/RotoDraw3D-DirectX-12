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
    IDC_ROTATION_EDIT->Text = L"10";
    m_pointColor = { 255,125,125,0 };
    IDC_POINT_SPACE_STATIC->Text = "0.300000f";
    PointSpacingTextBox->Text = "0.300000f";
    PartialRotateTextBox->Text = "1.0f";
    vars->SetPartialRotateAngle(1.0f);
    vars->SetOpenOrClosedChecked(true);
    m_bAxisChecked = true;
    vars->SetAxisChecked(m_bAxisChecked);
    m_bTopOrLeftChecked = false;
    vars->SetTopOrLeftChecked(m_bTopOrLeftChecked);
    m_bBottomOrRightChecked = false;
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
    IDC_TOP_OR_LEFT_CHECKBOX->IsChecked = false;// (nullptr, false);
    IDC_BOTTOM_OR_RIGHT_CHECKBOX->IsChecked = false;
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


/*
void Hot3dxRotoDraw::Scenario2_Normal::IDC_SAVE_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    // Clear previous returned file name, if it exists, between iterations of this scenario
    //IDC_WELCOME_STATIC->Text = "";//was used as a sample but needs to be changed; A writer function needs to be called with
    //the file data to be saved.

    FileSavePicker^ savePicker = ref new FileSavePicker();
    savePicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;

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
