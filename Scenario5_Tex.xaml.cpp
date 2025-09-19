#include "pch.h"
#include "Scenario5_Tex.xaml.h"
#include "Common/DirectXHelper.h"

//In C++ / CX(used for Windows Runtime development), Platform::String is a special type, and converting it to lowercase requires a bit of work since it doesn't directly support case conversion methods. Here's how you can achieve it :

#include <cctype>
#include <cwctype>//Example Code Cpp 


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

	IDC_GRID_OR_PIC_CHECKBOX->IsChecked::set(true);
	
	IDC_PIC_CHECKBOX->IsChecked::set(false);
	
	vars->SetGridChecked(true);
	vars->SetPicChecked(false);

	m_sDefaultLoadFolderPath = _rootPage->Getm_sDirPathDXP();// "ms-appx:///Assets/Textures/";// 

	if (vars->GetBitmapImageDXP1())
	{
		// loads chosen top left texture image
		LoadChosenImage1();
	}
	else
	{
		LoadDefaultImageUri1(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/fire.dds");
		//LoadDefaultImage1(L"Assets\\Textures\\fire.dds");
	}
	if (vars->GetBitmapImageDXP2())
	{
		// loads chosen top right texture image
		LoadChosenImage2();
	}
	else
	{
		LoadDefaultImageUri2(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/sphere.png");
		// LoadDefaultImage2(L"Assets\\Textures\\sphere.png");

	}
	/// <summary>
	if (vars->GetBitmapImageDXP3())
	{
		// loads chosen mid left texture image
		LoadChosenImage3();
	}
	else
	{
		LoadDefaultImageUri3(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/Toyrobot_RMA.DDS");
		//LoadDefaultImage3(L"Assets\\Textures\\Toyrobot_RMA.DDS");

	}
	/// </summary>
	if (vars->GetBitmapImageDXP4())
	{
		// loads chosen mid right texture image
		LoadChosenImage4();
	}
	else
	{
		LoadDefaultImageUri4(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/Toyrobot_Normal.DDS");
		//LoadDefaultImage4(L"Assets\\Textures\\Toyrobot_Normal.DDS");

	}
	if (vars->GetBitmapImageDXP5())
	{
		// loads chosen bottom left texture image
		LoadChosenImage5();
	}
	else
	{
		LoadDefaultImageUri5(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/ATRIUM_R.DDS");
		//LoadDefaultImage5(L"Assets\\Textures\\ATRIUM_R.DDS");

	}
	if (vars->GetBitmapImageDXP6())
	{
		// loads chosen bottom right texture image
		LoadChosenImage6();
	}
	else
	{
		LoadDefaultImageUri6(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/ATRIUM_IR.DDS");
		//LoadDefaultImage6(L"Assets\\Textures\\ATRIUM_IR.DDS");

	}
	if (vars->GetBitmapImagePicDXP1())
	{
		// loads chosen bottom right texture image
		LoadChosenImagePic();
	}
	else
	{

		LoadDefaultImageUriPic(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/tree01S.dds");
		//LoadDefaultImagePic(L"Assets\\Textures\\tree01S.dds");

	}
	if (vars->GetBitmapImageVideoDXP1())
	{
		// loads chosen bottom right texture image
		LoadChosenImageVideo();
	}
	else
	{

		LoadDefaultImageUriVideo(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/SampleVideo.mp4");
		//LoadDefaultImageVideo(L"Assets\\Textures\\SampleVideo.mp4");

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
	textBlock->Text = item->Name + "\n";
}

void Hot3dxRotoDraw::Scenario5_Tex::DisplayResultVideo(Windows::UI::Xaml::Controls::Image^ image,
	Windows::UI::Xaml::Controls::TextBlock^ textBlock,
	Platform::String^ thumbnailModeName,
	Windows::Storage::IStorageItem^ item,
	Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail,
	bool isGroup)
{
	BitmapImage^ bitmapImage = ref new BitmapImage();

	bitmapImage->SetSource(thumbnail);
	image->Source = bitmapImage;

	String^ itemType = isGroup ? "Group" : item->IsOfType(StorageItemTypes::File) ? "File" : "Folder";
	textBlock->Text = item->Name + "\n";
}

void Hot3dxRotoDraw::Scenario5_Tex::scrollBar_Scroll(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::ScrollEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImage1(Platform::String^ imagePath)
{

	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	// loads default top or bottom texture image
	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

	Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(s));
	unsigned int size = static_cast<unsigned int>(this->TextureImage1->ActualHeight);
	concurrency::create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
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
				concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							_this->SetTexture1Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadDefault TextureImage1: "));
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
	concurrency::create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
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
				concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							_this->SetTexture2Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadDefault TextureImage2: "));
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
	concurrency::create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
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
				concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							_this->SetTexture3Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadDefault TextureImage3: "));
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
	concurrency::create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
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
				concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							_this->SetTexture4Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadDefault TextureImage4: "));
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
	concurrency::create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
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
				concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							_this->SetTexture5Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadDefault TextureImage5: "));
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
	concurrency::create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
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
				concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							_this->SetTexture6Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadDefault TextureImage6: "));
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
		concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->SetTexture1Parameters(file, this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadChosen TextureImage1: "));
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file LoadChosen1 ", NotifyType::ErrorMessage);
				}
			});
	}
	else {
		this->_rootPage->NotifyUser("Error opening file LoadChosen1 ", NotifyType::ErrorMessage);
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
		concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->SetTexture2Parameters(file, this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadChosen TextureImage2: "));
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
		concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->SetTexture3Parameters(file, this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadChosen TextureImage3: "));
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
		concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->SetTexture4Parameters(file, this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadChosen TextureImage4: "));
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
		concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->SetTexture5Parameters(file, this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadChosen TextureImage5: "));
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
		concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->SetTexture6Parameters(file, this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file LoadChosen TextureImage6: "));
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
	//m_sPicDirPath = ref new Platform::String(L"ms-appx:///Assets/Textures/");
	if (file)
	{
		concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->SetTexturePicParameters(file, this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file Chosen TextureImagePic: "));
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
	//m_sVideoDirPath = ref new Platform::String(L"ms-appx:///Assets/Textures/");
	if (file)
	{
		concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					this->SetTextureVideoParameters(file, this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file Chosen TextureImageVideo: "));
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
	concurrency::create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
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
				concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							_this->SetTexturePicParameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file Default TextureImagePic: "));
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
	Scenario5TexVars^ vars = _rootPage->m_Scene5TexVars;

	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	pick->ViewMode = PickerViewMode::Thumbnail;
	pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;

	Platform::String^ s = ref new Platform::String(GetImagePath(imagePath)->Data());

	StorageFile^ file = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(s));
	unsigned int size = int(this->VideoTextureImage2->ActualHeight);
	concurrency::create_task(file->GetFileFromPathAsync(s)).then([this, size](StorageFile^ file)
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
				concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
					{
						if (thumbnail != nullptr)
						{
							Hot3dxRotoDraw::Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
							vars->SetFileVideo(file);
							vars->SetTextureImageVideo2File(file->Path);
							_this->_rootPage->SetTextureImageVideo2FileDXP(file->Path);
							_this->_rootPage->SetTextureImageVideo2NameDXP(file->Name);
							vars->SetTextureImageVideo2File(file->Path);
							vars->SetThumbnailVideoDXP(thumbnail);
							_this->DisplayResultVideo(_this->VideoTextureImage2, _this->filePathVideo2TextBlock, thumbnailModeName, file, thumbnail, false);
							vars->SetbitmapImageVideoDXP1(vars->GetThumbnailVideoDXP1());
							vars->GetBitmapImageVideoDXP1()->SetSource(vars->GetThumbnailVideoDXP1());
							_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
							_this->textureFileVideoTextBlock2->Text = file->Path;
						}

						else
						{
							_this->_rootPage->NotifyUser("Error opening file LoadDefaultImageVideo", NotifyType::ErrorMessage);

						}

					});
			}
			else {
				this->_rootPage->NotifyUser("Error opening file LoadDefaultImageVideo", NotifyType::ErrorMessage);
			}

		});
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImageUri1(Platform::String^ imagePath)
{
	Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(imagePath);
	concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri))
		.then([this](Windows::Storage::StorageFile^ file)
			{
				if (file)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					unsigned int size = static_cast<unsigned int>(this->TextureImage1->ActualHeight);
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
					auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
						{
							if (thumbnail != nullptr)
							{
								_this->SetTexture1Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage1 TextureImage1: "));
							}
							else
							{
								_this->_rootPage->NotifyUser("fire.dds not found in App Package! Error opening file LoadDefaultImage2 ", NotifyType::ErrorMessage);
							}
						});
				}
				else {
					this->_rootPage->NotifyUser("fire.dds not found in app package!", NotifyType::ErrorMessage);
				}
			});
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImageUri2(Platform::String^ imagePath)
{
	Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(imagePath);
	concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri))
		.then([this](Windows::Storage::StorageFile^ file)
			{
				if (file)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					unsigned int size = static_cast<unsigned int>(this->TextureImage2->ActualHeight);
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
					auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
						{
							if (thumbnail != nullptr)
							{
								_this->SetTexture2Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage2 TextureImage2: "));
							}
							else
							{
								_this->_rootPage->NotifyUser("sphere.png not found in App Package! Error opening file LoadDefaultImage2 ", NotifyType::ErrorMessage);
							}
						});
				}
				else {
					this->_rootPage->NotifyUser("sphere.png not found in app package!", NotifyType::ErrorMessage);
				}
			});
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImageUri3(Platform::String^ imagePath)
{
	Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(imagePath);
	concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri))
		.then([this](Windows::Storage::StorageFile^ file)
			{
				if (file)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					unsigned int size = static_cast<unsigned int>(this->TextureRMAImage3->ActualHeight);
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
					auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
						{
							if (thumbnail != nullptr)
							{
								_this->SetTexture3Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage3 TextureRMAImage3: "));
							}
							else
							{
								_this->_rootPage->NotifyUser("Toyrobot_RMA.DDS not found in App Package! Error opening file LoadDefaultImage3 ", NotifyType::ErrorMessage);
							}
						});
				}
				else {
					this->_rootPage->NotifyUser("Toyrobot_RMA.DDS not found in app package!", NotifyType::ErrorMessage);
				}
			});
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImageUri4(Platform::String^ imagePath)
{

	Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(imagePath);
	concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri))
		.then([this](Windows::Storage::StorageFile^ file)
			{
				if (file)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					unsigned int size = static_cast<unsigned int>(this->TextureEMMisiveImage4->ActualHeight);
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
					auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
						{
							if (thumbnail != nullptr)
							{
								_this->SetTexture4Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage4 TextureEMMisiveImage4: "));
							}
							else
							{
								_this->_rootPage->NotifyUser("Toyrobot_Normal.DDS not found in App Package! Error opening file LoadDefaultImage4 ", NotifyType::ErrorMessage);
							}
						});
				}
				else {
					this->_rootPage->NotifyUser("Toyrobot_Normal.DDS not found in app package!", NotifyType::ErrorMessage);
				}
			});

}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImageUri5(Platform::String^ imagePath)
{
	Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(imagePath);
	concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri))
		.then([this](Windows::Storage::StorageFile^ file)
			{
				if (file)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					unsigned int size = static_cast<unsigned int>(this->TextureRadianceImage5->ActualHeight);
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
					auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
						{
							if (thumbnail != nullptr)
							{
								_this->SetTexture5Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImageRadiance TextureImageRaiance: "));
							}
							else
							{
								_this->_rootPage->NotifyUser("ATRIUM_R.DDS not found in app package! Error opening file LoadDefaultImageRadiance ", NotifyType::ErrorMessage);
							}
						});
				}
				else {
					this->_rootPage->NotifyUser("ATRIUM_R.DDS not found in app package!", NotifyType::ErrorMessage);
				}
			});
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImageUri6(Platform::String^ imagePath)
{
	Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(imagePath);
	concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri))
		.then([this](Windows::Storage::StorageFile^ file)
			{
				if (file)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					unsigned int size = static_cast<unsigned int>(this->TextureIrradianceImage6->ActualHeight);
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
					auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
						{
							if (thumbnail != nullptr)
							{
								_this->SetTexture6Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImageIRRadiance TextureImageIRRadiance: "));
							}
							else
							{
								_this->_rootPage->NotifyUser("ATRIUM_IR.DDS not found in app package! Error opening file LoadDefaultImageIrradiance ", NotifyType::ErrorMessage);
							}
						});
				}
				else {
					this->_rootPage->NotifyUser("ATRIUM_IR.DDS not found in app package! Error opening file LoadDefaultImageIrradiance ", NotifyType::ErrorMessage);
				}
			});
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImageUriPic(Platform::String^ imagePath)
{
	Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(imagePath);
	concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri))
		.then([this](Windows::Storage::StorageFile^ file)
			{
				if (file)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					unsigned int size = static_cast<unsigned int>(this->PicTextureImage2->ActualHeight);
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
					auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
						{
							if (thumbnail != nullptr)
							{
								_this->SetTexturePicParameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImagePic TextureImagePic: "));
							}
							else
							{
								_this->_rootPage->NotifyUser("tree01S.dds not found in app package! Error opening file LoadDefaultImagePic ", NotifyType::ErrorMessage);
							}
						});
				}
				else {
					this->_rootPage->NotifyUser("tree01S.dds not found in app package! Error opening file LoadDefaultImage5 ", NotifyType::ErrorMessage);
				}
			});
}

