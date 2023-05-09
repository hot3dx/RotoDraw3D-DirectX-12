#include "pch.h"
#include "Scenario5_MatsTexs.xaml.h"

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

Scenario5_MatsTexs^ Scenario5_MatsTexs::Current = nullptr;

Hot3dxRotoDraw::Scenario5_MatsTexs::Scenario5_MatsTexs() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;
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
		LoadDefaultImage3(L"Assets\\Textures\\Floor_RMA.dds");
		
	}
	/// </summary>
	if (vars->GetBitmapImageDXP4())
	{
		// loads chosen mid right texture image
		LoadChosenImage4();
	}
	else
	{
		LoadDefaultImage4(L"Assets\\Textures\\Floor_Normal.dds");
		
	}
	if (vars->GetBitmapImageDXP5())
	{
		// loads chosen bottom left texture image
		LoadChosenImage5();
	}
	else
	{
		LoadDefaultImage5(L"Assets\\Textures\\WHITE_CUBEMAP_R.DDS");
		
	}
	if (vars->GetBitmapImageDXP6())
	{
		// loads chosen bottom right texture image
		LoadChosenImage6();
	}
	else
	{
		LoadDefaultImage6(L"Assets\\Textures\\WHITE_CUBEMAP_IR.DDS");
		
	}
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);

	Scenario5_MatsTexs::Current = this;
	_rootPage->m_Scene5Vars->SetScenario5Page(this);
}

Hot3dxRotoDraw::Scenario5_MatsTexs::~Scenario5_MatsTexs()
{

}

void Hot3dxRotoDraw::Scenario5_MatsTexs::DisplayResult(Image^ image, TextBlock^ textBlock, String^ thumbnailModeName,
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


void Hot3dxRotoDraw::Scenario5_MatsTexs::scrollBar_Scroll(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::ScrollEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadDefaultImage1(Platform::String^ imagePath)
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;
	
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

				unsigned int size = int(TextureImage1->ActualHeight);
				
				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
				{
					if (thumbnail != nullptr)
					{
						Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile1(file);
							vars->SetTextureImage1File(file->Path);
							_this->_rootPage->SetTextureImage1FileDXP(file->Path);
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
	
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadDefaultImage2(Platform::String^ imagePath)
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	// loads default top or bottom texture image
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

			unsigned int size = int(TextureImage2->ActualHeight);

			if (size > 0)
			{
				fastThumbnail = false;
				thumbnailOptions = ThumbnailOptions::UseCurrentScale;
			}
			Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
			create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
					vars->SetFile2(file);
						vars->SetTextureImage2File(file->Path);
						_this->_rootPage->SetTextureImage2FileDXP(file->Path);
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
					_this->_rootPage->NotifyUser("Error opening file ", NotifyType::ErrorMessage);

				}

			});
		}
		else {}
	});

	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}


