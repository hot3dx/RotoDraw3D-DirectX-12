# - New Version 10/14/2022- #

# RotoDraw3D-DirectX-12 #
RotoDraw3d DX 12 is a freehand symmetrical object drawer for DirectX 12

Many bug fixes in GUI, Drawing algorithms, everywhere! 9/20/2022
## namespace DirectX::DXTKXAML12 added
Windows 11 build 22H2, 
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

Uses, WIC ,dds files for textures

# This project requires: My DirectXToolkitXaml12 - also finished # 
## 1) at least 8 gb ram ##
## 2)intel hd graphics or a DirectX 12 capable graphics card ##
## 3) WinPixEventRuntime version=1.0.210209001 nuget package ##
## 4) Microsoft.UI.Xaml 2.8.1 nuget package with Webview

Setup for pen, mouse, or touchpad. a pen is best


![Step 1: Set Colors](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot49.png)
![Step 2: Set Textures](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot50.png)
![Step 3: Set Point Color and Draw](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot51.png)
![Step 4: Set PointsButton](https://github.com/hot3dx/RotoDraw3D-DirectX-12/blob/main/ScreenShot/Screenshot52.png)

The collada and obj file writers allow importation into the fbx Visual Studio modeller

Bitcoin donations accepted:
BTC 32HVJHEXh9bMLVkmoSE1d389q2Q1YvfrNE


## KNOWN ISSUES
- Create Algorithm for m_widthRatio and m_heightRatio variables in RotoDrawSceneRender::RotoDrawSceneRender() in 
Hot3dxRotoDrawSceneRender.cpp for optimal mouse/cursor movement
based on drawing area pixel width and pixel height Z,X,C,V keys can used to change this
-When built, Error list shows App.g.h, etc *.g.h not found:
Go to App.xaml.h left click 'App.g.h' in the #include "Generated..."
From context menu choose, "Go to document App.g.h" - fixed!
- If WICs don't load for textures, Scenario11_GridorPic.cpp has the solution to do that coded - an email easy fix
- models and textures and media files must be saved ..\\AppX\\Assets\\ or a public directory like Documents, Pictures,..
Added - Resize including monitor, resultion, pix per inch change.
      - New or clear button allows another new object to be drawn
      - A lot of try catches for accidental clicks
      - Under the current Solution setup the DirectXToolkitXaml12 is in the $(RootDir)\ Hot3dxRotoDraw can be in any sub-directory