void Hot3dxRotoDraw::Scenario5_Tex::LoadDefaultImageUriVideo(Platform::String^ imagePath)
{
	Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(imagePath);
	concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri))
		.then([this](Windows::Storage::StorageFile^ file)
			{
				if (file)
				{
					Scenario5TexVars^ vars = this->_rootPage->m_Scene5TexVars;
					unsigned int size = static_cast<unsigned int>(this->VideoTextureImage2->ActualHeight);
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
					auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
						{
							if (thumbnail != nullptr)
							{
								_this->SetTextureVideoParameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage Video TextureImageVideo: "));
							}
							else
							{
								_this->_rootPage->NotifyUser("SampleVideo.mp4 not found in app package! Error opening file LoadDefaultImageVideo ", NotifyType::ErrorMessage);
							}
						});
				}
				else {
					this->_rootPage->NotifyUser("SampleVideo.mp4 not found in app package! Error opening file LoadDefaultImage5 ", NotifyType::ErrorMessage);
				}
			});
}

// Example: Load thumbnail for a file and set it to the Image control
void Hot3dxRotoDraw::Scenario5_Tex::LoadThumbnail(Platform::String^ filePath, Platform::String^ imageCtrlName)
{
	create_task(StorageFile::GetFileFromPathAsync(filePath)).then([this, imageCtrlName](StorageFile^ file)
		{
			if (file)
			{

				Hot3dxRotoDraw::Scenario5_Tex^ _this = this;
				concurrency::create_task(file->GetThumbnailAsync(Windows::Storage::FileProperties::ThumbnailMode::PicturesView))
					.then([_this](Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail)
						{
							if (thumbnail)
							{
								auto bitmapImage = ref new BitmapImage();
								bitmapImage->SetSource(thumbnail);
								//auto ThumbnailImage = (Windows::UI::Xaml::Controls::Image^)thumbnail; 
								_this->TextureImage1->Source = bitmapImage; // ThumbnailImage is the x:Name of your Image control
							}
						});
			}
		});
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
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;// KnownFolders::PicturesLibrary
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

	concurrency::create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{

			if (file)
			{
				//Platform::String^ s = ref new Platform::String(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/");
				//s = s->Concat(s, file->Name);
				//Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(s);
				Scenario5_Tex^ _this = this;
				Windows::Storage::StorageFolder^ folder = KnownFolders::PicturesLibrary;
				Windows::System::User^ user = Windows::System::User::GetDefault();
				/*
				concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								//Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
								//unsigned int size = static_cast<unsigned int>(_this->TextureImage1->ActualHeight);
								ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ thumbnailModeName = thumbnailMode.ToString();
								bool fastThumbnail = false;
								ThumbnailOptions thumbnailOptions;

								if (size > 0)
								{
									fastThumbnail = false;
									thumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
									{
				*/
				concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								ThumbnailMode innerThumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ innerThumbnailModeName = innerThumbnailMode.ToString();
								bool innerFastThumbnail = false;
								ThumbnailOptions innerThumbnailOptions;

								if (size > 0)
								{
									innerFastThumbnail = false;
									innerThumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(innerThumbnailMode, size, innerThumbnailOptions))
									.then([_this, file, innerThumbnailMode, innerThumbnailModeName, innerThumbnailOptions, innerFastThumbnail, size](StorageItemThumbnail^ thumbnail)
										{
			
								        if (thumbnail != nullptr)
										{
											_this->SetTexture1Parameters(file, _this, thumbnail, innerThumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage1 TextureImage1: "));
										}
										else
										{
											_this->_rootPage->NotifyUser("Image1 not found in App Package! Error opening file LoadDefaultImage1 ", NotifyType::ErrorMessage);
										}
									});
							}
							else {
								_this->_rootPage->NotifyUser("Image1 not found in app package!", NotifyType::ErrorMessage);
							}

						});
				this->InitializeAppFoldersSc5(file->Path, file->Name, 1);

			}
			else {}
		});
}

