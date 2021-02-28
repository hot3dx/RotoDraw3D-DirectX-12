//--------------------------------------------------------------------------------------
// File: SceneRender.cpp
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "SceneRender.h"
#include "..\Common\DirectXHelper.h"
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

using namespace Hot3dxRotoDraw;

using namespace DX;
using namespace Concurrency;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::System::Threading;
using namespace Windows::UI::Xaml;


// Loads vertex and pixel shaders from files and instantiates the cube geometry.
SceneRender::SceneRender(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	
	m_posX(0.0f), m_posY(0.0f), m_posZ(0.0f),
	m_fPointSpace(0.3f),
	m_fScrollDist(0.1F),
	m_iScrollPointSetPos(150),
	m_iPointCount(0),
	m_widthRatio(0.0254000f),
	m_heightRatio(0.039000f),
	m_loadingComplete(false),
	m_radiansPerSecond(XM_PIDIV4 / 2),	// rotate 45 degrees per second
	m_angle(0.01f),
	m_tracking(false),
	sceneVertexCount(8),
	m_sceneDeviceResources(deviceResources),
	m_isRightHanded(false)
{
	SetStartCameraVars();
	
	LoadState();
	ZeroMemory(&m_constantBufferData, sizeof(m_constantBufferData));

	m_camera = ref new Hot3dxCamera();

	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

SceneRender::~SceneRender()
{
	m_loadingComplete = false;

}

void SceneRender::CreateDeviceDependentResources()
{

	if (m_loadingComplete == false)
	{


		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{

			return;
		}

		m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

		m_states = std::make_unique<CommonStates>(device);

		m_resourceDescriptors = std::make_unique<DescriptorHeap>(device,
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
			Descriptors::Count);

		m_batch = std::make_unique<PrimitiveBatch<DirectX::VertexPositionColor>>(device);

		m_shape = GeometricPrimitive::CreateTeapot(4.f, 8);

		m_shapeTetra = GeometricPrimitive::CreateTetrahedron(0.5f);
		// SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
		//   // C:\\Users\\hot3dx-home\\Source\\Repos\\GameD12UW\\Assets\\...

		m_model = Model::CreateFromSDKMESH(L"Assets\\tiny.sdkmesh");//, device);
		{

			ResourceUploadBatch* m_resourceUpload = new ResourceUploadBatch(device);

			// Begin Resource Upload
			m_resourceUpload->BeginXaml();
			m_model->LoadStaticBuffers(device, *m_resourceUpload);
			// C:\\Users\\hot3dx-home\\Source\\Hot3dxRotoDraw\\x64\\Debug\\Hot3dxRotoDraw\\AppX\\

			DX::ThrowIfFailed(
				CreateDDSTextureFromFile(device, *m_resourceUpload, L"Assets\\seafloor.dds", &m_texture1)//.ReleaseAndGetAddressOf())
			);

			CreateShaderResourceView(device, m_texture1.Get(), m_resourceDescriptors->GetCpuHandle(Descriptors::SeaFloor));

			DX::ThrowIfFailed(
				CreateDDSTextureFromFile(device, *m_resourceUpload, L"Assets\\windowslogo.dds", &m_texture2)//.ReleaseAndGetAddressOf())
			);

			CreateShaderResourceView(device, m_texture2.Get(), m_resourceDescriptors->GetCpuHandle(Descriptors::WindowsLogo));

			RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());
			// Each effect object must ne proceeded by its own 
			// EffectPipelineStateDescription pd even if the EffectPipelineStateDescription pd is the same
			{
				EffectPipelineStateDescription pd(
					&DirectX::VertexPositionColor::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthNone,
					CommonStates::CullNone,
					rtState,
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

				m_lineEffect = std::make_unique<BasicEffect>(device, EffectFlags::VertexColor, pd);
			}

			{
				EffectPipelineStateDescription pd(
					&GeometricPrimitive::VertexType::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					CommonStates::CullNone,
					rtState);

				m_shapeEffect = std::make_unique<BasicEffect>(device, EffectFlags::PerPixelLighting | EffectFlags::Texture, pd);
				m_shapeEffect->EnableDefaultLighting();
				m_shapeEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SeaFloor), m_states->AnisotropicWrap());

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
				m_shapeTetraEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SeaFloor), m_states->AnisotropicWrap());
			}

			{
				SpriteBatchPipelineStateDescription pd(rtState);

				m_sprites = std::make_unique<SpriteBatch>(device, *m_resourceUpload, pd);
			}

			m_modelResources = m_model->LoadTextures(device, *m_resourceUpload, L"Assets\\");

			{
				EffectPipelineStateDescription psd(
					nullptr,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					CommonStates::CullClockwise,    // Using RH coordinates, and SDKMESH is in LH coordiantes
					rtState);

				EffectPipelineStateDescription alphapsd(
					nullptr,
					CommonStates::NonPremultiplied, // Using straight alpha
					CommonStates::DepthRead,
					CommonStates::CullClockwise,    // Using RH coordinates, and SDKMESH is in LH coordiantes
					rtState);

				m_modelEffects = m_model->CreateEffects(psd, alphapsd, m_modelResources->Heap(), m_states->Heap());
			}

			m_smallFont = std::make_unique<SpriteFont>(device, *m_resourceUpload,
				L"Assets\\SegoeUI_18.spritefont", //L"italic.spritefont",
				m_resourceDescriptors->GetCpuHandle(Descriptors::SegoeFont),
				m_resourceDescriptors->GetGpuHandle(Descriptors::SegoeFont));

			m_font = std::make_unique<SpriteFont>(device, *m_resourceUpload,
				L"Assets\\SegoeUI_18.spritefont",
				m_resourceDescriptors->GetCpuHandle(Descriptors::SegoeFont),
				m_resourceDescriptors->GetGpuHandle(Descriptors::SegoeFont));
			HANDLE completeEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
			auto loaded = m_resourceUpload->EndXaml(m_sceneDeviceResources->GetCommandQueue());
			WaitForSingleObject(m_resourceUpload->GetGPUHandle(), INFINITE);

			// solution XMMATRIX projection = XMLoadFloat4x4(&m_projection4x4);
			m_shapeEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_shapeTetraEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_lineEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));

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
void SceneRender::CreateWindowSizeDependentResources()
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
	if (!m_isRightHanded) {
		perspectiveMatrix = XMMatrixPerspectiveFovLH(
			fovAngleY,
			aspectRatio,
			0.01f,
			1000.0f
		);
	}
	else {
		perspectiveMatrix = XMMatrixPerspectiveFovRH(
			fovAngleY,
			aspectRatio,
			0.01f,
			1000.0f
		);
	}
	XMFLOAT4X4 orientation = m_sceneDeviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	DirectX::XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);

	// m_eye is at (0,0.7,10.0), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	//static const 
	DirectX::XMVECTOR eye, at, up;
	if (!m_isRightHanded)
	{
		eye = { m_camVars.m_EyeX, m_camVars.m_EyeY, m_camVars.m_EyeZ, 0.0f }; // away -(m_fCameraDistance)
		eye = XMVectorSet(m_camVars.m_EyeX, m_camVars.m_EyeY, m_camVars.m_EyeZ, 0.0f);
	}
	else
	{
		eye = { m_camVars.m_EyeX, m_camVars.m_EyeY, m_camVars.m_EyeZ, 0.0f }; // away m_fCameraDistance
	}

	at = { m_camVars.m_LookAtX, m_camVars.m_LookAtY, m_camVars.m_LookAtZ, 0.0f };
	at = XMVectorSet(m_camVars.m_LookAtX, m_camVars.m_LookAtY, m_camVars.m_LookAtZ, 0.0f);
	up = { m_camVars.m_UpX, m_camVars.m_UpY, m_camVars.m_UpZ, 0.0f };
	up = XMVectorSet(m_camVars.m_UpX, m_camVars.m_UpY, m_camVars.m_UpZ, 0.0f);
	XMFLOAT4 Feye = { m_camVars.m_EyeX, m_camVars.m_EyeY, m_camVars.m_EyeZ, 0.0f };
	XMFLOAT4 Fat = { m_camVars.m_LookAtX, m_camVars.m_LookAtY, m_camVars.m_LookAtZ, 0.0f };

	if (!m_isRightHanded)
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
void SceneRender::Update(DX::StepTimer const& timer)
{
	m_timer = timer;

	if (m_loadingComplete == true)
	{
		if (!m_tracking)
		{
			// Rotate the cube a small amount.
			m_angle += static_cast<float>(m_timer.GetElapsedSeconds()) * m_radiansPerSecond;

			Rotate(m_angle);
		}


		// Update the constant buffer resouRender(rce.
		
		PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

		m_camera->Eye(XMFLOAT3(m_camVars.m_EyeX, m_camVars.m_EyeY, m_camVars.m_EyeZ));
		m_camera->LookDirection(XMFLOAT3(m_camVars.m_LookAtX, m_camVars.m_LookAtY, m_camVars.m_LookAtZ));
		m_camera->UpDirection(XMFLOAT3(m_camVars.m_UpX, m_camVars.m_UpY, m_camVars.m_UpZ));

		XMVECTOR eye = XMVectorSet(m_camVars.m_EyeX, m_camVars.m_EyeY, m_camVars.m_EyeZ, 0.0f);
		XMVECTOR at = XMVectorSet(m_camVars.m_LookAtX, m_camVars.m_LookAtY, m_camVars.m_LookAtZ, 0.0f);
		XMVECTOR up = XMVectorSet(m_camVars.m_UpX, m_camVars.m_UpY, m_camVars.m_UpZ, 0.0f);

		XMFLOAT4 Feye = { m_camVars.m_EyeX, m_camVars.m_EyeY, m_camVars.m_EyeZ, 0.0f };
		XMFLOAT4 Fat = { m_camVars.m_LookAtX, m_camVars.m_LookAtY, m_camVars.m_LookAtZ, 0.0f };
		XMFLOAT4 Fup = { m_camVars.m_UpX, m_camVars.m_UpY, m_camVars.m_UpZ, 0.0f };
		if (!m_isRightHanded)
		{
			DirectX::XMStoreFloat4x4(&m_constantBufferData.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye, at, up)));
			XMStoreFloat4(&Feye, XMVector4Transform(XMLoadFloat4(&Feye), XMMatrixTranslation(Feye.x, Feye.y, Feye.z)));
			XMMatrixTranslation(Feye.x, Feye.y, Feye.z);
			XMStoreFloat4x4( &m_view4x4, DirectX::XMMatrixLookAtLH(eye, at, up));
			XMStoreFloat4x4(&m_world4x4, DirectX::XMMatrixRotationY(float(m_timer.GetTotalSeconds() * XM_PIDIV4)));
		}
		else {

			DirectX::XMStoreFloat4x4(&m_constantBufferData.view,
				XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
			XMStoreFloat4(&Feye, XMVector4Transform(XMLoadFloat4(&Feye), XMMatrixTranslation(Feye.x, Feye.y, Feye.z)));
			XMMatrixTranslation(Feye.x, Feye.y, Feye.z);
			XMStoreFloat4x4(&m_view4x4, DirectX::XMMatrixLookAtRH(eye, at, up));
			XMStoreFloat4x4(&m_world4x4, DirectX::XMMatrixRotationY(float(m_timer.GetTotalSeconds() * XM_PIDIV4)));
		}
		
		m_lineEffect->SetView(XMLoadFloat4x4(&m_view4x4));
		m_lineEffect->SetWorld(DirectX::XMMatrixIdentity());

		m_shapeEffect->SetView(XMLoadFloat4x4(&m_view4x4));
		m_shapeTetraEffect->SetView(XMLoadFloat4x4(&m_view4x4));

		m_sprites->SetViewport(m_sceneDeviceResources->GetScreenViewport());
		PIXEndEvent();
	}
}

