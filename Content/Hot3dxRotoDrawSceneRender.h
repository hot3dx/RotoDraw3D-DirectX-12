#pragma once

#include "DirectXPage.xaml.h"
#include "Common\DeviceResources.h"
#include "Content\ShaderStructures.h"
#include "Common\StepTimer.h"
#include <AlignedNewXaml12.h>
#include <Hot3dxGeometry.h>
#include <RenderTargetStateXaml12.h>
#include <EffectPipelineStateDescriptionXaml12.h>
#include <CommonStatesXaml12.h>
#include <GraphicsMemoryXaml12.h>
#include <DescriptorHeapXaml12.h>
#include <EffectCommonXaml12.h>
#include <DDSTextureLoaderXaml12.h>
#include <VertexTypesXaml12.h>
#include <SimpleMathXaml12.h>
#include <ModelXaml12.h>
#include <PrimitiveBatchXaml12.h>
#include <GeometricPrimitiveXaml12.h>
#include "Content\Hot3dxDrawnObject.h"
#include <SpriteBatchXaml12.h>
#include <SpriteFontXaml12.h>
#include <Hot3dxCamera.h>
#include <PostProcessXaml12.h>
#include <Audio\AudioXaml12.h>
#include <Audio\MediaReaderXaml12.h>
#include <Hot3dxGeometry.h>
#include <Hot3dx12Rotate.h>
#include "Hot3dxRotoDrawVariables.h"
#include <mfapi.h>
#include <mfmediaengine.h>
#include <thread>
#include <chrono>
#include <assert.h>

namespace Hot3dxRotoDraw
{
	// A structure for our custom vertex type
	struct CUSTOMVERTEX
	{

		FLOAT x, y, z, rhw; // The transformed position for the vertex
		DWORD color;        // The vertex color
	};

	// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

	ref class CCameraXYMoveRotation
	{
	internal:
		CCameraXYMoveRotation() {}
		float  m_fCamMove_north;
		float  m_fCamMove_northeast;
		float  m_fCamMove_east;
		float  m_fCamMove_southeast;
		float  m_fCamMove_south;
		float  m_fCamMove_southwest;
		float  m_fCamMove_west;
		float  m_fCamMove_northwest;
		float  m_fCamMove_anypointcamera;
		float  m_fCamMove_centerofsphere;
		float  m_fCamMove_camerapoint;
		float  m_fCamMove_cameradirection;
		float  m_fCamMove_camerarotation;
		float  m_fCamMove_cameraradius;
		float  m_fCamMove_gridcenter;
		float  m_fCamMove_degreeradian;
		float  m_fCamMove_anglerotation;
		bool    m_bArrayInit;
	public:



		virtual	~CCameraXYMoveRotation();
	private:

		float* m_fCamMove_px;
		float* m_fCamMove_py;
		float* m_fCamMove_pz;
		void CalculateSphereCV(int* n, DWORD color);
		//float  DegreesToRadians(float degree);
		//void   InitSphereVars(void);
		//void   CalculateSphere(int* count);
		//void   MoveRotateCameraXY(int direction);
		//void   MoveRotateCameraAny(float x, float y, float z);

	};

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

	//-------------------------------------------------------------------------------------
	class IMFNotify
	{
	public:
		virtual ~IMFNotify() = default;

		IMFNotify(const IMFNotify&) = delete;
		IMFNotify& operator=(const IMFNotify&) = delete;

		IMFNotify(IMFNotify&&) = delete;
		IMFNotify& operator=(IMFNotify&&) = delete;

		virtual void OnMediaEngineEvent(uint32_t meEvent) = 0;

	protected:
		IMFNotify() = default;
	};


	//-------------------------------------------------------------------------------------
	class MediaEnginePlayer : public IMFNotify
	{
	public:
		MediaEnginePlayer(const std::shared_ptr<DX::DeviceResources>& deviceResources) noexcept;
		~MediaEnginePlayer();