void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadDefaultImage3(Platform::String^ imagePath)
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	// loads default top or bottom texture image
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

				unsigned int size = int(TextureRMAImage3->ActualHeight);

				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile3(file);
							vars->SetTextureImage3File(file->Path);
							_this->_rootPage->SetTextureImage3FileDXP(file->Path);
							vars->SetTextureImage3File(file->Path);
							vars->SetThumbnailDXP3(thumbnail);
							DisplayResult(_this->TextureRMAImage3, _this->filePath3TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP3(vars->GetThumbnailDXP3());
							vars->GetBitmapImageDXP3()->SetSource(vars->GetThumbnailDXP3());
							_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock3->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadDefaultImage4(Platform::String^ imagePath)
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	// loads default top or bottom texture image
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

				unsigned int size = int(TextureEMMisiveImage4->ActualHeight);

				if (size > 0)
				{
					fastThumbnail = false;
					thumbnailOptions = ThumbnailOptions::UseCurrentScale;
				}
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile4(file);
							vars->SetTextureImage4File(file->Path);
							_this->_rootPage->SetTextureImage4FileDXP(file->Path);
							vars->SetTextureImage4File(file->Path);
							vars->SetThumbnailDXP4(thumbnail);
							DisplayResult(_this->TextureEMMisiveImage4, _this->filePath4TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP4(vars->GetThumbnailDXP4());
							vars->GetBitmapImageDXP4()->SetSource(vars->GetThumbnailDXP4());
							_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock4->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadDefaultImage5(Platform::String^ imagePath)
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;


	Platform::String^ s = ref new Platform::String();
	s = _rootPage->GetProgramDirPathDXP();
	Platform::String^ sfile = ref new Platform::String(s->Data());
	Platform::String^ sfiles = sfile->Concat(sfile, imagePath);

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(sfiles));

	create_task(file->GetFileFromPathAsync(sfiles)).then([this](StorageFile^ file)
		{
			if (file)
			{
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
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile5(file);
							vars->SetTextureImage5File(file->Path);
							_this->_rootPage->SetTextureImage5FileDXP(file->Path);
							vars->SetTextureImage5File(file->Path);
							vars->SetThumbnailDXP5(thumbnail);
							DisplayResult(_this->TextureRadianceImage5, _this->filePath5TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP5(vars->GetThumbnailDXP5());
							vars->GetBitmapImageDXP5()->SetSource(vars->GetThumbnailDXP5());

							_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock5->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadDefaultImage6(Platform::String^ imagePath)
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;


	Platform::String^ s = ref new Platform::String();
	s = _rootPage->GetProgramDirPathDXP();
	Platform::String^ sfile = ref new Platform::String(s->Data());
	Platform::String^ sfile1 = sfile->Concat(sfile, imagePath);

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(sfile1));

	create_task(file->GetFileFromPathAsync(sfile1)).then([this](StorageFile^ file)
		{
			if (file)
			{
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
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile6(file);
							vars->SetTextureImage6File(file->Path);
							_this->_rootPage->SetTextureImage6FileDXP(file->Path);
							vars->SetTextureImage6File(file->Path);
							vars->SetThumbnailDXP6(thumbnail);
							DisplayResult(_this->TextureIrradianceImage6, _this->filePath6TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageDXP6(vars->GetThumbnailDXP6());
							vars->GetBitmapImageDXP6()->SetSource(vars->GetThumbnailDXP6());

							_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
							_this->textureFileTextBlock6->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadChosenImage1()
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;
	
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
					DisplayResult(this->TextureImage1, this->filePath1TextBlock, thumbnailModeName, file, thumbnail, false);
					
					this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock1->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadChosenImage2()
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;
	
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
					DisplayResult(this->TextureImage2, this->filePath2TextBlock, thumbnailModeName, file, thumbnail, false);
					
					this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock2->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadChosenImage3()
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

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
					DisplayResult(this->TextureRMAImage3, this->filePath3TextBlock, thumbnailModeName, file, thumbnail, false);

					this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock3->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadChosenImage4()
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

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
					DisplayResult(this->TextureEMMisiveImage4, this->filePath4TextBlock, thumbnailModeName, file, thumbnail, false);

					this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock4->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadChosenImage5()
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

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
					DisplayResult(this->TextureRadianceImage5, this->filePath5TextBlock, thumbnailModeName, file, thumbnail, false);

					this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock5->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadChosenImage6()
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

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
					DisplayResult(this->TextureIrradianceImage6, this->filePath6TextBlock, thumbnailModeName, file, thumbnail, false);

					this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock6->Text = file->Path;
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

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadDefaultImages(unsigned int imageIdNum, 
	Platform::String^ imagePath)
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;
	imageIdNums = imageIdNum;

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
				
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							
							switch (_this->imageIdNums)
							{
							case 1: {
								vars->SetFile1(file);
								vars->SetTextureImage1File(file->Path);
								_this->_rootPage->SetTextureImage1FileDXP(file->Path);
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
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadChosenImages()
{
	throw ref new Platform::NotImplementedException();
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::SetSingleMaterialTypesStringListboxItem(Platform::Object^ sender, unsigned int i, MaterialList const& s, MaterialTypes m,
	     Windows::UI::Xaml::Controls::TextBlock^ textBlockL)
{
	
	ListBox^ materialListBox = safe_cast<ListBox^>(sender); //as ListBox;
	ListBoxItem^ item = dynamic_cast<ListBoxItem^>(materialListBox->SelectedItem);
	if (item != nullptr)
	{
		// Clear the status block when changing scenarios
		_rootPage->NotifyUser("index: " + i + " of: " + s.dataName, NotifyType::StatusMessage);

		// Navigate to the selected scenario.
		TypeName materialType = { item->Name, TypeKind::Custom };
	}
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::SetMaterialTypesStrings()
{
	
	// Populate the ListBox with the scenarios as defined in SampleConfiguration.cpp.
	auto itemCollection = ref new Platform::Collections::Vector<Object^>();
	int i = 1;
	MaterialTypes m{};
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	for (auto const& s : materials)// DirectXPage::Current->materials)
	{
		// Create a textBlock to hold the content and apply the ListItemTextStyle from Styles.xaml
		textBlock = ref new TextBlock();
		ListBoxItem^ item = ref new ListBoxItem();
		auto style = App::Current->Resources->Lookup("ListItemTextStyle");

		switch (i)
		{
		case 1:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 2:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 3:
		{
			textBlock->Text = s.Title; i++;

		}break;
		case 4:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 5:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 6:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 7:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 8:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 9:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 10:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 11:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 12:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 13:
		{
			textBlock->Text = s.Title; i++;
		}break;

		case 14:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 15:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 16:
		{
			textBlock->Text = s.Title; i++;
		}break;

		case 17:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 18:
		{
			textBlock->Text = s.Title; i++;
		}break;
		case 19:
		{
			textBlock->Text = s.Title; i++;
		}break;
		default: break;

		}
		//textBlock->Text = s.Title + ;//(i++).ToString() + ") " + s.Title;
		textBlock->Style = safe_cast<Windows::UI::Xaml::Style^>(style);

		item->Name = s.dataName;
		item->Content = textBlock;
		itemCollection->Append(item);
	}

	// Set the newly created itemCollection as the ListBox ItemSource.
	MaterialListControl->ItemsSource = itemCollection;
	
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::SetMaterialTypesDataStrings()
{

	// Populate the ListBox with the scenarios as defined in SampleConfiguration.cpp.
	auto itemCollection = ref new Platform::Collections::Vector<Object^>();
	int i = 1;
	MaterialTypes m{};
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	for (auto const& s : materials)// DirectXPage::Current->materials)
	{
		// Create a textBlock to hold the content and apply the ListItemTextStyle from Styles.xaml
		textBlock = ref new TextBlock();
		ListBoxItem^ item = ref new ListBoxItem();
		auto style = App::Current->Resources->Lookup("ListItemTextStyle");

		switch (i)
		{
		case 1:
		{
			m.matName = vars->GetMatName1()->ToString();
			textBlock->Text = m.matName; i++;
		}break;
		case 2:
		{
			m.A = ref new Platform::String(std::to_wstring(_rootPage->GetFrontColorDXP().A).c_str());
			textBlock->Text = m.A; i++;
		}break;
		case 3:
		{
			m.R = ref new Platform::String(std::to_wstring(_rootPage->GetFrontColorDXP().R).c_str());
			textBlock->Text = m.R; i++;

		}break;
		case 4:
		{
			m.G = ref new Platform::String(std::to_wstring(_rootPage->GetFrontColorDXP().G).c_str());
			textBlock->Text = m.G; i++;
		}break;
		case 5:
		{
			m.B = ref new Platform::String(std::to_wstring(_rootPage->GetFrontColorDXP().B).c_str());
			textBlock->Text = m.B; i++;
		}break;
		case 6:
		{
			m.Power = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(0)).c_str());
			textBlock->Text = m.Power; i++;
		}break;
		case 7:
		{
			m.Alpha = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(1)).c_str());
			textBlock->Text = m.Alpha; i++;
		}break;
		case 8:
		{
			m.SpecRed = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(2)).c_str());
			textBlock->Text = m.SpecRed; i++;
		}break;
		case 9:
		{
			m.SpecGreen = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(3)).c_str());
			textBlock->Text = m.SpecGreen; i++;
		}break;
		case 10:
		{
			m.SpecBlue = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(4)).c_str());
			textBlock->Text = m.SpecBlue; i++;
		}break;
		case 11:
		{
			m.AmbientRed = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(5)).c_str());
			textBlock->Text = m.AmbientRed; i++;
		}break;
		case 12:
		{
			m.AmbientGreen = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(6)).c_str());
			textBlock->Text = m.AmbientGreen; i++;
		}break;
		case 13:
		{
			m.AmbientBlue = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(7)).c_str());
			textBlock->Text = m.AmbientBlue; i++;
		}break;

		case 14:
		{
			m.EmmissiveRed = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(8)).c_str());
			textBlock->Text = m.EmmissiveRed; i++;
		}break;
		case 15:
		{
			m.EmmissiveGreen = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(9)).c_str());
			textBlock->Text = m.EmmissiveGreen; i++;
		}break;
		case 16:
		{
			m.EmmissiveBlue = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(10)).c_str());
			textBlock->Text = m.EmmissiveBlue; i++;
		}break;

		case 17:
		{
			m.DiffuseRed = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(11)).c_str());
			textBlock->Text = m.DiffuseRed; i++;
		}break;
		case 18:
		{
			m.DiffuseGreen = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(12)).c_str());
			textBlock->Text = m.DiffuseGreen; i++;
		}break;
		case 19:
		{
			m.DiffuseBlue = ref new Platform::String(std::to_wstring(vars->GetMatArgs1Array()->get(13)).c_str());
			textBlock->Text = m.DiffuseBlue; i++;
		}break;
		
		default: break;

		}
		
		textBlock->Style = safe_cast<Windows::UI::Xaml::Style^>(style);

		item->Name = s.dataName;
		item->Content = textBlock;
		itemCollection->Append(item);
	}

	// Set the newly created itemCollection as the ListBox ItemSource.
	MaterialListControlData->ItemsSource = itemCollection;

}