#pragma endregion

#pragma region Frame Render

// Saves the current state of the renderer.
void SceneRender::SaveState()
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
void SceneRender::LoadState()
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
void SceneRender::Rotate(float radians)
{

	// Prepare to pass the updated model matrix to the shader.
	DirectX::XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void SceneRender::StartTracking()
{
	m_tracking = true;
}

BOOL SceneRender::checkDistance(float x, float y, float z, float mouseMoveDistDelta)
{
	BOOL tf = false;
	float xx = x - distX;
	if (xx < 0)xx = -xx;
	float yy = y - distY;
	if (yy < 0)yy = -yy;
	float zz = z - distZ;
	if (zz < 0)zz = -zz;

	if ((xx >= mouseMoveDistDelta) ||
		(yy >= mouseMoveDistDelta) ||
		(zz >= mouseMoveDistDelta))
	{

		distX = x; distY = y; distZ = z;
		tf = true;
	}
	return tf;

}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void SceneRender::TrackingUpdate(float positionX, float positionY)
{
	//MouseCursorRender(positionX, positionY);
	if (m_tracking)
	{
		D3D12_VIEWPORT rect = m_sceneDeviceResources->GetScreenViewport();
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
		point.x = positionX;
		point.y = positionY;
		if (m_loadingComplete) {

			if (m_bLButtonDown == true)
				if (positionX > 0 || point.x < (rect.TopLeftX - rect.Width)
					|| point.y > 0 || point.y < (rect.TopLeftY - rect.Height))
				{
					centerx = (int)(rect.Width - rect.TopLeftX) / 2;
					centery = (int)(rect.Height - rect.TopLeftY) / 2;
					x = (float)((point.x - centerx) * m_widthRatio);
					y = -(float)((point.y - centery) * m_heightRatio);
				}
		}
		else
		{
			centerx = (int)(rect.Width - rect.TopLeftX) / 2;
			centery = (int)(rect.Height - rect.TopLeftY) / 2;
			x = (float)(((point.x) - centerx) / (centerx / 28.0f));
			y = (float)-(((point.y) - centery) / (centery / 21.0f));
		}
		posX[m_iPointCount] = m_posX = x;
		posY[m_iPointCount] = m_posY = y;
		posZ[m_iPointCount] = m_posZ = z;
		m_iPointCount++;
		float radians = XM_2PI * 2.0f * positionX / m_sceneDeviceResources->GetOutputSize().Width;
		Rotate(radians);

		{
			XMFLOAT3 intersect3 = {};
			Hot3dx::CHot3dxD3D12Geometry* geo = new Hot3dx::CHot3dxD3D12Geometry();

			XMVECTOR a; XMVECTOR b; XMVECTOR c;

			a = { 50.0f, 50.0f, 0.0f, 0.0f };
			b = { 50.0f, -50.0f, 0.0f, 0.0f };
			c = { -50.0f, -50.0f, 0.0f, 0.0f };

			XMVECTOR eye = XMVectorSet(m_camVars.m_EyeX, m_camVars.m_EyeY, m_camVars.m_EyeZ, 0.0f);
			XMVECTOR lineDirection = XMVectorSet(m_camVars.m_LookAtX, m_camVars.m_LookAtY, m_camVars.m_LookAtZ, 0.0f);
			XMVECTOR planeNormal = geo->FindPlaneNormalVec(a, b, c);
			double planeConstant = geo->FindPlaneConstantVec(planeNormal, a);
			XMVECTOR intersect = geo->FindPointIntersectPlaneVec(planeNormal, eye, lineDirection, 20.0f);

			point.x = x;
			point.y = y;

			if (m_fPointSpace > 0.0F) {

				if (m_fScrollDist > 0.0F) {
					if (checkDistance(XMVectorGetX(intersect), XMVectorGetY(intersect), XMVectorGetZ(intersect), m_fScrollDist))
					{
						intersect3 = { XMVectorGetX(intersect), XMVectorGetY(intersect), XMVectorGetZ(intersect) };
						pos.push_back(intersect3);

						m_iTempGroup[m_iTempGroupCount] = m_iPointCount;
						m_iTempMouseX[m_iTempGroupCount] = point.x;
						m_iTempMouseY[m_iTempGroupCount] = point.y;
						m_iTempGroupCount++;
						m_iPointCount++;

					}//eo if(checkDistance(intersect.x, intersect.y, intersect.zz, m_fScrollDist)==true)
				}// eo if(m_fScrollDist>0.0F)
				else
				{

					pos.push_back(intersect3);

					m_iTempGroup[m_iTempGroupCount] = m_iPointCount;
					m_iTempMouseX[m_iTempGroupCount] = point.x;
					m_iTempMouseY[m_iTempGroupCount] = point.y;
					m_iTempGroupCount++;
					m_iPointCount++;
					
				}// eo else if(m_fScrollDist>0.0F)

				//Render();

			//}//eoif(SUCCEEDED(visual->Q

				pSect = intersect3;

			}// eo if(m_bLButtonDown==true){
		}

	}
}

void SceneRender::StopTracking()
{
	m_tracking = false;
}

void SceneRender::ReleaseDeviceDependentResources()
{

}

void SceneRender::OnDeviceLost()
{
	ReleaseDeviceDependentResources();

	m_texture1.Reset();
	m_texture2.Reset();

	m_font.reset();
	m_smallFont.reset();
	m_batch.reset();
	m_shape.reset();
	m_shapeTetra.reset();
	m_model.reset();
	m_lineEffect.reset();
	m_shapeEffect.reset();
	m_shapeTetraEffect.reset();
	m_modelEffects.clear();
	m_modelResources.reset();
	m_sprites.reset();
	m_resourceDescriptors.reset();
	m_states.reset();
	m_graphicsMemory.reset();
}

void SceneRender::OnDeviceRestored()
{
	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
	
}

void Hot3dxRotoDraw::SceneRender::SetStartCameraVars()
{
	if (m_isRightHanded) { m_camVars.m_EyeZ = 20.0f; }
	else{ m_camVars.m_EyeZ = -20.0f; }

	m_camVars.m_EyeX = 0.0f;
	m_camVars.m_EyeY = 0.0f;
	m_camVars.m_LookAtX = 0.0f;
	m_camVars.m_LookAtY = 0.01f;
	m_camVars.m_LookAtZ = 0.0f;
	m_camVars.m_UpX = 0.0f;
	m_camVars.m_UpY = 1.0f;
	m_camVars.m_UpZ = 0.0f;
}

void Hot3dxRotoDraw::SceneRender::MouseCursorRender(float positionX, float positionY)
{
	if (m_tracking)
	{
		if (m_bLButtonDown || m_bRButtonDown)
		{
			D3D12_RECT rect = m_sceneDeviceResources->GetScissorRect();
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

			if (m_loadingComplete) {
				if (positionX > 0 || point.x < (rect.left - rect.right)
					|| point.y > 0 || point.y < (rect.bottom - rect.top))
				{
					centerx = (rect.right - rect.left) / 2;
					centery = (rect.bottom - rect.top) / 2;
					x = (float)((point.x - centerx) / 7.5f);
					y = (float)-((point.y - centery) / 7.5f);
				}
			}
			
			XMFLOAT3 vCursor3dPos = XMFLOAT3(x, y, 0.0f);


			XMVECTOR xx, yy, zz;
			XMVECTOR m_vMouse3dPos;
			FXMVECTOR m_vXAxis = { 0.0f, 0.0f, 0.0f };
			xx = XMVectorScale(m_vXAxis, x);
			FXMVECTOR m_vYAxis = { 0.0f, 0.0f, 0.0f };
			yy = XMVectorScale(m_vYAxis, y);
			FXMVECTOR m_vZAxis = { 0.0f, 0.0f, 0.0f };
			zz = XMVectorScale(m_vZAxis, z);
			m_vMouse3dPos = { ((XMVectorGetX(yy) - XMVectorGetX(xx)) + m_camVars.m_LookAtX), ((XMVectorGetY(yy) - XMVectorGetY(xx)) + m_camVars.m_LookAtY), ((XMVectorGetZ(yy) - XMVectorGetZ(xx)) + m_camVars.m_LookAtZ) };


			if (m_bLButtonDown == true)
			{
				DrawPointsOne(m_vMouse3dPos, positionX, positionY);
				m_posX = XMVectorGetX(m_vMouse3dPos);
				m_posY = XMVectorGetY(m_vMouse3dPos);
				m_posZ = XMVectorGetZ(m_vMouse3dPos);
#ifdef _DEBUG
				OutputDebugString(TEXT("Inside m_bLButtonDown\n"));
#endif
			}

			// Turn on mouse cursor.
				// This disables relative mouse movement events.
			//CoreWindow::GetForCurrentThread()->PointerCursor = ref new CoreCursor(CoreCursorType::Cross, 1);

			ViewMatrix(m_view4x4, TEXT("m_view"));
			ViewMatrix(m_projection4x4, TEXT("m_projection"));
			//XMMatrixTransformation(D3DTS_WORLD, &matWorld);
			ViewMatrix(m_world4x4, TEXT("m_world"));

			
		} // eo mouseButtonDown if
	}// eo m_tracking if
}

void Hot3dxRotoDraw::SceneRender::DrawPointsOne(XMVECTOR intersect, float positiontX, float positionY)
{
	if (m_bLButtonDown == true)
	{
		if (m_fPointSpace > 0.0F) {

			if (m_fScrollDist > 0.0F) {
				if (checkDistance(XMVectorGetX(intersect), XMVectorGetY(intersect), XMVectorGetZ(intersect), m_fScrollDist))
				{

					posX[m_iPointCount] = XMVectorGetX(intersect);
					posY[m_iPointCount] = XMVectorGetY(intersect);
					posZ[m_iPointCount] = XMVectorGetZ(intersect);

					m_iTempGroup[m_iTempGroupCount] = m_iPointCount;
					m_iTempMouseX[m_iTempGroupCount] = point.x;
					m_iTempMouseY[m_iTempGroupCount] = point.y;

					m_iTempGroupCount++;
					m_iPointCount++;

				}//eo if(checkDistance(intersect.x, intersect.y, intersect.zz, m_fScrollDist)==true)
			}// eo if(m_fScrollDist>0.0F)
			else
			{
				posX[m_iPointCount] = XMVectorGetX(intersect);
				posY[m_iPointCount] = XMVectorGetY(intersect);
				posZ[m_iPointCount] = XMVectorGetZ(intersect);

				m_iTempGroup[m_iTempGroupCount] = m_iPointCount;
				m_iTempMouseX[m_iTempGroupCount] = point.x;
				m_iTempMouseY[m_iTempGroupCount] = point.y;
				m_iTempGroupCount++;
				m_iPointCount++;

			}// eo else if(m_fScrollDist>0.0F)

			pSect = XMFLOAT3(XMVectorGetX(intersect), XMVectorGetY(intersect), XMVectorGetZ(intersect));

		}// eo if(m_bLButtonDown==true){		
	}
}


void Hot3dxRotoDraw::SceneRender::Initialize()
{
	// Not Yet Used!!
	//----------------------------------------------------------------------

		//_In_ MoveLookController ^ controller,
		//_In_ GraphicRenderer ^ renderer


		// This method is expected to be called as an asynchronous task.
		// Care should be taken to not call rendering methods on the
		// m_renderer as this would result in the D3D Context being
		// used in multiple threads, which is not allowed.

		//m_controller = controller;
		//m_renderer = renderer;

	m_audioController = ref new Audio;
	m_audioController->CreateDeviceIndependentResources();
	Tetras tetras[1000] = {};
	for (int i = 0; i < 1000; i++)
	{
		tetras[i].m_shapeTetra = GeometricPrimitive::CreateTetrahedron(1.F);
	};
	//m_objects = std::vector<GraphicObject^>();
	//m_renderObjects = std::vector<GraphicObject^>();
	//m_level = std::vector<Level^>();

	//m_savedState = ref new PersistentState();
	//m_savedState->Initialize(ApplicationData::Current->LocalSettings->Values, "Graphic");

	StepTimer m_timer;// = new StepTimer();

	// Create a sphere primitive to represent the player.
	// The sphere will be used to handle collisions and constrain the player in the world.
	// It is not rendered so it is not added to the list of render objects.
	// It is added to the object list so it will be included in intersection calculations.
	//m_artistCamera = GeometricPrimitive::CreateSphere(1.0f);// XMFLOAT3(0.0f, -1.3f, 4.0f), 0.2f);
	//m_objects.push_back(m_artistDraw);
	//m_artistCamera->Active(true);

	m_camera = ref new Hot3dxCamera();
	m_camera->SetProjParams(XM_PI / 2, 1.0f, 0.01f, 100.0f);
	m_camera->SetViewParams(
		//m_artistCamera->Position(),            // Eye point in world coordinates.
		XMFLOAT3(m_camVars.m_EyeX, m_camVars.m_EyeY, m_camVars.m_EyeZ),
		XMFLOAT3(m_camVars.m_LookAtX, m_camVars.m_LookAtY, m_camVars.m_LookAtZ),     // Look at point in world coordinates.
		XMFLOAT3(m_camVars.m_UpX, m_camVars.m_UpY, m_camVars.m_UpZ)      // The Up vector for the camera.
	);

	//m_controller->Pitch(m_camera->Pitch());
	//m_controller->Yaw(m_camera->Yaw());

	
	// Instantiate a set of spheres to be used as ammunition for the Graphic
	// and set the material properties of the spheres.
	auto ammoHitSound = mediaReader->LoadMedia("Assets\\bounce.wav");

	for (unsigned int a = 0; a < m_iPointCount; a++)
	{
		m_tetraPoints[a].m_shapeTetra = GeometricPrimitive::CreateTetrahedron(0.1f);//ref new Sphere;
		
	}



	// Load the currentScore for saved state if it exists.
	LoadState();

	//m_controller->Active(false);

}

void Hot3dxRotoDraw::SceneRender::OnLButtonDown(UINT nFlags, XMFLOAT2 point)
{
	
}

void Hot3dxRotoDraw::SceneRender::OnRightButtonDown(UINT nFlags, XMFLOAT2 point)
{
	
}

void Hot3dxRotoDraw::SceneRender::OnMouseMove(UINT nFlags, XMFLOAT2 point)
{
	
}

// Renders one frame using the vertex and pixel shaders.
bool SceneRender::Render()
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
	// Draw procedurally generated XZ dynamic grid
	DirectX::XMMATRIX localgrid = XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(0.0f, 0.0f, -10.f);
	const XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
	const XMVECTORF32 yaxis = { 0.f, 0.f, 20.f };
	DrawGrid(xaxis, yaxis, g_XMZero, 20, 20, Colors::Aqua);
	// Draw procedurally generated XY dynamic grid
	const XMVECTORF32 xaxis1 = { 10.f, 0.f, 0.f };
	const XMVECTORF32 yaxis1 = { 0.f, 10.f, 0.f };
	const XMVECTORF32 zaxis1 = { 0.f, 0.f, 0.f };
	DrawGrid(xaxis1, yaxis1, zaxis1, 10, 10, Colors::Crimson);

	// Set the descriptor heaps
	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	// Draw sprite
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw sprite");
	m_sprites->Begin(commandList);
	m_sprites->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::WindowsLogo), GetTextureSize(m_texture2.Get()),
		XMFLOAT2(1, 1));// 10, 75));
	m_font->DrawString(m_sprites.get(), L"Hot3dx Hot3dxRotoDraw DirectXToolKitXaml12 App Sample Dev Res", XMFLOAT2(270, 30), Colors::Yellow);
	m_smallFont->DrawString(m_sprites.get(), L"DirectXToolKitXaml12 Sample Library Link Application", XMFLOAT2(10, 620), Colors::BlanchedAlmond);
	m_sprites->End();

	PIXEndEvent(commandList);
	// Draw 3D object
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw teapot");

	DirectX::XMMATRIX local = XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(-2.f, -2.f, 10.f);
	m_shapeEffect->SetWorld(local);
	m_shapeEffect->Apply(commandList);
	m_shape->Draw(commandList);
	for (unsigned int i = 0; i < m_iPointCount; i++)
	{
		DirectX::XMMATRIX localTetra = XMLoadFloat4x4(&m_world4x4) * DirectX::XMMatrixTranslation(posX[i], posY[i], posZ[i]);
		m_shapeTetraEffect->SetWorld(localTetra);
		m_shapeTetraEffect->Apply(commandList);
		m_shapeTetra->Draw(commandList);
	}
	PIXEndEvent(commandList);

	// Draw model
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw model");
	const XMVECTORF32 scale = { 0.01f, 0.01f, 0.01f };
	const XMVECTORF32 translate = { 3.f, -2.f, 10.f };
	// Old DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(XM_PI / 2.f, 0.f, -XM_PI / 2.f);

	XMVECTOR angles;
