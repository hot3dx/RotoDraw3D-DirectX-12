//--------------------------------------------------------------------------------------
// File: Hot3dxRotoDrawMain.h
//
// Copyright (c) 2020 Jeff Kubitz - hot3dx. All rights reserved.
// And by CoPilot AI - OpenAI
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------#pragma once

#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace Hot3dxRotoDraw
{
    class WavefrontObjFileWriter
    {
    public:
        WavefrontObjFileWriter(const std::string& filename);
        virtual ~WavefrontObjFileWriter();

        void WriteObjFile(std::ofstream& outFile);

        // Example methods for writing OBJ data
        std::string WriteVertex(float x, float y, float z);
        std::string WriteNormal(float nx, float ny, float nz);
        std::string WriteTexCoord(float u, float v);
        std::string WriteFace(const std::vector<int>& vertexIndices);

    private:
        std::string m_filename;
    };
}
