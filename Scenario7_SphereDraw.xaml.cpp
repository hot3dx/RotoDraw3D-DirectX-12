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
#include "Scenario7_SphereDraw.xaml.h"

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

Scenario7_SphereDraw^ Scenario7_SphereDraw::Current = nullptr;

Hot3dxRotoDraw::Scenario7_SphereDraw::Scenario7_SphereDraw() : _rootPage(DirectXPage::Current)
{
    InitializeComponent();
    
	Scenario7Vars^ vars = _rootPage->m_Scene7Vars;
	if (vars->GetBitmapImageVideoDXP1())
	{
		// loads chosen top texture image
		LoadChosenImage1();
	}
	else
	{
	//	LoadDefaultVideoImage(L"Assets\\Textures\\SampleVideo.mp4.dds");
	}
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
	

	if (_rootPage->GetVideoTextureImageFileDXP()->Data()) {
		Platform::String^ fl = ref new Platform::String(_rootPage->GetVideoTextureImageFileDXP()->Data());
		wcfileName = ref new Platform::String(fl->Data());
	}
	IDC_VIDEO_CHECKBOX->IsChecked::set(true);
	IDC_VIDEO_CHECKBOX->SetValue(IDC_VIDEO_CHECKBOX->IsCheckedProperty,
		IDC_VIDEO_CHECKBOX->IsChecked);
	
	vars->SetVideoChecked(true);

	Scenario7_SphereDraw::Current = this;
	_rootPage->m_Scene7Vars->SetScenario7Page(this);
	
    
    
}

Hot3dxRotoDraw::Scenario7_SphereDraw::~Scenario7_SphereDraw()
{
    
}

/*
void Hot3dxRotoDraw::Scenario7_SphereDraw::SourcePositionX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    _x = static_cast<float>(e->NewValue);
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::SourcePositionY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    _y = static_cast<float>(e->NewValue);
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::SourcePositionZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    _z = static_cast<float>(e->NewValue);
}
*/
/////////////////
void Hot3dxRotoDraw::Scenario7_SphereDraw::SphereRadiusTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario7_SphereDraw::PointSpacingTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario7_SphereDraw::DrawSphereButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (_rootPage->GetPointCount() == 0)
    {
        Platform::String^ str;

        str = Hot3dxRotoDraw::Scenario7_SphereDraw::m_SphereRadiusTextBox->Text;
        m_cameraradius = std::wcstof(str->Data(), nullptr);

        str = Hot3dxRotoDraw::Scenario7_SphereDraw::m_PointSpaceTextBox->Text;
        m_camerarotation = std::wcstof(str->Data(), nullptr);

        _rootPage->SET_SPHERE_BUTTON_Click(sender, e, m_cameraradius, m_camerarotation);
        _rootPage->NotifyUser(L"Sphere Drawn", NotifyType::StatusMessage);
    }
    else
    {
        _rootPage->NotifyUser(L"Go To Draw Page and Click the Clear Button before drawing another sphere", NotifyType::StatusMessage);
    }
}