void Hot3dxRotoDraw::Scenario5_MatsTexs::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	Scenario5_MatsTexs::Current = this;
	_rootPage->m_Scene5Vars->SetScenario5Page(this);

	// Populate the ListBox with the scenarios as defined in SampleConfiguration.cpp.
	SetMaterialTypesStrings();
	SetMaterialTypesDataStrings();

	int startingScenarioIndex;

	if (Window::Current->Bounds.Width < 640)
	{
		startingScenarioIndex = -1;
	}
	else
	{
		startingScenarioIndex = 0;
	}

	MaterialListControl->SelectedIndex = startingScenarioIndex;
	MaterialListControl->ScrollIntoView(MaterialListControl->SelectedItem);
	MaterialListControlData->SelectedIndex = startingScenarioIndex;
	MaterialListControlData->ScrollIntoView(MaterialListControlData->SelectedItem);

	
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::MaterialListControl_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	ListBox^ materialListBox = safe_cast<ListBox^>(sender); //as ListBox;
	ListBoxItem^ item = dynamic_cast<ListBoxItem^>(materialListBox->SelectedItem);
	if (item != nullptr)
	{
		// Clear the status block when changing scenarios
		_rootPage->NotifyUser("", NotifyType::StatusMessage);

		// Navigate to the selected scenario.
		TypeName materialType = { item->Name, TypeKind::Custom };
	}
	auto index = materialListBox->SelectedIndex;
	MaterialListControl->SelectedIndex = index;
	MaterialListControl->ScrollIntoView(MaterialListControl->SelectedItem);
	MaterialListControlData->SelectedIndex = index;
	MaterialListControlData->ScrollIntoView(MaterialListControlData->SelectedItem);
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::MaterialListControlData_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	ListBox^ materialListBox = safe_cast<ListBox^>(sender); //as ListBox;
	ListBoxItem^ item = dynamic_cast<ListBoxItem^>(materialListBox->SelectedItem);
	if (item != nullptr)
	{
		// Clear the status block when changing scenarios
		_rootPage->NotifyUser("", NotifyType::StatusMessage);

		// Navigate to the selected scenario.
		TypeName materialType = { item->Name, TypeKind::Custom };
	}
	auto index = materialListBox->SelectedIndex;
	MaterialListControl->SelectedIndex = index;
	MaterialListControl->ScrollIntoView(MaterialListControl->SelectedItem);
	MaterialListControlData->SelectedIndex = index;
	MaterialListControlData->ScrollIntoView(MaterialListControlData->SelectedItem);
}