void Hot3dxRotoDraw::Scenario5_Tex::TextureImage2_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;// KnownFolders::PicturesLibrary
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

	concurrency::create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{

			if (file)
			{
				Scenario5_Tex^ _this = this;
				Windows::Storage::StorageFolder^ folder = KnownFolders::PicturesLibrary;
				Windows::System::User^ user = Windows::System::User::GetDefault();
				/* concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								//Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
								unsigned int size = static_cast<unsigned int>(_this->TextureImage2->ActualHeight);
								ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ thumbnailModeName = thumbnailMode.ToString();
								bool fastThumbnail = false;
								ThumbnailOptions thumbnailOptions;

								if (size > 0)
								{
									fastThumbnail = false;
									thumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
									{
				*/
				concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								ThumbnailMode innerThumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ innerThumbnailModeName = innerThumbnailMode.ToString();
								bool innerFastThumbnail = false;
								ThumbnailOptions innerThumbnailOptions;

								if (size > 0)
								{
									innerFastThumbnail = false;
									innerThumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(innerThumbnailMode, size, innerThumbnailOptions))
									.then([_this, file, innerThumbnailMode, innerThumbnailModeName, innerThumbnailOptions, innerFastThumbnail, size](StorageItemThumbnail^ thumbnail)
										{
										if (thumbnail != nullptr)
										{
											_this->SetTexture2Parameters(file, _this, thumbnail, innerThumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage2 TextureImage2: "));
										}
										else
										{
											_this->_rootPage->NotifyUser("Image2 not found in App Package! Error opening file LoadDefaultImage2 ", NotifyType::ErrorMessage);
										}
									});
							}
							else {
								_this->_rootPage->NotifyUser("Image2 not found in app package!", NotifyType::ErrorMessage);
							}

						});
				this->InitializeAppFoldersSc5(file->Path, file->Name, 2);

			}
			else {}
		});
}



