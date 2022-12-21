//--------------------------------------------------------------------------------------
// File: Hot3dxDrawnObject.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Hot3dxDrawnObject.h"
#include "Hot3dxObjectGeometry.h"
#include "Hot3dxVertexTypes.h"
#include <Graphics\CommonStatesXaml12.h>
#include <Graphics\DirectXHelpersXaml12.h>
#include <Graphics\EffectsXaml12.h>
#include <Graphics\GeometryXaml12.h>
#include <Graphics\GraphicsMemoryXaml12.h>
#include <Graphics\PlatformHelpersXaml12.h>
#include <Graphics\MyResourceUploadBatchXaml12.h>

using namespace DirectX;
using namespace DirectX::DXTKXAML12;
using namespace std;
using Microsoft::WRL::ComPtr;

using namespace Hot3dxRotoDraw;


// Internal Hot3dxDrawnObject implementation class.
class Hot3dxRotoDraw::Hot3dxDrawnObject::Impl
{
public:
    Impl() noexcept : mIndexCount(0), mVertexBufferView{}, mIndexBufferView{} {}

    void Initialize(
        const VertexCollectionColor& vertices,
        const IndexCollectionColor& indices,
        _In_opt_ ID3D12Device* device);

    void InitializeTangent(
        const  VertexCollectionPositionNormalTextureTangent& vertices,
        const IndexCollectionColor& indices,
        _In_opt_ ID3D12Device* device);

    void InitializeDualTexture(
        const  VertexCollectionDualTexture& vertices,
        const IndexCollectionColor& indices,
        _In_opt_ ID3D12Device* device);

    void InitializeDrawnObjectColor(
        const VertexCollectionColor& vertices,
        const IndexCollectionColor& indices,
        _In_opt_ ID3D12Device* device);

    void InitializeDrawnObjectTexture(
        const VertexCollectionPositionNormalTexture& vertices,
        const IndexCollectionColor& indices,
        _In_opt_ ID3D12Device* device);

    void InitializeDrawnObjectColorTexture(
        const VertexCollectionPositionNormalColorTexture& vertices,
        const IndexCollectionColor& indices,
        _In_opt_ ID3D12Device* device);

    void InitializeDrawnObjectTangent(
        const VertexCollectionPositionNormalTextureTangent& vertices,
        const IndexCollectionColor& indices,
        _In_opt_ ID3D12Device* device);

    void InitializeDrawnObjectDualTexture(
        const VertexCollectionDualTexture& vertices,
        const IndexCollectionColor& indices,
        _In_opt_ ID3D12Device* device);

    void LoadStaticBuffers(_In_ ID3D12Device* device, DirectX::DXTKXAML12::ResourceUploadBatch& resourceUploadBatch);

    void Draw(_In_ ID3D12GraphicsCommandList* commandList) const;

    UINT                        mIndexCount;
    SharedGraphicsResource      mIndexBuffer;
    SharedGraphicsResource      mVertexBuffer;
    ComPtr<ID3D12Resource>      mStaticIndexBuffer;
    ComPtr<ID3D12Resource>      mStaticVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW    mVertexBufferView;
    D3D12_INDEX_BUFFER_VIEW     mIndexBufferView;

};


