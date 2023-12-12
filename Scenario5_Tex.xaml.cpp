#include "pch.h"
#include "Scenario5_Tex.xaml.h"
#include "Common/DirectXHelper.h"
using namespace Windows::Storage::FileProperties;

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

String^ Errors::NoExifThumbnail = "No result (no EXIF thumbnail or cached thumbnail available for fast retrieval)";
String^ Errors::NoThumbnail = "No result (no thumbnail could be obtained from the selected file)";
String^ Errors::NoAlbumArt = "No result (no album art available for this song)";
String^ Errors::NoIcon = "No result (no icon available for this document type)";
String^ Errors::NoImages = "No result (no thumbnail could be obtained from the selected folder - make sure that the folder contains images)";
String^ Errors::FileGroupEmpty = "No result (unexpected error: retrieved file group was null)";
String^ Errors::FileGroupLocation = "File groups are only available for library locations, please select a folder from one of your libraries";
String^ Errors::Cancel = "No result (operation cancelled, no item selected)";
String^ Errors::InvalidSize = "Invalid size (specified size must be numerical and greater than zero)";

Scenario5_Tex^ Scenario5_Tex::Current = nullptr;

Hot3dxRotoDraw::Scenario5_Tex::Scenario5_Tex() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();

	SetDefaultFolderPath();

	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;
	_rootPage->m_Scene5TexVars->SetScenario5TexPage(this);

	//if (_rootPage->GetGridPicTextureImageFileDXP()->Data())
	//	wcfileName = ref new Platform::String(_rootPage->GetGridPicTextureImageFileDXP()->Data());
	
	IDC_GRID_OR_PIC_CHECKBOX->IsChecked::set(true);
	//IDC_GRID_OR_PIC_CHECKBOX->SetValue(IDC_GRID_OR_PIC_CHECKBOX->IsCheckedProperty,
	//	IDC_GRID_OR_PIC_CHECKBOX->IsChecked);
	IDC_PIC_CHECKBOX->IsChecked::set(false);
	//IDC_PIC_CHECKBOX->SetValue(IDC_PIC_CHECKBOX->IsCheckedProperty,
	//	IDC_PIC_CHECKBOX->IsChecked);
	vars->SetGridChecked(true);
	vars->SetPicChecked(false);

	m_sDefaultLoadFolderPath = _rootPage->Getm_sDirPathDXP();

	if (vars->GetBitmapImageDXP1())
	{
		// loads chosen top left texture image
		LoadChosenImage1();
	}
	else
	{
		LoadDefaultImage1(L"Assets\\Textures\\fire.dds");
	}
	if (vars->GetBitmapImageDXP2())
	{
		// loads chosen top right texture image
		LoadChosenImage2();
	}
	else
	{
		LoadDefaultImage2(L"Assets\\Textures\\sphere.png");
	    
	}
	/// <summary>
	if (vars->GetBitmapImageDXP3())
	{
		// loads chosen mid left texture image
		LoadChosenImage3();
	}
	else
	{
		LoadDefaultImage3(L"Assets\\Textures\\Toyrobot_RMA.dds");
		
	}
	/// </summary>
	if (vars->GetBitmapImageDXP4())
	{
		// loads chosen mid right texture image
		LoadChosenImage4();
	}
	else
	{
		LoadDefaultImage4(L"Assets\\Textures\\Toyrobot_Normal.dds");
		
	}
	if (vars->GetBitmapImageDXP5())
	{
		// loads chosen bottom left texture image
		LoadChosenImage5();
	}
	else
	{
		LoadDefaultImage5(L"Assets\\Textures\\ATRIUM_R.DDS");
		
	}
	if (vars->GetBitmapImageDXP6())
	{
		// loads chosen bottom right texture image
		LoadChosenImage6();
	}
	else
	{
		LoadDefaultImage6(L"Assets\\Textures\\ATRIUM_IR.DDS");
		
	}
	if (vars->GetBitmapImagePicDXP1())
	{
		// loads chosen bottom right texture image
		LoadChosenImagePic();
	}
	else
	{
		LoadDefaultImagePic(L"Assets\\Textures\\tree01S.DDS");

	}
	if (vars->GetBitmapImageVideoDXP1())
	{
		// loads chosen bottom right texture image
		LoadChosenImageVideo();
	}
	else
	{
		LoadDefaultImageVideo(L"Assets\\Textures\\SampleVideo.mp4");

	}
	
	Scenario5_Tex::Current = this;
	_rootPage->NotifyUser("Scenario5 MatsTexs Initialized", NotifyType::StatusMessage);
}

Hot3dxRotoDraw::Scenario5_Tex::~Scenario5_Tex()
{

}

void Hot3dxRotoDraw::Scenario5_Tex::DisplayResult(Image^ image, TextBlock^ textBlock, String^ thumbnailModeName,
	IStorageItem^ item, StorageItemThumbnail^ thumbnail, bool isGroup)
{
	BitmapImage^ bitmapImage = ref new BitmapImage();

	bitmapImage->SetSource(thumbnail);
	image->Source = bitmapImage;

	String^ itemType = isGroup ? "Group" : item->IsOfType(StorageItemTypes::File) ? "File" : "Folder";
	textBlock->Text = /*"ThumbnailMode." + thumbnailModeName + "\n"*/
		//+ itemType + 
		":" + item->Name + "\n";
		//+ "size: " + thumbnail->OriginalWidth.ToString() + "x" + thumbnail->OriginalHeight.ToString()
		//+ "size: " + size.ToString() + "\n";
		
}