void Hot3dxRotoDraw::Scenario7_SphereDraw::Back_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->Button_Click(sender, e);
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::IDC_VIDEO_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario7Vars^ vars = _rootPage->m_Scene7Vars;
	if (IDC_VIDEO_CHECKBOX->IsChecked->Value)
	{
		vars->SetVideoChecked(true);
		_rootPage->NotifyUser("Draw with Background Picture Picked", NotifyType::StatusMessage);
	}
	else {
		vars->SetVideoChecked(false);
		_rootPage->NotifyUser("Draw without Background Photo Picked", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::VideoTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
    //throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::VideoFilesTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
    //throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::DisplayResultVideo(Windows::UI::Xaml::Controls::Image^ image, 
	Windows::UI::Xaml::Controls::TextBox^ textBox, 
	Platform::String^ thumbnailModeName, 
	size_t size, Windows::Storage::IStorageItem^ item, 
	Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, 
	bool isGroup)
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

void Hot3dxRotoDraw::Scenario7_SphereDraw::IDC_VIDEO_TEXTURE_IMAGE1_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;//PicturesLibrary;
	openPicker->FileTypeFilter->Append(".mp4");
	
	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions{};

	const unsigned int size = int(VideoTextureImage1->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{
				m_dirPath = ref new Platform::String(file->Path->Data());
				Platform::String^ str = ref new Platform::String(file->FileType->Data());
				Platform::String^ strc1 = ref new Platform::String(L".mp4");
				Platform::String^ strc2 = ref new Platform::String(L".MP4");

				if (str == strc1 || str == strc2)
				{
					//set RotoDrawSceneRender flag m_bDDS_WIC_FLAG = true
					_rootPage->SetDDS_WIC_FLAGGridPicFileDXP(true);
				}
				else
				{
					//set RotoDrawSceneRender flag m_bDDS_WIC_FLAG = false
					_rootPage->SetDDS_WIC_FLAGGridPicFileDXP(false);
				}

				Hot3dxRotoDraw::Scenario7_SphereDraw^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario7Vars^ vars = _this->_rootPage->m_Scene7Vars;
							vars->SetFileVideo(file);
							vars->SetVideoTextureImage1File(file->Path);
							_this->_rootPage->SetVideoTextureImageFileDXP(file->Path);
							vars->SetVideoTextureImage1File(file->Path);
							vars->SetThumbnailVideoDXP1(thumbnail);
							_this->DisplayResultVideo(_this->VideoTextureImage1, _this->m_videoFilesTextBox, thumbnailModeName, size, file, thumbnail, false);
							vars->SetbitmapImageVideoDXP1(vars->GetThumbnailVideoDXP1());
							vars->GetBitmapImageVideoDXP1()->SetSource(vars->GetThumbnailVideoDXP1());
							_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
							_this->m_videoFilesTextBox->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file" + file->Name, NotifyType::ErrorMessage);

						}
					});
			}
			else {}
		});
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::IDC_SHOW_VIDEO_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//_rootPage->DrawVideoRectangleDXP();
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::LoadChosenImage1()
{
	Scenario7Vars^ vars = _rootPage->m_Scene7Vars;

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	unsigned int size = int(VideoTextureImage1->ActualHeight);

	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}


	StorageFile^ file = vars->GetFileVideo();
	m_dirPath = ref new Platform::String(L"ms-appx:///Assets/Textures/");
	if (file)
	{
		create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->DisplayResultVideo(this->VideoTextureImage1, this->m_videoFilesTextBox, thumbnailModeName, size, file, thumbnail, false);

					this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
					this->m_videoFilesTextBox->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario7_SphereDraw::LoadDefaultVideoImage(Platform::String^ imagePath)
{
	Scenario7Vars^ vars = _rootPage->m_Scene7Vars;

	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;


	Platform::String^ s = ref new Platform::String();
	s = _rootPage->GetProgramDirPathDXP();
	Platform::String^ sfile = ref new Platform::String(s->Data());
	sfile = sfile->Concat(sfile, imagePath);

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(sfile));

	create_task(file->GetFileFromPathAsync(sfile)).then([this](StorageFile^ file)
		{
			if (file)
			{
				ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
				String^ thumbnailModeName = thumbnailMode.ToString();
				bool fastThumbnail = false;
				ThumbnailOptions thumbnailOptions;

				unsigned int size = int(VideoTextureImage1->ActualHeight);

				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario7_SphereDraw^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario7Vars^ vars = _this->_rootPage->m_Scene7Vars;
							vars->SetFileVideo(file);
							vars->SetVideoTextureImage1File(file->Path);
							_this->_rootPage->SetVideoTextureImageFileDXP(file->Path);
							vars->SetVideoTextureImage1File(file->Path);
							vars->SetThumbnailVideoDXP1(thumbnail);
							_this->DisplayResultVideo(_this->VideoTextureImage1, _this->m_videoFilesTextBox, thumbnailModeName, size, file, thumbnail, false);
							vars->SetbitmapImageVideoDXP1(vars->GetThumbnailDXP1());
							vars->GetBitmapImageVideoDXP1()->SetSource(vars->GetThumbnailVideoDXP1());

							_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
							_this->m_videoFilesTextBox->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file ", NotifyType::ErrorMessage);

						}

					});
			}
			else {}
		});

	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario7_SphereDraw::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	Scenario7Vars^ vars = _rootPage->m_Scene7Vars;
	//IDC_GRID_OR_PIC_CHECKBOX->IsChecked = vars->GetVideoChecked();
	IDC_VIDEO_CHECKBOX->IsChecked = vars->GetVideoChecked();
	Scenario7_SphereDraw::Current = this;
	_rootPage->m_Scene7Vars->SetScenario7Page(this);
}

