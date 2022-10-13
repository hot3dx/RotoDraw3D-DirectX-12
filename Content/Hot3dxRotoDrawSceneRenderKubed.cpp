//--------------------------------------------------------------------------------------
// File: Maker2SceneRender.cpp
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
#include <Graphics\RenderTargetStateXaml12.h>
#include <Graphics\EffectPipelineStateDescriptionXaml12.h>
#include <Graphics\CommonStatesXaml12.h>
#include <Graphics\GraphicsMemoryXaml12.h>
#include <Graphics\VertexTypesXaml12.h>
#include <Graphics\MyResourceUploadBatchXaml12.h>
#include <Graphics\GeometricPrimitiveXaml12.h>
#include <Graphics\GeometryXaml12.h>
#include <Graphics\EffectsXaml12.h>
#include <Graphics\EffectCommonXaml12.h>
#include <Graphics\DDSTextureLoaderXaml12.h>
#include <Graphics\WICTextureLoaderXaml12.h>
#include "FilesSave\ObjMaterialFile.h"
#include "FilesSave\Obj3DFile.h"
#include "FilesSave\Hot3dxTextFileSave.h"

using namespace Hot3dxRotoDraw;
using namespace Hot3dx;

using namespace DX;
using namespace Concurrency;
using namespace DirectX;
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


//static CUSTOMVERTEX* vertices;
Hot3dxRotoDraw::PtGroups::PtGroups() {}

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Maker2SceneRender::Maker2SceneRender(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_vars(),
	m_fCameraDistance(20.0f),
	m_EyeX(0.0f), m_EyeY(0.0f), m_EyeZ(m_fCameraDistance),
	m_LookAtX(0.0f), m_LookAtY(0.01f), m_LookAtZ(0.0f),
	m_UpX(0.0f), m_UpY(1.0f), m_UpZ(0.0f),
	m_posX(0.0f), m_posY(0.0f), m_posZ(0.0f),
	m_fPointSpace(0.30000f),
	m_fScrollDist(0.300000f),
	m_iScrollPointSetPos(150),
	m_bMouse3dPosDist(false),
	m_iPointCount(0),
	m_iTotalPointCount(0),
	m_iGroupCount(0),
	m_drawMouseWidthRatio(0.0212f),//(0.033000f),// fullscreen(0.0254000f),
	m_drawMouseHeightRatio(0.02641f),//(0.038100f),// fullscreen(0.0254000f),(0.039000f),
	m_loadingComplete(false),
	m_newAndClearComplete(false),
	m_loadingDrawnObjectComplete(false),
	m_performScaleDrawnObjectComplete(false),
	m_radiansPerSecond(XM_PIDIV4 / 2),	// rotate 45 degrees per second
	m_angle(0.01f),
	m_tracking(false),
	sceneVertexCount(8),
	m_sceneDeviceResources(deviceResources),
	m_drawMode((int)Maker2DrawMode::DrawLineOnlyObject),//0
	m_bIsYAxis(true),
	m_bRotateKeyPressed(false),
	m_previousPosX(0.0f),
	m_previousPosY(0.0f),
	m_previousPosZ(0.0f),
	m_SelIndexOne(0),
	m_IsLeftHanded(true),
	m_textureImage1File(L"Assets\\Textures\\Marble.dds"),
	m_textureImage2File(L"Assets\\Textures\\nightceiling.dds"),
	m_iEndPointOneCount(0),
    m_iEndPointTwoCount(0),
	m_bDDS_WIC_FLAG1(false),
	m_bDDS_WIC_FLAG2(true),
	m_bDDS_WIC_FLAGGridPic(false),
	m_bDDS_WIC_FLAGGridPicComplete(false)
{
	if (m_IsLeftHanded) { m_EyeZ = -m_fCameraDistance; }

	LoadState();
	ZeroMemory(&m_constantBufferData, sizeof(m_constantBufferData));

	m_camera = ref new Hot3dxCamera();

	m_audioController = ref new Audio;
	m_audioController->CreateDeviceIndependentResources();

	m_vars = ref new Hot3dxRotoDrawVariables();

	
	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

Maker2SceneRender::~Maker2SceneRender()
{
	m_loadingComplete = false;
	m_loadingDrawnObjectComplete = false;
}

void Maker2SceneRender::CreateDeviceDependentResources()
{

	if (m_loadingComplete == false)
	{


		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{

			return;
		}

		mediaReader = ref new MediaReader();
		auto targetHitSound = mediaReader->LoadMedia("Assets\\hit.wav");

		m_vars = ref new Hot3dxRotoDrawVariables();
		m_resourceUpload = std::make_unique<DirectX::DXTKXAML12::ResourceUploadBatch>(device);
		m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

		m_states = std::make_unique<CommonStates>(device);

		m_resourceDescriptors = std::make_unique<DescriptorHeap>(device,
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
			size_t(Descriptors::Count));

		m_batch = std::make_unique<DirectX::DXTKXAML12::PrimitiveBatch<DirectX::DXTKXAML12::VertexPositionColor>>(device);

		m_shapeTetra = GeometricPrimitive::CreateTetrahedron(0.5f);
		// SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
		//   // \\Assets\\C:\\Users\\hot3dx-home\\Source\\Hot3dxRotoDraw\\x64\\Debug\\Hot3dxRotoDraw\\AppX\\...
		{
            // Begin Resource Upload
			m_resourceUpload->BeginXaml();
			
			// C:\\Users\\hot3dx-home\\Source\\Hot3dxRotoDraw\\x64\\Debug\\Hot3dxRotoDraw\\AppX\\

			DX::ThrowIfFailed(
				DirectX::DXTKXAML12::CreateDDSTextureFromFile(device, *m_resourceUpload, L"Assets\\seafloor.dds", &m_texture1)//.ReleaseAndGetAddressOf())
			);

			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_texture1.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::SeaFloor)), false);
			/*
			DX::ThrowIfFailed(
				CreateWICTextureFromFile(device, *m_resourceUpload, m_textureImage2File->Data(), &m_texture2)//.ReleaseAndGetAddressOf())
			);

			CreateShaderResourceView(device, m_texture2.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::BackTexture)), false);
			*/
			RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());
			// Each effect object must ne proceeded by its own 
			// EffectPipelineStateDescription pd even if the EffectPipelineStateDescription pd is the same
			{
				EffectPipelineStateDescription pd(
					&DirectX::DXTKXAML12::VertexPositionColor::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthNone,
					CommonStates::CullNone,
					rtState,
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

				m_lineEffect = std::make_unique<BasicEffect>(device, EffectFlags::VertexColor, pd);
			}
			
			{
				EffectPipelineStateDescription cd(
					&DirectX::DXTKXAML12::VertexPositionColor::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthNone,
					CommonStates::CullNone,
					rtState,
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

				m_cursorEffect = std::make_unique<BasicEffect>(device, EffectFlags::VertexColor, cd);
			}
			
			{			
				EffectPipelineStateDescription pd(
					&GeometricPrimitive::VertexType::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					CommonStates::CullNone,
					rtState);

				m_shapeTetraEffect = std::make_unique<BasicEffect>(device, EffectFlags::PerPixelLighting | EffectFlags::Texture, pd);
				m_shapeTetraEffect->EnableDefaultLighting();
				m_shapeTetraEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::SeaFloor)), m_states->AnisotropicWrap());
			}

			{
				SpriteBatchPipelineStateDescription pd(rtState);

				m_sprites = std::make_unique<SpriteBatch>(device, *m_resourceUpload, pd);
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

			 m_PointCountFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
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

			//HANDLE completeEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
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

		}
	} // eo ! m_loadingComplete = false
}

// Initializes view parameters when the window size changes.
void Maker2SceneRender::CreateWindowSizeDependentResources()
{
	Size outputSize = m_sceneDeviceResources->GetOutputSize();
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
void Maker2SceneRender::Update(DX::StepTimer const& timer)
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
			m_drawRectangleEffect->SetView(XMLoadFloat4x4(&m_view4x4));
		}

		if (m_loadingDrawnObjectComplete)
		{
			m_shapeDrawnObjectEffect->SetView(XMLoadFloat4x4(&m_view4x4));
			m_shapeDrawnObjectEffect->SetWorld(DirectX::XMMatrixIdentity());
		}
		m_sprites->SetViewport(m_sceneDeviceResources->GetScreenViewport());
		

		PIXEndEvent();
	}
}

// Saves the current state of the renderer.
void Maker2SceneRender::SaveState()
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
void Maker2SceneRender::LoadState()
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
void Maker2SceneRender::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader.
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void Maker2SceneRender::StartTracking()
{
	m_tracking = true;
}

BOOL Maker2SceneRender::checkDistance(float x, float y, float z, float mouseMoveDistDelta)
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
		/*
		wchar_t buf[100];
		swprintf_s(buf, L"\n%.6f %.6f %.6f", xx, yy, mouseMoveDistDelta);
		OutputDebugString(buf);
		*/
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
void Maker2SceneRender::TrackingUpdate(float positionX, float positionY)
{
	point.x = positionX;
	point.y = positionY;
	MouseCursorRender(positionX, positionY);
}

void Maker2SceneRender::StopTracking()
{
	m_tracking = false;
}

void Hot3dxRotoDraw::Maker2SceneRender::ReleaseDeviceDependentResources()
{
	
}