void Hot3dxRotoDraw::Scenario5_Tex::DisplayResultVideo(Windows::UI::Xaml::Controls::Image^ image,
	Windows::UI::Xaml::Controls::TextBlock^ textBlock,
	Platform::String^ thumbnailModeName,
	size_t size, Windows::Storage::IStorageItem^ item,
	Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail,
	bool isGroup)
{
	BitmapImage^ bitmapImage = ref new BitmapImage();

	bitmapImage->SetSource(thumbnail);
	image->Source = bitmapImage;

	String^ itemType = isGroup ? "Group" : item->IsOfType(StorageItemTypes::File) ? "File" : "Folder";
	textBlock->Text = "ThumbnailMode." + thumbnailModeName + "\n"
		+ itemType + " : " + item->Name + "\n"
		+ "size: " + thumbnail->OriginalWidth.ToString() + "x" + thumbnail->OriginalHeight.ToString()
		+ "size: " + size.ToString() + "\n";
}

void Hot3dxRotoDraw::Scenario5_Tex::scrollBar_Scroll(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::ScrollEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImage1(Platform::String^ imagePath)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;
	
		// loads default top image
		FileOpenPicker^ pick = ref new FileOpenPicker();
		pick->ViewMode = PickerViewMode::Thumbnail;
		pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;
		Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());

		StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(s));
		unsigned int size = static_cast<unsigned int>(this->TextureImage1->ActualHeight);
		auto task1 = create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
		{
			if (file)
			{
				ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
				Platform::String^ thumbnailModeName = thumbnailMode.ToString();
				bool fastThumbnail = false;
				ThumbnailOptions thumbnailOptions;

				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				auto task2 = create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
				{
					if (thumbnail != nullptr)
					{
						Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile1(file);
							vars->SetTextureImage1File(file->Path);
							_this->_rootPage->SetTextureImage1NameDXP(file->Name);
							_this->_rootPage->SetTextureImage1FileDXP(file->Path);
							vars->SetTextureImage1File(file->Path);
							vars->SetThumbnailDXP1(thumbnail);
							DisplayResult(_this->TextureImage1, _this->filePath1TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP1(vars->GetThumbnailDXP1());
							vars->GetBitmapImageDXP1()->SetSource(vars->GetThumbnailDXP1());
						_this->_rootPage->NotifyUser("Opened file LoadDefaultImage1 " + file->Name, NotifyType::StatusMessage);
						_this->textureFileTextBlock1->Text = file->Path;
						
					}

					else
					{
						_this->_rootPage->NotifyUser("Error opening file LoadDefaultImage1 ", NotifyType::ErrorMessage);

					}

				});
			}
			else {}
		});
	
		
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImage2(Platform::String^ imagePath)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	// loads default top or bottom texture image
	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

	Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(s));
	unsigned int size = static_cast<unsigned int>(this->TextureImage2->ActualHeight);
	create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
	{
		if (file)
		{
			ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
			Platform::String^ thumbnailModeName = thumbnailMode.ToString();
			bool fastThumbnail = false;
			ThumbnailOptions thumbnailOptions;

			//unsigned int size = static_cast<unsigned int>(this->TextureImage2->ActualHeight);

			if (size > 0)
			{
				fastThumbnail = false;
				thumbnailOptions = ThumbnailOptions::UseCurrentScale;
			}
			Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
			create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
					vars->SetFile2(file);
						vars->SetTextureImage2File(file->Path);
						_this->_rootPage->SetTextureImage2NameDXP(file->Name);
						_this->_rootPage->SetTextureImage2FileDXP(file->Path);
						vars->SetTextureImage2File(file->Path);
						vars->SetThumbnailDXP2(thumbnail);
						DisplayResult(_this->TextureImage2, _this->filePath2TextBlock, thumbnailModeName, file, thumbnail, false);
						vars->SetbitmapImageDXP2(vars->GetThumbnailDXP2());
						vars->GetBitmapImageDXP2()->SetSource(vars->GetThumbnailDXP2());
					_this->_rootPage->NotifyUser("Opened file LoadDefaultImage2 " + file->Name, NotifyType::StatusMessage);
					_this->textureFileTextBlock2->Text = file->Path;
				}

				else
				{
					_this->_rootPage->NotifyUser("Error opening file LoadDefaultImage2 ", NotifyType::ErrorMessage);

				}

			});
		}
		else {}
	});

	
}


