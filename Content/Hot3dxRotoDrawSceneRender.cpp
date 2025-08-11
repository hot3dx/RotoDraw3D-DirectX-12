//--------------------------------------------------------------------------------------
// File: Hot3dxRotoDrawSceneRender.cpp
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Hot3dxRotoDrawSceneRender.h"
#include "Common\DirectXHelper.h"
#include <ppltasks.h>
#include <synchapi.h>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <system_error>
#include <thread>
#include <utility>
#include "DirectXPage.xaml.h"
#include "Scenario2_Normal.xaml.h"
#include <strsafe.h>
#include <RenderTargetStateXaml12.h>
#include <EffectPipelineStateDescriptionXaml12.h>
#include <CommonStatesXaml12.h>
#include <GraphicsMemoryXaml12.h>
#include <VertexTypesXaml12.h>
#include <MyResourceUploadBatchXaml12.h>
#include <GeometricPrimitiveXaml12.h>
#include <GeometryXaml12.h>
#include <EffectsXaml12.h>
#include <EffectCommonXaml12.h>
#include <WICTextureLoaderXaml12.h>
#include "FilesSave\ObjMaterialFile.h"
#include "FilesSave\Obj3DFile.h"
#include "FilesSave\Hot3dxTextFileSave.h"
#include <DescriptorHeapXaml12.h>

#include <stdlib.h>


#include "Hot3dxObjectGeometry.h"

using namespace Hot3dxRotoDraw;
using namespace Hot3dx;

using namespace DX;
using namespace Concurrency;
using namespace DirectX;
using namespace DirectX::DXTKXAML12;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::System::Threading;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;


// Indices into the application state map.
Platform::String^ AngleKey = "Angle";
Platform::String^ TrackingKey = "Tracking";

Hot3dxRotoDraw::PtGroups::PtGroups(int ptCount) : m_ptlistCount(ptCount) {}

Hot3dxRotoDraw::LinePtGrp::LinePtGrp(unsigned int arraySize) :
	m_uiArraySize(arraySize),
	m_ptCount(0),
	position{ 0.0f,0.0f,0.0f },
	color{ 0.0f,0.0f,0.0f,1.0f },
	m_fTranslate{ 0.0f, 0.0f, 0.0f },
	m_fRotate{ 0.0f, 0.0f, 0.0f },
	m_fScale{ 0.0f,0.0f,0.0f },
	m_fColor{ 0.0f,0.0f,0.0f,1.0f }

{
	m_PtList = ref new Platform::Array<uint16_t>(m_uiArraySize);
	m_PtIndexList = ref new Platform::Array<uint16_t>(m_uiArraySize);
	posX = ref new Platform::Array<float>(m_uiArraySize);
	posY = ref new Platform::Array<float>(m_uiArraySize);
	posZ = ref new Platform::Array<float>(m_uiArraySize);

	m_lineVerts.clear();
}

Platform::Array<uint16_t>^ Hot3dxRotoDraw::LinePtGrp::GetPtList()
{
	return m_PtList;
}

void Hot3dxRotoDraw::LinePtGrp::IncrementPtCnt() { m_ptCount++; }
unsigned int Hot3dxRotoDraw::LinePtGrp::GetLinePtCnt() const { return m_ptCount; }
void Hot3dxRotoDraw::LinePtGrp::SetLinePtCnt(unsigned int count) { m_ptCount = count; }
//void Hot3dxRotoDraw::LinePtGrp::SetPtList(unsigned int i, uint16_t val) { m_PtList->set(i, val); }
uint16_t Hot3dxRotoDraw::LinePtGrp::GetListPt(unsigned int i) { return m_PtList->get(i); }
void Hot3dxRotoDraw::LinePtGrp::ZeroOutPtList()
{
	m_ptCount = 0;
	m_PtList->~Array();
	posX->~Array();
	posY->~Array();
	posZ->~Array();
}

Platform::Array<uint16_t>^ Hot3dxRotoDraw::LinePtGrp::GetPtIndexList()
{
	return m_PtIndexList;
}

uint16_t Hot3dxRotoDraw::LinePtGrp::GetListPtIndex(unsigned int i) { return m_PtIndexList->get(i); }

void Hot3dxRotoDraw::LinePtGrp::ZeroOutPtIndexList()
{
	m_ptCount = 0;
	m_PtIndexList->~Array();
	posX->~Array();
	posY->~Array();
	posZ->~Array();
}

void Hot3dxRotoDraw::LinePtGrp::SetLinePositions(unsigned int i, float x, float y, float z)
{
	posX->set(i, x); posY->set(i, y); posZ->set(i, z);
}
// Loads vertex and pixel shaders from files and instantiates the cube geometry.
RotoDrawSceneRender::RotoDrawSceneRender(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_iEndPointOneCount(0),
	m_iEndPointTwoCount(0),
	m_vars(),
	m_iCullNoneToWireframe(0),
	m_iSamplIndexWrap(0),
	m_iPointCount(0),
	m_iTotalPointCount(0),
	m_iGroupCount(0),
	m_fScrollDist(0.300000f),
	m_fPointSpace(0.30000f),
	m_iScrollPointSetPos(150),
	//m_drawMouseWidthRatio(0.033000f),// fullscreen(0.0254000f),
	//m_drawMouseHeightRatio(0.038100f),// fullscreen(0.0254000f),(0.039000f),
	m_drawMouseWidthRatio(0.0212f),//(0.033000f),// fullscreen(0.0254000f),
	m_drawMouseHeightRatio(0.02641f),//(0.038100f),// fullscreen(0.0254000f),(0.039000f),
	m_sceneDeviceResources(deviceResources),
	m_loadingComplete(false),
	m_loadingDrawnObjectComplete(false),
	m_performScaleDrawnObjectComplete(false),
	m_fScale1stLineDrawnPts(1.33f), // compensator for line shrinkage of trig rotation function vector shrinkage
	m_radiansPerSecond(XM_PIDIV4 / 2),	// rotate 45 degrees per second
	m_angle(0.01f),
	m_tracking(false),
	sceneVertexCount(8),
	m_textureImage1File(),//L"Assets\\Textures\\fire.dds"),
	m_textureImage2File(),//L"Assets\\Textures\\sphere.png"),
	m_textureImage3File(),//L"Assets\\Textures\\Toyrobot_RMA.dds"),
	m_textureImage4File(),//L"Assets\\Textures\\toyrobotr_Normal.dds"),
	m_textureImage5File(),//L"Assets\\Textures\\ATRIUM_R.DDS.dds"),
	m_textureImage6File(),//L"Assets\\Textures\\ATRIUM_IR.DDS.dds"),
	m_textureImagePic1File(),//L"Assets\\Textures\\tree01S.dds"),
	m_textureImageVideo2File(),//L"Assets\\Textures\\SampleVideo.mp4"),
	m_bMouse3dPosDist(false),
	m_posX(0.0f), m_posY(0.0f), m_posZ(0.0f),
	m_previousPosX(0.0f),
	m_previousPosY(0.0f),
	m_previousPosZ(0.0f),
	m_bIsYAxis(true),
	m_SelIndexOne(0),
	m_IsLeftHanded(true),
	m_fCameraDistance(20.0f),
	m_EyeX(0.0f), m_EyeY(0.0f), m_EyeZ(m_fCameraDistance),
	m_LookAtX(0.0f), m_LookAtY(0.01f), m_LookAtZ(0.0f),
	m_UpX(0.0f), m_UpY(1.0f), m_UpZ(0.0f),
	m_bRotateKeyPressed(false),
	m_drawMode((int)RotoDrawDrawMode::DrawLineOnlyObject),//0
	m_bIsPBRModel(false),
	m_bIsBasicModel(true),
	m_bIsBasicModelColor(false),
	m_bIsDualTextureModel(false),
	m_bIsVideoTextureModel(false),
	m_bIsSculptWireframe(false),
	m_bIsPlayer(false),
	m_sharedVideoTexture(nullptr),
	m_videoWidth(0),
	m_videoHeight(0),
	m_bDDS_WIC_FLAG1(true),
	m_bDDS_WIC_FLAG2(true),
	m_bDDS_WIC_FLAGGridPic(false),
	m_bDDS_WIC_FLAGGridPicComplete(false),
	m_backGroundColor(DirectX::Colors::Black)

{
	if (m_IsLeftHanded) { m_EyeZ = -m_fCameraDistance; }
	
	LoadState();
	ZeroMemory(&m_constantBufferData, sizeof(m_constantBufferData));

	m_camera = ref new Hot3dxCamera();

	m_audioController = ref new Audio;
	m_audioController->CreateDeviceIndependentResources();

	m_vars = ref new Hot3dxRotoDrawVariables();
	m_vars->GetDXPage()->m_Scene5TexVars->SetGridChecked(true);

	SetUVPercentTextureDimension(1.0f);

	
	
	
	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
	
	CreateWindowSizeDependentResources();
	
}

RotoDrawSceneRender::~RotoDrawSceneRender()
{
	m_loadingComplete = false;
	m_loadingDrawnObjectComplete = false;
}

void RotoDrawSceneRender::CreateDeviceDependentResources()
{

	if (m_loadingComplete == false)
	{


		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{

			return;
		}
		m_resourceUpload.reset();
		m_graphicsMemory.reset();
		m_states.reset();
		m_resourceDescriptors.reset();
		m_sprites.reset();
		mediaReader = ref new MediaReader();
		auto targetHitSound = mediaReader->LoadMedia("Assets\\hit.wav");

		m_vars = ref new Hot3dxRotoDrawVariables();
		m_resourceUpload = std::make_unique<DirectX::DXTKXAML12::ResourceUploadBatch>(device);
		m_graphicsMemory = std::make_unique<DirectX::DXTKXAML12::GraphicsMemory>(device);

		m_states = std::make_unique<DirectX::DXTKXAML12::CommonStates>(device);



		m_resourceDescriptors = std::make_unique<DirectX::DXTKXAML12::DescriptorHeap>(device,
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
			static_cast<size_t>(Descriptors::Count));
		
		
			// create heaps
			m_resDescPile = std::make_unique<DirectX::DXTKXAML12::DescriptorPile>(device,
				D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
				D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
				128, // Maximum descriptors for both static and dynamic
				static_cast<size_t>(PBRDescriptors::Reserve));
			m_rtvHeapSceneRenderer = m_sceneDeviceResources->GetRtvHeap();
			// Above translates to:
			RenderTargetState backBufferRts(DXGI_FORMAT_R10G10B10A2_UNORM, DXGI_FORMAT_D32_FLOAT);

			// HDR10 signal
			m_HDR10 = std::make_unique<DirectX::DXTKXAML12::ToneMapPostProcess>(device, backBufferRts,
				DirectX::DXTKXAML12::ToneMapPostProcess::None, DirectX::DXTKXAML12::ToneMapPostProcess::ST2084);

			// Tonemap for SDR signal
			m_toneMap = std::make_unique<DirectX::DXTKXAML12::ToneMapPostProcess>(device, backBufferRts,
				DirectX::DXTKXAML12::ToneMapPostProcess::ACESFilmic, DirectX::DXTKXAML12::ToneMapPostProcess::SRGB);
		

		m_batch = std::make_unique<DirectX::DXTKXAML12::PrimitiveBatch<DirectX::DXTKXAML12::VertexPositionColor>>(device);

		m_shapeTetra = DirectX::DXTKXAML12::GeometricPrimitive::CreateTetrahedron(0.1f);

		m_shapeDrawnObject = std::make_unique<DirectX::DXTKXAML12::PrimitiveBatch<DirectX::DXTKXAML12::VertexPositionColor>>(device);
		
	// SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
		{
			XMVECTORF32 xaxis = { 10.0f, 0.0f, 0.0f };
			XMVECTORF32 yaxis = { 0.0f, 10.0f, 0.0f };
			XMVECTORF32 zaxis = { 0.0f, 0.0f, 0.0f };

			DrawGridInit(xaxis, yaxis, zaxis, 10, 10, Colors::Crimson);

			// Begin Resource Upload
			m_resourceUpload->BeginXaml();
			
			if (m_hot3dxDirPath) {
				m_hot3dxDirPath = m_vars->GetDXPage()->Getm_sDirPathDXP();
				Platform::String^ sfil = ref new Platform::String(m_hot3dxDirPath->Data());//(pwd, LENGTH);
				sfil = sfil->Concat(sfil, L"Assets\\Textures\\seafloor.dds");
				ThrowIfFailedHot(
					DirectX::DXTKXAML12::CreateDDSTextureFromFile(device, *m_resourceUpload, sfil->ToString()->Data(), &m_texture1)
				);

				DirectX::DXTKXAML12::CreateShaderResourceView(device, m_texture1.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SeaFloor)), false);
			}
			
			RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());

			
			// Each effect object must ne proceeded by its own 
			// EffectPipelineStateDescription pd even if the EffectPipelineStateDescription pd is the same
			{
				EffectPipelineStateDescription pdline(
					&DirectX::DXTKXAML12::VertexPositionColor::InputLayout,
					DirectX::DXTKXAML12::CommonStates::Opaque,
					DirectX::DXTKXAML12::CommonStates::DepthNone,
					DirectX::DXTKXAML12::CommonStates::CullNone,
					rtState,
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

				m_lineEffect = std::make_unique<DirectX::DXTKXAML12::BasicEffect>(device, EffectFlags::VertexColor, pdline);
			}

			{
				EffectPipelineStateDescription cd(
					&DirectX::DXTKXAML12::VertexPositionColor::InputLayout,
					DirectX::DXTKXAML12::CommonStates::Opaque,
					DirectX::DXTKXAML12::CommonStates::DepthNone,
					DirectX::DXTKXAML12::CommonStates::CullNone,
					rtState,
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

				m_cursorEffect = std::make_unique<DirectX::DXTKXAML12::BasicEffect>(device, EffectFlags::VertexColor, cd);
			}

			{
				EffectPipelineStateDescription pdsprite(
					&DirectX::DXTKXAML12::GeometricPrimitive::VertexType::InputLayout,
					DirectX::DXTKXAML12::CommonStates::Opaque,
					DirectX::DXTKXAML12::CommonStates::DepthDefault,
					DirectX::DXTKXAML12::CommonStates::CullNone,
					rtState);

				m_shapeTetraEffect = std::make_unique<DirectX::DXTKXAML12::BasicEffect>(device, EffectFlags::PerPixelLighting | EffectFlags::Texture, pdsprite);
				m_shapeTetraEffect->EnableDefaultLighting();
				m_shapeTetraEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SeaFloor)), m_states->AnisotropicWrap());
			}
			
			{
				SpriteBatchPipelineStateDescription pdbo(rtState, &CommonStates::Opaque);

				m_batchOpaque = std::make_unique<SpriteBatch>(device, *m_resourceUpload, pdbo);
			}

			{
				EffectPipelineStateDescription pdspr(
					&GeometricPrimitive::VertexType::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					CommonStates::CullNone,
					rtState);
			}

			{
				SpriteBatchPipelineStateDescription pdspr(rtState);

				m_sprites = std::make_unique<SpriteBatch>(device, *m_resourceUpload, pdspr);
			}
				m_CameraEyeFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont", //L"italic.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));

				m_CameraAtFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont", //L"italic.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));

				m_CameraUpFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont", //L"italic.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));


				m_MousePosFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));

				/*
				m_CursorPosFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont",
					//m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					//m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));

				*/
				m_PointCountFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont", //L"italic.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));

				m_MAX_PointCountFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont", //L"italic.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));

				m_TotalPointCountFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont", //L"italic.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));

				m_FaceCountFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont", //L"italic.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));


				m_GroupCountFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont", //L"italic.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));


				m_SelectedPointNumberFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
					L"Assets/SegoeUI_18.spritefont", //L"italic.spritefont",
					m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SegoeFont)),
					m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SegoeFont)));
			
			

			
			auto loaded = m_resourceUpload->EndXaml(m_sceneDeviceResources->GetCommandQueue());
			WaitForSingleObject(m_resourceUpload->GetGPUHandle(), INFINITE);

			// solution XMMATRIX projection = XMLoadFloat4x4(&m_projection4x4);
			m_shapeTetraEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_lineEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_cursorEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));

			const D3D12_VIEWPORT viewport = m_sceneDeviceResources->GetScreenViewport();

			m_sprites->SetViewport(viewport);

			m_sprites->SetRotation(m_sceneDeviceResources->GetRotation());

			
			m_sceneDeviceResources->WaitForGpu();
			loaded.then([this]()
				{
					m_loadingComplete = true;

				});

		} // eo ! m_loadingComplete = false
	}
}

