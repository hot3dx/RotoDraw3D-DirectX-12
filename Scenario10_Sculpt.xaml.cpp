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

Scenario10_Sculpt^ Scenario10_Sculpt::Current = nullptr;
Hot3dxRotoDraw::Scenario10_Sculpt::Scenario10_Sculpt() : _rootPage(DirectXPage::Current),
m_bSculptEnabled(false),
m_iselectedPoint(0),
m_iselectedGroup(0),
positionX(0.0f),
positionY(0.0f),
positionZ(0.0f)
{
    InitializeComponent();
	checkBoxSculptBool->IsChecked::set(false);
	checkBoxSculptBool->SetValue(checkBoxSculptBool->IsCheckedProperty, checkBoxSculptBool->IsChecked);
	checkBoxSculptXBool->IsChecked::set(false);
	checkBoxSculptXBool->SetValue(checkBoxSculptXBool->IsCheckedProperty, checkBoxSculptXBool->IsChecked);

	checkBoxSculptYBool->IsChecked::set(false);
	checkBoxSculptYBool->SetValue(checkBoxSculptYBool->IsCheckedProperty, checkBoxSculptYBool->IsChecked);

	checkBoxSculptZBool->IsChecked::set(false);
	checkBoxSculptZBool->SetValue(checkBoxSculptZBool->IsCheckedProperty, checkBoxSculptZBool->IsChecked);

	textBoxSelectedPointX->Text::set(ref new Platform::String(std::to_wstring(positionX).c_str()));
	textBoxSelectedPointX->SetValue(textBoxSelectedPointX->TextProperty, textBoxSelectedPointX->Text);
	textBoxSelectedPointY->Text::set(ref new Platform::String(std::to_wstring(positionY).c_str()));
	textBoxSelectedPointY->SetValue(textBoxSelectedPointY->TextProperty, textBoxSelectedPointX->Text);

    _rootPage->NotifyUser("Ready", NotifyType::StatusMessage);

}

Hot3dxRotoDraw::Scenario10_Sculpt::~Scenario10_Sculpt()
{

}

void Hot3dxRotoDraw::Scenario10_Sculpt::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	Scenario10_Sculpt::Current = this;
}

/*******************************************************/
/*  Enables or Disables Sculpting - movement of points */
/*******************************************************/
void Hot3dxRotoDraw::Scenario10_Sculpt::checkBoxSculptBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	bool checked = checkBoxSculptBool->IsChecked->Value;
	if (checked == false)
	{
		checkBoxSculptBool->IsChecked::set(false);
		m_bSculptEnabled = false;
		
	}
	else
	{
		checkBoxSculptBool->IsChecked::set(true);
		m_bSculptEnabled = true;
		_rootPage->GetPointCount();
		_rootPage->GetGroupCount();
		textBoxGroupCount->Text::set(ref new Platform::String(std::to_wstring(_rootPage->GetGroupCount()).c_str()));
		textBoxGroupCount->SetValue(textBoxGroupCount->TextProperty, textBoxGroupCount->Text);
		textBoxPointsPerGroup->Text::set(ref new Platform::String(std::to_wstring(_rootPage->GetPointCount()).c_str()));
		textBoxPointsPerGroup->SetValue(textBoxPointsPerGroup->TextProperty, textBoxPointsPerGroup->Text);
		textBoxPointCount->Text::set(ref new Platform::String(std::to_wstring(_rootPage->GetPointCount()).c_str()));
		textBoxPointCount->SetValue(textBoxPointCount->TextProperty, textBoxPointCount->Text);

		slider_SelectedGroup->Value::set(0.0f);
		slider_SelectedGroup->SetValue(slider_SelectedGroup->ValueProperty, slider_SelectedGroup->Value);
		slider_SelectedGroup->Minimum::set(0.0f);
		slider_SelectedGroup->SetValue(slider_SelectedGroup->MinimumProperty, slider_SelectedGroup->Minimum); 
		slider_SelectedGroup->Maximum::set(static_cast<double>(_rootPage->GetGroupCount()-1));
		slider_SelectedGroup->SetValue(slider_SelectedGroup->MaximumProperty, slider_SelectedGroup->Maximum);
		
		slider_SelectedGroup->Value::set(0.0f);
		slider_SelectedGroup->SetValue(slider_SelectedGroup->ValueProperty, slider_SelectedGroup->Value);
		slider_SelectedGroup->Minimum::set(0.0f);
		slider_SelectedGroup->SetValue(slider_SelectedGroup->MinimumProperty, slider_SelectedGroup->Minimum); 
		slider_SelectedPoint->Maximum::set(static_cast<double>(_rootPage->GetPointCount() - 1));
		slider_SelectedPoint->SetValue(slider_SelectedPoint->MaximumProperty, slider_SelectedPoint->Maximum);

	}
	checkBoxSculptBool->SetValue(checkBoxSculptBool->IsCheckedProperty, checkBoxSculptBool->IsChecked);
	
}

