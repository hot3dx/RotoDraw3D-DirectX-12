//--------------------------------------------------------------------------------------
// File: Hot3dxTextFileSave.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Content\Hot3dxRotoDrawSceneRender.h"

using namespace Platform;

namespace Hot3dxRotoDraw
{
	ref class Hot3dxTextFileSave sealed
	{
	public:
		virtual ~Hot3dxTextFileSave();

	internal:
		Hot3dxTextFileSave(Platform::String^ filename, unsigned int objectCount);

		Platform::String^ DrawnObjectSaveHot3dxTextFile(std::vector<DirectX::VertexPositionNormalTexture> vertexes,
		std::vector<uint16_t> indices, Platform::String^ textureImageFile, unsigned int count, VOID** nodes);
		
		Platform::String^ XM_CALLCONV PointDataValuesReturnHot3dxTextFileVertex(unsigned int number, float x, float y, float z, float nx, float ny, float nz, float tu, float tv);
		Platform::String^ XM_CALLCONV IndicesFaceValuesReturnHot3dxTextFile(unsigned int number, uint16_t a, uint16_t b, uint16_t c);
		
	private:

		Platform::String^ m_filename;
		unsigned int    m_objectCount;
	};
}