void Hot3dxRotoDraw::Maker2SceneRender::OnDeviceLost()
{
		m_bDDS_WIC_FLAGGridPicComplete = false;
		m_bDDS_WIC_FLAGGridPic = false;

		m_texture1.Reset();
		m_texture2.Reset();
		m_textureGridPic.Reset();

		m_MousePosFont.reset();
		m_CameraEyeFont.reset();
		m_CameraAtFont.reset();
		m_CameraUpFont.reset();
		m_PointCountFont.reset();
		m_TotalPointCountFont.reset();
		m_FaceCountFont.reset();
		m_GroupCountFont.reset();
		m_SelectedPointNumberFont.reset();

		m_batch.reset();
		//m_shape.reset();
		m_shapeTetra.reset();
		m_shapeGridPic.reset();
		m_shapeDrawnObjectTex.reset();
		//m_model.reset();

		m_lineEffect.reset();

		m_cursorEffect.reset();

		//m_shapeEffect.reset();
		m_shapeTetraEffect.reset();
		m_drawRectangleEffect.reset();
		m_shapeDrawnObjectEffect.reset();
		//m_modelEffects.clear();
		//m_modelResources.reset();
		m_sprites.reset();
		m_resourceDescriptors.reset();
		m_states.reset();
		m_graphicsMemory.reset();

		pos.clear();
		pos.resize(0);

		posX = ref new Platform::Array<float>(1000);
		posY = ref new Platform::Array<float>(1000);
		posZ = ref new Platform::Array<float>(1000);
		box = ref new Platform::Array<float>(6);

		m_iPointCount = 0;
		m_iTotalPointCount = 0;

		m_iTempGroup = ref new Platform::Array<unsigned int>(1000);
		m_iTempMouseX = ref new Platform::Array<float>(1000);
		m_iTempMouseY = ref new Platform::Array<float>(1000);
		m_iTempGroupCount = 0;

		m_PtGroupList.~vector();

		vertices.~vector();
		vertexes.~vector();
		indices.~vector();
		textureU.~vector();
		textureV.~vector();
		//m_textureImage1File = nullptr;
		//m_textureImage1File = ref new Platform::String();
		//m_textureImage2File = nullptr;
		//m_textureImage2File = ref new Platform::String();
		m_loadingComplete = false;
		m_drawMode = (int)Maker2DrawMode::DrawLineOnlyObject;// :DrawSceneParts;// 0;
}