void Hot3dxRotoDraw::Scenario10_Sculpt::checkBoxSculptXBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	bool checked = checkBoxSculptXBool->IsChecked->Value;
	if (checked == false)
	{
		checkBoxSculptXBool->IsChecked::set(false);

	}
	else
	{
		checkBoxSculptXBool->IsChecked::set(true);
	}
	checkBoxSculptXBool->SetValue(checkBoxSculptXBool->IsCheckedProperty, checkBoxSculptXBool->IsChecked);
}

void Hot3dxRotoDraw::Scenario10_Sculpt::checkBoxSculptYBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	bool checked = checkBoxSculptYBool->IsChecked->Value;
	if (checked == false)
	{
		checkBoxSculptYBool->IsChecked::set(false);

	}
	else
	{
		checkBoxSculptYBool->IsChecked::set(true);
	}
	checkBoxSculptYBool->SetValue(checkBoxSculptYBool->IsCheckedProperty, checkBoxSculptYBool->IsChecked);
}

void Hot3dxRotoDraw::Scenario10_Sculpt::checkBoxSculptZBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	bool checked = checkBoxSculptZBool->IsChecked->Value;
	if (checked == false)
	{
		checkBoxSculptZBool->IsChecked::set(false);

	}
	else
	{
		checkBoxSculptZBool->IsChecked::set(true);
	}
	checkBoxSculptZBool->SetValue(checkBoxSculptZBool->IsCheckedProperty, checkBoxSculptZBool->IsChecked);
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxGroupCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//_rootPage->GetGroupCount()->
	textBoxGroupCount->Text::set(ref new Platform::String(std::to_wstring(_rootPage->GetGroupCount()).c_str()));
	textBoxGroupCount->SetValue(textBoxGroupCount->TextProperty, textBoxGroupCount->Text);
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxPointsPerGroup_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	textBoxPointsPerGroup->Text::set(ref new Platform::String(std::to_wstring(_rootPage->GetPointCount()).c_str()));
	textBoxPointsPerGroup->SetValue(textBoxPointsPerGroup->TextProperty, textBoxPointsPerGroup->Text);
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxPointCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	textBoxPointCount->Text::set(ref new Platform::String(std::to_wstring(_rootPage->GetPointCount()).c_str()));
	textBoxPointCount->SetValue(textBoxPointCount->TextProperty, textBoxPointCount->Text);
}




void Hot3dxRotoDraw::Scenario10_Sculpt::ROTATE_X_GridCam_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	GRIDCAM_XROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_XROTATE_EDIT->Text->Data());
	GRIDCAM_XROTATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	value *=  0.017453293005625408f;
	//_rootPage->Set_xRotateGridCam(value);

	//_rootPage->SetXRotationScenario10(value);
	//RotatePitchSquid(value);

}

