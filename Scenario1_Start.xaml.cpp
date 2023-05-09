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
#include "OmnidirectionalSound.h"
#include "Scenario1_Start.xaml.h"
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
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace concurrency;

Hot3dxRotoDraw::Scenario1_Start::Scenario1_Start() : _rootPage(DirectXPage::Current)
{
    
    InitializeComponent();
    Scenario5Vars^ vars = _rootPage->m_Scene5Vars;
    if (vars->GetBitmapImageDXP1())
    {
        // loads chosen top left texture image
        LoadChosenSound1();
    }
    else
    {
        LoadDefaultSound1(L"Assets\\Textures\\fire.dds");

    }
}

Hot3dxRotoDraw::Scenario1_Start::~Scenario1_Start()
{
    if (_rootPage->Get_timerEventTokenDXP().Value != 0)
    {
        _rootPage->Get_timerDXP()->Tick -= _rootPage->Get_timerEventTokenDXP();
    }
}

void Hot3dxRotoDraw::Scenario1_Start::StopButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (_rootPage->Get_initializedDXP())
    {
        _rootPage->Stop_startDXP();
        _rootPage->Get_timerDXP()->Stop();
        _rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
    }
}

void Hot3dxRotoDraw::Scenario1_Start::OnTimerTick(Object^ sender, Object^ e)
{
    // Update the sound position on every dispatcher timer tick.
    _rootPage->Start_startOnUpdate(_rootPage->Get_angularVelocityDXP(), _rootPage->Get_heightDXP(), _rootPage->Get_radiusDXP());
}

void Hot3dxRotoDraw::Scenario1_Start::PlayButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (_rootPage->Get_initializedDXP())
    {
        _rootPage->Start_startDXP();
        _rootPage->Get_timerDXP()->Start();
        _rootPage->NotifyUser("Playing", NotifyType::StatusMessage);
    }
}

void Hot3dxRotoDraw::Scenario1_Start::RadiusSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    _rootPage->Set_radiusDXP(static_cast<float>(e->NewValue));
}

void Hot3dxRotoDraw::Scenario1_Start::EnvironmentComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
    if (_rootPage->Get_initializedDXP())
    {
        //_rootPage->Start_startDXPSetEnvironment(static_cast<HrtfEnvironment>(EnvironmentComboBox->SelectedIndex));
    }
}

void Hot3dxRotoDraw::Scenario1_Start::FullViewButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->FullScreen_Click(sender, e);
}

void Hot3dxRotoDraw::Scenario1_Start::RotationSpeedSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    // Update angular velocity based on specified orbit time in seconds.
    if (_rootPage->Get_initializedDXP())
    {
        auto milliseconds = static_cast<float>(e->NewValue) * 1000;
        if (milliseconds > 0)
        {
            _rootPage->Set_angularVelocityDXP(HRTF_2PI / milliseconds);
        }
        else
        {
            _rootPage->Set_angularVelocityDXP(0.0f);
        }
    }
}

void Hot3dxRotoDraw::Scenario1_Start::HeightSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    _height = static_cast<float>(e->NewValue);
    _rootPage->Set_heightDXP(_height);
}

void Hot3dxRotoDraw::Scenario1_Start::DisplayResult(Windows::UI::Xaml::Controls::Image^ image, 
    Windows::UI::Xaml::Controls::TextBox^ textBox,
    Platform::String^ thumbnailModeName, Windows::Storage::IStorageItem^ item, 
    Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail, bool isGroup)
{
    BitmapImage^ bitmapImage = ref new BitmapImage();

    bitmapImage->SetSource(thumbnail);
    image->Source = bitmapImage;

    String^ itemType = isGroup ? "Group" : item->IsOfType(StorageItemTypes::File) ? "File" : "Folder";
    //textBlock->Text = /*"ThumbnailMode." + thumbnailModeName + "\n"*/
    //    //+ itemType + 
        ":" + item->Name + "\n";
    //+ "size: " + thumbnail->OriginalWidth.ToString() + "x" + thumbnail->OriginalHeight.ToString()
    //+ "size: " + size.ToString() + "\n";
}

void Hot3dxRotoDraw::Scenario1_Start::LeftMenusButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
   
    _rootPage->SetLeftSwapChainPanel();
}

void Hot3dxRotoDraw::Scenario1_Start::RightMenusButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->SetRightSwapChainPanel();
}


void Hot3dxRotoDraw::Scenario1_Start::textBlock2_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario1_Start::LoadButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    LoadChosenSound1();
}

