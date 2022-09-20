#include "Hot3dxTextFileSave.h"
//--------------------------------------------------------------------------------------
// File: Hot3dxTextFileSave.cpp
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Hot3dxTextFileSave.h"

Hot3dxRotoDraw::Hot3dxTextFileSave::Hot3dxTextFileSave(Platform::String^ filename, unsigned int objectCount)
{
	m_filename=filename;
	m_objectCount=objectCount;
}

Platform::String^ Hot3dxRotoDraw::Hot3dxTextFileSave::DrawnObjectSaveHot3dxTextFile(std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> vertexes,
	std::vector<uint16_t> indices, Platform::String^ textureImageFile, unsigned int count, VOID** nodes)
{
	Platform::String^ file = ref new Platform::String(L"HEADER:Hot3dx:DirectX12:Model:Text:20201n");
	Platform::String^ m_ptVertexCntStr = ref new Platform::String(std::to_wstring(vertexes.size()).c_str());
	file = file->Concat(file, "VertexType: DirectX::DXTKXAML12::VertexPositionNormalTexture");
	file = file->Concat(file, "\n");
	file = file->Concat(file, "VertexCount: ");
	file = file->Concat(file, m_ptVertexCntStr);
	file = file->Concat(file, "\n");
	for (unsigned int i = 0; i < vertexes.size(); i++)
	{
		Platform::String^ aVertexStr = PointDataValuesReturnHot3dxTextFileVertex(i, vertexes.at(i).position.x, vertexes.at(i).position.y, vertexes.at(i).position.z, vertexes.at(i).normal.x, vertexes.at(i).normal.y, vertexes.at(i).normal.z, vertexes.at(i).textureCoordinate.x, vertexes.at(i).textureCoordinate.y);
		file = file->Concat(file, aVertexStr);
	}

	Platform::String^ m_IndexCntStr = ref new Platform::String(std::to_wstring(indices.size()).c_str());
	file = file->Concat(file, "IndexCount: ");
	file = file->Concat(file, m_IndexCntStr);
	file = file->Concat(file, "\n");

	unsigned int cnt = 0;
	for (unsigned int i = 0; i < indices.size(); )
	{
		Platform::String^ aFaceStr = IndicesFaceValuesReturnHot3dxTextFile(cnt, indices.at(i), indices.at(i + 1), indices.at(i + 2));
		file = file->Concat(file, aFaceStr);
		i += 3;
		cnt++;
	}

	Platform::String^ m_TexFleNameStr = ref new Platform::String(textureImageFile->Data());
	//L"Assets\\Textures\\Marble.dds");
	file = file->Concat(file, "TextureFileData: \n");
	file = file->Concat(file, "FileNameSize: ");
	file = file->Concat(file, m_TexFleNameStr->Length().ToString());
	file = file->Concat(file, "\n");
	file = file->Concat(file, "TextureFileName: \n");
	file = file->Concat(file, m_TexFleNameStr);
	file = file->Concat(file, "\nEOF");

	return file;
}

Hot3dxRotoDraw::Hot3dxTextFileSave::~Hot3dxTextFileSave() {}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Hot3dxTextFileSave::PointDataValuesReturnHot3dxTextFileVertex(unsigned int number, float x, float y, float z, float nx, float ny, float nz, float tu, float tv)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptCntStr = ref new Platform::String(std::to_wstring(number).c_str());
	Platform::String^ m_ptStrX = ref new Platform::String(std::to_wstring(x).c_str());
	Platform::String^ m_ptStrY = ref new Platform::String(std::to_wstring(y).c_str());
	Platform::String^ m_ptStrZ = ref new Platform::String(std::to_wstring(z).c_str());
	Platform::String^ m_ptStrNX = ref new Platform::String(std::to_wstring(nx).c_str());
	Platform::String^ m_ptStrNY = ref new Platform::String(std::to_wstring(ny).c_str());
	Platform::String^ m_ptStrNZ = ref new Platform::String(std::to_wstring(nz).c_str());
	Platform::String^ m_ptStrTU = ref new Platform::String(std::to_wstring(tu).c_str());
	Platform::String^ m_ptStrTV = ref new Platform::String(std::to_wstring(tv).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, "Pt:");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptCntStr);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// x
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// y
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// z
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// nx
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrNX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// ny
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrNY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// nz
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrNZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// tu
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrTU);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");//tv
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrTV);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Hot3dxTextFileSave::IndicesFaceValuesReturnHot3dxTextFile(unsigned int number, uint16_t a, uint16_t b, uint16_t c)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptCntStr = ref new Platform::String(std::to_wstring(number).c_str());
	Platform::String^ m_ptStrX = ref new Platform::String(std::to_wstring(a).c_str());
	Platform::String^ m_ptStrY = ref new Platform::String(std::to_wstring(b).c_str());
	Platform::String^ m_ptStrZ = ref new Platform::String(std::to_wstring(c).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, "F:");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptCntStr);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	return m_ptStr;
}




