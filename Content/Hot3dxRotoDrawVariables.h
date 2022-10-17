#pragma once

#include <pch.h>
#include "Scenario2_Normal.xaml.h"
#include "Scenario5_MatsTexs.xaml.h"
#include "Scenario11_GridorPic.xaml.h"


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
                    m_pointColor = { 255,0,0,0};
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
		
		bool GetGridPicChecked() { return m_bGridPicChecked; }
		void SetGridPicChecked(bool b) { m_bGridPicChecked = b; }

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
		bool m_bGridPicChecked;
		Windows::UI::Color m_pointColor;
		//Windows::UI::Xaml::Controls::TextBox^ m_PointSpaceTextBox;
		bool m_bOpenOrClosedChecked;
		float m_fPartialRotateAngle;
		int m_iCount;
	};
	
	struct MaterialData
	{
		Platform::Array<float>^ args1 = ref new Platform::Array<float>(14);
		unsigned int fCount;
		Platform::String^ wcfileName = ref new Platform::String();
		unsigned int nCount;
		Platform::String^ matName = ref new Platform::String();
		Platform::String^ nodeName = ref new Platform::String();
		Platform::String^ effectName = ref new Platform::String();
	};

	ref class MaterialDataVector
	{
	public:
		virtual ~MaterialDataVector() { }
		

	internal:
		MaterialDataVector() {};
		std::vector<MaterialData> GetMaterialDataVector() { return m_materialDataVector; }
		MaterialData GetMaterialsList(unsigned int n)
		{
			return m_materialDataVector.at(n);
		}
		void SetMaterialsList(MaterialData d) { m_materialDataVector.push_back(d); }

	private:
		std::vector<MaterialData> m_materialDataVector;
		//Platform::Array<MaterialData*> m_materialDataArray = ref new Platform::Array<MaterialData>(2);
	};

	ref class MaterialGroups sealed
	{
	public:
		MaterialGroups() {};
		virtual ~MaterialGroups() { ZeroOutMaterialList(); }
		Platform::Array<uint16_t>^ GetMaterialList() { return m_MaterialList; }
		void SetMaterialList(unsigned int i, uint16_t val) { m_MaterialList->set(i, val); }
		uint16_t GetListMaterial(unsigned int i) { return m_MaterialList->get(i); }
		void ZeroOutMaterialList() { m_MaterialList->~Array(); }
		
	
	private:
		Platform::Array<uint16_t>^ m_MaterialList = ref new Platform::Array<uint16_t>(1000);

	};

	public ref class Scenario5Vars sealed
	{
	public:
		virtual ~Scenario5Vars() {}
	internal:
		Scenario5Vars() {
			m_materialDataVector = ref new MaterialDataVector();
			MaterialData data;
			// Initialize Colors
			r1 = 255; g1 = 255; b1 = 255; a1 = 255;
			r2 = 255; g2 = 255; b2 = 255; a2 = 255;

			float ar[14] = { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
			float arMtl[14] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
			// initialize materials with ar[] values
			for (unsigned int i = 0; i < 14; i++)
			{
				args1->set(i, ar[i]);
				data.args1->set(i, arMtl[i]);
				args2->set(i, ar[1]);
			}

			fCount1 = 0;
			fCount2 = 0;
			data.fCount = 0;

			wchar_t* fn = L"Marble.dds";
			for (unsigned int i = 0; i < 10; i++)
			{
				wcfileName1->set(i, fn[i]);
				
			}
			fn = L"nightceiling.dds";
			for (unsigned int i = 0; i < 10; i++)
			{
				wcfileName2->set(i, fn[i]);
			}
			fn = nullptr;

			nCount1 = 0;
			nCount2 = 0;
			data.nCount = 0;

			wchar_t* fn1 = L"NONE1";
			for (unsigned int i = 0; i < 10; i++)
			{
				matName1->set(i, fn1[i]);
				
			}
			fn1 = L"NONE2";
			for (unsigned int i = 0; i < 10; i++)
			{
				matName2->set(i, fn1[i]);
			}
			
			        data.wcfileName = ref new Platform::String();
			        data.matName = ref new Platform::String(L"NONE1");
			        data.nodeName = ref new Platform::String(L"CubeNode");
					data.effectName = ref new Platform::String(L"Phong[13]");
					m_materialDataVector->SetMaterialsList(data);
			fn1 = nullptr;
			
			
		}
		

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
		Windows::UI::Xaml::Media::Imaging::BitmapImage^ GetBitmapImageDXP1() { return bitmapImageDXP1; }
		void SetbitmapImageDXP1(Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail)
		{
			bitmapImageDXP1 = ref new BitmapImage();
			bitmapImageDXP1->SetSourceAsync(thumbnail);
		}
		Windows::UI::Xaml::Media::Imaging::BitmapImage^ GetBitmapImageDXP2() { return bitmapImageDXP2; }
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

		Hot3dxRotoDraw::MaterialDataVector^ GetMaterialGroupsData() { return m_materialDataVector; }

	private:
		Scenario5_MatsTexs^ _scenario5Page;
		Scenario11_GridorPic^ _scenario11Page;
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

		Hot3dxRotoDraw::MaterialDataVector^ m_materialDataVector;


	}; 

	public ref class Scenario11Vars sealed
	{
	public:
		virtual ~Scenario11Vars() {}
	internal:
		Scenario11Vars() {

			m_bGridorPicChecked = true;
			Platform::String^ wcfileName = ref new Platform::String();

		}


		//Accessors
		Scenario11_GridorPic^ GetScenario11Page() { return _scenario11Page; }
		void SetScenario11Page(Scenario11_GridorPic^ s) { _scenario11Page = s; }

		// Hot3dxRotoDrawSceneRender Texture Filename Accessors Scenario5_MatsTexs stuff
		Windows::Storage::StorageFile^ GetFile1() { return fileImage1; }
		void SetFile1(Windows::Storage::StorageFile^ file) { fileImage1 = file; }
		Platform::String^ GetTextureImage1File() { return fileName1; }// m_textureImage1File
		void SetTextureImage1File(Platform::String^ fileName) {
			fileName1 = nullptr;
			fileName1 = ref new Platform::String(fileName->Data());
			fileName1 = fileName;
		}

		Windows::Storage::StorageFile^ GetFileGridPic() { return fileImageGridPic; }
		void SetFileGridPic(Windows::Storage::StorageFile^ file) { fileImageGridPic = file; }
		Platform::String^ GetGridPicTextureImage1File() { return fileNameGridPic; }// m_textureImage1File
		void SetGridPicTextureImage1File(Platform::String^ fileName) {
			fileNameGridPic = nullptr;
			fileNameGridPic = ref new Platform::String(fileName->Data());
			fileNameGridPic = fileName;
		}
		
		Windows::Storage::FileProperties::StorageItemThumbnail^ GetThumbnailGridPicDXP1() { return m_thumbnailGridPicDXP; }
		void SetThumbnailGridPicDXP1(Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail) { m_thumbnailGridPicDXP = thumbnail; }
		BitmapImage^ GetBitmapImageGridPicDXP1() { return bitmapImageGridPicDXP1; }
		void SetbitmapImageGridPicDXP1(Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail)
		{
			bitmapImageGridPicDXP1 = ref new BitmapImage();
			bitmapImageGridPicDXP1->SetSourceAsync(thumbnail);
		}

		Windows::Storage::FileProperties::StorageItemThumbnail^ GetThumbnailDXP1() { return m_thumbnailDXP1; }
		void SetThumbnailDXP1(Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail) { m_thumbnailDXP1 = thumbnail; }
		Windows::UI::Xaml::Media::Imaging::BitmapImage^ GetBitmapImageDXP1() { return bitmapImageDXP1; }
		void SetbitmapImageDXP1(Windows::Storage::FileProperties::StorageItemThumbnail^ thumbnail)
		{
			bitmapImageDXP1 = ref new BitmapImage();
			bitmapImageDXP1->SetSourceAsync(thumbnail);
		}
		
		bool GetGridorPicChecked() { return m_bGridorPicChecked; }
		void SetGridorPicChecked(bool b) { m_bGridorPicChecked = b; }

	private:

		Scenario11_GridorPic^ _scenario11Page;
		Windows::Storage::StorageFile^ fileImage1;
		Windows::Storage::FileProperties::StorageItemThumbnail^ m_thumbnailDXP1;
		Platform::String^ fileName1;
		Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapImageDXP1;

		Windows::Storage::FileProperties::StorageItemThumbnail^ m_thumbnailGridPicDXP;
		Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapImageGridPicDXP1;
		Windows::Storage::StorageFile^ fileImageGridPic; 
		Platform::String^ fileNameGridPic;
		
		bool m_bGridorPicChecked;
	};
}