		MediaEnginePlayer(const MediaEnginePlayer&) = delete;
		MediaEnginePlayer& operator=(const MediaEnginePlayer&) = delete;

		MediaEnginePlayer(MediaEnginePlayer&&) = default;
		MediaEnginePlayer& operator=(MediaEnginePlayer&&) = default;

		void Initialize(IDXGIFactory4* dxgiFactory, ID3D12Device* device, DXGI_FORMAT format);
		void Shutdown();

		void Play();
		void SetMuted(bool muted);

		void SetSource(_In_z_ const wchar_t* sourceUri);

		bool TransferFrame(HANDLE textureHandle, MFVideoNormalizedRect rect, RECT rcTarget);

		// Callbacks
		void OnMediaEngineEvent(uint32_t meEvent) override;

		// Properties
		void GetNativeVideoSize(uint32_t& cx, uint32_t& cy);
		bool IsPlaying() const { return m_isPlaying; }
		bool IsInfoReady() const { return m_isInfoReady; }
		bool IsFinished() const { return m_isFinished; }

	private:

		Microsoft::WRL::ComPtr<ID3D11Device1>       m_device;
		std::shared_ptr<DX::DeviceResources>          m_deviceResources;
		Microsoft::WRL::ComPtr<IMFMediaEngine>      m_mediaEngine;
		Microsoft::WRL::ComPtr<IMFMediaEngineEx>    m_engineEx;

		MFARGB  m_bkgColor;

		bool m_isPlaying;
		bool m_isInfoReady;
		bool m_isFinished;
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
		bool RenderPBR(DirectX::XMMATRIX localDrawnObject);
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
		void XM_CALLCONV DrawPointsSphere(XMFLOAT3 cursorPos);
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
		void XM_CALLCONV DrawSprites(ID3D12GraphicsCommandList* commandList);
		
		unsigned int m_iCullNoneToWireframe;
		unsigned int m_iSamplIndexWrap;
		unsigned int m_iEffectIndex;
		void SetCullNoneToWireframe(unsigned int descId) { m_iCullNoneToWireframe = descId; }
		void SetSamplIndexWrap(unsigned int descId) { m_iSamplIndexWrap = descId; }
		unsigned int GetEffectIndex() { return m_iEffectIndex; }
		void SetEffectIndex(unsigned int descId) { m_iEffectIndex = descId;
		 GetModelEffectDescp12(descId);
		}
			
		static const D3D12_RASTERIZER_DESC GetCommonStatesRasterizerDescp12(unsigned int descId)
		{
			switch (descId)
			{
			case 0:
				return CommonStates::CullNone;
			case 1:
				return CommonStates::CullClockwise;
			case 2:
				return CommonStates::CullCounterClockwise;
			case 3:
				return CommonStates::Wireframe;
			default:
				return CommonStates::CullNone;
			}
		}

		D3D12_GPU_DESCRIPTOR_HANDLE GetWrapType(DirectX::DXTKXAML12::CommonStates* States, unsigned int descId)
		{
			switch (descId)
			{
			case 0:
				return States->AnisotropicWrap();
			case 1:
				return States->AnisotropicClamp();
			case 2:
				return States->PointWrap();
			case 3:
				return States->PointClamp();
			case 4:
				return States->LinearWrap();
			case 5:
				return States->LinearClamp();
			default:
				return States->AnisotropicWrap();
			}
		} 

