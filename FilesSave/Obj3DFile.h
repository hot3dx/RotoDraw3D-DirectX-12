//--------------------------------------------------------------------------------------
// File: Obj3DFile.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Content\Hot3dxRotoDrawSceneRender.h"

using namespace Platform;
using namespace DirectX::DXTKXAML12;

namespace Hot3dxRotoDraw
{
	ref class Obj3DFile sealed
	{
	public:
		virtual ~Obj3DFile();

	internal:
		Obj3DFile(Platform::String^ filename, unsigned int objectCount);

		Platform::String^ DrawnObjectNodesSaveObjFile(unsigned int count, VOID** nodes);
		Platform::String^ DrawnObjectSaveObjFile(
			unsigned int pointCount,
			unsigned int groupCount,
			std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> vertexes,
			std::vector<uint16_t> indices,
			Platform::String^ mtlObjFilename,
			Platform::String^ nodeName,
			Platform::String^ effectName
		); 
		Platform::String^ XM_CALLCONV PointDataValuesReturnObjFileVertex(float x, float y, float z);
		Platform::String^ XM_CALLCONV PointDataValuesReturnObjFileNormal(float nx, float ny, float nz);
		Platform::String^ XM_CALLCONV PointDataValuesReturnObjFileTextureCoord(float tu, float tv);
		Platform::String^ XM_CALLCONV IndicesFaceValuesReturnObjFile(uint16_t a, uint16_t b, uint16_t c);
		
	private:

		Platform::String^ m_filename;
		unsigned int    m_objectCount;
	};
}