void Hot3dxRotoDraw::Scenario10_Sculpt::ROTATE_Y_GridCam_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	GRIDCAM_YROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_YROTATE_EDIT->Text->Data());
	GRIDCAM_YROTATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	value *=  0.017453293005625408f;
	//_rootPage->Set_yRotateGridCam(value);

	//_rootPage->SetRotatePitch(value);
	//XMStoreFloat4x4(&_rootPage->dxpGetWorld4X4(), DirectX::XMMatrixRotationX(float(_rootPage->dxpGetTimer() * value)));
	//_rootPage->SetYRotationScenario10(value);
	//RotatePitchSquid(value);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::ROTATE_Z_GridCam_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	GRIDCAM_ZROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_ZROTATE_EDIT->Text->Data());
	GRIDCAM_ZROTATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	value *=  0.017453293005625408f;
	//_rootPage->Set_zRotateGridCam(value);
	//_rootPage->SetYRotationScenario10(value);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::GRIDCAM_X_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	GRIDCAM_XROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_XROTATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(GRIDCAM_XROTATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value *=  0.017453293005625408f;
	_rootPage->Set_xRotateGridCam(value);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::GRIDCAM_Y_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	GRIDCAM_YROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_YROTATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(GRIDCAM_YROTATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value *=  0.017453293005625408f;
	_rootPage->Set_yRotateGridCam(value);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::GRIDCAM_Z_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	GRIDCAM_ZROTATE_EDIT->Text = ref new Platform::String(GRIDCAM_ZROTATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(GRIDCAM_ZROTATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value *=  0.017453293005625408f;
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

void  Hot3dxRotoDraw::Scenario10_Sculpt::SetObjectVertexIndexData(
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

void Hot3dxRotoDraw::Scenario10_Sculpt::SelectedGroupSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	unsigned int selectedGroup = m_iselectedGroup = static_cast<unsigned int>(slider_SelectedGroup->Value::get());
	Platform::String^ s = ref new Platform::String(L"Group Selected: ");
	s = s->Concat(s, ref new Platform::String(std::to_wstring(static_cast<unsigned int>(selectedGroup)).c_str()));
	m_selectedGroup->Text::set(s);
	m_selectedGroup->SetValue(m_selectedGroup->TextProperty, m_selectedGroup->Text);
	textBoxGroupSelected->Text::set(ref new Platform::String(std::to_wstring(static_cast<unsigned int>(selectedGroup)).c_str()));
	textBoxGroupSelected->SetValue(textBoxGroupSelected->TextProperty, textBoxGroupSelected->Text);

	std::vector<Hot3dxRotoDraw::PtGroups^> ptsLists(_rootPage->GetGroupCount());
	size_t grCnt = static_cast<size_t>(_rootPage->GetGroupCount());
	for (size_t i = 0; i < grCnt; i++)
	{
		Hot3dxRotoDraw::PtGroups^ ptList = ref new Hot3dxRotoDraw::PtGroups();
		Platform::Array<uint16_t>^ list = _rootPage->GetPtGroupListDXP(i);
		for (unsigned int j = 0; j < static_cast<unsigned int>(grCnt); j++)
		{
			ptList->SetPtList(j, list->get(j));
		}
		ptsLists.push_back(ptList);
	}

	uint16_t pointNum = ptsLists.at(selectedGroup)->GetListPt(m_iselectedPoint);

	point = 0;

	if (selectedGroup == 0)
	{
		point = pointNum;
	}
	else
	{
		point = ((static_cast<size_t>(selectedGroup) + 1) * (_rootPage->GetPointCount())) + pointNum;
	}
	std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> verts = _rootPage->GetVertexesDXP();
	
	if (point < verts.size())
	{
		positionPoint.position = verts.at(point).position;
		positionX = positionPoint.position.x;
		positionY = positionPoint.position.y;
		positionZ = positionPoint.position.z;
	}


}


void Hot3dxRotoDraw::Scenario10_Sculpt::SelectedPointSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	unsigned int selectedPoint = m_iselectedPoint = static_cast<unsigned int>(slider_SelectedPoint->Value::get());
	Platform::String^ s = ref new Platform::String(L"Point Selected: ");
	s = s->Concat(s, ref new Platform::String(std::to_wstring(static_cast<unsigned int>(selectedPoint)).c_str()));
	m_selectedPoint->Text = s;
	textBoxPointSelected->Text::set(ref new Platform::String(std::to_wstring(static_cast<unsigned int>(selectedPoint)).c_str()));
	textBoxPointSelected->SetValue(textBoxPointSelected->TextProperty, textBoxPointSelected->Text);

	std::vector<Hot3dxRotoDraw::PtGroups^> ptsLists(_rootPage->GetGroupCount());
	size_t grCnt = static_cast<size_t>(_rootPage->GetGroupCount());
	for (size_t i = 0; i < grCnt; i++)
	{
		Hot3dxRotoDraw::PtGroups^ ptList = ref new Hot3dxRotoDraw::PtGroups();
		Platform::Array<uint16_t>^ list	= _rootPage->GetPtGroupListDXP(i);
		for (unsigned int j = 0; j < static_cast<unsigned int>(grCnt); j++)
		{
			ptList->SetPtList(j, list->get(j));
		}
		ptsLists.push_back(ptList);
	}

	ptsLists.at(m_iselectedGroup)->GetListPt(m_iselectedPoint);

	uint16_t pointNum = ptsLists.at(m_iselectedGroup)->GetListPt(m_iselectedPoint);

	point = 0;

	if (m_iselectedGroup == 0)
	{
		point = pointNum;
	}
	else
	{
		point = ((static_cast<size_t>(m_iselectedGroup) + 1) * (_rootPage->GetPointCount())) + pointNum;
	}
	std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> verts = _rootPage->GetVertexesDXP();

	if (point < verts.size())
	{
		positionPoint.position = verts.at(point).position;
		positionX = positionPoint.position.x;
		positionY = positionPoint.position.y;
		positionZ = positionPoint.position.z;
	}

	textBoxSelectedPointX->Text::set(ref new Platform::String(std::to_wstring(positionX).c_str()));
	textBoxSelectedPointX->SetValue(textBoxSelectedPointX->TextProperty, textBoxSelectedPointX->Text);
	textBoxSelectedPointY->Text::set(ref new Platform::String(std::to_wstring(positionY).c_str()));
	textBoxSelectedPointY->SetValue(textBoxSelectedPointY->TextProperty, textBoxSelectedPointY->Text);
	textBoxSelectedPointZ->Text::set(ref new Platform::String(std::to_wstring(positionZ).c_str()));
	textBoxSelectedPointZ->SetValue(textBoxSelectedPointZ->TextProperty, textBoxSelectedPointZ->Text);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxSelectedGroup_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	textBoxGroupSelected;
}


void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxSelectedPoint_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	textBoxPointSelected;
}


void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxSelectedPointCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	textBoxSelectedPointCount;
}