		void GetModelEffectDescp12(unsigned int descId)
		{
			switch (descId)
			{
			case 0:
			{
				m_bIsBasicModel = true;
				m_bIsDualTextureModel = false;
				m_bIsPBRModel = false;
				m_bIsVideoTextureModel = false;
			}break;
			case 1:
			{
				m_bIsDualTextureModel = true;
				m_bIsBasicModel = false;
				m_bIsPBRModel = false;
				m_bIsVideoTextureModel = false;
			}break;
			case 2:
			{
				m_bIsPBRModel = true;
				m_bIsDualTextureModel = false;
				m_bIsBasicModel = false;
				m_bIsVideoTextureModel = false;
			}break;
			default:
			{
				m_bIsVideoTextureModel = true;
				m_bIsBasicModel = false;
				m_bIsDualTextureModel = false;
				m_bIsPBRModel = false;
			}break;
			}
		}
		void XM_CALLCONV InitDrawnObjectSingleTexture();
		void XM_CALLCONV InitDrawnObjectPBRSingleTexture();
		void XM_CALLCONV InitDrawnObjectDualTexture();
		void XM_CALLCONV InitDrawnObjectVideoTexture();
		Platform::String^ m_hot3dxDirPath = ref new Platform::String();
		void CameraReset();
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
		void XM_CALLCONV SetPoints(); // RotoDraw3D old SetPointsButton Function
		unsigned int GetGroupCount() { return m_iGroupCount; }
		std::vector<Hot3dxRotoDraw::PtGroups^> GetPtGroupList() { return m_PtGroupList; }
		Platform::Array<uint16_t>^ GetPtGroupListList() { return m_PtGroupList.at(0)->GetPtList(); }
		std::vector<DirectX::DXTKXAML12::VertexPositionColor> GetVertices() { return vertices; }
		std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> GetVertexes() { return vertexes; }
		std::vector<uint16_t> GetIndices() { return indices; }
		std::vector<float> GetTextureU() { return textureU; }
		std::vector<float> GetTextureV() { return textureV; }

		//void XM_CALLCONV SetLinePoints(unsigned int lineId);
		void XM_CALLCONV CalculateLinePointsTranslate(float xMove, float yMove, float zMove);
		void XM_CALLCONV CalculateAllLinesPointsTranslate(float xMove, float yMove, float zMove);
		//void XM_CALLCONV CalculateLinePointsRotations(float xAngle, float yAngle, float zAngle);
		//void XM_CALLCONV CalculateAllLinesPointsRotations(float xAngle, float yAngle, float zAngle);
		
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
		void RotatePitchSquid(float degree);
		void RotateYawSquid(float degree);

		bool GetRotateKeyPressed() { return m_bRotateKeyPressed; }
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
		void IncrementPtGroups() {
			PtGroups^ ptGroups = ref new PtGroups();
			m_PtGroupList.push_back(ptGroups);
		}

		uint16_t XM_CALLCONV DrawObjectPoints(uint16_t n);
		void XM_CALLCONV DrawObjectPointsTop();
		void XM_CALLCONV DrawObjectPointsBottom();
		void XM_CALLCONV DrawObjectPointsTopBottom();

		// Sets Width and height drawing mouse ratios
		void SetMouseWidthRatio(float w) { m_drawMouseWidthRatio = m_drawMouseWidthRatio + w; }
		void SetMouseHeightRatio(float h) { m_drawMouseHeightRatio = m_drawMouseHeightRatio + h; }
		float GetMouseWidthRatio() { return m_drawMouseWidthRatio; }
		float GetMouseHeightRatio() { return m_drawMouseHeightRatio; }
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
			Platform::String^ textureFilename);
		// .obj file writer
		Platform::String^ DrawnObjectSaveObjFile(
			Platform::String^ mtlObjFilename,
			Platform::String^ nodeName,
			Platform::String^ effectName);

		// .txt and .hbin file writers
		Platform::String^ DrawnObjectSaveText(Platform::String^ fileName, unsigned int objectCount);
		Platform::String^ DrawnObjectSaveBinary();
		void SetInitSphereVB2(float m_camradius, float m_camrotation) { InitSphereVB2(m_camradius, m_camrotation); }