void Hot3dxRotoDraw::Scenario5_Tex::TextureRMAImage3_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;// KnownFolders::PicturesLibrary
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

	concurrency::create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{

			if (file)
			{
				Scenario5_Tex^ _this = this;
				Windows::Storage::StorageFolder^ folder = KnownFolders::PicturesLibrary;
				Windows::System::User^ user = Windows::System::User::GetDefault();
				/* concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								//Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
								unsigned int size = static_cast<unsigned int>(_this->TextureRMAImage3->ActualHeight);
								ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ thumbnailModeName = thumbnailMode.ToString();
								bool fastThumbnail = false;
								ThumbnailOptions thumbnailOptions;

								if (size > 0)
								{
									fastThumbnail = false;
									thumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task3 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
									{
					*/
				concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								ThumbnailMode innerThumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ innerThumbnailModeName = innerThumbnailMode.ToString();
								bool innerFastThumbnail = false;
								ThumbnailOptions innerThumbnailOptions;

								if (size > 0)
								{
									innerFastThumbnail = false;
									innerThumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(innerThumbnailMode, size, innerThumbnailOptions))
									.then([_this, file, innerThumbnailMode, innerThumbnailModeName, innerThumbnailOptions, innerFastThumbnail, size](StorageItemThumbnail^ thumbnail)
										{
										if (thumbnail != nullptr)
										{
											_this->SetTexture3Parameters(file, _this, thumbnail, innerThumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage3 TextureImage3: "));
										}
										else
										{
											_this->_rootPage->NotifyUser("Image3 not found in App Package! Error opening file LoadDefaultImage3 ", NotifyType::ErrorMessage);
										}
									});
							}
							else {
								_this->_rootPage->NotifyUser("Image3 not found in app package!", NotifyType::ErrorMessage);
							}

						});
				this->InitializeAppFoldersSc5(file->Path, file->Name, 3);

			}
			else {}
		});
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureEMMisiveImage4_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;// KnownFolders::PicturesLibrary
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

	concurrency::create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{

			if (file)
			{
				Scenario5_Tex^ _this = this;
				Windows::Storage::StorageFolder^ folder = KnownFolders::PicturesLibrary;
				Windows::System::User^ user = Windows::System::User::GetDefault();
				/* concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								//Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
								unsigned int size = static_cast<unsigned int>(_this->TextureEMMisiveImage4->ActualHeight);
								ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ thumbnailModeName = thumbnailMode.ToString();
								bool fastThumbnail = false;
								ThumbnailOptions thumbnailOptions;

								if (size > 0)
								{
									fastThumbnail = false;
									thumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task4 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
									{
									*/
				concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								ThumbnailMode innerThumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ innerThumbnailModeName = innerThumbnailMode.ToString();
								bool innerFastThumbnail = false;
								ThumbnailOptions innerThumbnailOptions;

								if (size > 0)
								{
									innerFastThumbnail = false;
									innerThumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(innerThumbnailMode, size, innerThumbnailOptions))
									.then([_this, file, innerThumbnailMode, innerThumbnailModeName, innerThumbnailOptions, innerFastThumbnail, size](StorageItemThumbnail^ thumbnail)
										{
										if (thumbnail != nullptr)
										{
											_this->SetTexture4Parameters(file, _this, thumbnail, innerThumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage4 TextureImage4: "));
										}
										else
										{
											_this->_rootPage->NotifyUser("Image4 not found in App Package! Error opening file LoadDefaultImage4 ", NotifyType::ErrorMessage);
										}
									});
							}
							else {
								_this->_rootPage->NotifyUser("Image4 not found in app package!", NotifyType::ErrorMessage);
							}

						});
				this->InitializeAppFoldersSc5(file->Path, file->Name, 4);

			}
			else {}
		});
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureRadianceImage5_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;// KnownFolders::PicturesLibrary
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

	concurrency::create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{

			if (file)
			{
				Scenario5_Tex^ _this = this;
				Windows::Storage::StorageFolder^ folder = KnownFolders::PicturesLibrary;
				Windows::System::User^ user = Windows::System::User::GetDefault();
				/*  concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								//Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
								unsigned int size = static_cast<unsigned int>(_this->TextureRadianceImage5->ActualHeight);
								ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ thumbnailModeName = thumbnailMode.ToString();
								bool fastThumbnail = false;
								ThumbnailOptions thumbnailOptions;

								if (size > 0)
								{
									fastThumbnail = false;
									thumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task5 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
									{
								*/
									concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
									.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
										{
											if (file)
											{
												ThumbnailMode innerThumbnailMode = ThumbnailMode::SingleItem;
												Platform::String^ innerThumbnailModeName = innerThumbnailMode.ToString();
												bool innerFastThumbnail = false;
												ThumbnailOptions innerThumbnailOptions;

												if (size > 0)
												{
													innerFastThumbnail = false;
													innerThumbnailOptions = ThumbnailOptions::UseCurrentScale;
												}
												auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(innerThumbnailMode, size, innerThumbnailOptions))
													.then([_this, file, innerThumbnailMode, innerThumbnailModeName, innerThumbnailOptions, innerFastThumbnail, size](StorageItemThumbnail^ thumbnail)
														{
										if (thumbnail != nullptr)
										{
											_this->SetTexture5Parameters(file, _this, thumbnail, innerThumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage5 TextureImage5: "));
										}
										else
										{
											_this->_rootPage->NotifyUser("Image5 not found in App Package! Error opening file LoadDefaultImage5 ", NotifyType::ErrorMessage);
										}
									});
							}
							else {
								_this->_rootPage->NotifyUser("Image5 not found in app package!", NotifyType::ErrorMessage);
							}

						});
				this->InitializeAppFoldersSc5(file->Path, file->Name, 5);

			}
			else {}
		});
}