#if defined(_XM_ARM_NEON_INTRINSICS_)
	angles.n128_f32[0] = XM_PI / 2.f; angles.n128_f32[0] = 0.f; angles.n128_f32[0] = -XM_PI / 2.f;
#endif
#if defined(_XM_SSE_INTRINSICS_)
	angles = { XM_PI / 2.f, 0.f, -XM_PI / 2.f };
#endif
	XMVECTOR rotate = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles);
	// Old local = m_world * XMMatrixTransformation(g_XMZero, DirectX::SimpleMath::Quaternion::Identity, scale, g_XMZero, rotate, translate);
	local = XMLoadFloat4x4(&m_world4x4) * XMMatrixTransformation(g_XMZero, DirectX::XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
	Model::UpdateEffectMatrices(m_modelEffects, local, XMLoadFloat4x4(&m_view4x4), XMLoadFloat4x4(&m_projection4x4));
	heaps[0] = m_modelResources->Heap();
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_model->Draw(commandList, m_modelEffects.begin());

	PIXEndEvent(commandList);

	PIXEndEvent(commandList);

	// Show the new frame.
	PIXBeginEvent(m_sceneDeviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");

	m_sceneDeviceResources->Present();

	m_graphicsMemory->Commit(m_sceneDeviceResources->GetCommandQueue());

	PIXEndEvent(m_sceneDeviceResources->GetCommandQueue());

	return true;
}

// Helper method to clear the back buffers.
void SceneRender::Clear()
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


void XM_CALLCONV SceneRender::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
	auto commandList = m_sceneDeviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw grid");

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

		DirectX::VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
		DirectX::VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
		m_batch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= ydivs; i++)
	{
		float fPercent = float(i) / float(ydivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		DirectX::VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
		DirectX::VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
		m_batch->DrawLine(v1, v2);
	}

	m_batch->End();

	PIXEndEvent(commandList);
}

