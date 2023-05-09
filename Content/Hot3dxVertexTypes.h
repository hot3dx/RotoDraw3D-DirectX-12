#pragma once

#include "pch.h"
#include <VertexTypesXaml12.h>
#include "Common\d3dx12.h"
#include <d3d12.h>
#include <memory>
#include <vector>

using namespace DirectX;
using namespace DirectX::DXTKXAML12;
//using 

namespace Hot3dxRotoDraw {

    // For PBR effects
    struct VertexPositionNormalTextureTangent
    {
        VertexPositionNormalTextureTangent() = default;

        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 textureCoordinate;
        DirectX::XMFLOAT4 tangent;

        VertexPositionNormalTextureTangent(DirectX::XMFLOAT3 const& position,
            DirectX::XMFLOAT3 const& normal,
            DirectX::XMFLOAT2 const& textureCoordinate,
            DirectX::XMFLOAT4 const& tangent)
            : position(position),
            normal(normal),
            textureCoordinate(textureCoordinate),
            tangent(tangent)
        {
        }

        VertexPositionNormalTextureTangent(DirectX::FXMVECTOR position,
            DirectX::FXMVECTOR normal,
            DirectX::CXMVECTOR textureCoordinate,
            DirectX::FXMVECTOR tangent)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat3(&this->normal, normal);
            XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
            XMStoreFloat4(&this->tangent, tangent);
        }

        static const D3D12_INPUT_LAYOUT_DESC InputLayout;

    private:
        static const int InputElementCount = 4;
        static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };

    // For Opposite Dual Texture effects
    struct VertexPositionColorNormalNormalTextureTexture
    {
        VertexPositionColorNormalNormalTextureTexture() = default;

        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT3 normal0;
        DirectX::XMFLOAT3 normal1;
        DirectX::XMFLOAT2 textureCoordinate0;
        DirectX::XMFLOAT2 textureCoordinate1;
        

        VertexPositionColorNormalNormalTextureTexture(DirectX::XMFLOAT3 const& iposition,
            DirectX::XMFLOAT4 const& icolor,
            DirectX::XMFLOAT3 const& inormal0,
            DirectX::XMFLOAT3 const& inormal1,
            DirectX::XMFLOAT2 const& itextureCoordinate0,
            DirectX::XMFLOAT2 const& itextureCoordinate1
           )
            : position(iposition),
            color(icolor),
            normal0(inormal0),
            normal1(inormal1),
            textureCoordinate0(itextureCoordinate0),
            textureCoordinate1(itextureCoordinate1)
        {
        }

        VertexPositionColorNormalNormalTextureTexture(DirectX::FXMVECTOR iposition,
            DirectX::FXMVECTOR icolor,
            DirectX::FXMVECTOR inormal0,
            DirectX::FXMVECTOR inormal1,
            DirectX::FXMVECTOR itextureCoordinate0,
            DirectX::FXMVECTOR itextureCoordinate1)
        {
            XMStoreFloat3(&this->position, iposition);
            XMStoreFloat4(&this->color, icolor);
            XMStoreFloat3(&this->normal0, inormal0);
            XMStoreFloat3(&this->normal1, inormal1);
            XMStoreFloat2(&this->textureCoordinate0, itextureCoordinate0);
            XMStoreFloat2(&this->textureCoordinate1, itextureCoordinate1);
            
        }

        static const D3D12_INPUT_LAYOUT_DESC InputLayout;

    private:
        static const int InputElementCount = 6;
        static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };

}