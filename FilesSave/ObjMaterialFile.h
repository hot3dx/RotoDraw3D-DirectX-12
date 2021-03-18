//--------------------------------------------------------------------------------------
// File: Obj3DMaterialFile.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Content\Hot3dxRotoDrawSceneRender.h"
#include "Scenario5_MatsTexsConfig.h"

using namespace Platform;

namespace Hot3dxRotoDraw
{
	ref class Obj3DMaterialFile sealed
	{
	public:
		virtual ~Obj3DMaterialFile();

	internal:
		Obj3DMaterialFile(Platform::String^ filename, unsigned int materialCount);
		Platform::String^ XM_CALLCONV WriteObj3DMaterialHeaderData();
		Platform::String^ XM_CALLCONV WriteObj3DMaterialListData(Platform::String^ effectName, 
			unsigned int illumType, 
			Platform::String^ Node, Platform::String^ ColorType, 
			Platform::String^ filename, 
			Hot3dxRotoDraw::MaterialDataVector^ list);
		Platform::String^ XM_CALLCONV WriteObj3DMaterialNameTypeData(Platform::String^ Node, Platform::String^ materialType);
		Platform::String^ XM_CALLCONV WriteObj3DMaterialIllumData(unsigned int n); 
		Platform::String^ XM_CALLCONV WriteObj3DMaterialKaData(float r, float g, float b);
		Platform::String^ XM_CALLCONV WriteObj3DMaterialKdData(float r, float g, float b);
		Platform::String^ XM_CALLCONV WriteObj3DMaterialKsData(float r, float g, float b);
		Platform::String^ XM_CALLCONV WriteObj3DMaterialNsData(float a);
		Platform::String^ XM_CALLCONV WriteObj3DMaterialFileNameData(Platform::String^ type, Platform::String^ filename);
		

	private:

		Platform::String^ m_filename;
		unsigned int    m_materialCount;
	};
}
