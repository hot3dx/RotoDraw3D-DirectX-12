#include "pch.h"
#include "Scenario13_ScreenGrab.xaml.h"
#include "Content\Hot3dxRotoDrawVariables.h"
#include "Common\DeviceResources.h"
#include <ScreenGrabXaml12.h>
#include "App.xaml.h"

using namespace Hot3dxRotoDraw;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace concurrency;



Scenario13_ScreenGrab^ Scenario13_ScreenGrab::Current = nullptr;


Hot3dxRotoDraw::Scenario13_ScreenGrab::Scenario13_ScreenGrab() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();

	Scenario13Vars^ vars = _rootPage->m_Scene13Vars;
	Scenario13_ScreenGrab::Current = this;
	_rootPage->m_Scene13Vars->SetScenario13Page(this);
	IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked::set(false);
	IDC_SCREENGRAB_DDS_CHECKBOX->SetValue(IDC_SCREENGRAB_DDS_CHECKBOX->IsCheckedProperty,
		IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked);

	IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked::set(false);
	IDC_SCREENGRAB_BMP_CHECKBOX->SetValue(IDC_SCREENGRAB_BMP_CHECKBOX->IsCheckedProperty,
		IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked);

	IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked::set(false);
	IDC_SCREENGRAB_PNG_CHECKBOX->SetValue(IDC_SCREENGRAB_PNG_CHECKBOX->IsCheckedProperty,
		IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked);

	//IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked::set(false);
	//IDC_SCREENGRAB_ICO_CHECKBOX->SetValue(IDC_SCREENGRAB_ICO_CHECKBOX->IsCheckedProperty,
	//	IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked);

	IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked::set(false);
	IDC_SCREENGRAB_JPG_CHECKBOX->SetValue(IDC_SCREENGRAB_JPG_CHECKBOX->IsCheckedProperty,
		IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked);

	IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked::set(false);
	IDC_SCREENGRAB_TIFF_CHECKBOX->SetValue(IDC_SCREENGRAB_TIFF_CHECKBOX->IsCheckedProperty,
		IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked);

	IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked::set(false);
	IDC_SCREENGRAB_GIF_CHECKBOX->SetValue(IDC_SCREENGRAB_GIF_CHECKBOX->IsCheckedProperty,
		IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked);

	//IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked::set(false);
	//IDC_SCREENGRAB_WMP_CHECKBOX->SetValue(IDC_SCREENGRAB_WMP_CHECKBOX->IsCheckedProperty,
	//	IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked);
	vars->SetScreenGrabDDSChecked(false);
	vars->SetScreenGrabBMPChecked(false);
	vars->SetScreenGrabPNGChecked(false);
	vars->SetScreenGrabICOChecked(false);
	vars->SetScreenGrabJPGChecked(false);
	vars->SetScreenGrabTIFChecked(false);
	vars->SetScreenGrabGIFChecked(false);
	vars->SetScreenGrabWMPChecked(false);


}


Hot3dxRotoDraw::Scenario13_ScreenGrab::~Scenario13_ScreenGrab()
{

}

void Hot3dxRotoDraw::Scenario13_ScreenGrab::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	Scenario13Vars^ vars = _rootPage->m_Scene13Vars;
	Scenario13_ScreenGrab::Current = this;
	_rootPage->m_Scene13Vars->SetScenario13Page(this);
}



void Hot3dxRotoDraw::Scenario13_ScreenGrab::DisplayResultScreenGrabSave(Windows::UI::Xaml::Controls::Image^ image, Windows::UI::Xaml::Controls::TextBox^ textBox, Platform::String^ thumbnailModeName, size_t size, Windows::Storage::IStorageItem^ item, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, bool isGroup)
{
	BitmapImage^ bitmapImage = ref new BitmapImage();

	bitmapImage->SetSource(thumbnail);
	image->Source = bitmapImage;

	String^ itemType = isGroup ? "Group" : item->IsOfType(StorageItemTypes::File) ? "File" : "Folder";
	textBox->Text = "ThumbnailMode." + thumbnailModeName + "\n"
		+ itemType + " : " + item->Name + "\n"
		+ "size: " + thumbnail->OriginalWidth.ToString() + "x" + thumbnail->OriginalHeight.ToString()
		+ "size: " + size.ToString() + "\n";
}