void Hot3dxRotoDraw::SceneRender::ViewMatrix(XMFLOAT4X4 m, wchar_t* str)
{

	XMMatrixSet(m._11, m._12, m._13, m._14,
		m._21, m._22, m._23, m._24,
		m._31, m._32, m._33, m._34,
		m._41, m._42, m._43, m._44
	);

	wchar_t* t = {};
	//OutputDebugString(t);
	//OutputDebugString(str);

	wchar_t* s = { L"\nThe Matrix values: \n%s\n%.6f  %.6f  %.6f %.6f\n%.6f  %.6f  %.6f %.6f\n%.6f  %.6f  %.6f %.6f\n%.6f  %.6f  %.6f %.6f\n" };
	swprintf_s(t, 1000, s, str, m._11, m._12, m._13, m._14,
		m._21, m._22, m._23, m._24,
		m._31, m._32, m._33, m._34,
		m._41, m._42, m._43, m._44);

	OutputDebugString(t);
}

XMMATRIX XM_CALLCONV Hot3dxRotoDraw::SceneRender::SetXMMatrix(DirectX::XMFLOAT4X4 m, XMMATRIX xm)
{
	return xm;
}

XMMATRIX XM_CALLCONV Hot3dxRotoDraw::SceneRender::GetXMMatrix(DirectX::XMFLOAT4X4 m)
{
	return XMLoadFloat4x4(&m);
}

void Hot3dxRotoDraw::SceneRender::ScreenMouse3DWorldAlignment()
{
	TCHAR dest[500] = {};;
	TCHAR* s = TEXT("%s\nW: %.6f H: %.6f \n");
	TCHAR* t = TEXT("The ratios:");

	StringCbPrintf(dest, 500, s, t, m_widthRatio, m_heightRatio);
	OutputDebugString(dest);
}
#pragma endregion

#pragma region Message Handlers
#pragma endregion