void Hot3dxRotoDraw::Scenario5_Tex::TextureIrradianceImage6_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;// KnownFolders::PicturesLibrary
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

	const unsigned int size = int(TextureIrradianceImage6->ActualHeight);
	if (size > 0)
	{
		fastThumbnail = false;
		thumbnailOptions = ThumbnailOptions::UseCurrentScale;
	}

	concurrency::create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{

			if (file)
			{
				Scenario5_Tex^ _this = this;
				Windows::Storage::StorageFolder^ folder = KnownFolders::PicturesLibrary;
				Windows::System::User^ user = Windows::System::User::GetDefault();
				/* concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								//Scenario6TexVars^ vars = _this->_rootPage->m_Scene6TexVars;
								unsigned int size = static_cast<unsigned int>(_this->TextureIrradianceImage6->ActualHeight);
								ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ thumbnailModeName = thumbnailMode.ToString();
								bool fastThumbnail = false;
								ThumbnailOptions thumbnailOptions;

								if (size > 0)
								{
									fastThumbnail = false;
									thumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task6 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
									{
										*/
											concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
											.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
												{
													if (file)
													{
														ThumbnailMode innerThumbnailMode = ThumbnailMode::SingleItem;
														Platform::String^ innerThumbnailModeName = innerThumbnailMode.ToString();
														bool innerFastThumbnail = false;
														ThumbnailOptions innerThumbnailOptions;

														if (size > 0)
														{
															innerFastThumbnail = false;
															innerThumbnailOptions = ThumbnailOptions::UseCurrentScale;
														}
														auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(innerThumbnailMode, size, innerThumbnailOptions))
															.then([_this, file, innerThumbnailMode, innerThumbnailModeName, innerThumbnailOptions, innerFastThumbnail, size](StorageItemThumbnail^ thumbnail)
																{
										if (thumbnail != nullptr)
										{
											_this->SetTexture6Parameters(file, _this, thumbnail, innerThumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage6 TextureImage6: "));
										}
										else
										{
											_this->_rootPage->NotifyUser("Image6 not found in App Package! Error opening file LoadDefaultImage6 ", NotifyType::ErrorMessage);
										}
									});
							}
							else {
								_this->_rootPage->NotifyUser("Image6 not found in app package!", NotifyType::ErrorMessage);
							}

						});
				this->InitializeAppFoldersSc5(file->Path, file->Name, 6);

			}
			else {}
		});
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

Windows::Foundation::Uri^ Hot3dxRotoDraw::Scenario5_Tex::GetAppImageUriFromString(Platform::String^ imagePath)
{
	Platform::String^ s1 = ref new Platform::String(L"ms-appx:///Assets/Textures/");
	Platform::String^ s = s1->Concat(s1, imagePath);
	Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(s);
	return uri;
}


// Add this before using IBufferByteAccess
#include <windows.h>
#include <Unknwn.h>

MIDL_INTERFACE("905a0fe1-bc53-11df-8c49-001e4fc686da")
IBufferByteAccess : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE Buffer(BYTE * *value) = 0;
};


//
// Upper Add Texture Right Click on Textures User Interface
//

void Hot3dxRotoDraw::Scenario5_Tex::TextureImage1_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	// Gets the name from an Image in the PicturesLibrary
	// and if it is in App Local
	// Lods the Image
	/*
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;// KnownFolders::PicturesLibrary
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

	concurrency::create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{

			if (file)
			{
				Platform::String^ s = ref new Platform::String(L"ms-appdata:///local/Hot3dxRotoDraw3D/Textures/");
					s = s->Concat(s, file->Name);
				Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(s);
				Scenario5_Tex^ _this = this;
				concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
								unsigned int size = static_cast<unsigned int>(_this->TextureImage1->ActualHeight);
								ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ thumbnailModeName = thumbnailMode.ToString();
								bool fastThumbnail = false;
								ThumbnailOptions thumbnailOptions;

								if (size > 0)
								{
									fastThumbnail = false;
									thumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
									{
										if (thumbnail != nullptr)
										{
											_this->SetTexture1Parameters(file, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage1 TextureImage1: "));
										}
										else
										{
											_this->_rootPage->NotifyUser("Image1 not found in App Package! Error opening file LoadDefaultImage1 ", NotifyType::ErrorMessage);
										}
									});
							}
							else {
								_this->_rootPage->NotifyUser("Image1 not found in app package!", NotifyType::ErrorMessage);
							}
						});

			}
			else {}
		});
	*/
}

