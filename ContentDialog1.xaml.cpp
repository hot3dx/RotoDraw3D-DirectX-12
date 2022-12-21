//
// MyContentDialog.xaml.cpp
// Implementation of the MyContentDialog class
//

#include "pch.h"
#include "ContentDialog1.xaml.h"

using namespace Hot3dxRotoDraw;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

Hot3dxRotoDraw::ContentDialog1::ContentDialog1()
{
	InitializeComponent();

	//auto result = ShowAsync();
	//if (result->GetResults() == ContentDialogResult::Primary) {}
	//if (result->GetResults() == ContentDialogResult::Secondary) {}
}

void Hot3dxRotoDraw::ContentDialog1::ContentDialog_SetTitle(Platform::String^ title)
{
	Title = title;
}

void Hot3dxRotoDraw::ContentDialog1::ContentDialog_SetTextBlock(Platform::String^ textBody)
{
	textBlock->Text = textBody;
}

void Hot3dxRotoDraw::ContentDialog1::ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
}

void Hot3dxRotoDraw::ContentDialog1::ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
}