// Initializes a geometric primitive instance that will draw the specified vertex and index data.
void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::Initialize(
    const VertexCollectionColor& vertices,
    const IndexCollectionColor& indices,
    _In_opt_ ID3D12Device* device)
{
    if (vertices.size() >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    if (indices.size() > UINT32_MAX)
        throw std::exception("Too many indices");

    // Vertex data
    uint64_t sizeInBytes = uint64_t(vertices.size()) * sizeof(vertices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("VB too large for DirectX 12");

    auto vertSizeBytes = static_cast<size_t>(sizeInBytes);

    mVertexBuffer = GraphicsMemory::Get(device).Allocate(vertSizeBytes);

    auto verts = reinterpret_cast<const uint8_t*>(vertices.data());
    memcpy(mVertexBuffer.Memory(), verts, vertSizeBytes);

    // Index data
    sizeInBytes = uint64_t(indices.size()) * sizeof(indices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("IB too large for DirectX 12");

    auto indSizeBytes = static_cast<size_t>(sizeInBytes);

    mIndexBuffer = GraphicsMemory::Get(device).Allocate(indSizeBytes);

    auto ind = reinterpret_cast<const uint8_t*>(indices.data());
    memcpy(mIndexBuffer.Memory(), ind, indSizeBytes);

    // Record index count for draw
    mIndexCount = static_cast<UINT>(indices.size());

    // Create views
    mVertexBufferView.BufferLocation = mVertexBuffer.GpuAddress();
    mVertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(VertexCollectionColor::value_type));
    mVertexBufferView.SizeInBytes = static_cast<UINT>(mVertexBuffer.Size());

    mIndexBufferView.BufferLocation = mIndexBuffer.GpuAddress();
    mIndexBufferView.SizeInBytes = static_cast<UINT>(mIndexBuffer.Size());
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

// Initializes a geometric primitive instance that will draw the specified vertex and index data.
void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::InitializeTangent(
    const VertexCollectionPositionNormalTextureTangent& vertices,
    const IndexCollectionColor& indices,
    _In_opt_ ID3D12Device* device)
{
    if (vertices.size() >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    if (indices.size() > UINT32_MAX)
        throw std::exception("Too many indices");

    // Vertex data
    uint64_t sizeInBytes = uint64_t(vertices.size()) * sizeof(vertices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("VB too large for DirectX 12");

    auto vertSizeBytes = static_cast<size_t>(sizeInBytes);

    mVertexBuffer = GraphicsMemory::Get(device).Allocate(vertSizeBytes);

    auto verts = reinterpret_cast<const uint8_t*>(vertices.data());
    memcpy(mVertexBuffer.Memory(), verts, vertSizeBytes);

    // Index data
    sizeInBytes = uint64_t(indices.size()) * sizeof(indices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("IB too large for DirectX 12");

    auto indSizeBytes = static_cast<size_t>(sizeInBytes);

    mIndexBuffer = GraphicsMemory::Get(device).Allocate(indSizeBytes);

    auto ind = reinterpret_cast<const uint8_t*>(indices.data());
    memcpy(mIndexBuffer.Memory(), ind, indSizeBytes);

    // Record index count for draw
    mIndexCount = static_cast<UINT>(indices.size());

    // Create views
    mVertexBufferView.BufferLocation = mVertexBuffer.GpuAddress();
    mVertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(VertexCollectionPositionNormalTextureTangent::value_type));
    mVertexBufferView.SizeInBytes = static_cast<UINT>(mVertexBuffer.Size());

    mIndexBufferView.BufferLocation = mIndexBuffer.GpuAddress();
    mIndexBufferView.SizeInBytes = static_cast<UINT>(mIndexBuffer.Size());
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

// Initializes a geometric primitive instance that will draw the specified vertex and index data.
void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::InitializeDualTexture(
    const VertexCollectionDualTexture& vertices,
    const IndexCollectionColor& indices,
    _In_opt_ ID3D12Device* device)
{
    if (vertices.size() >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    if (indices.size() > UINT32_MAX)
        throw std::exception("Too many indices");

    // Vertex data
    uint64_t sizeInBytes = uint64_t(vertices.size()) * sizeof(vertices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("VB too large for DirectX 12");

    auto vertSizeBytes = static_cast<size_t>(sizeInBytes);

    mVertexBuffer = GraphicsMemory::Get(device).Allocate(vertSizeBytes);

    auto verts = reinterpret_cast<const uint8_t*>(vertices.data());
    memcpy(mVertexBuffer.Memory(), verts, vertSizeBytes);

    // Index data
    sizeInBytes = uint64_t(indices.size()) * sizeof(indices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("IB too large for DirectX 12");

    auto indSizeBytes = static_cast<size_t>(sizeInBytes);

    mIndexBuffer = GraphicsMemory::Get(device).Allocate(indSizeBytes);

    auto ind = reinterpret_cast<const uint8_t*>(indices.data());
    memcpy(mIndexBuffer.Memory(), ind, indSizeBytes);

    // Record index count for draw
    mIndexCount = static_cast<UINT>(indices.size());

    // Create views
    mVertexBufferView.BufferLocation = mVertexBuffer.GpuAddress();
    mVertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(VertexCollectionDualTexture::value_type));
    mVertexBufferView.SizeInBytes = static_cast<UINT>(mVertexBuffer.Size());

    mIndexBufferView.BufferLocation = mIndexBuffer.GpuAddress();
    mIndexBufferView.SizeInBytes = static_cast<UINT>(mIndexBuffer.Size());
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::InitializeDrawnObjectColor(const VertexCollectionColor& vertices, const IndexCollectionColor& indices, _In_opt_ ID3D12Device* device)
{

    if (vertices.size() >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    if (indices.size() > UINT32_MAX)
        throw std::exception("Too many indices");

    // Vertex data
    uint64_t sizeInBytes = uint64_t(vertices.size()) * sizeof(vertices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("VB too large for DirectX 12");

    auto vertSizeBytes = static_cast<size_t>(sizeInBytes);

    mVertexBuffer = GraphicsMemory::Get(device).Allocate(vertSizeBytes);

    auto verts = reinterpret_cast<const uint8_t*>(vertices.data());
    memcpy(mVertexBuffer.Memory(), verts, vertSizeBytes);

    // Index data
    sizeInBytes = uint64_t(indices.size()) * sizeof(indices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("IB too large for DirectX 12");

    auto indSizeBytes = static_cast<size_t>(sizeInBytes);

    mIndexBuffer = GraphicsMemory::Get(device).Allocate(indSizeBytes);

    auto ind = reinterpret_cast<const uint8_t*>(indices.data());
    memcpy(mIndexBuffer.Memory(), ind, indSizeBytes);

    // Record index count for draw
    mIndexCount = static_cast<UINT>(indices.size());

    // Create views
    mVertexBufferView.BufferLocation = mVertexBuffer.GpuAddress();
    mVertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(VertexCollectionColor::value_type));
    mVertexBufferView.SizeInBytes = static_cast<UINT>(mVertexBuffer.Size());

    mIndexBufferView.BufferLocation = mIndexBuffer.GpuAddress();
    mIndexBufferView.SizeInBytes = static_cast<UINT>(mIndexBuffer.Size());
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::InitializeDrawnObjectTexture(
    const VertexCollectionPositionNormalTexture& vertices,
    const IndexCollectionColor& indices,
    _In_opt_ ID3D12Device* device)
{
    if (vertices.size() >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    if (indices.size() > UINT32_MAX)
        throw std::exception("Too many indices");

    // Vertex data
    uint64_t sizeInBytes = uint64_t(vertices.size()) * sizeof(vertices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("VB too large for DirectX 12");

    auto vertSizeBytes = static_cast<size_t>(sizeInBytes);

    mVertexBuffer = GraphicsMemory::Get(device).Allocate(vertSizeBytes);

    auto verts = reinterpret_cast<const uint8_t*>(vertices.data());
    memcpy(mVertexBuffer.Memory(), verts, vertSizeBytes);

    // Index data
    sizeInBytes = uint64_t(indices.size()) * sizeof(indices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("IB too large for DirectX 12");

    auto indSizeBytes = static_cast<size_t>(sizeInBytes);

    mIndexBuffer = GraphicsMemory::Get(device).Allocate(indSizeBytes);

    auto ind = reinterpret_cast<const uint8_t*>(indices.data());
    memcpy(mIndexBuffer.Memory(), ind, indSizeBytes);

    // Record index count for draw
    mIndexCount = static_cast<UINT>(indices.size());

    // Create views
    mVertexBufferView.BufferLocation = mVertexBuffer.GpuAddress();
    mVertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(VertexPositionNormalTexture::InputLayout));
    mVertexBufferView.SizeInBytes = static_cast<UINT>(mVertexBuffer.Size());

    mIndexBufferView.BufferLocation = mIndexBuffer.GpuAddress();
    mIndexBufferView.SizeInBytes = static_cast<UINT>(mIndexBuffer.Size());
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::InitializeDrawnObjectColorTexture(
    const VertexCollectionPositionNormalColorTexture& vertices,
    const IndexCollectionColor& indices,
    _In_opt_ ID3D12Device* device)
{
    if (vertices.size() >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    if (indices.size() > UINT32_MAX)
        throw std::exception("Too many indices");

    // Vertex data
    uint64_t sizeInBytes = uint64_t(vertices.size()) * sizeof(vertices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("VB too large for DirectX 12");

    auto vertSizeBytes = static_cast<size_t>(sizeInBytes);

    mVertexBuffer = GraphicsMemory::Get(device).Allocate(vertSizeBytes);

    auto verts = reinterpret_cast<const uint8_t*>(vertices.data());
    memcpy(mVertexBuffer.Memory(), verts, vertSizeBytes);

    // Index data
    sizeInBytes = uint64_t(indices.size()) * sizeof(indices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("IB too large for DirectX 12");

    auto indSizeBytes = static_cast<size_t>(sizeInBytes);

    mIndexBuffer = GraphicsMemory::Get(device).Allocate(indSizeBytes);

    auto ind = reinterpret_cast<const uint8_t*>(indices.data());
    memcpy(mIndexBuffer.Memory(), ind, indSizeBytes);

    // Record index count for draw
    mIndexCount = static_cast<UINT>(indices.size());

    // Create views
    mVertexBufferView.BufferLocation = mVertexBuffer.GpuAddress();
    mVertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(VertexPositionNormalColorTexture::InputLayout));
    mVertexBufferView.SizeInBytes = static_cast<UINT>(mVertexBuffer.Size());

    mIndexBufferView.BufferLocation = mIndexBuffer.GpuAddress();
    mIndexBufferView.SizeInBytes = static_cast<UINT>(mIndexBuffer.Size());
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::InitializeDrawnObjectTangent(
    const VertexCollectionPositionNormalTextureTangent& vertices,
    const IndexCollectionColor& indices,
    _In_opt_ ID3D12Device* device)
{
    if (vertices.size() >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    if (indices.size() > UINT32_MAX)
        throw std::exception("Too many indices");

    // Vertex data
    uint64_t sizeInBytes = uint64_t(vertices.size()) * sizeof(vertices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("VB too large for DirectX 12");

    auto vertSizeBytes = static_cast<size_t>(sizeInBytes);

    mVertexBuffer = GraphicsMemory::Get(device).Allocate(vertSizeBytes);

    auto verts = reinterpret_cast<const uint8_t*>(vertices.data());
    memcpy(mVertexBuffer.Memory(), verts, vertSizeBytes);

    // Index data
    sizeInBytes = uint64_t(indices.size()) * sizeof(indices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("IB too large for DirectX 12");

    auto indSizeBytes = static_cast<size_t>(sizeInBytes);

    mIndexBuffer = GraphicsMemory::Get(device).Allocate(indSizeBytes);

    auto ind = reinterpret_cast<const uint8_t*>(indices.data());
    memcpy(mIndexBuffer.Memory(), ind, indSizeBytes);

    // Record index count for draw
    mIndexCount = static_cast<UINT>(indices.size());

    // Create views
    mVertexBufferView.BufferLocation = mVertexBuffer.GpuAddress();
    mVertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(VertexCollectionPositionNormalTextureTangent::value_type));
    mVertexBufferView.SizeInBytes = static_cast<UINT>(mVertexBuffer.Size());

    mIndexBufferView.BufferLocation = mIndexBuffer.GpuAddress();
    mIndexBufferView.SizeInBytes = static_cast<UINT>(mIndexBuffer.Size());
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::InitializeDrawnObjectDualTexture(
    const VertexCollectionDualTexture& vertices,
    const IndexCollectionColor& indices,
    _In_opt_ ID3D12Device* device)
{
    if (vertices.size() >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    if (indices.size() > UINT32_MAX)
        throw std::exception("Too many indices");

    // Vertex data
    uint64_t sizeInBytes = uint64_t(vertices.size()) * sizeof(vertices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("VB too large for DirectX 12");

    auto vertSizeBytes = static_cast<size_t>(sizeInBytes);

    mVertexBuffer = GraphicsMemory::Get(device).Allocate(vertSizeBytes);

    auto verts = reinterpret_cast<const uint8_t*>(vertices.data());
    memcpy(mVertexBuffer.Memory(), verts, vertSizeBytes);

    // Index data
    sizeInBytes = uint64_t(indices.size()) * sizeof(indices[0]);
    if (sizeInBytes > uint64_t(D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u))
        throw std::exception("IB too large for DirectX 12");

    auto indSizeBytes = static_cast<size_t>(sizeInBytes);

    mIndexBuffer = GraphicsMemory::Get(device).Allocate(indSizeBytes);

    auto ind = reinterpret_cast<const uint8_t*>(indices.data());
    memcpy(mIndexBuffer.Memory(), ind, indSizeBytes);

    // Record index count for draw
    mIndexCount = static_cast<UINT>(indices.size());

    // Create views
    mVertexBufferView.BufferLocation = mVertexBuffer.GpuAddress();
    mVertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(VertexCollectionDualTexture::value_type));
    mVertexBufferView.SizeInBytes = static_cast<UINT>(mVertexBuffer.Size());

    mIndexBufferView.BufferLocation = mIndexBuffer.GpuAddress();
    mIndexBufferView.SizeInBytes = static_cast<UINT>(mIndexBuffer.Size());
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

// Load VB/IB resources for static geometry.
_Use_decl_annotations_
void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::LoadStaticBuffers(
    ID3D12Device* device,
    DirectX::DXTKXAML12::ResourceUploadBatch& resourceUploadBatch)
{
    CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);

    // Convert dynamic VB to static VB
    if (!mStaticVertexBuffer)
    {
        assert(mVertexBuffer);

        auto desc = CD3DX12_RESOURCE_DESC::Buffer(mVertexBuffer.Size());

        DXTKXAML12::ThrowIfFailed(device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_GRAPHICS_PPV_ARGS(mStaticVertexBuffer.GetAddressOf())
        ));

        SetDebugObjectName(mStaticVertexBuffer.Get(), L"Hot3dxDrawnObject");

        resourceUploadBatch.Upload(mStaticVertexBuffer.Get(), mVertexBuffer);

        resourceUploadBatch.Transition(mStaticVertexBuffer.Get(),
            D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

        // Update view
        mVertexBufferView.BufferLocation = mStaticVertexBuffer->GetGPUVirtualAddress();

        mVertexBuffer.Reset();
    }

    // Convert dynamic IB to static IB
    if (!mStaticIndexBuffer)
    {
        assert(mIndexBuffer);

        auto desc = CD3DX12_RESOURCE_DESC::Buffer(mIndexBuffer.Size());

        DXTKXAML12::ThrowIfFailed(device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_GRAPHICS_PPV_ARGS(mStaticIndexBuffer.GetAddressOf())
        ));

        SetDebugObjectName(mStaticIndexBuffer.Get(), L"Hot3dxDrawnObject");

        resourceUploadBatch.Upload(mStaticIndexBuffer.Get(), mIndexBuffer);

        resourceUploadBatch.Transition(mStaticIndexBuffer.Get(),
            D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);

        // Update view
        mIndexBufferView.BufferLocation = mStaticIndexBuffer->GetGPUVirtualAddress();

        mIndexBuffer.Reset();
    }
}


// Draws the primitive.
_Use_decl_annotations_
void Hot3dxRotoDraw::Hot3dxDrawnObject::Impl::Draw(ID3D12GraphicsCommandList* commandList) const
{
    commandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
    commandList->IASetIndexBuffer(&mIndexBufferView);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList->DrawIndexedInstanced(mIndexCount, 1, 0, 0, 0);
}

//--------------------------------------------------------------------------------------
// Hot3dxDrawnObject
//--------------------------------------------------------------------------------------

// Constructor.
Hot3dxRotoDraw::Hot3dxDrawnObject::Hot3dxDrawnObject() noexcept(false)
    : pImpl(std::make_unique<Impl>())
{
}


// Destructor.
Hot3dxRotoDraw::Hot3dxDrawnObject::~Hot3dxDrawnObject()
{
}


// Public entrypoints.
_Use_decl_annotations_
void Hot3dxRotoDraw::Hot3dxDrawnObject::LoadStaticBuffers(ID3D12Device* device, DirectX::DXTKXAML12::ResourceUploadBatch& resourceUploadBatch)
{
    pImpl->LoadStaticBuffers(device, resourceUploadBatch);
}


_Use_decl_annotations_
void Hot3dxRotoDraw::Hot3dxDrawnObject::Draw(ID3D12GraphicsCommandList* commandList) const
{
    pImpl->Draw(commandList);
}


//--------------------------------------------------------------------------------------
// Cube (aka a Hexahedron) or Box
//--------------------------------------------------------------------------------------

// Creates a cube primitive.
std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCube(
    float size,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    VertexCollectionColor vertices;
   IndexCollectionColor indices;
    ComputeBoxColor(vertices, indices, XMFLOAT3(size, size, size), rhcoords);

    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> drawnObject(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    drawnObject->pImpl->Initialize(vertices, indices, device);

    return drawnObject;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCube(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float size,
    bool rhcoords)
{
    ComputeBoxColor(vertices, indices, XMFLOAT3(size, size, size), rhcoords);
}


// Creates a box primitive.
std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateBox(
    const XMFLOAT3& size,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    VertexCollectionColor vertices;
    IndexCollectionColor indices;
    ComputeBoxColor(vertices, indices, size, rhcoords);

    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> drawnObject(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    drawnObject->pImpl->Initialize(vertices, indices, device);

    return drawnObject;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateBox(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    const XMFLOAT3& size,
    bool rhcoords)
{
    ComputeBoxColor(vertices, indices, size, rhcoords);
}


//--------------------------------------------------------------------------------------
// Sphere
//--------------------------------------------------------------------------------------

// Creates a sphere primitive.
std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateSphere(
    float diameter,
    size_t tessellation,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    VertexCollectionColor vertices;
    IndexCollectionColor indices;

    ComputeSphereColor(vertices, indices, diameter, tessellation, rhcoords);

    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateSphere(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float diameter,
    size_t tessellation,
    bool rhcoords)
{
    ComputeSphereColor(vertices, indices, diameter, tessellation, rhcoords);
}


//--------------------------------------------------------------------------------------
// Geodesic sphere
//--------------------------------------------------------------------------------------

// Creates a geosphere primitive.
std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateGeoSphere(
    float diameter,
    size_t tessellation,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    VertexCollectionColor vertices;
    IndexCollectionColor indices;
    ComputeGeoSphereColor(vertices, indices, diameter, tessellation, rhcoords);

    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateGeoSphere(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float diameter,
    size_t tessellation,
    bool rhcoords)
{
    ComputeGeoSphereColor(vertices, indices, diameter, tessellation, rhcoords);
}


//--------------------------------------------------------------------------------------
// Cylinder / Cone
//--------------------------------------------------------------------------------------

// Creates a cylinder primitive.
std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCylinder(
    float height,
    float diameter,
    size_t tessellation,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    VertexCollectionColor vertices;
    IndexCollectionColor indices;
    ComputeCylinderColor(vertices, indices, height, diameter, tessellation, rhcoords);

    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCylinder(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float height, float diameter,
    size_t tessellation,
    bool rhcoords)
{
    ComputeCylinderColor(vertices, indices, height, diameter, tessellation, rhcoords);
}


// Creates a cone primitive.
std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCone(
    float diameter,
    float height,
    size_t tessellation,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    VertexCollectionColor vertices;
    IndexCollectionColor indices;
    ComputeConeColor(vertices, indices, diameter, height, tessellation, rhcoords);

    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCone(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float diameter,
    float height,
    size_t tessellation,
    bool rhcoords)
{
    ComputeConeColor(vertices, indices, diameter, height, tessellation, rhcoords);
}


//--------------------------------------------------------------------------------------
// Torus
//--------------------------------------------------------------------------------------

// Creates a torus primitive.
std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateTorus(
    float diameter,
    float thickness,
    size_t tessellation,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    VertexCollectionColor vertices;
    IndexCollectionColor indices;
    ComputeTorusColor(vertices, indices, diameter, thickness, tessellation, rhcoords);

    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateTorus(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float diameter,
    float thickness,
    size_t tessellation,
    bool rhcoords)
{
    ComputeTorusColor(vertices, indices, diameter, thickness, tessellation, rhcoords);
}


//--------------------------------------------------------------------------------------
// Tetrahedron
//--------------------------------------------------------------------------------------

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateTetrahedron(
    float size,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    VertexCollectionColor vertices;
    IndexCollectionColor indices;
    ComputeTetrahedronColor(vertices, indices, size, rhcoords);

    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateTetrahedron(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float size,
    bool rhcoords)
{
    ComputeTetrahedronColor(vertices, indices, size, rhcoords);
}


//--------------------------------------------------------------------------------------
// Octahedron
//--------------------------------------------------------------------------------------

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateOctahedron(
    float size,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    VertexCollectionColor vertices;
    IndexCollectionColor indices;
    ComputeOctahedronColor(vertices, indices, size, rhcoords);

    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateOctahedron(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float size,
    bool rhcoords)
{
    ComputeOctahedronColor(vertices, indices, size, rhcoords);
}


//--------------------------------------------------------------------------------------
// Dodecahedron
//--------------------------------------------------------------------------------------

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateDodecahedron(
    float size,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    VertexCollectionColor vertices;
    IndexCollectionColor indices;
    ComputeDodecahedronColor(vertices, indices, size, rhcoords);

    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateDodecahedron(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float size,
    bool rhcoords)
{
    ComputeDodecahedronColor(vertices, indices, size, rhcoords);
}


//--------------------------------------------------------------------------------------
// Icosahedron
//--------------------------------------------------------------------------------------

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateIcosahedron(
    float size,
    bool rhcoords,
    _In_opt_ ID3D12Device* device)
{
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    VertexCollectionColor vertices;
    IndexCollectionColor indices;
    ComputeIcosahedronColor(vertices, indices, size, rhcoords);

    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

void Hot3dxRotoDraw::Hot3dxDrawnObject::CreateIcosahedron(
    std::vector<VertexTypePC>& vertices,
    std::vector<uint16_t>& indices,
    float size,
    bool rhcoords)
{
    ComputeIcosahedronColor(vertices, indices, size, rhcoords);
}

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> __cdecl Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCustomTangent(
        const std::vector<Hot3dxRotoDraw::VertexPositionNormalTextureTangent>& vertices,
        const std::vector<uint16_t>& indices, 
    _In_opt_ ID3D12Device* device)
{
    // Extra validation
    if (vertices.empty() || indices.empty())
        throw std::exception("Requires both vertices and indices");

    if (indices.size() % 3)
        throw std::exception("Expected triangular faces");

    size_t nVerts = vertices.size();
    if (nVerts >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    for (auto it = indices.cbegin(); it != indices.cend(); ++it)
    {
        if (*it >= nVerts)
        {
            throw std::exception("Index not in vertices list");
        }
    }
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    // copy geometry
    primitive->pImpl->InitializeTangent(vertices, indices, device);

    return primitive;
    
}

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> __cdecl Hot3dxRotoDraw::Hot3dxDrawnObject::CreateDrawnObjectTangent(
    const std::vector<Hot3dxRotoDraw::VertexPositionNormalTextureTangent> vertices, 
    const std::vector<uint16_t>& indices, 
    ID3D12Device* device)
{
    // Extra validation
    if (vertices.empty() || indices.empty())
        throw std::exception("Requires both vertices and indices");

    if (indices.size() % 3)
        throw std::exception("Expected triangular faces");

    size_t nVerts = vertices.size();
    if (nVerts >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    for (auto it = indices.cbegin(); it != indices.cend(); ++it)
    {
        if (*it >= nVerts)
        {
            throw std::exception("Index not in vertices list");
        }
    }
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    // copy geometry
    primitive->pImpl->InitializeDrawnObjectTangent(vertices, indices, device);

    return primitive;
    
}

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> __cdecl Hot3dxRotoDraw::Hot3dxDrawnObject::CreateDrawnObjectColorTangent(
    const std::vector<DirectX::DXTKXAML12::VertexPositionNormalColorTexture> vertices,
    const std::vector<uint16_t>& indices,
    ID3D12Device* device)
{
    // Extra validation
    if (vertices.empty() || indices.empty())
        throw std::exception("Requires both vertices and indices");

    if (indices.size() % 3)
        throw std::exception("Expected triangular faces");

    size_t nVerts = vertices.size();
    if (nVerts >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    for (auto it = indices.cbegin(); it != indices.cend(); ++it)
    {
        if (*it >= nVerts)
        {
            throw std::exception("Index not in vertices list");
        }
    }
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    // copy geometry
    primitive->pImpl->InitializeDrawnObjectColorTexture(vertices, indices, device);

    return primitive;

}

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> __cdecl Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCustomDualTexture(
    const std::vector<DirectX::DXTKXAML12::VertexPositionDualTexture>& vertices,
    const std::vector<uint16_t>& indices,
    ID3D12Device* device)
{
    // Extra validation
    if (vertices.empty() || indices.empty())
        throw std::exception("Requires both vertices and indices");

    if (indices.size() % 3)
        throw std::exception("Expected triangular faces");

    size_t nVerts = vertices.size();
    if (nVerts >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    for (auto it = indices.cbegin(); it != indices.cend(); ++it)
    {
        if (*it >= nVerts)
        {
            throw std::exception("Index not in vertices list");
        }
    }
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    // copy geometry
    primitive->pImpl->InitializeDrawnObjectDualTexture(vertices, indices, device);

    return primitive;

}

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> __cdecl Hot3dxRotoDraw::Hot3dxDrawnObject::CreateDrawnObjectDualTexture(
    const std::vector<DirectX::DXTKXAML12::VertexPositionDualTexture>& vertices,
    const std::vector<uint16_t>& indices,
    ID3D12Device* device)
{
    // Extra validation
    if (vertices.empty() || indices.empty())
        throw std::exception("Requires both vertices and indices");

    if (indices.size() % 3)
        throw std::exception("Expected triangular faces");

    size_t nVerts = vertices.size();
    if (nVerts >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    for (auto it = indices.cbegin(); it != indices.cend(); ++it)
    {
        if (*it >= nVerts)
        {
            throw std::exception("Index not in vertices list");
        }
    }
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    // copy geometry
    primitive->pImpl->InitializeDrawnObjectDualTexture(vertices, indices, device);

    return primitive;

}


//--------------------------------------------------------------------------------------
// Custom
//--------------------------------------------------------------------------------------

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCustom(
    const std::vector<VertexTypePC>& vertices,
    const std::vector<uint16_t>& indices,
    _In_opt_ ID3D12Device* device)
{
    // Extra validation
    if (vertices.empty() || indices.empty())
        throw std::exception("Requires both vertices and indices");

    if (indices.size() % 3)
        throw std::exception("Expected triangular faces");

    size_t nVerts = vertices.size();
    if (nVerts >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    for (auto it = indices.cbegin(); it != indices.cend(); ++it)
    {
        if (*it >= nVerts)
        {
            throw std::exception("Index not in vertices list");
        }
    }
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> primitive(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    // copy geometry
    primitive->pImpl->Initialize(vertices, indices, device);

    return primitive;
}

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> __cdecl Hot3dxRotoDraw::Hot3dxDrawnObject::CreateDrawnObjectColor(
    const std::vector<DirectX::DXTKXAML12::VertexPositionColor>& vertices,
    const std::vector<uint16_t>& indices,
    ID3D12Device* device)
{
    // Extra validation
    if (vertices.empty() || indices.empty())
        throw std::exception("Requires both vertices and indices");

    if (indices.size() % 3)
        throw std::exception("Expected triangular faces");

    size_t nVerts = vertices.size();
    if (nVerts >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    for (auto it = indices.cbegin(); it != indices.cend(); ++it)
    {
        if (*it >= nVerts)
        {
            throw std::exception("Index not in vertices list");
        }
    }
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> hot3dxDrawn(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    // copy geometry
    hot3dxDrawn->pImpl->InitializeDrawnObjectColor(vertices, indices, device);

    return hot3dxDrawn;
}

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> __cdecl Hot3dxRotoDraw::Hot3dxDrawnObject::CreateDrawnObjectTexture(
    const std::vector<VertexPositionNormalTexture>& vertices,
    const std::vector<uint16_t>& indices,
    ID3D12Device* device)
{
    // Extra validation
    if (vertices.empty() || indices.empty())
        throw std::exception("Requires both vertices and indices");

    if (indices.size() % 3)
        throw std::exception("Expected triangular faces");

    size_t nVerts = vertices.size();
    if (nVerts >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    for (auto it = indices.cbegin(); it != indices.cend(); ++it)
    {
        if (*it >= nVerts)
        {
            throw std::exception("Index not in vertices list");
        }
    }
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> hot3dxDrawn(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    // copy geometry
    hot3dxDrawn->pImpl->InitializeDrawnObjectTexture(vertices, indices, device);

    return hot3dxDrawn;
}

std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> __cdecl Hot3dxRotoDraw::Hot3dxDrawnObject::CreateDrawnObjectColorTexture(
    const std::vector<VertexPositionNormalColorTexture>& vertices,
    const std::vector<uint16_t>& indices,
    ID3D12Device* device)
{
    // Extra validation
    if (vertices.empty() || indices.empty())
        throw std::exception("Requires both vertices and indices");

    if (indices.size() % 3)
        throw std::exception("Expected triangular faces");

    size_t nVerts = vertices.size();
    if (nVerts >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    for (auto it = indices.cbegin(); it != indices.cend(); ++it)
    {
        if (*it >= nVerts)
        {
            throw std::exception("Index not in vertices list");
        }
    }
    // Create the primitive object.
    std::unique_ptr<Hot3dxRotoDraw::Hot3dxDrawnObject> hot3dxDrawn(new Hot3dxRotoDraw::Hot3dxDrawnObject());

    // copy geometry
    hot3dxDrawn->pImpl->InitializeDrawnObjectColorTexture(vertices, indices, device);

    return hot3dxDrawn;

}