// Initializes view parameters when the window size changes.
void RotoDrawSceneRender::CreateWindowSizeDependentResources()
{
	m_outputSize = m_sceneDeviceResources->GetOutputSize();
	Size outputSize = m_outputSize;
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = m_sceneDeviceResources->GetScreenViewport();
	m_sceneDeviceResources->SetScissorRect(0, 0,
		static_cast<LONG>(viewport.Width),
		static_cast<LONG>(viewport.Height));

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix;
	if (m_IsLeftHanded)
	{
		perspectiveMatrix = XMMatrixPerspectiveFovLH(
			fovAngleY,
			aspectRatio,
			0.01f,
			1000.0f
		);
	}
	else
	{
		perspectiveMatrix = XMMatrixPerspectiveFovRH(
			fovAngleY,
			aspectRatio,
			0.01f,
			1000.0f
		);
	}

	XMFLOAT4X4 orientation = m_sceneDeviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);
	
	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);

	// m_eye is at (0,0.7,10.0), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	//static const 
	DirectX::XMVECTOR eye,at,up;
	if (m_IsLeftHanded)
	{
		m_eye = { m_EyeX, m_EyeY, m_EyeZ, 0.0f }; // away -(m_fCameraDistance)
		eye = XMVectorSet(m_EyeX, m_EyeY, m_EyeZ, 0.0f);
	}
	else
	{
		m_eye = { m_EyeX, m_EyeY, m_EyeZ, 0.0f }; // away m_fCameraDistance
		eye = XMVectorSet(m_EyeX, m_EyeY, m_EyeZ, 0.0f);
	}

	m_at = { m_LookAtX, m_LookAtY, m_LookAtZ, 0.0f };
		at = XMVectorSet(m_LookAtX, m_LookAtY, m_LookAtZ, 0.0f);
	m_up = { m_UpX, m_UpY, m_UpZ, 0.0f };
	    up = XMVectorSet(m_UpX, m_UpY, m_UpZ, 0.0f);
	XMFLOAT4 Feye = { m_EyeX, m_EyeY, m_EyeZ, 0.0f };
	XMFLOAT4 Fat = { m_LookAtX, m_LookAtY, m_LookAtZ, 0.0f };
	
	if (m_IsLeftHanded)
	{
		XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
	}
	else
		{
			XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
		}
	XMStoreFloat4(&Feye, XMVector4Transform(XMLoadFloat4(&Feye), XMMatrixTranslation(Fat.x, Fat.y, Fat.z)));

	XMStoreFloat4x4(&m_projection4x4, (perspectiveMatrix * orientationMatrix));
	
}    

// Called once per frame, rotates the cube and calculates the model and view matrices.
void RotoDrawSceneRender::Update(StepTimer const& timer)
{
	m_timer = timer;
	if (m_loadingComplete)
	{
		if (!m_tracking)
		{
			// Rotate the cube a small amount.
			m_angle += static_cast<float>(timer.GetElapsedSeconds())* m_radiansPerSecond;

			Rotate(m_angle);
		}

		PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

		m_camera->Eye(XMFLOAT3(m_EyeX, m_EyeY, m_EyeZ));
		m_camera->LookDirection(XMFLOAT3(m_LookAtX, m_LookAtY, m_LookAtZ));
		m_camera->UpDirection(XMFLOAT3(m_UpX, m_UpY, m_UpZ));

		XMVECTOR eye = XMVectorSet(m_EyeX, m_EyeY, m_EyeZ, 0.0f);
		XMVECTOR at = XMVectorSet(m_LookAtX, m_LookAtY, m_LookAtZ, 0.0f);
		XMVECTOR up = XMVectorSet(m_UpX, m_UpY, m_UpZ, 0.0f);

		XMFLOAT4 Feye = { m_EyeX, m_EyeY, m_EyeZ, 0.0f };
		XMFLOAT4 Fat = { m_LookAtX, m_LookAtY, m_LookAtZ, 0.0f };
		XMFLOAT4 Fup = { m_UpX, m_UpY, m_UpZ, 0.0f };
		
		if (m_IsLeftHanded)
		{
			DirectX::XMStoreFloat4x4(&m_constantBufferData.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye, at, up)));
			XMStoreFloat4(&Feye, XMVector4Transform(XMLoadFloat4(&Feye), XMMatrixTranslation(Feye.x, Feye.y, Feye.z)));
			XMMatrixTranslation(Feye.x, Feye.y, Feye.z);
			// store m_view
			XMStoreFloat4x4(&m_view4x4, DirectX::XMMatrixLookAtLH(eye, at, up));
			// store m_world
			if (!m_bRotateKeyPressed)
			{
				XMStoreFloat4x4(&m_world4x4, DirectX::XMMatrixRotationY(float(m_timer.GetTotalSeconds() * 0.0f))); //XM_PIDIV4)));
			}
		}
		else {

			DirectX::XMStoreFloat4x4(&m_constantBufferData.view,
				XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
			XMStoreFloat4(&Feye, XMVector4Transform(XMLoadFloat4(&Feye), XMMatrixTranslation(Feye.x, Feye.y, Feye.z)));
			XMMatrixTranslation(Feye.x, Feye.y, Feye.z);
			// store m_view
			XMStoreFloat4x4(&m_view4x4, DirectX::XMMatrixLookAtRH(eye, at, up));
			// store m_world 
			if (!m_bRotateKeyPressed)
			{
				XMStoreFloat4x4(&m_world4x4, DirectX::XMMatrixRotationY(float(m_timer.GetTotalSeconds() * 0.0f))); //XM_PIDIV4)));
			}
		}

		m_lineEffect->SetView(XMLoadFloat4x4(&m_view4x4));
		m_lineEffect->SetWorld(DirectX::XMMatrixIdentity());

		m_cursorEffect->SetView(XMLoadFloat4x4(&m_view4x4));
		m_cursorEffect->SetWorld(DirectX::XMMatrixIdentity());

		m_shapeTetraEffect->SetView(XMLoadFloat4x4(&m_view4x4));

		
		if (m_bDDS_WIC_FLAGGridPic == true && m_bDDS_WIC_FLAGGridPicComplete == true)
		{
			if (m_drawRectangleEffect)
			{
				m_drawRectangleEffect->SetView(XMLoadFloat4x4(&m_view4x4));
				m_drawRectangleEffect->SetWorld(DirectX::XMMatrixIdentity());
			}
		}

		if (m_loadingDrawnObjectComplete)
		{

			if (m_bIsBasicModel == true || m_bIsVideoTextureModel == true || m_bIsBasicModelColor == true) {
				if (m_shapeDrawnObjectEffect) {
					m_shapeDrawnObjectEffect->SetView(XMLoadFloat4x4(&m_view4x4));
					m_shapeDrawnObjectEffect->SetWorld(DirectX::XMMatrixIdentity());
				}
			}
			else if (m_bIsDualTextureModel == true) {
				if (m_dualTextureEffect) {
					m_dualTextureEffect->SetView(XMLoadFloat4x4(&m_view4x4));
					m_dualTextureEffect->SetWorld(DirectX::XMMatrixIdentity());
				}
			}
			else if (m_bIsPBRModel == true) {
				if (m_shapeDrawnObjectPBREffect) {
					//m_sprites->SetViewport(m_sceneDeviceResources->GetScreenViewport());
					m_shapeDrawnObjectPBREffect->SetView(XMLoadFloat4x4(&m_view4x4));
					m_shapeDrawnObjectPBREffect->SetWorld(DirectX::XMMatrixIdentity());
				}
			}
			if (m_bIsVideoTextureModel == true) {
				if (m_shapeDrawnObjectVideoEffect) {
					m_shapeDrawnObjectVideoEffect->SetView(XMLoadFloat4x4(&m_view4x4));
					m_shapeDrawnObjectVideoEffect->SetWorld(DirectX::XMMatrixIdentity());
				}
			}
		}
		if (m_vars->GetDXPage()->m_Scene5TexVars->GetGridChecked() == true)
		{
			m_sprites->SetViewport(m_sceneDeviceResources->GetScreenViewport());
		}
		
		PIXEndEvent();
		
	}
}

// Saves the current state of the renderer.
void RotoDrawSceneRender::SaveState()
{
	auto state = ApplicationData::Current->LocalSettings->Values;

	if (state->HasKey(AngleKey))
	{
		state->Remove(AngleKey);
	}
	if (state->HasKey(TrackingKey))
	{
		state->Remove(TrackingKey);
	}

	state->Insert(AngleKey, PropertyValue::CreateSingle(m_angle));
	state->Insert(TrackingKey, PropertyValue::CreateBoolean(m_tracking));
}

// Restores the previous state of the renderer.
void RotoDrawSceneRender::LoadState()
{
	auto state = ApplicationData::Current->LocalSettings->Values;
	if (state->HasKey(AngleKey))
	{
		m_angle = safe_cast<IPropertyValue^>(state->Lookup(AngleKey))->GetSingle();
		state->Remove(AngleKey);
	}
	if (state->HasKey(TrackingKey))
	{
		m_tracking = safe_cast<IPropertyValue^>(state->Lookup(TrackingKey))->GetBoolean();
		state->Remove(TrackingKey);
	}
}

// Rotate the 3D cube model a set amount of radians.
void RotoDrawSceneRender::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader.
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void RotoDrawSceneRender::StartTracking()
{
	m_tracking = true;
}

BOOL RotoDrawSceneRender::checkDistance(float x, float y, float z, float mouseMoveDistDelta)
{
	if (m_iPointCount == 0)
	{
		m_bMouse3dPosDist = true;
	}
	else if (m_iPointCount > 0)
	{
		unsigned int i = m_iPointCount - 1;
		float xx = abs(x - posX->get(i));
		float yy = abs(y - posY->get(i));
		float zz = abs(z - posZ->get(i));

		if(xx >= mouseMoveDistDelta || yy >= mouseMoveDistDelta || zz >= mouseMoveDistDelta)
		{
			m_bMouse3dPosDist = true;
		}
		else { 
			m_bMouse3dPosDist = false; 
		}
	}
	return true;
}


// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void RotoDrawSceneRender::TrackingUpdate(float positionX, float positionY)
{
	point.x = positionX;
	point.y = positionY;
	MouseCursorRender(positionX);
	//MouseCursor3dPoint(positionX, positionY);
}

void RotoDrawSceneRender::StopTracking()
{
	m_tracking = false;
}

void Hot3dxRotoDraw::RotoDrawSceneRender::ReleaseDeviceDependentResources()
{
	
}

void Hot3dxRotoDraw::RotoDrawSceneRender::OnDeviceLost()
{
	ReleaseDeviceDependentResources();
	m_bDDS_WIC_FLAGGridPicComplete = false;
	m_bDDS_WIC_FLAGGridPic = false;
	m_loadingComplete = false;
	m_loadingDrawnObjectComplete = false;

	if (m_bIsPBRModel == true)
	{
		m_resDescPile.reset();
	}
	m_bIsBasicModelColor = false;
	m_bIsBasicModel = false;
	m_bIsDualTextureModel = false;
	m_bIsPBRModel = false;
	m_bIsSculptWireframe = false;
	m_bIsVideoTextureModel = false;
		
	m_MousePosFont.reset();
	//m_CursorPosFont.reset();
	m_CameraEyeFont.reset();
	m_CameraAtFont.reset();
	m_CameraUpFont.reset();
	m_PointCountFont.reset();
	m_MAX_PointCountFont.reset();
	m_TotalPointCountFont.reset();
	m_FaceCountFont.reset();
	m_GroupCountFont.reset();
	m_SelectedPointNumberFont.reset();

	m_batch = nullptr;
	m_shape = nullptr;
	m_shapeTetra = nullptr;
	m_shapeGridPic = nullptr;
	m_model.reset();
	
	m_lineEffect = nullptr;

	m_cursorEffect = nullptr;

	m_shapeEffect = nullptr;
	
	m_shapeTetraEffect = nullptr;
	m_drawRectangleEffect = nullptr;
	m_DrawnMeshTexture1 = nullptr;
	m_shapeDrawnObjectEffect = nullptr;
	m_shapeDrawnObjectTex = nullptr;

	m_DrawnMeshTexture2 = nullptr;
	m_dualTextureEffect = nullptr;
	m_hot3dxDrawnObjectDual = nullptr;

	m_PBRTexture1 = nullptr;
	m_PBRTexture2 = nullptr;
	m_RMATexture = nullptr;
	m_NormalTexture = nullptr;
	m_radianceIBL = nullptr;
	m_irradianceIBL = nullptr;
	m_shapeDrawnObjectPBREffect = nullptr;
	m_hot3dxDrawnObjectPBR = nullptr;

	m_videoTexture = nullptr;
	m_shapeDrawnObjectVideoEffect = nullptr;
	m_shapeDrawnObjectVideoTex = nullptr;

	m_shapeDrawnObjectEffectSculpt = nullptr;
	m_shapeDrawnObjectSculpt = nullptr;
	m_shapeDrawnObjectColor = nullptr;
	//m_modelEffects.clear();
	//m_modelResources.reset();
	m_sprites.reset();
	m_resourceDescriptors.reset();
	
	m_states.reset();
	m_graphicsMemory.reset();
	pos.clear();
	pos.resize(0);

	posX = ref new Platform::Array<float>(10000);
	posY = ref new Platform::Array<float>(10000);
	posZ = ref new Platform::Array<float>(10000);
	box = ref new Platform::Array<float>(6);

	m_iGroupCount = 0;
	m_iPointCount = 0;
	m_iTotalPointCount = 0;
	
	m_iTempGroup = ref new Platform::Array<unsigned int>(10000);
	m_iTempMouseX = ref new Platform::Array<float>(10000);
	m_iTempMouseY = ref new Platform::Array<float>(10000);
	m_iTempGroupCount = 0;
	
	m_PtGroupList.~vector();
		
	vertices.~vector();
	vertexes.~vector();
	indices.~vector();
	textureU.~vector();
	textureV.~vector();
	verticesDual.~vector();
	verticesPBR.~vector();
	
	// not in original must watch
	m_loadingComplete = false;
	m_loadingDrawnObjectComplete = false;
	//CreateDeviceDependentResources();
}

