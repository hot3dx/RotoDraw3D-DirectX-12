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

Scenario1_Start::Scenario1_Start() : _rootPage(DirectXPage::Current)
{
    
    InitializeComponent();
    /*
    //auto hr = _start.Initialize(L"Assets//MonoSound.wav");
    auto hr = _start.Initialize(L"Assets//musicmono_adpcm.wav");
    if (SUCCEEDED(hr))
    {
        _timer = ref new DispatcherTimer();
        _timerEventToken = _timer->Tick += ref new EventHandler<Platform::Object^>(this, &Scenario1_Start::OnTimerTick);
        TimeSpan timespan;
        timespan.Duration = 10000 / 30;
        _timer->Interval = timespan;
       
    }
    else
    {
        if (hr == E_NOTIMPL)
        {
            _rootPage->NotifyUser("HRTF API is not supported on this platform. Use X3DAudio API instead - https://code.msdn.microsoft.com/XAudio2-Win32-Samples-024b3933", NotifyType::ErrorMessage);
        }
        else
        {
            throw ref new COMException(hr);
        }
    }

    _initialized = SUCCEEDED(hr);
    */
}

Scenario1_Start::~Scenario1_Start()
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

void Hot3dxRotoDraw::Scenario1_Start::MenuViewButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->FullScreen_Click(sender, e);
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

void Hot3dxRotoDraw::Scenario1_Start::LeftMenusButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
   
    _rootPage->SetLeftSwapChainPanel();
}

void Hot3dxRotoDraw::Scenario1_Start::RightMenusButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _rootPage->SetRightSwapChainPanel();
}