void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImage3(Platform::String^ imagePath)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	// loads default top or bottom texture image
	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

	Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(s));
	unsigned int size = static_cast<unsigned int>(this->TextureRMAImage3->ActualHeight);
	create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
		{
			if (file)
			{
				ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
				Platform::String^ thumbnailModeName = thumbnailMode.ToString();
				bool fastThumbnail = false;
				ThumbnailOptions thumbnailOptions;

				

				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile3(file);
							vars->SetTextureImage3File(file->Path);
							_this->_rootPage->SetTextureImage3NameDXP(file->Name);
							_this->_rootPage->SetTextureImage3FileDXP(file->Path);
							vars->SetTextureImage3File(file->Path);
							vars->SetThumbnailDXP3(thumbnail);
							DisplayResult(_this->TextureRMAImage3, _this->filePath3TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP3(vars->GetThumbnailDXP3());
							vars->GetBitmapImageDXP3()->SetSource(vars->GetThumbnailDXP3());
							_this->_rootPage->NotifyUser("Opened file LoadDefaultImage3 " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock3->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file LoadDefaultImage3 ", NotifyType::ErrorMessage);

						}

					});
			}
			else {}
		});

	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImage4(Platform::String^ imagePath)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	// loads default top or bottom texture image
	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

	Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(s));
	unsigned int size = int(this->TextureEMMisiveImage4->ActualHeight);
	create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
		{
			if (file)
			{
				ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
				Platform::String^ thumbnailModeName = thumbnailMode.ToString();
				bool fastThumbnail = false;
				ThumbnailOptions thumbnailOptions;

				

				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile4(file);
							vars->SetTextureImage4File(file->Path);
							_this->_rootPage->SetTextureImage4NameDXP(file->Name);
							_this->_rootPage->SetTextureImage4FileDXP(file->Path);
							vars->SetTextureImage4File(file->Path);
							vars->SetThumbnailDXP4(thumbnail);
							DisplayResult(_this->TextureEMMisiveImage4, _this->filePath4TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP4(vars->GetThumbnailDXP4());
							vars->GetBitmapImageDXP4()->SetSource(vars->GetThumbnailDXP4());
							_this->_rootPage->NotifyUser("Opened file LoadDefaultImage4 " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock4->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file LoadDefaultImage4 ", NotifyType::ErrorMessage);

						}

					});
			}
			else {}
		});

	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImage5(Platform::String^ imagePath)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

	Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(s));
	unsigned int size = int(this->TextureRadianceImage5->ActualHeight);
	create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
		{
			if (file)
			{
				ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
				Platform::String^ thumbnailModeName = thumbnailMode.ToString();
				bool fastThumbnail = false;
				ThumbnailOptions thumbnailOptions;

				

				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile5(file);
							vars->SetTextureImage5File(file->Path);
							_this->_rootPage->SetTextureImage5NameDXP(file->Name);
							_this->_rootPage->SetTextureImage5FileDXP(file->Path);
							vars->SetTextureImage5File(file->Path);
							vars->SetThumbnailDXP5(thumbnail);
							DisplayResult(_this->TextureRadianceImage5, _this->filePath5TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP5(vars->GetThumbnailDXP5());
							vars->GetBitmapImageDXP5()->SetSource(vars->GetThumbnailDXP5());

							_this->_rootPage->NotifyUser("Opened file LoadDefaultImage5 " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock5->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file LoadDefaultImage5 ", NotifyType::ErrorMessage);

						}

					});
			}
			else {}
		});

	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImage6(Platform::String^ imagePath)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

	Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(s));
	unsigned int size = int(this->TextureIrradianceImage6->ActualHeight);
	create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
		{
			if (file)
			{
				ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
				Platform::String^ thumbnailModeName = thumbnailMode.ToString();
				bool fastThumbnail = false;
				ThumbnailOptions thumbnailOptions;

				

				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile6(file);
							vars->SetTextureImage6File(file->Path);
							_this->_rootPage->SetTextureImage6NameDXP(file->Name);
							_this->_rootPage->SetTextureImage6FileDXP(file->Path);
							vars->SetTextureImage6File(file->Path);
							vars->SetThumbnailDXP6(thumbnail);
							DisplayResult(_this->TextureIrradianceImage6, _this->filePath6TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP6(vars->GetThumbnailDXP6());
							vars->GetBitmapImageDXP6()->SetSource(vars->GetThumbnailDXP6());

							_this->_rootPage->NotifyUser("Opened file LoadDefaultImage6 " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock6->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file LoadDefaultImage6 ", NotifyType::ErrorMessage);

						}

					});
			}
			else {}
		});

	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadChosenImage1()
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;
	
	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
		String^ thumbnailModeName = thumbnailMode.ToString();
		bool fastThumbnail = false;
		ThumbnailOptions thumbnailOptions;
		
		unsigned int size = int(TextureImage1->ActualHeight);

		if (size > 0)
		{
			fastThumbnail = false;
			thumbnailOptions = ThumbnailOptions::UseCurrentScale;
		}

		StorageFile^ file = vars->GetFile1();
		
		if (file)
		{
			create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					vars->SetFile1(file);
					vars->SetTextureImage1File(file->Path);
					this->_rootPage->SetTextureImage1FileDXP(file->Path);
					this->_rootPage->SetTextureImage1NameDXP(file->Name);
					vars->SetTextureImage1File(file->Path);
					vars->SetThumbnailDXP1(thumbnail);
					DisplayResult(this->TextureImage1, this->filePath1TextBlock, thumbnailModeName, file, thumbnail, false);
					vars->SetbitmapImageDXP1(vars->GetThumbnailDXP1());
					vars->GetBitmapImageDXP1()->SetSource(vars->GetThumbnailDXP1());
					this->_rootPage->SetTextureImage1NameDXP(file->Name);
					this->_rootPage->NotifyUser("Opened file LoadChosen1 " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock1->Text::set(file->Path);
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file LoadChosen1", NotifyType::ErrorMessage);
				}
			});
		}
		else {
			this->_rootPage->NotifyUser("Error opening file LoadChosen1", NotifyType::ErrorMessage);
		}
		

}

