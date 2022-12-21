//--------------------------------------------------------------------------------------
// File: Obj3DMaterialFile.cpp
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "ObjMaterialFile.h"

using namespace Platform;
using namespace Hot3dxRotoDraw;

Hot3dxRotoDraw::Obj3DMaterialFile::Obj3DMaterialFile(Platform::String^ filename, unsigned int materialCount)
{
	m_filename = filename;
	m_materialCount = materialCount;
}

Hot3dxRotoDraw::Obj3DMaterialFile::~Obj3DMaterialFile()
{
	//throw ref new Platform::NotImplementedException();
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DMaterialFile::WriteObj3DMaterialHeaderData(unsigned int index, Platform::String^ objectName)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	if (index > 0) {
		m_ptStr = m_ptStr->Concat(m_ptStr, L"\n");
	}
	      Platform::String^ s = ref new Platform::String(L"#\n#\\n#\n\n ");
		
		
     return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DMaterialFile::WriteObj3DMaterialListData(Platform::String^ effectName, unsigned int illumType, Platform::String^ Node, Platform::String^ ColorType, Platform::String^  filename, unsigned int i, Hot3dxRotoDraw::MaterialDataVector^ list)
{
	
	Platform::String^ m_ptStr = ref new Platform::String();
	{
		Platform::String^ s = WriteObj3DMaterialNameTypeData(Node, effectName);// list->GetMaterialsList(i).effectName);
		m_ptStr = m_ptStr->Concat(m_ptStr, s);
		Platform::String^ s1 = WriteObj3DMaterialIllumData(illumType);
		m_ptStr = m_ptStr->Concat(m_ptStr, s1);
		Platform::String^ s2 = WriteObj3DMaterialKaData(list->GetMaterialsList(i).args1->get(0), list->GetMaterialsList(i).args1->get(1), list->GetMaterialsList(i).args1->get(2));
		m_ptStr = m_ptStr->Concat(m_ptStr, s2); 
		Platform::String^ s3 = WriteObj3DMaterialKdData(list->GetMaterialsList(i).args1->get(3), list->GetMaterialsList(i).args1->get(4), list->GetMaterialsList(i).args1->get(5));
		m_ptStr = m_ptStr->Concat(m_ptStr, s3); 
		Platform::String^ s4 = WriteObj3DMaterialKsData(list->GetMaterialsList(i).args1->get(7), list->GetMaterialsList(i).args1->get(8), list->GetMaterialsList(i).args1->get(9));
		m_ptStr = m_ptStr->Concat(m_ptStr, s4); 
		Platform::String^ s5 = WriteObj3DMaterialNsData(list->GetMaterialsList(i).args1->get(1));
		m_ptStr = m_ptStr->Concat(m_ptStr, s5); 
		Platform::String^ s6 = WriteObj3DMaterialFileNameData(ColorType, filename);//list->GetMaterialsList(i).wcfileName);
		m_ptStr = m_ptStr->Concat(m_ptStr, s6);
		
	}

	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DMaterialFile::WriteObj3DMaterialNameTypeData(Platform::String^ Node, Platform::String^ materialType)
{
	Platform::String^ m_ptStr = ref new Platform::String(L"newmtl ");
	m_ptStr = m_ptStr->Concat(m_ptStr, Node);
	m_ptStr = m_ptStr->Concat(m_ptStr, L" Mesh_");
	m_ptStr = m_ptStr->Concat(m_ptStr, Node);
	m_ptStr = m_ptStr->Concat(m_ptStr, L"_Mesh_Default - Lambert\n");
	//m_ptStr = m_ptStr->Concat(m_ptStr, Node);
	//m_ptStr = m_ptStr->Concat(m_ptStr, L"_");
	//m_ptStr = m_ptStr->Concat(m_ptStr, materialType);
	//m_ptStr = m_ptStr->Concat(m_ptStr, L"\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DMaterialFile::WriteObj3DMaterialIllumData(unsigned int n)
{
	Platform::String^ m_ptStr = ref new Platform::String(L"illum ");
	Platform::String^ m_ptStr2 = ref new Platform::String(std::to_wstring(n).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr2);
	m_ptStr = m_ptStr->Concat(m_ptStr, L"\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DMaterialFile::WriteObj3DMaterialKaData(float r, float g, float b)
{
	Platform::String^ m_ptStr = ref new Platform::String(L"Ka ");
	Platform::String^ m_ptStr2 = ref new Platform::String(std::to_wstring(r).c_str());
	Platform::String^ m_ptStr3 = ref new Platform::String(std::to_wstring(g).c_str());
	Platform::String^ m_ptStr4 = ref new Platform::String(std::to_wstring(b).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr2);
	m_ptStr = m_ptStr->Concat(m_ptStr, L" ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr3);
	m_ptStr = m_ptStr->Concat(m_ptStr, L" "); 
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr4);
	m_ptStr = m_ptStr->Concat(m_ptStr, L"\n");
	return m_ptStr;

}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DMaterialFile::WriteObj3DMaterialKdData(float r, float g, float b)
{
	Platform::String^ m_ptStr = ref new Platform::String(L"Kd ");
	Platform::String^ m_ptStr2 = ref new Platform::String(std::to_wstring(r).c_str());
	Platform::String^ m_ptStr3 = ref new Platform::String(std::to_wstring(g).c_str());
	Platform::String^ m_ptStr4 = ref new Platform::String(std::to_wstring(b).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr2);
	m_ptStr = m_ptStr->Concat(m_ptStr, L" ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr3);
	m_ptStr = m_ptStr->Concat(m_ptStr, L" ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr4);
	m_ptStr = m_ptStr->Concat(m_ptStr, L"\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DMaterialFile::WriteObj3DMaterialKsData(float r, float g, float b)
{
	Platform::String^ m_ptStr = ref new Platform::String(L"Ks ");
	Platform::String^ m_ptStr2 = ref new Platform::String(std::to_wstring(r).c_str());
	Platform::String^ m_ptStr3 = ref new Platform::String(std::to_wstring(g).c_str());
	Platform::String^ m_ptStr4 = ref new Platform::String(std::to_wstring(b).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr2);
	m_ptStr = m_ptStr->Concat(m_ptStr, L" ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr3);
	m_ptStr = m_ptStr->Concat(m_ptStr, L" ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr4);
	m_ptStr = m_ptStr->Concat(m_ptStr, L"\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DMaterialFile::WriteObj3DMaterialNsData(float a)
{
	Platform::String^ m_ptStr = ref new Platform::String(L"Ns ");
	Platform::String^ m_ptStr2 = ref new Platform::String(std::to_wstring(a).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStr2);
	m_ptStr = m_ptStr->Concat(m_ptStr, L"\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Obj3DMaterialFile::WriteObj3DMaterialFileNameData(Platform::String^ type, Platform::String^ filename)
{
	Platform::String^ m_ptStr = ref new Platform::String(L"map_Kd ");
	//Platform::String^ m_ptStr2 = ref new Platform::String(type);
	//Platform::String^ m_ptStr2 = ref new Platform::String(filename);
	m_ptStr = m_ptStr->Concat(m_ptStr, type);
	m_ptStr = m_ptStr->Concat(m_ptStr, L" ");
	m_ptStr = m_ptStr->Concat(m_ptStr, filename);
	m_ptStr = m_ptStr->Concat(m_ptStr, L"\n");
	return m_ptStr;
}