void Hot3dxRotoDraw::RotoDrawSceneRender::OnDeviceRestored()
{
	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::MouseCursorRender(float positionX)
{
	    if (m_tracking)
		{
			if (m_loadingComplete)
			{
				D3D12_VIEWPORT rect = m_sceneDeviceResources->GetScreenViewport();

				XMVECTOR xAxis = XMVectorSet(10.0f, 0.0f, 0.0f, 0.0f);
				XMVECTOR yAxis = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
				XMVECTOR zAxis = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

				float vrx = m_vars->GetDXPage()->Get_xRotateGridCam();
				float vry = m_vars->GetDXPage()->Get_yRotateGridCam();
				//float vrz = m_vars->GetDXPage()->Get_zRotateGridCam();// +20.0f;

				xAxis = RotateYawPitchPointXMV(xAxis, vry, vrx);
				yAxis = RotateYawPitchPointXMV(yAxis, vry, vrx);
				zAxis = RotateYawPitchPointXMV(zAxis, vry, vrx);

				// convert mouse points to number line 
				// plus/ minus coordinates
				// and convert to float
				float x = 0.0f;
				float y = 0.0f;
				float z = 0.0f;
				float centerx;
				float centery;

				// convert mouse points to number line 
				// plus/ minus coordinates
				// and convert to float
				XMFLOAT3 camdir = XMFLOAT3(m_hot3dxGeo->directionBetweenPoints(XMFLOAT3(m_EyeX, m_EyeY, m_EyeZ), XMFLOAT3(m_LookAtZ, m_LookAtY, m_LookAtZ)));
				float cammag = (float)m_hot3dxGeo->Magnitude(camdir);
				float midWidth = rect.Width / 2.0f;
				float midHeight = rect.Height / 2.0f;
				float m_cameraDistanceWidthRatio = cammag / 1.43f;// 1.428571428571429f; // -20.0f / 14.0f (The original camera position divided by the height and width in DirectX units
				m_cameraDistanceRatio = cammag / 1.43f;// 1.052631578947368f; // for a perfectly square grid
				m_drawMouseWidthRatio = m_cameraDistanceWidthRatio / midWidth;// 14.0f //28.0f / Width would also work for the drawWidthRatio
				m_drawMouseHeightRatio = m_cameraDistanceRatio / midHeight;// 14.0f // 28.0f / Height would also work for the drawHeightRatio

				if (positionX > 0 || point.x < (rect.TopLeftX - rect.Width)
					|| point.y > 0 || point.y < (rect.TopLeftY - rect.Height))
				{
					centerx = midWidth;
					centery = midHeight;

					if (m_IsLeftHanded)
					{
						x = ((point.x - centerx) * m_drawMouseWidthRatio);// for left handed minus sign on x
					}
					else
					{
						x = -((point.x - centerx) * m_drawMouseWidthRatio);// for right handed remove minus sign on x
					}// eo if (m_IsLeftHanded)
					y = -((point.y - centery) * m_drawMouseHeightRatio);

					XMVECTOR xx, yy;// , zz;
					xx = XMVectorScale(xAxis, x);
					yy = XMVectorScale(yAxis, y);
					XMVECTOR mouse3dPos = XMVectorSet((XMVectorGetX(yy) - XMVectorGetX(xx)) + m_LookAtX, (XMVectorGetY(yy) - XMVectorGetY(xx)) + m_LookAtY, (XMVectorGetZ(yy) - XMVectorGetZ(xx)) + m_LookAtZ, 0.0f);

					float radians = XM_2PI * 2.0f * positionX / m_sceneDeviceResources->GetOutputSize().Width;
					Rotate(radians);

					XMVECTOR eye = XMVectorSet(m_EyeX, m_EyeY, m_EyeZ, 0.0f);
					XMVECTOR look = XMVectorSet(m_LookAtZ, m_LookAtY, m_LookAtZ, 0.0f);
					float camToFaceCenterDist = (float)m_hot3dxGeo->distanceBetweenPoints(XMFLOAT3(m_EyeX, m_EyeY, m_EyeZ), XMFLOAT3(m_LookAtZ, m_LookAtY, m_LookAtZ));
					if (camToFaceCenterDist == 0.0f) {}
					XMFLOAT3 dir = XMFLOAT3(m_hot3dxGeo->directionBetweenPoints(XMFLOAT3(m_EyeX, m_EyeY, m_EyeZ), XMFLOAT3(m_LookAtZ, m_LookAtY, m_LookAtZ)));
					XMVECTOR lineDirection = XMVectorSet(dir.x * 20.f, dir.y * 20.f, dir.z * 20.f, 0.0f);
					XMVECTOR planeNormal = m_hot3dxGeo->FindPlaneNormalVec(xAxis, yAxis, zAxis);
					float planeConstant = (float)m_hot3dxGeo->FindPlaneConstantVec(planeNormal, xAxis);
					XMVECTOR intersect = m_hot3dxGeo->FindPointIntersectPlaneVec(planeNormal, eye, lineDirection, planeConstant);
					//XMVECTOR planePt = SimpleMath::XMPlaneFromPointNormal(look, planeNormal);
					
					point.x = x;
					point.y = y;
					z = dir.z;

					DirectX::XMVectorSetX(mouse3dPos, XMVectorGetX(intersect));
					DirectX::XMVectorSetY(mouse3dPos, XMVectorGetY(intersect));
					DirectX::XMVectorSetZ(mouse3dPos, XMVectorGetZ(intersect));

					m_posX = XMVectorGetX(mouse3dPos);
					m_posY = XMVectorGetY(mouse3dPos);
					m_posZ = XMVectorGetZ(mouse3dPos);
				}
			}
		}// eo m_tracking if
}

/// @brief The 3D drawing cursor
/// @param cursorPos 
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawPointsOne(XMFLOAT3 cursorPos)
{
	CheckMaximumPointCount(3000);
	if (m_tracking)
	{
		if (m_bMouse3dPosDist)
		{
			if (m_iPointCount > 0)
			{
				if (m_vars->GetDXPage()->GetIfRightShiftKeyHeldDrawStraightLine() == true)
				{
					cursorPos.y = m_previousPosY;
				}
				else if (m_vars->GetDXPage()->GetIfLeftShiftKeyHeldDrawStraightLine() == true)
				{
					cursorPos.x = m_previousPosX;
				}
				else if (m_vars->GetDXPage()->GetIfIfRightShiftKeySphereRadius() == true)
				{
					//XMVECTOR v1;
					DirectX::FXMVECTOR norm1 = { (m_xmfSphereRadiusPoint.x, m_xmfSphereRadiusPoint.y, 0.0f) };
					XMVECTOR dist = norm1;
					dist = XMVector2Length(norm1);
					XMVECTOR norm = XMVector2Normalize(norm1);
					    float psp = (m_fPointSpace/2) * (m_fPointSpace/2);
						float dxp = m_xmfSphereRadiusPoint.x * m_xmfSphereRadiusPoint.x;
						float bsq = sqrtf(dxp - psp);
						if (bsq == 0.0f) {}
						float dyp = m_previousPosY - m_xmfSphereRadiusPoint.y;
						if(dyp == 0.0f){}
						float dx = dist.m128_f32[0] * 1.0f * cosf(0.017453293005625408f);
						float dy = sqrtf(powf(dist.m128_f32[1], 2.0f) - powf(dx, 2.0f));
							//(m_previousPosY + m_fPointSpace) * sinf(0.017453293005625408f);
						cursorPos.x = dx;
						cursorPos.y = dy;
				}
				else if (m_vars->GetDXPage()->GetIfLeftShiftKeyHeldDraw45Line() == true)
				{
					if (cursorPos.x - m_previousPosX > 0)
					{
						cursorPos.x = m_previousPosX + m_fPointSpace;
					}
					else if (cursorPos.x - m_previousPosX < 0)
					{
						cursorPos.x = m_previousPosX - m_fPointSpace;
					}

					if (cursorPos.y - m_previousPosY > 0)
					{
						cursorPos.y = m_previousPosY + m_fPointSpace;
					}
					else if (cursorPos.y - m_previousPosY < 0)
					{
						cursorPos.y = m_previousPosY - m_fPointSpace;
					}
				} // if (m_vars->GetDXPage()->GetIfLeftShiftKeyHeldDraw45Line())
			}// if (m_iPointCount > 0)

			if (!posX || posX->Length < 10000)
			{
				posX = ref new Platform::Array<float>(10000);
			}
			if (!posY || posY->Length < 10000)
			{
				posY = ref new Platform::Array<float>(10000);
			}
			if (!posZ || posZ->Length < 10000)
			{
				posZ = ref new Platform::Array<float>(10000);
			}

			if (!m_iTempMouseX || m_iTempMouseX->Length < 10000)
			{
				m_iTempMouseX = ref new Platform::Array<float>(10000);
			}
			if (!m_iTempMouseY || m_iTempMouseY->Length < 10000)
			{
				this->m_iTempMouseY = ref new Platform::Array<float>(10000);
			}

			posX->set(m_iPointCount, cursorPos.x);// posCursor.x;
			posY->set(m_iPointCount, cursorPos.y);// posCursor.y;
			posZ->set(m_iPointCount, cursorPos.z);// posCursor.z;
			
			m_previousPosX = cursorPos.x;
			m_previousPosY = cursorPos.y;
			m_previousPosZ = cursorPos.z;

			m_iTempGroup->set(m_iTempGroupCount, m_iPointCount);
			m_iTempMouseX->set(m_iTempGroupCount, point.x);
			m_iTempMouseY->set(m_iTempGroupCount, point.y);
			m_iTempGroupCount++;
			m_iPointCount++;
		}
	} // eo if (m_tracking)
}

/// <summary>
/// Saves a memory copy of the drawn line in Hot3dxRotoDraw
/// That can be used to make a same object with different
/// textures and material settings
/// </summary>
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyPointsXYAxis()
{

	unsigned int count = m_iPointCount;
	
	for (unsigned int i = 0; i < count; i++)
	{
		posXCpy->set(m_iCpyPointCount, posX->get(i));// posCursor.x;
		posYCpy->set(m_iCpyPointCount, posY->get(i));// posCursor.y;
		posZCpy->set(m_iCpyPointCount, posZ->get(i));// posCursor.z;

		m_previousPosX = posXCpy->get(i);
		m_previousPosY = posYCpy->get(i);
		m_previousPosZ = posZCpy->get(i);

		m_iCpyPointCount++;
	}
}

/// <summary>
/// Draws a memory copy of the drawn line in Hot3dxRotoDraw
/// That can be used to make a same object with different
/// textures and material settings
/// </summary>
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawCopyPointsXYAxis(unsigned int ptCount)
{
	for (unsigned int i = 0; i < ptCount; i++)
	{
		posX->set(i, posXCpy->get(i));// posCursor.x;
		posY->set(i, posYCpy->get(i));// posCursor.y;
		posZ->set(i, posZCpy->get(i));// posCursor.z;

		m_previousPosX = posX->get(i);
		m_previousPosY = posY->get(i);
		m_previousPosZ = posZ->get(i);

		m_iTempGroup->set(m_iTempGroupCount, m_iCpyPointCount);
		m_iTempMouseX->set(m_iTempGroupCount, posX->get(i));
		m_iTempMouseY->set(m_iTempGroupCount, posY->get(i));
		m_iTempGroupCount++;
		m_iPointCount++;
	}
}

/// <summary>
/// Clear a memory copy of the drawn line in Hot3dxRotoDraw
/// That can be used to make a same object with different
/// textures and material settings
/// </summary>
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::ClearCopyPointsXYAxis()
{
	m_iCpyPointCount = 0;
	if (posXCopy->Length > 0)
	{
		if (posXCopy)
		{
			posXCopy->~Array();
			posYCopy->~Array();
			posZCopy->~Array();
		}
		posXCpy = ref new Platform::Array<float>(200);
		posYCpy = ref new Platform::Array<float>(200);
		posZCpy = ref new Platform::Array<float>(200);

		ClearDrawnObject();
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyPointsXAxis() {}
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyPointsYAxis() {}
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyPointsZAxis() {}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyFlipPointsXAxis()
{
	unsigned int count = m_iPointCount;
	unsigned int j = count - 1;
	//m_iPointCount++;
	for (unsigned int i = 0; i < count; i++)
	{
		posX->set(m_iPointCount, posX->get(j));// posCursor.x;
		posY->set(m_iPointCount, -posY->get(j));// posCursor.y;
		posZ->set(m_iPointCount, posZ->get(j));// posCursor.z;
		m_previousPosX = posX->get(j);
		m_previousPosY = -posY->get(j);
		m_previousPosZ = posZ->get(j);

		m_iTempGroup->set(m_iTempGroupCount, m_iPointCount);
		m_iTempMouseX->set(m_iTempGroupCount, m_iTempMouseX->get(j));
		m_iTempMouseY->set(m_iTempGroupCount, m_iTempMouseY->get(j));
		m_iTempGroupCount++;
		m_iPointCount++;
		j--;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyFlipPointsYAxis()
{
	//m_iTempGroupCount++;
	unsigned int count = m_iPointCount;
	unsigned int j = count - 1;
	//m_iPointCount++;
	for (unsigned int i = 0; i < count; i++)
	{
		posX->set(m_iPointCount, -posX->get(j));// posCursor.x;
		posY->set(m_iPointCount, posY->get(j));// posCursor.y;
		posZ->set(m_iPointCount, posZ->get(j));// posCursor.z;
		m_previousPosX = -posX->get(j);
		m_previousPosY = posY->get(j);
		m_previousPosZ = posZ->get(j);
		
		m_iTempGroup->set(m_iTempGroupCount, m_iPointCount);
		m_iTempMouseX->set(m_iTempGroupCount, m_iTempMouseX->get(j));
		m_iTempMouseY->set(m_iTempGroupCount, m_iTempMouseY->get(j));
		m_iTempGroupCount++;
		m_iPointCount++;
		j--;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyFlipPointsZAxis()
{
	unsigned int count = m_iPointCount;
	unsigned int j = count - 1;
	//m_iPointCount++;
	for (unsigned int i = 0; i < count; i++)
	{
		posX->set(m_iPointCount, -posX->get(j));// posCursor.x;
		posY->set(m_iPointCount, -posY->get(j));// posCursor.y;
		posZ->set(m_iPointCount, posZ->get(j));// posCursor.z;
		m_previousPosX = -posX->get(j);
		m_previousPosY = -posY->get(j);
		m_previousPosZ = posZ->get(j);

		m_iTempGroup[m_iTempGroupCount] = m_iPointCount;
		m_iTempMouseX[m_iTempGroupCount] = m_iTempMouseX[j];
		m_iTempMouseY[m_iTempGroupCount] = m_iTempMouseY[j];
		m_iTempGroupCount++;
		m_iPointCount++;
		j--;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndTopPointYAxis()
{
	if (m_iPointCount > 0)
	{
		XMFLOAT3 m_topEndPoint = XMFLOAT3(0.0f, posY->get(0), 0.0f);

		unsigned int i = m_iPointCount;
		for (i; i > 0;i--) 
		{
			XMFLOAT3 pt = XMFLOAT3(posX->get(i - 1), posY->get(i-1), posZ->get(i - 1));
			posX->set(i, pt.x);// posCursor.x;
			posY->set(i, pt.y);// posCursor.y;
			posZ->set(i, pt.z);// posCursor.z;
		}
		m_iPointCount++;
		posX->set(0, m_topEndPoint.x);// posCursor.x;
		posY->set(0, m_topEndPoint.y);// posCursor.y;
		posZ->set(0, m_topEndPoint.z);// posCursor.z;
		

	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndBottomPointYAxis()
{
	if (m_iPointCount > 0)
	{
		XMFLOAT3 m_bottomEndPoint = XMFLOAT3(0.0f, posY->get(m_iPointCount - 1), 0.0f);
		m_iPointCount++;
		posX->set(m_iPointCount - 1, m_bottomEndPoint.x);// posCursor.x;
		posY->set(m_iPointCount - 1, m_bottomEndPoint.y);// posCursor.y;
		posZ->set(m_iPointCount - 1, m_bottomEndPoint.z);// posCursor.z;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndLeftPointsXAxis()
{
	if (m_iPointCount > 0)
	{
		XMFLOAT3 m_leftEndPoint = XMFLOAT3(posX->get(0), 0.0f, 0.0f);

		posX->set(0, m_leftEndPoint.x);// posCursor.x;
		posY->set(0, m_leftEndPoint.y);// posCursor.y;
		posZ->set(0, m_leftEndPoint.z);// posCursor.z;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndRightPointsXAxis()
{
	if (m_iPointCount > 0)
	{
		XMFLOAT3 m_rightEndPoint = XMFLOAT3(posX->get(m_iPointCount - 1), 0.0f, 0.0f);

		posX->set(m_iPointCount - 1, m_rightEndPoint.x);// posCursor.x;
		posY->set(m_iPointCount - 1, m_rightEndPoint.y);// posCursor.y;
		posZ->set(m_iPointCount - 1, m_rightEndPoint.z);// posCursor.z;
	}
}

void Hot3dxRotoDraw::RotoDrawSceneRender::OnLButtonDown(UINT nFlags, XMFLOAT2 apoint)
{
	XMFLOAT2 curposxy{ 0.0f, 0.0f };
	if (apoint.x > 3 && apoint.x < 345 && apoint.y>3 && apoint.y < 345)
	{
		if (m_vars->GetDXPage()->GetLinearAcrossDXP() == true)
			curposxy.y = apoint.y;
		if (m_vars->GetDXPage()->GetLinearUpDXP() == true)
			curposxy.x = apoint.x;

		m_bLButtonDown = true;
		if (m_iDrawMode == 2)
		{
			apoint.y -= 2;
			Platform::String^ s = ref new Platform::String(std::to_wstring(nFlags).c_str());
			s->Length();
			//SelectVertex(nFlags, point);
			//XMFLOAT intersect = SelectGridFace(nFlags, point);
			
		}
		else
		{
			//Select(nFlags, point);

		}
	}
	
}

void Hot3dxRotoDraw::RotoDrawSceneRender::OnRightButtonDown(UINT nFlags, XMFLOAT2 apoint)
{
	
	XMFLOAT2 curposxy{ 0.0f, 0.0f };
	if (apoint.x > 3 && apoint.x < 345 && apoint.y>3 && apoint.y < 345)
	{
		//SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		if (m_vars->GetDXPage()->GetLinearAcrossDXP() == true)
			curposxy.y = apoint.y;
		if (m_vars->GetDXPage()->GetLinearUpDXP() == true)
			curposxy.x = apoint.x;

		m_bLButtonDown = true;
		if (m_iDrawMode == 2)
		{
			apoint.y -= 2;
			Platform::String^ s = ref new Platform::String(std::to_wstring(nFlags).c_str());
			s->Length();
			//SelectVertex(nFlags, point);
			//XMFLOAT intersect = SelectGridFace(nFlags, point);
			
		}
		else
		{
			//Select(nFlags, point);

		}
	}
}

void Hot3dxRotoDraw::RotoDrawSceneRender::OnMouseMove(UINT nFlags, XMFLOAT2 apoint)
{

	XMFLOAT2 curposxy{ 0.0f, 0.0f };
	if (apoint.x > 3 && apoint.x < 345 && apoint.y>3 && apoint.y < 345)
	{
		//SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		if (m_vars->GetDXPage()->GetLinearAcrossDXP() == true)
			curposxy.y = apoint.y;
		if (m_vars->GetDXPage()->GetLinearUpDXP() == true)
			curposxy.x = apoint.x;
		//XMFLOAT3 intersect = SelectGridFace(nFlags, point);

		if (m_iDrawMode == 2)
		{
			point.y -= 2;
			Platform::String^ s = ref new Platform::String(std::to_wstring(nFlags).c_str());
			s->Length();
			
		}
		else if (m_iDrawMode == 3)
		{
			if (m_bLButtonDown == true)
			{
			}
		}
	}
}

// Copies a Group of DirectX::VertexPositionColor points
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::Copy()
{
	unsigned int i = 0;

	m_GroupListSelectedIndex = i;
	DirectX::DXTKXAML12::VertexPositionColor vpc{}; vpc.position = XMFLOAT3(0, 0, 0); vpc.color = XMFLOAT4(0, 0, 0, 1);
	unsigned int sz = m_iPointCount - 1;
	XMFLOAT3* vs = new XMFLOAT3[(sz + 1) * sizeof(XMFLOAT3)];
	if (vs != NULL)
	{
		vertices.resize(static_cast<size_t>(sz));

		for (unsigned int j = 0; j < sz; j++)
		{
			XMFLOAT3 v = vpc.position;
			DWORD n = m_PtGroupList.at(i)->GetPtList()->Data[j];
			vertices.at(j) = vpc;
			vertices.at(n).position.x = vpc.position.x = v.x;
			vertices.at(n).position.y = vpc.position.y = v.y;
			vertices.at(n).position.z = vpc.position.z = v.z;
			XMVECTOR t = { 0.0f,0.0f,0.0f,0.0f };
			XMFLOAT4 txm3 = {};
			XMStoreFloat4(&txm3, t);
			
			XMMATRIX Transform = XMMatrixMultiply(XMLoadFloat4x4(&m_world4x4), XMLoadFloat4x4(&m_view4x4));
			Transform = XMMatrixMultiply( Transform, XMLoadFloat4x4(&m_projection4x4));
			Transform = XMMatrixInverse(&t, Transform);

			if (j < sz)
			vs[j] = XMFLOAT3(XMVectorGetX(t), XMVectorGetY(t), XMVectorGetZ(t));
		}//eojfor

		InitDimensionsBox();
		CreateDimensions(vs, sz);
		MakeBox(vs, (int)sz, box);

		if (sz > 0)
		{
			vs = nullptr;
			delete[] vs;
		}
	}
	if (sz > 0 || vs)
	{
		vs = nullptr;
		delete[] vs;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::InitDimensionsBox()
{
	box->set(0, 0.0f);
	box->set(1, 0.0f);
	box->set(2, 0.0f);
	box->set(3, 0.0f);
	box->set(4, 0.0f);
	box->set(5, 0.0f);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CreateDimensions(XMFLOAT3* copier, size_t qCount)
{
	for (size_t i = 0; i < qCount; i++)
	{
		if (copier[i].x < box->get(0)) { box->set(0, copier[i].x); }
		if (copier[i].x > box->get(1)) { box->set(1, copier[i].x); }
		if (copier[i].y < box->get(2)) { box->set(2, copier[i].y); }
		if (copier[i].y > box->get(3)) { box->set(3, copier[i].y); }
		if (copier[i].z < box->get(4)) { box->set(4, copier[i].z); }
		if (copier[i].z > box->get(5)) { box->set(5, copier[i].z); }
	}

}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::MakeBox(XMFLOAT3* copier, int qCount, Platform::Array<float>^ b)
{
	DWORD adwFace1Data[] = { 4,4,4,0,0,2,2,6,6,
							  4,4,4,6,6,2,2,0,0,
							  4,4,4,5,5,1,1,0,0,
							  4,4,4,0,0,1,1,5,5,
							  4,4,4,6,6,7,7,5,5,
							  4,4,4,5,5,7,7,6,6,
							  4,0,0,1,1,3,3,2,2,
							  4,0,0,2,2,3,3,1,1,
							  4,1,1,5,5,7,7,3,3,
							  4,1,1,3,3,7,7,5,5,
							  4,3,3,7,7,6,6,2,2,
							  4,3,3,2,2,6,6,7,7,0 };

	XMFLOAT3 avCubeVertices[] = { XMFLOAT3(box->get(0), box->get(2), box->get(4)),  XMFLOAT3(box->get(0), box->get(2),  box->get(5)),
									   XMFLOAT3(box->get(0),  box->get(3), box->get(4)),  XMFLOAT3(box->get(0),  box->get(3),  box->get(5)),
									   XMFLOAT3(box->get(1), box->get(2), box->get(4)),   XMFLOAT3(box->get(1), box->get(2),  box->get(5)),
									   XMFLOAT3(box->get(1),  box->get(3), box->get(4)),  XMFLOAT3(box->get(1),  box->get(3),  box->get(5)) };
	XMFLOAT3 avCube1Normals[8]{};

	for (int i = 0; i < 8; i++)
	{

		avCube1Normals[i] = avCubeVertices[i];
		DirectX::XMVector3Normalize(XMVectorSet(avCube1Normals[i].x, avCube1Normals[i].y, avCube1Normals[i].z, 0.0f));
	}

	//int cnt = qCount - 1;
	for (int i = 0; i < qCount; i++)
	{
		XMFLOAT3 v1{};
		v1.x = copier[i].x;
		v1.y = copier[i].y;
		v1.z = copier[i].z;
		DirectX::XMVector3Normalize(XMVectorSet(v1.x,v1.y,v1.z,0.0f));
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetPoints()
{
	//m_loadingDrawnObjectComplete = false;
	
	// Register our SwapChainPanel to get independent input pointer events
	auto drawItemHandler = ref new WorkItemHandler([this](IAsyncAction^ action)
		{
				critical_section::scoped_lock lock(m_drawCriticalSection);
				Hot3dxRotoDraw::Scenario2Vars^ page = m_vars->GetDXPage()->GetScene2Vars();
				if (page->GetTopOrLeftChecked() == true
					&& page->GetBottomOrRightChecked() == true)
				{
					DrawObjectPointsTopBottom();
					CalculateMeshFacesTopBottom();
				}
				else if (page->GetTopOrLeftChecked() == true
					&& page->GetBottomOrRightChecked() == false)
				{
					DrawObjectPointsTop();
					EndpointTopLeftFaces();
				}
				else if (page->GetTopOrLeftChecked() == false
					&& page->GetBottomOrRightChecked() == true)
				{
					DrawObjectPointsBottom();
					EndpointBottomRightFaces();
				}

				else if (page->GetTopOrLeftChecked() == false
					&& page->GetBottomOrRightChecked() == false)
				{
					DrawObjectPoints(0);
					CalculateMeshFaces();
				}
				m_loadingDrawnObjectComplete = false;

				if (GetEffectIndex() == 0)
					m_bIsBasicModel = true;

				if (m_bIsBasicModel == true) { InitDrawnObjectSingleTexture(); }
				else if (m_bIsDualTextureModel == true) { InitDrawnObjectDualTexture(); }
				else if (m_bIsPBRModel == true) { InitDrawnObjectPBRFiveTextures(); }
				else if (m_bIsVideoTextureModel == true) { InitDrawnObjectVideoTexture(); }
				else if (m_bIsBasicModelColor == true) { InitDrawnObjectSingleColor(); }
				m_iDrawMode = 2;
		});
	
	// Run task on a dedicated high priority background thread.
	m_drawObjectWorker = ThreadPool::RunAsync(drawItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
	m_drawObjectWorker->Completed::get();
}

bool RotoDrawSceneRender::RenderPBR(DirectX::XMMATRIX localDrawnObject)
{
	
	m_bDDS_WIC_FLAGGridPicComplete = false;
	auto commandList = m_sceneDeviceResources->GetCommandList();
	auto device = m_sceneDeviceResources->GetD3DDevice();
	//ID3D12DescriptorHeap* heapss[] = { m_resDescPile->Heap(), m_states->Heap() };
	//commandList->SetDescriptorHeaps(_countof(heapss), heapss);

	m_shapeDrawnObjectPBREffect->SetWorld(localDrawnObject);
	m_shapeDrawnObjectPBREffect->Apply(commandList);
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"RenderPBR");
	
	m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateCustomTangent(verticesPBR, indices, device);

	//DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadGridPic = new DirectX::DXTKXAML12::ResourceUploadBatch(m_sceneDeviceResources->GetD3DDevice());
	//m_resourceUploadGridPic->BeginXaml();

	//auto depthStencilDescriptor = m_sceneDeviceResources->GetDepthStencilView();
	//auto toneMapRTVDescriptor = m_sceneDeviceResources->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
	//commandList->OMSetRenderTargets(1, &toneMapRTVDescriptor, FALSE, &depthStencilDescriptor);

	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw PBR Object");
	
	m_shapeDrawnObjectPBREffect->SetWorld(localDrawnObject);
	m_shapeDrawnObjectPBREffect->Apply(commandList);
	m_hot3dxDrawnObjectPBR->Draw(commandList);
	PIXEndEvent(commandList); // Model Draw
	/*
	{
		PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Tonemap");
		auto rtv = static_cast<D3D12_CPU_DESCRIPTOR_HANDLE>(m_sceneDeviceResources->GetRenderTargetView());
		commandList->OMSetRenderTargets(1, &rtv, FALSE, NULL);
		
		if (m_sceneDeviceResources->GetColorSpace() == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
		{
			// HDR10 signal
			m_HDR10->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(static_cast<size_t>(PBRDescriptors::PicTex)));///::SceneTex));
			m_HDR10->Process(commandList);
		}
		else
		{
			// Tonemap for SDR signal
			m_toneMap->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(static_cast<size_t>(PBRDescriptors::PicTex)));//::SceneTex));
			m_toneMap->Process(commandList);
		}
		
		PIXEndEvent(commandList);

		PIXEndEvent(commandList);


	}*/
	return true;
}
// Renders one frame using the vertex and pixel shaders.
bool RotoDrawSceneRender::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return false;
	}
	DirectXPage^ dxpage = m_vars->GetDXPage();
	// Prepare the command list to render a new frame.
	m_sceneDeviceResources->Prepare();
	Clear();
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");
	
	Scenario5_Tex^ sc5 = dxpage->m_Scene5TexVars->GetScenario5TexPage();
	if (dxpage->m_Scene5TexVars->GetGridChecked() == true)
	{
		DrawGridXY();
	}
	///////////////////////////////////////
	// Set the descriptor heaps
	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	if (m_bDDS_WIC_FLAGGridPic == true && m_bDDS_WIC_FLAGGridPicComplete == true)
	{
		if (dxpage->m_Scene5TexVars->GetPicChecked() == true)
		{
			PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw GridPic");
			// Draw WIC PIC
			DirectX::XMMATRIX localGridPic = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f));
			// *XMMatrixRotationRollPitchYawFromVector(angles));
			m_drawRectangleEffect->SetWorld(localGridPic);
			m_drawRectangleEffect->Apply(commandList);
			m_shapeGridPic->Draw(commandList);
			PIXEndEvent(commandList);
			// EO Draw WIC PIC
		}
	}

	if (m_drawMode == (int)RotoDrawDrawMode::DrawSceneParts || m_drawMode == (int)RotoDrawDrawMode::DrawLineOnlyObject)
	{
		//Draw3DCursorXY(xaxis12, yaxis12, zaxis12, XMFLOAT3(m_posX, m_posY, m_posZ));
		Draw3DCursorXY(XMFLOAT3(m_posX, m_posY, m_posZ));
	}
	
	// Draw the Initial Line
	if (m_drawMode == (int)RotoDrawDrawMode::DrawLineOnlyObject)//7
	{
		Windows::UI::Color c = m_vars->GetDXPage()->GetPointColorDXP();
		float a = c.A * RGBA_FRACTAL;
		float r = c.R * RGBA_FRACTAL;
		float g = c.G * RGBA_FRACTAL;
		float b = c.B * RGBA_FRACTAL;
		GXMVECTOR color = XMVectorSet(r, g, b, a);
		DrawLineOnlyObject(color);
	}
	
	if (m_loadingDrawnObjectComplete)
	{
		
		PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw Object");
		m_drawMode = (int)RotoDrawDrawMode::DrawTexturedObject;

		
		const XMVECTORF32 scale = { dxpage->Get_xScaleDrawnObject(), dxpage->Get_yScaleDrawnObject(), dxpage->Get_zScaleDrawnObject(), 1.0f };
		const XMVECTORF32 translate = { dxpage->Get_xTranslateDrawnObject(), dxpage->Get_yTranslateDrawnObject(), dxpage->Get_zTranslateDrawnObject() };
		XMVECTOR angles = { dxpage->Get_xRotateDrawnObject(), dxpage->Get_yRotateDrawnObject(), dxpage->Get_zRotateDrawnObject() };// { XM_PI / 2.f, 0.f, -XM_PI / 2.f };
		XMVECTOR rotate = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles);
		// orig
		DirectX::XMMATRIX localDrawnObject = XMLoadFloat4x4(&m_world4x4) * XMMatrixTransformation(g_XMZero, DirectX::XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);

		//DirectX::XMMATRIX localDrawnObject = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
		// *XMMatrixRotationRollPitchYawFromVector(angles));
		

		if (m_performScaleDrawnObjectComplete)
		{
			const XMVECTORF32 scale1 = { 0.01f, 0.01f, 0.01f };
			const XMVECTORF32 translate1 = { 0.0f, 0.0f, 0.0f };
			XMVECTOR angles1 = { 0.0f, 0.0f, 0.0f };// { XM_PI / 2.f, 0.f, -XM_PI / 2.f };
			XMVECTOR rotate1 = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles1);
			// orig
			//localDrawnObject = XMLoadFloat4x4(&m_world4x4) * XMMatrixTransformation(g_XMZero, DirectX::XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
			//Model::UpdateEffectMatrices(m_modelEffects, local, XMLoadFloat4x4(&m_view4x4), XMLoadFloat4x4(&m_projection4x4));
			//m_shapeDrawnObject->DrawIndexed(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices.data(), indices.size(), vertices.data(), vertices.size());
		}

		if (m_bIsBasicModel == true)
		{
			if (m_shapeDrawnObjectEffect) {
				
				m_shapeDrawnObjectEffect->SetWorld(localDrawnObject);
				m_shapeDrawnObjectEffect->Apply(commandList);
				m_shapeDrawnObjectTex->Draw(commandList);
			}
		}
		if (m_bIsBasicModelColor == true)
		{
			if (m_shapeDrawnObjectEffect) {

				m_shapeDrawnObjectEffect->SetWorld(localDrawnObject);
				m_shapeDrawnObjectEffect->Apply(commandList);
				m_shapeDrawnObjectColor->Draw(commandList);
			}
		}
		else if (m_bIsDualTextureModel == true)
		{
			if (m_dualTextureEffect) {
				m_dualTextureEffect->SetWorld(localDrawnObject);
				m_dualTextureEffect->Apply(commandList);
				m_hot3dxDrawnObjectDual->Draw(commandList);
			}
		}
		else if (m_bIsPBRModel == true)
		{
			if (m_shapeDrawnObjectPBREffect) {
				ID3D12DescriptorHeap* heapsss[] = { m_resDescPile->Heap(), m_states->Heap() };
				commandList->SetDescriptorHeaps(_countof(heapsss), heapsss);
				/*
				auto vp = m_sceneDeviceResources->GetScreenViewport();
				m_batchOpaque->SetViewport(vp);
				RECT output{ 0, 0, long(m_sceneDeviceResources->GetOutputSize().Width), long(m_sceneDeviceResources->GetOutputSize().Height) };
				RECT safeRect = DirectX::DXTKXAML12::SimpleMath::Viewport::ComputeTitleSafeArea(output.right, output.bottom);
				m_batchOpaque->Begin(commandList);

				m_batchOpaque->Draw(
					m_resourceDescriptors->GetGpuHandle(static_cast<size_t>(PBRDescriptors::NormalTex)),
					XMUINT2(vp.Width, vp.Height),
					XMFLOAT2(float(safeRect.left), float(safeRect.top)), nullptr, Colors::White);
				m_batchOpaque->End();
				*/
				//RenderPBR(localDrawnObject);
				m_shapeDrawnObjectPBREffect->SetWorld(localDrawnObject);
				m_shapeDrawnObjectPBREffect->Apply(commandList);
				m_hot3dxDrawnObjectPBR->Draw(commandList);
				
			}
		}
		else if (m_bIsVideoTextureModel == true)
		{
			if (m_shapeDrawnObjectVideoEffect != nullptr && m_shapeDrawnObjectVideoTex != nullptr) {
				/*
				auto vp = m_sceneDeviceResources->GetScreenViewport();
				m_batchOpaque->SetViewport(vp);
				RECT output{ 0, 0, long(m_sceneDeviceResources->GetOutputSize().Width), long(m_sceneDeviceResources->GetOutputSize().Height) };
				RECT safeRect = DirectX::DXTKXAML12::SimpleMath::Viewport::ComputeTitleSafeArea(output.right, output.bottom);
				m_batchOpaque->Begin(commandList);

				m_batchOpaque->Draw(
					m_resourceDescriptors->GetGpuHandle(static_cast<size_t>(Descriptors::VideoTexture)),
					XMUINT2(m_videoWidth, m_videoHeight),
					XMFLOAT2(float(safeRect.left), float(safeRect.top)), nullptr, Colors::White);
				m_batchOpaque->End();
				*/
				m_shapeDrawnObjectVideoEffect->SetWorld(localDrawnObject);
				m_shapeDrawnObjectVideoEffect->Apply(commandList);
			    m_shapeDrawnObjectVideoTex->Draw(commandList);
			}
		}
		else if (m_bIsSculptWireframe == true)
		{
			if (m_shapeDrawnObjectEffectSculpt) {

				m_shapeDrawnObjectEffectSculpt->SetWorld(localDrawnObject);
				m_shapeDrawnObjectEffectSculpt->Apply(commandList);
				m_shapeDrawnObjectSculpt->Draw(commandList);
			}
		}
		PIXEndEvent(commandList);
	}
	
		//PIXEndEvent(commandList);
	
	
	if (m_drawMode == (int)RotoDrawDrawMode::DrawSceneParts || m_drawMode == (int)RotoDrawDrawMode::DrawLineOnlyObject || m_drawMode == (int)RotoDrawDrawMode::DrawSelectWithTetras) //1
	{
		
			XMVECTOR angles = { XM_PI / 2.f, 0.f, -XM_PI / 2.f };//0.0f, 0.0f, 0.0f};// {
			XMVECTOR rotate = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles);
			DirectX::XMVECTOR scale = { 0.25f, 0.25f, 0.25f, 0.25f };
			DirectX::XMVECTOR translate = {};
			
			// Draw 3D object
			PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw tetras");
			if (m_drawMode == (int)RotoDrawDrawMode::DrawSceneParts || m_drawMode == (int)RotoDrawDrawMode::DrawLineOnlyObject)// 0;
			{
				for (unsigned int i = 0; i < m_iPointCount; i++)
				{
					DirectX::XMMATRIX localTetra2 = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(posX->get(i), posY->get(i), posZ->get(i)));
					XMMatrixRotationRollPitchYawFromVector(angles);
					m_shapeTetraEffect->SetWorld(localTetra2);
					m_shapeTetraEffect->Apply(commandList);
					m_shapeTetra->Draw(commandList);
				}
			}


			if (m_drawMode == (int)RotoDrawDrawMode::DrawSelectWithTetras)// 1;
			{
				size_t cnt = vertices.size();
				for (size_t i = 0; i < cnt; i++)
				{
					DirectX::DXTKXAML12::VertexPositionColor v = vertices.at(i);
					DirectX::XMMATRIX localTetra2 = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(v.position.x, v.position.y, v.position.z));
					XMMatrixRotationRollPitchYawFromVector(angles);
					m_shapeTetraEffect->SetWorld(localTetra2);
					m_shapeTetraEffect->Apply(commandList);
					m_shapeTetra->Draw(commandList);
				}

			} // eo if (m_drawMode == (int)RotoDrawDrawMode::DrawSelectWithTetras
			PIXEndEvent(commandList);
		
	}
	
	PIXEndEvent(commandList);
	DrawSprites(commandList);// this must stay here!!!
	
	
		// Show the new frame.
		PIXBeginEvent(m_sceneDeviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Switch Frames");
		if (m_bIsVideoTextureModel == true) {
			RECT r = { 0, 0, long(m_videoWidth), long(m_videoHeight) };
			MFVideoNormalizedRect rect = { 0.0f, 0.0f, 1.0f, 1.0f };
			if (m_bIsPlayer == true && m_player->IsPlaying() == true)// && m_player->IsInfoReady())
			{
				m_player->TransferFrame(m_sharedVideoTexture, rect, r);
				if (m_player->IsFinished() == true)
				{
					
					m_player->Shutdown();
					m_bIsPlayer = false;
					//m_sharedVideoTexture = 0;
					m_bIsVideoTextureModel = false;
					m_vars->GetDXPage()->NotifyUser("Video Texture Finished Playing. Press Clear Button to Draw Next Object", NotifyType::StatusMessage);
					
				}
			}
			
		}
		PIXEndEvent(commandList);
	
	PIXEndEvent(commandList);
		
	//////////////////////////////////////
	PIXBeginEvent(m_sceneDeviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");

	m_sceneDeviceResources->Present();

	m_graphicsMemory->Commit(m_sceneDeviceResources->GetCommandQueue());

	PIXEndEvent(m_sceneDeviceResources->GetCommandQueue());

	return true;
}

// Helper method to clear the back buffers.
void Hot3dxRotoDraw::RotoDrawSceneRender::Clear()
{
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

	// Clear the views.
	auto rtvDescriptor = m_sceneDeviceResources->GetRenderTargetView();
	auto dsvDescriptor = m_sceneDeviceResources->GetDepthStencilView();
	
	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	commandList->ClearRenderTargetView(rtvDescriptor, m_backGroundColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
	auto viewport = m_sceneDeviceResources->GetScreenViewport();
	auto scissorRect = m_sceneDeviceResources->GetScissorRect();
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

	PIXEndEvent(commandList);
}

//void XM_CALLCONV Draw3DCursorXY(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, XMFLOAT3 curPos);
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::Draw3DCursorXY(XMFLOAT3 curPos)
{
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw 3D cursor");
	DirectX::XMMATRIX localCursorXY = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));//curPos.x, curPos.y, curPos.z));// 
	m_cursorEffect->SetWorld(localCursorXY);
	m_cursorEffect->Apply(commandList);
	m_batch->Begin(commandList);
	curPos.x = point.x;
	curPos.y = point.y;
	XMFLOAT3 posCursor = { curPos.x, curPos.y, curPos.z };

	float len = 0.5f;
			GXMVECTOR color = XMVectorSet(0.0f, 0.5f, 0.5f, 1.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSet(posCursor.x - len, posCursor.y, posCursor.z, 0.0f), color);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorSet(posCursor.x + len, posCursor.y, posCursor.z, 0.0f), color);
			m_batch->DrawLine(v1, v2);
		
			GXMVECTOR color2 = XMVectorSet(0.5f, 0.5f, 0.0f, 1.0f);
			DirectX::DXTKXAML12::VertexPositionColor v3(XMVectorSet(posCursor.x, posCursor.y - len, posCursor.z, 0.0f), color2);
			DirectX::DXTKXAML12::VertexPositionColor v4(XMVectorSet(posCursor.x, posCursor.y + len, posCursor.z, 0.0f), color2);
			m_batch->DrawLine(v3, v4);
		


			GXMVECTOR color3 = XMVectorSet(0.5f, 0.0f, 0.5f, 1.0f);
			DirectX::DXTKXAML12::VertexPositionColor v5(XMVectorSet(posCursor.x, posCursor.y, posCursor.z - len, 0.0f), color3);
			DirectX::DXTKXAML12::VertexPositionColor v6(XMVectorSet(posCursor.x, posCursor.y, posCursor.z + len, 0.0f), color3);
			m_batch->DrawLine(v5, v6);
			
			if (checkDistance(curPos.x, curPos.y, curPos.z, m_vars->GetDXPage()->GetPointSpacingDXP()))
				m_posX = curPos.x;
			    m_posY = curPos.y;
			    m_posZ = curPos.z;
			if (m_bLButtonDown)
			{
				if(checkDistance(curPos.x, curPos.y, curPos.z, m_vars->GetDXPage()->GetPointSpacingDXP()))
				DrawPointsOne(curPos);
			// the below 3 lines cause the drawing
			// change the final drawing from what is drawn
			//m_posX = curPos.x;
			//m_posY = curPos.y;
			//m_posZ = curPos.z;
			}// eo if (m_bLButtonDown)
			/*
			if (m_bRButtonDown)
			{
				if (checkDistance(curPos.x, curPos.y, curPos.z, m_vars->GetDXPage()->GetPointSpacingDXP()))
					DrawPointsOne(curPos); 
				m_xmfSphereRadiusPoint = { curPos.x, curPos.y, curPos.z };
				m_posX = curPos.x;
				m_posY = curPos.y;
				m_posZ = curPos.z;
			}
			*/
	m_batch->End();
	
	PIXEndEvent(commandList);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::ClearGridPicRectangle()
{
	m_bDDS_WIC_FLAGGridPic = false;
	m_bDDS_WIC_FLAGGridPicComplete = false;
	m_texturePic2Name = nullptr;// .Reset();
	m_shapeGridPic = nullptr;// .reset();
	m_drawRectangleEffect = nullptr;// .reset();
}


//void XM_CALLCONV DrawLineOnlyObject(FXMVECTOR xAxis, FXMVECTOR zAxis, FXMVECTOR origin, GXMVECTOR color);
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawLineOnlyObject(GXMVECTOR color)
{
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw Single Line");
	DirectX::XMMATRIX objectLineXY = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	m_lineEffect->SetWorld(objectLineXY);
	m_lineEffect->Apply(commandList);
	m_batch->Begin(commandList);
	
	for (unsigned int i = 0; i < m_iPointCount; ++i)
	{
		
		if (i == 0)
		{
			GXMVECTOR position = XMVectorSet(posX->get(i), posY->get(i), posZ->get(i), 0.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(position, color);// XMVectorSubtract(vScale, zAxis), color2);
			m_batch->DrawLine(v1, v1);
		}
		else
		{
			GXMVECTOR position = XMVectorSet(posX->get(i - 1), posY->get(i - 1), posZ->get(i - 1), 0.0f);
			GXMVECTOR position2 = XMVectorSet(posX->get(i), posY->get(i), posZ->get(i), 0.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(position, color);// XMVectorSubtract(vScale, zAxis), color2);
			DirectX::DXTKXAML12::VertexPositionColor v2(position2, color);// XMVectorSubtract(vScale, zAxis), color2);
			m_batch->DrawLine(v1, v2);
		}
	}

	m_batch->End();

	PIXEndEvent(commandList);
}

void Hot3dxRotoDraw::RotoDrawSceneRender::DrawGridInit(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
	lineGridVertices1.clear();
	lineGridVertices2.clear();

	lineGridVertices1.resize(0);
	lineGridVertices2.resize(0);

	lineGridVertices1.resize((xdivs + 1) + (ydivs + 1));
	lineGridVertices2.resize((xdivs + 1) + (ydivs + 1));
	for (size_t i = 0; i <= xdivs; ++i)
	{
		float fPercent = float(i) / float(xdivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		if (i == 5)
		{
			GXMVECTOR color2 = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color2);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color2);
			lineGridVertices1.at(i) = v1;
			lineGridVertices2.at(i) = v2;
		}
		else
		{
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
			lineGridVertices1.at(i) = v1;
			lineGridVertices2.at(i) = v2;
		}
	}

	for (size_t i = 0; i <= ydivs; i++)
	{
		float fPercent = float(i) / float(ydivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		if (i == 5)
		{
			GXMVECTOR color2 = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color2);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color2);
			lineGridVertices1.at(i + xdivs + 1) = v1;
			lineGridVertices2.at(i + xdivs + 1) = v2;
		}
		else
		{
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
			lineGridVertices1.at(i + xdivs + 1) = v1;
			lineGridVertices2.at(i + xdivs + 1) = v2;
		}
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawGridXY()
{
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw grid");
	DirectX::XMMATRIX localGridXY = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	//Kubed next 4 lines
	DirectXPage^ dxp = m_vars->GetDXPage();
	float vrx = dxp->Get_xRotateGridCam();
	float vry = dxp->Get_yRotateGridCam();
	float vrz = dxp->Get_zRotateGridCam();
	if (vrz == 0.0f) {}

	m_lineEffect->SetWorld(localGridXY);
	m_lineEffect->Apply(commandList);
	m_batch->Begin(commandList);
	
	size_t cnt = lineGridVertices1.size();

	for (size_t i = 0; i < cnt; i++)
	{
		XMFLOAT3 x1 = RotateYawPitchPointXMFL3(lineGridVertices1.at(i).position, vry, vrx);
		DirectX::DXTKXAML12::VertexPositionColor v1(x1, lineGridVertices1.at(i).color);

		XMFLOAT3 x2 = RotateYawPitchPointXMFL3(lineGridVertices2.at(i).position, vry, vrx);
		DirectX::DXTKXAML12::VertexPositionColor v2(x2, lineGridVertices2.at(i).color);
		m_batch->DrawLine(v1, v2);
	}
	m_batch->End();
	
	PIXEndEvent(commandList);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawSprites(ID3D12GraphicsCommandList* commandList)
{
	
		ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap()};
		commandList->SetDescriptorHeaps(_countof(heaps), heaps);
		//auto commandList = m_sceneDeviceResources->GetCommandList();
		// Draw sprite
		PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw sprite");
		m_sprites->Begin(commandList);

		// Put MousePosition on screen
		Platform::String^ m_fontString = nullptr;
		m_MousePosFont->DrawString(m_sprites.get(),
			ObjectXYZPositionString(m_fontString, "Mouse ", m_posX, m_posY, m_posZ)->Data(),
			XMFLOAT2(10, 10), Colors::OrangeRed);

		// Put Camera Eye on screen
		Platform::String^ m_fontStringCameraEye = nullptr;
		m_CameraEyeFont->DrawString(m_sprites.get(),
			ObjectXYZPositionString(m_fontStringCameraEye, "Camera Eye ", m_EyeX, m_EyeY, m_EyeZ)->Data(),
			XMFLOAT2(10, 30), Colors::DarkSeaGreen);

		// Put Camera At on screen
		Platform::String^ m_fontStringCameraAt = nullptr;
		m_CameraAtFont->DrawString(m_sprites.get(),
			ObjectXYZPositionString(m_fontStringCameraAt, "Camera At  ", m_LookAtX, m_LookAtY, m_LookAtZ)->Data(),
			XMFLOAT2(10, 60), Colors::BlueViolet);

		// Put Camera Up on screen
		Platform::String^ m_fontStringCameraUp = nullptr;
		m_CameraUpFont->DrawString(m_sprites.get(),
			ObjectXYZPositionString(m_fontStringCameraUp, "Camera Up  ", m_UpX, m_UpY, m_UpZ)->Data(),
			XMFLOAT2(10, 90), Colors::DarkOrchid);

		// Put Camera Up on screen
		Platform::String^ m_fontStringPointCount = nullptr;
		m_PointCountFont->DrawString(m_sprites.get(),
			PointCountString(m_fontStringPointCount, "Pt Cnt", m_iPointCount)->Data(),
			XMFLOAT2(10, 300), Colors::MintCream);

		// Put Camera Up on screen
		Platform::String^ m_fontStringMAXPointCount = nullptr;
		m_MAX_PointCountFont->DrawString(m_sprites.get(),
			PointCountString(m_fontStringMAXPointCount, "MAX Pt Cnt", 1000)->Data(),
			XMFLOAT2(10, 370), Colors::MintCream);

		Platform::String^ m_fontStringTotalPointCount = nullptr;
		m_TotalPointCountFont->DrawString(m_sprites.get(),
			PointCountString(m_fontStringTotalPointCount, "#Pts", (unsigned int)m_iTotalPointCount)->Data(),
			XMFLOAT2(10, 450), Colors::MintCream);

		Platform::String^ m_fontStringFaceCount = nullptr;
		m_FaceCountFont->DrawString(m_sprites.get(),
			PointCountString(m_fontStringFaceCount, "#Faces", (unsigned int)indices.size() / 3)->Data(),
			XMFLOAT2(10, 550), Colors::MintCream);

		Platform::String^ m_fontStringGroupCount = nullptr;
		m_GroupCountFont->DrawString(m_sprites.get(),
			PointCountString(m_fontStringGroupCount, "#Groups", (unsigned int)m_iGroupCount)->Data(),
			XMFLOAT2(10, 650), Colors::MintCream);

		Platform::String^ m_fontStringSelectedPoint = nullptr;
		m_SelectedPointNumberFont->DrawString(m_sprites.get(),
			PointCountString(m_fontStringSelectedPoint, "Sel Pt", (unsigned int)m_SelIndexOne)->Data(),
			XMFLOAT2(10, 750), Colors::MintCream);
		/*
		Platform::String^ m_fontStringLineCount = nullptr;
		m_GroupCountFont->DrawString(m_sprites.get(),
			PointCountString(m_fontStringGroupCount, "#Lines", (unsigned int)m_iLineCount)->Data(),
			XMFLOAT2(540, 950), Colors::MintCream);
	
		const D3D12_VIEWPORT viewport = m_sceneDeviceResources->GetScreenViewport();

		m_sprites->SetViewport(viewport);

		m_sprites->SetRotation(m_sceneDeviceResources->GetRotation());
		DirectX::XMUINT2 texDim{ (uint32_t)viewport.Width, (uint32_t)viewport.Height };
		m_sprites->Draw(m_resourceDescriptors->GetGpuHandle((size_t)PBRDescriptors::PicTex), texDim, Colors::MintCream);
		*/
		m_sprites->End();
		
		PIXEndEvent(commandList);
	
}

XMVECTOR XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::RotateYawPitchPoint(XMVECTOR xAxis, float degreeYaw, float degreePitch)
{
	XMVECTOR vx = RotateYawPoint(xAxis, degreeYaw);
	XMFLOAT3 pox(XMVectorGetX(vx), XMVectorGetY(vx), XMVectorGetZ(vx));
	XMVECTOR vx1 = RotatePitchPoint(XMLoadFloat3(&pox), degreePitch);
	XMVECTOR axis1 = { XMVectorGetX(vx1), XMVectorGetY(vx1), XMVectorGetZ(vx1) };
	return axis1;
}

XMVECTOR XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::RotateYawPitchPoint(XMFLOAT3 point1, float degreeYaw, float degreePitch)
{
	XMVECTOR vx = RotateYawPoint(XMLoadFloat3(&point1), degreeYaw);
	XMFLOAT3 pox(XMVectorGetX(vx), XMVectorGetY(vx), XMVectorGetZ(vx));
	XMVECTOR vx1 = RotatePitchPoint(XMLoadFloat3(&pox), degreePitch);
	XMVECTOR axis = { XMVectorGetX(vx1), XMVectorGetY(vx1), XMVectorGetZ(vx1) };
	return axis;
}

XMVECTOR XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::RotateYawPitchPointXMV(XMVECTOR point1, float degreeYaw, float degreePitch)
{
	XMVECTOR vx = RotateYawPoint(point1, degreeYaw);
	XMFLOAT3 pox(XMVectorGetX(vx), XMVectorGetY(vx), XMVectorGetZ(vx));
	XMVECTOR vx1 = RotatePitchPoint(XMLoadFloat3(&pox), degreePitch);
	XMVECTOR axis1 = { XMVectorGetX(vx1), XMVectorGetY(vx1), XMVectorGetZ(vx1) };
	return axis1;
}

XMFLOAT3 XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::RotateYawPitchPointXMFL3(XMFLOAT3 point1, float degreeYaw, float degreePitch)
{
	XMVECTOR vx = RotateYawPoint(XMLoadFloat3(&point1), degreeYaw);
	XMFLOAT3 pox(XMVectorGetX(vx), XMVectorGetY(vx), XMVectorGetZ(vx));
	XMVECTOR vx1 = RotatePitchPoint(XMLoadFloat3(&pox), degreePitch);
	XMFLOAT3 axis = { XMVectorGetX(vx1), XMVectorGetY(vx1), XMVectorGetZ(vx1) };
	return axis;
}

XMVECTOR XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::RotatePitchPoint(XMVECTOR v1, float degree)
{
	XMFLOAT4 Feye = { m_EyeX, m_EyeY, m_EyeZ, 0.0f };
	//XM_2PI
	XMFLOAT4 Fup = { m_UpX, m_UpY, m_UpZ, 0.0f };
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(XMLoadFloat4(&Feye), XMLoadFloat4(&Fup)));
	XMMATRIX rotation = XMMatrixRotationAxis(right, degree);

	return XMVector3TransformCoord(v1, rotation);
}

XMVECTOR XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::RotateYawPoint(XMVECTOR v1, float degree)
{
	//XMFLOAT4 Feye = { m_EyeX, m_EyeY, m_EyeZ, 0.0f };
	//XMFLOAT4 Fat = { m_LookAtX, m_LookAtY, m_LookAtZ, 0.0f };
	XMFLOAT4 Fup = { m_UpX, m_UpY, m_UpZ, 0.0f };
	XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat4(&Fup), degree);
	return XMVector3TransformCoord(v1, rotation);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawGridXZ(FXMVECTOR xAxis, FXMVECTOR zAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
	DirectXPage^ xpg = m_vars->GetDXPage();//Kubed
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw grid");
	// Next 4 lines Kubed
	const XMVECTORF32 scale = { xpg->Get_xScaleDrawnObject(), xpg->Get_yScaleDrawnObject(), xpg->Get_zScaleDrawnObject() };
	const XMVECTORF32 translate = { xpg->Get_xTranslateDrawnObject(), xpg->Get_yTranslateDrawnObject(), xpg->Get_zTranslateDrawnObject() };
	XMVECTOR angles = { 0.0f, 0.0f, -xpg->Get_zRotateDrawnObject() };// { XM_PI / 2.f, 0.f, -XM_PI / 2.f };
	XMVECTOR rotate = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles);

	//DirectX::XMMATRIX localGridXZ = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f) * XMMatrixRotationX(-dxp->Get_xRotateDrawnObject()));
	DirectX::XMMATRIX localGridXZ = XMLoadFloat4x4(&m_world4x4) * XMMatrixTransformation(g_XMZero, DirectX::XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
	
	
	//DirectX::XMMATRIX localGridXZ = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	
	m_lineEffect->SetWorld(localGridXZ);
	m_lineEffect->Apply(commandList);
	m_batch->Begin(commandList);
	
	xdivs = std::max<size_t>(1, xdivs);
	ydivs = std::max<size_t>(1, ydivs);

	for (size_t i = 0; i <= xdivs; ++i)
	{
		float fPercent = float(i) / float(xdivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);
		if (i == 5)
		{
			GXMVECTOR color2 = XMVectorSet(1.0f, 0.5f, 0.0f, 1.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, zAxis), color2);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, zAxis), color2);
			m_batch->DrawLine(v1, v2);
		}
		else
		{
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, zAxis), color);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, zAxis), color);
			m_batch->DrawLine(v1, v2);
		}
    }

	for (size_t i = 0; i <= ydivs; i++)
	{
		float fPercent = float(i) / float(ydivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(zAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		if (i == 5)
		{
			GXMVECTOR color2 = XMVectorSet(0.0f, 0.0f, 0.5f, 1.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color2);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color2);
			m_batch->DrawLine(v1, v2);
		}
		else
		{
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
			m_batch->DrawLine(v1, v2);
		}
	}
	
	m_batch->End();
	
	PIXEndEvent(commandList);
}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateLinePointsTranslate(float xMove, float yMove, float zMove)
{
	m_iLineCount = static_cast<unsigned int>(m_PtGroupList.size());

	for (size_t j = 0; j < static_cast<size_t>(m_iLineCount); ++j)
	{
		unsigned int cnt = m_PtGroupList.at(j)->GetPtList()->Length;
		for (unsigned int i = 0; i < cnt; ++i)
		{
			DirectX::DXTKXAML12::VertexPositionColor v1 =
				vertices.at(m_PtGroupList.at(j)->GetListPt(i));

			v1.position.x += xMove;
			v1.position.y += yMove;
			v1.position.z += zMove;

			vertices.at(m_PtGroupList.at(j)->GetListPt(i)).position = v1.position;
		} // eofor(unsigned int i = 0;
	}
	m_vars->GetDXPage()->NotifyUser("Line Points Transormation Complete", NotifyType::StatusMessage);

	m_sceneDeviceResources->WaitForGpu();
	//m_loadingDrawnLineObjectComplete = true;
}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateAllLinesPointsTranslate(float xMove, float yMove, float zMove)
{
	m_iLineCount = static_cast<unsigned int>(m_PtGroupList.size());
	size_t j = static_cast<size_t>(m_iLineCount - 1);

	unsigned int cnt = m_PtGroupList.at(j)->GetPtList()->Length;

	for (unsigned int i = 0; i < cnt; ++i)
	{
		DirectX::DXTKXAML12::VertexPositionColor v1 =
			vertices.at(m_PtGroupList.at(j)->GetListPt(i));

		v1.position.x += xMove;
		v1.position.y += yMove;
		v1.position.z += zMove;

		vertices.at(m_PtGroupList.at(j)->GetListPt(i)).position = v1.position;
	} // eofor(unsigned int i = 0;

	m_vars->GetDXPage()->NotifyUser("Line Points Transormation Complete", NotifyType::StatusMessage);

	m_sceneDeviceResources->WaitForGpu();
	//m_loadingDrawnLineObjectComplete = true;

	//m_vars->GetDXPage()->NotifyUser("\nReady to draw = true" + m_iLineCount, NotifyType::StatusMessage);
}

void Hot3dxRotoDraw::RotoDrawSceneRender::ViewMatrix(XMFLOAT4X4 m, wchar_t* str)
{

	XMMatrixSet(m._11, m._12, m._13, m._14,
		        m._21, m._22, m._23, m._24,
		        m._31, m._32, m._33, m._34,
		        m._41, m._42, m._43, m._44
	);

	
	wchar_t* t = {L""};
	wchar_t* s = { L"\nThe Matrix values: \n%s\n%.6f  %.6f  %.6f %.6f\n%.6f  %.6f  %.6f %.6f\n%.6f  %.6f  %.6f %.6f\n%.6f  %.6f  %.6f %.6f\n" };
	StringCbPrintf(t, 1000, s, str, m._11, m._12, m._13, m._14,
		m._21, m._22, m._23, m._24,
		m._31, m._32, m._33, m._34,
		m._41, m._42, m._43, m._44 );
	
	
}

void Hot3dxRotoDraw::RotoDrawSceneRender::RotatePitch(float degree)
{
	XMStoreFloat4x4(&m_world4x4, DirectX::XMMatrixRotationX(float(m_timer.GetTotalSeconds() * degree))); 
	
}

void Hot3dxRotoDraw::RotoDrawSceneRender::RotateYaw(float degree)
{
	XMStoreFloat4x4(&m_world4x4, DirectX::XMMatrixRotationY(float(m_timer.GetTotalSeconds() * degree))); //XM_PIDIV4)));

}

void Hot3dxRotoDraw::RotoDrawSceneRender::RotatePitchSquid(float degree)
{
	DirectX::XMVECTOR m_Eye1 = DirectX::XMVectorSet(m_EyeX, m_EyeY, m_EyeZ, 0.0f);
	DirectX::XMVECTOR m_Up1 = DirectX::XMVectorSet(m_UpX, m_UpY, m_UpZ, 0.0f);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationAxis(m_Up1, degree);
	DirectX::XMVECTOR m_Eye = { DirectX::XMVector3TransformCoord(m_Eye1, rotation) };
	Setm_EyeX(DirectX::XMVectorGetX(m_Eye1));
	Setm_EyeY(DirectX::XMVectorGetY(m_Eye1));
	Setm_EyeZ(DirectX::XMVectorGetZ(m_Eye1));

	Setm_UpX(DirectX::XMVectorGetX(m_Up1));
	Setm_UpY(DirectX::XMVectorGetY(m_Up1));
	Setm_UpZ(DirectX::XMVectorGetZ(m_Up1));
}

void Hot3dxRotoDraw::RotoDrawSceneRender::RotateYawSquid(float degree)
{
	DirectX::XMVECTOR m_Eye = DirectX::XMVectorSet(m_EyeX, m_EyeY, m_EyeZ, 0.0f);
	DirectX::XMVECTOR m_Up = DirectX::XMVectorSet(m_UpX, m_UpY, m_UpZ, 0.0f);
	DirectX::XMVECTOR right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_Eye, m_Up));
	XMMATRIX rotation = DirectX::XMMatrixRotationAxis(right, degree);
	m_Eye = XMVector3TransformCoord(m_Eye, rotation);
	m_EyeX = XMVectorGetX(m_Eye);
	m_EyeY = XMVectorGetY(m_Eye);
	m_EyeZ = XMVectorGetZ(m_Eye);

	Setm_UpX(DirectX::XMVectorGetX(m_Up));
	Setm_UpY(DirectX::XMVectorGetY(m_Up));
	Setm_UpZ(DirectX::XMVectorGetZ(m_Up));
}


void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::GetUVPercent(float dimension, size_t count)
{
	textureU.clear();
	textureV.clear();
	textureU.resize(count);
	textureV.resize(count);
	
	float g = dimension / static_cast<float>(m_iGroupCount -1);
	float p = dimension / static_cast<float>(m_iPointCount -1);
	float pt = 0.0f;
	float gt = dimension;
	size_t k = 0;

	if (m_iEndPointOneCount == 1)
	{
		textureU.at(k) = gt;
		textureV.at(k) = pt;
		k++;
	}
	
	for (unsigned int i = 0; i < (m_iGroupCount); i++)
	{
		for (unsigned int j = 0; j < m_iPointCount; j++)
		{
			textureU.at(k) = gt;
			textureV.at(k) = pt;
			k++;
			pt = pt + p;
		}
		gt = gt - g;
		pt = 0.0f;
	}
	if (m_iEndPointTwoCount == 1)
	{
		textureU.at(k) = gt;
		textureV.at(k) = pt;
		k++;
	}
}

void Hot3dxRotoDraw::RotoDrawSceneRender::GetUVPercentMySphere(float dimension, unsigned int ptCount)
{
	if (ptCount < 1)
	{
		Platform::String^ message = ref new Platform::String(L"ERROR: GetUVPercentMySphere() Line 2386 There are no points to calculate texture coordinate");
		m_vars->GetDXPage()->NotifyUser("File Error Open " + message, NotifyType::ErrorMessage);
		return;
	}

	m_textureDimension = dimension;
	float g = m_textureDimension / static_cast<float>(m_iGroupCount - 1);
	float p = m_textureDimension / static_cast<float>(m_iPointCount - 1);
	float pt = 0.0f;
	float gt = m_textureDimension;
	textureU.clear();
	textureV.clear();
	textureU.resize(0);
	textureV.resize(0);
	textureU.resize(ptCount);
	textureV.resize(ptCount);
	size_t k = 0;
	if (m_iEndPointOneCount == 1)
	{
		textureU.at(k) = gt;
		textureV.at(k) = pt;
		k++;
	}

	// unsigned int tp = m_iTotalPointCount;
	for (unsigned int i = 0; i < (m_iGroupCount); i++)
	{

		unsigned int count = m_iPointCount;// m_vSphereGroupsSizes->get(i);
		for (unsigned int j = 0; j < count; j++)
		{
			p = m_textureDimension / static_cast<float>(count);
			textureU.at(k) = gt;
			textureV.at(k) = pt;
			k++;
			pt = pt + p;
		}
		gt = gt - g;
		pt = 0.0f;
	}
	if (m_iEndPointTwoCount == 1)
	{
		textureU.at(k) = gt;
		textureV.at(k) = pt;
		k++;
	}
}

float* XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::GetU(XMVECTOR v, Platform::Array<float>^ b)
{
	static float uv[2];

	float a = XMVectorGetX(v);
	float c = XMVectorGetY(v);
	
	XMVECTOR r;
	XMVECTOR m;
	XMVECTOR s;
	m = XMVectorSet(b->get(0), b->get(2), b->get(4), 0.0f); //b->min.x, b->min.y, b->min.z, 0.0f);
	s = XMVectorSet(b->get(1), b->get(3), b->get(5), 0.0f);//b->max.x, b->max.y, b->max.z, 0.0f);
	r=XMVectorSubtract(m, s);


	if (a == b->get(0))
	{
		uv[0] = 0.0F;
	}

	else {
		float x = XMVectorGetX(r);
		if(x<=0)
		{
			uv[0] = 1 * ((b->get(0) - a) / x);
		}
		else {
			uv[0] = 1 * ((a - b->get(0)) / x);
		}
		
	}
	if (c == b->get(3)){
		uv[1] = 0.0F;
	}
	else {
		uv[1] = 1 * ((c - b->get(3)) / XMVectorGetY(r));
		
	}
	
	return uv;
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawGridPicRectangle()
{
	m_bIsPlayer=false;
	m_sharedVideoTexture = nullptr;
	m_videoWidth = 0;
	m_videoHeight = 0;
	m_bDDS_WIC_FLAG1=true;
	m_bDDS_WIC_FLAG2 = true;
	m_bDDS_WIC_FLAGGridPic = false;
		
	m_bDDS_WIC_FLAGGridPicComplete = false;
	DirectX::DXTKXAML12::VertexCollection vCol;
	vCol.resize(0);
	vCol.resize(4);
	vCol.at(0)=(DirectX::DXTKXAML12::VertexPositionNormalTexture(XMFLOAT3(-20.0f, 20.0f, 0.0f), XMFLOAT3(0.000000f, 1.000000f, -0.000000f), XMFLOAT2(0.0000f, 0.0000f)));// , vPNT[3]);
	vCol.at(1)=(DirectX::DXTKXAML12::VertexPositionNormalTexture(XMFLOAT3(20.0f, 20.0f, 0.0f), XMFLOAT3(0.999894f, 0.000000f, -0.014547f), XMFLOAT2(1.00000f, 0.0000f)));
	vCol.at(2)=(DirectX::DXTKXAML12::VertexPositionNormalTexture(XMFLOAT3(20.0f, -20.0f, 0.0f), XMFLOAT3(0.000000f, 1.000000f, -0.000000f), XMFLOAT2(1.00000f, 1.00000f)));
	vCol.at(3)=(DirectX::DXTKXAML12::VertexPositionNormalTexture(XMFLOAT3(-20.0f, -20.0f, 0.0f), XMFLOAT3(0.014547f, 0.000000f, -0.999894f), XMFLOAT2(0.0000f, 1.000000f)));
	DirectX::DXTKXAML12::IndexCollectionColor vColColor = { 0,1,2,2,3,0 };
	m_shapeGridPic = GeometricPrimitive::CreateCustom(vCol, vColColor, m_sceneDeviceResources->GetD3DDevice());

	DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadGridPic = new DirectX::DXTKXAML12::ResourceUploadBatch(m_sceneDeviceResources->GetD3DDevice());
	m_resourceUploadGridPic->BeginXaml();

		LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadGridPic, m_textureImagePic1File->Data(), &m_texturePic2, GetMsgTypes(0), GetMessages(0));
	
	DirectX::DXTKXAML12::CreateShaderResourceView(m_sceneDeviceResources->GetD3DDevice(), m_texturePic2.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::GridPicTexture)), false);

	RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());

	{
		EffectPipelineStateDescription pd(
			&GeometricPrimitive::VertexType::InputLayout,
			CommonStates::Opaque,
			CommonStates::DepthDefault,
			CommonStates::CullNone,
			rtState);

		m_drawRectangleEffect = std::make_unique<BasicEffect>(m_sceneDeviceResources->GetD3DDevice(), EffectFlags::PerPixelLighting | EffectFlags::Texture, pd);
		m_drawRectangleEffect->EnableDefaultLighting();
		m_drawRectangleEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::GridPicTexture)), m_states->AnisotropicWrap());
	}

	auto loaded = m_resourceUploadGridPic->EndXaml(m_sceneDeviceResources->GetCommandQueue());
	WaitForSingleObject(m_resourceUploadGridPic->GetGPUHandle(), INFINITE);

	m_drawRectangleEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
	m_drawMode = m_drawMode == (int)RotoDrawDrawMode::DrawLineOnlyObject;
	m_sceneDeviceResources->WaitForGpu();
	loaded.then([this]()
	{
		m_bDDS_WIC_FLAGGridPicComplete = true;
		m_bDDS_WIC_FLAGGridPic = true;
	});
	m_drawMode = (int)RotoDrawDrawMode::DrawLineOnlyObject;
}

