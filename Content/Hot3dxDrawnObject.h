//--------------------------------------------------------------------------------------
// File: Hot3dxDrawnObject.h
//
// Copyright (c) Jeff Kubitz - Hot3dx. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#pragma once

#include "pch.h"
#include <VertexTypesXaml12.h>
#include "Hot3dxVertexTypes.h"
#include "Common\d3dx12.h"
#include <d3d12.h>
#include <memory>
#include <vector>

using namespace DirectX;
using namespace DirectX::DXTKXAML12;
//using 

namespace Hot3dxRotoDraw {


    
            class IEffect;
            class ResourceUploadBatch;

            class Hot3dxDrawnObject
            {
            public:
                Hot3dxDrawnObject(Hot3dxDrawnObject const&) = delete;
                Hot3dxDrawnObject& operator= (Hot3dxDrawnObject const&) = delete;

                virtual ~Hot3dxDrawnObject();

                using VertexTypePC = DirectX::DXTKXAML12::VertexPositionColor;
                //using VertexTypeTT = Hot3dxRotoDraw::VertexPositionNormalTextureTangent;
                // Factory methods.
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateCube(float size = 1, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateBox(const XMFLOAT3& size, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateSphere(float diameter = 1, size_t tessellation = 16, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateGeoSphere(float diameter = 1, size_t tessellation = 3, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateCylinder(float height = 1, float diameter = 1, size_t tessellation = 32, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateCone(float diameter = 1, float height = 1, size_t tessellation = 32, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateTorus(float diameter = 1, float thickness = 0.333f, size_t tessellation = 32, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateTetrahedron(float size = 1, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateOctahedron(float size = 1, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateDodecahedron(float size = 1, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateIcosahedron(float size = 1, bool rhcoords = true, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateCustom(const std::vector<DirectX::DXTKXAML12::VertexPositionColor>& vertices, const std::vector<uint16_t>& indices, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateDrawnObjectColor(const std::vector<DirectX::DXTKXAML12::VertexPositionColor>& vertices, const std::vector<uint16_t>& indices, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateDrawnObjectTexture(const std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture>& vertices, const std::vector<uint16_t>& indices, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateDrawnObjectColorTexture(const std::vector<DirectX::DXTKXAML12::VertexPositionNormalColorTexture>& vertices, const std::vector<uint16_t>& indices, _In_opt_ ID3D12Device* device = nullptr);

                static void __cdecl CreateCube(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float size = 1, bool rhcoords = true);
                static void __cdecl CreateBox(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, const XMFLOAT3& size, bool rhcoords = true);
                static void __cdecl CreateSphere(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float diameter = 1, size_t tessellation = 16, bool rhcoords = true);
                static void __cdecl CreateGeoSphere(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float diameter = 1, size_t tessellation = 3, bool rhcoords = true);
                static void __cdecl CreateCylinder(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float height = 1, float diameter = 1, size_t tessellation = 32, bool rhcoords = true);
                static void __cdecl CreateCone(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float diameter = 1, float height = 1, size_t tessellation = 32, bool rhcoords = true);
                static void __cdecl CreateTorus(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float diameter = 1, float thickness = 0.333f, size_t tessellation = 32, bool rhcoords = true);
                static void __cdecl CreateTetrahedron(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float size = 1, bool rhcoords = true);
                static void __cdecl CreateOctahedron(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float size = 1, bool rhcoords = true);
                static void __cdecl CreateDodecahedron(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float size = 1, bool rhcoords = true);
                static void __cdecl CreateIcosahedron(std::vector<VertexTypePC>& vertices, std::vector<uint16_t>& indices, float size = 1, bool rhcoords = true);

                
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateCustomTangent(const std::vector<Hot3dxRotoDraw::VertexPositionNormalTextureTangent>& vertices, const std::vector<uint16_t>& indices, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateDrawnObjectTangent(const std::vector<Hot3dxRotoDraw::VertexPositionNormalTextureTangent> vertices, const std::vector<uint16_t>& indices, _In_opt_ ID3D12Device* device = nullptr);

                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateDrawnObjectColorTangent(const std::vector<DirectX::DXTKXAML12::VertexPositionNormalColorTexture> vertices, const std::vector<uint16_t>& indices, _In_opt_ ID3D12Device* device = nullptr);

                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateCustomDualTexture(const std::vector<DirectX::DXTKXAML12::VertexPositionDualTexture>& vertices, const std::vector<uint16_t>& indices, _In_opt_ ID3D12Device* device = nullptr);
                static std::unique_ptr<Hot3dxDrawnObject> __cdecl CreateDrawnObjectDualTexture(const std::vector<DirectX::DXTKXAML12::VertexPositionDualTexture>& vertices, const std::vector<uint16_t>& indices, _In_opt_ ID3D12Device* device = nullptr);

                // Load VB/IB resources for static geometry.
                void __cdecl LoadStaticBuffers(_In_ ID3D12Device* device, DirectX::DXTKXAML12::ResourceUploadBatch& resourceUploadBatch);

                // Draw the primitive.
                void __cdecl Draw(_In_ ID3D12GraphicsCommandList* commandList) const;

            private:
                //
                Hot3dxDrawnObject() noexcept(false);
                // Private implementation.
                class Impl;

                std::unique_ptr<Impl> pImpl;
            };

            

            
        
            
}