void Hot3dxRotoDraw::Scenario5_Tex::LoadChosenImage2()
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;
	
	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
		String^ thumbnailModeName = thumbnailMode.ToString();
		bool fastThumbnail = false;
		ThumbnailOptions thumbnailOptions;

		unsigned int size = int(TextureImage2->ActualHeight);

		if (size > 0)
		{
			fastThumbnail = false;
			thumbnailOptions = ThumbnailOptions::UseCurrentScale;
		}

		StorageFile^ file = vars->GetFile2();
		
		if (file)
		{
			create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					vars->SetFile2(file);
					vars->SetTextureImage2File(file->Path);
					this->_rootPage->SetTextureImage2FileDXP(file->Path);
					this->_rootPage->SetTextureImage2NameDXP(file->Name);
					vars->SetTextureImage2File(file->Path);
					vars->SetThumbnailDXP2(thumbnail);
					DisplayResult(this->TextureImage2, this->filePath2TextBlock, thumbnailModeName, file, thumbnail, false);
					vars->SetbitmapImageDXP2(vars->GetThumbnailDXP2());
					vars->GetBitmapImageDXP2()->SetSource(vars->GetThumbnailDXP2());
					this->_rootPage->SetTextureImage2NameDXP(file->Name);
					this->_rootPage->NotifyUser("Opened file LoadChosen2 " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock2->Text::set(file->Path);
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file LoadChosen2 ", NotifyType::ErrorMessage);

				}
			});
		}
		else {
			this->_rootPage->NotifyUser("Error opening file LoadChosen2 ", NotifyType::ErrorMessage);
		}
	
	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadChosenImage3()
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	unsigned int size = int(TextureRMAImage3->ActualHeight);

	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	StorageFile^ file = vars->GetFile3();

	if (file)
	{
		create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					vars->SetFile3(file);
					vars->SetTextureImage3File(file->Path);
					this->_rootPage->SetTextureImage3FileDXP(file->Path);
					vars->SetTextureImage3File(file->Path);
					vars->SetThumbnailDXP3(thumbnail);
					DisplayResult(this->TextureRMAImage3, this->filePath3TextBlock, thumbnailModeName, file, thumbnail, false);
					vars->SetbitmapImageDXP3(vars->GetThumbnailDXP3());
					vars->GetBitmapImageDXP3()->SetSource(vars->GetThumbnailDXP3());
					this->_rootPage->SetTextureImage3NameDXP(file->Name);
					this->_rootPage->NotifyUser("Opened file LoadChosen3 " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock3->Text::set(file->Path);
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file LoadChosen3 ", NotifyType::ErrorMessage);

				}
			});
	}
	else {
		this->_rootPage->NotifyUser("Error opening file LoadChosen3 ", NotifyType::ErrorMessage);
	}

	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadChosenImage4()
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	unsigned int size = int(TextureEMMisiveImage4->ActualHeight);

	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	StorageFile^ file = vars->GetFile4();

	if (file)
	{
		create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					vars->SetFile4(file);
					vars->SetTextureImage4File(file->Path);
					this->_rootPage->SetTextureImage4FileDXP(file->Path);
					vars->SetTextureImage4File(file->Path);
					vars->SetThumbnailDXP4(thumbnail);
					DisplayResult(this->TextureEMMisiveImage4, this->filePath4TextBlock, thumbnailModeName, file, thumbnail, false);
					vars->SetbitmapImageDXP4(vars->GetThumbnailDXP4());
					vars->GetBitmapImageDXP4()->SetSource(vars->GetThumbnailDXP4());
					this->_rootPage->SetTextureImage4NameDXP(file->Name);
					this->_rootPage->NotifyUser("Opened file LoadChosen4 " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock4->Text::set(file->Path);
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file LoadChosen4 ", NotifyType::ErrorMessage);

				}
			});
	}
	else {
		this->_rootPage->NotifyUser("Error opening file LoadChosen4 ", NotifyType::ErrorMessage);
	}

	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadChosenImage5()
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	unsigned int size = int(TextureRadianceImage5->ActualHeight);

	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	StorageFile^ file = vars->GetFile5();

	if (file)
	{
		create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					vars->SetFile5(file);
					vars->SetTextureImage5File(file->Path);
					this->_rootPage->SetTextureImage5FileDXP(file->Path);
					vars->SetTextureImage5File(file->Path);
					vars->SetThumbnailDXP5(thumbnail);
					DisplayResult(this->TextureRadianceImage5, this->filePath5TextBlock, thumbnailModeName, file, thumbnail, false);
					vars->SetbitmapImageDXP5(vars->GetThumbnailDXP5());
					vars->GetBitmapImageDXP5()->SetSource(vars->GetThumbnailDXP5());
					this->_rootPage->SetTextureImage5NameDXP(file->Name);
					this->_rootPage->NotifyUser("Opened file LoadChosen5 " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock5->Text::set(file->Path);
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file LoadChosen5 ", NotifyType::ErrorMessage);

				}
			});
	}
	else {
		this->_rootPage->NotifyUser("Error opening file LoadChosen5 ", NotifyType::ErrorMessage);
	}

	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadChosenImage6()
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	unsigned int size = int(TextureIrradianceImage6->ActualHeight);

	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	StorageFile^ file = vars->GetFile6();

	if (file)
	{
		create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					vars->SetFile6(file);
					vars->SetTextureImage6File(file->Path);
					this->_rootPage->SetTextureImage6FileDXP(file->Path);
					vars->SetTextureImage6File(file->Path);
					vars->SetThumbnailDXP6(thumbnail);
					DisplayResult(this->TextureIrradianceImage6, this->filePath6TextBlock, thumbnailModeName, file, thumbnail, false);
					vars->SetbitmapImageDXP6(vars->GetThumbnailDXP6());
					vars->GetBitmapImageDXP6()->SetSource(vars->GetThumbnailDXP6());
					this->_rootPage->SetTextureImage6NameDXP(file->Name);
					this->_rootPage->NotifyUser("Opened file LoadChosen6 " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock6->Text::set(file->Path);
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file LoadChosen6 ", NotifyType::ErrorMessage);

				}
			});
	}
	else {
		this->_rootPage->NotifyUser("Error opening file LoadChosen6 ", NotifyType::ErrorMessage);
	}

	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadChosenImagePic()
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	unsigned int size = int(PicTextureImage2->ActualHeight);

	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}


	StorageFile^ file = vars->GetFilePic1();
	m_sPicDirPath = ref new Platform::String(L"ms-appx:///Assets/Textures/");
	if (file)
	{
		create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					vars->SetFilePic1(file);
					vars->SetTextureImagePic1File(file->Path);
					this->_rootPage->SetTextureImagePicFileDXP(file->Path);
					vars->SetTextureImagePic1File(file->Path);
					this->_rootPage->SetTextureImagePic2NameDXP(file->Name);
					vars->SetThumbnailPicDXP1(thumbnail);
					this->DisplayResult(this->PicTextureImage2, this->filePathPic1TextBlock, thumbnailModeName, file, thumbnail, false);
					vars->SetbitmapImagePicDXP1(vars->GetThumbnailPicDXP1());
					vars->GetBitmapImagePicDXP1()->SetSource(vars->GetThumbnailPicDXP1());
					this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
					this->textureFilePicTextBlock1->Text = file->Path;
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
	_rootPage->NotifyUser("Stopped Reload", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadChosenImageVideo()
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	unsigned int size = int(VideoTextureImage2->ActualHeight);

	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}


	StorageFile^ file = vars->GetFileVideo();
	m_sVideoDirPath = ref new Platform::String(L"ms-appx:///Assets/Textures/");
	if (file)
	{
		create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Hot3dxRotoDraw::Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					vars->SetFileVideo(file);
					vars->SetTextureImageVideo2File(file->Path);
					this->_rootPage->SetTextureImageVideo2FileDXP(file->Path);
					vars->SetTextureImageVideo2File(file->Path);
					this->_rootPage->SetTextureImageVideo2NameDXP(file->Name);
					vars->SetThumbnailVideoDXP(thumbnail);
					this->DisplayResult(this->VideoTextureImage2, this->filePathVideo2TextBlock, thumbnailModeName, file, thumbnail, false);
					vars->SetbitmapImageVideoDXP1(vars->GetThumbnailVideoDXP1());
					vars->GetBitmapImageVideoDXP1()->SetSource(vars->GetThumbnailVideoDXP1());
					this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
					this->textureFileVideoTextBlock2->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImagePic(Platform::String^ imagePath)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

	Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(s));
	unsigned int size = int(this->PicTextureImage2->ActualHeight);
	create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
		{
			if (file)
			{
				ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
				Platform::String^ thumbnailModeName = thumbnailMode.ToString();
				bool fastThumbnail = false;
				ThumbnailOptions thumbnailOptions;

				

				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFilePic1(file);
							vars->SetTextureImagePic1File(file->Path);
							_this->_rootPage->SetTextureImagePicFileDXP(file->Path);
							vars->SetTextureImagePic1File(file->Path);
							_this->_rootPage->SetTextureImagePic2NameDXP(file->Name);
							vars->SetThumbnailPicDXP1(thumbnail);
							_this->DisplayResult(_this->PicTextureImage2, _this->filePathPic1TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImagePicDXP1(vars->GetThumbnailPicDXP1());
							vars->GetBitmapImagePicDXP1()->SetSource(vars->GetThumbnailPicDXP1());
							_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
							_this->textureFilePicTextBlock1->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file LoadDefaultImagePic", NotifyType::ErrorMessage);

						}

					});
			}
			else {}
		});
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImageVideo(Platform::String^ imagePath)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;
	openPicker->FileTypeFilter->Append(".mp4");

	const unsigned int size = int(VideoTextureImage2->ActualHeight);
	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions{};
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}
	Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());
		
	create_task(openPicker->PickSingleFileAsync(s)).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{
				Platform::String^ str = ref new Platform::String(file->FileType->Data());
				Platform::String^ strc1 = ref new Platform::String(L".mp4");
				Platform::String^ strc2 = ref new Platform::String(L".MP4");

				if (str == strc1 || str == strc2)
				{
					//set RotoDrawSceneRender flag m_bDDS_WIC_FLAG = true
					_rootPage->SetDDS_WIC_FLAGGridPicFileDXP(false);
				}
				else
				{
					//set RotoDrawSceneRender flag m_bDDS_WIC_FLAG = false
					_rootPage->SetDDS_WIC_FLAGGridPicFileDXP(false);
				}
								
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Hot3dxRotoDraw::Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFileVideo(file);
							vars->SetTextureImageVideo2File(file->Path);
							_this->_rootPage->SetTextureImageVideo2FileDXP(file->Path);
							vars->SetTextureImageVideo2File(file->Path);
							_this->_rootPage->SetTextureImageVideo2NameDXP(file->Name);
							vars->SetThumbnailVideoDXP(thumbnail);
							_this->DisplayResult(_this->VideoTextureImage2, _this->filePathVideo2TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageVideoDXP1(vars->GetThumbnailVideoDXP1());
							vars->GetBitmapImageVideoDXP1()->SetSource(vars->GetThumbnailVideoDXP1());
							_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
							_this->textureFileVideoTextBlock2->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file LoadDefaultImageVideo", NotifyType::ErrorMessage);

						}

					});
			}
			else {}
		});
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImages(unsigned int imageIdNum, 
	Platform::String^ imagePath)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;
	imageIdNums = imageIdNum;

	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;


	Platform::String^ s = ref new Platform::String();
	s = ref new Platform::String(_rootPage->Getm_sDirPathDXP()->Data());
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

				unsigned int size = 0;

				switch (this->imageIdNums)
				{
				case 1: {
					size = int(this->TextureImage1->ActualHeight);
				}break;
				case 2: {
					size = int(this->TextureImage2->ActualHeight);
				}break;
				case 3: {
					size = int(this->TextureRMAImage3->ActualHeight);
				}break;
				case 4: {
					size = int(this->TextureEMMisiveImage4->ActualHeight);
				}break;
				case 5: {
					size = int(this->TextureRadianceImage5->ActualHeight);
				}break;
				case 6: {
					size = int(this->TextureIrradianceImage6->ActualHeight);
			    }break;
				}
				
				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							
							switch (_this->imageIdNums)
							{
							case 1: {
								vars->SetFile1(file);
								vars->SetTextureImage1File(file->Path);
								_this->_rootPage->SetTextureImage1FileDXP(file->Path);
								_this->_rootPage->SetTextureImage1NameDXP(file->Name);
								vars->SetTextureImage1File(file->Path);
								vars->SetThumbnailDXP1(thumbnail);
								DisplayResult(_this->TextureImage1, _this->filePath1TextBlock, thumbnailModeName, file, thumbnail, false);
								vars->SetbitmapImageDXP1(vars->GetThumbnailDXP1());
								vars->GetBitmapImageDXP1()->SetSource(vars->GetThumbnailDXP1());
								_this->textureFileTextBlock1->Text = file->Path;
								_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
								
							}break;
							case 2: {
								vars->SetFile2(file);
								vars->SetTextureImage2File(file->Path);
								_this->_rootPage->SetTextureImage2FileDXP(file->Path);
								_this->_rootPage->SetTextureImage2NameDXP(file->Name);
								vars->SetTextureImage2File(file->Path);
								vars->SetThumbnailDXP2(thumbnail);
								DisplayResult(_this->TextureImage2, _this->filePath2TextBlock, thumbnailModeName, file, thumbnail, false);
								vars->SetbitmapImageDXP2(vars->GetThumbnailDXP2());
								vars->GetBitmapImageDXP2()->SetSource(vars->GetThumbnailDXP2());
								_this->textureFileTextBlock2->Text = file->Path;
								_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
								
							}break;
							case 3: {
								vars->SetFile3(file);
								vars->SetTextureImage3File(file->Path);
								_this->_rootPage->SetTextureImage3FileDXP(file->Path);
								_this->_rootPage->SetTextureImage3NameDXP(file->Name);
								vars->SetTextureImage3File(file->Path);
								vars->SetThumbnailDXP3(thumbnail);
								DisplayResult(_this->TextureRMAImage3, _this->filePath3TextBlock, thumbnailModeName, file, thumbnail, false);
								vars->SetbitmapImageDXP3(vars->GetThumbnailDXP3());
								vars->GetBitmapImageDXP3()->SetSource(vars->GetThumbnailDXP3());
								_this->textureFileTextBlock3->Text = file->Path;
								_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
								
							}break;
							case 4: {
								vars->SetFile4(file);
								vars->SetTextureImage4File(file->Path);
								_this->_rootPage->SetTextureImage4FileDXP(file->Path);
								_this->_rootPage->SetTextureImage4NameDXP(file->Name);
								vars->SetTextureImage4File(file->Path);
								vars->SetThumbnailDXP4(thumbnail);
								DisplayResult(_this->TextureEMMisiveImage4, _this->filePath4TextBlock, thumbnailModeName, file, thumbnail, false);
								vars->SetbitmapImageDXP4(vars->GetThumbnailDXP4());
								vars->GetBitmapImageDXP4()->SetSource(vars->GetThumbnailDXP4());
								_this->textureFileTextBlock4->Text = file->Path;
								_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
								
							}break;
							case 5: {
								vars->SetFile5(file);
								vars->SetTextureImage5File(file->Path);
								_this->_rootPage->SetTextureImage5FileDXP(file->Path);
								_this->_rootPage->SetTextureImage5NameDXP(file->Name);
								vars->SetTextureImage5File(file->Path);
								vars->SetThumbnailDXP5(thumbnail);
								DisplayResult(_this->TextureRadianceImage5, _this->filePath5TextBlock, thumbnailModeName, file, thumbnail, false);
								vars->SetbitmapImageDXP5(vars->GetThumbnailDXP5());
								vars->GetBitmapImageDXP5()->SetSource(vars->GetThumbnailDXP5());
								_this->textureFileTextBlock5->Text = file->Path;
								_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
								
							}break;
							case 6: {
								vars->SetFile6(file);
								vars->SetTextureImage6File(file->Path);
								_this->_rootPage->SetTextureImage6FileDXP(file->Path);
								_this->_rootPage->SetTextureImage6NameDXP(file->Name);
								vars->SetTextureImage6File(file->Path);
								vars->SetThumbnailDXP6(thumbnail);
								DisplayResult(_this->TextureIrradianceImage6, _this->filePath6TextBlock, thumbnailModeName, file, thumbnail, false);
								vars->SetbitmapImageDXP6(vars->GetThumbnailDXP6());
								vars->GetBitmapImageDXP6()->SetSource(vars->GetThumbnailDXP6());
								_this->textureFileTextBlock6->Text = file->Path;
								_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
								
							}break;
							}//eo switch
						}
						else
						{
							_this->_rootPage->NotifyUser("Error opening file ", NotifyType::ErrorMessage);
						}
					});
			}
			else {}
		});
	
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadChosenImages()
{
	throw ref new Platform::NotImplementedException();
}


