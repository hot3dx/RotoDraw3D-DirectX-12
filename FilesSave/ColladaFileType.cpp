#include "pch.h"
#include "ColladaFileType.h"

using namespace Hot3dxRotoDraw;

Hot3dxRotoDraw::ColladaFileType::ColladaFileType()
{
	//throw ref new Platform::NotImplementedException();
}

Hot3dxRotoDraw::ColladaFileType::~ColladaFileType()
{
	//throw ref new Platform::NotImplementedException();
}

//
// Collada file header and version
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::HeaderVersion1_4_1()
{
	Platform::String^ header = ref new Platform::String(L"<?xml version=\"1.0\" encoding=\"utf - 8\"?>\n");
	Platform::String^ version_1_4_1 = ref new Platform::String(L"<COLLADA xmlns = \"http://www.collada.org/2005/11/COLLADASchema\" version = \"1.4.1\">\n");
	return header->Concat(header, version_1_4_1);
}

//
// Collad file asset
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::Asset_1_4_1(Platform::String^ author, Platform::String^ authoring_tool,
	Platform::String^ comments, Platform::String^ createdDate, 
	Platform::String^ createdTime, Platform::String^ keywords, Platform::String^ revision,
	Platform::String^ subject, Platform::String^ title, 
	Platform::String^ unit, Platform::String^ unitName, Platform::String^ up_axis)
{
	Platform::String^ assetsBegin = ref new Platform::String(L"\t<asset><contributor><author>");
	assetsBegin->Concat(assetsBegin, author);
	assetsBegin->Concat(assetsBegin, L"</author><authoring_tool>");
	assetsBegin->Concat(assetsBegin, authoring_tool); // FBX COLLADA exporter
	assetsBegin->Concat(assetsBegin, L"</authoring_tool><comments>");
	assetsBegin->Concat(assetsBegin, comments);
	assetsBegin->Concat(assetsBegin, L"</comments></contributor><created>");
	assetsBegin->Concat(assetsBegin, createdDate);// YYYY-MM-DD 2021-03-06T23:48:39Z
	assetsBegin->Concat(assetsBegin, L"T");
	assetsBegin->Concat(assetsBegin, createdTime);//MH::MM:MS 23:48:39Z
	assetsBegin->Concat(assetsBegin, L"Z</created><keywords>");
	assetsBegin->Concat(assetsBegin, keywords);
	assetsBegin->Concat(assetsBegin, L"</keywords><modified>");
	assetsBegin->Concat(assetsBegin, createdDate);// YYYY-MM-DD 2021-03-06T23:48:39Z
	assetsBegin->Concat(assetsBegin, L"T");
	assetsBegin->Concat(assetsBegin, createdTime);//MH::MM:MS 23:48:39Z
	assetsBegin->Concat(assetsBegin, L"Z</modified><revision>");
	assetsBegin->Concat(assetsBegin, revision);
	assetsBegin->Concat(assetsBegin, L"</revision><subject>");
	assetsBegin->Concat(assetsBegin, subject);
	assetsBegin->Concat(assetsBegin, L"</subject><title>");
	assetsBegin->Concat(assetsBegin, title);
	assetsBegin->Concat(assetsBegin, L"</title><unit ");
	assetsBegin->Concat(assetsBegin, unit); // meter=\"0.010000\"
	assetsBegin->Concat(assetsBegin, L"name=\"");
	assetsBegin->Concat(assetsBegin, unitName); // centimeter
	assetsBegin->Concat(assetsBegin, L"\"></unit><up_axis>");
	assetsBegin->Concat(assetsBegin, up_axis); // Y_UP
	assetsBegin->Concat(assetsBegin, L"></unit><up_axis>");
	assetsBegin->Concat(assetsBegin, L"</up_axis></asset>\n");
	return assetsBegin;
}

//
//<library_materials>
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryMaterials(std::vector<Platform::String^> materialList, unsigned int count)
{
	Platform::String^ libMatsBegin = ref new Platform::String(L"\t<library_materials>\n");

	for (size_t i = 0; i < count; i++)
	{
		libMatsBegin->Concat(libMatsBegin, L"\t\t<material id=\"");
		libMatsBegin->Concat(libMatsBegin, materialList.at(i));
		libMatsBegin->Concat(libMatsBegin, L"\" name=\"");
		libMatsBegin->Concat(libMatsBegin, materialList.at(i));
		libMatsBegin->Concat(libMatsBegin, L"\">\n");
		libMatsBegin->Concat(libMatsBegin, L"\t\t\t<instance_effect url=\"");
		libMatsBegin->Concat(libMatsBegin, materialList.at(i));
		libMatsBegin->Concat(libMatsBegin, L"\"-fx\"/>\n");
		libMatsBegin->Concat(libMatsBegin, L"\t\t</material>\n");
	}
	libMatsBegin->Concat(libMatsBegin, L"\t</library_materials>\n");

	return libMatsBegin;
}

