# - New Version 9/20/2022- #

# RotoDraw3D-DirectX-12 #
RotoDraw3d DX 12 is a freehand symmetrical object drawer for DirectX 12

Many bug fixes in GUI, Drawing algorithms, everywhere! 9/20/2022
## namespace DirectX::DXTKXAML12 added
Windows 11 build 21H2, 
Visual Studio 2022 c++ 17.3.4,  v143, 10.0.22621.0,
## supported Win32 Debug/Release - x64 Debug/Release (only working on x64 versions)

Included are the latest DirectXTK12 stuff but I have not made examples or tested them, such as new audio algos, 
PBREffect, CMO filetypes but I have not made any examples to test those as  of 9/20/2022
#Draw One Line! Press the Button!

# Xaml designer does not work without Windows 11, build 22H2 in Visual Studio 2022
# 
By:
![Ico](https://github.com/hot3dx/AppXamlDX12/blob/master/Assets/AutoDraw2.ico) Jeff Kubitz #

This really is close to finished the only things left to go are to hook up a few 
of the UI Xaml features, finish adding the Sculpt Feature, Save Files to other formats like fbx, dae, obj, sdkmesh2,
and whatever else we can think up like XBox

Oh yes there are no help files yet but everything is numbered by steps

only draws one single object! Which would be meant to transfer to a scene editor

Currently only uses, ,dds files for textures

# This project requires: My DirectXToolkitXaml12 - also finished # 
## 1) at least 8 gb ram ##
## 2)intel hd graphics or a DirectX 12 capable graphics card ##
## 3) WinPixEventRuntime version=1.0.210209001 nuget package ##
## 4) Microsoft.UI.Xaml 2.8.1 nuget package

Setup for pen, mouse, or touchpad. a pen is best


![Step 1: Set Colors](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot49.png)
![Step 2: Set Textures](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot50.png)
![Step 3: Set Point Color and Draw](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot51.png)
![Step 4: Set PointsButton](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot52.png)

Bitcoin donations accepted:
BTC 32HVJHEXh9bMLVkmoSE1d389q2Q1YvfrNE

## KNOWN ISSUES ##
- 200 point MAXIMUM on Single First LINE! @360 lines!
- Failure of all necessary files to copy into AppX\Assets\ or AppX\Assets\Textures\ manually copy if that happens
- SegoeUI_18.spritefont does not copy from assets to AppX/Assets and must be manually copied or an invokewatson exception results in vccorlib
