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


#pragma once

#include "Scenario7_SphereDraw.g.h"
#include "DirectXPage.xaml.h"
#include "Hot3dxRotoDrawConfiguration.h"


using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace Hot3dxRotoDraw
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Scenario7_SphereDraw sealed
    {
    public:
        Scenario7_SphereDraw();
        virtual ~Scenario7_SphereDraw();

        Windows::UI::Xaml::Controls::TextBox^ GetSphereRadiusTextBox() { return m_SphereRadiusTextBox; }
        Windows::UI::Xaml::Controls::TextBox^ GetPointSpaceTextBox() { return m_PointSpaceTextBox; }

    private:
        //void StartButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
       //void OnTimerTick(Object^ sender, Object^ e);
        //void RadiusSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        //void AngularVelocitySlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
         // void HeightSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);

    private:

        DirectXPage^ _rootPage;
        bool                            _initialized = false;


        void SphereRadiusTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        void PointSpacingTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        void DrawSphereButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void Back_Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        float m_cameraradius;
        float m_camerarotation;
        float m_anglerotation;
        int m_iCount;
    };
}