void Hot3dxRotoDraw::Maker2SceneRender::OnDeviceRestored()
{
	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::MouseCursorRender(float positionX, float positionY)
{
	if (m_tracking)
	{
		if (m_loadingComplete)
		{
			D3D12_VIEWPORT rect = m_sceneDeviceResources->GetScreenViewport();

			XMVECTOR xAxis = XMVectorSet(0.0f, 50.0f, 0.0f, 0.0f);
			XMVECTOR yAxis = XMVectorSet(50.0f, -50.0f, 0.0f, 0.0f);
			XMVECTOR zAxis = XMVectorSet(-50.0f, -50.0f, 0.0f, 0.0f);
			// convert mouse points to number line 
			// plus/ minus coordinates
			// and convert to float
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			int centerx;
			int centery;
			// convert mouse points to number line 
			// plus/ minus coordinates
			// and convert to float

			if (positionX > 0 || point.x < (rect.TopLeftX - rect.Width)
				|| point.y > 0 || point.y < (rect.TopLeftY - rect.Height))
			{
				centerx = (int)(rect.Width - rect.TopLeftX) / 2;
				centery = (int)(rect.Height - rect.TopLeftY) / 2;
				if (m_IsLeftHanded)
				{
					x = ((float)((point.x - centerx) * m_drawMouseWidthRatio));// for left handed minus sign on x
				}
				else
				{
					x = -((float)((point.x - centerx) * m_drawMouseWidthRatio));// for right handed remove minus sign on x

				}// eo if (m_IsLeftHanded)
				y = -((float)((point.y - centery) * m_drawMouseHeightRatio));

				XMVECTOR xx, yy;// , zz;
				xx = XMVectorScale(xAxis, x);
				yy = XMVectorScale(yAxis, y);
				XMVECTOR mouse3dPos = XMVectorSet((XMVectorGetX(yy) - XMVectorGetX(xx)) + m_LookAtX, (XMVectorGetY(yy) - XMVectorGetY(xx)) + m_LookAtY, (XMVectorGetZ(yy) - XMVectorGetZ(xx)) + m_LookAtZ, 0.0f);

					float radians = XM_2PI * 2.0f * positionX / m_sceneDeviceResources->GetOutputSize().Width;
					Rotate(radians);


					XMFLOAT3 intersect3 = {};
					//CHot3dxD3D12Geometry^ geo = ref new CHot3dxD3D12Geometry();

					XMVECTOR a = XMVectorSet(50.0f, 50.0f, 0.0f, 0.0f);
					XMVECTOR b = XMVectorSet(50.0f, -50.0f, 0.0f, 0.0f);
					XMVECTOR c = XMVectorSet(-50.0f, -50.0f, 0.0f, 0.0f);

					XMVECTOR eye = XMVectorSet(m_EyeX, m_EyeY, m_EyeZ, 0.0f);
					XMVECTOR look = XMVectorSet(m_LookAtZ, m_LookAtY, m_LookAtZ, 0.0f);
					//double camToFaceCenterDist = geo->distanceBetweenPoints(XMFLOAT3(m_EyeX, m_EyeY, m_EyeZ), XMFLOAT3(m_LookAtZ, m_LookAtY, m_LookAtZ));
					XMFLOAT3 dir = XMFLOAT3(m_hot3dxGeo->directionBetweenPoints(XMFLOAT3(m_EyeX, m_EyeY, m_EyeZ), XMFLOAT3(m_LookAtZ, m_LookAtY, m_LookAtZ)));
					XMVECTOR lineDirection = XMVectorSet(dir.x * 1000.f, dir.y * 1000.f, dir.z * 1000.f, 0.0f);
					//XMVECTOR lineDirection = XMVectorSet(m_LookAtZ, m_LookAtY, m_LookAtZ, 0.0f);
					XMVECTOR planeNormal = m_hot3dxGeo->FindPlaneNormalVec(a, b, c);
					double planeConstant = m_hot3dxGeo->FindPlaneConstantVec(planeNormal, a);
					XMVECTOR intersect = m_hot3dxGeo->FindPointIntersectPlaneVec(planeNormal, eye, lineDirection, (float)planeConstant); // -(m_fCameraDistance));
					XMFLOAT3 isect = {};
					XMStoreFloat3(&isect, intersect);
					
						point.x = x;
						point.y = y;
						z = dir.z;
						XMVectorSetX(mouse3dPos, XMVectorGetX(intersect));
						XMVectorSetY(mouse3dPos, XMVectorGetX(intersect));
						XMVectorSetZ(mouse3dPos, z);
						m_posX = XMVectorGetX(mouse3dPos);
						m_posY = XMVectorGetY(mouse3dPos);
						m_posZ = XMVectorGetZ(mouse3dPos);
				   
								
			}
	    }
	}// eo m_tracking if
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawPointsOne(XMFLOAT3 cursorPos)
{
	
	if (m_tracking)
	{
		if (m_bMouse3dPosDist)
		{
			if (m_iPointCount > 0)
			{
				if (m_vars->GetDXPage()->GetIfRightShiftKeyHeldDrawStraightLine())
				{
					cursorPos.y = m_previousPosY;
				}
				else if (m_vars->GetDXPage()->GetIfLeftShiftKeyHeldDrawStraightLine())
				{
					cursorPos.x = m_previousPosX;
				}
				

				else if (m_vars->GetDXPage()->GetIfLeftShiftKeyHeldDraw45Line())
				{
					if (cursorPos.x - m_previousPosX > 0)
					{
						cursorPos.x = m_previousPosX + (m_fPointSpace * sqrtf(2.0f));
					}
					else if (cursorPos.x - m_previousPosX < 0)
					{
						cursorPos.x = m_previousPosX - (m_fPointSpace * sqrtf(2.0f));
					}

					if (cursorPos.y - m_previousPosY > 0)
					{
						cursorPos.y = m_previousPosY + (m_fPointSpace * sqrtf(2.0f));
					}
					else if (cursorPos.y - m_previousPosY < 0)
					{
						cursorPos.y = m_previousPosY - (m_fPointSpace * sqrtf(2.0f));
					}
				} // if (m_vars->GetDXPage()->GetIfLeftShiftKeyHeldDraw45Line())
			}// if (m_iPointCount > 0)

			if (!posX || posX->Length < 1000)
			{
				posX = ref new Platform::Array<float>(1000);
			}
			if (!posY || posY->Length < 1000)
			{
				posY = ref new Platform::Array<float>(1000);
			}
			if (!posZ || posZ->Length < 1000)
			{
				posZ = ref new Platform::Array<float>(1000);
			}

			if (!m_iTempMouseX || m_iTempMouseX->Length < 1000)
			{
				m_iTempMouseX = ref new Platform::Array<float>(1000);
			}
			if (!m_iTempMouseY || m_iTempMouseY->Length < 1000)
			{
				this->m_iTempMouseY = ref new Platform::Array<float>(1000);
			}

			posX[m_iPointCount] = cursorPos.x;// posCursor.x;
			posY[m_iPointCount] = cursorPos.y;// posCursor.y;
			posZ[m_iPointCount] = cursorPos.z;// posCursor.z;
			m_previousPosX = cursorPos.x;
			m_previousPosY = cursorPos.y;
			m_previousPosZ = cursorPos.z;

			m_iTempGroup[m_iTempGroupCount] = m_iPointCount;
			m_iTempMouseX[m_iTempGroupCount] = point.x;
			m_iTempMouseY[m_iTempGroupCount] = point.y;
			m_iTempGroupCount++;
			m_iPointCount++;
		}
	} // eo if (m_tracking)
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::CopyFlipPointsXAxis()
{
	unsigned int count = m_iPointCount;
	unsigned int j = count - 1;
	//m_iPointCount++;
	for (unsigned int i = 0; i < count; i++)
	{
		posX[m_iPointCount] = posX[j];// posCursor.x;
		posY[m_iPointCount] = -posY[j];// posCursor.y;
		posZ[m_iPointCount] = posZ[j];// posCursor.z;
		m_previousPosX = posX[j];
		m_previousPosY = -posY[j];
		m_previousPosZ = posZ[j];

		m_iTempGroup[m_iTempGroupCount] = m_iPointCount;
		m_iTempMouseX[m_iTempGroupCount] = m_iTempMouseX[j];
		m_iTempMouseY[m_iTempGroupCount] = m_iTempMouseY[j];
		m_iTempGroupCount++;
		m_iPointCount++;
		j--;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::CopyFlipPointsYAxis()
{
	//m_iTempGroupCount++;
	unsigned int count = m_iPointCount;
	unsigned int j = count - 1;
	//m_iPointCount++;
	for (unsigned int i = 0; i < count; i++)
	{
		posX[m_iPointCount] = -posX[j];// posCursor.x;
		posY[m_iPointCount] = posY[j];// posCursor.y;
		posZ[m_iPointCount] = posZ[j];// posCursor.z;
		m_previousPosX = -posX[j];
		m_previousPosY = posY[j];
		m_previousPosZ = posZ[j];

		m_iTempGroup[m_iTempGroupCount] = m_iPointCount;
		m_iTempMouseX[m_iTempGroupCount] = m_iTempMouseX[j];
		m_iTempMouseY[m_iTempGroupCount] = m_iTempMouseY[j];
		m_iTempGroupCount++;
		m_iPointCount++;
		j--;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::EndTopPointYAxis()
{
	if (m_iPointCount > 0)
	{
		XMFLOAT3 m_topEndPoint = XMFLOAT3(0.0f, posY->get(0), 0.0f);
		
		posX->set(0, m_topEndPoint.x);// posCursor.x;
		posY->set(0, m_topEndPoint.y);// posCursor.y;
		posZ->set(0, m_topEndPoint.z);// posCursor.z;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::EndBottomPointYAxis()
{
	if (m_iPointCount > 0)
	{
		XMFLOAT3 m_bottomEndPoint = XMFLOAT3(0.0f, posY->get(m_iPointCount - 1), 0.0f);
		
		posX->set(m_iPointCount - 1, m_bottomEndPoint.x);// posCursor.x;
		posY->set(m_iPointCount - 1, m_bottomEndPoint.y);// posCursor.y;
		posZ->set(m_iPointCount - 1, m_bottomEndPoint.z);// posCursor.z;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::EndLeftPointsXAxis()
{
	if (m_iPointCount > 0)
	{
		XMFLOAT3 m_leftEndPoint = XMFLOAT3(posX->get(0), 0.0f, 0.0f);

		posX->set(0, m_leftEndPoint.x);// posCursor.x;
		posY->set(0, m_leftEndPoint.y);// posCursor.y;
		posZ->set(0, m_leftEndPoint.z);// posCursor.z;
	}
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::EndRightPointsXAxis()
{
	if (m_iPointCount > 0)
	{
		XMFLOAT3 m_rightEndPoint = XMFLOAT3(posX->get(m_iPointCount - 1), 0.0f, 0.0f);

		posX->set(m_iPointCount - 1, m_rightEndPoint.x);// posCursor.x;
		posY->set(m_iPointCount - 1, m_rightEndPoint.y);// posCursor.y;
		posZ->set(m_iPointCount - 1, m_rightEndPoint.z);// posCursor.z;
	}
}

void Hot3dxRotoDraw::Maker2SceneRender::OnLButtonDown(UINT nFlags, XMFLOAT2 apoint)
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

void Hot3dxRotoDraw::Maker2SceneRender::OnRightButtonDown(UINT nFlags, XMFLOAT2 apoint)
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

void Hot3dxRotoDraw::Maker2SceneRender::OnMouseMove(UINT nFlags, XMFLOAT2 apoint)
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

// Copies a Group of DirectX::DXTKXAML12::VertexPositionColor points
void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::Copy()
{
	unsigned int i = 0;

	m_GroupListSelectedIndex = i;
	DirectX::DXTKXAML12::VertexPositionColor vpc{}; vpc.position = XMFLOAT3(0, 0, 0); vpc.color = XMFLOAT4(0, 0, 0, 1);
	unsigned int sz = m_iPointCount - 1;
	//unsigned int len = m_PtGroupList.at(i)->GetPtList()->Length - 1;
	XMFLOAT3* vs = (XMFLOAT3*)malloc(sz * sizeof(XMFLOAT3));
	
	if (vs != NULL)
	{
		for (unsigned int j = 0; j < sz; j++)
		{
			XMFLOAT3 v = vpc.position;
			DWORD n = m_PtGroupList.at(i)->GetPtList()->Data[j];
			vertices.push_back(vpc);
			vertices.at(n).position.x = vpc.position.x = v.x;// push_back(n, &v.x);
			vertices.at(n).position.y = vpc.position.y = v.y;     //pop(n, &v.y);
			vertices.at(n).position.z = vpc.position.z = v.z;// pop(n, &v.z);
			XMVECTOR t = { 0.0f,0.0f,0.0f,0.0f };
			XMFLOAT4 txm3 = {};
			XMStoreFloat4(&txm3, t);
			
			//XMVector3InverseRotate(t, XMVectorSet(0.0f, v.y, 0.0f, 0.0f));
			//XMMatrixInverse(&t, XMLoadFloat4x4(&m_projection4x4));
			//XMVector3Transform(t, XMLoadFloat4x4(&m_projection4x4));
			
			XMMATRIX Transform = XMMatrixMultiply(XMLoadFloat4x4(&m_world4x4), XMLoadFloat4x4(&m_view4x4));
			Transform = XMMatrixMultiply( Transform, XMLoadFloat4x4(&m_projection4x4));
			//Transform = XMMatrixInverse(&t, Transform);
			Transform = XMMatrixInverse(&t/*&XMLoadFloat4(&txm3)*/, Transform);

			vs[j] = {XMVectorGetX(t), XMVectorGetY(t), XMVectorGetZ(t)};
			
		}//eojfor

		InitDimensionsBox();
		CreateDimensions(vs, sz);
		MakeBox(vs, sz, box);

		if (sz > 0)free(vs);
	}
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::InitDimensionsBox()
{
	box[0] = 0.0f;
	box[1] = 0.0f;
	box[2] = 0.0f;
	box[3] = 0.0f;
	box[4] = 0.0f;
	box[5] = 0.0f;
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::CreateDimensions(XMFLOAT3* copier, size_t qCount)
{
	for (size_t i = 0; i < qCount; i++)
	{
		if (copier[i].x < box[0]) { box[0] = copier[i].x; }
		if (copier[i].x > box[1]) { box[1] = copier[i].x; }
		if (copier[i].y < box[2]) { box[2] = copier[i].y; }
		if (copier[i].y > box[3]) { box[3] = copier[i].y; }
		if (copier[i].z < box[4]) { box[4] = copier[i].z; }
		if (copier[i].z > box[5]) { box[5] = copier[i].z; }
	}

}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::MakeBox(XMFLOAT3* copier, int qCount, Platform::Array<float>^ b)
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

	XMFLOAT3 avCubeVertices[] = { XMFLOAT3(box[0], box[2], box[4]),  XMFLOAT3(box[0], box[2],  box[5]),
									   XMFLOAT3(box[0],  box[3], box[4]),  XMFLOAT3(box[0],  box[3],  box[5]),
									   XMFLOAT3(box[1], box[2], box[4]),   XMFLOAT3(box[1], box[2],  box[5]),
									   XMFLOAT3(box[1],  box[3], box[4]),  XMFLOAT3(box[1],  box[3],  box[5]) };
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

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::SetPoints()
{
	
	// Register our SwapChainPanel to get independent input pointer events
	auto drawItemHandler = ref new WorkItemHandler([this](IAsyncAction^ action)
	{
		//if(action->Status == Windows::Foundation::AsyncStatus::Started)
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
				DrawObjectPoints();
				CalculateMeshFaces();
			}
		}
		});
	
	// Run task on a dedicated high priority background thread.
	m_drawObjectWorker = ThreadPool::RunAsync(drawItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
	
}

// Renders one frame using the vertex and pixel shaders.
bool Maker2SceneRender::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return false;
	}

	// Prepare the command list to render a new frame.
	m_sceneDeviceResources->Prepare();
	Clear();
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

	if (m_bDDS_WIC_FLAGGridPic == true && m_bDDS_WIC_FLAGGridPicComplete == true)
	{
		// Draw WIC PIC
		DirectX::XMMATRIX localGridPic = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f));
		// *XMMatrixRotationRollPitchYawFromVector(angles));
		m_drawRectangleEffect->SetWorld(localGridPic);
		m_drawRectangleEffect->Apply(commandList);
		m_shapeGridPic->Draw(commandList);
		// EO Draw WIC PIC
	}
	
	// Draw procedurally generated XZ dynamic grid
	//const XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
	//const XMVECTORF32 yaxis = { 0.f, 0.f, 20.f };
	//DrawGrid(xaxis, yaxis, g_XMZero, 20, 20, Colors::Gray);
	const XMVECTORF32 xaxis = { 10.f, 0.f, 0.f };
	const XMVECTORF32 yaxis = { 0.f, 0.f, 0.f };
	const XMVECTORF32 zaxis = { 0.f, 0.f, 10.f };
	DrawGridXZ(xaxis, zaxis, yaxis, 10, 10, Colors::SaddleBrown);
	// Draw procedurally generated XY dynamic grid
	const XMVECTORF32 xaxis1 = { 10.f, 0.f, 0.f };
	const XMVECTORF32 yaxis1 = { 0.f, 10.f, 0.f };
	const XMVECTORF32 zaxis1 = { 0.f, 0.f, 0.f };
	DrawGridXY(xaxis1, yaxis1, zaxis1, 10, 10, Colors::Crimson);
	// Draw procedurally generated XY Cursor
	const XMVECTORF32 xaxis12 = { 10.f, 0.f, 0.f };
	const XMVECTORF32 yaxis12 = { 0.f, 10.f, 0.f };
	const XMVECTORF32 zaxis12 = { 0.f, 0.f, 0.f };
	if(m_drawMode == (int)Maker2DrawMode::DrawSceneParts || m_drawMode == (int)Maker2DrawMode::DrawLineOnlyObject)// 0;
	Draw3DCursorXY(xaxis12, yaxis12, zaxis12, XMFLOAT3(m_posX, m_posY, m_posZ));
	
	// Draw the Initial Line
	if (m_drawMode == (int)Maker2DrawMode::DrawLineOnlyObject)//7
	{
		Windows::UI::Color c = m_vars->GetDXPage()->GetPointColorDXP();
		float a = c.A * RGBA_FRACTAL;
		float r = c.R * RGBA_FRACTAL;
		float g = c.G * RGBA_FRACTAL;
		float b = c.B * RGBA_FRACTAL;
		GXMVECTOR color = XMVectorSet(r,g,b,a);
		DrawLineOnlyObject(xaxis12, yaxis12, zaxis12, color);
	}

	// Set the descriptor heaps
	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	if (m_loadingDrawnObjectComplete)
	{
		PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw Object");
		m_drawMode = (int)Maker2DrawMode::DrawTexturedObject;

		DirectXPage^ xpg = m_vars->GetDXPage();
		const XMVECTORF32 scale = { xpg->Get_xScaleDrawnObject(), xpg->Get_yScaleDrawnObject(), xpg->Get_zScaleDrawnObject() };
		const XMVECTORF32 translate = { xpg->Get_xTranslateDrawnObject(), xpg->Get_yTranslateDrawnObject(), xpg->Get_zTranslateDrawnObject() };
		XMVECTOR angles = { xpg->Get_xRotateDrawnObject(), xpg->Get_yRotateDrawnObject(), xpg->Get_zRotateDrawnObject() };// { XM_PI / 2.f, 0.f, -XM_PI / 2.f };
		XMVECTOR rotate = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles);
		// orig
		DirectX::XMMATRIX localDrawnObject = XMLoadFloat4x4(&m_world4x4) * XMMatrixTransformation(g_XMZero, DirectX::XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);

		//DirectX::XMMATRIX localDrawnObject = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
		// *XMMatrixRotationRollPitchYawFromVector(angles));
		if (m_performScaleDrawnObjectComplete)
		{
			const XMVECTORF32 scale = { 0.01f, 0.01f, 0.01f };
			const XMVECTORF32 translate = { 0.0f, 0.0f, 20.0f };
			XMVECTOR angles = {0.0f, 0.0f, 0.0f};// { XM_PI / 2.f, 0.f, -XM_PI / 2.f };
	        XMVECTOR rotate = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles);
			// orig
			//localDrawnObject = XMLoadFloat4x4(&m_world4x4) * XMMatrixTransformation(g_XMZero, DirectX::XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
	        //Model::UpdateEffectMatrices(m_modelEffects, local, XMLoadFloat4x4(&m_view4x4), XMLoadFloat4x4(&m_projection4x4));
		}
		m_shapeDrawnObjectEffect->SetWorld(localDrawnObject);
		m_shapeDrawnObjectEffect->Apply(commandList);
		m_shapeDrawnObjectTex->Draw(commandList);
		PIXEndEvent(commandList);
	}
	// Draw sprite
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw sprite");
	m_sprites->Begin(commandList);
		
	// Put MousePosition on screen
	Platform::String^ m_fontString = nullptr;
	m_MousePosFont->DrawString(m_sprites.get(),
		ObjectXYZPositionString(m_fontString, "Mouse ", m_posX, m_posY, m_posZ)->Data(),  
	    XMFLOAT2(10, 10), Colors::Yellow);
	
	// Put Camera Eye on screen
	Platform::String^ m_fontStringCameraEye = nullptr;
	m_CameraEyeFont->DrawString(m_sprites.get(), 
		ObjectXYZPositionString(m_fontStringCameraEye, "Camera Eye ", m_EyeX, m_EyeY, m_EyeZ)->Data(),
		XMFLOAT2(10, 30), Colors::DarkSeaGreen);

	// Put Camera At on screen
	Platform::String^ m_fontStringCameraAt = nullptr;
	m_CameraAtFont->DrawString(m_sprites.get(),
		ObjectXYZPositionString(m_fontStringCameraAt, "Camera At  ", m_LookAtX, m_LookAtY, m_LookAtZ)->Data(),
		XMFLOAT2(10, 60), Colors::Beige); 
	
	// Put Camera Up on screen
	Platform::String^ m_fontStringCameraUp = nullptr;
	m_CameraUpFont->DrawString(m_sprites.get(), 
		ObjectXYZPositionString(m_fontStringCameraUp, "Camera Up  ", m_UpX, m_UpY, m_UpZ)->Data(),
        XMFLOAT2(10, 90), Colors::DarkOrchid);

	// Put Camera Up on screen
	Platform::String^ m_fontStringPointCount= nullptr;
	m_PointCountFont->DrawString(m_sprites.get(),
		PointCountString(m_fontStringPointCount, "Pt Cnt", m_iPointCount)->Data(),
		XMFLOAT2(10, 650), Colors::Black);

	Platform::String^ m_fontStringTotalPointCount = nullptr;
	m_TotalPointCountFont->DrawString(m_sprites.get(),
		PointCountString(m_fontStringTotalPointCount, "#Pts", (unsigned int)m_iTotalPointCount)->Data(),
		XMFLOAT2(180, 650), Colors::Black);

	Platform::String^ m_fontStringFaceCount = nullptr;
	m_FaceCountFont->DrawString(m_sprites.get(),
		PointCountString(m_fontStringFaceCount, "#Faces", (unsigned int)indices.size()/3)->Data(),
		XMFLOAT2(360, 650), Colors::Black);

	Platform::String^ m_fontStringGroupCount = nullptr;
	m_GroupCountFont->DrawString(m_sprites.get(),
		PointCountString(m_fontStringGroupCount, "#Groups", (unsigned int)m_iGroupCount)->Data(),
		XMFLOAT2(540, 650), Colors::Black);

	Platform::String^ m_fontStringSelectedPoint = nullptr;
	m_SelectedPointNumberFont->DrawString(m_sprites.get(),
		PointCountString(m_fontStringSelectedPoint, "Sel Pt", (unsigned int)m_SelIndexOne)->Data(),
		XMFLOAT2(720, 650), Colors::Black);

	m_sprites->End();

	PIXEndEvent(commandList);

	//XMVECTOR angles = {0.0f, 0.0f, 0.0f};// { XM_PI / 2.f, 0.f, -XM_PI / 2.f };
	//XMVECTOR rotate = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles);
	//localTetra = XMLoadFloat4x4(&m_world4x4) * XMMatrixTransformation(g_XMZero, DirectX::XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
	//Model::UpdateEffectMatrices(m_modelEffects, local, XMLoadFloat4x4(&m_view4x4), XMLoadFloat4x4(&m_projection4x4));

	if (m_drawMode == (int)Maker2DrawMode::DrawSceneParts || m_drawMode == (int)Maker2DrawMode::DrawSelectWithTetras) //1
	{
		
		// Draw 3D object
		PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw tetras");
		/*
		if (m_drawMode == (int)Maker2DrawMode::DrawSceneParts)// 0;
		{
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				DirectX::XMMATRIX localTetra = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(posX[i], posY[i], posZ[i]));
				// *XMMatrixRotationRollPitchYawFromVector(angles));
				m_shapeTetraEffect->SetWorld(localTetra);
				m_shapeTetraEffect->Apply(commandList);
				m_shapeTetra->Draw(commandList);
			}
		}
		
		
		if (m_drawMode == (int)Maker2DrawMode::DrawSelectWithTetras)// 1;
		{
			unsigned int cnt = (unsigned int)vertices.size();
			for (unsigned int i = 0; i < cnt; i++)
			{
				DirectX::DXTKXAML12::Vertex v = vertices.at(i);
				DirectX::XMMATRIX localTetra = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(v.position.x, v.position.y, v.position.z));
				// *XMMatrixRotationRollPitchYawFromVector(angles));
				m_shapeTetraEffect->SetWorld(localTetra);
				m_shapeTetraEffect->Apply(commandList);
				m_shapeTetra->Draw(commandList);
			}
		}
		*/
		PIXEndEvent(commandList);
	}

	
	PIXEndEvent(commandList);

	// Show the new frame.
	PIXBeginEvent(m_sceneDeviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");

	m_sceneDeviceResources->Present();

	m_graphicsMemory->Commit(m_sceneDeviceResources->GetCommandQueue());

	PIXEndEvent(m_sceneDeviceResources->GetCommandQueue());

	return true;
}

// Helper method to clear the back buffers.
void Hot3dxRotoDraw::Maker2SceneRender::Clear()
{
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

	// Clear the views.
	auto rtvDescriptor = m_sceneDeviceResources->GetRenderTargetView();
	auto dsvDescriptor = m_sceneDeviceResources->GetDepthStencilView();

	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
	commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
	auto viewport = m_sceneDeviceResources->GetScreenViewport();
	auto scissorRect = m_sceneDeviceResources->GetScissorRect();
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

	PIXEndEvent(commandList);
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::Draw3DCursorXY(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, XMFLOAT3 curPos)
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

			if (m_bLButtonDown)
			{
				if(checkDistance(curPos.x, curPos.y, curPos.z, m_vars->GetDXPage()->GetPointSpacingDXP()))
				DrawPointsOne(curPos);
			}// eo if (m_bLButtonDown)
			m_posX = curPos.x;
			m_posY = curPos.y;
			m_posZ = curPos.z;
	m_batch->End();
	
	PIXEndEvent(commandList);
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::ClearGridPicRectangle()
{
	m_bDDS_WIC_FLAGGridPicComplete = false;
	m_bDDS_WIC_FLAGGridPic = false;
	m_textureGridPic.Reset();
	m_shapeGridPic.reset();
	m_drawRectangleEffect.reset();
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawGridPicRectangle()
{
	//m_bDDS_WIC_FLAGGridPic = false;
	m_bDDS_WIC_FLAGGridPicComplete = false;
	DirectX::DXTKXAML12::VertexCollection vCol;
	vCol.push_back(DirectX::DXTKXAML12::VertexPositionNormalTexture(XMFLOAT3(-10.5f, 10.5f, 0.0f), XMFLOAT3(0.000000f, 1.000000f, -0.000000f), XMFLOAT2(0.0000f, 0.0000f)));// , vPNT[3]);
	vCol.push_back(DirectX::DXTKXAML12::VertexPositionNormalTexture(XMFLOAT3(10.5f, 10.5f, 0.0f), XMFLOAT3(0.999894f, 0.000000f, -0.014547f), XMFLOAT2(1.00000f, 0.0000f)));
	vCol.push_back(DirectX::DXTKXAML12::VertexPositionNormalTexture(XMFLOAT3(10.5f, -10.5f, 0.0f), XMFLOAT3(0.000000f, 1.000000f, -0.000000f), XMFLOAT2(1.00000f, 1.00000f)));
	vCol.push_back(DirectX::DXTKXAML12::VertexPositionNormalTexture(XMFLOAT3(-10.5f, -10.5f, 0.0f), XMFLOAT3(0.014547f, 0.000000f, -0.999894f), XMFLOAT2(0.0000f, 1.000000f)));
	DirectX::IndexCollectionColor vColColor = { 0,1,2,2,3,0 };
	m_shapeGridPic = GeometricPrimitive::CreateCustom(vCol, vColColor, m_sceneDeviceResources->GetD3DDevice());

	DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadGridPic = new DirectX::DXTKXAML12::ResourceUploadBatch(m_sceneDeviceResources->GetD3DDevice());
	m_resourceUploadGridPic->BeginXaml();

	// 
	if (m_bDDS_WIC_FLAGGridPic == true)
	{
		DX::ThrowIfFailed(
			DXTKXAML12::CreateDDSTextureFromFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadGridPic, m_textureImageGridPicFile->Data(), &m_textureGridPic)//.ReleaseAndGetAddressOf())
		);
	}
	else
	{
		DX::ThrowIfFailed(
			DXTKXAML12::CreateWICTextureFromFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadGridPic, m_textureImageGridPicFile->Data(), &m_textureGridPic)//.ReleaseAndGetAddressOf())
		);
	}
	DXTKXAML12::CreateShaderResourceView(m_sceneDeviceResources->GetD3DDevice(), m_textureGridPic.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::GridPicTexture)), false);

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

	// solution XMMATRIX projection = XMLoadFloat4x4(&m_projection4x4);
	m_drawRectangleEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));

	m_sceneDeviceResources->WaitForGpu();
	loaded.then([this]()
		{
			m_bDDS_WIC_FLAGGridPicComplete = true;
			m_bDDS_WIC_FLAGGridPic = true;
		});

}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawLineOnlyObject(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, GXMVECTOR color)
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
			GXMVECTOR position = XMVectorSet(posX[i], posY[i], posZ[1], 0.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(position, color);// XMVectorSubtract(vScale, zAxis), color2);
			m_batch->DrawLine(v1, v1);
		}
		else
		{
			GXMVECTOR position = XMVectorSet(posX[i-1], posY[i-1], posZ[1-1], 0.0f);
			GXMVECTOR position2 = XMVectorSet(posX[i], posY[i], posZ[1], 0.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(position, color);// XMVectorSubtract(vScale, zAxis), color2);
			DirectX::DXTKXAML12::VertexPositionColor v2(position2, color);// XMVectorSubtract(vScale, zAxis), color2);
			m_batch->DrawLine(v1, v2);
		}
	}

	m_batch->End();

	PIXEndEvent(commandList);
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawGridXY(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw grid");
	DirectX::XMMATRIX localGridXY = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	m_lineEffect->SetWorld(localGridXY);
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
			GXMVECTOR color2 = XMVectorSet(0.0f,0.5f,0.0f,1.0f);
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color2);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color2);
			m_batch->DrawLine(v1, v2);
		}
		else 
		{
			DirectX::DXTKXAML12::VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
			DirectX::DXTKXAML12::VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
			m_batch->DrawLine(v1, v2);
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

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawGridXZ(FXMVECTOR xAxis, FXMVECTOR zAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw grid");
	DirectX::XMMATRIX localGridXZ = (XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
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


void Hot3dxRotoDraw::Maker2SceneRender::ViewMatrix(XMFLOAT4X4 m, wchar_t* str)
{

	XMMatrixSet(m._11, m._12, m._13, m._14,
		        m._21, m._22, m._23, m._24,
		        m._31, m._32, m._33, m._34,
		        m._41, m._42, m._43, m._44
	);

	
	wchar_t* t = nullptr;// = {};
	//OutputDebugString(t);
	//OutputDebugString(str);
	
		wchar_t* s = { L"\nThe Matrix values: \n%s\n%.6f  %.6f  %.6f %.6f\n%.6f  %.6f  %.6f %.6f\n%.6f  %.6f  %.6f %.6f\n%.6f  %.6f  %.6f %.6f\n" };
	//swprintf_s(t, 1000, s, str, m._11, m._12, m._13, m._14,
		StringCbPrintf(t, 1000, s, str, m._11, m._12, m._13, m._14,
		m._21, m._22, m._23, m._24,
		m._31, m._32, m._33, m._34,
		m._41, m._42, m._43, m._44 );
	

	// OutputDebugString(t);
	
}

void Hot3dxRotoDraw::Maker2SceneRender::RotatePitch(float degree)
{
	//RotateYawSquid(degree);
	XMStoreFloat4x4(&m_world4x4, DirectX::XMMatrixRotationX((float)(m_timer.GetTotalSeconds() * XM_PIDIV4)));
}

void Hot3dxRotoDraw::Maker2SceneRender::RotateYaw(float degree)
{
	//RotatePitchSquid(degree); 
	XMStoreFloat4x4(&m_world4x4, DirectX::XMMatrixRotationY((float)(m_timer.GetTotalSeconds() * XM_PIDIV4)));
}

void Hot3dxRotoDraw::Maker2SceneRender::RotatePitchSquid(float degree)
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

void Hot3dxRotoDraw::Maker2SceneRender::RotateYawSquid(float degree)
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

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawObjectPoints()
{
	if (m_iPointCount <= 0) { return; }
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	m_drawMode = (int)Maker2DrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	float m_fCamMove_degreeradian = 0.017453293005625408f;
	//float m_fdegreesOfPartialDrawnObject
	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = m_vars->GetDXPage()->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)((360.0f - m_vars->GetDXPage()->m_Scene2Vars->GetPartialRotateAngle()) / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);

	uint16_t k = 0;
	for (unsigned int j = 0; j < 1; j++)
	{
		IncrementPtGroups();
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX[i], posY[i], posZ[i]), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(static_cast<size_t>(0))->SetPtList(i, k);
			k++;
		}
	}

	if (m_bIsYAxis)
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			IncrementPtGroups();
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				//float pointRadius = posX[i];
				float aa = j * a;
				float x = m_hot3dxRotate->xCoordofYRot3(posX[i], posZ[i], aa);
				float z = m_hot3dxRotate->zCoordofYRot3(posX[i], posZ[i], aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, posY[i], z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(static_cast<size_t>(j))->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}
	else
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			IncrementPtGroups();
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				//float pointRadius = posX[i];
				float aa = j * a;
				float y = m_hot3dxRotate->yCoordofXRot3(posY[i], posZ[i], aa);
				float z = m_hot3dxRotate->zCoordofXRot3(posY[i], posZ[i], aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX[i], y, z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(static_cast<size_t>(j))->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}

	IncrementPtGroups();

	size_t sz = m_PtGroupList.size() - 1;
	for (unsigned int i = 0; i < m_iPointCount; i++)
	{
		DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
		vertices.push_back(vpc);
		m_PtGroupList.at(sz)->SetPtList(i, k);
		k++;
	}
	size_t szv = vertices.size();

	m_iTotalPointCount = vertices.size();
	m_iGroupCount = (unsigned int)m_PtGroupList.size();
	//k++;

}

uint16_t XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawObjectPoints(uint16_t n)
{
	if (m_iPointCount <= 0) { return 0; }
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	m_drawMode = (int)Maker2DrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	float m_fCamMove_degreeradian = 0.017453293005625408f;
	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = page->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)((360.0f - page->m_Scene2Vars->GetPartialRotateAngle()) / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = page->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);

	uint16_t k = n;
	for (unsigned int j = 0; j < 1; j++)
	{
		IncrementPtGroups();

		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(static_cast<size_t>(0))->SetPtList(i, k);
			k++;
		}
	}

	if (m_bIsYAxis)
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				//float pointRadius = posX->get(i);
				float aa = (float)j * a;
				float x = m_hot3dxRotate->xCoordofYRot3(posX->get(i), posZ->get(i), aa);
				float z = m_hot3dxRotate->zCoordofYRot3(posX->get(i), posZ->get(i), aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, posY->get(i), z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j

	}
	else
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				//float pointRadius = posX->get(i);
				float aa = (float)j * a;
				float y = m_hot3dxRotate->yCoordofXRot3(posY->get(i), posZ->get(i), aa);
				float z = m_hot3dxRotate->zCoordofXRot3(posY->get(i), posZ->get(i), aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), y, z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(static_cast<size_t>(j))->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}

	//Set the First Point Group in the Last Group if OPEN_OR_CLOSED CHECKBOX IS CLOSED 
	//if (page->m_Scene2Vars->GetOpenOrClosedChecked() == true){
	IncrementPtGroups();

	size_t sz = m_PtGroupList.size() - 1;
	for (unsigned int i = 0; i < m_iPointCount; i++)
	{
		DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
		vertices.push_back(vpc);
		m_PtGroupList.at(sz)->SetPtList(i, k);
		k++;
	}
	//} // eo if (page->m_Scene2Vars->GetOpenOrClosedChecked() == true)

	size_t szv = vertices.size();

	m_iTotalPointCount = vertices.size();
	m_iGroupCount = (unsigned int)m_PtGroupList.size();
	return k;
}