void Hot3dxRotoDraw::Scenario5_Tex::TextureImage2_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::TextureRMAImage3_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::TextureNormalImage4_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::TextureRadianceImage5_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::TextureIrradianceImage6_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::PicTextureImage2_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;// KnownFolders::PicturesLibrary
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

	concurrency::create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{

			if (file)
			{
				Scenario5_Tex^ _this = this;
				Windows::Storage::StorageFolder^ folder = KnownFolders::PicturesLibrary;
				Windows::System::User^ user = Windows::System::User::GetDefault();
				/* concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								//Scenario6TexVars^ vars = _this->_rootPage->m_Scene6TexVars;
								unsigned int size = static_cast<unsigned int>(_this->PicTextureImage2->ActualHeight);
								ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ thumbnailModeName = thumbnailMode.ToString();
								bool fastThumbnail = false;
								ThumbnailOptions thumbnailOptions;

								if (size > 0)
								{
									fastThumbnail = false;
									thumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task6 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
									{
								*/
				concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								ThumbnailMode innerThumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ innerThumbnailModeName = innerThumbnailMode.ToString();
								bool innerFastThumbnail = false;
								ThumbnailOptions innerThumbnailOptions;

								if (size > 0)
								{
									innerFastThumbnail = false;
									innerThumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(innerThumbnailMode, size, innerThumbnailOptions))
									.then([_this, file, innerThumbnailMode, innerThumbnailModeName, innerThumbnailOptions, innerFastThumbnail, size](StorageItemThumbnail^ thumbnail)
										{
										if (thumbnail != nullptr)
										{
											_this->SetTexturePicParameters(file, _this, thumbnail, innerThumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage7 TextureImage7: "));
										}
										else
										{
											_this->_rootPage->NotifyUser("Image7 not found in App Package! Error opening file LoadDefaultImage6 ", NotifyType::ErrorMessage);
										}
									});
							}
							else {
								_this->_rootPage->NotifyUser("Image7 not found in app package!", NotifyType::ErrorMessage);
							}

						});
				this->InitializeAppFoldersSc5(file->Path, file->Name, 7);

			}
			else {}
		});
}


void Hot3dxRotoDraw::Scenario5_Tex::VideoTextureImage2_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::VideosLibrary;// KnownFolders::PicturesLibrary
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

	concurrency::create_task(openPicker->PickSingleFileAsync()).then([this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageFile^ file)
		{

			if (file)
			{
				Scenario5_Tex^ _this = this;
				Windows::Storage::StorageFolder^ folder = KnownFolders::PicturesLibrary;
				Windows::System::User^ user = Windows::System::User::GetDefault();
				/* concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								//Scenario6TexVars^ vars = _this->_rootPage->m_Scene6TexVars;
								unsigned int size = static_cast<unsigned int>(_this->VideoTextureImage2->ActualHeight);
								ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ thumbnailModeName = thumbnailMode.ToString();
								bool fastThumbnail = false;
								ThumbnailOptions thumbnailOptions;

								if (size > 0)
								{
									fastThumbnail = false;
									thumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task6 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
									{
									*/
				concurrency::create_task(file->GetFileFromPathForUserAsync(user, file->Path))
					.then([_this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
						{
							if (file)
							{
								ThumbnailMode innerThumbnailMode = ThumbnailMode::SingleItem;
								Platform::String^ innerThumbnailModeName = innerThumbnailMode.ToString();
								bool innerFastThumbnail = false;
								ThumbnailOptions innerThumbnailOptions;

								if (size > 0)
								{
									innerFastThumbnail = false;
									innerThumbnailOptions = ThumbnailOptions::UseCurrentScale;
								}
								auto task2 = concurrency::create_task(file->GetScaledImageAsThumbnailAsync(innerThumbnailMode, size, innerThumbnailOptions))
									.then([_this, file, innerThumbnailMode, innerThumbnailModeName, innerThumbnailOptions, innerFastThumbnail, size](StorageItemThumbnail^ thumbnail)
										{
										if (thumbnail != nullptr)
										{
											_this->SetTextureVideoParameters(file, _this, thumbnail, innerThumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImageVideo TextureImageVideo: "));
										}
										else
										{
											_this->_rootPage->NotifyUser(L"Image6 not found in App Package! Error opening file LoadDefaultImageVideo ", NotifyType::ErrorMessage);
										}
									});
							}
							else {
								_this->_rootPage->NotifyUser(L"ImageVideo not found in app package!", NotifyType::ErrorMessage);
							}

						});
				this->InitializeAppFoldersSc5(file->Path, file->Name, 8);

			}
			else {}
		});
}


void Hot3dxRotoDraw::Scenario5_Tex::PicTextureImage2_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_Tex::VideoTextureImage2_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{

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

void Hot3dxRotoDraw::Scenario5_Tex::SetTexture1Parameters(
	Windows::Storage::StorageFile^ file,
	Hot3dxRotoDraw::Scenario5_Tex^ _this,
	Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail,
	Platform::String^ thumbnailModeName,
	Platform::String^ message)
{
	if (!_this)
	{
		// _this is null, cannot safely dereference
		if (_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexture2Parameters (_this is null)", NotifyType::ErrorMessage);
		}
		return;
	}
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
	_this->_rootPage->NotifyUser(message + file->Name, NotifyType::StatusMessage);
	_this->textureFileTextBlock1->Text = file->Path;
}

void Hot3dxRotoDraw::Scenario5_Tex::SetTexture2Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message)
{
	if (!_this)
	{
		// _this is null, cannot safely dereference
		if (_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexture2Parameters (_this is null)", NotifyType::ErrorMessage);
		}
		return;
	}
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
	_this->_rootPage->NotifyUser(message + file->Name, NotifyType::StatusMessage);
	_this->textureFileTextBlock2->Text = file->Path;
}

void Hot3dxRotoDraw::Scenario5_Tex::SetTexture3Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message)
{
	if (!_this)
	{
		// _this is null, cannot safely dereference
		if (_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexture3Parameters (_this is null)", NotifyType::ErrorMessage);
		}
		return;
	}
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

void Hot3dxRotoDraw::Scenario5_Tex::SetTexture4Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message)
{
	if (!_this)
	{
		// _this is null, cannot safely dereference
		if (_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexture4Parameters (_this is null)", NotifyType::ErrorMessage);
		}
		return;
	}
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

void Hot3dxRotoDraw::Scenario5_Tex::SetTexture5Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message)
{
	if (!_this)
	{
		// _this is null, cannot safely dereference
		if (_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexture5Parameters (_this is null)", NotifyType::ErrorMessage);
		}
		return;
	}
	if (!file || !thumbnail || !_this->TextureImage1 || !_this->filePath5TextBlock)
	{
		if (_this->_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexture5Parameters", NotifyType::ErrorMessage);
		}
		return;
	}

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

void Hot3dxRotoDraw::Scenario5_Tex::SetTexture6Parameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message)
{
	if (!_this)
	{
		// _this is null, cannot safely dereference
		if (_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexture6Parameters (_this is null)", NotifyType::ErrorMessage);
		}
		return;
	}
	if (!file || !thumbnail || !_this->TextureIrradianceImage6 || !_this->filePath6TextBlock)
	{
		if (_this->_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexture6Parameters", NotifyType::ErrorMessage);
		}
		return;
	}

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

void Hot3dxRotoDraw::Scenario5_Tex::SetTexturePicParameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message)
{

	if (!_this)
	{
		// _this is null, cannot safely dereference
		if (_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexturePicParameters (_this is null)", NotifyType::ErrorMessage);
		}
		return;
	}
	if (!file || !thumbnail || !_this->PicTextureImage2 || !_this->filePathPic1TextBlock)
	{
		if (_this->_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTexturePicParameters", NotifyType::ErrorMessage);
		}
		return;
	}

	Hot3dxRotoDraw::Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
	vars->SetFilePic1(file);
	vars->SetTextureImagePic1File(file->Path);
	_this->_rootPage->SetGridPicTextureImageFileDXP(file->Path);
	_this->_rootPage->SetTextureImagePic2NameDXP(file->Name);
	vars->SetTextureImagePic1File(file->Path);
	vars->SetThumbnailPicDXP1(thumbnail);
	_this->DisplayResult(_this->PicTextureImage2, _this->filePathPic1TextBlock, thumbnailModeName, file, thumbnail, false);
	vars->SetThumbnailPicDXP1(thumbnail);
	vars->SetbitmapImagePicDXP1(vars->GetThumbnailPicDXP1());
	vars->GetBitmapImagePicDXP1()->SetSource(vars->GetThumbnailPicDXP1());
	//_this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
	//_this->textureFilePicTextBlock1->Text = file->Path;

	auto bitmapImage = vars->GetBitmapImagePicDXP1();
	auto thumb = vars->GetThumbnailPicDXP1();


	if (bitmapImage && thumb)
	{
		bitmapImage->SetSource(thumb);
		vars->SetThumbnailPicDXP1(thumb);
		vars->SetbitmapImagePicDXP1(thumb);
	}
	else
	{
		if (!_this->_rootPage)
		{
			_this->_rootPage->NotifyUser("Error: Pic BitmapImage or Thumbnail is null", NotifyType::ErrorMessage);
		}
	}
	_this->_rootPage->NotifyUser("Opened Pic file " + file->Name, NotifyType::StatusMessage);
	_this->textureFilePicTextBlock1->Text = file->Path;
}

