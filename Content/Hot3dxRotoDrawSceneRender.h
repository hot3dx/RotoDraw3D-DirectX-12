#pragma once

#include "DirectXPage.xaml.h"
#include "Common\DeviceResources.h"
#include "Content\ShaderStructures.h"
#include "Common\StepTimer.h"
#include <Graphics\AlignedNewXaml12.h>
#include <Graphics\Hot3dxGeometry.h>
#include <Graphics\RenderTargetStateXaml12.h>
#include <Graphics\EffectPipelineStateDescriptionXaml12.h>
#include <Graphics\CommonStatesXaml12.h>
#include <Graphics\GraphicsMemoryXaml12.h>
#include <Graphics\DescriptorHeapXaml12.h>
#include <Graphics\EffectCommonXaml12.h>
#include <Graphics\DDSTextureLoaderXaml12.h>
#include <Graphics\VertexTypesXaml12.h>
#include <Graphics\SimpleMathXaml12.h>
#include <Graphics\ModelXaml12.h>
#include <Graphics\PrimitiveBatchXaml12.h>
#include <Graphics\GeometricPrimitiveXaml12.h>
#include "Content\Hot3dxDrawnObject.h"
#include <Graphics\SpriteBatchXaml12.h>
#include <Graphics\SpriteFontXaml12.h>
#include <Graphics\Hot3dxCamera.h>
#include <Audio\AudioXaml12.h>
#include <Audio\MediaReaderXaml12.h>
#include <Graphics\Hot3dxGeometry.h>
#include <Graphics\Hot3dx12Rotate.h>
#include "Hot3dxRotoDrawVariables.h"
#include <thread>
#include <chrono>
#include <assert.h>

