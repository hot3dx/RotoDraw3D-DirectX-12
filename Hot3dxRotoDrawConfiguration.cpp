
#include "pch.h"
#include "DirectXPage.xaml.h"
#include "Hot3dxRotoDrawConfiguration.h"

using namespace Hot3dxRotoDraw;

Platform::Array<Scenario>^ DirectXPage::scenariosInner = ref new Platform::Array<Scenario>
{
    { "ColorPicker", "Hot3dxRotoDraw.Scenario6_ColorPicker" },
    { "Textures", "Hot3dxRotoDraw.Scenario5_MatsTexs" },
    { "Draw!", "Hot3dxRotoDraw.Scenario2_Normal" },
    { "Scale Object", "Hot3dxRotoDraw.Scenario4_MySettings" },
    { "Rotate Object", "Hot3dxRotoDraw.Scenario9_Rotate" },
    { "FileIO", "Hot3dxRotoDraw.Scenario3_FileIO" },
    { "Settings", "Hot3dxRotoDraw.Scenario1_Start" },
    { "SphereDraw", "Hot3dxRotoDraw.Scenario7_SphereDraw" },
    { "Sculpt", "Hot3dxRotoDraw.Scenario10_Sculpt" },
    { "Help", "Hot3dxRotoDraw.Scenario8_Help" },
    { "Grid or Pic", "Hot3dxRotoDraw.Scenario11_GridorPic" },
    // TODO: Example of How To Add the next scenario
    //{ "Something3", "Hot3dxRotoDraw.Scenario_Something3" }
};