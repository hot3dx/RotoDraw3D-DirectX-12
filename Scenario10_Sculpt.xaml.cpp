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
		m_iptCnt = _rootPage->GetPointCount();
		m_igroupCnt = _rootPage->GetGroupCount();
		m_iptTotalCnt = static_cast<unsigned int>(_rootPage->GetPointsTotalCount());
		textBoxGroupCount->Text::set(ref new Platform::String(std::to_wstring(m_igroupCnt).c_str()));
		textBoxGroupCount->SetValue(textBoxGroupCount->TextProperty, textBoxGroupCount->Text);
		textBoxPointsPerGroup->Text::set(ref new Platform::String(std::to_wstring(m_iptCnt).c_str()));
		textBoxPointsPerGroup->SetValue(textBoxPointsPerGroup->TextProperty, textBoxPointsPerGroup->Text);
		textBoxPointCount->Text::set(ref new Platform::String(std::to_wstring(m_iptTotalCnt).c_str()));
		textBoxPointCount->SetValue(textBoxPointCount->TextProperty, textBoxPointCount->Text);

		slider_SelectedGroup->Value::set(0.0f);
		slider_SelectedGroup->SetValue(slider_SelectedGroup->ValueProperty, slider_SelectedGroup->Value);
		slider_SelectedGroup->Minimum::set(0.0f);
		slider_SelectedGroup->SetValue(slider_SelectedGroup->MinimumProperty, slider_SelectedGroup->Minimum);
		slider_SelectedGroup->Maximum::set(static_cast<double>(m_igroupCnt - 1));
		slider_SelectedGroup->SetValue(slider_SelectedGroup->MaximumProperty, slider_SelectedGroup->Maximum);

		slider_SelectedPoint->Value::set(0.0f);
		slider_SelectedPoint->SetValue(slider_SelectedPoint->ValueProperty, slider_SelectedPoint->Value);
		slider_SelectedPoint->Minimum::set(0.0f);
		slider_SelectedPoint->SetValue(slider_SelectedPoint->MinimumProperty, slider_SelectedPoint->Minimum);
		slider_SelectedPoint->Maximum::set(static_cast<double>(m_iptCnt - 1));
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

	textBoxGroupCount->Text::set(ref new Platform::String(std::to_wstring(m_igroupCnt).c_str()));
	textBoxGroupCount->SetValue(textBoxGroupCount->TextProperty, textBoxGroupCount->Text);
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxPointsPerGroup_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	textBoxPointsPerGroup->Text::set(ref new Platform::String(std::to_wstring(m_iptCnt).c_str()));
	textBoxPointsPerGroup->SetValue(textBoxPointsPerGroup->TextProperty, textBoxPointsPerGroup->Text);
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxPointCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	textBoxPointCount->Text::set(ref new Platform::String(std::to_wstring(m_iptTotalCnt).c_str()));
	textBoxPointCount->SetValue(textBoxPointCount->TextProperty, textBoxPointCount->Text);
}





void Hot3dxRotoDraw::Scenario10_Sculpt::X_VALUE_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	X_VALUE_VERTEX_EDIT->Text = ref new Platform::String(X_VALUE_VERTEX_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(X_VALUE_VERTEX_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	positionX += value;
	DirectX::XMVECTOR v = DirectX::XMVectorSet(positionX, positionY, positionZ, 0.0f);


}


void Hot3dxRotoDraw::Scenario10_Sculpt::Y_VALUE_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Y_VALUE_VERTEX_EDIT->Text = ref new Platform::String(Y_VALUE_VERTEX_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(Y_VALUE_VERTEX_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	positionY += value;
	DirectX::XMVECTOR v = DirectX::XMVectorSet(positionX, positionY, positionZ, 0.0f);
}


void Hot3dxRotoDraw::Scenario10_Sculpt::Z_VALUE_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Z_VALUE_VERTEX_EDIT->Text = ref new Platform::String(Z_VALUE_VERTEX_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(Z_VALUE_VERTEX_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	positionZ += value;
	DirectX::XMVECTOR v = DirectX::XMVectorSet(positionX, positionY, positionZ, 0.0f);
}

void  Hot3dxRotoDraw::Scenario10_Sculpt::SculptTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}


void  Hot3dxRotoDraw::Scenario10_Sculpt::SculptFilesTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}
/*
void  Hot3dxRotoDraw::Scenario10_Sculpt::GetObjectVertexIndexData(
	unsigned int pointCount,
	unsigned int groupCount,
	std::vector<DirectX::DXTKXAML12::VertexPositionColor> vertexes,
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
	std::vector<DirectX::DXTKXAML12::VertexPositionColor> vertexes,
	std::vector<uint16_t> indices,
	Platform::String^ mtlObjFilename,
	Platform::String^ nodeName,
	Platform::String^ effectName
)
{

}
*/
void Hot3dxRotoDraw::Scenario10_Sculpt::SelectedGroupSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	m_iselectedGroup = static_cast<unsigned int>(slider_SelectedGroup->Value::get());
	Platform::String^ s = ref new Platform::String(L"Group Selected: ");
	s = s->Concat(s, ref new Platform::String(std::to_wstring(static_cast<unsigned int>(m_iselectedGroup)).c_str()));
	m_selectedGroup->Text::set(s);
	m_selectedGroup->SetValue(m_selectedGroup->TextProperty, m_selectedGroup->Text);
	textBoxGroupSelected->Text::set(ref new Platform::String(std::to_wstring(static_cast<unsigned int>(m_iselectedGroup)).c_str()));
	textBoxGroupSelected->SetValue(textBoxGroupSelected->TextProperty, textBoxGroupSelected->Text);

}


