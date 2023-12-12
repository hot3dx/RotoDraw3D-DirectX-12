#pragma once

#include "pch.h"
#include "Scenario5_MatsTexs.xaml.h"

namespace Hot3dxRotoDraw
{
    value struct MaterialList;
    value struct MaterialTypes;

    public value struct MaterialList
    {
        Platform::String^ Title;
        Platform::String^ dataName;
    };

    public value struct MaterialTypes
    {
        Platform::String^ matName;
        Platform::String^ Alpha; //_rootPage->m_Scene5Vars->GetTextureImage1File() },
        Platform::String^ Power;
        Platform::String^ A;
        Platform::String^ R;
        Platform::String^ G;
        Platform::String^ B;
        Platform::String^ SpecRed;
        Platform::String^ SpecGreen;
        Platform::String^ SpecBlue;
        Platform::String^ AmbientRed;
        Platform::String^ AmbientGreen;
        Platform::String^ AmbientBlue;
        Platform::String^ EmmissiveRed;
        Platform::String^ EmmissiveBlue;
        Platform::String^ EmmissiveGreen;
        Platform::String^ DiffuseRed;
        Platform::String^ DiffuseBlue;
        Platform::String^ DiffuseGreen;
        Platform::String^ TextureName;
        Platform::String^ TextureFile;
    };

    partial ref class Scenario5_MatsTexs
    {
    internal:
        
        static property Platform::Array<MaterialList>^ materials
        {
            Platform::Array<MaterialList>^ get()
            {
                return materialListsInner;
            }

            void set(Platform::Array<MaterialList>^ list)
            {
                materialListsInner = list;
            }
        }

        static property Platform::Array<MaterialTypes>^ materialTypes
        {
            Platform::Array<MaterialTypes>^ get()
            {
                return materialTypesLists;
            }
        }

                
    private:
        static Platform::Array<MaterialList>^ materialListsInner;
        static Platform::Array<MaterialTypes>^ materialTypesLists;
    };

    
    
}

