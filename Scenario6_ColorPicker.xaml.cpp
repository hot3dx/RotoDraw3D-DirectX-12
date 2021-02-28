#include "pch.h"
#include "Scenario6_ColorPicker.xaml.h"
#include "DirectXPage.xaml.h"

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

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

Hot3dxRotoDraw::Scenario6_ColorPicker::Scenario6_ColorPicker() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();
	
	ColorFrontRect->Fill = _rootPage->GetBrushFrontFaceColorDXP();
	ColorBackRect->Fill = _rootPage->GetBrushBackFaceColorDXP();
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
	//_rootPage->m_Scene2Vars->SetPointColor(_rootPage->GetBrushFrontFaceColorDXP()->Color);
}

Hot3dxRotoDraw::Scenario6_ColorPicker::~Scenario6_ColorPicker()
{
	
}

void Hot3dxRotoDraw::Scenario6_ColorPicker::ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
}

void Hot3dxRotoDraw::Scenario6_ColorPicker::ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
}


void Hot3dxRotoDraw::Scenario6_ColorPicker::frontColor_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	
	if (m_frontColor->IsChecked->Value == true)
	{
		m_backColor->IsChecked=false;
		SolidColorBrush^ brush = ref new SolidColorBrush(colorPicker->Color);
		ColorFrontRect->Fill = brush;
		_rootPage->SetBrushFrontFaceColorDXP(brush->Color);
		_rootPage->SetFrontColorDXP(brush->Color);
		_rootPage->m_Scene2Vars->SetPointColor(brush->Color);
	}
	

		_rootPage->NotifyUser("Front Face Color Picked", NotifyType::StatusMessage);
	
}


void Hot3dxRotoDraw::Scenario6_ColorPicker::backColor_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (m_backColor->IsChecked->Value == true)
	{
		m_frontColor->IsChecked = false;
		
		SolidColorBrush^ brush = ref new SolidColorBrush(colorPicker->Color);

		ColorBackRect->Fill = brush;
		_rootPage->SetBrushBackFaceColorDXP(brush->Color);
		_rootPage->SetBackColorDXP(brush->Color);
		_rootPage->m_Scene2Vars->SetPointColor(brush->Color);
	}
	_rootPage->NotifyUser("Back Face Color Picked", NotifyType::StatusMessage);

	

}
