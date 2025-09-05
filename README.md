# The Current MAIN is not working if it is download to another directory other than DRIVE:\Hot3dxRotoDraw, remember the DirectXToolKitXaml12.lib is required and installed on the same drive as Hot3dxRotoDDraw, otherwise you will have to change the settings - the reason it does not work is because of S(Shortcut)/ Macro and other MAcros that need to be changed...

# 8-11-25 CoPilot Helped and I have made a lot of changes.It is ridiculously faster as I removed a lot of push_back() and replaced them with self devised formulas for object vertices and indices counts so that is a lot faster.

I got the formula for the Latitudinal Sphere much more correct for a rounder sphere object, yes facing it was hell but I finally figuredit out for most float whole numbers. 9 radius and 7 point spacing does not work. 9, 15 9,13 9,11 9,9 down to 9,5 work so do many even numbers not just odd numbers.

Thanks to CoPilot help the swapchainpanel drawing surface screen is perfectly stable. And the start up textures should load every time as for adding other textures from the pictures library it should work, will it? We hope so.

ScreenGrab seems to work well for .png, .dds, jpg, .bmp, and .gif sometimes. Textextures can also be rectangular.

I have done some work on the DirectXToolkitXaml12 and it works very well. I always use it separate from a project but it can be added directly to a project

Visual Studio 17.6.14 was used.

Windows build 26100

If you wish you can back Rotodraw3D up to something earlier like build 19041 but the tools are 143.

I will move on to Xaml WinRT most likely here shortly, I have a base program that is C++23 I made today so. I shall upload the latest version of the DirectXXamlToolkit12. That will be made in WinRT to I think but maybe not. I shall attempt to publish this on the Microsoft Store maybe it will be there, maybe not

# 6-2-2023 PBREFFECT Drawing completely fixed and operational. Cannot ScreenGrab in PBR Emmisive texture added and EffectInfo and EffectFactory

#  5-31-2023 fixed PBREffect crash. Added using VeretexTypePNTT
fixed tangent in InitDrawnObjectPBRFiveTextures Function
Successive PBR Physical Based Render Objects can now be drawn after hitting the New/Clear Button on Draw Scenario2

the first PBR you draw will not show.
1) click button 2) in  12) Grid or Pic
2) The cursor will reappear 
3) Go to 3) Draw and Click the New or Clear Button. Click the Drawing Grid Area.
4) Click the New or Clear Button Again
5) Choose PBR Effect Type from the ComboBox
6) Draw your line, hit Set Points Button
7) Now every New or Clear Button push will allow you to Draw objects endlessly or at least for a long time. All types of Effects 

I had this fixed two weeks ago, accidentally erased the and couldn't remember exactly how I did it...but today I did remember or rediscovered it again

# New Version 05-09-2023

What's New: VideoTexture and ScreenGrab
PackageManifest Capability Settings for read writes!
Rememeber most errors are caused by not being in the correct Directory for x64 Release/Debug and win32 Release/Debug
and The Assets did not copy into the Hot3dxRotoDraw...AppX\\Assets\\...Directories so hand copy!
Windows 11 22H2, Visual Studio 2022 C\C++ 17, build 143 (can be rolled back to Windows 10 by retargeting)
Nuget Dependencies:
Microsoft.UI.Xaml          2.8.4
Microsoft.Web.WebView2     1.0.1774.30
WinPixEventRuntime         1.0.230302001

# New Version 1/9/23

Gone are the problems loading files from other directories and their subfolders like Documents, Pictures, Object 3D, Videos

Finding the Directory of the program from within the program is fixed.

Several other bugs are also fixed.

Enjoy!

# Version 10/14/2022- #

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

### The text collada and obj file writers allow importation into the fbx Visual Studio modeller
### and import into Blender. Can be edited in Notepad 

# Bitcoin and Steem donations accepted:
BTC 32HVJHEXh9bMLVkmoSE1d389q2Q1YvfrNE


Steem
@jeff-kubitz


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

Ctrl + Shift N, M, L hold down while drawing draw horizontal, Vertical, 45 degree angle lines

Z-X, C-V - adjust width and height ratios so the mouse matches the cursor for your screen size

Added - Resize including monitor, resultion, pix per inch change, switches across monitors, does not crash if monitor loses power or is unplugged.
      - New or clear button allows another new object to be drawn
      - A lot of try catches for accidental clicks
      - Under the current Solution setup the DirectXToolkitXaml12 is in the $(RootDir)\ Hot3dxRotoDraw can be in any sub-directory
