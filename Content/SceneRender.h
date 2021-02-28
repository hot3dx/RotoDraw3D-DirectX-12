//--------------------------------------------------------------------------------------
// File: SceneRender.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------
#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include <Graphics\AlignedNewXaml12.h>
#include <Graphics\RenderTargetStateXaml12.h>
#include <Graphics\EffectPipelineStateDescriptionXaml12.h>
#include <Graphics\CommonStatesXaml12.h>
#include <Graphics\GraphicsMemoryXaml12.h>
#include <Graphics\DescriptorHeapXaml12.h>
#include <Graphics\EffectCommonXaml12.h>
#include <Graphics\VertexTypesXaml12.h>
#include <Graphics\SimpleMathXaml12.h>
#include <Graphics\ModelXaml12.h>
#include <Graphics\PrimitiveBatchXaml12.h>
#include <Graphics\GeometricPrimitiveXaml12.h>
#include <Graphics\SpriteBatchXaml12.h>
#include <Graphics\SpriteFontXaml12.h>
#include <Graphics\Hot3dxCamera.h>
#include <Audio\AudioXaml12.h>
#include <Audio\MediaReaderXaml12.h>
#include <Graphics\Hot3dxGeometry.h>
#include <thread>
#include <chrono>
#include <assert.h>


namespace Hot3dxRotoDraw
{
	
