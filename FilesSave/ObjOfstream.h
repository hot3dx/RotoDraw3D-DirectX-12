#pragma once

#include "Content\Hot3dxRotoDrawSceneRender.h"
#include "Scenario5_MatsTexsConfig.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace Hot3dxRotoDraw
{

    ref class OfstreamMTL sealed
    {
        
    public:
        virtual ~OfstreamMTL(){}

    internal:

        OfstreamMTL(const std::string& filename, const std::string& texFilename, unsigned int materialCount);
        bool WriteOfstreamMTL(std::ofstream& file);
        bool WriteOfstreamMTL2(std::ofstream& file);
        void WriteObj3DMaterialFile(std::ofstream& file);
        void WriteObj3DMaterialKaData(std::ofstream& file, float r, float g, float b);
        const std::string& XM_CALLCONV WriteObj3DMaterialHeaderData(std::ofstream& file, unsigned int index, const std::string& objectName);
        const std::string& XM_CALLCONV /*__fastcall*/ WriteObj3DMaterialListData(std::ofstream& file, 
            const std::string& effectName,
            unsigned int illumType,
            const std::string& Node,
            const std::string& ColorType,
            const std::string& filename,
            unsigned int MaterialDataIndex,
            Hot3dxRotoDraw::MaterialDataVector^ list);
        const std::string& XM_CALLCONV WriteObj3DMaterialNameTypeData(std::ofstream& file, const std::string& Node, const std::string& materialType);
        const std::string& XM_CALLCONV WriteObj3DMaterialIllumData(std::ofstream& file, unsigned int n);
        const std::string& XM_CALLCONV WriteObj3DMaterialKdData(std::ofstream& file, float r, float g, float b);
        const std::string& XM_CALLCONV WriteObj3DMaterialKsData(std::ofstream& file, float r, float g, float b);
        const std::string& XM_CALLCONV WriteObj3DMaterialNsData(std::ofstream& file, float a);
        const std::string& XM_CALLCONV WriteObj3DMaterialNiData(std::ofstream& file, float a);
        const std::string& XM_CALLCONV WriteObj3DMaterialDData(std::ofstream& file, float a);
        const std::string& XM_CALLCONV WriteObj3DMaterialFileNameData(std::ofstream& file, const std::string& type);


    private:
        std::ofstream* m_file;
        const std::string& m_filename;
        const std::string& m_texfilename;
        unsigned int    m_materialCount;

    };




    ref class IfstreamMTL sealed
    {
    public:
		virtual ~IfstreamMTL() {}


    internal:

        IfstreamMTL(const std::string& filename, const std::string& texFilename, unsigned int materialCount);

        
        void ReadFile(const std::string& filename);
        bool ReadIfstreamMTL();

    private:
        const std::string& m_filename;
        const std::string& m_texfilename;
        unsigned int    m_materialCount;
        Hot3dxRotoDraw::MaterialDataVector^ m_materialDataList = ref new Hot3dxRotoDraw::MaterialDataVector();
	    // Replace the problematic line with a pointer-based approach  
            Hot3dxRotoDraw::MaterialData* m_materialData = new Hot3dxRotoDraw::MaterialData();
    };


    /////////////////////////////////////////////////
    ///
    /// CoPilot std::ofstream .mtl
    ///
    /////////////////////////////////////////////////


    

}