void Hot3dxRotoDraw::Scenario5_Tex::SetTextureVideoParameters(Windows::Storage::StorageFile^ file, Hot3dxRotoDraw::Scenario5_Tex^ _this, Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, Platform::String^ thumbnailModeName, Platform::String^ message)
{
	if (!_this)
	{
		// _this is null, cannot safely dereference
		if (_rootPage)
		{
			_rootPage->NotifyUser("Error: Null object in SetTextureVideoParameters (_this is null)", NotifyType::ErrorMessage);
		}
		return;
	}
	Hot3dxRotoDraw::Scenario5TexVars^ vars = _this->_rootPage->m_Scene5TexVars;
	vars->SetFileVideo(file);
	vars->SetTextureImageVideo2File(file->Path);
	_this->_rootPage->SetTextureImageVideo2FileDXP(file->Path);
	_this->_rootPage->SetTextureImageVideo2NameDXP(file->Name);
	vars->SetTextureImageVideo2File(file->Path);
	vars->SetThumbnailVideoDXP(thumbnail);
	_this->DisplayResultVideo(_this->VideoTextureImage2, _this->filePathVideo2TextBlock, thumbnailModeName, file, thumbnail, false);

	auto thumb = vars->GetThumbnailVideoDXP1();
	auto bitmapImage = vars->GetBitmapImageVideoDXP1();
	if (bitmapImage && thumb)
	{
		bitmapImage->SetSource(thumb);
		vars->SetThumbnailVideoDXP(thumb);
		vars->SetbitmapImageVideoDXP1(thumb);
	}
	else
	{
		if (_this->_rootPage)
		{
			_this->_rootPage->NotifyUser("Error: Video BitmapImage or Thumbnail is null", NotifyType::ErrorMessage);
		}
	}
	_this->_rootPage->NotifyUser("Opened Video file " + file->Name, NotifyType::StatusMessage);
	_this->textureFileVideoTextBlock2->Text = file->Path;
}


Platform::String^ Hot3dxRotoDraw::Scenario5_Tex::ToLower(Platform::String^ input)
{
	// Convert Platform::String to std::wstring
	std::wstring wstr(input->Data());

	// Transform to lowercase using std::transform and std::towlower
	std::transform(wstr.begin(), wstr.end(), wstr.begin(), [](wchar_t c) {
		return std::towlower(c);
		});

	// Convert back to Platform::String
	return ref new Platform::String(wstr.c_str());
}

bool Hot3dxRotoDraw::Scenario5_Tex::EndsWithEqual(Platform::String^ input, const std::wstring& extension) {
	if (!input || extension.size() > input->Length()) {
		return false;
	}

	std::wstring inputStr(input->Data());
	return std::equal(extension.rbegin(), extension.rend(), inputStr.rbegin());
}

bool Hot3dxRotoDraw::Scenario5_Tex::EndsWithCompare(Platform::String^ input, const std::wstring& extension) {
	if (!input || extension.size() > input->Length()) {
		return false;
	}

	std::wstring inputStr(input->Data());
	return inputStr.compare(inputStr.size() - extension.size(), extension.size(), extension) == 0;
}

void Hot3dxRotoDraw::Scenario5_Tex::InitializeAppFoldersSc5(Platform::String^ sourcefilePath, Platform::String^ fileName, unsigned int parameterID)
{
	m_uiParameterID = parameterID;
	auto localFolder = GetAppDataLocalFolder(); //Windows::Storage::ApplicationData::Current->LocalFolder;

	auto texturesFolder = concurrency::create_task(
		localFolder->CreateFolderAsync(
			L"Hot3dxRotoDraw3D\\Textures",
			Windows::Storage::CreationCollisionOption::OpenIfExists)).then([this, sourcefilePath, fileName](StorageFolder^ folder) {
				// Folder created or opened
				// Continue app initialization
				this->CopyTextureToLocalFolderSc5(sourcefilePath, fileName, this->m_uiParameterID);

				});

	// You can now safely load textures from texturesFolder

}

