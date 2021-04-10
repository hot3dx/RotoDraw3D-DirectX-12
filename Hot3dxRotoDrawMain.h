//--------------------------------------------------------------------------------------
// File: Hot3dxRotoDrawMain.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------

#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Hot3dxRotoDrawSceneRender.h"

// This path works because although Hot3dxRotoDraw and DirectXToolkitXaml12 are
// in a separate folder they are both within the next folder up!
// On your computer this and the /WHOLEARCHIVE folders below must match!!!
// The goal will be to put this library in Windows Kits in the appropriate 10.0.18362.0 folders!
// Or C://Hot3dx
// 
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

/* In Project > Hot3dxRotoDraw Properties > Configuration Properties > Linker > All Options > Additional Options
// This line is for x64//Debug
// /WHOLEARCHIVE:C://Users//hot3dx-home//Source//Repos//Xaml//DirectXToolKitXaml12//x64//Debug//DirectXToolKitXaml12//DirectXToolKitXaml12.lib %(AdditionalOptions)
// This line is for x64//Release
// /WHOLEARCHIVE:C://Users//hot3dx-home//Source//Repos//Xaml//DirectXToolKitXaml12//x64//Release//DirectXToolKitXaml12//DirectXToolKitXaml12.lib %(AdditionalOptions)
// This line is for x86(Win32)//Debug
// /WHOLEARCHIVE:C://Users//hot3dx-home//Source//Repos//Xaml//DirectXToolKitXaml12//Debug//DirectXToolKitXaml12//DirectXToolKitXaml12.lib %(AdditionalOptions)
// This line is for x86(Win32)//Release
// /WHOLEARCHIVE:C://Users//hot3dx-home//Source//Repos//Xaml//DirectXToolKitXaml12//Release//DirectXToolKitXaml12//DirectXToolKitXaml12.lib %(AdditionalOptions)
// Each Path is different for each compiled and loinked library
// This switch removes warning LNK4264 caused by 
// In Project > Hot3dxRotoDraw Properties > Configuration Properties > C++ > All Options > Consume Windows Runtime Extensions 
// Yes(/ZW) option
*/


//#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace Hot3dxRotoDraw
{
	class Hot3dxRotoDrawMain : public DX::IDeviceNotify
	{
	public:
		Hot3dxRotoDrawMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		virtual ~Hot3dxRotoDrawMain();
		void CreateWindowSizeDependentResources();
		void CreateDeviceDependentResources();
		void Update();
		bool Render();
		void Clear();
		void OnSuspending();
		void OnWindowSizeChanged();
		void ValidateDevice();
		void OnResuming();
		void OnDeviceRemoved();
		void StartTracking() { m_sceneRenderer->StartTracking(); }
		void TrackingUpdate(float positionX, float positionY) { m_pointerLocationX = positionX; m_pointerLocationY = positionY; }
		void StopTracking() { m_sceneRenderer->StopTracking(); }
		bool IsTracking() { return m_sceneRenderer->IsTracking(); }
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		// Accessors

		//RotoDrawSceneRender* GetSceneRenderer(){ return m_sceneRenderer.get();}
		RotoDrawSceneRender^ GetSceneRenderer() { return m_sceneRenderer; }
		void SetSceneRenderer() {
			//m_sceneRenderer = std::unique_ptr<RotoDrawSceneRender>(new RotoDrawSceneRender(m_deviceResources)); 
			m_sceneRenderer = ref new RotoDrawSceneRender(m_deviceResources);
		}
		//CD3D12GridXaml* GetCD3D12GridRenderer() { return m_cd3d12GridRenderer.get(); }

		void PauseRequested() {
			m_timer.Stop();
			m_pauseRequested = true;
		};
		void PressComplete() {// if (m_updateState == RotoDraw3DX12::UpdateEngineState::WaitingForPress) 
			m_pressComplete = true;
		};


		void WindowActivationChanged(Windows::UI::Core::CoreWindowActivationState activationState);

		void KeyDown(Windows::System::VirtualKey key);
		void KeyUp(Windows::System::VirtualKey key);

		DX::StepTimer GetTimer() { return m_timer; }

	private:
		// Process all input from the user before updating game state
		void ProcessInput();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		//std::unique_ptr<RotoDrawSceneRender> m_sceneRenderer;
		RotoDrawSceneRender^ m_sceneRenderer;
		//std::unique_ptr<RotoDrawFpsTextRender> m_fpsTextRenderer;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		///////////////////////////////////////////////////
		bool                                                m_pauseRequested;
		bool                                                m_pressComplete;
		bool                                                m_renderNeeded;
		bool                                                m_haveFocus;
		bool                                                m_visible;

		//Hot3dxController^ m_controller;

		///////////////////////////////////////////////////

		// Track current input pointer position.
		float m_pointerLocationX;
		float m_pointerLocationY;

		Windows::UI::Core::CoreWindowActivationState m_activationState;
		Windows::System::VirtualKey m_key;


	};
}
