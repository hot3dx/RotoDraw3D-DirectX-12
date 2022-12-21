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
#include "Scenario4_MySettings.xaml.h"
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

Hot3dxRotoDraw::Scenario4_MySettings::Scenario4_MySettings() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();

	_rootPage->NotifyUser("Scale x: 1.0f y:1.0f z:1.0f", NotifyType::StatusMessage);
	IDC_ALLSCALE_EDIT->Text = L"1.0f";
	IDC_XSCALE_EDIT->Text = L"1.0f";
	IDC_YSCALE_EDIT->Text = L"1.0f";
	IDC_ZSCALE_EDIT->Text = L"1.0f";
}

Hot3dxRotoDraw::Scenario4_MySettings::~Scenario4_MySettings()
{
	
}

void Hot3dxRotoDraw::Scenario4_MySettings::IDC_SetScale_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_ALLSCALE_EDIT->Text = L"1.0f";
	IDC_XSCALE_EDIT->Text = L"1.0f";
	IDC_YSCALE_EDIT->Text = L"1.0f";
	IDC_ZSCALE_EDIT->Text = L"1.0f";
	AllScaleSlider->Value = (1.0f);
	AllScaleSlider->SetValue(AllScaleSlider->ValueProperty, 1.0f);
	XScaleSlider->Value = (1.0f);
	XScaleSlider->SetValue(XScaleSlider->ValueProperty, 1.0f);
	YScaleSlider->Value = (1.0f);
	YScaleSlider->SetValue(YScaleSlider->ValueProperty, 1.0f);
	ZScaleSlider->Value = (1.0f);
	ZScaleSlider->SetValue(ZScaleSlider->ValueProperty, 1.0f);
	
}

void Hot3dxRotoDraw::Scenario4_MySettings::IDC_ALL_SCALE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	//value *= 0.01f;
	IDC_ALLSCALE_EDIT->Text = ref new Platform::String(IDC_ALLSCALE_EDIT->Text->Data());
	IDC_ALLSCALE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	_rootPage->Set_xScaleDrawnObject(value);
	_rootPage->Set_yScaleDrawnObject(value);
	_rootPage->Set_zScaleDrawnObject(value);
	DependencyObject^ dp;
//	XScaleSlider->SetValue(nullptr, static_cast<float>(e->NewValue));
//	YScaleSlider->SetValue(nullptr, static_cast<float>(e->NewValue));
//	ZScaleSlider->SetValue(nullptr, static_cast<float>(e->NewValue));
}

void Hot3dxRotoDraw::Scenario4_MySettings::IDC_X_SCALE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	//value *= 0.01f;
	IDC_XSCALE_EDIT->Text = ref new Platform::String(IDC_XSCALE_EDIT->Text->Data());
	IDC_XSCALE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	_rootPage->Set_xScaleDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario4_MySettings::IDC_Y_SCALE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	//value *= 0.01f;
	IDC_YSCALE_EDIT->Text = ref new Platform::String(IDC_YSCALE_EDIT->Text->Data());
	IDC_YSCALE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	_rootPage->Set_yScaleDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario4_MySettings::IDC_Z_SCALE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	//value *= 0.01f;
	IDC_ZSCALE_EDIT->Text = ref new Platform::String(IDC_ZSCALE_EDIT->Text->Data());
	IDC_ZSCALE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	_rootPage->Set_zScaleDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario4_MySettings::IDC_ALL_SCALE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_ALLSCALE_EDIT->Text = ref new Platform::String(IDC_ALLSCALE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_ALLSCALE_EDIT->Text->Data());
	_rootPage->Set_xScaleDrawnObject(std::wcstof(str->Data(), nullptr));
	_rootPage->Set_yScaleDrawnObject(std::wcstof(str->Data(), nullptr));
	_rootPage->Set_zScaleDrawnObject(std::wcstof(str->Data(), nullptr));
}

void Hot3dxRotoDraw::Scenario4_MySettings::IDC_X_SCALE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_XSCALE_EDIT->Text = ref new Platform::String(IDC_XSCALE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_XSCALE_EDIT->Text->Data());
	_rootPage->Set_xScaleDrawnObject(std::wcstof(str->Data(), nullptr));
	}

void Hot3dxRotoDraw::Scenario4_MySettings::IDC_Y_SCALE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_YSCALE_EDIT->Text = ref new Platform::String(IDC_YSCALE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_YSCALE_EDIT->Text->Data());
	_rootPage->Set_yScaleDrawnObject(std::wcstof(str->Data(), nullptr));
}

void Hot3dxRotoDraw::Scenario4_MySettings::IDC_Z_SCALE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_ZSCALE_EDIT->Text = ref new Platform::String(IDC_ZSCALE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_ZSCALE_EDIT->Text->Data());
	_rootPage->Set_zScaleDrawnObject(std::wcstof(str->Data(), nullptr));
}