void Hot3dxRotoDraw::Scenario10_Sculpt::SelectedPointSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	unsigned int selectedPoint = m_iselectedPoint = static_cast<unsigned int>(slider_SelectedPoint->Value::get());
	Platform::String^ s = ref new Platform::String(L"Point Selected: ");
	s = s->Concat(s, ref new Platform::String(std::to_wstring(static_cast<unsigned int>(selectedPoint)).c_str()));
	m_selectedPoint->Text = s;
	textBoxPointSelected->Text::set(ref new Platform::String(std::to_wstring(static_cast<unsigned int>(selectedPoint)).c_str()));
	textBoxPointSelected->SetValue(textBoxPointSelected->TextProperty, textBoxPointSelected->Text);


	//size_t grCnt = static_cast<size_t>(m_igroupCnt);

	point = m_iselectedPoint = _rootPage->GetVerticeColorIndexDXP(m_iselectedGroup, m_iselectedPoint);

	XMFLOAT3 vert = _rootPage->GetVerticeColorDXP(static_cast<unsigned int>(point));
	textBoxPointSelected->Text::set(ref new Platform::String(std::to_wstring(point).c_str()));
	if (point < m_iptTotalCnt)
	{
		//vert.x += positionX;
		//vert.y += positionY;
		//vert.z += positionZ;
		vert = PointScale(vert, 1.1f);
		_rootPage->SetVerticeColorDXP(static_cast<unsigned int>(point), vert);
		X_VERTEX_EDIT_Copy->Text::set(ref new Platform::String(std::to_wstring(vert.x).c_str()));
		X_VERTEX_EDIT_Copy->SetValue(X_VERTEX_EDIT_Copy->TextProperty, X_VERTEX_EDIT_Copy->Text);
		Y_VERTEX_EDIT_Copy->Text::set(ref new Platform::String(std::to_wstring(vert.x).c_str()));
		Y_VERTEX_EDIT_Copy->SetValue(Y_VERTEX_EDIT_Copy->TextProperty, Y_VERTEX_EDIT_Copy->Text);
		Z_VERTEX_EDIT_Copy->Text::set(ref new Platform::String(std::to_wstring(vert.x).c_str()));
		Z_VERTEX_EDIT_Copy->SetValue(Z_VERTEX_EDIT_Copy->TextProperty, Z_VERTEX_EDIT_Copy->Text);
		_rootPage->SetVerticeColorDXP(static_cast<unsigned int>(point), vert);
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
	textBoxGroupSelected->Text::get();
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

	//size_t grCnt = static_cast<size_t>(m_igroupCnt);

	point = _rootPage->GetVerticeColorIndexDXP(m_iselectedGroup, m_iselectedPoint);

	XMFLOAT3 vert = _rootPage->GetVerticeColorDXP(static_cast<unsigned int>(point));
	textBoxPointSelected->Text::set(ref new Platform::String(std::to_wstring(point).c_str()));
	if (point < m_igroupCnt)
	{
		//vert.x += positionX;
		//vert.y += positionY;
		//vert.z += positionZ;

		//_rootPage->SetVerticeColorDXP(m_iselectedGroup, m_iselectedPoint, vert);
	}
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxYPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Platform::String^ pox = textBoxSelectedPointY->Text::get();
	positionY = std::wcstof(pox->Data(), nullptr);
	textBoxSelectedPointY->Text::set(textBoxSelectedPointY->Text::get());
	textBoxSelectedPointY->SetValue(textBoxSelectedPointY->TextProperty, textBoxSelectedPointY->Text);

	//size_t grCnt = static_cast<size_t>(m_igroupCnt);

	point = _rootPage->GetVerticeColorIndexDXP(m_iselectedGroup, m_iselectedPoint);

	XMFLOAT3 vert = _rootPage->GetVerticeColorDXP(static_cast<unsigned int>(point));
	textBoxPointSelected->Text::set(ref new Platform::String(std::to_wstring(point).c_str()));
	if (point < m_igroupCnt)
	{
		//vert.x += positionX;
		//vert.y += positionY;
		//vert.z += positionZ;

		//_rootPage->SetVerticeColorDXP(m_iselectedGroup, m_iselectedPoint, vert);
	}
}