void Hot3dxRotoDraw::Scenario5_Tex::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	Scenario5_Tex::Current = this;
	_rootPage->m_Scene5TexVars->SetScenario5TexPage(this);

}




void Hot3dxRotoDraw::Scenario5_Tex::TextureImage1_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureImage1->BaseUri;
}

void Hot3dxRotoDraw::Scenario5_Tex::TextureImage2_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureImage2->BaseUri;
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureImage3_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureRMAImage3->BaseUri;
	
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureImage4_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureEMMisiveImage4->BaseUri;
	
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureImage5_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureRadianceImage5->BaseUri;
	
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureImage6_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureIrradianceImage6->BaseUri;
	
}

void Hot3dxRotoDraw::Scenario5_Tex::PicTextureImage2_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = PicTextureImage2->BaseUri;
}

void Hot3dxRotoDraw::Scenario5_Tex::VideoTextureImage2_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = VideoTextureImage2->BaseUri;
}


void Hot3dxRotoDraw::Scenario5_Tex::filePath1TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTextureImage1NameDXP(ref new Platform::String(filePath1TextBlock->Text->Data()));
}


void Hot3dxRotoDraw::Scenario5_Tex::filePath2TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTextureImage2NameDXP(ref new Platform::String(filePath2TextBlock->Text->Data()));
}


