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
#include "Scenario3_FileIO.xaml.h"
#include "DirectXPage.xaml.h"

using namespace Hot3dxRotoDraw;

using namespace concurrency; 
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation; 

Hot3dxRotoDraw::Scenario3_FileIO::Scenario3_FileIO() : _rootPage(DirectXPage::Current)
{
	InitializeComponent();
	
	_rootPage->NotifyUser("Stopped", NotifyType::StatusMessage);
}

Hot3dxRotoDraw::Scenario3_FileIO::~Scenario3_FileIO()
{
	
}

void Hot3dxRotoDraw::Scenario3_FileIO::New_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario3_FileIO::Open_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}

IBuffer^ Hot3dxRotoDraw::Scenario3_FileIO::GetBufferFromString(Platform::String^ str)
{
    InMemoryRandomAccessStream^ memoryStream = ref new InMemoryRandomAccessStream();
    DataWriter^ dataWriter = ref new DataWriter(memoryStream);
    dataWriter->WriteString(str);
    return dataWriter->DetachBuffer();
}
void Hot3dxRotoDraw::Scenario3_FileIO::Save_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    // Clear previous returned file name, if it exists, between iterations of this scenario
    //IDC_WELCOME_STATIC->Text = "";//was used as a sample but needs to be changed; A writer function needs to be called with
    //the file data to be saved.

    FileSavePicker^ savePicker = ref new FileSavePicker();
    savePicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;

    auto plainTextExtensions1 = ref new Platform::Collections::Vector<String^>();
    auto plainTextExtensions2 = ref new Platform::Collections::Vector<String^>();
    plainTextExtensions1->Append(".txt");
    plainTextExtensions2->Append(".hbin");
    savePicker->FileTypeChoices->Insert("Plain Text", plainTextExtensions1);
    savePicker->FileTypeChoices->Insert("Hot3dx binary", plainTextExtensions2);
    savePicker->SuggestedFileName = "New Document";

    create_task(savePicker->PickSaveFileAsync()).then([this](StorageFile^ file)
        {
            if (file != nullptr)
            {
                Hot3dxRotoDraw::Scenario3_FileIO^ _this = this;
                // Prevent updates to the remote version of the file until we finish making changes and call CompleteUpdatesAsync.
                CachedFileManager::DeferUpdates(file);
                // write to file
                
                    Platform::String^ fileData = _rootPage->GetSceneRenderDrawnObjectSaveText(file->Name, 1);

                    create_task(FileIO::WriteTextAsync(file, fileData/*file->Name*/)).then([_this, file]()
                        {
                            // Let Windows know that we're finished changing the file so the other app can update the remote version of the file.
                            // Completing updates may require Windows to ask for user input.
                            create_task(CachedFileManager::CompleteUpdatesAsync(file)).then([_this, file](FileUpdateStatus status)
                                {
                                    if (status == FileUpdateStatus::Complete)
                                    {
                                        //task.get();
                                        _this->_rootPage->NotifyUser("The following " + file + file->Name + " was saved.", NotifyType::StatusMessage);
                                        
                                    }
                                    else
                                    {
                                        _this->_rootPage->NotifyUser("Error writing to file" + file + file->Name, NotifyType::ErrorMessage);
                                    }
                                });
                        });
                }
                else
                {
                    _rootPage->NotifyUser("There is no model, please draw a model and then save again.", NotifyType::ErrorMessage);
                }
            
        });

}

