//--------------------------------------------------------------------------------------
// File: 
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------
//
// File: App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "Generated Files\App.g.h"
#include "Generated Files\DirectXPage.g.h"
#include "DirectXPage.xaml.h"
#include "Common\DeviceResources.h"
#include "Hot3dxRotoDrawMain.h"
#include "Common/DirectXHelper.h"

using namespace DX;

namespace Hot3dxRotoDraw
{
		/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed
	{
	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnResuming(Platform::Object ^sender, Platform::Object ^args);
		void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);
		DirectXPage^ m_directXPage;

		// Private accessor for m_deviceResources, protects against device removed errors.
		//std::shared_ptr<DX::DeviceResources> GetDeviceResourcesApp();


		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<Hot3dxRotoDrawMain> m_main;
	};
}