void Hot3dxRotoDraw::Scenario5_Tex::filePath3TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTextureImage3NameDXP(ref new Platform::String(filePath3TextBlock->Text->Data()));
}


void Hot3dxRotoDraw::Scenario5_Tex::filePath4TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTextureImage4NameDXP(ref new Platform::String(filePath4TextBlock->Text->Data()));
}


void Hot3dxRotoDraw::Scenario5_Tex::filePath5TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTextureImage5NameDXP(ref new Platform::String(filePath5TextBlock->Text->Data()));
}


void Hot3dxRotoDraw::Scenario5_Tex::filePath6TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTextureImage6NameDXP(ref new Platform::String(filePath6TextBlock->Text->Data()));
}

void Hot3dxRotoDraw::Scenario5_Tex::filePathPic1TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTextureImagePic2NameDXP(ref new Platform::String(filePathPic1TextBlock->Text->Data()));
}

void Hot3dxRotoDraw::Scenario5_Tex::filePathVideo2TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTextureImageVideo2NameDXP(ref new Platform::String(filePathVideo2TextBlock->Text->Data()));
}

void Hot3dxRotoDraw::Scenario5_Tex::IDC_D3DMATERIAL_DISPLAY_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::TextureImage1_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	
}

void Hot3dxRotoDraw::Scenario5_Tex::TextureImage2_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureRMAImage3_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureEMMisiveImage4_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureRadianceImage5_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureIrradianceImage6_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	
}