void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxXPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Platform::String^ pox = textBoxSelectedPointX->Text::get();
	positionX = std::wcstof(pox->Data(), nullptr); 
	textBoxSelectedPointX->Text::set(textBoxSelectedPointX->Text::get());
	textBoxSelectedPointX->SetValue(textBoxSelectedPointX->TextProperty, textBoxSelectedPointX->Text);

	std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> verts = _rootPage->GetVertexesDXP();

	if (point < verts.size())
	{
		 verts.at(point).position.x = positionX;
		
	}
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxYPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Platform::String^ pox = textBoxSelectedPointY->Text::get();
	positionY = std::wcstof(pox->Data(), nullptr); 
	textBoxSelectedPointY->Text::set(textBoxSelectedPointY->Text::get());
	textBoxSelectedPointY->SetValue(textBoxSelectedPointY->TextProperty, textBoxSelectedPointY->Text);

	std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> verts = _rootPage->GetVertexesDXP();

	if (point < verts.size())
	{
		verts.at(point).position.y = positionY;
	}
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxZPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Platform::String^ pox = textBoxSelectedPointZ->Text::get();
	positionZ = std::wcstof(pox->Data(), nullptr);
	textBoxSelectedPointZ->Text::set(textBoxSelectedPointZ->Text::get());
	textBoxSelectedPointZ->SetValue(textBoxSelectedPointZ->TextProperty, textBoxSelectedPointZ->Text);

	std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> verts = _rootPage->GetVertexesDXP();

	if (point < verts.size())
	{
		verts.at(point).position.z = positionZ;
	}
}