void Hot3dxRotoDraw::Scenario1_Start::LoadDefaultSound1(Platform::String^ soundPath)
{
    m_soundPath = soundPath;
}

void Hot3dxRotoDraw::Scenario1_Start::LoadChosenSound1()
{
    Scenario1Vars^ vars = _rootPage->m_Scene1Vars;

    // loads default sound and top image

    Pickers::FileOpenPicker^ pick = ref new Pickers::FileOpenPicker();
    pick->ViewMode = PickerViewMode::Thumbnail; //Pickers::PickerViewMode::List;
    pick->SuggestedStartLocation = PickerLocationId::ComputerFolder;// ickers::PickerLocationId::MusicLibrary;
    pick->FileTypeFilter->Append(".wav");
    pick->FileTypeFilter->Append(".mp3");
    pick->FileTypeFilter->Append(".wma");


    ThumbnailMode thumbnailMode = ThumbnailMode::SingleItem;
    String^ thumbnailModeName = thumbnailMode.ToString();
    bool fastThumbnail = false;
    ThumbnailOptions thumbnailOptions{};

    const unsigned int size = int(SoundFile1->ActualHeight);
    if (size > 0)
    {
        fastThumbnail = false;
        thumbnailOptions = ThumbnailOptions::UseCurrentScale;
    }
    concurrency::create_task(pick->PickSingleFileAsync()).then(
        [this](Windows::Storage::StorageFile^ file)
        {
            if (nullptr != file)
            {
                // Open the stream
                Hot3dxRotoDraw::Scenario1_Start^ _this = this;
                concurrency::create_task(file->OpenAsync(FileAccessMode::Read)).then(
                    [_this, file](IRandomAccessStream^ stream)
                    {
                        if (stream != nullptr)
                        {
                            Scenario1Vars^ vars = _this->_rootPage->m_Scene1Vars;
                            _this->m_ContentStream = stream;
                            _this->UpdateContentProps(file->Path);
                            vars->SetFile1(file);
                            vars->SetFileSound(file);
                            _this->_rootPage->SetFileSound1DXP(file->Path);
                            vars->SetFileSound(file);
                            _this->_rootPage->AudioInitialize();
                        }
                    });
            }
        });

    concurrency::create_task(pick->PickSingleFileAsync()).then(
        [this, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](Windows::Storage::StorageFile^ file)
        {
            if (nullptr != file)
            {
                // Open the stream
                Hot3dxRotoDraw::Scenario1_Start^ _this = this;
                create_task(file->GetScaledImageAsThumbnailAsync(thumbnailMode, size, thumbnailOptions)).then([_this, file, thumbnailMode, thumbnailModeName, thumbnailOptions, fastThumbnail, size](StorageItemThumbnail^ thumbnail)
                    {
                        Scenario1Vars^ vars = _this->_rootPage->m_Scene1Vars;
                if (thumbnail != nullptr)
                {
                    vars->SetFile1(file);
                    vars->SetFileSound(file);
                    _this->_rootPage->SetFileSound1DXP(file->Path);
                    vars->SetFileSound(file);
                    vars->SetThumbnailDXP1(thumbnail);
                    DisplayResult(_this->SoundFile1, _this->txtContentProps, thumbnailModeName, file, thumbnail, false);
                    vars->SetbitmapImageDXP1(vars->GetThumbnailDXP1());
                    vars->GetBitmapImageDXP1()->SetSource(vars->GetThumbnailDXP1());

                    _this->_rootPage->NotifyUser("Opened file " + file->Name, NotifyType::StatusMessage);
                    _this->txtContentProps->Text = file->Path;
                }
                else
                {
                    _this->_rootPage->NotifyUser("Error opening file " + file->Name, NotifyType::ErrorMessage);
                }

                    });
            }
            else {}
        });

    _rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::Scenario1_Start::UpdateContentProps(Platform::String^ str)
{
    String^ text = str;

    if (nullptr != txtContentProps)
    {
        // The event handler may be invoked on a background thread, so use the Dispatcher to invoke the UI-related code on the UI thread.
        txtContentProps->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler(
            [this, text]()
            {
                Windows::UI::Xaml::Media::SolidColorBrush^ brush;
        txtContentProps->Text = text;

       // if (("" == text) && (m_ContentType == ContentType::ContentTypeFile))
        //{
            brush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::ColorHelper::FromArgb(0xCC, 0xFF, 0x00, 0x00));
        //}
       // else
        //{
            brush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::ColorHelper::FromArgb(0xFF, 0xFF, 0xFF, 0xFF));
       // }

        txtContentProps->Background = brush;
            }));
    }
}
