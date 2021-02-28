#pragma once

#include <pch.h>
#include "Scenario2_Normal.xaml.h"
#include "Scenario5_MatsTexs.xaml.h"


namespace Hot3dxRotoDraw
{
	
	
	ref class Hot3dxRotoDrawVariables sealed
	{
      public:
	       Hot3dxRotoDrawVariables();
		   virtual ~Hot3dxRotoDrawVariables();

		   DirectXPage^ GetDXPage() { return _dxPage; }
		   
		   //Accessors
		   Windows::UI::Xaml::Media::SolidColorBrush^ 
			   Getm_brushFrontFaceColorRV() {return m_brushFrontFaceColorRV;}
		   Windows::UI::Xaml::Media::SolidColorBrush^
			   Getm_brushBackFaceColorRV() { return m_brushBackFaceColorRV; }
		   void Setm_brushBackFaceColorRV(Windows::UI::Color color) {
			   m_brushBackFaceColorRV = ref new Windows::UI::Xaml::Media::SolidColorBrush(color);
		   }
		   void Setm_brushFrontFaceColorRV(Windows::UI::Color color) {
			   m_brushFrontFaceColorRV = ref new Windows::UI::Xaml::Media::SolidColorBrush(color);
		   }


		   Windows::UI::Color SetColor(bool m, unsigned char a, unsigned char r, unsigned char g, unsigned char b) {
			   //int a, int r, int g, int b) {
			   if (m)
			   {
				   fcolorRV.R = r;
				   fcolorRV.G = g;
				   fcolorRV.B = b;
				   fcolorRV.A = a;
				   return fcolorRV;
			   }
			   else {
				   bcolorRV.R = r;
				   bcolorRV.G = g;
				   bcolorRV.B = b;
				   bcolorRV.A = a;
				   return  bcolorRV;
			   }
		   }

		   bool GetAddTopFaces() { return m_bAddTopFacesRV; }
		   void SetAddTopFaces(bool b) { m_bAddTopFacesRV = b; }
		   bool GetAddBottomFaces() { return m_bAddBottomFacesRV; }
		   void SetAddBottomFaces(bool b) { m_bAddBottomFacesRV = b; }

	private:	
		   DirectXPage^ _dxPage;
		   Windows::UI::Xaml::Media::SolidColorBrush^ m_brushFrontFaceColorRV;
		   Windows::UI::Xaml::Media::SolidColorBrush^ m_brushBackFaceColorRV;
		   Windows::UI::Color fcolorRV;
		   Windows::UI::Color bcolorRV;

		   bool m_bAddTopFacesRV;
		   bool m_bAddBottomFacesRV;
	};

	using namespace DX;
	using namespace Windows::UI::Core;
	using namespace Windows::UI::Xaml::Media::Imaging;

	public enum class NotifyType
	{
		StatusMessage,
		ErrorMessage
	};

	public ref class Scenario2Vars sealed
	{
	public:
		Scenario2Vars() {
		}
		virtual ~Scenario2Vars() {}

		Scenario2_Normal^ GetScenario2Page() { return _scenario2Page; }
		void SetScenario2Page(Scenario2_Normal^ s) { _scenario2Page = s;  }
		// Accessors
		bool GetDoFrontFaces() { return m_bDoFrontFaces;  }
		void SetDoFrontFaces(bool b) { m_bDoFrontFaces = b; }

		bool GetDoBackFaces() { return m_bDoBackFaces; }
		void SetDoBackFaces(bool b) { m_bDoBackFaces = b; }

		bool GetFirstToLast() { return m_bFirstToLast; }
		void SetFirstToLast(bool b) { m_bFirstToLast = b; }

		bool GetAxisChecked() { return m_bAxisChecked; }
		void SetAxisChecked(bool b) { m_bAxisChecked = b; }

		bool GetTopOrLeftChecked() { return m_bTopOrLeftChecked; }
		void SetTopOrLeftChecked(bool b) { m_bTopOrLeftChecked = b; }
		
		bool GetBottomOrRightChecked() { return m_bBottomOrRightChecked; }
		void SetBottomOrRightChecked(bool b) { m_bBottomOrRightChecked = b; }
		
		Windows::UI::Color GetPointColor() { return m_pointColor; }
		void SetPointColor(Windows::UI::Color b) { m_pointColor = b; }
		
		bool GetOpenOrClosedChecked() { return m_bOpenOrClosedChecked; }
		void SetOpenOrClosedChecked(bool b) { m_bOpenOrClosedChecked = b; }

		float GetPartialRotateAngle() { return m_fPartialRotateAngle; }
		void SetPartialRotateAngle(float angle) { m_fPartialRotateAngle = angle; }

		int Get() { return m_iCount; }
		void Set(int b) { m_iCount = b; }
		void SetCheckBox(Windows::UI::Xaml::Controls::CheckBox^ b, bool tf) { b->IsChecked = tf; }
		//Windows::UI::Xaml::Controls::TextBox^ GetPointSpaceTextBox() { return m_PointSpaceTextBox; }
		//void SetPointSpaceTextBox(Windows::UI::Xaml::Controls::TextBox^ b) { m_PointSpaceTextBox = b; }

	private:
		Scenario2_Normal^ _scenario2Page;
		bool m_bDoFrontFaces;
		bool m_bDoBackFaces;
		bool m_bFirstToLast;
		bool m_bAxisChecked;
		bool m_bTopOrLeftChecked;
		bool m_bBottomOrRightChecked;
		Windows::UI::Color m_pointColor;
		//Windows::UI::Xaml::Controls::TextBox^ m_PointSpaceTextBox;
		bool m_bOpenOrClosedChecked;
		float m_fPartialRotateAngle;
		int m_iCount;
	};