		// Texture Filename Accessors
		void Setm_bDDS_WIC_FLAG1(bool flag) { m_bDDS_WIC_FLAG1 = flag; }
		void Setm_bDDS_WIC_FLAG2(bool flag) { m_bDDS_WIC_FLAG2 = flag; }
		void SetDDS_WIC_FLAGGridPic(bool flag) { m_bDDS_WIC_FLAGGridPic = flag; }
		Platform::String^ GetTextureImage1File() { return m_textureImage1File; }
		void SetTextureImage1File(Platform::String^ fileName) {
			m_textureImage1File = nullptr;
			m_textureImage1File = ref new Platform::String(fileName->Data());
		}
		Platform::String^ GetTextureImage2File() { return m_textureImage2File; }
		void SetTextureImage2File(Platform::String^ fileName) {
			m_textureImage2File = nullptr;
			m_textureImage2File = ref new Platform::String(fileName->Data());
		}
		Platform::String^ GetTextureImage3File() { return m_textureImage3File; }
		void SetTextureImage3File(Platform::String^ fileName) {
			m_textureImage3File = nullptr;
			m_textureImage3File = ref new Platform::String(fileName->Data());
		}
		Platform::String^ GetTextureImage4File() { return m_textureImage4File; }
		void SetTextureImage4File(Platform::String^ fileName) {
			m_textureImage4File = nullptr;
			m_textureImage4File = ref new Platform::String(fileName->Data());
		}
		Platform::String^ GetTextureImage5File() { return m_textureImage5File; }
		void SetTextureImage5File(Platform::String^ fileName) {
			m_textureImage5File = nullptr;
			m_textureImage5File = ref new Platform::String(fileName->Data());
		}
		Platform::String^ GetTextureImage6File() { return m_textureImage6File; }
		void SetTextureImage6File(Platform::String^ fileName) {
			m_textureImage6File = nullptr;
			m_textureImage6File = ref new Platform::String(fileName->Data());
		}
		Platform::String^ GetTexture1Name() { return m_texture1Name; };
		Platform::String^ GetTexture2Name() { return m_texture2Name; };
		Platform::String^ GetTexture3Name() { return m_texture3Name; };
		Platform::String^ GetTexture4Name() { return m_texture4Name; };
		Platform::String^ GetTexture5Name() { return m_texture5Name; };
		Platform::String^ GetTexture6Name() { return m_texture6Name; };
		void SetTexture1Name(Platform::String^ name) {
			m_texture1Name = nullptr; m_texture1Name = ref new Platform::String(name->Data());
		};
		void SetTexture2Name(Platform::String^ name) {
			m_texture2Name = nullptr; m_texture2Name = ref new Platform::String(name->Data());
		};
		void SetTexture3Name(Platform::String^ name) {
			m_texture3Name = nullptr; m_texture3Name = ref new Platform::String(name->Data());
		};
		void SetTexture4Name(Platform::String^ name) {
			m_texture4Name = nullptr; m_texture4Name = ref new Platform::String(name->Data());
		};
		void SetTexture5Name(Platform::String^ name) {
			m_texture5Name = nullptr; m_texture5Name = ref new Platform::String(name->Data());
		};
		void SetTexture6Name(Platform::String^ name) {
			m_texture6Name = nullptr; m_texture6Name = ref new Platform::String(name->Data());
		};

