//--------------------------------------------------------------------------------------
// File: Hot3dxRotoDrawMain.h
//
// Copyright (c) 2020 Jeff Kubitz - hot3dx. All rights reserved.
// 
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------

#pragma once

#include "Content\Hot3dxRotoDrawSceneRender.h"

namespace Hot3dxRotoDraw
{
	ref class ColladaFileType
	{
	public:
		virtual ~ColladaFileType();

		// Header and Version
		Platform::String^ HeaderVersion1_4_1();
		// Assets, 3rd line
		Platform::String^ Asset_1_4_1(Platform::String^ author,
			Platform::String^ authoring_tool, Platform::String^ comments, 
			Platform::String^ createdDate, Platform::String^ createdTime, 
			Platform::String^ keywords, Platform::String^ revision, Platform::String^ subject,
			Platform::String^ title, Platform::String^ unit, 
			Platform::String^ unitName, Platform::String^ up_axis);
		
		

	internal:
		ColladaFileType();



	private:
		Platform::Array<float>^ rgbs = ref new Platform::Array<float>(25);

		Platform::String^ LibraryMaterials(std::vector<Platform::String^> materialList, unsigned int count);
		Platform::String^ LibraryEffects(std::vector<Platform::String^> materialList, unsigned int count,
			Platform::String^ profile, Platform::String^ techniqueSID, Platform::String^ transOpaque,
			Platform::String^ effect, Platform::Array<float>^ rgbs);
		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometries(Platform::String^ geoId, Platform::String^ geoNamw,
			Platform::String^ meshSrcId, Platform::String^ floatArrayId, Platform::String^ flArrCnt
			
		);
		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesVertices(
			Platform::String^ technique, // each technique same
			Platform::String^ accessorSrc, Platform::String^ accessorCnt, Platform::String^ accessorStride,
			Platform::String^ paramNameX, Platform::String^ paramTypeX,
			Platform::String^ paramNameY, Platform::String^ paramTypeY,
			Platform::String^ paramNameZ, Platform::String^ paramTypeZ,
			Platform::String^ sourceId, Platform::String^ flArrId, Platform::String^ flArrCnt2,
			std::vector<float> vertex
		);

		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesNormals(
			Platform::String^ technique, // each technique same
			Platform::String^ accessorSrc, Platform::String^ accessorCnt, Platform::String^ accessorStride,
			Platform::String^ paramNameX, Platform::String^ paramTypeX,
			Platform::String^ paramNameY, Platform::String^ paramTypeY,
			Platform::String^ paramNameZ, Platform::String^ paramTypeZ,
			Platform::String^ sourceId, Platform::String^ flArrId, Platform::String^ flArrCnt2,
			std::vector<float> normals
		);

		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesUVCoords(
			Platform::String^ technique, // each technique same
			Platform::String^ accessorSrc, Platform::String^ accessorCnt, Platform::String^ accessorStride,
			Platform::String^ paramNameS, Platform::String^ paramTypeS,
			Platform::String^ paramNameT, Platform::String^ paramTypeT,
			Platform::String^ sourceId, Platform::String^ flArrId, Platform::String^ flArrCnt2
		);

		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesVertColorArr(
			Platform::String^ technique, // each technique same
			Platform::String^ accessorSrc, Platform::String^ accessorCnt, Platform::String^ accessorStride,
			Platform::String^ paramNameR, Platform::String^ paramTypeR,
			Platform::String^ paramNameG, Platform::String^ paramTypeG,
			Platform::String^ paramNameB, Platform::String^ paramTypeB,
			Platform::String^ paramNameA, Platform::String^ paramTypeA,
			Platform::String^ sourceId, Platform::String^ flArrId, Platform::String^ flArrCnt2
		);

		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesVerticesInfo(
			Platform::String^ verticesId, // each technique same
			Platform::String^ semantic, Platform::String^ source);

		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesTriangles(
			Platform::String^ count, // each technique same
			Platform::String^ material, Platform::String^ semantic, Platform::String^ offset,
			Platform::String^ source, 
			Platform::String^ inputSemantic, Platform::String^ inputOffset, Platform::String^ inputSource,
			Platform::String^ inputSemanticT, Platform::String^ inputOffsetT,
			Platform::String^ inputSetT, Platform::String^ inputSourceT,
			Platform::String^ inputSemanticC, Platform::String^ inputOffsetC,
			Platform::String^ inputSetC, Platform::String^ inputSourceC,
			std::vector<unsigned int> triangles
			);

		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryVisualScene(
			Platform::String^ id, Platform::String^ name,
			Platform::String^ nodeName, Platform::String^ nodeNameId, Platform::String^ nodeNameid,
			Platform::String^ matrixSid, std::vector<float> matrix,
			Platform::String^ url, Platform::String^ symbol, Platform::String^ target,
			Platform::String^ profile, Platform::String^ visibility
		);

		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryVisualSceneExtra(
			Platform::String^ techProfile, Platform::String^ frameRate,
			Platform::String^ fileProfile, Platform::String^ startTime, Platform::String^ endTime,
			Platform::String^ matrixSid, std::vector<float> matrix,
			Platform::String^ url, Platform::String^ symbol, Platform::String^ target,
			Platform::String^ profile, Platform::String^ visibility

		);

		Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryVisualScene(Platform::String^ url);

	};
}