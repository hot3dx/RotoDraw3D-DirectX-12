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

#include "Generated Files\Scenario10_Sculpt.g.h"
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
    public ref class Scenario10_Sculpt sealed
    {
    public:
        Scenario10_Sculpt();
        virtual ~Scenario10_Sculpt();

    internal:
        static Scenario10_Sculpt^ Current;

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:

        DirectXPage^ _rootPage;
        bool         _initialized = false;

        bool         m_bSculptEnabled;

        unsigned int m_iselectedPoint;

        size_t point;
        unsigned int m_iselectedGroup;
        DirectX::DXTKXAML12::VertexPositionNormalTexture positionPoint;
        float positionX;
        float positionY;
        float positionZ;

        void checkBoxSculptBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void checkBoxSculptXBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void checkBoxSculptYBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void checkBoxSculptZBool_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void textBoxGroupCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void textBoxPointsPerGroup_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void textBoxPointCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);


        void SculptTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void SculptFilesTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void GetObjectVertexIndexData(unsigned int pointCount, unsigned int groupCount, std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> vertexes, std::vector<uint16_t> indices, Platform::String^ mtlObjFilename, Platform::String^ nodeName, Platform::String^ effectName);
        void SetObjectVertexIndexData(unsigned int pointCount, unsigned int groupCount, std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> vertexes, std::vector<uint16_t> indices, Platform::String^ mtlObjFilename, Platform::String^ nodeName, Platform::String^ effectName);

        void ROTATE_X_GridCam_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void ROTATE_Y_GridCam_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void ROTATE_Z_GridCam_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void GRIDCAM_X_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void GRIDCAM_Y_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void GRIDCAM_Z_ROTATE_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void SelectedGroupSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void SelectedPointSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void textBoxSelectedGroup_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxSelectedPoint_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxSelectedPointCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxXPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxYPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxZPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
    };
}




