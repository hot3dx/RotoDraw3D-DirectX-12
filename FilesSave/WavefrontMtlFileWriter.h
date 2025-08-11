//--------------------------------------------------------------------------------------
// File: Hot3dxRotoDrawMain.h
//
// Copyright (c) 2020 Jeff Kubitz - hot3dx. All rights reserved.
// And by CoPilot AI - OpenAI
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------#pragma once

#include "Content/Hot3dxRotoDrawSceneRender.h"
#include "Content/Hot3dxRotoDrawVariables.h"
#include "Scenario5_MatsTexsConfig.h"  
#include <fstream>  
#include <string>  

using namespace std;
using namespace Platform;
namespace Hot3dxRotoDraw  
{  
    ref class WavefrontMtlFileWriter sealed
    {  
    public: 
        virtual ~WavefrontMtlFileWriter();

    internal:
        WavefrontMtlFileWriter(const std::string& filename, unsigned int materialCount);  
         

        void WriteMtlFile(std::ofstream& outFile);  
        std::string WriteHeader(unsigned int index, const std::string& objectName);  
         
        std::string WriteMaterialListHeader(
            const std::string& effectName,
            unsigned int illumType,
            const std::string& node,
            const std::string& colorType,
            const std::string& textureFilename,
            unsigned int materialIndex,
            MaterialDataVector^ list);
        std::string WriteMaterialListII(
            const std::string& effectName,
            unsigned int illumType,
            const std::string& node,
            const std::string& colorType,
            const std::string& textureFilename,
            unsigned int materialIndex,
            MaterialDataVector^ list);
        std::string WriteMaterialNameType(const std::string& node, const std::string& materialType);  
        std::string WriteIllum(unsigned int n);  
        std::string WriteKa(float r, float g, float b);  
        std::string WriteKd(float r, float g, float b);  
        std::string WriteKs(float r, float g, float b);  
        std::string WriteNs(float value);  
        std::string WriteNi(float value);  
        std::string WriteD(float value);  
        std::string WriteTextureMap(const std::string& type, const std::string& filename);

        std::string BuildMaterialBlock(const MaterialDataVector^ list, const MaterialData& matData, unsigned int index);
		

    private:  

        std::string ConvertToStdString(Platform::String^ ps);
        std::ofstream* m_file; 
        std::string m_filename;
        std::string m_texfilename;  
        unsigned int m_materialCount;
        
    };  
    
}