void Hot3dxRotoDraw::RotoDrawSceneRender::ScreenMouse3DWorldAlignment() const
{
	//TCHAR dest[500] = {};;
	//TCHAR* s = TEXT("%s\nW: %.6f H: %.6f \n");
	//TCHAR* t = TEXT("The ratios:");

	//StringCbPrintf(dest, 500, s, t, m_drawMouseWidthRatio, m_drawMouseHeightRatio);
	//OutputDebugString(dest);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::PointDataValues(unsigned int number, float x, float y, float z)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptCntStr = ref new Platform::String(std::to_wstring(number).c_str());
	Platform::String^ m_ptStrX = ref new Platform::String(std::to_wstring(x).c_str());
	Platform::String^ m_ptStrY = ref new Platform::String(std::to_wstring(y).c_str());
	Platform::String^ m_ptStrZ = ref new Platform::String(std::to_wstring(z).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, "Pt:");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptCntStr);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// x
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// y
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " ");// z
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::PointCountString(Platform::String^ m_fontString, Platform::String^ objectString, unsigned int cnt)
{
	m_fontString = ref new Platform::String();
	Platform::String^ m_fontStringCount = ref new Platform::String(std::to_wstring(cnt).c_str());
	m_fontString = m_fontString->Concat(m_fontString, objectString);
	m_fontString = m_fontString->Concat(m_fontString, ": \n");
	m_fontString = m_fontString->Concat(m_fontString, m_fontStringCount);
	return m_fontString;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::ObjectXYZPositionString(Platform::String^ m_fontString, Platform::String^ objectString, float x, float y, float z)
{
	m_fontString = ref new Platform::String();
	Platform::String^ m_fontStringX = ref new Platform::String(std::to_wstring(x).c_str());
	Platform::String^ m_fontStringY = ref new Platform::String(std::to_wstring(y).c_str());
	Platform::String^ m_fontStringZ = ref new Platform::String(std::to_wstring(z).c_str());
	m_fontString = m_fontString->Concat(m_fontString, objectString);
	m_fontString = m_fontString->Concat(m_fontString, " X: ");
	m_fontString = m_fontString->Concat(m_fontString, m_fontStringX);
	m_fontString = m_fontString->Concat(m_fontString, " Y: ");
	m_fontString = m_fontString->Concat(m_fontString, m_fontStringY);
	m_fontString = m_fontString->Concat(m_fontString, " Z: ");
	m_fontString = m_fontString->Concat(m_fontString, m_fontStringZ);
	return m_fontString;
}

inline HRESULT XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::LoadDDSOrWicTextureFile(_In_ ID3D12Device* device,
	DirectX::DXTKXAML12::ResourceUploadBatch& batch,
	_In_z_ const wchar_t* szfileName,
	_Outptr_ ID3D12Resource** textureResource, Platform::String^ msgType, Platform::String^ message)
{
	unsigned int tries = 0;
	HRESULT hr1 = DirectX::DXTKXAML12::CreateDDSTextureFromFile(device, batch, szfileName, textureResource);
	if (hr1 != S_OK)
	{   
		tries++;
		hr1 = DirectX::DXTKXAML12::CreateWICTextureFromFile(device, batch, szfileName, textureResource);
	}
	if (hr1 != S_OK)
	{
		msgType = L"Error Message: Directory Not Accessible  ";
		message = L"All textures must be chosen from the x64\\Release or Debug\\Hot3dxRotoDraw\\AppX\\Assets\\(Folder or sub-Folders \nPress the Clear Button\n1)Go To: Textures\n2) Add Texture1 from proper directory\nThe directory is in the status box lower left)";
		m_vars->GetDXPage()->NotifyUser("File Error Open " + message, NotifyType::ErrorMessage); 
		ClearDrawnObject();
		m_vars->GetDXPage()->NotifyUser("Opened file " + ref new Platform::String(szfileName), NotifyType::StatusMessage);
		m_vars->GetDXPage()->SetErrorMessagePopup(msgType, message);
	}

	else
	{
		m_vars->GetDXPage()->NotifyUser("Opened file " + ref new Platform::String(szfileName), NotifyType::StatusMessage);
		return hr1;

	}
	m_vars->GetDXPage()->NotifyUser("Opened file " + ref new Platform::String(szfileName), NotifyType::StatusMessage);
	return hr1;
}



XMMATRIX XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetXMMatrix(DirectX::XMFLOAT4X4 m, XMMATRIX xm)
{
	xm = XMMatrixSet(m._11, m._12, m._13, m._14,
		m._21, m._22, m._23, m._24,
		m._31, m._32, m._33, m._34,
		m._41, m._42, m._43, m._44
      );
	return xm;
}

XMMATRIX XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::GetXMMatrix(DirectX::XMFLOAT4X4 m)
{
	return XMMatrixSet(m._11, m._12, m._13, m._14,
		m._21, m._22, m._23, m._24,
		m._31, m._32, m._33, m._34,
		m._41, m._42, m._43, m._44
	);
}
#pragma endregion

#pragma region Message Handlers
#pragma endregion

void Hot3dxRotoDraw::RotoDrawSceneRender::CameraReset()
{
	m_fCameraDistance = 20.0f;
	m_EyeX = 0.0f; m_EyeY = 0.0f; m_EyeZ = m_fCameraDistance;
	if (m_IsLeftHanded) { m_EyeZ = m_fCameraDistance = -20.0f; }
	m_LookAtX = 0.0f; m_LookAtY = 0.01f; m_LookAtZ = 0.0f;
	m_UpX = 0.0f; m_UpY = 1.0f; m_UpZ = 0.0f;
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::ClearDrawnObject()
{
	m_drawMode = static_cast<int>(RotoDrawDrawMode::DrawLineOnlyObject);// :DrawSceneParts;// 0;
	
	CameraReset();
	m_loadingDrawnObjectComplete = false;
	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;

	m_iEndPointOneCount = 0;
	m_iEndPointTwoCount = 0;

	pos.clear();
	pos.resize(0);

	posX = ref new Platform::Array<float>(1000);
	posY = ref new Platform::Array<float>(1000);
	posZ = ref new Platform::Array<float>(1000);


	box = ref new Platform::Array<float>(6);

	m_iPointCount = 0;
	m_iTotalPointCount = 0;

	m_iTempGroup = ref new Platform::Array<unsigned int>(10000);
	m_iTempMouseX = ref new Platform::Array<float>(10000);
	m_iTempMouseY = ref new Platform::Array<float>(10000);
	m_iTempGroupCount = 0;
	
	m_PtGroupList.clear();
	m_PtGroupList.resize(0);
	m_iGroupCount = 0;

	m_vSphereGroupsSizes = ref new Platform::Array<unsigned int>(1000);
	faceCnt = 0;
	faceIndiceCnt = 0;
	//m_iLastPoint = 0;
	point = XMFLOAT2(0.0f, 0.0f);
	vertices.clear();
	vertices.resize(0);
	verticesDual.clear();
	verticesDual.resize(0);
	verticesPBR.clear();
	verticesPBR.resize(0);
	indices.clear();
	indices.resize(0);
	textureU.clear();
	textureU.resize(0);
	textureV.clear();
	textureV.resize(0);

	vertexes.clear();
	vertexes.resize(0);

	
	m_bIsPBRModel = false;
	m_bIsBasicModel = true;
	m_bIsDualTextureModel = false;
	m_bIsVideoTextureModel = false;
	m_bIsSculptWireframe = false;
	m_bIsBasicModelColor = false;
	m_bIsPlayer = false;
	if(m_player)
	m_player->Shutdown();
	m_sharedVideoTexture = 0;
	m_vars->GetDXPage()->SetEffectIndexRenderer(0);

		Scenario5_Tex^ sc5 = m_vars->GetDXPage()->m_Scene5TexVars->GetScenario5TexPage();
	m_vars->GetDXPage()->m_Scene5TexVars->SetGridChecked(true);
	m_vars->GetDXPage()->m_Scene5TexVars->SetPicChecked(false); 
	sc5->SetIDC_GRID_OR_PIC_CHECKED(true);
	sc5->SetIDC_PIC_CHECKED(false);
	
	Scenario2_Normal^ sc2 = m_vars->GetDXPage()->m_Scene2Vars->GetScenario2Page();
	sc2->Current->SetTopLeftCheckBoxFalse();
	sc2->Current->SetBottomRightCheckBoxFalse();
	sc2->Current->SetAxisChecked(true);
	m_vars->GetDXPage()->SetAxisCheckedDXP(true);
	m_vars->GetDXPage()->m_Scene2Vars->SetTopOrLeftChecked(false);
	m_vars->GetDXPage()->m_Scene2Vars->SetBottomOrRightChecked(false);
	m_vars->GetDXPage()->m_Scene2Vars->SetPartialRotateAngle(0.0f);
	sc2->Current->SetPartialSlider();
	m_vars->GetDXPage()->SetIfRightShiftKeyHeldDrawStraightLine(false);
	m_vars->GetDXPage()->SetIfLeftShiftKeyHeldDrawStraightLine(false);
	m_vars->GetDXPage()->SetIfLeftShiftKeyHeldDraw45Line(false);
	
	m_drawMode = static_cast<int>(RotoDrawDrawMode::DrawLineOnlyObject);
	m_loadingDrawnObjectComplete = false;
	m_bDDS_WIC_FLAG1 = true;
	m_bDDS_WIC_FLAG2 = true;
	m_bDDS_WIC_FLAGGridPic = false;
	m_bDDS_WIC_FLAGGridPicComplete = false;
	/*
	XMVECTORF32 xaxis = { 10.0f, 0.0f, 0.0f };
	XMVECTORF32 yaxis = { 0.0f, 10.0f, 0.0f };
	XMVECTORF32 zaxis = { 0.0f, 0.0f, 0.0f };
	DrawGridInit(xaxis, yaxis, zaxis, 10, 10, Colors::Crimson);
	*/
	
	m_drawMode = (int)RotoDrawDrawMode::DrawLineOnlyObject;
}

Platform::String^ Hot3dxRotoDraw::RotoDrawSceneRender::DrawnObjectOpenText()
{
	Platform::String^ file = ref new Platform::String(L"");
	
	return file;
}

Platform::String^ Hot3dxRotoDraw::RotoDrawSceneRender::DrawnObjectOpenBinary()
{
	Platform::String^ file = ref new Platform::String(L"");

	return file;
}

Platform::String^ Hot3dxRotoDraw::RotoDrawSceneRender::DrawnObjectSaveText(Platform::String^ fileName, unsigned int objectCount) const
{
	Hot3dxTextFileSave^ fileSave = ref new  Hot3dxTextFileSave(fileName, objectCount);
	Platform::String^ file = fileSave->DrawnObjectSaveHot3dxTextFile(vertexes, indices, m_textureImage1File, 1, nullptr);
	
	return file;
}

Platform::String^ Hot3dxRotoDraw::RotoDrawSceneRender::DrawnObjectSaveBinary()
{
	Platform::String^ file = ref new Platform::String(L"");
	return file;
}

// Write Obj .mtl file
Platform::String^ Hot3dxRotoDraw::RotoDrawSceneRender::DrawnObjectSaveObj3DandMtl(
	Platform::String^ effectName,
	unsigned int illumType,
	Platform::String^ mtlObjFilename,
	Platform::String^ textureFilename)
{
	illumType = 2; 
	textureFilename =
		GetTexture1Name();
	Platform::String^ file = ref new Platform::String(L"");
	Obj3DMaterialFile^ f = ref new Obj3DMaterialFile(L"Scene1", 1);
	Scenario5Vars^ vars = m_vars->GetDXPage()->m_Scene5Vars;
	unsigned int length = static_cast<unsigned int>(vars->GetMaterialGroupsData()->GetMaterialDataVector().size());
	
	for (unsigned int MaterialDataIndex = 0; MaterialDataIndex < length; MaterialDataIndex++) {
		file = file->Concat(file, f->WriteObj3DMaterialHeaderData(MaterialDataIndex, mtlObjFilename));
		file = file->Concat(file, f->WriteObj3DMaterialListData(effectName, illumType, mtlObjFilename, L"", textureFilename, MaterialDataIndex, vars->GetMaterialGroupsData()));
	}
	return file;

}

// Write .obj & .mtl file

Platform::String^ Hot3dxRotoDraw::RotoDrawSceneRender::DrawnObjectSaveObjFile(
	Platform::String^ mtlObjFilename,
	Platform::String^ nodeName,
	Platform::String^ effectName
) const
{
	Platform::String^ file = ref new Platform::String(L"");
	Obj3DFile^ f = ref new Obj3DFile(nodeName, 1);
	file = file->Concat(file, f->DrawnObjectSaveObjFile(m_iPointCount, m_iGroupCount, vertexes, indices, mtlObjFilename, mtlObjFilename, effectName));
	return file;
}