void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawObjectPointsTop()
{
	if (m_iPointCount <= 0) { return; }
	m_drawMode = (int)Maker2DrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	float m_fCamMove_degreeradian = 0.017453293005625408f;

	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = m_vars->GetDXPage()->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)(360.0f / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);

	uint16_t k = 0;
	for (unsigned int j = 0; j < 1; j++)
	{
		PtGroups^ ptGroups = ref new PtGroups();
		m_PtGroupList.push_back(ptGroups);
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX[i], posY[i], posZ[i]), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(0)->SetPtList(i, k);
			k++;
		}
	}

	if (m_bIsYAxis)
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			PtGroups^ ptGroups = ref new PtGroups();
			m_PtGroupList.push_back(ptGroups);
			for (unsigned int i = 1; i < m_iPointCount; i++)
			{
				//float pointRadius = posX[i];
				float aa = j * a;
				float x = m_hot3dxRotate->xCoordofYRot3(posX[i], posZ[i], aa);
				float z = m_hot3dxRotate->zCoordofYRot3(posX[i], posZ[i], aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, posY[i], z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}
	else
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			PtGroups^ ptGroups = ref new PtGroups();
			m_PtGroupList.push_back(ptGroups);
			for (unsigned int i = 1; i < m_iPointCount; i++)
			{
				//float pointRadius = posX[i];
				float aa = j * a;
				float y = m_hot3dxRotate->yCoordofXRot3(posY[i], posZ[i], aa);
				float z = m_hot3dxRotate->zCoordofXRot3(posY[i], posZ[i], aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX[i], y, z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}
	m_iTotalPointCount = vertices.size();
	m_iGroupCount = cnt;

}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawObjectPointsBottom()
{
	if (m_iPointCount <= 0) { return; }
	m_drawMode = (int)Maker2DrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	float m_fCamMove_degreeradian = 0.017453293005625408f;

	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = m_vars->GetDXPage()->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)(360.0f / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);

	uint16_t k = 0;
	for (unsigned int j = 0; j < 1; j++)
	{
		PtGroups^ ptGroups = ref new PtGroups();
		m_PtGroupList.push_back(ptGroups);
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX[i], posY[i], posZ[i]), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(0)->SetPtList(i, k);
			k++;
		}
	}

	if (m_bIsYAxis)
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			PtGroups^ ptGroups = ref new PtGroups();
			m_PtGroupList.push_back(ptGroups);
			for (unsigned int i = 0; i < m_iPointCount-1; i++)
			{
				//float pointRadius = posX[i];
				float aa = j * a;
				float x = m_hot3dxRotate->xCoordofYRot3(posX[i], posZ[i], aa);
				float z = m_hot3dxRotate->zCoordofYRot3(posX[i], posZ[i], aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, posY[i], z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}
	else
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			PtGroups^ ptGroups = ref new PtGroups();
			m_PtGroupList.push_back(ptGroups);
			for (unsigned int i = 0; i < m_iPointCount-1; i++)
			{
				//float pointRadius = posX[i];
				float aa = j * a;
				float y = m_hot3dxRotate->yCoordofXRot3(posY[i], posZ[i], aa);
				float z = m_hot3dxRotate->zCoordofXRot3(posY[i], posZ[i], aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX[i], y, z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}
	m_iTotalPointCount = vertices.size();
	m_iGroupCount = cnt;

}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::DrawObjectPointsTopBottom()
{
	if (m_iPointCount <= 0) { return; }
	m_drawMode = (int)Maker2DrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	float m_fCamMove_degreeradian = 0.017453293005625408f;

	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = m_vars->GetDXPage()->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)(360.0f / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);

	uint16_t k = 0;
	for (unsigned int j = 0; j < 1; j++)
	{
		PtGroups^ ptGroups = ref new PtGroups();
		m_PtGroupList.push_back(ptGroups);
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX[i], posY[i], posZ[i]), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(0)->SetPtList(i, k);
			k++;
		}
	}

	if (m_bIsYAxis)
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			PtGroups^ ptGroups = ref new PtGroups();
			m_PtGroupList.push_back(ptGroups);
			for (unsigned int i = 1; i < m_iPointCount-1; i++)
			{
				//float pointRadius = posX[i];
				float aa = j * a;
				float x = m_hot3dxRotate->xCoordofYRot3(posX[i], posZ[i], aa);
				float z = m_hot3dxRotate->zCoordofYRot3(posX[i], posZ[i], aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, posY[i], z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}
	else
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			PtGroups^ ptGroups = ref new PtGroups();
			m_PtGroupList.push_back(ptGroups);
			for (unsigned int i = 1; i < m_iPointCount-1; i++)
			{
				//float pointRadius = posX[i];
				float aa = j * a;
				float y = m_hot3dxRotate->yCoordofXRot3(posY[i], posZ[i], aa);
				float z = m_hot3dxRotate->zCoordofXRot3(posY[i], posZ[i], aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX[i], y, z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}
	m_iTotalPointCount = vertices.size();
	m_iGroupCount = cnt;

}

void Hot3dxRotoDraw::Maker2SceneRender::CalculateMeshFaces()
{
	
	for (unsigned int i = 0; i < m_iGroupCount-1; i++)
	{
		for (unsigned int j = 0; j < m_iPointCount-1; j++)
		{
			// First Face
			size_t posI = static_cast<size_t>(i);
			// First Face
			uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
			uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
			uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
			uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			// Second Face
			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(d);
		}
	}

	if (m_vars->GetDXPage()->m_Scene2Vars->GetOpenOrClosedChecked())
	{
		// From Last Group to First Group
		unsigned int i = m_iGroupCount - 1;
		for (unsigned int j = 0; j < m_iPointCount - 1; j++)
		{
			// First Face
			uint16_t a = m_PtGroupList.at(i)->GetListPt(j);
			uint16_t b = m_PtGroupList.at(i)->GetListPt(j + 1);
			uint16_t c = m_PtGroupList.at(0)->GetListPt(j + 1);
			uint16_t d = m_PtGroupList.at(0)->GetListPt(j);
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			// Second Face
			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(d);
		}
	}
	//EndPointSetFaceValues();
	InitDrawnObjectSingleTexture();
	m_iDrawMode = 2;
	
}

void Hot3dxRotoDraw::Maker2SceneRender::CalculateMeshFacesTopBottom()
{

	for (unsigned int i = 0; i < m_iGroupCount - 1; i++)
	{
		for (unsigned int j = 0; j < m_iPointCount - 1; j++)
		{
			size_t posI = static_cast<size_t>(i);
			// First Face
			uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
			uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
			uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
			uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
			if (j == 0)
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
			}
			else if (j == m_iPointCount - 2)
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(d);
			}
			else
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
				// Second Face
				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(d);
			}
		}
	}


	//EndPointSetFaceValues();
	InitDrawnObjectSingleTexture();
	m_iDrawMode = 2;

}


