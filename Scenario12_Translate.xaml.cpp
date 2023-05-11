//
// Scenario12_Translate.xaml.cpp
// Implementation of the Scenario12_Translate class
//

#include "pch.h"
#include "Scenario12_Translate.xaml.h"

using namespace Hot3dxRotoDraw;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238
Hot3dxRotoDraw::Scenario12_Translate::Scenario12_Translate() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();
	_rootPage->NotifyUser("Translate x: 0.0f y:0.0f z:0.0f", NotifyType::StatusMessage);
	IDC_XTRANSLATE_EDIT->Text = L"0.0f";
	IDC_YTRANSLATE_EDIT->Text = L"0.0f";
	IDC_ZTRANSLATE_EDIT->Text = L"0.0f";
}

Hot3dxRotoDraw::Scenario12_Translate::~Scenario12_Translate()
{
	
}

void Hot3dxRotoDraw::Scenario12_Translate::IDC_SetTranslate_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_XTRANSLATE_EDIT->Text = L"0.0f";
	IDC_YTRANSLATE_EDIT->Text = L"0.0f";
	IDC_ZTRANSLATE_EDIT->Text = L"0.0f";
	XTranslateSlider->Value::set(0);
	XTranslateSlider->SetValue(XTranslateSlider->ValueProperty, XTranslateSlider->Value);
	YTranslateSlider->Value::set(0);
	YTranslateSlider->SetValue(YTranslateSlider->ValueProperty, YTranslateSlider->Value);
	ZTranslateSlider->Value::set(0);
	ZTranslateSlider->SetValue(ZTranslateSlider->ValueProperty, ZTranslateSlider->Value);
}

void Hot3dxRotoDraw::Scenario12_Translate::IDC_SetTranslate_BUTTON2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTranslateLineValues();
}

void Hot3dxRotoDraw::Scenario12_Translate::IDC_X_TRANSLATE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	IDC_XTRANSLATE_EDIT->Text = ref new Platform::String(IDC_XTRANSLATE_EDIT->Text->Data());
	value += 0.005f;
	IDC_XTRANSLATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	//value += 0.005f;
	_rootPage->Set_xTranslateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario12_Translate::IDC_Y_TRANSLATE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	IDC_YTRANSLATE_EDIT->Text = ref new Platform::String(IDC_YTRANSLATE_EDIT->Text->Data());
	value += 0.005f;
	IDC_YTRANSLATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	//value *= 0.33f;
	_rootPage->Set_yTranslateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario12_Translate::IDC_Z_TRANSLATE_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	float value = static_cast<float>(e->NewValue);
	IDC_ZTRANSLATE_EDIT->Text = ref new Platform::String(IDC_ZTRANSLATE_EDIT->Text->Data());
	value += 0.005f;
	IDC_ZTRANSLATE_EDIT->Text = ref new Platform::String(std::to_wstring(value).c_str());
	//value *= 0.33f;
	_rootPage->Set_zTranslateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario12_Translate::IDC_X_TRANSLATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_XTRANSLATE_EDIT->Text = ref new Platform::String(IDC_XTRANSLATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_XTRANSLATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value += 0.005f;
	_rootPage->Set_xTranslateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario12_Translate::IDC_Y_TRANSLATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_YTRANSLATE_EDIT->Text = ref new Platform::String(IDC_YTRANSLATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_YTRANSLATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value += 0.005f;
	_rootPage->Set_yTranslateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario12_Translate::IDC_Z_TRANSLATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IDC_ZTRANSLATE_EDIT->Text = ref new Platform::String(IDC_ZTRANSLATE_EDIT->Text->Data());
	Platform::String^ str = ref new Platform::String(IDC_ZTRANSLATE_EDIT->Text->Data());
	float value = std::wcstof(str->Data(), nullptr);
	value += 0.005f;
	_rootPage->Set_zTranslateDrawnObject(value);
}

void Hot3dxRotoDraw::Scenario12_Translate::IDC_SetTranslate_BUTTON3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTranslateAllLinesValues();
}