void Hot3dxRotoDraw::Scenario3_FileIO::Save_Obj_Material_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    // Clear previous returned file name, if it exists, between iterations of this scenario
    //IDC_WELCOME_STATIC->Text = "";//was used as a sample but needs to be changed; A writer function needs to be called with
    //the file data to be saved.

    FileSavePicker^ savePicker = ref new FileSavePicker();
    savePicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;

    auto plainTextExtensions1 = ref new Platform::Collections::Vector<String^>();
    plainTextExtensions1->Append(".mtl");
    savePicker->FileTypeChoices->Insert("Plain Text", plainTextExtensions1);
    savePicker->SuggestedFileName = "ObjectNode1";

    create_task(savePicker->PickSaveFileAsync()).then([this](StorageFile^ file)
    {
        if (file != nullptr)
        {
            Hot3dxRotoDraw::Scenario3_FileIO^ _this = this;
            // Prevent updates to the remote version of the file until we finish making changes and call CompleteUpdatesAsync.
            CachedFileManager::DeferUpdates(file);
            // write to file
            this->mtlFilename = file;
            Platform::String^ sf = file->DisplayName;
            Platform::String^ fileData = _rootPage->GetSceneRenderDrawnObjectSaveObj3DandMtl(L"Phong[13]", 2, sf, _rootPage->GetTextureImage1FileDXP());

            create_task(FileIO::WriteTextAsync(file, fileData/*file->Name*/)).then([_this, file]()
            {
                // Let Windows know that we're finished changing the file so the other app can update the remote version of the file.
                // Completing updates may require Windows to ask for user input.
                create_task(CachedFileManager::CompleteUpdatesAsync(file)).then([_this, file](FileUpdateStatus status)
                {
                    if (status == FileUpdateStatus::Complete)
                    {
                        //task.get();
                        _this->_rootPage->NotifyUser("The following " + file + file->Name + " was saved.", NotifyType::StatusMessage);

                    }
                    else
                    {
                        _this->_rootPage->NotifyUser("Error writing to file" + file + file->Name, NotifyType::ErrorMessage);
                    }
                });
            });
        }
        else
        {
            _rootPage->NotifyUser("There is no model, please draw a model and then save again.", NotifyType::ErrorMessage);
        }

    });

    FileSavePicker^ savePicker1 = ref new FileSavePicker();
    savePicker1->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;

    auto plainTextExtensions2 = ref new Platform::Collections::Vector<String^>();
    plainTextExtensions2->Append(".obj");
    savePicker1->FileTypeChoices->Insert("3D Obj fbx", plainTextExtensions2);
    savePicker1->SuggestedFileName = "ObjectNode1";
    create_task(savePicker1->PickSaveFileAsync()).then([this](StorageFile^ file1)
    {
        if (file1 != nullptr)
        {
            Hot3dxRotoDraw::Scenario3_FileIO^ _this = this;
            // Prevent updates to the remote version of the file until we finish making changes and call CompleteUpdatesAsync.
            CachedFileManager::DeferUpdates(file1);
            // write to file

            Platform::String^ fileData = _rootPage->GetSceneRenderDrawnObjectSaveObj3DFile(_this->mtlFilename->Name, file1->DisplayName, L"Phong[13]");
            
            create_task(FileIO::WriteTextAsync(file1, fileData/*file->Name*/)).then([_this, file1]()
            {
                // Let Windows know that we're finished changing the file so the other app can update the remote version of the file.
                // Completing updates may require Windows to ask for user input.
                create_task(CachedFileManager::CompleteUpdatesAsync(file1)).then([_this, file1](FileUpdateStatus status)
                {
                    if (status == FileUpdateStatus::Complete)
                    {
                        //task.get();
                        _this->_rootPage->NotifyUser("The following " + file1 + file1->Name + " was saved.", NotifyType::StatusMessage);

                    }
                    else
                    {
                        _this->_rootPage->NotifyUser("Error writing to file" + file1 + file1->Name, NotifyType::ErrorMessage);
                    }
                });
            });
        }
        else
        {
            _rootPage->NotifyUser("There is no model, please draw a model and then save again.", NotifyType::ErrorMessage);
        }

    });

}


void Hot3dxRotoDraw::Scenario3_FileIO::SaveAs_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    FileSavePicker^ savePicker = ref new FileSavePicker();
    savePicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;

    auto plainTextExtensions2 = ref new Platform::Collections::Vector<String^>();
    plainTextExtensions2->Append(".hbin");
    savePicker->FileTypeChoices->Insert("Hot3dx binary", plainTextExtensions2);
    savePicker->SuggestedFileName = "New Document";

    create_task(savePicker->PickSaveFileAsync()).then([this](StorageFile^ file)
        {
            if (file != nullptr)
            {
                Hot3dxRotoDraw::Scenario3_FileIO^ _this = this;

                String^ userContent = _rootPage->GetSceneRenderDrawnObjectSaveText(file->Name, 1);
                if (userContent != nullptr && !userContent->IsEmpty())
                {
                    IBuffer^ buffer = GetBufferFromString(userContent);
                    create_task(FileIO::WriteBufferAsync(file, buffer)).then([_this, file, buffer, userContent](task<void> task)
                        {
                            try
                            {
                                task.get();
                                _this->_rootPage->NotifyUser("The following " + buffer->Length.ToString() + " bytes of text were written to '" + file->Name + "':\n" + userContent, NotifyType::StatusMessage);
                            }
                            catch (COMException^ ex)
                            {
                                _this->_rootPage->NotifyUser("Error writing to file", NotifyType::ErrorMessage);
                            }
                        });
                }
                else
                {
                    _rootPage->NotifyUser("There is no model, please draw a model and then save again.", NotifyType::ErrorMessage);
                }

            }// eo else if (file->FileType->Data() == L".txt")
        });
}


void Hot3dxRotoDraw::Scenario3_FileIO::SaveAll_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void Hot3dxRotoDraw::Scenario3_FileIO::Save_Texture_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
