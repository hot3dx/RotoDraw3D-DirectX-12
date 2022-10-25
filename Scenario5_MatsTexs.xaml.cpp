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
		// loads chosen top texture image
		LoadChosenImage1();
	}
	else
	{
		LoadDefaultImage1();
	}
	if (vars->GetBitmapImageDXP2())
	{
		// loads chosen top texture image
		LoadChosenImage2();
	}
	else
	{
		LoadDefaultImage2();
	}
	
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);

	Scenario5_MatsTexs::Current = this;
	_rootPage->m_Scene5Vars->SetScenario5Page(this);
}

Hot3dxRotoDraw::Scenario5_MatsTexs::~Scenario5_MatsTexs()
{

}

void Hot3dxRotoDraw::Scenario5_MatsTexs::DisplayResult(Image^ image, TextBlock^ textBlock, String^ thumbnailModeName, size_t size, IStorageItem^ item, StorageItemThumbnail^ thumbnail, bool isGroup)
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


void Hot3dxRotoDraw::Scenario5_MatsTexs::scrollBar_Scroll(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::ScrollEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario5_MatsTexs::IDC_TEXTURE_IMAGE1_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::ComputerFolder;// :PicturesLibrary;
	openPicker->FileTypeFilter->Append(".jpg");
	openPicker->FileTypeFilter->Append(".jpeg");
	openPicker->FileTypeFilter->Append(".png");
	openPicker->FileTypeFilter->Append(".dds");
	openPicker->FileTypeFilter->Append(".bmp");
	openPicker->FileTypeFilter->Append(".tga");

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	const unsigned int size = int(TextureImage1->ActualHeight);
	if (size > 0)
	{
		bool fastThumbnail = false;
		ThumbnailOptions thumbnailOptions = ThumbnailOptions::UseCurrentScale;
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
					DisplayResult(_this->TextureImage1, _this->filePath1TextBlock, thumbnailModeName, size, file, thumbnail, false);
					vars->SetbitmapImageDXP1(vars->GetThumbnailDXP1());
					vars->GetBitmapImageDXP1()->SetSource(vars->GetThumbnailDXP1());				
					_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
					_this->textureFileTextBlock1->Text = file->Path;
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

	ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
	String^ thumbnailModeName = thumbnailMode.ToString();
	bool fastThumbnail = false;
	ThumbnailOptions thumbnailOptions;

	const unsigned int size = int(TextureImage2->ActualHeight);
	if (size > 0)
	{
		bool fastThumbnail = false;
		ThumbnailOptions thumbnailOptions = ThumbnailOptions::UseCurrentScale;
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
					_this->_rootPage->SetTextureImage2FileDXP(file->Path);
					vars->SetTextureImage2File(file->Path);
					vars->SetThumbnailDXP2(thumbnail);
					DisplayResult(_this->TextureImage2, _this->filePath2TextBlock, thumbnailModeName, size, file, thumbnail, false);
					vars->SetbitmapImageDXP2(vars->GetThumbnailDXP2());
					vars->GetBitmapImageDXP2()->SetSource(vars->GetThumbnailDXP2());
					_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
					_this->textureFileTextBlock2->Text = file->Path;
				}

				else
				{
					_this->_rootPage->NotifyUser("Error opening file" + file->Name, NotifyType::ErrorMessage);
				}

			});
		}
		else {}
	}); 
	
	_rootPage->TEXTURE_IMAGE2(sender, e);
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadDefaultImage1()
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;
	
		// loads default top image
		FileOpenPicker^ pick = ref new FileOpenPicker();

		TCHAR pwd[512];
		DWORD LENGTH = GetCurrentDirectory(512, pwd);
		Platform::String^ sfile = ref new Platform::String(pwd, LENGTH);

#ifdef _DEBUG
		//sfile = ref new Platform::String(L"C:\\Hot3dxRotoDraw\\x64\\Debug\\Hot3dxRotoDraw\\AppX\\Assets\\Textures\\Marble.dds");// vars->GetTextureImage1File();
		sfile = sfile->Concat(sfile, L"\\Assets\\Textures\\Marble.dds");// vars->GetTextureImage1File();
#endif // DEBUG
#ifdef NDEBUG
		//sfile = ref new Platform::String(L"C:\\Hot3dxRotoDraw\\x64\\Release\\Hot3dxRotoDraw\\AppX\\Assets\\Textures\\Marble.dds");// vars->GetTextureImage1File();
		sfile = sfile->Concat(sfile, L"\\Assets\\Textures\\Marble.dds");