	// This sample renderer instantiates a basic rendering pipeline.
	ref class SceneRender
	{
	public:virtual~SceneRender();
	internal:
		SceneRender(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		
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
		void SetStartCameraVars();
		
		XMMATRIX XM_CALLCONV SetXMMatrix(DirectX::XMFLOAT4X4 m, XMMATRIX xm);
		XMMATRIX XM_CALLCONV GetXMMatrix(DirectX::XMFLOAT4X4 m);
		//CCameraXYMoveRotation          m_CamXYMoveRotate;
		void MouseCursorRender(float positionX, float positionY);
		void DrawPointsOne(XMVECTOR intersect, float positiontX, float positionY);
		
		bool GetLoadingComplete() { return m_loadingComplete; }
		void SetLoadingComplete(bool complete) { m_loadingComplete = complete; }

		void LoadState();
		void Rotate(float radians);
		void XM_CALLCONV DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color);
		void ScreenMouse3DWorldAlignment();
		void Initialize();
		void OnLButtonDown(UINT nFlags, XMFLOAT2 point);
		void OnRightButtonDown(UINT nFlags, XMFLOAT2 point);
		void ViewMatrix(XMFLOAT4X4 m, wchar_t* str);
		void OnMouseMove(UINT nFlags, XMFLOAT2 point);

		// Accessors
		bool Getm_bLButtonDown() { return m_bLButtonDown; }
		bool Getm_bRButtonDown() { return m_bRButtonDown; }
		bool Getm_bMButtonDown() { return m_bRButtonDown; }
		bool Getm_bMouseMove() { return m_bMouseMove; }
		void Setm_bLButtonDown(bool b) { m_bLButtonDown = b; }
		void Setm_bRButtonDown(bool b) { m_bRButtonDown = b; }
		void Setm_bMButtonDown(bool b) { m_bRButtonDown = b; }
		void Setm_bMouseMove(bool b) { m_bMouseMove = b; }

		Hot3dxCamera^ GetCamera() { return m_camera; }
		void Setm_EyeX(float e) { m_camVars.m_EyeX += e; }
		void Setm_EyeY(float e) { m_camVars.m_EyeY += e; }
		void Setm_EyeZ(float e) { m_camVars.m_EyeZ += e; }
		void Setm_LookAtX(float e) { m_camVars.m_LookAtX += e; }
		void Setm_LookAtY(float e) { m_camVars.m_LookAtY += e; }
		void Setm_LookAtZ(float e) { m_camVars.m_LookAtZ += e; }
		void Setm_UpX(float e) { m_camVars.m_UpX += e; }
		void Setm_UpY(float e) { m_camVars.m_UpY += e; }
		void Setm_UpZ(float e) { m_camVars.m_UpZ += e; }
		//void RotatePitch(float degree);
		//void RotateYaw(float degree);

	protected private:
		// Indices into the application state map.
		Platform::String^ AngleKey = "Angle";
		Platform::String^ TrackingKey = "Tracking";

		bool                           is3DVisible;

		XMFLOAT3 pSect;
		bool m_bFaceSelected;
		int m_iV;
		bool m_bLButtonDown;
		bool m_bRButtonDown;
		bool m_bMButtonDown;
		bool m_bMouseMove;
		
		unsigned int m_iPointCount;
		unsigned int m_iTotalPointCount;
		unsigned int m_iLastPoint;

		XMFLOAT2 point;
		Platform::Array<unsigned int>^ m_iTempGroup = ref new Platform::Array<unsigned int>(10000);
		Platform::Array<float>^ m_iTempMouseX = ref new Platform::Array<float>(10000);
		Platform::Array<float>^ m_iTempMouseY = ref new Platform::Array<float>(10000);

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
		float	m_radiansPerSecond;
		float	m_angle;
		bool	m_tracking;
		UINT    sceneVertexCount;
		// Track our independent input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		//Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;
		/////////////
		// DirectXTK Test objects.
		std::shared_ptr<DirectX::ResourceUploadBatch>                           mesourceUpload;
		std::unique_ptr<DirectX::GraphicsMemory>                                m_graphicsMemory;
		std::unique_ptr<DirectX::DescriptorHeap>                                m_resourceDescriptors;
		std::unique_ptr<DirectX::CommonStates>                                  m_states;
		std::unique_ptr<DirectX::BasicEffect>                                   m_lineEffect;
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
		std::unique_ptr<DirectX::BasicEffect>                                   m_shapeEffect;
		std::unique_ptr<DirectX::BasicEffect>                                   m_artistCameraEffect;
		std::unique_ptr<DirectX::BasicEffect>                                   m_shapeTetraEffect;
		std::unique_ptr<DirectX::Model>                                         m_model;
		std::vector<std::shared_ptr<DirectX::IEffect>>                          m_modelEffects;
		std::unique_ptr<DirectX::EffectTextureFactory>                          m_modelResources;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_shape;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_artistCamera;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_shapeTetra;
		std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
		std::unique_ptr<DirectX::SpriteFont>                                    m_smallFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_font;

		//std::unique_ptr<DirectX::AudioEngine>                                   m_audEngine;
		//std::unique_ptr<DirectX::WaveBank>                                      m_waveBank;
		//std::unique_ptr<DirectX::SoundEffect>                                   m_soundEffect;
		//std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect1;
		//std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect2;

		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_texture1;
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_texture2;


		// Solution for Win32 warning is to use below instead of XMMATRIX
		//DirectX::SimpleMath::Matrix
		DirectX::XMFLOAT4X4                                                        m_world4x4;
		//DirectX::SimpleMath::Matrix  
		DirectX::XMFLOAT4X4                                                        m_view4x4;
		//DirectX::SimpleMath::Matrix   
		DirectX::XMFLOAT4X4 	                                              m_projection4x4;
		
		// Descriptors
		enum Descriptors
		{
			WindowsLogo,
			SeaFloor,
			SegoeFont,
			Count = 256
		};




		Hot3dxCamera^ m_camera;
		struct CameraVars
		{
			float m_EyeX;
			float m_EyeY;
			float m_EyeZ;
			float m_LookAtX;
			float m_LookAtY;
			float m_LookAtZ;
			float m_UpX;
			float m_UpY;
			float m_UpZ;
		};
		CameraVars m_camVars;
		
		  Audio^ m_audioController;
		  MediaReader^ mediaReader = ref new MediaReader();
		  // Screen Draw Variables
		  std::unique_ptr<Hot3dx::CHot3dxD3D12Geometry> m_geo;
		  struct Tetras
		  {
			  std::unique_ptr<DirectX::GeometricPrimitive>                   m_shapeTetra;
		  };
		  UINT32                                      m_tetraCount;
		  UINT32                                      m_tetraNext;
		  std::vector<Tetras>                                                m_tetraPoints;
		  float m_posX;
		  float m_posY;
		  float m_posZ;

		  Platform::Array<float>^ posX = ref new Platform::Array<float>(10000);
		  Platform::Array<float>^ posY = ref new Platform::Array<float>(10000);
		  Platform::Array<float>^ posZ = ref new Platform::Array<float>(10000);

		  // Set handed direction of world flag
		  bool    m_isRightHanded;
	};
}



