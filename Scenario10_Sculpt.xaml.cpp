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
#include "Scenario10_Sculpt.xaml.h"

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


Hot3dxRotoDraw::Scenario10_Sculpt::Scenario10_Sculpt() : _rootPage(DirectXPage::Current)
{
    InitializeComponent();
    _rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);

}

Hot3dxRotoDraw::Scenario10_Sculpt::~Scenario10_Sculpt()
{

}

/*******************************************************/
/*  Enables or Disables Sculpting - movement of points */
/*******************************************************/
void Hot3dxRotoDraw::Scenario10_Sculpt::checkBoxSculptBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario10_Sculpt::checkBoxSculptXBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario10_Sculpt::checkBoxSculptYBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario10_Sculpt::checkBoxSculptZBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxGroupCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxPointsPerGroup_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxPointCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//throw ref new Platform::NotImplementedException();
}




void Hot3dxRotoDraw::Scenario10_Sculpt::ROTATE_X_GridCam_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	GRIDCAM_XROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_XROTATE_EDIT->Text->Data());
	GRIDCAM_XROTATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	value *= 0.03046174197867085f;
	//_rootPage->Set_xRotateGridCam(value);

	_rootPage->SetXRotationScenario10(value);
	//RotatePitchSquid(value);

}

void Hot3dxRotoDraw::Scenario10_Sculpt::ROTATE_Y_GridCam_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	GRIDCAM_YROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_YROTATE_EDIT->Text->Data());
	GRIDCAM_YROTATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	value *= 0.03046174197867085f;
	//_rootPage->Set_yRotateGridCam(value);

	//_rootPage->SetRotatePitch(value);
	//XMStoreFloat4x4(&_rootPage->dxpGetWorld4X4(), DirectX::XMMatrixRotationX(float(_rootPage->dxpGetTimer() * value)));
	_rootPage->SetYRotationScenario10(value);
	//RotatePitchSquid(value);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::ROTATE_Z_GridCam_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	GRIDCAM_ZROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_ZROTATE_EDIT->Text->Data());
	GRIDCAM_ZROTATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	value *= 0.03046174197867085f;
	_rootPage->Set_zRotateGridCam(value);
	_rootPage->SetYRotationScenario10(value);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::GRIDCAM_X_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	GRIDCAM_XROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_XROTATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(GRIDCAM_XROTATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value *= 0.03046174197867085f;
	_rootPage->Set_xRotateGridCam(value);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::GRIDCAM_Y_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	GRIDCAM_YROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_YROTATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(GRIDCAM_YROTATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value *= 0.03046174197867085f;
	_rootPage->Set_yRotateGridCam(value);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::GRIDCAM_Z_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	GRIDCAM_ZROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_ZROTATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(GRIDCAM_ZROTATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value *= 0.03046174197867085f;
	_rootPage->Set_zRotateGridCam(value);
}

void  Hot3dxRotoDraw::Scenario10_Sculpt::SculptTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}


void  Hot3dxRotoDraw::Scenario10_Sculpt::SculptFilesTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}

void  Hot3dxRotoDraw::Scenario10_Sculpt::GetObjectVertexIndexData(
	unsigned int pointCount,
	unsigned int groupCount,
	std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> vertexes,
	std::vector<uint16_t> indices,
	Platform::String^ mtlObjFilename,
	Platform::String^ nodeName,
	Platform::String^ effectName
)
{

}