#endif // NDEBUG		
		StorageFile^ file2 = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(sfile));

		create_task(file2->GetFileFromPathAsync(sfile)).then([this](StorageFile^ file)
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
					bool fastThumbnail = false;
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
							DisplayResult(_this->TextureImage1, _this->filePath1TextBlock, thumbnailModeName, size, file, thumbnail, false);
							vars->SetbitmapImageDXP1(vars->GetThumbnailDXP1());
							vars->GetBitmapImageDXP1()->SetSource(vars->GetThumbnailDXP1());
						
						_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
						_this->textureFileTextBlock1->Text = file->Path;
					}

					else
					{
						_this->_rootPage->NotifyUser("Error opening file" + file->Name, NotifyType::ErrorMessage);

					}

				});
			}
			else {}
		});
	
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::LoadDefaultImage2()
{
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	// loads default top or bottom texture image
	// loads default top image
	FileOpenPicker^ pick = ref new FileOpenPicker();
	
	TCHAR pwd[512];
	DWORD LENGTH = GetCurrentDirectory(512, pwd);
	Platform::String^ sfile = ref new Platform::String(pwd, LENGTH);

#ifdef _DEBUG
	//sfile = ref new Platform::String(L"C:\\Hot3dxRotoDraw\\x64\\Debug\\Hot3dxRotoDraw\\AppX\\Assets\\Textures\\Marble.dds");// vars->GetTextureImage1File();
	sfile = sfile->Concat(sfile, L"\\Assets\\Textures\\nightceiling.dds");// vars->GetTextureImage1File();
#endif // NDEBUG
#ifdef NDEBUG
	//sfile = ref new Platform::String(L"C:\\Hot3dxRotoDraw\\x64\\Release\\Hot3dxRotoDraw\\AppX\\Assets\\Textures\\nightceiling.dds");// vars->GetTextureImage1File();
	sfile = sfile->Concat(sfile, L"\\Assets\\Textures\\nightceiling.dds");
#endif // NDEBUG	
	StorageFile^ file2 = reinterpret_cast<StorageFile^>(pick->PickSingleFileAsync(sfile));

	create_task(file2->GetFileFromPathAsync(sfile)).then([this](StorageFile^ file)
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
				bool fastThumbnail = false;
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
						DisplayResult(_this->TextureImage2, _this->filePath2TextBlock, thumbnailModeName, size, file, thumbnail, false);
						vars->SetbitmapImageDXP2(vars->GetThumbnailDXP2());
						vars->GetBitmapImageDXP2()->SetSource(vars->GetThumbnailDXP2());
					_this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
					_this->textureFileTextBlock2->Text = file->Path;
				}

				else
				{
					_this->_rootPage->NotifyUser("Error opening file" + file->Name, NotifyType::ErrorMessage);

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
			bool fastThumbnail = false;
			thumbnailOptions = ThumbnailOptions::UseCurrentScale;
		}

		StorageFile^ file = vars->GetFile1();
		
		if (file)
		{
			create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					DisplayResult(this->TextureImage1, this->filePath1TextBlock, thumbnailModeName, size, file, thumbnail, false);
					
					this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock1->Text = file->Path;
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file" + file->Name, NotifyType::ErrorMessage);
				}
			});
		}
		else {
			this->_rootPage->NotifyUser("Error opening file" + file->Name, NotifyType::ErrorMessage);
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
			bool fastThumbnail = false;
			thumbnailOptions = ThumbnailOptions::UseCurrentScale;
		}

		StorageFile^ file = vars->GetFile2();
		
		if (file)
		{
			create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
			{
				if (thumbnail != nullptr)
				{
					DisplayResult(this->TextureImage2, this->filePath2TextBlock, thumbnailModeName, size, file, thumbnail, false);
					
					this->_rootPage->NotifyUser("Opened file" + file->Name, NotifyType::StatusMessage);
					this->textureFileTextBlock1->Text = file->Path;
				}
				else
				{
					this->_rootPage->NotifyUser("Error opening file" + file->Name, NotifyType::ErrorMessage);

				}
			});
		}
		else {
			this->_rootPage->NotifyUser("Error opening file" + file->Name, NotifyType::ErrorMessage);
		}
	
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::SetSingleMaterialTypesStringListboxItem(Platform::Object^ sender, unsigned int i, MaterialList const& s, MaterialTypes m,
	     Windows::UI::Xaml::Controls::TextBlock^ textBlock)
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
}

void Hot3dxRotoDraw::Scenario5_MatsTexs::SetMaterialTypesStrings()
{
	
	// Populate the ListBox with the scenarios as defined in SampleConfiguration.cpp.
	auto itemCollection = ref new Platform::Collections::Vector<Object^>();
	int i = 1;
	MaterialTypes m;
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	for (auto const& s : materials)// DirectXPage::Current->materials)
	{
		// Create a textBlock to hold the content and apply the ListItemTextStyle from Styles.xaml
		TextBlock^ textBlock = ref new TextBlock();
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
	MaterialTypes m;
	Scenario5Vars^ vars = _rootPage->m_Scene5Vars;

	for (auto const& s : materials)// DirectXPage::Current->materials)
	{
		// Create a textBlock to hold the content and apply the ListItemTextStyle from Styles.xaml
		TextBlock^ textBlock = ref new TextBlock();
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


