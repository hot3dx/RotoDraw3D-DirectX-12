//--------------------------------------------------------------------------------------
// File: DirectXPage.xaml.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------
//
// DirectXPage.xaml.h
// Declaration of the DirectXPage class.
//

#pragma once

#include "pch.h"
#include "Generated Files\DirectXPage.g.h"

#include "Common\DeviceResources.h"
#include "Hot3dxRotoDrawMain.h"

#include "Content\Hot3dxRotoDrawVariables.h"

#include <xapo.h>
#include <hrtfapoapi.h>
#include "OmnidirectionalSound.h"

using namespace DX;
using namespace DirectX;
using namespace DirectX::DXTKXAML12;

value struct Scenario;

namespace Hot3dxRotoDraw
{
	public value struct Scenario
	{
		Platform::String^ Title;
		Platform::String^ ClassName;
	};

	/// <summary>
	/// A page that hosts a DirectX SwapChainPanel.
	/// </summary>
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();
		virtual ~DirectXPage();

		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void AudioInitialize();
		void AudioStop();
		void IDC_CLEAR_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void TEXTURE_IMAGE1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void TEXTURE_IMAGE2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SetLeftSwapChainPanel();
		void SetRightSwapChainPanel();
		void ErrorMsgButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e, Platform::String^ msgType, Platform::String^ message);
		void SetErrorMessagePopup(Platform::String^ msgNum, Platform::String^ s);

		DirectXPage^ GetThisDirectXPage() { return this; }

		// Accessors ColorPicker Scenario6_
		Windows::UI::Xaml::Media::SolidColorBrush^ GetBrushFrontFaceColorDXP() { return m_brushFrontFaceColorDXP; };
		Windows::UI::Xaml::Media::SolidColorBrush^ GetBrushBackFaceColorDXP() { return m_brushBackFaceColorDXP; };

		void SetBrushFrontFaceColorDXP(Windows::UI::Color color) {
			m_brushFrontFaceColorDXP = ref new Windows::UI::Xaml::Media::SolidColorBrush(color);
		};
		void SetBrushBackFaceColorDXP(Windows::UI::Color color) {
			m_brushBackFaceColorDXP = ref new Windows::UI::Xaml::Media::SolidColorBrush(color);
		};
		Windows::UI::Color GetFrontColorDXP() { return fcolorDXP; }
		Windows::UI::Color GetBackColorDXP() { return bcolorDXP; }
		void SetFrontColorDXP(Windows::UI::Color c) { fcolorDXP = c; }
		void SetBackColorDXP(Windows::UI::Color c) { bcolorDXP = c; }
		Windows::UI::Color GetPointColorDXP() { return m_pointColorDXP; }
		void SetPointColorDXP(Windows::UI::Color c) { m_pointColorDXP = c; }

		//Shift Key drawing straight line
		bool GetIfRightShiftKeyHeldDrawStraightLine() { return m_bIfRightShiftKeyHeldDrawStraightLine; }
		void SetIfRightShiftKeyHeldDrawStraightLine(bool tf) { m_bIfRightShiftKeyHeldDrawStraightLine = tf; }
		//Shift Key drawing 45 degree line
		bool GetIfLeftShiftKeyHeldDrawStraightLine() { return m_bIfLeftShiftKeyHeldDrawStraightLine; }
		void SetIfLeftShiftKeyHeldDrawStraightLine(bool tf) { m_bIfLeftShiftKeyHeldDrawStraightLine = tf; }
		//Shift Key drawing 45 degree line
		bool GetIfLeftShiftKeyHeldDraw45Line() { return m_bIfLeftShiftKeyHeldDraw45Line; }
		void SetIfLeftShiftKeyHeldDraw45Line(bool tf) { m_bIfLeftShiftKeyHeldDraw45Line = tf; }
		// Shift Key Drawing Sphere from Radius Point
		bool GetIfIfRightShiftKeySphereRadius() { return m_bIfRightShiftKeySphereRadius; }
		void SetIfIfRightShiftKeySphereRadius(bool tf) { m_bIfRightShiftKeySphereRadius = tf; }

		// Accessors Normal Scenario2
		bool GetDoFrontFacesDXP(bool frontFaces) { if (frontFaces) { m_bDoFrontFacesDXP = true; } else { m_bDoFrontFacesDXP = false; } return m_bDoFrontFacesDXP; }
		bool GetDoBackFacesDXP(bool backFaces) { if (backFaces) { m_bDoBackFacesDXP = true; } else { m_bDoBackFacesDXP = false; } return m_bDoBackFacesDXP; }
		void SetDoFrontFacesDXP(bool frontFaces) { m_bDoFrontFacesDXP = frontFaces; }
		void SetDoBackFacesDXP(bool backFaces) { m_bDoBackFacesDXP = backFaces; }

		bool GetAddTopFacesDXP() { return m_bAddTopFacesDXP; }
		void SetAddTopFacesDXP(bool b) { m_bAddTopFacesDXP = b; }
		bool GetAddBottomFacesDXP() { return m_bAddBottomFacesDXP; }
		void SetAddBottomFacesDXP(bool b) { m_bAddBottomFacesDXP = b; }

		bool GetLastToFirstFacesDXP() { return m_bLastToFirstFacesDXP; }
		void SetLastToFirstFacesDXP(bool b) { m_bLastToFirstFacesDXP = b; }
		bool GetObjectDrawnDXP() { return m_bObjectDrawnDXP; }
		void SetObjectDrawnDXP(bool b) { m_bObjectDrawnDXP = b; }
		bool GetFirstToLastDXP() { return m_Scene2Vars->GetFirstToLast(); };
		void SetFirstToLastDXP(bool b) { m_Scene2Vars->SetFirstToLast(b); }
		bool GetAxisCheckedDXP() { return m_Scene2Vars->GetAxisChecked(); }
		void SetAxisCheckedDXP(bool b) {
			m_Scene2Vars->SetAxisChecked(b);
			m_main->GetSceneRenderer()->SetIsYAxis(m_Scene2Vars->GetAxisChecked());
		}
		bool GetTopOrLeftCheckedDXP() { return m_Scene2Vars->GetTopOrLeftChecked(); }
		void SetTopOrLeftCheckedDXP(bool b) {
			m_Scene2Vars->SetTopOrLeftChecked(b);
			if (m_main->GetSceneRenderer()->GetIsYAxis())
			{
				m_main->GetSceneRenderer()->EndTopPointYAxis();
			}
			else
			{
				m_main->GetSceneRenderer()->EndLeftPointsXAxis();
			}
		}
		bool GetBottomOrRightCheckedDXP() { return m_Scene2Vars->GetBottomOrRightChecked(); }
		void SetBottomOrRightCheckedDXP(bool b) {
			m_Scene2Vars->SetBottomOrRightChecked(b);
			if (m_main->GetSceneRenderer()->GetIsYAxis())
			{
				m_main->GetSceneRenderer()->EndBottomPointYAxis();
			}
			else
			{
				m_main->GetSceneRenderer()->EndRightPointsXAxis();
			}
		}
		bool GetLinearAcrossDXP() { return m_bLinearAcrossDXP; }
		void SetLinearAcrossDXP(bool val) { m_bLinearAcrossDXP = val; }
		bool GetLinearUpDXP() { return m_bLinearUpDXP; }
		void SetLinearUpDXP(bool val) { m_bLinearUpDXP = val; }
		void SET_POINTS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
			if (m_main->GetSceneRenderer()->GetPointCount() == 0) {
				return;
			}
			IDC_SET_POINTS_BUTTON_Click(sender, e);
		}
		void SetCullNoneToWireframeRenderer(unsigned int descId) {
			m_main->GetSceneRenderer()->SetCullNoneToWireframe(descId);
		}

		void SetSamplIndexWrapRenderer(unsigned int descId) {
			m_main->GetSceneRenderer()->SetSamplIndexWrap(descId);
		}
		unsigned int GetEffectIndexDXP() {return m_main->GetSceneRenderer()->GetEffectIndex();}
		void SetEffectIndexRenderer(unsigned int descId) {
			m_main->GetSceneRenderer()->SetEffectIndex(descId);
			Hot3dxRotoDraw::Scenario2_Normal::Current->SetEffectDescComboBox(descId);
		}

		// Sets and Gets the number of times a texture will be squared like bricks as a texture
		// on an object - 1.0f is the default
		float GetUVPercentTextureDimensionDXP(){ return m_main->GetSceneRenderer()->GetUVPercentTextureDimension(); }
			void SetUVPercentTextureDimensionDXP(float dimension) {
			m_main->GetSceneRenderer()->SetUVPercentTextureDimension(dimension);
			m_Scene2Vars->SetUVPercentTextureDimensionSc2(dimension);}

		void SET_SPHERE_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e,
			float cameraradius, float camerarotation);
		void SET_SPHERE_GEODESIC_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e,
			float diameter, size_t tesselation);
		void SET_SPHERE_LONG_LAT_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e,
			float diameter, size_t tesselation);

		// Sound Filename Accessors
		Windows::Storage::StorageFile^ GetSound1FileDXP() { return m_Scene1Vars->GetFileSound(); }
		Platform::String^ GetSoundFilePathDXP() { return m_backgroundMusic; }
		void SetFileSound1DXP(Platform::String^ fileName) {
			Platform::String^ file = m_Scene1Vars->GetFileSound()->Path;// = nullptr;
			file = ref new Platform::String(fileName->Data());
			m_Scene1Vars->SetFileNameSound(fileName);
			m_backgroundMusic = ref new Platform::String(fileName->Data());
		}



		// Texture Filename Accessors
		void Setm_bDDS_WIC_FLAGDXP1(bool flag) { m_main->GetSceneRenderer()->Setm_bDDS_WIC_FLAG1(flag); }
		void Setm_bDDS_WIC_FLAGDXP2(bool flag) { m_main->GetSceneRenderer()->Setm_bDDS_WIC_FLAG2(flag); }
		void SetDDS_WIC_FLAGGridPicFileDXP(bool flag) { m_main->GetSceneRenderer()->SetDDS_WIC_FLAGGridPic(flag); }
		void DrawGridPicRectangleDXP() { m_main->GetSceneRenderer()->DrawGridPicRectangle(); }

		Platform::String^ GetTextureImage1FileDXP() { return m_main->GetSceneRenderer()->GetTextureImage1File(); }
		Platform::String^ GetTextureImage2FileDXP() { return m_main->GetSceneRenderer()->GetTextureImage2File(); }
		Platform::String^ GetTextureImage3FileDXP() { return m_main->GetSceneRenderer()->GetTextureImage3File(); }
		Platform::String^ GetTextureImage4FileDXP() { return m_main->GetSceneRenderer()->GetTextureImage4File(); }
		Platform::String^ GetTextureImage5FileDXP() { return m_main->GetSceneRenderer()->GetTextureImage5File(); }
		Platform::String^ GetTextureImage6FileDXP() { return m_main->GetSceneRenderer()->GetTextureImage6File(); }
		//Platform::String^ GetTextureImagePic2FileDXP() { return m_main->GetSceneRenderer()->GetTextureImagePic1File(); }
		//Platform::String^ GetTextureImageVideo2FileDXP() { return m_main->GetSceneRenderer()->GetTextureImageVideo2File(); }

		Platform::String^ GetTextureImage1NameDXP() { return m_main->GetSceneRenderer()->GetTexture1Name(); }
		Platform::String^ GetTextureImage2NameDXP() { return m_main->GetSceneRenderer()->GetTexture2Name(); }
		Platform::String^ GetTextureImage3NameDXP() { return m_main->GetSceneRenderer()->GetTexture3Name(); }
		Platform::String^ GetTextureImage4NameDXP() { return m_main->GetSceneRenderer()->GetTexture4Name(); }
		Platform::String^ GetTextureImage5NameDXP() { return m_main->GetSceneRenderer()->GetTexture5Name(); }
		Platform::String^ GetTextureImage6NameDXP() { return m_main->GetSceneRenderer()->GetTexture6Name(); }
		//Platform::String^ GetTextureImagePic2NameDXP(){ return m_main->GetSceneRenderer()->GetTexturePic2Name(); }
		//Platform::String^ GetTextureImageVideo2NameDXP() { return m_main->GetSceneRenderer()->GetTextureVideo2Name(); }

		void SetTextureImage1NameDXP(Platform::String^ name) { m_main->GetSceneRenderer()->SetTexture1Name(name); }
		void SetTextureImage2NameDXP(Platform::String^ name) { m_main->GetSceneRenderer()->SetTexture2Name(name); }
		void SetTextureImage3NameDXP(Platform::String^ name) { m_main->GetSceneRenderer()->SetTexture3Name(name); }
		void SetTextureImage4NameDXP(Platform::String^ name) { m_main->GetSceneRenderer()->SetTexture4Name(name); }
		void SetTextureImage5NameDXP(Platform::String^ name) { m_main->GetSceneRenderer()->SetTexture5Name(name); }
		void SetTextureImage6NameDXP(Platform::String^ name) { m_main->GetSceneRenderer()->SetTexture6Name(name); }
		void SetTextureImagePic2NameDXP(Platform::String^ name) { m_main->GetSceneRenderer()->SetTexturePic2Name(name); }
		void SetTextureImageVideo2NameDXP(Platform::String^ name) { m_main->GetSceneRenderer()->SetTextureVideo2Name(name); }

		Platform::String^ GetTextureImageGridPicFileDXP() { return m_main->GetSceneRenderer()->GetTextureImageGridPicFile(); }

		void SetTextureImage1FileDXP(Platform::String^ fileName) {
			Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImage1File();// = nullptr;
			file = ref new Platform::String(fileName->Data());
			m_main->GetSceneRenderer()->SetTextureImage1File(fileName);
		}
		void SetTextureImage2FileDXP(Platform::String^ fileName) {
			Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImage2File();// = nullptr;
			file = ref new Platform::String(fileName->Data());
			m_main->GetSceneRenderer()->SetTextureImage2File(fileName);
		}
		void SetTextureImage3FileDXP(Platform::String ^ fileName) {
				Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImage3File();// = nullptr;
				file = ref new Platform::String(fileName->Data());
				m_main->GetSceneRenderer()->SetTextureImage3File(fileName);
		}
		void SetTextureImage4FileDXP(Platform::String ^ fileName) {
					Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImage4File();// = nullptr;
					file = ref new Platform::String(fileName->Data());
					m_main->GetSceneRenderer()->SetTextureImage4File(fileName);
		}
		void SetTextureImage5FileDXP(Platform::String^ fileName) {
			Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImage5File();// = nullptr;
			file = ref new Platform::String(fileName->Data());
			m_main->GetSceneRenderer()->SetTextureImage5File(fileName);
		}
		void SetTextureImage6FileDXP(Platform::String^ fileName) {
			Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImage6File();// = nullptr;
			file = ref new Platform::String(fileName->Data());
			m_main->GetSceneRenderer()->SetTextureImage6File(fileName);
		}
		// Scenario5_Tex
		Platform::String^ GetTextureImagePicFileDXP() { return m_main->GetSceneRenderer()->GetTextureImagePicFile(); }
		void SetTextureImagePicFileDXP(Platform::String^ fileName) {
			Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImagePicFile();// = nullptr;
			file = ref new Platform::String(fileName->Data());
			m_main->GetSceneRenderer()->SetTextureImagePic1File(fileName);
		}
		Platform::String^ GetTextureImageVideo2FileDXP() { return m_main->GetSceneRenderer()->GetTextureImageVideo2File(); }
		void SetTextureImageVideo2FileDXP(Platform::String^ fileName) {
			Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImageVideo2File();// = nullptr;
			file = ref new Platform::String(fileName->Data());
			m_main->GetSceneRenderer()->SetTextureImageVideo2File(fileName);
		}
		
		// Scenario7_SphereDraw
		Platform::String^ GetVideoTextureImageFileDXP() { return m_main->GetSceneRenderer()->GetTextureImageVideoFile(); }
		void SetVideoTextureImageFileDXP(Platform::String^ fileName) {
			Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImageVideoFile();// = nullptr;
			file = ref new Platform::String(fileName->Data());
			m_main->GetSceneRenderer()->SetTextureImageVideoFile(fileName);
		}
		// Scenario11_GridorPic
		Platform::String^ GetGridPicTextureImageFileDXP() { return m_main->GetSceneRenderer()->GetTextureImageGridPicFile(); }
		void SetGridPicTextureImageFileDXP(Platform::String^ fileName) {
			Platform::String^ file = m_main->GetSceneRenderer()->GetTextureImageGridPicFile();// = nullptr;
			file = ref new Platform::String(fileName->Data());
			m_main->GetSceneRenderer()->SetTextureImageGridPicFile(fileName);
		}
		// Scale Variables
		float Get_xScaleDrawnObject() { return m_xScaleDrawnObject; }
		void Set_xScaleDrawnObject(float x) { m_xScaleDrawnObject = x; }
		float Get_yScaleDrawnObject() { return m_yScaleDrawnObject; }
		void Set_yScaleDrawnObject(float y) { m_yScaleDrawnObject = y; }
		float Get_zScaleDrawnObject() { return m_zScaleDrawnObject; }
		void Set_zScaleDrawnObject(float z) { m_zScaleDrawnObject = z; }

		// Translate Variables
		float Get_xTranslateDrawnObject() { return m_xTranslateDrawnObject; }
		void Set_xTranslateDrawnObject(float x) { m_xTranslateDrawnObject = x; }
		float Get_yTranslateDrawnObject() { return m_yTranslateDrawnObject; }
		void Set_yTranslateDrawnObject(float y) { m_yTranslateDrawnObject = y; }
		float Get_zTranslateDrawnObject() { return m_zTranslateDrawnObject; }
		void Set_zTranslateDrawnObject(float z) { m_zTranslateDrawnObject = z; }

		void SetTranslateLineValues() {
			m_main->GetSceneRenderer()->CalculateLinePointsTranslate(m_xTranslateDrawnObject, m_yTranslateDrawnObject, m_zTranslateDrawnObject);
		}
		void SetTranslateAllLinesValues() {
			m_main->GetSceneRenderer()->CalculateAllLinesPointsTranslate(m_xTranslateDrawnObject, m_yTranslateDrawnObject, m_zTranslateDrawnObject);
		}
		// Rotate Variables
		float Get_xRotateDrawnObject() { return m_xRotateDrawnObject; }
		void Set_xRotateDrawnObject(float x) { m_xRotateDrawnObject = x; }
		float Get_yRotateDrawnObject() { return m_yRotateDrawnObject; }
		void Set_yRotateDrawnObject(float y) { m_yRotateDrawnObject = y; }
		float Get_zRotateDrawnObject() { return m_zRotateDrawnObject; }
		void Set_zRotateDrawnObject(float z) { m_zRotateDrawnObject = z; }

		float GetPointDrawGroupAngleDXP() { return m_fPointDrawGroupAngleDXP; }
		void SetPointDrawGroupAngleDXP(float f) { m_fPointDrawGroupAngleDXP = f; }

		float GetPointSpacingDXP() { return m_fPointSpacingDXP; }
		void SetPointSpacingDXP(float f) {
			m_fPointSpacingDXP = f;
			m_main->GetSceneRenderer()->SetPointSpace(f);
			m_main->GetSceneRenderer()->SetScrollDist(f);
		}
		unsigned int GetPointCount() {
			return m_main->GetSceneRenderer()->GetPointCount();
		}

		unsigned int GetGroupCount() {
			return m_main->GetSceneRenderer()->GetGroupCount();
		}
		
		size_t GetPointsTotalCount() {
			return m_main->GetSceneRenderer()->GetTotalPointCount();
		}

		Platform::Array<uint16_t>^ GetPtGroupListIndicesDXP(size_t index) {
			Platform::Array<uint16_t>^ list = ref new Platform::Array<uint16_t>(GetGroupCount());
			list = m_main->GetSceneRenderer()->GetPtGroupList().at(index)->GetPtList();
			return list;
		}
		Platform::String^ Getm_sDirPathDXP() { return m_sDirPathDXP; }
		void SetProgramDirPathDXP(Platform::String^ s) { m_sDirPathDXP = ref new Platform::String(s->Data()); }
		

		internal:
		
		std::vector<DirectX::DXTKXAML12::VertexPositionColor> GetVerticesDXP() { return m_main->GetSceneRenderer()->GetVertices(); }
		std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> GetVertexesDXP() { return m_main->GetSceneRenderer()->GetVertexes(); }
		std::vector<uint16_t> GetIndices() { return m_main->GetSceneRenderer()->GetIndices(); }
		std::vector<float> GetTextureU() { return m_main->GetSceneRenderer()->GetTextureU(); }
		std::vector<float> GetTextureV() { return m_main->GetSceneRenderer()->GetTextureV(); }
		unsigned int GetVerticeColorIndexDXP(unsigned int j, unsigned int i) { return m_main->GetSceneRenderer()->GetVerticeColorIndex(j, i); }
		DirectX::XMFLOAT3 GetVerticeColorDXP(unsigned int j) { return m_main->GetSceneRenderer()->GetVerticeColor(j); }
		void SetVerticeColorDXP(unsigned int j, DirectX::XMFLOAT3 pos) { m_main->GetSceneRenderer()->SetVerticeColor(j, pos); }

		void SetHot3dxDrawnObjectSculpt() { m_main->GetSceneRenderer()->InitDrawnObjectSculptWireframe(); }

		public:
		float GetPartialRotateAngleDXP() { return m_Scene2Vars->GetPartialRotateAngle(); }
		void  SetPartialRotateAngleDXP(float angle) { m_Scene2Vars->SetPartialRotateAngle(angle); }

		// Rotate GridCam Variables Scenario10_Sculpt sliders
		float Get_xRotateGridCam() { return m_xRotateGridCam; }
		void Set_xRotateGridCam(float x) { m_xRotateGridCam = x; }
		float Get_yRotateGridCam() { return m_yRotateGridCam; }
		void Set_yRotateGridCam(float y) { m_yRotateGridCam = y; }
		float Get_zRotateGridCam() { return m_zRotateGridCam; }
		void Set_zRotateGridCam(float z) { m_zRotateGridCam = z; }
		void SetRotateYaw(float deg) { RotateYaw(deg); }
		void SetRotatePitch(float deg) { RotatePitch(deg); }
		//double        dxpGetTimer() { double ts = m_main->GetSceneRenderer()->GetTimer(); return ts; }
		void SetXRotationScenario10(float value) {
			//DirectX::XMStoreFloat4x4(&m_main->GetSceneRenderer()->GetWorld4X4(), DirectX::XMMatrixRotationX((float)dxpGetTimer() * value));
			m_main->GetSceneRenderer()->RotatePitchSquid(value);
		}
		void SetYRotationScenario10(float value) {
			//XMStoreFloat4x4(&m_main->GetSceneRenderer()->GetWorld4X4(), DirectX::XMMatrixRotationY((float)dxpGetTimer() * value));
			m_main->GetSceneRenderer()->RotateYawSquid(value);
		}

		// Audio Sound
		void SetOnTimerTickDXP(Object^ sender, Object^ e) { OnTimerTickDXP(sender, e); }
		// Audio
		bool Get_initializedDXP() { return _initializedDXP; }
		void Set_initializedDXP(bool b) { _initializedDXP = b; }
		void Start_startDXP() { _startDXP.Start(); }
		void Stop_startDXP() { _startDXP.Stop(); }
		void Start_startOnUpdate(float a, float h, float r) { _startDXP.OnUpdate(a, h, r); }
		//void Start_startDXPSetEnvironment(HrtfEnvironment env) { _startDXP.SetEnvironment(env); }
		Windows::UI::Xaml::DispatcherTimer^ Get_timerDXP() { return _timerDXP; }
		void Set_timerDXP(Windows::UI::Xaml::DispatcherTimer^ timer) { _timerDXP = timer; }
		Windows::Foundation::EventRegistrationToken Get_timerEventTokenDXP() { return _timerEventTokenDXP; }
		void Set_timerEventTokenDXP(Windows::Foundation::EventRegistrationToken token) { _timerEventTokenDXP = token; }
		float Get_radiusDXP() { return _radiusDXP; }  // Radius of the orbit
		void  Set_radiusDXP(float radius) { _radiusDXP = radius; }
		float Get_heightDXP() { return _heightDXP; } // Height at which the sound is orbiting (0 for centered around listener's head, +ve for above and -ve for below)
		void Set_heightDXP(float height) { _heightDXP = height; }
		float Get_angularVelocityDXP() { return _angularVelocityDXP; } // Speed of orbit, default is stationary
		void Set_angularVelocityDXP(float angular) { _angularVelocityDXP = angular; }

		// Hot3dxRotoDrawSceneRender Texture Filename Accessors Scenario5_MatsTexs stuff
		Platform::String^ GetSceneRenderDrawnObjectOpenText() {
			return m_main->GetSceneRenderer()->DrawnObjectOpenText();
		}
		Platform::String^ GetSceneRenderDrawnObjectOpenBinary() {
			return m_main->GetSceneRenderer()->DrawnObjectOpenBinary();
		}
		
		Platform::String^ GetSceneRenderDrawnObjectSaveText(Platform::String^ fileName, unsigned int objectCount) {
			return m_main->GetSceneRenderer()->DrawnObjectSaveText(fileName, objectCount);
		}
		Platform::String^ GetSceneRenderDrawnObjectSaveObj3DandMtl(Platform::String^ effectName,
			unsigned int illumType,
			Platform::String^ mtlObjFilename,
			Platform::String^ textureFilename)
		{
			illumType = 2;
			return m_main->GetSceneRenderer()->DrawnObjectSaveObj3DandMtl(effectName, illumType, mtlObjFilename, textureFilename);
		}
		Platform::String^ GetSceneRenderDrawnObjectSaveObj3DFile(Platform::String^ mtlObjFilename,
			Platform::String^ nodeName,
			Platform::String^ effectName)
		{
			return m_main->GetSceneRenderer()->DrawnObjectSaveObjFile(mtlObjFilename,
				nodeName,
				effectName);
		}
		Platform::String^ GetSceneRenderDrawnObjectSaveBinary() {
			return m_main->GetSceneRenderer()->DrawnObjectSaveBinary();
		}

		

		void FullScreen_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		Scenario1Vars^ GetScene1Vars() { return m_Scene1Vars; }
		Scenario2Vars^ GetScene2Vars() { return m_Scene2Vars; }
		Scenario5Vars^ GetScene5Vars() { return m_Scene5Vars; }
		Scenario5TexVars^ GetScene5TexVars() { return m_Scene5TexVars; }
		Scenario7Vars^ GetScene7Vars() { return m_Scene7Vars; }
		Scenario10Vars^ GetScene10Vars() { return m_Scene10Vars; }
		Scenario11Vars^ GetScene11Vars() { return m_Scene11Vars; }
		Scenario13Vars^ GetScene13Vars() { return m_Scene13Vars; }

		// Symmetrical Copy of a flipped set of points
		// on the other side of the X or Y axis
		void CopyFlipPointsXAxisDXP() {
			if (m_main->GetSceneRenderer()->GetPointCount() <= 0)return;
			m_main->GetSceneRenderer()->CopyFlipPointsXAxis();
		}

		void CopyFlipPointsYAxisDXP() {
			if (m_main->GetSceneRenderer()->GetPointCount() <= 0)return;
			m_main->GetSceneRenderer()->CopyFlipPointsYAxis();
		}

		// on the other side of the X or Y axis
		void CopyPointsXYAxisDXP() {
			if (m_main->GetSceneRenderer()->GetPointCount() <= 0)return;
			m_main->GetSceneRenderer()->CopyPointsXYAxis();
		}

		void DrawPointsXYAxisDXP() {
			if (m_main->GetSceneRenderer()->GetCpyPointCount() <= 0)return;
			m_main->GetSceneRenderer()->DrawCopyPointsXYAxis(m_main->GetSceneRenderer()->GetCpyPointCount());
		}

		void ClearPointsXYAxisDXP() {
			if (m_main->GetSceneRenderer()->GetPointCount() <= 0)return;
			m_main->GetSceneRenderer()->ClearCopyPointsXYAxis();
		}

		void ScreenGrabDXP();
		

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:

		void ScenarioControl_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void Footer_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void UpdateStatus(Platform::String^ strMessage, NotifyType type);
		// XAML low-level rendering event handler.
		void OnRendering(Platform::Object^ sender, Platform::Object^ args);
		float FrameChange();


		// Window event handlers.
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// Other event handlers.
		void AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

		void IDC_WELCOME_STATIC_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void IDC_NEW_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		// Track our independent input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

		// Independent input handling functions.
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerWheelChanged(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerEntered(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerExited(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerCaptureLost(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

		void RotateYaw(float deg);

		void RotatePitch(float deg);

		void OnKeyDown(Windows::UI::Core::CoreWindow^ window, Windows::UI::Core::KeyEventArgs^ args);
		void OnKeyUp(Windows::UI::Core::CoreWindow^ /*window*/, Windows::UI::Core::KeyEventArgs^ args);

		// Resources used to render the DirectX content in the XAML page background.
		std::shared_ptr<DeviceResources> m_deviceResources;
		std::unique_ptr<Hot3dxRotoDrawMain> m_main;
		

		bool m_windowVisible;
		/*
		void SphereRadiusTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void PointSpacingTextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void DrawSphereButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void IDC_SET_POINTS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		*/
		void IDC_SET_POINTS_BUTTON_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		//ColorPicker variables
		Windows::UI::Xaml::Media::SolidColorBrush^ m_brushFrontFaceColorDXP;
		Windows::UI::Xaml::Media::SolidColorBrush^ m_brushBackFaceColorDXP;
		Windows::UI::Color fcolorDXP;
		Windows::UI::Color bcolorDXP;
		Windows::UI::Color m_pointColorDXP;
		// Shift Key straightLine
		bool m_bIfRightShiftKeyHeldDrawStraightLine; // "M" Key
		bool m_bIfLeftShiftKeyHeldDrawStraightLine;  // "N" Key
		// Shift Key 45 degree Line
		bool m_bIfLeftShiftKeyHeldDraw45Line; // "L" key
		bool m_bIfRightShiftKeySphereRadius;  // "K" key
		//Scenario2_Normal Variables
		bool m_bDoFrontFacesDXP;
		bool m_bDoBackFacesDXP;
		bool m_bAddTopFacesDXP;
		bool m_bAddBottomFacesDXP;
		bool m_bLastToFirstFacesDXP;
		bool m_bObjectDrawnDXP;
		bool m_bLinearAcrossDXP;
		bool m_bLinearUpDXP;

		float m_fPointDrawGroupAngleDXP;
		float m_fPointSpacingDXP;
		float m_fPartialRotateAngleDXP;

		// Scale Variables
		float m_xScaleDrawnObject;
		float m_yScaleDrawnObject;
		float m_zScaleDrawnObject;

		// Translate Variables
		float m_xTranslateDrawnObject;
		float m_yTranslateDrawnObject;
		float m_zTranslateDrawnObject;

		// Rotate Variables
		float m_xRotateDrawnObject;
		float m_yRotateDrawnObject;
		float m_zRotateDrawnObject;

		// Rotate GridCam Variables Scenatio_10Sculpt Sliders
		float m_xRotateGridCam;
		float m_yRotateGridCam;
		float m_zRotateGridCam;

		// Camera Variables
		float m_cameraradius;
		float m_camerarotation;
		float m_anglerotation;
		int m_iPointCount;

		int m_mouseWheelDeltaDXP;

		// fullscreen
		bool m_fullscreen;
		bool IsLeftMenuSwapChainPanel = true;

		void MyFilePathDXP(Platform::String^ image);
		Platform::String^ m_sDirPathDXP = ref new Platform::String();
		bool m_bDirPathFound;

		//void ScreenGrabDXP();
		bool m_bIsScreenGrabInProgress = true;
		
	internal:
		
		static DirectXPage^ Current;
		void NotifyUser(Platform::String^ strMessage, NotifyType type);
		Scenario1Vars^ m_Scene1Vars = ref new Scenario1Vars();
		Scenario2Vars^ m_Scene2Vars = ref new Scenario2Vars();
		Scenario5Vars^ m_Scene5Vars = ref new Scenario5Vars();
		Scenario5TexVars^ m_Scene5TexVars = ref new Scenario5TexVars();
		Scenario7Vars^ m_Scene7Vars = ref new Scenario7Vars();
		Scenario10Vars^ m_Scene10Vars = ref new Scenario10Vars();
		Scenario11Vars^ m_Scene11Vars = ref new Scenario11Vars();
		Scenario13Vars^ m_Scene13Vars = ref new Scenario13Vars();

		static property Platform::String^ FEATURE_NAME
		{
			Platform::String^ get()
			{
				return "Kubed3D Hot3dx Maker2 12";
			}
		}



		static property Platform::Array<Scenario>^ scenarios
		{
			Platform::Array<Scenario>^ get()
			{
				return scenariosInner;
			}
		}

		Platform::String^ m_backgroundMusic = ref new Platform::String(L"");

	private:

		static Platform::Array<Scenario>^ scenariosInner;

		void ScenarioFrame_Navigated(Platform::Object^ sender, Windows::UI::Xaml::Navigation::NavigationEventArgs^ e);

		void OnTimerTickDXP(Object^ sender, Object^ e);
		// Audio
		bool                            _initializedDXP = false;
		OmnidirectionalSound            _startDXP;
		Windows::UI::Xaml::DispatcherTimer^ _timerDXP;
		Windows::Foundation::EventRegistrationToken          _timerEventTokenDXP;
		float                           _radiusDXP = 2.0f;            // Radius of the orbit
		float                           _heightDXP = 0;               // Height at which the sound is orbiting (0 for centered around listener's head, +ve for above and -ve for below)
		float                           _angularVelocityDXP = 0;      // Speed of orbit, default is stationary
	};
}
