
#include "pch.h"
#include "ObjOfstream.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace Hot3dxRotoDraw;

Hot3dxRotoDraw::OfstreamMTL::OfstreamMTL(const std::string& filename, const std::string& texFilename, unsigned int materialCount) :
	m_filename(filename),
	m_texfilename(texFilename),
    m_materialCount(materialCount)

{
   std::ofstream file(filename, std::ios::out | std::ios::trunc);

   m_file = &file;
	throw ref new Platform::NotImplementedException();
}

bool Hot3dxRotoDraw::OfstreamMTL::WriteOfstreamMTL(std::ofstream& file)
{
    
    if (file.is_open()) {
        file << "newmtl MyMaterial\n";
        file << "Kd 1.0 0.0 0.0\n"; // Red color
        file.close();
    }
    return true;
}

bool Hot3dxRotoDraw::OfstreamMTL::WriteOfstreamMTL2(std::ofstream& file)
{ 
    
    if (file.is_open()) {
        file << "newmtl MyMaterial\n";
        file << "Kd 1.0 0.0 0.0\n"; // Red color
        file.close();
    }
    return true;
}

void Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialFile(std::ofstream& file)
{
    if (file.is_open()) {
        file << "#\n# Material File\n#\n\n";
        file << "newmtl MyMaterial\n";
        file << "Kd 1.0 0.0 0.0\n"; // Diffuse color (red)
        file << "Ks 0.5 0.5 0.5\n"; // Specular color (gray)
        file << "Ns 500\n"; // Specular exponent
        file << "map_Kd texture.png\n"; // Texture file
        file.close();
    }
    else {
        std::cerr << "Error: Could not open file " << std::endl;
    }
}

    void Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialKaData(std::ofstream& file, float r, float g, float b)
    {
       if (file.is_open()) {
            file << "Ka " << r << " " << g << " " << b << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
        }
    }

    const std::string& XM_CALLCONV Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialHeaderData(std::ofstream& file, unsigned int index, const std::string& objectName)
    {
        if (file.is_open()) {
            file << "#\n# Material File\n#\n\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    const std::string& XM_CALLCONV /*__fastcall*/ Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialListData(
        std::ofstream& file, 
        const std::string& effectName, 
        unsigned int illumType, 
        const std::string& Node,
        const std::string& ColorType,
        const std::string& filename,
        unsigned int MaterialDataIndex,
        Hot3dxRotoDraw::MaterialDataVector^ list)
    {
        if (file.is_open()) {
            file << "illum " << illumType << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    const std::string& XM_CALLCONV Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialNameTypeData(std::ofstream& file, const std::string & node, const std::string & materialType) {
        if (file.is_open()) {
            file << "newmtl " << node << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    const std::string& XM_CALLCONV Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialIllumData(std::ofstream& file, unsigned int n) {
        if (file.is_open()) {
            file << "illum " << n << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    const std::string& XM_CALLCONV Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialKdData(std::ofstream& file, float r, float g, float b) {
        if (file.is_open()) {
            file << "Kd " << r << " " << g << " " << b << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    const std::string& XM_CALLCONV Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialKsData(std::ofstream& file, float r, float g, float b) {
        if (file.is_open()) {
            file << "Ks " << r << " " << g << " " << b << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    const std::string& XM_CALLCONV Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialNsData(std::ofstream& file, float a) {
        if (file.is_open()) {
            file << "Ns " << a << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    const std::string& XM_CALLCONV Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialNiData(std::ofstream& file, float a) {
        if (file.is_open()) {
            file << "Ni " << a << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    const std::string& XM_CALLCONV Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialDData(std::ofstream& file, float a) {
        if (file.is_open()) {
            file << "d " << a << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    const std::string& XM_CALLCONV Hot3dxRotoDraw::OfstreamMTL::WriteObj3DMaterialFileNameData(std::ofstream& file, const std::string & type) {
        if (file.is_open()) {
            file << "map_Kd " << type << " " << "\n";
        }
        else {
            std::cerr << "Error: Could not open file" << std::endl;
            return "error";
        }
        return "ok";
    }

    Hot3dxRotoDraw::IfstreamMTL::IfstreamMTL(const std::string& filename, const std::string& texFilename, unsigned int materialCount) :
        m_filename(filename),
        m_texfilename(texFilename),
        m_materialCount(materialCount)
    {
		// Constructor implementation can be added here if needed
	}
    void Hot3dxRotoDraw::IfstreamMTL::ReadFile(const std::string& filename) {
        std::ifstream file(filename); // std::ifstream is an istream for files
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << std::endl; // Process the line
            }
            file.close();
        }
        else {
            std::cerr << "Could not open file: " << filename << std::endl;
        }
    }

    bool Hot3dxRotoDraw::IfstreamMTL::ReadIfstreamMTL()
    {

        std::ifstream file("materials.mtl");

        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << std::endl; // Print each line of the file
            }
            file.close();
        }
        else {
            std::cerr << "Error: Could not open materials.mtl" << std::endl;
        }

        return true;
    }