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

# Xaml designer does not work without Windows 11, build 22H2 in Visual Studio 2022, tools v143, 10.0.22261, C++17
# (It is possible to build it for yourself in VS 2019 using tools 142, with 10.0.18...)
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

10_25_2022 There have been many fixes. The GUI can be correctly flopped from left to right.  And Right to left.  It is better to resize with the GUI on the left, then flop it but if you can hit right flip key on the settings it should wright.  The toggle menu can also fix a flop error display when switching from the left to the right. Fullscreen mode is fixed by using the button is settings "Fullscreen" Fullscreen mode is exited with a press of the "F" key.
The use of .DDS or WIC is now allowed without a checkbox check and will automatically decide between DDS, JPEG, JPG, PNG, BMP, etc.  Graphic files for textures must be located in the $(ProgramOutputDir)\\AppX\\Assets and subdirectories are allowed. Textures can be changed from the defaults. The saved OBJ, MTL file can be used to import into the VS 2017-2022 model/scene maker in VS.  The rotate menu item and the scale menu item save the point at whatever changes you have made. The next object will be drawn and put to those degrees of rotation or scale. 
The program now catches hitting most buttons or functions, if not all without a program crash such as hitting set points before you have drawn anything.

The sound stays on unless you hit the stop button on the settings page.

The "New or Clear" button now allows and endless amount of new objects to be drawn without a crash. It also has a catch if there are no points yet or textures

The problem with using prime numbers that do not divide into 360 degrees without a remainder seems fixed, numbers like 11, 13, 45, etc. If there is a problem set the partial slider to "1" before hitting the "Set Points"

Not fixed, Focus on the Rotation Edit box does not cease without clicking out side the edit box, yet. After entering a Rotate Angle for a group, always click outside the box. I'll get it someday.

The Shift N, horizontal Line, Shift M, vertical line, Shift L will add a whole lot letters after you desired angle is entered if you do not click outside the Rotate Box.  I'll get that someday.

Other than that, there are full, partial open and closed object drawing caps.

IThe program is set up to where it will work with warp emulation software as long as your computer has DirectX 11 capabilities but it works great and fast in DirectX 12 - Now the Default.

Most things are fixed no help files but ummm, the key list is in DxPage.xaml.cpp KeyUp and KeyDown functions

Q-E Z axis in out
W-S Y axis up down
A-D X axis left right

Arrow Keys
Up-Down Vertical (x axis) Rotation for object View
Left-Right Horizontal (y axis Rotation for object view

F - exits FullScreen mode button clicked in Settings Page

Shift N, M, L hold down while drawing draw horizontal, Vertical, 45 degree angle lines

Z-X, C-V - adjust width and height ratios so the mouse matches the cursor for your screen size

Added - Resize including monitor, resultion, pix per inch change, switches across monitors, does not crash if monitor loses power or is unplugged.
      - New or clear button allows another new object to be drawn
      - A lot of try catches for accidental clicks
      - Under the current Solution setup the DirectXToolkitXaml12 is in the $(RootDir)\ Hot3dxRotoDraw can be in any sub-directory