		// Scenario7_Video Accessors
		Platform::String^ GetTextureImageVideoFile() { return m_textureImageVideoFile; }
		void SetTextureImageVideoFile(Platform::String^ fileName) {
			m_textureImageVideoFile = nullptr;
			m_textureImageVideoFile = ref new Platform::String(fileName->Data());
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
		void XM_CALLCONV CalculateSphereFaces();
		void XM_CALLCONV EndpointTopLeftFaces();
		void XM_CALLCONV EndpointBottomRightFaces();
		void XM_CALLCONV EndpointSphereTopLeftFaces(unsigned int secondGroupPoints);
		void XM_CALLCONV EndpointSphereBottomRightFaces(unsigned int secondFromEndGroupPoints);
		void XM_CALLCONV MidSphereLesserLatitudeTopLeftFaces(unsigned int groupNum, unsigned int firstGroupPoints, unsigned int secondGroupPoints);
		void XM_CALLCONV MidSphereGreaterLatitudeTopLeftFaces(unsigned int groupNum, unsigned int firstGroupPoints, unsigned int secondGroupPoints);
		

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
		XMFLOAT2 point; XMFLOAT2 pointC;
		Platform::Array<unsigned int>^ m_iTempGroup = ref new Platform::Array<unsigned int>(10000);
		Platform::Array<float>^ m_iTempMouseX = ref new Platform::Array<float>(10000);
		Platform::Array<float>^ m_iTempMouseY = ref new Platform::Array<float>(10000);

		std::vector<Hot3dxRotoDraw::PtGroups^> m_PtGroupList;// = ref new Platform::Array<PtGroups>(360);
		std::vector<DirectX::DXTKXAML12::VertexPositionColor> vertices;
		std::vector<DirectX::DXTKXAML12::VertexPositionNormalTexture> vertexes;
		std::vector<DirectX::DXTKXAML12::VertexPositionDualTexture> verticesDual;
		std::vector<Hot3dxRotoDraw::VertexPositionNormalTextureTangent> verticesPBR;
		//std::vector<DirectX::DXTKXAML12::VertexPositionNormalColorTexture> verticesPBR;
		std::vector<uint16_t> indices;
		std::vector<float> textureU;
		std::vector<float> textureV;
		//unsigned int m_iTextureUVCount;

		Platform::Array<unsigned int>^ m_vSphereGroupsSizes = ref new Platform::Array<unsigned int>(1000);
		size_t faceCnt = 0;
		size_t faceIndiceCnt = 0;

		unsigned int m_iTempGroupCount;
		unsigned int m_iGroupCount;
		std::vector<XMFLOAT3> pos;

		float m_fScrollDist;
		float m_fPointSpace;
		int m_iScrollPointSetPos;
		float m_fScale1stLineDrawnPts;
		float distX;
		float distY;
		float distZ;

		float Setrotx;
		float Setroty;
		float Setrotz;

		float m_drawMouseWidthRatio;
		float m_drawMouseHeightRatio;
		float m_cameraDistanceRatio;
		Windows::Foundation::Size  m_outputSize;

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
		std::shared_ptr<DirectX::DXTKXAML12::ResourceUploadBatch>                           m_resourceUpload;
		std::unique_ptr<DirectX::DXTKXAML12::GraphicsMemory>                                m_graphicsMemory;
		std::unique_ptr<DirectX::DXTKXAML12::DescriptorHeap>                                m_resourceDescriptors;
		std::unique_ptr<DirectX::DXTKXAML12::CommonStates>                                  m_states;
		std::unique_ptr<DirectX::DXTKXAML12::BasicEffect>                                   m_lineEffect;
		std::unique_ptr<DirectX::DXTKXAML12::BasicEffect>                                   m_cursorEffect;
		std::unique_ptr<DirectX::DXTKXAML12::PrimitiveBatch<DirectX::DXTKXAML12::VertexPositionColor>>  m_batch;
		std::unique_ptr<DirectX::DXTKXAML12::BasicEffect>                                   m_shapeEffect;
		std::unique_ptr<DirectX::DXTKXAML12::BasicEffect>                                   m_artistCameraEffect;
		std::unique_ptr<DirectX::DXTKXAML12::BasicEffect>                                   m_shapeTetraEffect;
		std::unique_ptr<DirectX::DXTKXAML12::BasicEffect>                                   m_drawRectangleEffect;
		std::unique_ptr<DirectX::DXTKXAML12::Model>                                         m_model;
		std::vector<std::shared_ptr<DirectX::DXTKXAML12::IEffect>>                          m_modelEffects;
		std::unique_ptr<DirectX::DXTKXAML12::EffectTextureFactory>                          m_modelResources;
		std::unique_ptr<DirectX::DXTKXAML12::GeometricPrimitive>                            m_shape;
		std::unique_ptr<DirectX::DXTKXAML12::GeometricPrimitive>                            m_artistCamera;
		std::unique_ptr<DirectX::DXTKXAML12::GeometricPrimitive>                            m_shapeTetra;
		std::unique_ptr<DirectX::DXTKXAML12::GeometricPrimitive>                            m_shapeGridPic;
		std::unique_ptr<Hot3dxDrawnObject>                                                  m_hot3dxDrawnObject;
		std::unique_ptr<DirectX::DXTKXAML12::SpriteBatch>                                   m_sprites;
		std::unique_ptr<DirectX::DXTKXAML12::SpriteBatch>                                   m_batchOpaque;
		std::unique_ptr<DirectX::SpriteFont>                                    m_CameraEyeFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_CameraAtFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_CameraUpFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_MousePosFont;
		//std::unique_ptr<DirectX::SpriteFont>                                    m_CursorPosFont;// Windows cursor
		std::unique_ptr<DirectX::SpriteFont>                                    m_PointCountFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_TotalPointCountFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_FaceCountFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_GroupCountFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_SelectedPointNumberFont;
		std::unique_ptr<DirectX::DXTKXAML12::DualTextureEffect>                             m_dualTextureEffect;

		std::unique_ptr<DirectX::DXTKXAML12::BasicEffect>                                   m_shapeDrawnObjectEffect;
		std::unique_ptr<DirectX::DXTKXAML12::AlphaTestEffect>                               m_shapeDrawnObjectAlphaEffect;
		std::unique_ptr<DirectX::DXTKXAML12::PrimitiveBatch<DirectX::DXTKXAML12::VertexPositionColor>> m_shapeDrawnObject;
		std::unique_ptr<DirectX::DXTKXAML12::GeometricPrimitive>                            m_shapeDrawnObjectTex;
		std::unique_ptr<DirectX::DXTKXAML12::PrimitiveBatch<Hot3dxRotoDraw::VertexPositionNormalTextureTangent>> m_shapeDrawnObjectPBR;
		std::shared_ptr<DirectX::DXTKXAML12::ResourceUploadBatch>                           mesourceUploadDrawnObject;
		std::unique_ptr<DirectX::DXTKXAML12::GraphicsMemory>                                m_graphicsMemoryDrawnObject;

		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_texture1;
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_texture2;
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_textureGridPic;

		// Video texture
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_videoTexture;
		HANDLE                                                                  m_sharedVideoTexture;

		std::unique_ptr<Hot3dxRotoDraw::MediaEnginePlayer>          m_player;

		uint32_t    m_videoWidth;
		uint32_t    m_videoHeight;

		//Our IBL cubemaps
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_radianceIBL;
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_irradianceIBL;
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_NormalTexture;
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_RMATexture;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_DrawnMeshTexture1;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_DrawnMeshTexture2;
		Platform::String^ m_textureImage1File = ref new Platform::String();
		Platform::String^ m_textureImage2File = ref new Platform::String();
		Platform::String^ m_textureImage3File = ref new Platform::String();
		Platform::String^ m_textureImage4File = ref new Platform::String();
		Platform::String^ m_textureImage5File = ref new Platform::String();
		Platform::String^ m_textureImage6File = ref new Platform::String();
		Platform::String^ m_texture1Name = ref new Platform::String();
		Platform::String^ m_texture2Name = ref new Platform::String();
		Platform::String^ m_texture3Name = ref new Platform::String();
		Platform::String^ m_texture4Name = ref new Platform::String();
		Platform::String^ m_texture5Name = ref new Platform::String();
		Platform::String^ m_texture6Name = ref new Platform::String();
		Platform::String^ m_textureImageGridPicFile = ref new Platform::String();
		Platform::String^ m_textureImageVideoFile = ref new Platform::String();
		Platform::String^ m_sound1File = ref new Platform::String();
		
		// TextureFile Loader
		void XM_CALLCONV LoadDDSOrWicTextureFile(_In_ ID3D12Device* device,
			DirectX::DXTKXAML12::ResourceUploadBatch& resourceUpload,
			_In_z_ const wchar_t* szFileName,
			_Outptr_ ID3D12Resource** texture, Platform::String^ msgType, Platform::String^ message);

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
			DrawnObjectNormalTexture,
			DrawnObjectRMATexture,
			DrawnObjectRadiance,
			DrawnObjectIrradiance,
			VideoTexture,
			Reserve,
			Count = 256
		} Descriptors;