void Hot3dxRotoDraw::Maker2SceneRender::CalculateMeshFacesBottom()
{

	for (unsigned int i = 0; i < m_iGroupCount - 1; i++)
	{
		for (unsigned int j = 0; j < m_iPointCount - 2; j++)
		{
			size_t posI = static_cast<size_t>(i);
			// First Face
			uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
			uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
			uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
			uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			// Second Face
			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(d);
		}
	}

	// From Last Group to First Group
	unsigned int i = m_iGroupCount - 1;
	for (unsigned int j = 0; j < m_iPointCount - 2; j++)
	{
		// First Face
		uint16_t a = m_PtGroupList.at(i)->GetListPt(j);
		uint16_t b = m_PtGroupList.at(i)->GetListPt(j + 1);
		uint16_t c = m_PtGroupList.at(0)->GetListPt(j + 1);
		uint16_t d = m_PtGroupList.at(0)->GetListPt(j);
		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
		// Second Face
		indices.push_back(a);
		indices.push_back(c);
		indices.push_back(d);
	}
	//EndPointSetFaceValues();
	//InitDrawnObjectSingleTexture();
	//m_iDrawMode = 2;

}


void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::EndpointTopLeftFaces()
{
	uint16_t a = 0;
	for (unsigned int i = 0; i < m_iGroupCount - 1; i++)
	{
		size_t posI = static_cast<size_t>(i);
		// First Face
		uint16_t b = m_PtGroupList.at(posI)->GetListPt(0);
		uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(0);
		    indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
		
	}
	// From Last Group to First Group
	unsigned int i = m_iGroupCount - 1;
	
	// First Face
		uint16_t b = m_PtGroupList.at(i)->GetListPt(0);
		uint16_t c = m_PtGroupList.at(0)->GetListPt(0);
		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);

		InitDrawnObjectSingleTexture();
		m_iDrawMode = 2;
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::EndpointBottomRightFaces()
{
	uint16_t a = m_iPointCount - 1;
	unsigned int j = m_iPointCount - 2;
	for (unsigned int i = 0; i < m_iGroupCount-1; i++)
	{
		
		size_t posI = static_cast<size_t>(i);
		// First Face
		uint16_t b = m_PtGroupList.at(posI + 1)->GetListPt(j);
		uint16_t c = m_PtGroupList.at(posI)->GetListPt(j);
			indices.push_back(b);
			indices.push_back(c);
			indices.push_back(a);


	}
	// From Last Group to First Group
	unsigned int i = m_iGroupCount - 1;
	
	// First Face
	uint16_t b = m_PtGroupList.at(i)->GetListPt(j);
	uint16_t c = m_PtGroupList.at(0)->GetListPt(j);
	indices.push_back(c);
	indices.push_back(b);
	indices.push_back(a);

	InitDrawnObjectSingleTexture();
	m_iDrawMode = 2;
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::GetUVPercent()
{
	textureU.clear();
	textureV.clear();
	unsigned int cnt = 0;
	float g = 1.0f / (float)(m_iGroupCount - 1);
	float p = 1.0f / (float)(m_iPointCount - 1);
	float pt = 0.0f;
	float gt = 1.0f;

	if (m_iEndPointOneCount == 1)
	{
		textureU.push_back(gt);
		textureV.push_back(pt);
	}

	// unsigned int tp = m_iTotalPointCount;
	for (unsigned int i = 0; i < (m_iGroupCount); i++)
	{
		for (unsigned int j = 0; j < m_iPointCount; j++)
		{
			textureU.push_back(gt);
			textureV.push_back(pt);
			pt = pt + p;
		}
		gt = gt - g;
		pt = 0.0f;
	}
	if (m_iEndPointTwoCount == 1)
	{
		textureU.push_back(gt);
		textureV.push_back(pt);
	}
}

float* XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::GetU(XMVECTOR v, Platform::Array<float>^ b)
{
	static float uv[2];

	float a = XMVectorGetX(v);
	float c = XMVectorGetY(v);
	//float e = XMVectorGetZ(v);


	XMVECTOR r;
	XMVECTOR m;
	XMVECTOR s;
	m = XMVectorSet(b->get(0), b->get(2), b->get(4), 0.0f); //b->min.x, b->min.y, b->min.z, 0.0f);
	s = XMVectorSet(b->get(1), b->get(3), b->get(5), 0.0f);//b->max.x, b->max.y, b->max.z, 0.0f);
	r = XMVectorSubtract(m, s);


	if (a == b->get(0))//b->min.x)
	{
		uv[0] = 0.0F;
	}

	else {
		float x = XMVectorGetX(r);
		if (x <= 0)
		{
			uv[0] = 1 * ((b->get(0) - a) / x);
		}
		else {
			uv[0] = 1 * ((a - b->get(0)) / x);
		}
		//uv[0] = -1 * ((a - b->min.x) / XMVectorGetX(r));
	}
	if (c == b->get(3)) {//b->max.y) {
		uv[1] = 0.0F;
	}
	else {
		uv[1] = 1 * ((c - b->get(3)) / XMVectorGetY(r));
		//uv[1] = 1 * ((c - b->max.y) / XMVectorGetY(r));
	}
	/*
	TCHAR dest[100];
	TCHAR* str = { L"U: %.6f V: %.6f\n" };
	StringCbPrintf(dest, 100, str, uv[0], uv[1]);
	OutputDebugString(dest);
	*/
	return uv;
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::InitDrawnObjectSingleTexture()
{
	if (m_loadingDrawnObjectComplete == false)
	{
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}
		//m_shapeDrawnObject = Hot3dxDrawnObject::CreateDrawnObjectColor( vertices, indices, device);
		size_t cnt = vertices.size();
		//vertexes.reserve(cnt);
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		GetUVPercent();

		for (size_t i = 0; i < cnt; i++)
		{
			/*
				if (i == cnt - 2)
				{
					vertices.at(i).position.x;
				}
			*/
			XMVECTOR n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			//float* tuv = GetU(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f), box);
			//DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(-tuv[0], tuv[1]) };
			if (i > textureU.size()) { textureU.push_back(0.0f); }
			if (i > textureV.size()) { textureV.push_back(0.0f); }
			DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(textureU.at(i), textureV.at(i)) };
			vertexes.push_back(vpnt);

		}

		/*
		for (size_t i = 0; i < m_iPointCount; i++)
		{
			XMVECTOR n = XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			float* tuv = GetU(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f), box);
			DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(-tuv[0], tuv[1]) };
			vertexes.push_back(vpnt);
			//vertexes.at(i).position=vertices.at(i).position;
		}
		*/

		m_shapeDrawnObjectTex = GeometricPrimitive::CreateCustom(vertexes, indices, device);
		{

			DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);

			// Begin Resource Upload
			m_resourceUploadDrawnObject->BeginXaml();

			if (m_bDDS_WIC_FLAG1 == true)
			{
				DX::ThrowIfFailed(
					DirectX::DXTKXAML12::CreateDDSTextureFromFile(device, *m_resourceUploadDrawnObject, m_textureImage1File->Data(), &m_DrawnMeshTexture1));
			}
			else
			{
				DX::ThrowIfFailed(
					CreateWICTextureFromFile(device, *m_resourceUploadDrawnObject, m_textureImage1File->Data(), &m_DrawnMeshTexture1));
			}

			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_DrawnMeshTexture1.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::DrawnObjectTexture1)));

			RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());
			// Each effect object must be proceeded by its own 
			// EffectPipelineStateDescription pd 
			// even if the EffectPipelineStateDescription pd is the same

			{
				EffectPipelineStateDescription pd(
					&GeometricPrimitive::VertexType::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					CommonStates::CullNone,
					rtState);

				m_shapeDrawnObjectEffect = std::make_unique<BasicEffect>(device, EffectFlags::PerPixelLighting | EffectFlags::Texture, pd);
				m_shapeDrawnObjectEffect->EnableDefaultLighting();
				m_shapeDrawnObjectEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::DrawnObjectTexture1)), m_states->AnisotropicWrap());// PointWrap());// > AnisotropicWrap());


			}


			//HANDLE completeEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
			auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
			WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);

			// solution XMMATRIX projection = XMLoadFloat4x4(&m_projection4x4);
			//m_shapeEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_shapeDrawnObjectEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));


			//m_sprites->SetRotation(m_sceneDeviceResources->GetRotation());
			m_sceneDeviceResources->WaitForGpu();
			loaded.then([this]()
				{
					m_loadingDrawnObjectComplete = true;

				});

		}
	}
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::InitDrawnObjectDualTexture()
{
	if (m_loadingDrawnObjectComplete == false)
	{
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}
		//m_shapeDrawnObject = Hot3dxDrawnObject::CreateDrawnObjectColor( vertices, indices, device);
		size_t cnt = vertices.size();
		//vertexes.reserve(cnt);
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		GetUVPercent();

		for (size_t i = 0; i < cnt; i++)
		{
			XMVECTOR n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			//float* tuv = GetU(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f), box);
			//DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(-tuv[0], tuv[1]) };
			DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(textureU.at(i), textureV.at(i)) };
			vertexes.push_back(vpnt);

		}

		/*
		for (size_t i = 0; i < m_iPointCount; i++)
		{
			XMVECTOR n = XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			float* tuv = GetU(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f), box);
			DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(-tuv[0], tuv[1]) };
			vertexes.push_back(vpnt);
			//vertexes.at(i).position=vertices.at(i).position;
		}
		*/

		m_shapeDrawnObjectTex = GeometricPrimitive::CreateCustom(vertexes, indices, device);
		{

			DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);

			// Begin Resource Upload
			m_resourceUploadDrawnObject->BeginXaml();

			Platform::String^ m_strTextureFileNameFirst = ref new Platform::String(L"Assets\\Textures\\Marble.dds");

			if (m_bDDS_WIC_FLAG1 == true)
			{
				//DX::ThrowIfFailed(DirectX::DXTKXAML12::CreateDDSTextureFromFile(device, *m_resourceUploadDrawnObject, m_textureImage1File->Data(), &m_DrawnMeshTexture1));
				DX::ThrowIfFailed(
					DirectX::DXTKXAML12::CreateDDSTextureFromFile(device, *m_resourceUploadDrawnObject, m_strTextureFileNameFirst->Data(), &m_DrawnMeshTexture1)//.ReleaseAndGetAddressOf())
				);
			}
			else
			{
				DX::ThrowIfFailed(
					CreateWICTextureFromFile(device, *m_resourceUploadDrawnObject, m_strTextureFileNameFirst->Data(), &m_DrawnMeshTexture1));
			}

			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_DrawnMeshTexture1.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::DrawnObjectTexture1)));

			//////////////////////////////////////////////////////////////////
			// Second Texture Load
			Platform::String^ m_strTextureFileNameSecond = ref new Platform::String(L"Assets\\Textures\\seafloor.dds");
			if (m_bDDS_WIC_FLAG2 == true)
			{
				DX::ThrowIfFailed(
					DirectX::DXTKXAML12::CreateDDSTextureFromFile(device, *m_resourceUploadDrawnObject, m_strTextureFileNameSecond->Data(), &m_DrawnMeshTexture2));
			}
			else
			{
				DX::ThrowIfFailed(
					CreateWICTextureFromFile(device, *m_resourceUploadDrawnObject, m_strTextureFileNameSecond->Data(), &m_DrawnMeshTexture2));
			}

			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_DrawnMeshTexture2.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::DrawnObjectTexture2)));
			//////////////////////////////////////////////

			RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());
			// Each effect object must be proceeded by its own 
			// EffectPipelineStateDescription pd 
			// even if the EffectPipelineStateDescription pd is the same

			{
				EffectPipelineStateDescription pd(
					&GeometricPrimitive::VertexType::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					CommonStates::CullNone,
					rtState);

				m_dualTextureEffect = std::make_unique<DualTextureEffect>(device, EffectFlags::PerPixelLighting | EffectFlags::Texture, pd);
				//m_dualTextureEffect-> EnableDefaultLighting();
				m_dualTextureEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::DrawnObjectTexture1)), m_states->PointWrap());// > AnisotropicWrap());
				m_dualTextureEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::DrawnObjectTexture2)), m_states->PointWrap());// > AnisotropicWrap());
			}


			//HANDLE completeEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
			auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
			WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);

			// solution XMMATRIX projection = XMLoadFloat4x4(&m_projection4x4);
			//m_shapeEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_shapeDrawnObjectEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));


			//m_sprites->SetRotation(m_sceneDeviceResources->GetRotation());
			m_sceneDeviceResources->WaitForGpu();
			loaded.then([this]()
				{
					m_loadingDrawnObjectComplete = true;

				});

		}
	}
}