/*
* Upper Add Texture button on Textures User Interface
*/
void Hot3dxRotoDraw::Scenario5_MatsTexs::IDC_TEXTURE_IMAGE1_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder | PickerLocationId::DocumentsLibrary;// | PickerLocationId::PicturesLibrary | PickerLocationId::Objects3D | PickerLocationId::VideosLibrary;
	openPicker->FileTypeFilter->Append(".jpg");
	openPicker->FileTypeFilter->Append(".jpeg");
	openPicker->FileTypeFilter->Append(".png");
	openPicker->FileTypeFilter->Append(".dds");
	openPicker->FileTypeFilter->Append(".bmp");
	openPicker->FileTypeFilter->Append(".tga");
	openPicker->FileTypeFilter->Append(".mp4");

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

				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile1(file);
							vars->SetTextureImage1File(file->Path);
							_this->_rootPage->SetTextureImage1FileDXP(file->Path);
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

/*
* Lower Add Texture button on Textures User Interface
*/
void Hot3dxRotoDraw::Scenario5_MatsTexs::IDC_TEXTURE_IMAGE2_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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
	openPicker->FileTypeFilter->Append(".mp4");

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
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile2(file);
							vars->SetTextureImage2File(file->Path);
							_this->_rootPage->SetTextureImage2FileDXP(file->Path);
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
							_this->_rootPage->NotifyUser("Error opening file ", NotifyType::ErrorMessage);
						}

					});
			}
			else {}
		});

	//_rootPage->TEXTURE_IMAGE1(sender, e);
}