		//  PBR Descriptors
		enum class PBRDescriptors
		{
			PicTex = 0,
			NormalTex=1,
			RMATex=2,
			RadianceTex=3,
			IrradianceTex=4,
			Reserve=5
		} PBRDescriptors;

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
			DrawLineOnlyObject = 7,
			DrawLineOnlyRadius = 8,
			SelectPoint = 9,
			SelectFace = 10,
			SelectEdge = 11,
			SelectObject = 12
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

		Platform::Array<float>^ posX = ref new Platform::Array<float>(10000);
		Platform::Array<float>^ posY = ref new Platform::Array<float>(10000);
		Platform::Array<float>^ posZ = ref new Platform::Array<float>(10000);

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

		float m_CamPosX; float m_CamPosx;
		float m_CamPosY; float m_CamPosy;
		float m_CamPosZ; float m_CamPosz;
		float m_CamRotX; float m_CamRotx;
		float m_CamRotY; float m_CamRoty;
		float m_CamRotZ; float m_CamRotz;
		float m_CamTheta[1]; float m_CamThetaVal;
		float m_CamOrDirX; float m_CamDirx;
		float m_CamOrDirY; float m_CamDiry;
		float m_CamOrDirZ; float m_CamDirz;
		float m_CamOrUpX; float m_CamUpx;
		float m_CamOrUpY; float m_CamUpy;
		float m_CamOrUpZ; float m_CamUpz;
		bool m_bRotateKeyPressed;
		float m_fPointRadius;
		float m_fSpherePointRadius;
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

