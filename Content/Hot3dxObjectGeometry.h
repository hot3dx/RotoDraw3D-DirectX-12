#pragma once

#include "pch.h"
#include <VertexTypesXaml12.h>
#include "Common\d3dx12.h"
#include <memory>
#include <vector>
#include <d3d12.h>

using namespace DirectX;
using namespace DirectX::DXTKXAML12;
    
using namespace Hot3dxRotoDraw;

namespace DirectX
{
    namespace DXTKXAML12
    {

        typedef std::vector<DirectX::DXTKXAML12::VertexPositionColor> VertexCollectionColor;
        typedef std::vector<Hot3dxRotoDraw::VertexPositionNormalTextureTangent> VertexCollectionPositionNormalTextureTangent;
        typedef std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> VertexCollectionPositionNormalTexture;
        typedef std::vector<DirectX::DXTKXAML12::VertexPositionNormalColorTexture> VertexCollectionPositionNormalColorTexture;
        typedef std::vector<DirectX::DXTKXAML12::VertexPositionDualTexture> VertexCollectionDualTexture;
        typedef std::vector<uint16_t> IndexCollectionColor;

        void ComputeBoxColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, const XMFLOAT3& size, bool rhcoords);
        void ComputeSphereColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords);
        void ComputeGeoSphereColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords);
        void CreateCylinderCapColor(VertexCollectionColor& vertices,IndexCollectionColor& indices, size_t tessellation, float height, float radius, bool isTop);
        void ComputeCylinderColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float height, float diameter, size_t tessellation, bool rhcoords);
        void ComputeConeColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float diameter, float height, size_t tessellation, bool rhcoords);
        void ComputeTorusColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float diameter, float thickness, size_t tessellation, bool rhcoords);
        void ComputeTetrahedronColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float size, bool rhcoords);
        void ComputeOctahedronColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float size, bool rhcoords);
        void ComputeDodecahedronColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float size, bool rhcoords);
        void ComputeIcosahedronColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float size, bool rhcoords);
        

        void ComputeBoxTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, const XMFLOAT3& size, bool rhcoords);
        void ComputeSphereTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords);
        void ComputeGeoSphereTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords);
        void ComputeCylinderTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float height, float diameter, size_t tessellation, bool rhcoords);
        void ComputeConeTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float diameter, float height, size_t tessellation, bool rhcoords);
        void ComputeTorusTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float diameter, float thickness, size_t tessellation, bool rhcoords);
        void ComputeTetrahedronTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, bool rhcoords, float size);
        void ComputeOctahedronTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, bool rhcoords, float size);
        void ComputeDodecahedronTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, bool rhcoords, float size);
        void ComputeIcosahedronTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, bool rhcoords, float size);

        void ComputeBoxDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, const XMFLOAT3& size, bool rhcoords);
        void ComputeSphereDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords);
        void ComputeGeoSphereDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords);
        void ComputeCylinderDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float height, float diameter, size_t tessellation, bool rhcoords);
        void ComputeConeDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float diameter, float height, size_t tessellation, bool rhcoords);
        void ComputeTorusDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float diameter, float thickness, size_t tessellation, bool rhcoords);
        void ComputeTetrahedronDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float size, bool rhcoords);
        void ComputeOctahedronDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float size, size_t tessellation, bool rhcoords, bool isTop);
        void ComputeDodecahedronDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float size, bool rhcoords);
        void ComputeIcosahedronDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float size, bool rhcoords);
    }
}