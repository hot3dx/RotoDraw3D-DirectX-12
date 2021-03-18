//--------------------------------------------------------------------------------------
// File: Obj3DFile.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Obj3Dfile.h"

Hot3dxRotoDraw::Obj3DFile::Obj3DFile(Platform::String^ filename, unsigned int objectCount)
{
	m_filename = filename;
	m_objectCount = objectCount;
}

Hot3dxRotoDraw::Obj3DFile::~Obj3DFile()
{
	//throw ref new Platform::NotImplementedException();
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DFile::PointDataValuesReturnObjFileVertex(float x, float y, float z)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptStrX = ref new Platform::String(std::to_wstring(x).c_str());
	Platform::String^ m_ptStrY = ref new Platform::String(std::to_wstring(y).c_str());
	Platform::String^ m_ptStrZ = ref new Platform::String(std::to_wstring(z).c_str());

	// Vertices
	m_ptStr = m_ptStr->Concat(m_ptStr, "v ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DFile::PointDataValuesReturnObjFileNormal(float nx, float ny, float nz)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptStrNX = ref new Platform::String(std::to_wstring(nx).c_str());
	Platform::String^ m_ptStrNY = ref new Platform::String(std::to_wstring(ny).c_str());
	Platform::String^ m_ptStrNZ = ref new Platform::String(std::to_wstring(nz).c_str());
	// Normals
	m_ptStr = m_ptStr->Concat(m_ptStr, "vn ");
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrNX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrNY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrNZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DFile::PointDataValuesReturnObjFileTextureCoord(float tu, float tv)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptStrTU = ref new Platform::String(std::to_wstring(tu).c_str());
	Platform::String^ m_ptStrTV = ref new Platform::String(std::to_wstring(tv).c_str());
	// Texture Coordinates
	m_ptStr = m_ptStr->Concat(m_ptStr, "vt ");
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrTU);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrTV);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DFile::IndicesFaceValuesReturnObjFile(uint16_t a, uint16_t b, uint16_t c)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptStrX = ref new Platform::String(std::to_wstring(a).c_str());
	Platform::String^ m_ptStrY = ref new Platform::String(std::to_wstring(b).c_str());
	Platform::String^ m_ptStrZ = ref new Platform::String(std::to_wstring(c).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, "f ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, "/");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, "/");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, "/");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, "/");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, "/");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, "/");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	return m_ptStr;
}

Platform::String^ Hot3dxRotoDraw::Obj3DFile::DrawnObjectNodesSaveObjFile(unsigned int count, VOID** nodes)
{
	Platform::String^ file = ref new Platform::String(L"#\n#\n#\n\n");
	for (unsigned int i = 0; i < count; i++)
	{
		VOID* Node = nodes[i];
		//DrawnObjectSaveObjFile(nullptr,	nullptr, nullptr, nullptr, nullptr);
	}
	return file;
}

Platform::String^ Hot3dxRotoDraw::Obj3DFile::DrawnObjectSaveObjFile(
	unsigned int pointCount, 
	unsigned int groupCount,
	std::vector<DirectX::VertexPositionNormalTexture> vertexes,
	std::vector<uint16_t> indices,
	Platform::String^ mtlObjFilename,
	Platform::String^ nodeName,
	Platform::String^ effectName
)
{
	// Header
	Platform::String^ file = ref new Platform::String(L"#\n#\n#\n\n");
	// library file
	file = file->Concat(file, L"mtllib ");
	file = file->Concat(file, mtlObjFilename);
	file = file->Concat(file, L"\n\n\n#\n# ");
	
	unsigned int l = (size_t)nodeName->Length()-4;
	const wchar_t* s1 = mtlObjFilename->Data();
	
	Platform::String^ s = ref new Platform::String();
	for (unsigned int j = 0; j < l; j++)
	{
		s = s->Concat(s, s1[j]);
	}
	
	nodeName = ref new Platform::String(s->Data());
	// Node Name Block
	file = file->Concat(file, nodeName);
	file = file->Concat(file, L"\n#\ng ");
	file = file->Concat(file, nodeName);

	// Node Name + Effect Block
	file = file->Concat(file, L"\n\n\n#\n# ");
	file = file->Concat(file, nodeName);
	file = file->Concat(file, L"_");
	file = file->Concat(file, effectName);
	file = file->Concat(file, L"\n#\nusemtl ");
	file = file->Concat(file, nodeName);
	file = file->Concat(file, L"_");
	file = file->Concat(file, effectName);
	file = file->Concat(file, L"\n\n");


	//pointCount = vertexes.size();
	// Vertices Block
	for (unsigned int i = 0; i < vertexes.size();i++)
	{
		Platform::String^ aVertexStr = PointDataValuesReturnObjFileVertex(vertexes.at(i).position.x, vertexes.at(i).position.y, vertexes.at(i).position.z);
		file = file->Concat(file, aVertexStr);
	}
	// Normals Block
	for (unsigned int i = 0; i < vertexes.size(); i++)
	{
		Platform::String^ aNormalStr = PointDataValuesReturnObjFileNormal(vertexes.at(i).normal.x, vertexes.at(i).normal.y, vertexes.at(i).normal.z);
		file = file->Concat(file, aNormalStr);
	}
	// TextureCoord Block
	for (unsigned int i = 0; i < vertexes.size(); i++)
	{
		Platform::String^ aTextureCoordStr = PointDataValuesReturnObjFileTextureCoord(vertexes.at(i).textureCoordinate.x, vertexes.at(i).textureCoordinate.y);
		file = file->Concat(file, aTextureCoordStr);
	}

	for (unsigned int i = 0; i < indices.size(); )
	{
		unsigned int a = indices.at(i)+1;
		unsigned int b = indices.at(i + 1)+1;
		unsigned int c = indices.at(i + 2) + 1;
		Platform::String^ aFaceStr = IndicesFaceValuesReturnObjFile(a, b, c);
		file = file->Concat(file, aFaceStr);
		//Platform::String^ bFaceStr = IndicesFaceValuesReturnObjFile(indices.at(i), indices.at(i + 2), indices.at(i + 1));
		//file = file->Concat(file, bFaceStr);
		i += 3;
	}

	return file;
}