		// Background Color for the drawing swapchainpanel
		DirectX::XMVECTORF32 m_backGroundColor;

		Platform::String^ msgTypes;
		Platform::String^ GetMsgTypes(unsigned int i);
		Platform::String^ messages;
		Platform::String^ GetMessages(unsigned int i);

		CCameraXYMoveRotation          m_CamXYMoveRotate;

		float m_cameraradius;
		float m_camerarotation;
		float m_anglerotation;
		float  m_fCamMove_cameraradius;
		float  m_fCamMove_degreeradian;
		float  m_fCamMove_anglerotation;
		XMFLOAT3 m_xmfSphereCenterPoint{ 0.0f, 0.0f, 0.0f };
		XMFLOAT3 m_xmfSphereRadiusPoint{ 0.0f, 0.0f, 0.0f };
		void CalculateSphereVPCXAxis(float camradius, float camrotation);
		void CalculateSphereVPCYAxis(float camradius, float camrotation);
		internal:
		void InitSphereVB2(float m_camradius, float m_camrotation);

		bool m_bArrayInit;

		//////////// PBREffect ////////
		enum class MaterialTypesPBR : int {
			DIFFUSE = 0, //V2+3
			SPECULAR = 1, //V3
			NORMAL = 2, //V3
			EMISSIVE = 3 //V3
		};

		
			void SetShouldRender(bool _shouldRender) { m_shouldRender = _shouldRender; };

			bool IsLoaded() { return m_loaded; };