void Hot3dxRotoDraw::Maker2SceneRender::ScreenMouse3DWorldAlignment()
{
	TCHAR dest[500] = {};;
	TCHAR* s = TEXT("%s\nW: %.6f H: %.6f \n");
	TCHAR* t = TEXT("The ratios:");

	StringCbPrintf(dest, 500, s, t, m_drawMouseWidthRatio, m_drawMouseHeightRatio);
	//OutputDebugString(dest);
}

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::PointDataValues(unsigned int number, float x, float y, float z)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptCntStr = ref new Platform::String(std::to_wstring(number).c_str());
	Platform::String^ m_ptStrX = ref new Platform::String(std::to_wstring(x).c_str());
	Platform::String^ m_ptStrY = ref new Platform::String(std::to_wstring(y).c_str());
	Platform::String^ m_ptStrZ = ref new Platform::String(std::to_wstring(z).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, "Point:");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptCntStr);
	m_ptStr = m_ptStr->Concat(m_ptStr, " X: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " Y: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " Z: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	//m_MousePosFont->DrawString(m_sprites.get(), m_fontString->Data(), XMFLOAT2(10, 10), Colors::Yellow);
	//OutputDebugString(m_ptStr->Data()); 
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::PointDataValuesReturn(unsigned int number, float x, float y, float z, float nx, float ny, float nz, float tu, float tv)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptCntStr = ref new Platform::String(std::to_wstring(number).c_str());
	Platform::String^ m_ptStrX = ref new Platform::String(std::to_wstring(x).c_str());
	Platform::String^ m_ptStrY = ref new Platform::String(std::to_wstring(y).c_str());
	Platform::String^ m_ptStrZ = ref new Platform::String(std::to_wstring(z).c_str());
	Platform::String^ m_ptStrNX = ref new Platform::String(std::to_wstring(nx).c_str());
	Platform::String^ m_ptStrNY = ref new Platform::String(std::to_wstring(ny).c_str());
	Platform::String^ m_ptStrNZ = ref new Platform::String(std::to_wstring(nz).c_str());
	Platform::String^ m_ptStrTU = ref new Platform::String(std::to_wstring(tu).c_str());
	Platform::String^ m_ptStrTV = ref new Platform::String(std::to_wstring(tv).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, "Point:");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptCntStr);
	m_ptStr = m_ptStr->Concat(m_ptStr, " X: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " Y: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " Z: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, " NX: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrNX);
	m_ptStr = m_ptStr->Concat(m_ptStr, " NY: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrNY);
	m_ptStr = m_ptStr->Concat(m_ptStr, " NZ: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrNZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, " TU: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrTU);
	m_ptStr = m_ptStr->Concat(m_ptStr, " TV: ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrTV);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	return m_ptStr;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::IndicesDataValuesReturn(unsigned int number, uint16_t a, uint16_t b, uint16_t c)
{
	Platform::String^ m_ptStr = ref new Platform::String();
	Platform::String^ m_ptCntStr = ref new Platform::String(std::to_wstring(number).c_str());
	Platform::String^ m_ptStrX = ref new Platform::String(std::to_wstring(a).c_str());
	Platform::String^ m_ptStrY = ref new Platform::String(std::to_wstring(b).c_str());
	Platform::String^ m_ptStrZ = ref new Platform::String(std::to_wstring(c).c_str());
	m_ptStr = m_ptStr->Concat(m_ptStr, "Face:");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptCntStr);
	m_ptStr = m_ptStr->Concat(m_ptStr, ": ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrX);
	m_ptStr = m_ptStr->Concat(m_ptStr, ", ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrY);
	m_ptStr = m_ptStr->Concat(m_ptStr, ", ");
	m_ptStr = m_ptStr->Concat(m_ptStr, m_ptStrZ);
	m_ptStr = m_ptStr->Concat(m_ptStr, "\n");
	return m_ptStr;

}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::PointCountString(Platform::String^ m_fontString, Platform::String^ objectString, unsigned int cnt)
{
	m_fontString = ref new Platform::String();
	Platform::String^ m_fontStringCount = ref new Platform::String(std::to_wstring(cnt).c_str());
	m_fontString = m_fontString->Concat(m_fontString, objectString);
	m_fontString = m_fontString->Concat(m_fontString, ": ");
	m_fontString = m_fontString->Concat(m_fontString, m_fontStringCount);
	return m_fontString;
}

Platform::String^ XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::ObjectXYZPositionString(Platform::String^ m_fontString, Platform::String^ objectString, float x, float y, float z)
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

XMMATRIX XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::SetXMMatrix(DirectX::XMFLOAT4X4 m, XMMATRIX xm)
{
	xm = XMMatrixSet(m._11, m._12, m._13, m._14,
		m._21, m._22, m._23, m._24,
		m._31, m._32, m._33, m._34,
		m._41, m._42, m._43, m._44
      );
	return xm;
}

XMMATRIX XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::GetXMMatrix(DirectX::XMFLOAT4X4 m)
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

void XM_CALLCONV Hot3dxRotoDraw::Maker2SceneRender::ClearDrawnObject()
{

	m_drawMode = (int)Maker2DrawMode::DrawLineOnlyObject;// :DrawSceneParts;// 0;
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
	
	m_iTempGroup = ref new Platform::Array<unsigned int>(1000);
	m_iTempMouseX = ref new Platform::Array<float>(1000);
	m_iTempMouseY = ref new Platform::Array<float>(1000);
	m_iTempGroupCount = 0;
	m_PtGroupList.clear();
	m_PtGroupList.resize(0);
	m_iGroupCount = 0; 
	//m_iLastPoint = 0;
	point = XMFLOAT2(0.0f,0.0f);
	vertices.clear();
	vertices.resize(0);
	indices.clear();
	indices.resize(0);
	textureU.clear();
	textureU.resize(0);
	textureV.clear();
	textureV.resize(0);
	
	vertexes.clear();
	vertexes.resize(0);

	m_bDDS_WIC_FLAGGridPicComplete = false;
	m_bDDS_WIC_FLAGGridPic = false;
	m_textureGridPic.Reset();
	m_shapeGridPic.reset();
	m_drawRectangleEffect.reset();

	m_shapeDrawnObjectTex.reset();
	m_shapeDrawnObjectEffect.reset();
	Scenario2_Normal^ sc2 = m_vars->GetDXPage()->m_Scene2Vars->GetScenario2Page();

	sc2->Current->SetTopLeftCheckBoxFalse();
	sc2->Current->SetBottomRightCheckBoxFalse();
	m_vars->GetDXPage()->m_Scene2Vars->SetTopOrLeftChecked(false);
	m_vars->GetDXPage()->m_Scene2Vars->SetBottomOrRightChecked(false);
}

Platform::String^ Hot3dxRotoDraw::Maker2SceneRender::DrawnObjectOpenText()
{
	Platform::String^ file = ref new Platform::String(L"");
	
	return file;
}

Platform::String^ Hot3dxRotoDraw::Maker2SceneRender::DrawnObjectOpenBinary()
{
	Platform::String^ file = ref new Platform::String(L"");

	return file;
}

Platform::String^ Hot3dxRotoDraw::Maker2SceneRender::DrawnObjectSaveObj3DandMtl(Platform::String^ effectName, unsigned int illumType, Platform::String^ mtlObjFilename, Platform::String^ textureFilename)
{
	illumType = 2;
	Platform::String^ file = ref new Platform::String(L"");
	Obj3DMaterialFile^ f = ref new Obj3DMaterialFile(L"Scene1", 1);
	file = file->Concat(file, f->WriteObj3DMaterialHeaderData());
	Scenario5Vars^ vars = m_vars->GetDXPage()->m_Scene5Vars;
	file = file->Concat(file, f->WriteObj3DMaterialListData(effectName, illumType, mtlObjFilename, L"", L"dayfloor.dds", vars->GetMaterialGroupsData()));
	return file;
}

Platform::String^ Hot3dxRotoDraw::Maker2SceneRender::DrawnObjectSaveObjFile(Platform::String^ mtlObjFilename, Platform::String^ nodeName, Platform::String^ effectName)
{
	Platform::String^ file = ref new Platform::String(L"");
	Obj3DFile^ f = ref new Obj3DFile(nodeName, 1);
	file = file->Concat(file, f->DrawnObjectSaveObjFile(m_iPointCount, m_iGroupCount, vertexes, indices, mtlObjFilename, mtlObjFilename, effectName));
	return file;
}

Platform::String^ Hot3dxRotoDraw::Maker2SceneRender::DrawnObjectSaveText(Platform::String^ fileName, unsigned int objectCount)
{
	Platform::String^ file = ref new Platform::String(L"HEADER:Hot3dx:DirectX12:Model:Text:2020\n");
	Platform::String^ m_ptVertexCntStr = ref new Platform::String(std::to_wstring(vertexes.size()).c_str());
	file = file->Concat(file, "VertexType: DirectX::VertexPositionNormalTexture");
	file = file->Concat(file, "\n"); 
	file = file->Concat(file, "VertexCount: ");
	file = file->Concat(file, m_ptVertexCntStr);
	file = file->Concat(file, "\n");
	for (unsigned int i = 0; i < vertexes.size(); i++)
	{
		Platform::String^ aVertexStr = PointDataValuesReturn(i, vertexes.at(static_cast<size_t>(i)).position.x, vertexes.at(static_cast<size_t>(i)).position.y, vertexes.at(static_cast<size_t>(i)).position.z, vertexes.at(static_cast<size_t>(i)).normal.x, vertexes.at(static_cast<size_t>(i)).normal.y, vertexes.at(static_cast<size_t>(i)).normal.z, vertexes.at(static_cast<size_t>(i)).textureCoordinate.x, vertexes.at(static_cast<size_t>(i)).textureCoordinate.y);
		file = file->Concat(file, aVertexStr);
	}

	Platform::String^ m_IndexCntStr = ref new Platform::String(std::to_wstring(indices.size()).c_str());
	file = file->Concat(file, "IndexCount: ");
	file = file->Concat(file, m_IndexCntStr);
	file = file->Concat(file, "\n");

	unsigned int cnt = 0;
	for (unsigned int i = 0; i < indices.size(); )
	{
		size_t posI = static_cast<size_t>(i);
		// First Face
		Platform::String^ aFaceStr = IndicesDataValuesReturn(cnt, indices.at(posI), indices.at(posI+1), indices.at(posI+2));
		file = file->Concat(file, aFaceStr);
		i += 3;
		cnt++;
	}

	Platform::String^ m_TexFleNameStr = ref new Platform::String(m_textureImage1File->Data());
		//L"Assets\\Textures\\Marble.dds");
	file = file->Concat(file, "TextureFileData: \n");
	file = file->Concat(file, "FileNameSize: ");
	file = file->Concat(file, m_TexFleNameStr->Length().ToString());
	file = file->Concat(file, "\n");
	file = file->Concat(file, "TextureFileName: \n");
	file = file->Concat(file, m_TexFleNameStr);
	file = file->Concat(file, "\nEOF");

	return file;
}

Platform::String^ Hot3dxRotoDraw::Maker2SceneRender::DrawnObjectSaveBinary()
{
	Platform::String^ file = ref new Platform::String(L"");

	return file;
}

