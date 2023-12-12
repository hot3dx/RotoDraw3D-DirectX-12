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
#include "Content\Hot3dxRotoDrawSceneRender.h"


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
        unsigned int m_iptCnt;
        unsigned int m_igroupCnt;
        unsigned int m_iptTotalCnt;


        //void SetPtGrpList(Platform::Array<uint16_t>^ list) {ptlist = list; }
        //Platform::Array<uint16_t>^ ptlist = ref new Platform::Array<uint16_t>(0);


        DirectX::DXTKXAML12::VertexPositionColor positionPoint;
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
        //void GetObjectVertexIndexData(unsigned int pointCount, unsigned int groupCount, std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> vertexes, std::vector<uint16_t> indices, Platform::String^ mtlObjFilename, Platform::String^ nodeName, Platform::String^ effectName);
        //void SetObjectVertexIndexData(unsigned int pointCount, unsigned int groupCount, std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> vertexes, std::vector<uint16_t> indices, Platform::String^ mtlObjFilename, Platform::String^ nodeName, Platform::String^ effectName);

        void X_VALUE_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void Y_VALUE_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void Z_VALUE_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void SelectedGroupSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void SelectedPointSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void textBoxSelectedGroup_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxSelectedPoint_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxSelectedPointCount_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxXPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxYPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void textBoxZPointValue_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        void slider_SculptX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void slider_SculptY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void slider_SculptZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
        void SetButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void X_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void Y_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
        void Z_VERTEX_EDIT_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        DirectX::XMFLOAT3 XM_CALLCONV PointAdd(DirectX::XMFLOAT3 vert)
        {
            DirectX::XMVECTOR v = DirectX::XMVectorSet(vert.x, vert.y, vert.z, 0.0f);
            DirectX::XMVECTOR v1 = DirectX::XMVectorSet(positionX, positionY, positionZ, 0.0f);
            DirectX::XMVECTOR added = DirectX::XMVectorAdd(v, v1);
            vert.x = DirectX::XMVectorGetX(added);
            vert.y = DirectX::XMVectorGetY(added);
            vert.z = DirectX::XMVectorGetZ(added);
            return vert;
        }

        DirectX::XMFLOAT3 XM_CALLCONV PointScale(DirectX::XMFLOAT3 vert, float scale)
        {
            DirectX::XMVECTOR v = DirectX::XMVectorSet(vert.x, vert.y, vert.z, 0.0f);
            DirectX::XMVECTOR v1 = DirectX::XMVectorSet(positionX, positionY, positionZ, 0.0f);
            DirectX::XMVECTOR added = DirectX::XMVectorScale(v, scale);
            vert.x = DirectX::XMVectorGetX(added);
            vert.y = DirectX::XMVectorGetY(added);
            vert.z = DirectX::XMVectorGetZ(added);
            return vert;
        }
    };
}