			//These functions are intended for skybox use
			void DisableDepthDefault() { enable_depth_default = false; }
			//void AlbedoEmissiveOverride(std::wstring path);
			//---

			
			//Engine features
			//GameFilepaths m_filepath;

			//Settings and data
			Platform::String^ dirpath_wchar = ref new Platform::String(L"Assets\\Textures\\");
			Platform::String^ filename;
			int resourceDescriptorOffset = 0;
			int meshTexture_index = -1;
			int radiance_index = -1;
			int irradiance_index = -1;
			bool is_debug_mesh = false;
			bool m_shouldRender = true;
			bool m_loaded = false;
			int anim_config_version = -1;
			bool is_metallic = false;
			bool m_bIsPBRModel = false;
			bool m_bIsBasicModel = false;
			bool m_bIsDualTextureModel = false;
			bool m_bIsVideoTextureModel = false;
			bool m_bIsPlayer = false;

			protected private:
				std::unique_ptr<DirectX::DXTKXAML12::PBREffect>   m_shapeDrawnObjectPBREffect;
				//Model resources
				
				std::vector<std::shared_ptr<DirectX::DXTKXAML12::IEffect>>  m_modelNormal;
				//std::unique_ptr<DirectX::DXTKXAML12::DescriptorPile>        m_resDescPile;
				std::unique_ptr<DirectX::DXTKXAML12::DescriptorHeap>        m_rtvHeap;
				std::unique_ptr<DirectX::DXTKXAML12::ToneMapPostProcess>    m_toneMap;
				std::unique_ptr<DirectX::DXTKXAML12::ToneMapPostProcess>    m_HDR10;
				Microsoft::WRL::ComPtr<ID3D12Resource> m_materialOverride;

				int material_override_index = -1;
				bool material_override_applied = false;

				//Should use DepthDefault? (available to change mainly b/c of skyboxes)
				bool enable_depth_default = true;


				
				/* A struct to define a single animated map - we use this multiple times per material (e.g. animated diffuse, animated specular, etc) */
				struct AnimatedMap {
					bool is_animated = false;
					float animation_time = 0.0f;
					std::vector<std::string> texture_names;
					std::vector<int> gpu_indexes;
					std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> d3d12_textures;
					int current_anim_index = 0;

					float animation_timer = 0.0f;
				};

				/* A struct to hold a material's configurations as defined by the ThiCC toolkit. */
				struct MaterialConfig {
					int material_index = -1;

					bool is_metallic = false;

					AnimatedMap animated_diffuse;  //Supported by V2 and V3
					AnimatedMap animated_specular; //Supported by V3
					AnimatedMap animated_normal;   //Supported by V3
					AnimatedMap animated_emissive; //Supported by V3
				};

				//Material config
				std::vector<MaterialConfig> m_material_config;
				/*
				// Generate paths for resources
				enum class Textures
				{
					Albedo = 0,
					Normal = 1,
					RMA = 2,
					Radiance = 3,
					Irradiance = 4,
					MaxTextures = 5
				} Textures;

				Microsoft::WRL::ComPtr<ID3D12Resource>  m_textureResources[5];
				*/

			////////////// EO PBREffect
		private:
			
			

			// Direct3D resources for cube geometry.
			Microsoft::WRL::ComPtr<ID3D12RootSignature>			m_rootSignature;
			Microsoft::WRL::ComPtr<ID3D12PipelineState>			m_pipelineState;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_cbvHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource>				m_vertexBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource>				m_indexBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource>				m_constantBuffer;
			
			//D3D12_RECT											m_scissorRect;
			std::vector<BYTE>									m_vertexShader;
			std::vector<BYTE>									m_pixelShader;
			std::vector<BYTE>									m_vertexShaderPBR;
			std::vector<BYTE>									m_pixelShaderPBR;
			D3D12_VERTEX_BUFFER_VIEW							m_vertexBufferView;
			D3D12_INDEX_BUFFER_VIEW								m_indexBufferView;
	};
}