void Hot3dxRotoDraw::Scenario10_Sculpt::textBoxZPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Platform::String^ pox = textBoxSelectedPointZ->Text::get();
	positionZ = std::wcstof(pox->Data(), nullptr);
	textBoxSelectedPointZ->Text::set(textBoxSelectedPointZ->Text::get());
	textBoxSelectedPointZ->SetValue(textBoxSelectedPointZ->TextProperty, textBoxSelectedPointZ->Text);

	//size_t grCnt = static_cast<size_t>(m_igroupCnt);

	point = _rootPage->GetVerticeColorIndexDXP(m_iselectedGroup, m_iselectedPoint);

	XMFLOAT3 vert = _rootPage->GetVerticeColorDXP(static_cast<unsigned int>(point));
	textBoxPointSelected->Text::set(ref new Platform::String(std::to_wstring(point).c_str()));
	if (point < m_igroupCnt)
	{
		//vert.x += positionX;
		//vert.y += positionY;
		//vert.z += positionZ;

		//_rootPage->SetVerticeColorDXP(m_iselectedGroup, m_iselectedPoint, vert);
	}
}


void Hot3dxRotoDraw::Scenario10_Sculpt::slider_SculptX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	X_VALUE_VERTEX_EDIT->Text::set(ref new Platform::String(std::to_wstring(slider_SculptX->Value::get()).c_str()));
	positionX = static_cast<float>(slider_SculptX->Value::get());
}


void Hot3dxRotoDraw::Scenario10_Sculpt::slider_SculptY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	positionY = static_cast<float>(slider_SculptY->Value::get());
	Y_VALUE_VERTEX_EDIT->Text::set(ref new Platform::String(std::to_wstring(slider_SculptY->Value::get()).c_str()));
}


void Hot3dxRotoDraw::Scenario10_Sculpt::slider_SculptZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	Z_VALUE_VERTEX_EDIT->Text::set(ref new Platform::String(std::to_wstring(slider_SculptX->Value::get()).c_str()));
	positionZ = static_cast<float>(slider_SculptZ->Value::get());
}


void Hot3dxRotoDraw::Scenario10_Sculpt::SetButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	_rootPage->SetHot3dxDrawnObjectSculpt();
}


void Hot3dxRotoDraw::Scenario10_Sculpt::X_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	unsigned int vx = _rootPage->GetVerticeColorIndexDXP(m_iselectedGroup, m_iselectedPoint);
	DirectX::XMFLOAT3 verts = _rootPage->GetVerticeColorDXP(vx);
	X_VERTEX_EDIT_Copy->Text::set(ref new Platform::String(std::to_wstring(
		verts.x).c_str()));
}


void Hot3dxRotoDraw::Scenario10_Sculpt::Y_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	unsigned int vx = _rootPage->GetVerticeColorIndexDXP(m_iselectedGroup, m_iselectedPoint);
	DirectX::XMFLOAT3 verts = _rootPage->GetVerticeColorDXP(vx);
	Y_VERTEX_EDIT_Copy->Text::set(ref new Platform::String(std::to_wstring(
		verts.y).c_str()));

}


void Hot3dxRotoDraw::Scenario10_Sculpt::Z_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	unsigned int vx = _rootPage->GetVerticeColorIndexDXP(m_iselectedGroup, m_iselectedPoint);
	DirectX::XMFLOAT3 verts = _rootPage->GetVerticeColorDXP(vx);
	Z_VERTEX_EDIT_Copy->Text::set(ref new Platform::String(std::to_wstring(
		verts.z).c_str()));

}