void Hot3dxRotoDraw::Scenario13_ScreenGrab::LoadChosenImage1()
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	unsigned int size = int(ScreenGrabImage1->ActualHeight);

	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}


	StorageFile^ file = vars->GetFilePic1();
	m_dirPath = ref new Platform::String(L"ms-appx:///Assets/Textures/");
	if (file)
	{
		create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->DisplayResultScreenGrabSave(this->ScreenGrabImage1, this->m_screenGrabFilesTextBox, thumbnailModeName, size, file, thumbnail, false);

					this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
					this->m_screenGrabFilesTextBox->Text = file->Path;
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file ", NotifyType::ErrorMessage);
				}
			});
	}
	else {
		this->_rootPage->NotifyUser("Error opening file ", NotifyType::ErrorMessage);
	}
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario13_ScreenGrab::IDC_SCREENGRAB_DDS_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario13Vars^ vars = _rootPage->m_Scene13Vars;
	if (IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked->Value)
	{
		vars->SetScreenGrabDDSChecked(true);
		vars->SetScreenGrabBMPChecked(false);
		vars->SetScreenGrabPNGChecked(false);
		vars->SetScreenGrabICOChecked(false);
		vars->SetScreenGrabJPGChecked(false);
		vars->SetScreenGrabTIFChecked(false);
		vars->SetScreenGrabGIFChecked(false);
		vars->SetScreenGrabWMPChecked(false);

		IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_BMP_CHECKBOX->SetValue(IDC_SCREENGRAB_BMP_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_PNG_CHECKBOX->SetValue(IDC_SCREENGRAB_PNG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_ICO_CHECKBOX->SetValue(IDC_SCREENGRAB_ICO_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_JPG_CHECKBOX->SetValue(IDC_SCREENGRAB_JPG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_TIFF_CHECKBOX->SetValue(IDC_SCREENGRAB_TIFF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_GIF_CHECKBOX->SetValue(IDC_SCREENGRAB_GIF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_WMP_CHECKBOX->SetValue(IDC_SCREENGRAB_WMP_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked);
		vars->SetGuidFormatForScreenGrabVars(7);
		_rootPage->NotifyUser("ScreenGrab to DDS Picked", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario13_ScreenGrab::IDC_SCREENGRAB_BMP_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario13Vars^ vars = _rootPage->m_Scene13Vars;
	if (IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked->Value)
	{
		vars->SetScreenGrabBMPChecked(true);
		vars->SetScreenGrabDDSChecked(false);
		vars->SetScreenGrabPNGChecked(false);
		vars->SetScreenGrabICOChecked(false);
		vars->SetScreenGrabJPGChecked(false);
		vars->SetScreenGrabTIFChecked(false);
		vars->SetScreenGrabGIFChecked(false);
		vars->SetScreenGrabWMPChecked(false);

		IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_DDS_CHECKBOX->SetValue(IDC_SCREENGRAB_DDS_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_PNG_CHECKBOX->SetValue(IDC_SCREENGRAB_PNG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_ICO_CHECKBOX->SetValue(IDC_SCREENGRAB_ICO_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_JPG_CHECKBOX->SetValue(IDC_SCREENGRAB_JPG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_TIFF_CHECKBOX->SetValue(IDC_SCREENGRAB_TIFF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_GIF_CHECKBOX->SetValue(IDC_SCREENGRAB_GIF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_WMP_CHECKBOX->SetValue(IDC_SCREENGRAB_WMP_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked);
		vars->SetGuidFormatForScreenGrabVars(0);
		_rootPage->NotifyUser("ScreenGrab to BMP Picked", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario13_ScreenGrab::IDC_SCREENGRAB_PNG_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario13Vars^ vars = _rootPage->m_Scene13Vars;
	if (IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked->Value)
	{
		vars->SetScreenGrabPNGChecked(true);
		vars->SetScreenGrabDDSChecked(false);
		vars->SetScreenGrabBMPChecked(false);
		vars->SetScreenGrabICOChecked(false);
		vars->SetScreenGrabJPGChecked(false);
		vars->SetScreenGrabTIFChecked(false);
		vars->SetScreenGrabGIFChecked(false);
		vars->SetScreenGrabWMPChecked(false);

		IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_DDS_CHECKBOX->SetValue(IDC_SCREENGRAB_DDS_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_BMP_CHECKBOX->SetValue(IDC_SCREENGRAB_BMP_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_ICO_CHECKBOX->SetValue(IDC_SCREENGRAB_ICO_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_JPG_CHECKBOX->SetValue(IDC_SCREENGRAB_JPG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_TIFF_CHECKBOX->SetValue(IDC_SCREENGRAB_TIFF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_GIF_CHECKBOX->SetValue(IDC_SCREENGRAB_GIF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_WMP_CHECKBOX->SetValue(IDC_SCREENGRAB_WMP_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked);
		vars->SetGuidFormatForScreenGrabVars(1);
		_rootPage->NotifyUser("ScreenGrab to PNG Picked", NotifyType::StatusMessage);
	}
}
/*
void Hot3dxRotoDraw::Scenario13_ScreenGrab::IDC_SCREENGRAB_ICO_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario13Vars^ vars = _rootPage->m_Scene13Vars;
	if (IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked->Value)
	{
		vars->SetScreenGrabICOChecked(true);
		vars->SetScreenGrabDDSChecked(false);
		vars->SetScreenGrabBMPChecked(false);
		vars->SetScreenGrabPNGChecked(false);
		vars->SetScreenGrabJPGChecked(false);
		vars->SetScreenGrabTIFChecked(false);
		vars->SetScreenGrabGIFChecked(false);
		vars->SetScreenGrabWMPChecked(false);

		IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_DDS_CHECKBOX->SetValue(IDC_SCREENGRAB_DDS_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_BMP_CHECKBOX->SetValue(IDC_SCREENGRAB_BMP_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_PNG_CHECKBOX->SetValue(IDC_SCREENGRAB_PNG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_JPG_CHECKBOX->SetValue(IDC_SCREENGRAB_JPG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_TIFF_CHECKBOX->SetValue(IDC_SCREENGRAB_TIFF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_GIF_CHECKBOX->SetValue(IDC_SCREENGRAB_GIF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_WMP_CHECKBOX->SetValue(IDC_SCREENGRAB_WMP_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked);
		vars->SetGuidFormatForScreenGrabVars(2);
		_rootPage->NotifyUser("ScreenGrab to ICO Picked", NotifyType::StatusMessage);
	}
}
*/
void Hot3dxRotoDraw::Scenario13_ScreenGrab::IDC_SCREENGRAB_JPG_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario13Vars^ vars = _rootPage->m_Scene13Vars;
	if (IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked->Value)
	{
		vars->SetScreenGrabJPGChecked(true);
		vars->SetScreenGrabDDSChecked(false);
		vars->SetScreenGrabBMPChecked(false);
		vars->SetScreenGrabPNGChecked(false);
		vars->SetScreenGrabICOChecked(false);
		vars->SetScreenGrabTIFChecked(false);
		vars->SetScreenGrabGIFChecked(false);
		vars->SetScreenGrabWMPChecked(false);

		IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_DDS_CHECKBOX->SetValue(IDC_SCREENGRAB_DDS_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_BMP_CHECKBOX->SetValue(IDC_SCREENGRAB_BMP_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_PNG_CHECKBOX->SetValue(IDC_SCREENGRAB_PNG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_ICO_CHECKBOX->SetValue(IDC_SCREENGRAB_ICO_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_TIFF_CHECKBOX->SetValue(IDC_SCREENGRAB_TIFF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_GIF_CHECKBOX->SetValue(IDC_SCREENGRAB_GIF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_WMP_CHECKBOX->SetValue(IDC_SCREENGRAB_WMP_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked);
		vars->SetGuidFormatForScreenGrabVars(3);
		_rootPage->NotifyUser("ScreenGrab to JPG Picked", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario13_ScreenGrab::IDC_SCREENGRAB_TIFF_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario13Vars^ vars = _rootPage->m_Scene13Vars;
	if (IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked->Value)
	{
		vars->SetScreenGrabTIFChecked(true);
		vars->SetScreenGrabDDSChecked(false);
		vars->SetScreenGrabBMPChecked(false);
		vars->SetScreenGrabPNGChecked(false);
		vars->SetScreenGrabICOChecked(false);
		vars->SetScreenGrabJPGChecked(false);
		vars->SetScreenGrabGIFChecked(false);
		vars->SetScreenGrabWMPChecked(false);

		IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_DDS_CHECKBOX->SetValue(IDC_SCREENGRAB_DDS_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_BMP_CHECKBOX->SetValue(IDC_SCREENGRAB_BMP_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_PNG_CHECKBOX->SetValue(IDC_SCREENGRAB_PNG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_ICO_CHECKBOX->SetValue(IDC_SCREENGRAB_ICO_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_JPG_CHECKBOX->SetValue(IDC_SCREENGRAB_JPG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_GIF_CHECKBOX->SetValue(IDC_SCREENGRAB_GIF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_WMP_CHECKBOX->SetValue(IDC_SCREENGRAB_WMP_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked);
		vars->SetGuidFormatForScreenGrabVars(4);
		_rootPage->NotifyUser("ScreenGrab to TIF Picked", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario13_ScreenGrab::IDC_SCREENGRAB_GIF_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario13Vars^ vars = _rootPage->m_Scene13Vars;
	if (IDC_SCREENGRAB_GIF_CHECKBOX->IsChecked->Value)
	{
		vars->SetScreenGrabGIFChecked(true);
		vars->SetScreenGrabDDSChecked(false);
		vars->SetScreenGrabBMPChecked(false);
		vars->SetScreenGrabPNGChecked(false);
		vars->SetScreenGrabICOChecked(false);
		vars->SetScreenGrabJPGChecked(false);
		vars->SetScreenGrabTIFChecked(false);
		vars->SetScreenGrabWMPChecked(false);

		IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_DDS_CHECKBOX->SetValue(IDC_SCREENGRAB_DDS_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_DDS_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_BMP_CHECKBOX->SetValue(IDC_SCREENGRAB_BMP_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_BMP_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_PNG_CHECKBOX->SetValue(IDC_SCREENGRAB_PNG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_PNG_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_ICO_CHECKBOX->SetValue(IDC_SCREENGRAB_ICO_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_ICO_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_JPG_CHECKBOX->SetValue(IDC_SCREENGRAB_JPG_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_JPG_CHECKBOX->IsChecked);

		IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked::set(false);
		IDC_SCREENGRAB_TIFF_CHECKBOX->SetValue(IDC_SCREENGRAB_TIFF_CHECKBOX->IsCheckedProperty,
			IDC_SCREENGRAB_TIFF_CHECKBOX->IsChecked);

		//IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked::set(false);
		//IDC_SCREENGRAB_WMP_CHECKBOX->SetValue(IDC_SCREENGRAB_WMP_CHECKBOX->IsCheckedProperty,
		//	IDC_SCREENGRAB_WMP_CHECKBOX->IsChecked);
		vars->SetGuidFormatForScreenGrabVars(5);
		_rootPage->NotifyUser("ScreenGrab to GIF Picked", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario13_ScreenGrab::ScreenGrabButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	_rootPage->ScreenGrabDXP();
}


void Hot3dxRotoDraw::Scenario13_ScreenGrab::ScreenGrabFilesTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario13_ScreenGrab::ScreenGrabTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}