void Hot3dxRotoDraw::Scenario5_MatsTexs::IDC_TEXTURE_RMA_IMAGE3_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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
	openPicker->FileTypeFilter->Append(".mp4");

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
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile3(file);
							vars->SetTextureImage3File(file->Path);
							_this->_rootPage->SetTextureImage3FileDXP(file->Path);
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


void Hot3dxRotoDraw::Scenario5_MatsTexs::IDC_TEXTURE_EMMISIVE_IMAGE4_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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
	openPicker->FileTypeFilter->Append(".mp4");

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
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile4(file);
							vars->SetTextureImage4File(file->Path);
							_this->_rootPage->SetTextureImage4FileDXP(file->Path);
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


void Hot3dxRotoDraw::Scenario5_MatsTexs::IDC_TEXTURE_RADIANCE_IMAGE5_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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
	openPicker->FileTypeFilter->Append(".mp4");

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
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile5(file);
							vars->SetTextureImage5File(file->Path);
							_this->_rootPage->SetTextureImage5FileDXP(file->Path);
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


void Hot3dxRotoDraw::Scenario5_MatsTexs::IDC_TEXTURE_IRRADIANCE_IMAGE6_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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
	openPicker->FileTypeFilter->Append(".mp4");

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
				Hot3dxRotoDraw::Scenario5_MatsTexs^ _this = this;
				create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Scenario5Vars^ vars = _this->_rootPage->m_Scene5Vars;
							vars->SetFile6(file);
							vars->SetTextureImage6File(file->Path);
							_this->_rootPage->SetTextureImage6FileDXP(file->Path);
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


void Hot3dxRotoDraw::Scenario5_MatsTexs::TextureImage1_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureImage1->BaseUri;

	

}

void Hot3dxRotoDraw::Scenario5_MatsTexs::TextureImage2_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureImage2->BaseUri;
}


void Hot3dxRotoDraw::Scenario5_MatsTexs::TextureImage3_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureRMAImage3->BaseUri;
	
}


void Hot3dxRotoDraw::Scenario5_MatsTexs::TextureImage4_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureEMMisiveImage4->BaseUri;
	
}


void Hot3dxRotoDraw::Scenario5_MatsTexs::TextureImage5_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureRadianceImage5->BaseUri;
	
}


void Hot3dxRotoDraw::Scenario5_MatsTexs::TextureImage6_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Uri^ uri = TextureIrradianceImage6->BaseUri;
	
}
