#include "pch.h"
#include "Scenario11_GridorPic.xaml.h"

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



Scenario11_GridorPic^ Scenario11_GridorPic::Current = nullptr;


Hot3dxRotoDraw::Scenario11_GridorPic::Scenario11_GridorPic() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();
	Scenario11Vars^ vars = _rootPage->m_Scene11Vars;
	if (vars->GetBitmapImageGridPicDXP1())
	{
		// loads chosen top texture image
		LoadChosenImage1();
	}
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);

	
	
	if(_rootPage->GetGridPicTextureImageFileDXP()->Data())
	wcfileName = ref new Platform::String(_rootPage->GetGridPicTextureImageFileDXP()->Data());

	IDC_GRID_OR_PIC_CHECKBOX->IsChecked::set(true);
	IDC_GRID_OR_PIC_CHECKBOX->SetValue(IDC_GRID_OR_PIC_CHECKBOX->IsCheckedProperty,
		IDC_GRID_OR_PIC_CHECKBOX->IsChecked);
	IDC_PIC_CHECKBOX->IsChecked::set(false);
	IDC_PIC_CHECKBOX->SetValue(IDC_PIC_CHECKBOX->IsCheckedProperty,
		IDC_PIC_CHECKBOX->IsChecked);
	vars->SetGridChecked(true);
	vars->SetPicChecked(false);
	Scenario11_GridorPic::Current = this;
	_rootPage->m_Scene11Vars->SetScenario11Page(this);
}

Hot3dxRotoDraw::Scenario11_GridorPic::~Scenario11_GridorPic()
{
	//throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario11_GridorPic::IDC_GRID_OR_PIC_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario11Vars^ vars = _rootPage->m_Scene11Vars;
	if (IDC_GRID_OR_PIC_CHECKBOX->IsChecked->Value)
	{
		vars->SetGridChecked(true);
		_rootPage->NotifyUser("Draw with Grid Picked", NotifyType::StatusMessage);
	}
	else {
		vars->SetGridChecked(false);
		_rootPage->NotifyUser("Draw with Photo Picked", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario11_GridorPic::IDC_PIC_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario11Vars^ vars = _rootPage->m_Scene11Vars;
	if (IDC_PIC_CHECKBOX->IsChecked->Value)
	{
		vars->SetPicChecked(true);
		_rootPage->NotifyUser("Draw with Background Picture Picked", NotifyType::StatusMessage);
	}
	else {
		vars->SetPicChecked(false);
		_rootPage->NotifyUser("Draw without Background Photo Picked", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario11_GridorPic::GridorPicTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	//throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario11_GridorPic::GridorPicFilesTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	//throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario11_GridorPic::DisplayResultGridPic(Image^ image, TextBox^ textBox, String^ thumbnailModeName, size_t size, IStorageItem^ item, StorageItemThumbnail^ thumbnail, bool isGroup)
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
void Hot3dxRotoDraw::Scenario11_GridorPic::IDC_PIC_TEXTURE_IMAGE1_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;//PicturesLibrary;
	openPicker->FileTypeFilter->Append(".jpg");
	openPicker->FileTypeFilter->Append(".jpeg");
	openPicker->FileTypeFilter->Append(".png");
	openPicker->FileTypeFilter->Append(".dds");
	openPicker->FileTypeFilter->Append(".bmp");
	openPicker->FileTypeFilter->Append(".tga");

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions{};

	const unsigned int size = int(PicTextureImage1->ActualHeight);
	if (size > 0)
	{
		bool fastThumbnail = false;
		ThumbnailOptions thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}
	
	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
	{
		if (file)
		{
			    m_dirPath = ref new Platform::String(file->Path->Data());
				Platform::String^ str = ref new Platform::String(file->FileType->Data());
				Platform::String^ strc1 = ref new Platform::String(L".dds");
				Platform::String^ strc2 = ref new Platform::String(L".DDS");

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

			Hot3dxRotoDraw::Scenario11_GridorPic^ _this = this;
			create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario11Vars^ vars = _this->_rootPage->m_Scene11Vars;
					vars->SetFileGridPic(file);
					vars->SetGridPicTextureImage1File(file->Path);
					_this->_rootPage->SetGridPicTextureImageFileDXP(file->Path);
					vars->SetGridPicTextureImage1File(file->Path);
					vars->SetThumbnailGridPicDXP1(thumbnail);
					_this->DisplayResultGridPic(_this->PicTextureImage1, _this->m_gridorPicFilesTextBox, thumbnailModeName, size, file, thumbnail, false);
					vars->SetbitmapImageGridPicDXP1(vars->GetThumbnailGridPicDXP1());
					vars->GetBitmapImageGridPicDXP1()->SetSource(vars->GetThumbnailGridPicDXP1());
					_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
					_this->m_gridorPicFilesTextBox->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario11_GridorPic::IDC_SHOW_BACKGROUND_PIC_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->DrawGridPicRectangleDXP();
}


void Hot3dxRotoDraw::Scenario11_GridorPic::LoadChosenImage1()
{
	Scenario11Vars^ vars = _rootPage->m_Scene11Vars;

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	unsigned int size = int(PicTextureImage1->ActualHeight);

	if (size > 0)
	{
		bool fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}
	

	StorageFile^ file = vars->GetFileGridPic();
	m_dirPath = ref new Platform::String(L"ms-appx:///Assets/Textures/");
	if (file)
	{
		create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
		{
			if (thumbnail != nullptr)
			{
				this->DisplayResultGridPic(this->PicTextureImage1, this->m_gridorPicFilesTextBox, thumbnailModeName, size, file, thumbnail, false);

				this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
				this->m_gridorPicFilesTextBox->Text = file->Path;
			}
			else
			{
				this->_rootPage->NotifyUser("Error opening file " + file->Name, NotifyType::ErrorMessage);
			}
		});
	}
	else {
		this->_rootPage->NotifyUser("Error opening file " + file->Name, NotifyType::ErrorMessage);
	}
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario11_GridorPic::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	Scenario11Vars^ vars = _rootPage->m_Scene11Vars;
	IDC_GRID_OR_PIC_CHECKBOX->IsChecked = vars->GetGridChecked();
	IDC_PIC_CHECKBOX->IsChecked = vars->GetPicChecked();
	Scenario11_GridorPic::Current = this;
	_rootPage->m_Scene11Vars->SetScenario11Page(this);
}