	public ref class Scenario5Vars sealed
	{
	public:
		Scenario5Vars() {

			// Initialize Colors
			r1 = 255; g1 = 255; b1 = 255; a1 = 255;
			r2 = 255; g2 = 255; b2 = 255; a2 = 255;

			float ar[14] = { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

			// initialize materials with ar[] values
			for (int i = 0; i < 14; i++)
			{
				args1->set(i, ar[i]);
				args2->set(i, ar[1]);
			}

			fCount1 = 0;
			fCount2 = 0;

			wchar_t* fn = L"NONE1";
			for (int i = 0; i < 10; i++)
			{
				wcfileName1->set(i, fn[i]);
			}
			fn = L"NONE2";
			for (int i = 0; i < 10; i++)
			{
				wcfileName2->set(i, fn[i]);
			}
			fn = nullptr;

			nCount1 = 0;
			nCount2 = 0;

			wchar_t* fn1 = L"NONE1";
			for (int i = 0; i < 10; i++)
			{
				matName1->set(i, fn1[i]);
			}
			fn1 = L"NONE2";
			for (int i = 0; i < 10; i++)
			{
				matName2->set(i, fn1[i]);
			}
			fn1 = nullptr;
		}
		virtual ~Scenario5Vars() {}

		//Accessors
		Scenario5_MatsTexs^ GetScenario5Page() { return _scenario5Page; }
		void SetScenario5Page(Scenario5_MatsTexs^ s) { _scenario5Page = s; }

		Windows::UI::Color GetRGBA1() { return color1; }
		void SetRGBA1(unsigned int c) {
			a1 = ((c >> 24) & 0xff), r1 = ((c) & 0xff), g1 = ((c >> 8) & 0xff), b1 = ((c >> 16) & 0xff);
			color1.A = a1;
			color1.R = r1;
			color1.G = g1;
			color1.B = b1;
		}
		void SetColor1(Windows::UI::Color c) {
			color1.A = a1 = c.A;
			color1.R = r1 = c.R;
			color1.G = g1 = c.G;
			color1.B = b1 = c.B;
		}
		Windows::UI::Color GetRGBA2() { return color2; }
		void SetRGBA2(unsigned int c) {
			a2 = ((c >> 24) & 0xff), r2 = ((c) & 0xff), g2 = ((c >> 8) & 0xff), b2 = ((c >> 16) & 0xff);
			color2.A = a2;
			color2.R = r2;
			color2.G = g2;
			color2.B = b2;
		}
		void SetColor2(Windows::UI::Color col) {
			color2.A = a2 = col.A;
			color2.R = r2 = col.R;
			color2.G = g2 = col.G;
			color2.B = b2 = col.B;
		}

		// Hot3dxRotoDrawSceneRender Texture Filename Accessors Scenario5_MatsTexs stuff
		Windows::Storage::StorageFile^ GetFile1() { return fileImage1; }
		void SetFile1(Windows::Storage::StorageFile^ file) { fileImage1 = file; }
		Windows::Storage::StorageFile^ GetFile2() { return fileImage2; }
		void SetFile2(Windows::Storage::StorageFile^ file) { fileImage2 = file; }
		Platform::String^ GetTextureImage1File() { return fileName1; }// m_textureImage1File
		void SetTextureImage1File(Platform::String^ fileName) {
			fileName1 = nullptr;
			fileName1 = ref new Platform::String(fileName->Data());
			fileName1 = fileName;
		}
		Platform::String^ GetTextureImage2File() { return fileName2; }// m_textureImage2File
		void SetTextureImage2File(Platform::String^ fileName) {
			fileName2 = nullptr;
			fileName2 = ref new Platform::String(fileName->Data());
			fileName2 = fileName;
		}

		Windows::Storage::FileProperties::StorageItemThumbnail^ GetThumbnailDXP1() { return m_thumbnailDXP1; }
		void SetThumbnailDXP1(Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail) { m_thumbnailDXP1 = thumbnail; }
		Windows::Storage::FileProperties::StorageItemThumbnail^ GetThumbnailDXP2() { return m_thumbnailDXP2; }
		void SetThumbnailDXP2(Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail) { m_thumbnailDXP2 = thumbnail; }
		BitmapImage^ GetBitmapImageDXP1() { return bitmapImageDXP1; }
		void SetbitmapImageDXP1(Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail)
		{
			bitmapImageDXP1 = ref new BitmapImage();
			bitmapImageDXP1->SetSourceAsync(thumbnail);
		}
		BitmapImage^ GetBitmapImageDXP2() { return bitmapImageDXP2; }
		void SetbitmapImageDXP2(Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail)
		{
			bitmapImageDXP2 = ref new BitmapImage();
			bitmapImageDXP2->SetSource(thumbnail);
		}

		Platform::Array<float>^ GetMatArgs1Array() { return args1; }
		void SetMatArgs1Array(unsigned int i, float val) { args1->set(i, val); }
		Platform::Array<float>^ GetMatArgs2Array() { return args2; }
		void SetMatArgsArray(unsigned int i, float val) { args2->set(i, val); }

		Platform::Array<wchar_t>^ GetMatName1() { return matName1; }
		Platform::Array<wchar_t>^ GetMatName2() { return matName2; }
		void SetMatName1(const Platform::Array<wchar_t>^ name) {
			matName1 = nullptr;
			matName1 = ref new  Platform::Array<wchar_t>(name->Length);
			for (unsigned int i = 0; i < name->Length; i++)
			{
				matName1->set(i, name->get(i));
			}
		}
		void SetMatName2(const Platform::Array<wchar_t>^ name) {
			matName2 = nullptr;
			matName2 = ref new  Platform::Array<wchar_t>(name->Length);
			for (unsigned int i = 0; i < name->Length; i++)
			{
				matName2->set(i, name->get(i));
			}
		}

	private:
		Scenario5_MatsTexs^ _scenario5Page;
		Windows::Storage::StorageFile^ fileImage1;
		Windows::Storage::StorageFile^ fileImage2;
		Windows::Storage::FileProperties::StorageItemThumbnail^ m_thumbnailDXP1;
		Windows::Storage::FileProperties::StorageItemThumbnail^ m_thumbnailDXP2;
		Platform::String^ fileName1;
		Platform::String^ fileName2;
		BitmapImage^ bitmapImageDXP1;
		BitmapImage^ bitmapImageDXP2;

		unsigned char r1, g1, b1, a1;
		unsigned char r2, g2, b2, a2;
		Platform::Array<float>^ args1 = ref new Platform::Array<float>(14);
		Platform::Array<float>^ args2 = ref new Platform::Array<float>(14);

		unsigned int fCount1;
		unsigned int fCount2;

		Platform::Array<wchar_t>^ wcfileName1 = ref new Platform::Array<wchar_t>(512);
		Platform::Array<wchar_t>^ wcfileName2 = ref new Platform::Array<wchar_t>(512);

		unsigned int nCount1;
		unsigned int nCount2;

		Platform::Array<wchar_t>^ matName1 = ref new Platform::Array<wchar_t>(50);
		Platform::Array<wchar_t>^ matName2 = ref new Platform::Array<wchar_t>(50);

		Windows::UI::Color color1;
		Windows::UI::Color color2;

	}; 
}