inline void Hot3dxRotoDraw::Scenario5_Tex::SetDefaultFolderPath()
{
	m_sDefaultLoadFolderPath = ref new Platform::String(_rootPage->Getm_sDirPathDXP()->Data());
}

Platform::String^ Hot3dxRotoDraw::Scenario5_Tex::GetImagePath(Platform::String^ imagePath)
{
	Platform::String^ s1 = ref new Platform::String(m_sDefaultLoadFolderPath->Data());
	Platform::String^ s = s1->Concat(s1, imagePath);
	return s;
}



/*
* Upper Add Texture Right Click on Textures User Interface
*/

void Hot3dxRotoDraw::Scenario5_Tex::TextureImage1_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;
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

	const unsigned int size = int(TextureImage1->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{

				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile1(file);
							vars->SetTextureImage1File(file->Path);
							_this->_rootPage->SetTextureImage1FileDXP(file->Path);
							_this->_rootPage->SetTextureImage1NameDXP(file->Name);
							vars->SetTextureImage1File(file->Path);
							vars->SetThumbnailDXP1(thumbnail);
							DisplayResult(_this->TextureImage1, _this->filePath1TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP1(vars->GetThumbnailDXP1());
							vars->GetBitmapImageDXP1()->SetSource(vars->GetThumbnailDXP1());
							_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock1->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file ", NotifyType::ErrorMessage);


						}

					});
			}
			else {}
		});
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureImage2_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;
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

	const unsigned int size = int(TextureImage2->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile2(file);
							vars->SetTextureImage2File(file->Path);
							_this->_rootPage->SetTextureImage2FileDXP(file->Path);
							_this->_rootPage->SetTextureImage2NameDXP(file->Name);
							vars->SetTextureImage2File(file->Path);
							vars->SetThumbnailDXP2(thumbnail);
							DisplayResult(_this->TextureImage2, _this->filePath2TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP2(vars->GetThumbnailDXP2());
							vars->GetBitmapImageDXP2()->SetSource(vars->GetThumbnailDXP2());
							_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock2->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening TextureImage2 file ", NotifyType::ErrorMessage);
						}

					});
			}
			else {}
		});
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureRMAImage3_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;
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

	const unsigned int size = int(TextureRMAImage3->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile3(file);
							vars->SetTextureImage3File(file->Path);
							_this->_rootPage->SetTextureImage3FileDXP(file->Path);
							_this->_rootPage->SetTextureImage3NameDXP(file->Name);
							vars->SetTextureImage3File(file->Path);
							vars->SetThumbnailDXP3(thumbnail);
							DisplayResult(_this->TextureRMAImage3, _this->filePath3TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP3(vars->GetThumbnailDXP3());
							vars->GetBitmapImageDXP3()->SetSource(vars->GetThumbnailDXP3());
							_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock3->Text = file->Path;
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


void Hot3dxRotoDraw::Scenario5_Tex::TextureNormalImage4_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;
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

	const unsigned int size = int(TextureEMMisiveImage4->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile4(file);
							vars->SetTextureImage4File(file->Path);
							_this->_rootPage->SetTextureImage4FileDXP(file->Path);
							_this->_rootPage->SetTextureImage4NameDXP(file->Name);
							vars->SetTextureImage4File(file->Path);
							vars->SetThumbnailDXP4(thumbnail);
							DisplayResult(_this->TextureEMMisiveImage4, _this->filePath4TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP4(vars->GetThumbnailDXP4());
							vars->GetBitmapImageDXP4()->SetSource(vars->GetThumbnailDXP4());
							_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock4->Text = file->Path;
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


void Hot3dxRotoDraw::Scenario5_Tex::TextureRadianceImage5_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;
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

	const unsigned int size = int(TextureRadianceImage5->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile5(file);
							vars->SetTextureImage5File(file->Path);
							_this->_rootPage->SetTextureImage5FileDXP(file->Path);
							_this->_rootPage->SetTextureImage5NameDXP(file->Name);
							vars->SetTextureImage5File(file->Path);
							vars->SetThumbnailDXP5(thumbnail);
							DisplayResult(_this->TextureRadianceImage5, _this->filePath5TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP5(vars->GetThumbnailDXP5());
							vars->GetBitmapImageDXP5()->SetSource(vars->GetThumbnailDXP5());
							_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock5->Text = file->Path;
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


void Hot3dxRotoDraw::Scenario5_Tex::TextureIrradianceImage6_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;
	openPicker->FileTypeFilter->Append(".jpg");
	openPicker->FileTypeFilter->Append(".jpeg");
	openPicker->FileTypeFilter->Append(".png");
	openPicker->FileTypeFilter->Append(".dds");
	openPicker->FileTypeFilter->Append(".bmp");
	openPicker->FileTypeFilter->Append(".tga");openPicker->FileTypeFilter->Append(".mp4");

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions{};

	const unsigned int size = int(TextureIrradianceImage6->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{
				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFile6(file);
							vars->SetTextureImage6File(file->Path);
							_this->_rootPage->SetTextureImage6FileDXP(file->Path);
							_this->_rootPage->SetTextureImage6NameDXP(file->Name);
							vars->SetTextureImage6File(file->Path);
							vars->SetThumbnailDXP6(thumbnail);
							DisplayResult(_this->TextureIrradianceImage6, _this->filePath6TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP6(vars->GetThumbnailDXP6());
							vars->GetBitmapImageDXP6()->SetSource(vars->GetThumbnailDXP6());
							_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock6->Text = file->Path;
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


void Hot3dxRotoDraw::Scenario5_Tex::PicTextureImage2_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::VideoTextureImage2_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::PicTextureImage2_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;
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

	const unsigned int size = int(PicTextureImage2->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{
				m_sPicDirPath = ref new Platform::String(file->Path->Data());
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

				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Hot3dxRotoDraw::Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFilePic1(file);
							vars->SetTextureImagePic1File(file->Path);
							_this->_rootPage->SetGridPicTextureImageFileDXP(file->Path);
							vars->SetTextureImagePic1File(file->Path);
							vars->SetThumbnailPicDXP1(thumbnail);
							_this->DisplayResult(_this->PicTextureImage2, _this->filePathPic1TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImagePicDXP1(vars->GetThumbnailPicDXP1());
							vars->GetBitmapImagePicDXP1()->SetSource(vars->GetThumbnailPicDXP1());
							_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
							_this->filePathPic1TextBlock->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file", NotifyType::ErrorMessage);

						}

					});
			}
			else {}
		});
}


void Hot3dxRotoDraw::Scenario5_Tex::VideoTextureImage2_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;
	openPicker->FileTypeFilter->Append(".mp4");
	
	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions{};

	const unsigned int size = int(VideoTextureImage2->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{
			if (file)
			{
				m_sVideoDirPath = ref new Platform::String(file->Path->Data());
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

				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Hot3dxRotoDraw::Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFileVideo(file);
							vars->SetTextureImageVideo2File(file->Path);
							_this->_rootPage->SetTextureImageVideo2FileDXP(file->Path);
							vars->SetTextureImageVideo2File(file->Path);
							_this->_rootPage->SetTextureImageVideo2NameDXP(file->Name);
							vars->SetThumbnailVideoDXP(thumbnail);
							_this->DisplayResult(_this->VideoTextureImage2, _this->filePathVideo2TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageVideoDXP1(vars->GetThumbnailVideoDXP1());
							vars->GetBitmapImageVideoDXP1()->SetSource(vars->GetThumbnailVideoDXP1());
							_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
							_this->filePathVideo2TextBlock->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file", NotifyType::ErrorMessage);

						}

					});
			}
			else {}
		});
}




void Hot3dxRotoDraw::Scenario5_Tex::IDC_D3DMATERIAL_DISPLAY_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}

void Hot3dxRotoDraw::Scenario5_Tex::IDC_PIC_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;
	if (IDC_PIC_CHECKBOX->IsChecked->Value)
	{
		vars->SetPicChecked(true);
		_rootPage->NotifyUser("Draw with Background Picture Picked true", NotifyType::StatusMessage);
	}
	else {
		vars->SetPicChecked(false);
		_rootPage->NotifyUser("Draw without Background Photo Picked false", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario5_Tex::IDC_GRID_OR_PIC_CHECKBOX_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;
	if (IDC_GRID_OR_PIC_CHECKBOX->IsChecked->Value)
	{
		vars->SetGridChecked(true);
		_rootPage->NotifyUser("Draw with Grid Picked true", NotifyType::StatusMessage);
	}
	else {
		vars->SetGridChecked(false);
		_rootPage->NotifyUser("Draw with Grid Picked false", NotifyType::StatusMessage);
	}
}

void Hot3dxRotoDraw::Scenario5_Tex::IDC_SHOW_BKGRND_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->DrawGridPicRectangleDXP();
}
