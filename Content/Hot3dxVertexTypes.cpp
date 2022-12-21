#include "pch.h"
#include "pch.h"
#include <Graphics\VertexTypesXaml12.h>
#include "Hot3dxVertexTypes.h"
#include "Common\d3dx12.h"
#include <d3d12.h>
#include <memory>
#include <vector>

using namespace DirectX;
using namespace DirectX::DXTKXAML12;
using namespace Hot3dxRotoDraw;

//--------------------------------------------------------------------------------------
            // PBR 
const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalTextureTangent::InputElements[] =
{
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionNormalTextureTangent) == 48, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC Hot3dxRotoDraw::VertexPositionNormalTextureTangent::InputLayout =
{
    VertexPositionNormalTextureTangent::InputElements,
    VertexPositionNormalTextureTangent::InputElementCount
};

//--------------------------------------------------------------------------------------
            // Dual Texture Opposite Sides 
const D3D12_INPUT_ELEMENT_DESC VertexPositionColorNormalNormalTextureTexture::InputElements[] =
{
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionColorNormalNormalTextureTexture) == 68, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC Hot3dxRotoDraw::VertexPositionColorNormalNormalTextureTexture::InputLayout =
{
    VertexPositionColorNormalNormalTextureTexture::InputElements,
    VertexPositionColorNormalNormalTextureTexture::InputElementCount
};