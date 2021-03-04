# - uploading is finished - 3/1/21- #

# RotoDraw3D-DirectX-12 #
RotoDraw3d DX 12 is a freehand symmetrical object drawer for DirectX 12

## Visual Studio 2019 C++ 17 build 19041 tools 142 Windows 10 v 20H2 ##
## supported Win32 Debug/Release - x64 Debug/Release
#Draw One Line! Press the Button!

# by: Hot3dx, Jeff Kubitz #

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
## 4) Microsoft.UI.Xaml version 2.5.0 nuget package

Setup for pen, mouse, or touchpad. a pen is best


![Step 1: Set Colors](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot49.png)
![Step 2: Set Textures](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot50.png)
![Step 3: Set Point Color and Draw](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot51.png)
![Step 4: Set PointsButton](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot52.png)

## KNOWN ISSUES ##
- 200 point MAXIMUM on Single First LINE! @360 lines!
- Failure of all necessary files to copy into AppX\Assets\ or AppX\Assets\Textures\ manually copy if that happens
- SegoeUI_18.spritefont does not copy from assets to AppX/Assets and must be manually copied or an invokewatson exception results in vccorlib