namespace Hot3dxRotoDraw
{
	ref class PtGroups sealed
	{
	public:
		PtGroups();
		virtual ~PtGroups() { ZeroOutPtList(); }
		Platform::Array<uint16_t>^ GetPtList() { return m_PtList; }
		void SetPtList(unsigned int i, uint16_t val) { m_PtList->set(i, val); }
		uint16_t GetListPt(unsigned int i) { return m_PtList->get(i); }
		void ZeroOutPtList() { m_PtList->~Array(); }
	private:
		Platform::Array<uint16_t>^ m_PtList = ref new Platform::Array<uint16_t>(1000);
	};
	// This sample renderer instantiates a basic rendering pipeline.
	ref class RotoDrawSceneRender
	{
	public:
		virtual ~RotoDrawSceneRender();
		
	internal:
		RotoDrawSceneRender(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void Update(DX::StepTimer const& timer);
		bool Render();
		void Clear();

		void SaveState();

		void StartTracking();
		BOOL checkDistance(float x, float y, float z, float mouseMoveDistDelta);
		void TrackingUpdate(float positionX, float positionY);
		void StopTracking();
		bool IsTracking() { return m_tracking; }
		void ReleaseDeviceDependentResources();
		void OnDeviceLost();
		void OnDeviceRestored();
		//CCameraXYMoveRotation          m_CamXYMoveRotate;
		void XM_CALLCONV MouseCursorRender(float positionX, float positionY);
		void XM_CALLCONV DrawPointsOne(XMFLOAT3 cursorPos);
		void XM_CALLCONV CopyFlipPointsXAxis();
		void XM_CALLCONV CopyFlipPointsYAxis();

		// Centered End Points Top and Bottom X Axis
		void XM_CALLCONV EndTopPointYAxis();
		void XM_CALLCONV EndBottomPointYAxis();
		// Centered End Points Left and Right Y Axis
		void XM_CALLCONV EndLeftPointsXAxis();
		void XM_CALLCONV EndRightPointsXAxis();
		//void XM_CALLCONV EndPointSetPointValues(float r, float g, float b, float alpha);
		//void XM_CALLCONV EndPointSetFaceValues();
		unsigned int m_iEndPointOneCount;
		unsigned int m_iEndPointTwoCount;

		bool                           is3DVisible;

		bool GetLoadingComplete() { return m_loadingComplete; }
		void SetLoadingComplete(bool complete) { m_loadingComplete = complete; }



		void LoadState();
		void Rotate(float radians);
		//void XM_CALLCONV Draw3DCursorXY(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, XMFLOAT3 curPos);
		void XM_CALLCONV Draw3DCursorXY(XMFLOAT3 curPos);
		void XM_CALLCONV ClearGridPicRectangle();
		void XM_CALLCONV DrawGridPicRectangle();
		//void XM_CALLCONV DrawLineOnlyObject(FXMVECTOR xAxis, FXMVECTOR zAxis, FXMVECTOR origin, GXMVECTOR color);
		void XM_CALLCONV DrawLineOnlyObject(GXMVECTOR color);
		void XM_CALLCONV DrawGridXY(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color);
		void XM_CALLCONV DrawGridXZ(FXMVECTOR xAxis, FXMVECTOR zAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color);
		void XM_CALLCONV InitDrawnObjectSingleTexture();
		void XM_CALLCONV InitDrawnObjectDualTexture();
		void XM_CALLCONV ClearDrawnObject();
		void ScreenMouse3DWorldAlignment();
		void XM_CALLCONV PointDataValues(unsigned int number, float x, float y, float z);
		Platform::String^ XM_CALLCONV PointCountString(Platform::String^ m_fontString, Platform::String^ objectString, unsigned int cnt);
		Platform::String^ XM_CALLCONV ObjectXYZPositionString(Platform::String^ m_fontString, Platform::String^ objectString, float x, float y, float z);

		void OnLButtonDown(UINT nFlags, XMFLOAT2 point);
		void OnRightButtonDown(UINT nFlags, XMFLOAT2 point);
		void OnMouseMove(UINT nFlags, XMFLOAT2 point);

		void XM_CALLCONV Copy();

		void XM_CALLCONV InitDimensionsBox();
		void XM_CALLCONV CreateDimensions(XMFLOAT3* copier, size_t qCount);
		void XM_CALLCONV MakeBox(XMFLOAT3* copier, int qCount, Platform::Array<float>^ b);

		// Not Yet Used Draws Mesh Points 
		unsigned int GetPointCount() { return m_iPointCount; }
		void XM_CALLCONV SetPoints(); // RotoDraw3D old SetPintsButton Function

		void ViewMatrix(XMFLOAT4X4 M, wchar_t* str);

		// Accessors
		bool Getm_bLButtonDown() { return m_bLButtonDown; }
		bool Getm_bRButtonDown() { return m_bRButtonDown; }
		bool Getm_bMButtonDown() { return m_bRButtonDown; }
		bool Getm_bMouseMove() { return m_bMouseMove; }
		void Setm_bLButtonDown(bool b) { m_bLButtonDown = b; }
		void Setm_bRButtonDown(bool b) { m_bRButtonDown = b; }
		void Setm_bMButtonDown(bool b) { m_bRButtonDown = b; }
		void Setm_bMouseMove(bool b) { m_bMouseMove = b; }

		// Camera vectors
		Hot3dxCamera^ GetCamera() { return m_camera; }
		void Setm_EyeX(float e) { m_EyeX += e; }
		void Setm_EyeY(float e) { m_EyeY += e; }
		void Setm_EyeZ(float e) { m_EyeZ += e; }
		float GetEyeX() { return m_EyeX; }
		float GetEyeY() { return m_EyeY; }
		float GetEyeZ() { return m_EyeZ; }
		void Setm_LookAtX(float e) { m_LookAtX += e; }
		void Setm_LookAtY(float e) { m_LookAtY += e; }
		void Setm_LookAtZ(float e) { m_LookAtZ += e; }
		float GetLookAtX() { return m_LookAtX; }
		float GetLookAtY() { return m_LookAtY; }
		float GetLookAtZ() { return m_LookAtZ; }
		void Setm_UpX(float e) { m_UpX += e; }
		void Setm_UpY(float e) { m_UpY += e; }
		void Setm_UpZ(float e) { m_UpZ += e; }
		float GetUpX() { return m_UpX; }
		float GetUpY() { return m_UpY; }
		float GetUpZ() { return m_UpZ; }

		// Camera Rotation around object transforms world coordinates
		void RotatePitch(float degree);
		void RotateYaw(float degree);
		bool GetRotateKeyPressed() {return m_bRotateKeyPressed;	}
		void SetRotateKeyPressed(bool b) { m_bRotateKeyPressed = b; }

		// Calculates next Group Location
		
		float GetPointDrawGroupAngle() { return m_fPointDrawGroupAngle; }
		void SetPointDrawGroupAngle(float f) { m_fPointDrawGroupAngle = f; }

		// point spacing
		float GetScrollDist() { return m_fScrollDist; }
		void SetScrollDist(float dist) { m_fScrollDist = dist; }
		float GetPointSpace() { return m_fPointSpace; }
		void SetPointSpace(float dist) { m_fPointSpace = dist; }

		// Draws the single line fro which the object is calculated
		void IncrementPtGroups(){
			PtGroups^ ptGroups = ref new PtGroups();
			m_PtGroupList.push_back(ptGroups);
		}
		uint16_t XM_CALLCONV DrawObjectPoints(uint16_t n);
		void XM_CALLCONV DrawObjectPointsTop();
		void XM_CALLCONV DrawObjectPointsBottom();
		void XM_CALLCONV DrawObjectPointsTopBottom();

		// Sets Width and height drawing mouse ratios
		void SetMouseWidthRatio(float w) { m_widthRatio = m_widthRatio + w;}
		void SetMouseHeightRatio(float h) { m_heightRatio = m_heightRatio + h; }
		float GetMouseWidthRatio() { return m_widthRatio; }
		float GetMouseHeightRatio() { return m_heightRatio; }
		void MouseWidthHeightRatioOutput()
		{
			TCHAR dest[100];
			TCHAR* str = { L"W: %.6f H: %.6f\n" };
			StringCbPrintf(dest, 100, str, GetMouseWidthRatio(), GetMouseHeightRatio());
			//OutputDebugString(dest);
		}
		// .txt and .hbin file readers
		Platform::String^ DrawnObjectOpenText();
		Platform::String^ DrawnObjectOpenBinary();

		// .mtl file writer
		Platform::String^ DrawnObjectSaveObj3DandMtl(
			Platform::String^ effectName,
			unsigned int illumType,
			Platform::String^ mtlObjFilename,
			Platform::String^ textureFilename );
		// .obj file writer
		Platform::String^ DrawnObjectSaveObjFile(
			Platform::String^ mtlObjFilename, 
			Platform::String^ nodeName,
			Platform::String^ effectName);

		// .txt and .hbin file writers
		Platform::String^ DrawnObjectSaveText(Platform::String^ fileName, unsigned int objectCount);
		Platform::String^ DrawnObjectSaveBinary();

		// Texture Filename Accessors
		void Setm_bDDS_WIC_FLAG1(bool flag) { m_bDDS_WIC_FLAG1 = flag; }
		void Setm_bDDS_WIC_FLAG2(bool flag) { m_bDDS_WIC_FLAG2 = flag; }
		void SetDDS_WIC_FLAGGridPic(bool flag) { m_bDDS_WIC_FLAGGridPic = flag; }
		Platform::String^ GetTextureImage1File() { return m_textureImage1File;  }
		void SetTextureImage1File(Platform::String^ fileName) { 
			m_textureImage1File = nullptr; 
			m_textureImage1File = ref new Platform::String(fileName->Data()); 
		}
		Platform::String^ GetTextureImage2File() { return m_textureImage2File; }
		void SetTextureImage2File(Platform::String^ fileName) { 
			m_textureImage2File = nullptr; 
			m_textureImage2File = ref new Platform::String(fileName->Data());
		}
		// Scenario111-GridPic Accessors
		Platform::String^ GetTextureImageGridPicFile() { return m_textureImageGridPicFile; }
		void SetTextureImageGridPicFile(Platform::String^ fileName) {
			m_textureImageGridPicFile = nullptr;
			m_textureImageGridPicFile = ref new Platform::String(fileName->Data());
		}

		bool GetIsYAxis() { return m_bIsYAxis; }
		void SetIsYAxis(bool b) { m_bIsYAxis = b; }
		
	private:
		
		// Calculates the faces of a mesh
		void CalculateMeshFaces();
		void CalculateMeshFacesTopBottom();
		void XM_CALLCONV EndpointTopLeftFaces();
		void XM_CALLCONV EndpointBottomRightFaces();

		// Calculates Texture Coordinates for whole object
		void XM_CALLCONV GetUVPercent();
		//void XM_CALLCONV GetUVPercentTop();
		float* XM_CALLCONV GetU(XMVECTOR v, Platform::Array<float>^ box);
		// Needed to Get DirectXPage^
		Hot3dxRotoDrawVariables^ m_vars;

	protected private:
	
		
		// Indices into the application state map.
		Platform::String^ AngleKey = "Angle";
		Platform::String^ TrackingKey = "Tracking";

		// Track our independent input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_drawObjectWorker;
		Concurrency::critical_section m_drawCriticalSection;

		XMFLOAT3 pSect;
		bool m_bFaceSelected;
		int m_iV;
		

		int m_iDrawMode;
		unsigned int m_GroupListSelectedIndex;
		unsigned int m_iPointCount;
		size_t m_iTotalPointCount;
		//unsigned int m_iLastPoint;
		XMFLOAT2 point;
		Platform::Array<unsigned int>^ m_iTempGroup = ref new Platform::Array<unsigned int>(1000);
		Platform::Array<float>^ m_iTempMouseX = ref new Platform::Array<float>(1000);
		Platform::Array<float>^ m_iTempMouseY = ref new Platform::Array<float>(1000);
		
		std::vector<PtGroups^> m_PtGroupList;// = ref new Platform::Array<PtGroups>(360);
		std::vector<DirectX::VertexPositionColor> vertices;
		std::vector<DirectX::VertexPositionNormalTexture> vertexes;
		std::vector<uint16_t> indices;
		std::vector<float> textureU;
		std::vector<float> textureV;
		//unsigned int m_iTextureUVCount;

		unsigned int m_iTempGroupCount;
		unsigned int m_iGroupCount;
		std::vector<XMFLOAT3> pos;

		float m_fScrollDist;
		float m_fPointSpace;
		int m_iScrollPointSetPos;
		float distX;
		float distY;
		float distZ;

		float Setrotx;
		float Setroty;
		float Setrotz;

		float m_widthRatio;
		float m_heightRatio;


		// Constant buffers must be 256-byte aligned.
		static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_sceneDeviceResources;
		
		// Rendering loop timer.
		DX::StepTimer                               m_timer;


		ModelViewProjectionConstantBuffer					m_constantBufferData;
		UINT8* m_mappedConstantBuffer;
		UINT												m_cbvDescriptorSize;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		bool    m_loadingDrawnObjectComplete;
		bool    m_performScaleDrawnObjectComplete;
		float	m_radiansPerSecond;
		float	m_angle;
		bool	m_tracking;
		UINT    sceneVertexCount;
		// Track our independent input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		//Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;
		/////////////
		// DirectXTK Test objects.
		std::shared_ptr<DirectX::ResourceUploadBatch>                           m_resourceUpload;
		std::unique_ptr<DirectX::GraphicsMemory>                                m_graphicsMemory;
		std::unique_ptr<DirectX::DescriptorHeap>                                m_resourceDescriptors;
		std::unique_ptr<DirectX::CommonStates>                                  m_states;
		std::unique_ptr<DirectX::BasicEffect>                                   m_lineEffect;
		std::unique_ptr<DirectX::BasicEffect>                                   m_cursorEffect;
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
		std::unique_ptr<DirectX::BasicEffect>                                   m_shapeEffect;
		std::unique_ptr<DirectX::BasicEffect>                                   m_artistCameraEffect;
		std::unique_ptr<DirectX::BasicEffect>                                   m_shapeTetraEffect;
		std::unique_ptr<DirectX::BasicEffect>                                   m_drawRectangleEffect;
		std::unique_ptr<DirectX::Model>                                         m_model;
		std::vector<std::shared_ptr<DirectX::IEffect>>                          m_modelEffects;
		std::unique_ptr<DirectX::EffectTextureFactory>                          m_modelResources;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_shape;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_artistCamera;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_shapeTetra;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_shapeGridPic;
		std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
		std::unique_ptr<DirectX::SpriteFont>                                    m_CameraEyeFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_CameraAtFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_CameraUpFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_MousePosFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_PointCountFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_TotalPointCountFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_FaceCountFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_GroupCountFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_SelectedPointNumberFont;
		std::unique_ptr<DirectX::DualTextureEffect>                             m_dualTextureEffect;
		
		std::unique_ptr<DirectX::BasicEffect>                                   m_shapeDrawnObjectEffect;
		std::unique_ptr<DirectX::Hot3dxDrawnObject>                             m_shapeDrawnObject;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_shapeDrawnObjectTex;
		std::shared_ptr<DirectX::ResourceUploadBatch>                           mesourceUploadDrawnObject;
		std::unique_ptr<DirectX::GraphicsMemory>                                m_graphicsMemoryDrawnObject;

		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_texture1;
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_texture2;
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_textureGridPic;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_DrawnMeshTexture1;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_DrawnMeshTexture2;
		Platform::String^ m_textureImage1File = ref new Platform::String();
		Platform::String^ m_textureImage2File = ref new Platform::String();
		Platform::String^ m_textureImageGridPicFile = ref new Platform::String();
		
		Platform::String^ m_selTexFrontPath;
		Platform::String^ m_selTexBackPath;

		// Solution for Win32 warning is to use below instead of XMMATRIX
		//DirectX::SimpleMath::Matrix
		DirectX::XMFLOAT4X4                                                        m_world4x4;
		//DirectX::SimpleMath::Matrix  
		DirectX::XMFLOAT4X4                                                        m_view4x4;
		//DirectX::SimpleMath::Matrix   
		DirectX::XMFLOAT4X4 	                                              m_projection4x4;
		XMMATRIX XM_CALLCONV SetXMMatrix(DirectX::XMFLOAT4X4 m, XMMATRIX xm);
		XMMATRIX XM_CALLCONV GetXMMatrix(DirectX::XMFLOAT4X4 m);

		// Descriptors
		enum class Descriptors
		{
			WindowsLogo,
			SeaFloor,
			SegoeFont,
			FrontTexture,
			BackTexture,
			GridPicTexture,
			DrawnObjectTexture1,
			DrawnObjectTexture2,
			Count = 256
		} Descriptors;

		// drawing modes
		enum class RotoDrawDrawMode
		{
			DrawSceneParts = 0,
			DrawSelectWithTetras = 1,
			DrawTexturedObject = 2,
			DrawWICTextureObject = 3,
			DrawDualTextureObject = 4,
			DrawWICDualTextureObject = 5,
			DrawPointColorObject = 6,
			DrawLineOnlyObject = 7
		} RotoDrawDrawMode;

		Audio^ m_audioController;
		MediaReader^ mediaReader;
		// Screen Draw Variables
		Hot3dx::CHot3dxD3D12Geometry^ m_hot3dxGeo = ref new Hot3dx::CHot3dxD3D12Geometry();
		Hot3dx::Hot3dx12Rotate^ m_hot3dxRotate = ref new Hot3dx::Hot3dx12Rotate();
		/*
		struct Tetras
		{
			std::unique_ptr<DirectX::GeometricPrimitive>                   m_shapeTetra;
		};
		*/
		XMFLOAT4                                                           m_vMouse3dPos;
		bool                                                               m_bMouse3dPosDist;
		
		float m_posX;
		float m_posY;
		float m_posZ;

		// Previous or last x,y,z, position used for shift key straight line drawing
		float m_previousPosX;
		float m_previousPosY;
		float m_previousPosZ;
		
		Platform::Array<float>^ posX = ref new Platform::Array<float>(1000);
		Platform::Array<float>^ posY = ref new Platform::Array<float>(1000);
		Platform::Array<float>^ posZ = ref new Platform::Array<float>(1000);

		Platform::Array<float>^ box = ref new Platform::Array<float>(6);
		


		bool m_bAddTopFaces;
		bool m_bAddBottomFaces;
		bool m_bIsYAxis;
		int m_SelIndexOne;
		int m_SelIndexTwo;
		// 3D World Left or Right Hand flag
		bool                                               m_IsLeftHanded;
		bool m_bLButtonDown;
		bool m_bRButtonDown;
		bool m_bMButtonDown;
		bool m_bMouseMove;

		// Camera vars
		Hot3dxCamera^ m_camera;
		float m_fCameraDistance;
		float m_fCameraRotation;
		float m_EyeX;
		float m_EyeY;
		float m_EyeZ;
		float m_LookAtX;
		float m_LookAtY;
		float m_LookAtZ;
		float m_UpX;
		float m_UpY;
		float m_UpZ;

		float m_CamPosX;float m_CamPosx;
		float m_CamPosY;float m_CamPosy;
		float m_CamPosZ;float m_CamPosz;
		float m_CamRotX;float m_CamRotx;
		float m_CamRotY;float m_CamRoty;
		float m_CamRotZ;float m_CamRotz;
		float m_CamTheta[1];float m_CamThetaVal;
		float m_CamOrDirX;float m_CamDirx;
		float m_CamOrDirY;float m_CamDiry;
		float m_CamOrDirZ;float m_CamDirz;
		float m_CamOrUpX;float m_CamUpx;
		float m_CamOrUpY;float m_CamUpy;
		float m_CamOrUpZ;float m_CamUpz;
		bool m_bRotateKeyPressed;
		float m_fPointRadius;
		float m_fPointDrawGroupAngle;

		DirectX::XMFLOAT4                                        m_eye;
		DirectX::XMFLOAT4                                        m_at;
		DirectX::XMFLOAT4                                        m_up;

		Windows::UI::Xaml::Media::SolidColorBrush^ m_brushFrontFaceColorSR;
		Windows::UI::Xaml::Media::SolidColorBrush^ m_brushBackFaceColorSR;
		Windows::UI::Color m_frontColorSR;
		Windows::UI::Color m_backColorSR;

		int m_drawMode;	

		bool m_bDDS_WIC_FLAG1;
		bool m_bDDS_WIC_FLAG2;
		bool m_bDDS_WIC_FLAGGridPic;
		bool m_bDDS_WIC_FLAGGridPicComplete;
	};          
}