void Hot3dxRotoDraw::Scenario5_Tex::CopyTextureToLocalFolderSc5(Platform::String^ sourcefilePath, Platform::String^ fileName, unsigned int parameterID)
{
	m_uiParameterID = parameterID;
	// 1. Get source file from app package
	Platform::String^ filen = ref new Platform::String(L"C:/Users/Pictures/");
	Platform::String^ fn = ref new Platform::String(fileName->Data());
	filen = filen->Concat(filen, fn);
	// Create the URI	
	auto srcUri = ref new Windows::Foundation::Uri(filen);
	concurrency::create_task(Windows::Storage::StorageFile::GetFileFromPathForUserAsync(Windows::System::User::GetDefault(), sourcefilePath))//:GetFileFromApplicationUriAsync(srcUri))
		.then([this, fileName](Windows::Storage::StorageFile^ srcFile) {

		// If the file could not be found, GetFileFromApplicationUriAsync will raise an exception.
		if (srcFile) {
			Platform::String^ f = ref new Platform::String(L"\nSource file found. ");
			f = f->Concat(f, fileName);
			f = f->Concat(f, L"  GetFileFromApplicationUriAsync(srcUri)\n");
			//OutputDebugString(f->Data());
		}
		else {
			Platform::String^ f = ref new Platform::String(L"\nSource file not found. ");
			f = f->Concat(f, fileName);
			f = f->Concat(f, L"  GetFileFromApplicationUriAsync(srcUri)\n");
			//OutputDebugString(f->Data());
		}

		Scenario5_Tex^ _this = this;
		auto localFolder = GetAppDataLocalFolder();
		// localFolder->Path; // for setting breakpoint
		// 2. Get destination folder (create if it doesn't exist)
		Platform::String^ destFolderPath = ref new Platform::String(localFolder->Path->Data());
		Platform::String^ destFolderPath2 = ref new Platform::String(L"Hot3dxRotoDraw3D\\Textures");
		return concurrency::create_task(localFolder->CreateFolderAsync(
			destFolderPath2,
			Windows::Storage::CreationCollisionOption::OpenIfExists
		)).then([srcFile, fileName](Windows::Storage::StorageFolder^ destFolder) {
			if (destFolder)
			{
				//OutputDebugString(L"\n Destination Folder Created or Opened\n");
			}
			else
			{
				//OutputDebugString(L"\n Destination Folder NOT Created or Opened\n");
			}
			return srcFile->CopyAsync(destFolder, fileName, Windows::Storage::NameCollisionOption::ReplaceExisting);
			});
			}).then([this](Windows::Storage::StorageFile^ copiedFile) {
				if (copiedFile)
				{
					// Success
					// NotifyUser("Copied: " + copiedFile->Path, NotifyType::StatusMessage);
					//this->NotifyUser(L"File Copied to App Folder", NotifyType::StatusMessage);
					//OutputDebugString(L"File Copied to App Folder");
					Scenario5_Tex^ _this = this;

					if (copiedFile)
					{
						unsigned int size = static_cast<unsigned int>(_this->TextureImage1->ActualHeight);
						ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
						Platform::String^ thumbnailModeName = thumbnailMode.ToString();
						bool fastThumbnail = false;
						ThumbnailOptions thumbnailOptions;

						if (size > 0)
						{
							fastThumbnail = false;
							thumbnailOptions = ThumbnailOptions::UseCurrentScale;
						}
						auto task2 = concurrency::create_task(copiedFile->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, copiedFile, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
							{
								if (thumbnail != nullptr)
								{
									switch (_this->m_uiParameterID)
									{
									case 1:
									{
										_this->SetTexture1Parameters(copiedFile, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage1 TextureImage1: "));
										break;
									}
									case 2:
									{
										_this->SetTexture2Parameters(copiedFile, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage2 TextureImage2: "));
										break;
									}
									case 3:
									{
										_this->SetTexture3Parameters(copiedFile, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage3 TextureImage3: "));
										break;
									}
									case 4:
									{
										_this->SetTexture4Parameters(copiedFile, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage4 TextureImage4: "));
										break;
									}
									case 5:
									{
										_this->SetTexture5Parameters(copiedFile, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage5 TextureImage5: "));
										break;
									}
									case 6:
									{
										_this->SetTexture6Parameters(copiedFile, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage6 TextureImage6: "));
										break;
									}
									case 7:
									{
										_this->SetTexturePicParameters(copiedFile, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage7 TextureImage7: "));
										break;
									}
									case 8:
									{
										_this->SetTextureVideoParameters(copiedFile, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImageVideo TextureImageVideo: "));
										break;
									}
									default:
									{
										_this->SetTexture1Parameters(copiedFile, _this, thumbnail, thumbnailModeName, ref new Platform::String(L"Opened file  LoadDefaultImage1 TextureImage1: "));
										break;
									}
									}
								}
								else
								{
									_this->_rootPage->NotifyUser("Image not found in App Package! Error opening file LoadDefaultImage ", NotifyType::ErrorMessage);
								}
							});
					}
					else {
						_this->_rootPage->NotifyUser("Image not found in app package!", NotifyType::ErrorMessage);
					}

				}
				}).then([this](concurrency::task<void> t) {

					try { t.get(); }
					catch (Platform::Exception^ ex) {
#ifdef DEBUG
						Platform::String^ f = L"\nCatch Source file not found. \n";
						if (!f) {
							//OutputDebugString(f->Data());//throw ref new Platform::Exception(E_FAIL, L"Source file not found.");
						}
#endif
						this->_rootPage->NotifyUser("Error: Catch Source file not found. " + ex->Message, NotifyType::ErrorMessage);
					}

					});
}

void Hot3dxRotoDraw::Scenario5_Tex::textureFileTextBlock1_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_rootPage->SetTextureImage1NameDXP(ref new Platform::String(filePath1TextBlock->Text->Data()));
}