//
// <library_effects
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryEffects(std::vector<Platform::String^> materialList, unsigned int count,
	Platform::String^ profile, Platform::String^ techniqueSID, Platform::String^ transOpaque,
	Platform::String^ effect, Platform::Array<float>^ rgb_s)
{
	Platform::String^ libEffectsBegin = ref new Platform::String(L"\t<library_effects>\n");

	// 
	for (unsigned int i = 0; i < count; i++)
	{
	   //<effect id="Grass-fx" name="Grass">
	     libEffectsBegin->Concat(libEffectsBegin, L"\t\t<effect id=\"");
		 libEffectsBegin->Concat(libEffectsBegin, materialList.at(i));
		 libEffectsBegin->Concat(libEffectsBegin,  L"-fx\" name=\"");
		 libEffectsBegin->Concat(libEffectsBegin, materialList.at(i));
		 libEffectsBegin->Concat(libEffectsBegin,  L"\">\n");
		 // <profile_COMMON>
	     libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t<profile_");
		 libEffectsBegin->Concat(libEffectsBegin, profile);
		 libEffectsBegin->Concat(libEffectsBegin, L">\n");
         // <technique sid="standard">
		 libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t<technique sid=\"");
		 libEffectsBegin->Concat(libEffectsBegin,  techniqueSID);
		 libEffectsBegin->Concat(libEffectsBegin, L"\">\n");
         // <phong>  // effect
		 libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t<");
		 libEffectsBegin->Concat(libEffectsBegin, effect);
		 libEffectsBegin->Concat(libEffectsBegin, L">\n");

          //  <emission>  Emmisive
		 libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<emission>\n");
          libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<color sid=\"emission\">");
		  Platform::String^ emR = ref new Platform::String(std::to_wstring(rgb_s->get(0)).c_str());
		   libEffectsBegin->Concat(libEffectsBegin, emR); libEffectsBegin->Concat(libEffectsBegin, L"  ");
		  Platform::String^ emG = ref new Platform::String(std::to_wstring(rgb_s->get(1)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, emG); libEffectsBegin->Concat(libEffectsBegin, L" ");
		  Platform::String^ emB = ref new Platform::String(std::to_wstring(rgb_s->get(2)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, emB); libEffectsBegin->Concat(libEffectsBegin, L" ");
		  Platform::String^ emA = ref new Platform::String(std::to_wstring(rgb_s->get(3)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, emA); libEffectsBegin->Concat(libEffectsBegin, L"</color>");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t</emission>\n");
		 
		   // <ambient>
		 libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<ambient>\n");
          libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<color sid=\"ambient\">");
		  Platform::String^ amR = ref new Platform::String(std::to_wstring(rgb_s->get(4)).c_str());
		   libEffectsBegin->Concat(libEffectsBegin, amR); libEffectsBegin->Concat(libEffectsBegin, L"  ");
		  Platform::String^ amG = ref new Platform::String(std::to_wstring(rgb_s->get(5)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, amG); libEffectsBegin->Concat(libEffectsBegin, L" ");
		  Platform::String^ amB = ref new Platform::String(std::to_wstring(rgb_s->get(6)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, amB); libEffectsBegin->Concat(libEffectsBegin, L" ");
		  Platform::String^ amA = ref new Platform::String(std::to_wstring(rgb_s->get(7)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, amA); libEffectsBegin->Concat(libEffectsBegin, L"</color>");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t</ambient>\n");
            //  <diffuse>
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<diffuse>\n");
          libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<color sid=\"diffuse\">");
		  Platform::String^ difR = ref new Platform::String(std::to_wstring(rgb_s->get(8)).c_str());
		   libEffectsBegin->Concat(libEffectsBegin, difR); libEffectsBegin->Concat(libEffectsBegin, L"  ");
		  Platform::String^ difG = ref new Platform::String(std::to_wstring(rgb_s->get(9)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, difG); libEffectsBegin->Concat(libEffectsBegin, L" ");
		  Platform::String^ difB = ref new Platform::String(std::to_wstring(rgb_s->get(10)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, difB); libEffectsBegin->Concat(libEffectsBegin, L" ");
		  Platform::String^ difA = ref new Platform::String(std::to_wstring(rgb_s->get(11)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, difA); libEffectsBegin->Concat(libEffectsBegin, L"</color>");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t</diffuse>\n");
            // <specular>
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<specular>\n");
          libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<color sid=\"specular\">");
		  Platform::String^ specR = ref new Platform::String(std::to_wstring(rgb_s->get(12)).c_str());
		   libEffectsBegin->Concat(libEffectsBegin, specR); libEffectsBegin->Concat(libEffectsBegin, L"  ");
		  Platform::String^ specG = ref new Platform::String(std::to_wstring(rgb_s->get(13)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, specG); libEffectsBegin->Concat(libEffectsBegin, L" ");
		  Platform::String^ specB = ref new Platform::String(std::to_wstring(rgb_s->get(14)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, specB); libEffectsBegin->Concat(libEffectsBegin, L" ");
		  Platform::String^ specA = ref new Platform::String(std::to_wstring(rgb_s->get(15)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, specA); libEffectsBegin->Concat(libEffectsBegin, L"</color>");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t</specular>\n");
		  //
		  
		  // <shininess>
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<shininess\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t\t<float sid=\"shininess\">");
		  Platform::String^ shininess = ref new Platform::String(std::to_wstring(rgb_s->get(16)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, shininess);
		   libEffectsBegin->Concat(libEffectsBegin, L"</float>\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t</shininess>\n");
              
          //  <reflective>
          libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<reflective\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t\t<color sid=\"reflective\">");
		  Platform::String^ reflectiveR = ref new Platform::String(std::to_wstring(rgb_s->get(16)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, reflectiveR);
		  Platform::String^ reflectiveG = ref new Platform::String(std::to_wstring(rgb_s->get(17)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, reflectiveG);
		  Platform::String^ reflectiveB = ref new Platform::String(std::to_wstring(rgb_s->get(18)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, reflectiveB);
		  Platform::String^ reflectiveA = ref new Platform::String(std::to_wstring(rgb_s->get(19)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, reflectiveA);
		   libEffectsBegin->Concat(libEffectsBegin, L"</color>\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t</reflective>\n");
		  
		   // <reflectivity>
          libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<reflectivity\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t\t<float sid=\"reflectivity\">");
		  Platform::String^ reflectivity = ref new Platform::String(std::to_wstring(rgb_s->get(20)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, reflectivity);
		   libEffectsBegin->Concat(libEffectsBegin, L"</float>\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t</reflectivity>\n");
		  
          //  <transparent opaque="RGB_ZERO">transOpaque
          libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<transparent opaque=\"");
		  libEffectsBegin->Concat(libEffectsBegin, transOpaque);
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t\">\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t\t<color sid=\"transparent\">");
		  Platform::String^ transparentR = ref new Platform::String(std::to_wstring(rgb_s->get(21)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, transparentR);
		  Platform::String^ transparentG = ref new Platform::String(std::to_wstring(rgb_s->get(22)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, transparentG);
		  Platform::String^ transparentB = ref new Platform::String(std::to_wstring(rgb_s->get(23)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, transparentB);
		  Platform::String^ transparentA = ref new Platform::String(std::to_wstring(rgb_s->get(24)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, transparentA);
		   libEffectsBegin->Concat(libEffectsBegin, L"</color>\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t</transparent>\n");
           
          //  <transparency>
          libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t<transparency\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t\t<float sid=\"transparency\">");
		  Platform::String^ transparency = ref new Platform::String(std::to_wstring(rgb_s->get(25)).c_str());
		  libEffectsBegin->Concat(libEffectsBegin, transparency);
		   libEffectsBegin->Concat(libEffectsBegin, L"</float>\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t\t</transparency>\n");
       
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t\t</");
		  libEffectsBegin->Concat(libEffectsBegin, effect);
		  libEffectsBegin->Concat(libEffectsBegin, L">\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t</technique>\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t</profile_");
		  libEffectsBegin->Concat(libEffectsBegin, profile);
		  libEffectsBegin->Concat(libEffectsBegin, L">\n");
		  libEffectsBegin->Concat(libEffectsBegin, L"\t\t\t\t</effect>\n");
    } // EO FOR
	
	libEffectsBegin->Concat(libEffectsBegin, L"\t</library_effects>\n");
	
	return libEffectsBegin;
}


//
//<library_geometries>
//

Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometries(Platform::String^ geoId, Platform::String^ geoNamw,
	Platform::String^ meshSrcId, Platform::String^ floatArrayId, Platform::String^ flArrCnt

)
{
	return geoId;
}

//
// <library vertices>
//

Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesVertices(
	Platform::String^ technique, // each technique same
	Platform::String^ accessorSrc, Platform::String^ accessorCnt, Platform::String^ accessorStride,
	Platform::String^ paramNameX, Platform::String^ paramTypeX,
	Platform::String^ paramNameY, Platform::String^ paramTypeY,
	Platform::String^ paramNameZ, Platform::String^ paramTypeZ,
	Platform::String^ sourceId, Platform::String^ flArrId, Platform::String^ flArrCnt2,
	std::vector<float> vertex
)
{
	return technique;
}

//
// <library_normals>
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesNormals(
	Platform::String^ technique, // each technique same
	Platform::String^ accessorSrc, Platform::String^ accessorCnt, Platform::String^ accessorStride,
	Platform::String^ paramNameX, Platform::String^ paramTypeX,
	Platform::String^ paramNameY, Platform::String^ paramTypeY,
	Platform::String^ paramNameZ, Platform::String^ paramTypeZ,
	Platform::String^ sourceId, Platform::String^ flArrId, Platform::String^ flArrCnt2,
	std::vector<float> normals
)
{
	return technique;
}

//
//<library_UVCoords>
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesUVCoords(
	Platform::String^ technique, // each technique same
	Platform::String^ accessorSrc, Platform::String^ accessorCnt, Platform::String^ accessorStride,
	Platform::String^ paramNameS, Platform::String^ paramTypeS,
	Platform::String^ paramNameT, Platform::String^ paramTypeT,
	Platform::String^ sourceId, Platform::String^ flArrId, Platform::String^ flArrCnt2
)
{
	return technique;
}

//
//<library_vertex_color>
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesVertColorArr(
	Platform::String^ technique, // each technique same
	Platform::String^ accessorSrc, Platform::String^ accessorCnt, Platform::String^ accessorStride,
	Platform::String^ paramNameR, Platform::String^ paramTypeR,
	Platform::String^ paramNameG, Platform::String^ paramTypeG,
	Platform::String^ paramNameB, Platform::String^ paramTypeB,
	Platform::String^ paramNameA, Platform::String^ paramTypeA,
	Platform::String^ sourceId, Platform::String^ flArrId, Platform::String^ flArrCnt2
)
{
	return technique;
}

//
// <library_vertices_color>
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryGeometriesVerticesInfo(
	Platform::String^ verticesId, // each technique same
	Platform::String^ semantic, Platform::String^ source)
{
	return verticesId;
}

//
// <library_triangles>
//
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
)
{return count;}

//
// <library_visual_scene>
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryVisualScene(
	Platform::String^ id, Platform::String^ name,
	Platform::String^ nodeName, Platform::String^ nodeNameId, Platform::String^ nodeNameid,
	Platform::String^ matrixSid, std::vector<float> matrix,
	Platform::String^ url, Platform::String^ symbol, Platform::String^ target,
	Platform::String^ profile, Platform::String^ visibility
)
{
	return id;
}

//
// <library_visual_scene_extra>
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryVisualSceneExtra(
	Platform::String^ techProfile, Platform::String^ frameRate,
	Platform::String^ fileProfile, Platform::String^ startTime, Platform::String^ endTime,
	Platform::String^ matrixSid, std::vector<float> matrix,
	Platform::String^ url, Platform::String^ symbol, Platform::String^ target,
	Platform::String^ profile, Platform::String^ visibility

)
{
	return techProfile;
}

//
// <library_visual_scene>
//
Platform::String^ Hot3dxRotoDraw::ColladaFileType::LibraryVisualScene(Platform::String^ url)
{
	if (url == nullptr) { url = ref new Platform::String(L"#"); }
	Platform::String^ scene = ref new Platform::String(L"\t<scene>");
	Platform::String^ sceneUrl = ref new Platform::String(L"\t\t<instance_visual_scene url=\"");
	scene->Concat(scene, sceneUrl);
	scene->Concat(scene, url);
	scene->Concat(scene, L"\"></instance_visual_scene>");
	scene->Concat(scene, L"\t</scene>");
	scene->Concat(scene, L"\t</COLLADA>");
	return scene;
}