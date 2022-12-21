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
#include "Scenario9_Rotate.xaml.h"
#include "DirectXPage.xaml.h"

using namespace Hot3dxRotoDraw;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

Scenario9_Rotate^ Scenario9_Rotate::Current = nullptr;
Hot3dxRotoDraw::Scenario9_Rotate::Scenario9_Rotate() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();

	_rootPage->NotifyUser("Rotate x: 0.0f y:0.0f z:0.0f", NotifyType::StatusMessage);
	IDC_XROTATE_EDIT->Text = L"0.0f";
	IDC_YROTATE_EDIT->Text = L"0.0f";
	IDC_ZROTATE_EDIT->Text = L"0.0f";
}

Hot3dxRotoDraw::Scenario9_Rotate::~Scenario9_Rotate()
{

}

void Hot3dxRotoDraw::Scenario9_Rotate::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	Scenario9_Rotate::Current = this;
}

void Hot3dxRotoDraw::Scenario9_Rotate::IDC_SetRotate_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_XROTATE_EDIT->Text = L"0.0f";
	IDC_YROTATE_EDIT->Text = L"0.0f";
	IDC_ZROTATE_EDIT->Text = L"0.0f";
	XRotateSlider->Value = (0.0f);
	XRotateSlider->SetValue(XRotateSlider->ValueProperty, 0.0f);
	YRotateSlider->Value = (0.0f);
	YRotateSlider->SetValue(YRotateSlider->ValueProperty, 0.0f);
	ZRotateSlider->Value = (0.0f);
	ZRotateSlider->SetValue(ZRotateSlider->ValueProperty, 0.0f);
}

void Hot3dxRotoDraw::Scenario9_Rotate::IDC_X_ROTATE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	IDC_XROTATE_EDIT->Text = ref new Platform::String(IDC_XROTATE_EDIT->Text->Data());
	IDC_XROTATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	value *=  0.017453293005625408f;// old 0.03046174197867085f;
	_rootPage->Set_xRotateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario9_Rotate::IDC_Y_ROTATE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	IDC_YROTATE_EDIT->Text = ref new Platform::String(IDC_YROTATE_EDIT->Text->Data());
	IDC_YROTATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	value *=  0.017453293005625408f;
	_rootPage->Set_yRotateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario9_Rotate::IDC_Z_ROTATE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	IDC_ZROTATE_EDIT->Text = ref new Platform::String(IDC_ZROTATE_EDIT->Text->Data());
	IDC_ZROTATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	value *=  0.017453293005625408f;
	_rootPage->Set_zRotateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario9_Rotate::IDC_X_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_XROTATE_EDIT->Text = ref new Platform::String(IDC_XROTATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_XROTATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value *=  0.017453293005625408f;
	_rootPage->Set_xRotateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario9_Rotate::IDC_Y_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_YROTATE_EDIT->Text = ref new Platform::String(IDC_YROTATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_YROTATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value *=  0.017453293005625408f;
	_rootPage->Set_yRotateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario9_Rotate::IDC_Z_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_ZROTATE_EDIT->Text = ref new Platform::String(IDC_ZROTATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_ZROTATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value *=  0.017453293005625408f;
	_rootPage->Set_zRotateDrawnObject(value);
}