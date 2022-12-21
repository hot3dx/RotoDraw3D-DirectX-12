//
// ContentDialog.xaml.h
// Declaration of the ContentDialog class
//

#pragma once

#include "ContentDialog1.g.h"

using namespace Windows::UI::Xaml::Controls;

namespace Hot3dxRotoDraw
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContentDialog1 sealed
	{
	public:
		virtual ~ContentDialog1() {}
	internal:
		ContentDialog1();
	
		void ContentDialog_SetTitle(Platform::String^ title);
		void ContentDialog_SetTextBlock(Platform::String^ textBody);
		void ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
		void ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
	private:
	};
}
