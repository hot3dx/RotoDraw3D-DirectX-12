#include "pch.h"
#include "Scenario5_MatsTexs.xaml.h"
#include "Scenario5_MatsTexsConfig.h"
#include "Content\Hot3dxRotoDrawVariables.h"

using namespace Hot3dxRotoDraw;


Platform::Array<Hot3dxRotoDraw::MaterialList>^ Scenario5_MatsTexs::materialListsInner = ref new Platform::Array<Hot3dxRotoDraw::MaterialList>
{
    { "matName: ", "" },
    { "A: ", "" }, 
    { "R: ", "" },
    { "G: ", "" },
    { "B: ", "" },
    { "Alpha: ", "" }, 
    { "Power: ", "" },
    { "Spec Red: ", "" },
    { "Spec Green: ", "" },
    { "Spec Blue: ", "" },
    { "Amb Red: ", "" },
    { "Amb Green: ", "" },
    { "Amb Blue: ", "" },
    { "Emm Red: ", "" },
    { "Emm Blue: ","" },
    { "Emm Green: ","" },
    { "Dif Red: ","" },
    { "Dif Blue: ","" },
    { "Dif Green: ", "" }
};