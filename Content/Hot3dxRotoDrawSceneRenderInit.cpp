#include "pch.h"
#include "Hot3dxRotoDrawSceneRender.h"
#include "Common\DirectXHelper.h"

using namespace Hot3dxRotoDraw;
using namespace Hot3dx;

using namespace DX;
using namespace Concurrency;
using namespace DirectX;
using namespace DirectX::DXTKXAML12;
using namespace Microsoft::WRL;

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::InitDrawnObjectSingleTexture()
{
	if (m_loadingDrawnObjectComplete == false)
	{
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}

		m_DrawnMeshTexture1.Reset();
		m_shapeDrawnObjectEffect.reset();// = nullptr;
		m_shapeDrawnObjectTex.reset();// = nullptr;

		size_t cnt = vertices.size();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		GetUVPercent(m_textureDimension, cnt);
		vertexes.clear();
		vertexes.resize(cnt);
		if (textureU.size() == cnt)
			for (size_t i = 0; i < cnt; i++)
			{
				XMVECTOR n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
				DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(textureU.at(i), textureV.at(i)) };
				vertexes.at(i) = vpnt;
			}
		
		if (vertexes.size() <= 0)return;
		if (indices.size() == 0)return;
		m_shapeDrawnObjectTex = GeometricPrimitive::CreateCustom(vertexes, indices, device);
		DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);

		// Begin Resource Upload
		m_resourceUploadDrawnObject->BeginXaml();
		
		// Load shaders asynchronously.
		/* //Works
		Concurrency::task createLoadTask = ReadDataAsync(L"\\Assets\\Textures\\Marble.DDS").then([this](std::vector<mybyte>& fileData) {
			fileData1 = fileData;
			});
		if (fileData1.size() >= 1)
		{
			
		}

		*/ //Works
					
			LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage1File->Data(), &m_DrawnMeshTexture1, GetMsgTypes(0), GetMessages(0));
			
		// If there is a failure here it is because the open file dialog is not in the project directory
		DirectX::DXTKXAML12::CreateShaderResourceView(device, m_DrawnMeshTexture1.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::DrawnObjectTexture1)));
			
		RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());

		// Each effect object must be proceeded by its own 
		// EffectPipelineStateDescription pd 
		// even if the EffectPipelineStateDescription pd is the same
		
			EffectPipelineStateDescription pdSDOE(
				&GeometricPrimitive::VertexType::InputLayout,
				CommonStates::Opaque,
				CommonStates::DepthDefault,
				GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
				rtState,
				D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			);

			//Transparent materials
			EffectPipelineStateDescription pdAlphaSDOE(
				&GeometricPrimitive::VertexType::InputLayout,
				CommonStates::AlphaBlend,
				CommonStates::DepthDefault,
				GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
				rtState);

			// Vertex Position Color
			EffectPipelineStateDescription pdVPC(
				&DirectX::DXTKXAML12::VertexPositionColor::InputLayout,
				DirectX::DXTKXAML12::CommonStates::Opaque,
				DirectX::DXTKXAML12::CommonStates::DepthNone,
				DirectX::DXTKXAML12::CommonStates::CullNone,
				rtState,
				D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

			m_shapeDrawnObjectEffect = std::make_unique<BasicEffect>(device, EffectFlags::PerPixelLighting | EffectFlags::Texture, pdSDOE);
			//m_shapeDrawnObjectEffect = std::make_unique<BasicEffect>(device, EffectFlags::PerPixelLighting | EffectFlags::Texture, pdAlphaSDOE);
			//m_shapeDrawnObjectEffect = std::make_unique<BasicEffect>(device, EffectFlags::VertexColor, pdVPC);

			m_shapeDrawnObjectEffect->EnableDefaultLighting();
			m_shapeDrawnObjectEffect->SetDiffuseColor(XMVECTOR{ 1.0f,1.0f ,1.0f,1.0f });
			m_shapeDrawnObjectEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::DrawnObjectTexture1)), GetWrapType(m_states.get(), m_iSamplIndexWrap));
			m_shapeDrawnObjectEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
		
		auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
		WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);

		m_sceneDeviceResources->WaitForGpu();
		loaded.then([this]()
			{
				m_loadingDrawnObjectComplete = true;
				m_bIsBasicModel = true;
				m_bIsBasicModelColor = false;
				m_bIsPBRModel = false;
				m_bIsDualTextureModel = false;
				m_bIsVideoTextureModel = false;
				m_bIsSculptWireframe = false;
				m_player = false;
			});

	}
}
////////////////// 4 tex pbr /////////////////// Albedo, Emmissive, Radiance, Irradiance //////
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::InitDrawnObjectPBRFourTextures()
{
	if (m_loadingDrawnObjectComplete == false)
	{
		m_bDDS_WIC_FLAG1 = true;
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}

		m_DrawnMeshTexture1.Reset();
		m_DrawnMeshTexture2.Reset();
		m_PBRTexture1.Reset();
		m_PBRTexture2.Reset();
		m_radianceIBL.Reset();//  = nullptr;
		m_irradianceIBL.Reset();//  = nullptr;
		m_shapeDrawnObjectPBREffect.reset();//  = nullptr;
		m_hot3dxDrawnObjectPBR.reset();//  = nullptr;

		size_t cnt = vertices.size();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		GetUVPercent(m_textureDimension, cnt);

		verticesPBR.clear();
		verticesPBR.resize(cnt);
		for (size_t i = 0; i < cnt; i++)
		{
			XMVECTOR n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			XMVECTOR tan = XMVectorTan(XMVECTOR(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f)));
			Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt = Hot3dxRotoDraw::VertexPositionNormalTextureTangent(XMFLOAT3(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n), XMVectorGetY(n), XMVectorGetZ(n)), XMFLOAT2(textureU.at(i), textureV.at(i)),
				XMFLOAT4(XMVectorGetW(tan), XMVectorGetX(tan), XMVectorGetY(tan), XMVectorGetZ(tan)));
			verticesPBR.at(i) = vpntt;

		}

		if (verticesPBR.size() <= 0)return;
		if (indices.size() == 0)return;

		// Set the descriptor heaps
		//m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateCustom(vertices, indices, device);// works no texture coords
		//m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateCustomTangent(verticesPBR, indices, device);
		m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateDrawnObjectTangent(verticesPBR, indices, device);// fixed - oh yeah baby

		DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);
		m_resourceUploadDrawnObject->BeginXaml();

		LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage1File->Data(), &m_PBRTexture1, GetMsgTypes(0), GetMessages(0));
		LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage2File->Data(), &m_PBRTexture2, GetMsgTypes(0), GetMessages(0));
		LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage5File->Data(), &m_radianceIBL, GetMsgTypes(0), GetMessages(0));
		LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage6File->Data(), &m_irradianceIBL, GetMsgTypes(0), GetMessages(0));


		//} // If there is a failure here it is because the open file dialog is not in the project directory
		//Push mesh texture to resource stores
		DirectX::DXTKXAML12::CreateShaderResourceView(device, m_PBRTexture1.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::PicTex)));
		//Push mesh texture to resource stores
		DirectX::DXTKXAML12::CreateShaderResourceView(device, m_PBRTexture2.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::PicTex2)));
		//Push rma texture to resource stores
		DirectX::DXTKXAML12::CreateShaderResourceView(device, m_radianceIBL.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::RadianceTex)), false);
		//Push irradiance texture to resource stores
		DirectX::DXTKXAML12::CreateShaderResourceView(device, m_irradianceIBL.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::IrradianceTex)), false);

		RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());
		// Each effect object must be proceeded by its own 
		// EffectPipelineStateDescription pd 
		// even if the EffectPipelineStateDescription pd is the same
		//Get effect factory - shouldn't this be per model?! It's currently shared around everything which seems a little odd
		DirectX::DXTKXAML12::IEffectFactory* fxFactory = nullptr;
		auto m_fxFactoryPBR = std::make_unique<DirectX::DXTKXAML12::PBREffectFactory>(m_resDescPile->Heap(), m_states->Heap());
		fxFactory = m_fxFactoryPBR.get();

		D3D12_INPUT_LAYOUT_DESC inputLayout = VertexPositionNormalTextureTangent::InputLayout;
		EffectPipelineStateDescription pdPBR(
			&inputLayout,
			CommonStates::Opaque,
			CommonStates::DepthDefault,
			GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
			rtState,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		//Transparent materials

		D3D12_INPUT_LAYOUT_DESC inputLayoutA = VertexPositionNormalTextureTangent::InputLayout;
		EffectPipelineStateDescription pdAlphaPBR(
			&inputLayoutA,
			CommonStates::AlphaBlend,
			CommonStates::DepthDefault,
			GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
			rtState,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);


		DirectX::DXTKXAML12::IEffectFactory::EffectInfo einfo = DirectX::DXTKXAML12::IEffectFactory::EffectInfo();
		einfo.alphaValue = 1.0f;
		einfo.ambientColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		einfo.emissiveColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		einfo.emissiveTextureIndex = size_t(PBRDescriptors::PicTex2);
		einfo.samplerIndex = size_t(PBRDescriptors::PicTex);
		einfo.enableDualTexture = true;
		einfo.diffuseTextureIndex = size_t(PBRDescriptors::RMATex);
		einfo.diffuseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		einfo.biasedVertexNormals = true;
		einfo.enableSkinning = true;
		einfo.enableNormalMaps = true;
		einfo.perVertexColor = true;
		einfo.specularTextureIndex = size_t(PBRDescriptors::PicTex);

		std::shared_ptr<DirectX::DXTKXAML12::IEffect> fxEffect = fxFactory->CreateEffect(einfo, pdPBR, pdAlphaPBR, inputLayout, size_t(PBRDescriptors::PicTex), size_t(PBRDescriptors::PicTex));


		m_shapeDrawnObjectPBREffect = std::make_unique<DirectX::DXTKXAML12::PBREffect>(device, EffectFlags::Texture, pdPBR, true, true);// , true);// , true);


		// Lighting

		const int numMips = m_radianceIBL->GetDesc().MipLevels - 3;

		m_shapeDrawnObjectPBREffect->SetIBLTextures(
			m_resDescPile->GetGpuHandle(sizeof(PBRDescriptors::RadianceTex)),
			numMips,
			m_resDescPile->GetGpuHandle(sizeof(PBRDescriptors::IrradianceTex)),
			GetWrapType(m_states.get(), m_iSamplIndexWrap));

		//int liteCnt = m_shapeDrawnObjectPBREffect->MaxDirectionalLights;
		m_shapeDrawnObjectPBREffect->EnableDefaultLighting();

		m_shapeDrawnObjectPBREffect->SetAlpha(1.0f);
		float a = 1.0f;
		float r = 0.0f;
		float g = 0.0f;
		float b = 1.0f;
		XMVECTOR colorv = XMVectorSet(r, g, b, a);
		m_shapeDrawnObjectPBREffect->SetAlbedoTexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::PicTex)), GetWrapType(m_states.get(), m_iSamplIndexWrap));// m_states->AnisotropicClamp());
		m_shapeDrawnObjectPBREffect->SetEmissiveTexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::PicTex2)));
		m_shapeDrawnObjectPBREffect->SetConstantAlbedo(DirectX::DXTKXAML12::SimpleMath::Vector3(1, 1, 1));

		m_shapeDrawnObjectPBREffect->SetLightDiffuseColor(0, colorv);
		m_shapeDrawnObjectPBREffect->SetConstantMetallic(0.0f);
		m_shapeDrawnObjectPBREffect->SetConstantRoughness(0.5f);

		//Static buffers and create TextureEffectFactory
		//m_hot3dxDrawnObjectPBR->LoadStaticBuffers(device, *m_resourceUploadDrawnObject);


		auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
		WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);
		m_shapeDrawnObjectPBREffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
		m_sceneDeviceResources->WaitForGpu();
		loaded.then([this, m_resourceUploadDrawnObject]()
			{
				m_loadingDrawnObjectComplete = true;
				m_bIsPBRModel = true;
				m_bIsBasicModel = false;
				m_bIsBasicModelColor = false;
				m_bIsDualTextureModel = false;
				m_bIsVideoTextureModel = false;
				m_bIsSculptWireframe = false;
				m_player = false;
				m_resourceUploadDrawnObject->~ResourceUploadBatch();

			});

	}
}

////////////////// 4 tex pbr /////////////////// Albedo, Emmissive, Radiance, Irradiance //////

////////////////// 6 tex pbr /////////////////// Albedo, Emmissive, RMA, Normal, Radiance, Irradiance //////
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::InitDrawnObjectPBRFiveTextures()
{
	if (m_loadingDrawnObjectComplete == false)
	{
		m_bDDS_WIC_FLAG1 = true;
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}
		
		m_DrawnMeshTexture1.Reset();
		m_DrawnMeshTexture2.Reset();
		m_PBRTexture1.Reset();//  = nullptr;
		m_PBRTexture2.Reset();//  = nullptr;
		m_RMATexture.Reset();//  = nullptr;
		m_NormalTexture.Reset();//  = nullptr;
		m_radianceIBL.Reset();//  = nullptr;
		m_irradianceIBL.Reset();//  = nullptr;
		m_shapeDrawnObjectPBREffect.reset();//  = nullptr;
		m_hot3dxDrawnObjectPBR.reset();//  = nullptr;

		size_t cnt = vertices.size();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		GetUVPercent(m_textureDimension, cnt);
		
		verticesPBR.clear();
		verticesPBR.resize(cnt);
		for (size_t i = 0; i < cnt; i++)
		{
			XMVECTOR n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			XMVECTOR tan = XMVectorTan(XMVECTOR(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f)));
			Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt = Hot3dxRotoDraw::VertexPositionNormalTextureTangent(XMFLOAT3(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n), XMVectorGetY(n), XMVectorGetZ(n)), XMFLOAT2(textureU.at(i), textureV.at(i)),
				XMFLOAT4(XMVectorGetW(tan), XMVectorGetX(tan), XMVectorGetY(tan), XMVectorGetZ(tan)));
			verticesPBR.at(i) = vpntt;

		}
		
		if (verticesPBR.size() <= 0)return;
		if (indices.size() == 0)return;

		// Set the descriptor heaps
		//m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateCustom(vertices, indices, device);// works no texture coords
		//m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateCustomTangent(verticesPBR, indices, device);
		m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateDrawnObjectTangent(verticesPBR, indices, device);// fixed - oh yeah baby
		
			DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);
			m_resourceUploadDrawnObject->BeginXaml();
			
			    LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage1File->Data(), &m_PBRTexture1, GetMsgTypes(0), GetMessages(0));
				LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage2File->Data(), &m_PBRTexture2, GetMsgTypes(0), GetMessages(0));
				LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage4File->Data(), &m_NormalTexture, GetMsgTypes(0), GetMessages(0));
				LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage3File->Data(), &m_RMATexture, GetMsgTypes(0), GetMessages(0));
				LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage5File->Data(), &m_radianceIBL, GetMsgTypes(0), GetMessages(0));
				LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage6File->Data(), &m_irradianceIBL, GetMsgTypes(0), GetMessages(0));
				
			
			//} // If there is a failure here it is because the open file dialog is not in the project directory
			//Push mesh texture to resource stores
			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_PBRTexture1.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::PicTex)));
			//Push mesh texture to resource stores
			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_PBRTexture2.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::PicTex2)));
			//Push rma texture to resource stores
			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_RMATexture.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::RMATex)));
			//Push normal texture to resource stores
			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_NormalTexture.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::NormalTex)));
			//Push radiance texture to resource stores
			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_radianceIBL.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::RadianceTex)), false);
			//Push irradiance texture to resource stores
			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_irradianceIBL.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::IrradianceTex)), false);
			
			RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());
			// Each effect object must be proceeded by its own 
			// EffectPipelineStateDescription pd 
			// even if the EffectPipelineStateDescription pd is the same
			//Get effect factory - shouldn't this be per model?! It's currently shared around everything which seems a little odd
			DirectX::DXTKXAML12::IEffectFactory* fxFactory = nullptr;
			auto m_fxFactoryPBR = std::make_unique<DirectX::DXTKXAML12::PBREffectFactory>(m_resDescPile->Heap(), m_states->Heap());
			fxFactory = m_fxFactoryPBR.get();

			D3D12_INPUT_LAYOUT_DESC inputLayout = VertexPositionNormalTextureTangent::InputLayout;
			EffectPipelineStateDescription pdPBR(
				&inputLayout,
				CommonStates::Opaque,
				CommonStates::DepthDefault,
				GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
				rtState,
				D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
			//Transparent materials

			D3D12_INPUT_LAYOUT_DESC inputLayoutA = VertexPositionNormalTextureTangent::InputLayout;
			EffectPipelineStateDescription pdAlphaPBR(
				&inputLayoutA,
				CommonStates::AlphaBlend,
				CommonStates::DepthDefault,
				GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
				rtState,
				D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
			

			DirectX::DXTKXAML12::IEffectFactory::EffectInfo einfo = DirectX::DXTKXAML12::IEffectFactory::EffectInfo();
			einfo.alphaValue = 1.0f;
			einfo.ambientColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
			einfo.emissiveColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
			einfo.emissiveTextureIndex = size_t(PBRDescriptors::PicTex2);
			einfo.normalTextureIndex = size_t(PBRDescriptors::NormalTex);
			einfo.samplerIndex = size_t(PBRDescriptors::PicTex);
			einfo.enableDualTexture = true;
			einfo.diffuseTextureIndex = size_t(PBRDescriptors::RMATex);
			einfo.diffuseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
			einfo.biasedVertexNormals = true;
			einfo.enableSkinning = true;
			einfo.enableNormalMaps = true;
			einfo.perVertexColor = true;
			einfo.specularTextureIndex = size_t(PBRDescriptors::PicTex);

			std::shared_ptr<DirectX::DXTKXAML12::IEffect> fxEffect = fxFactory->CreateEffect(einfo, pdPBR, pdAlphaPBR, inputLayout, size_t(PBRDescriptors::PicTex), size_t(PBRDescriptors::PicTex));
			

			m_shapeDrawnObjectPBREffect = std::make_unique<DirectX::DXTKXAML12::PBREffect>(device, EffectFlags::Texture, pdPBR, true, true);// , true);// , true);
			
			
			//m_shapeDrawnObjectPBREffect = std::make_unique<DirectX::DXTKXAML12::PBREffect>(device, EffectFlags::None, pdAlphaPBR);

			m_shapeDrawnObjectPBREffect->SetSurfaceTextures(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::PicTex)),
				m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::NormalTex)),
				m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::RMATex)),
				GetWrapType(m_states.get(), m_iSamplIndexWrap));
			// Lighting
			
			const int numMips = m_radianceIBL->GetDesc().MipLevels - 3;
			
			m_shapeDrawnObjectPBREffect->SetIBLTextures(
				m_resDescPile->GetGpuHandle(sizeof(PBRDescriptors::RadianceTex)),
				numMips,
				m_resDescPile->GetGpuHandle(sizeof(PBRDescriptors::IrradianceTex)),
				GetWrapType(m_states.get(), m_iSamplIndexWrap));
				
			//int liteCnt = m_shapeDrawnObjectPBREffect->MaxDirectionalLights;
			m_shapeDrawnObjectPBREffect->EnableDefaultLighting();

			m_shapeDrawnObjectPBREffect->SetAlpha(1.0f);
			float a = 1.0f;
			float r = 0.0f;
			float g = 0.0f;
			float b = 1.0f;
			XMVECTOR colorv = XMVectorSet(r, g, b, a);
			m_shapeDrawnObjectPBREffect->SetAlbedoTexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::PicTex)), GetWrapType(m_states.get(), m_iSamplIndexWrap));// m_states->AnisotropicClamp());
			m_shapeDrawnObjectPBREffect->SetEmissiveTexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::PicTex2)));
			m_shapeDrawnObjectPBREffect->SetNormalTexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::NormalTex)));
			m_shapeDrawnObjectPBREffect->SetRMATexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::RMATex)));
			m_shapeDrawnObjectPBREffect->SetConstantAlbedo(DirectX::DXTKXAML12::SimpleMath::Vector3(1, 1, 1));
			
			m_shapeDrawnObjectPBREffect->SetLightDiffuseColor(0, colorv);
			m_shapeDrawnObjectPBREffect->SetConstantMetallic(0.0f);
			m_shapeDrawnObjectPBREffect->SetConstantRoughness(0.5f);

			//Static buffers and create TextureEffectFactory
			//m_hot3dxDrawnObjectPBR->LoadStaticBuffers(device, *m_resourceUploadDrawnObject);


			auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
			WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);
			m_shapeDrawnObjectPBREffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_sceneDeviceResources->WaitForGpu();
			loaded.then([this, m_resourceUploadDrawnObject]()
				{
					m_loadingDrawnObjectComplete = true;
					m_bIsPBRModel = true;
					m_bIsBasicModel = false;
					m_bIsBasicModelColor = false;
					m_bIsDualTextureModel = false;
					m_bIsVideoTextureModel = false;
					m_bIsSculptWireframe = false;
					m_player = false;
					m_resourceUploadDrawnObject->~ResourceUploadBatch();
					
				});
	
	}
}
////////////////// 6 tex pbr /////////////////// Albedo, Emmissive, RMA, Normal, Radiance, Irradiance //////

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::InitDrawnObjectPBRThreeTextures()
{
	if (m_loadingDrawnObjectComplete == false)
	{
		m_bDDS_WIC_FLAG1 = true;
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}

		m_DrawnMeshTexture1.Reset();
		m_DrawnMeshTexture2.Reset();
		m_PBRTexture1.Reset();//  = nullptr;
		m_RMATexture.Reset();//  = nullptr;
		m_NormalTexture.Reset();//  = nullptr;
		//m_radianceIBL.Reset();//  = nullptr;
		//m_irradianceIBL.Reset();//  = nullptr;
		m_shapeDrawnObjectPBREffect.reset();//  = nullptr;
		m_hot3dxDrawnObjectPBR.reset();//  = nullptr;

		size_t cnt = vertices.size();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		GetUVPercent(m_textureDimension, cnt);

		verticesPBR.clear();
		verticesPBR.resize(cnt);
		for (size_t i = 0; i < cnt; i++)
		{
			XMVECTOR n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			XMVECTOR tan = XMVectorTan(XMVECTOR(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f)));
			Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt = Hot3dxRotoDraw::VertexPositionNormalTextureTangent(XMFLOAT3(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n), XMVectorGetY(n), XMVectorGetZ(n)), XMFLOAT2(textureU.at(i), textureV.at(i)),
				XMFLOAT4(XMVectorGetW(tan), XMVectorGetX(tan), XMVectorGetY(tan), XMVectorGetZ(tan)));
			verticesPBR.at(i) = vpntt;

		}

		if (verticesPBR.size() <= 0)return;
		if (indices.size() == 0)return;

		// Set the descriptor heaps
		//m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateCustom(vertices, indices, device);// works no texture coords
		//m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateCustomTangent(verticesPBR, indices, device);
		m_hot3dxDrawnObjectPBR = Hot3dxDrawnObject::CreateDrawnObjectTangent(verticesPBR, indices, device);// fixed - oh yeah baby

		DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);
		m_resourceUploadDrawnObject->BeginXaml();

		LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage1File->Data(), &m_PBRTexture1, GetMsgTypes(0), GetMessages(0));
		//LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage2File->Data(), &m_PBRTexture2, GetMsgTypes(0), GetMessages(0));
		LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage4File->Data(), &m_NormalTexture, GetMsgTypes(0), GetMessages(0));
		LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage3File->Data(), &m_RMATexture, GetMsgTypes(0), GetMessages(0));
		

		//} // If there is a failure here it is because the open file dialog is not in the project directory
		//Push mesh texture to resource stores
		DirectX::DXTKXAML12::CreateShaderResourceView(device, m_PBRTexture1.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::PicTex)));
		//Push mesh texture to resource stores
		//DirectX::DXTKXAML12::CreateShaderResourceView(device, m_PBRTexture2.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::PicTex2)));
		//Push rma texture to resource stores
		DirectX::DXTKXAML12::CreateShaderResourceView(device, m_RMATexture.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::RMATex)));
		//Push normal texture to resource stores
		DirectX::DXTKXAML12::CreateShaderResourceView(device, m_NormalTexture.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::NormalTex)));
		//Push radiance texture to resource stores
		//DirectX::DXTKXAML12::CreateShaderResourceView(device, m_radianceIBL.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::RadianceTex)), false);
		//Push irradiance texture to resource stores
		//DirectX::DXTKXAML12::CreateShaderResourceView(device, m_irradianceIBL.Get(), m_resDescPile->GetCpuHandle(size_t(PBRDescriptors::IrradianceTex)), false);

		RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());
		// Each effect object must be proceeded by its own 
		// EffectPipelineStateDescription pd 
		// even if the EffectPipelineStateDescription pd is the same
		//Get effect factory - shouldn't this be per model?! It's currently shared around everything which seems a little odd
		DirectX::DXTKXAML12::IEffectFactory* fxFactory = nullptr;
		auto m_fxFactoryPBR = std::make_unique<DirectX::DXTKXAML12::PBREffectFactory>(m_resDescPile->Heap(), m_states->Heap());
		fxFactory = m_fxFactoryPBR.get();

		D3D12_INPUT_LAYOUT_DESC inputLayout = VertexPositionNormalTextureTangent::InputLayout;
		EffectPipelineStateDescription pdPBR(
			&inputLayout,
			CommonStates::Opaque,
			CommonStates::DepthDefault,
			GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
			rtState,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		//Transparent materials

		D3D12_INPUT_LAYOUT_DESC inputLayoutA = VertexPositionNormalTextureTangent::InputLayout;
		EffectPipelineStateDescription pdAlphaPBR(
			&inputLayoutA,
			CommonStates::AlphaBlend,
			CommonStates::DepthDefault,
			GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
			rtState,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);


		DirectX::DXTKXAML12::IEffectFactory::EffectInfo einfo = DirectX::DXTKXAML12::IEffectFactory::EffectInfo();
		einfo.alphaValue = 1.0f;
		einfo.ambientColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		einfo.emissiveColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//einfo.emissiveTextureIndex = size_t(PBRDescriptors::PicTex2);
		einfo.normalTextureIndex = size_t(PBRDescriptors::NormalTex);
		einfo.samplerIndex = size_t(PBRDescriptors::PicTex);
		einfo.enableDualTexture = true;
		einfo.diffuseTextureIndex = size_t(PBRDescriptors::RMATex);
		einfo.diffuseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		einfo.biasedVertexNormals = true;
		einfo.enableSkinning = true;
		einfo.enableNormalMaps = true;
		einfo.perVertexColor = true;
		einfo.specularTextureIndex = size_t(PBRDescriptors::PicTex);

		std::shared_ptr<DirectX::DXTKXAML12::IEffect> fxEffect = fxFactory->CreateEffect(einfo, pdPBR, pdAlphaPBR, inputLayout, size_t(PBRDescriptors::PicTex), size_t(PBRDescriptors::PicTex));


		m_shapeDrawnObjectPBREffect = std::make_unique<DirectX::DXTKXAML12::PBREffect>(device, EffectFlags::Texture, pdPBR, true, true);// , true);// , true);


		//m_shapeDrawnObjectPBREffect = std::make_unique<DirectX::DXTKXAML12::PBREffect>(device, EffectFlags::None, pdAlphaPBR);

		m_shapeDrawnObjectPBREffect->SetSurfaceTextures(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::PicTex)),
			m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::NormalTex)),
			m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::RMATex)),
			GetWrapType(m_states.get(), m_iSamplIndexWrap));
		// Lighting
		/*
		const int numMips = m_radianceIBL->GetDesc().MipLevels - 3;
		
		m_shapeDrawnObjectPBREffect->SetIBLTextures(
			m_resDescPile->GetGpuHandle(sizeof(PBRDescriptors::RadianceTex)),
			numMips,
			m_resDescPile->GetGpuHandle(sizeof(PBRDescriptors::IrradianceTex)),
			GetWrapType(m_states.get(), m_iSamplIndexWrap));
*/
		//int liteCnt = m_shapeDrawnObjectPBREffect->MaxDirectionalLights;
		m_shapeDrawnObjectPBREffect->EnableDefaultLighting();

		m_shapeDrawnObjectPBREffect->SetAlpha(1.0f);
		float a = 1.0f;
		float r = 0.0f;
		float g = 0.0f;
		float b = 1.0f;
		XMVECTOR colorv = XMVectorSet(r, g, b, a);
		m_shapeDrawnObjectPBREffect->SetAlbedoTexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::PicTex)), GetWrapType(m_states.get(), m_iSamplIndexWrap));// m_states->AnisotropicClamp());
		m_shapeDrawnObjectPBREffect->SetEmissiveTexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::PicTex2)));
		m_shapeDrawnObjectPBREffect->SetNormalTexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::NormalTex)));
		m_shapeDrawnObjectPBREffect->SetRMATexture(m_resDescPile->GetGpuHandle(size_t(PBRDescriptors::RMATex)));
		m_shapeDrawnObjectPBREffect->SetConstantAlbedo(DirectX::DXTKXAML12::SimpleMath::Vector3(1, 1, 1));

		m_shapeDrawnObjectPBREffect->SetLightDiffuseColor(0, colorv);
		m_shapeDrawnObjectPBREffect->SetConstantMetallic(0.0f);
		m_shapeDrawnObjectPBREffect->SetConstantRoughness(0.5f);

		//Static buffers and create TextureEffectFactory
		//m_hot3dxDrawnObjectPBR->LoadStaticBuffers(device, *m_resourceUploadDrawnObject);


		auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
		WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);
		m_shapeDrawnObjectPBREffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
		m_sceneDeviceResources->WaitForGpu();
		loaded.then([this, m_resourceUploadDrawnObject]()
			{
				m_loadingDrawnObjectComplete = true;
				m_bIsPBRModel = true;
				m_bIsBasicModel = false;
				m_bIsBasicModelColor = false;
				m_bIsDualTextureModel = false;
				m_bIsVideoTextureModel = false;
				m_bIsSculptWireframe = false;
				m_player = false;
				m_resourceUploadDrawnObject->~ResourceUploadBatch();

			});

	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::InitDrawnObjectDualTexture()
{
	if (m_loadingDrawnObjectComplete == false)
	{
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}

		m_DrawnMeshTexture1.Reset();// = nullptr;
		m_DrawnMeshTexture2.Reset();// = nullptr;
		m_dualTextureEffect.reset();// = nullptr;
		m_hot3dxDrawnObjectDual.reset();//  = nullptr;

		size_t cnt = vertices.size();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		GetUVPercent(m_textureDimension, cnt);
		vertexes.clear();
		vertexes.resize(cnt);
		verticesDual.clear();
		verticesDual.resize(cnt);
		if (textureU.size() == cnt)
		for (size_t i = 0; i < cnt; i++)
		{
			XMVECTOR n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			//if (i > textureU.size()) { textureU.emplace_back(0.0f); }
			//if (i > textureV.size()) { textureV.emplace_back(0.0f); }
			DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(textureU.at(i), textureV.at(i)) };
			vertexes.at(i) = vpnt;
		
			n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			DirectX::DXTKXAML12::VertexPositionDualTexture vpnt1 = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT2(textureU.at(i), textureV.at(i)), XMFLOAT2(textureU.at(i), textureV.at(i)) };
			verticesDual.at(i) = vpnt1;
		}

		if (vertices.size() <= 0)return;
		if (verticesDual.size() <= 0)return;
		if (indices.size() == 0)return;
		//m_shapeDrawnObjectTex = GeometricPrimitive::CreateCustom(vertexes, indices, device);
		m_hot3dxDrawnObjectDual = Hot3dxRotoDraw::Hot3dxDrawnObject::CreateCustomDualTexture(verticesDual, indices, device);
    
		DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);
		{
			// Begin Resource Upload
			m_resourceUploadDrawnObject->BeginXaml();

			    LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage1File->Data(), &m_DrawnMeshTexture1, GetMsgTypes(0), GetMessages(0));
				LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage2File->Data(), &m_DrawnMeshTexture2, GetMsgTypes(0), GetMessages(0));
				
			// If there is a failure here it is because the open file dialog is not in the project directory

			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_DrawnMeshTexture1.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::DrawnObjectTexture1)));
			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_DrawnMeshTexture2.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::DrawnObjectTexture2)));




			// Each effect object must be proceeded by its own 
			// EffectPipelineStateDescription pd 
			// even if the EffectPipelineStateDescription pd is the same
			RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());

			{

				EffectPipelineStateDescription pd(
					&DirectX::DXTKXAML12::VertexPositionDualTexture::InputLayout,
					DirectX::DXTKXAML12::CommonStates::Opaque,
					DirectX::DXTKXAML12::CommonStates::DepthDefault,
					DirectX::DXTKXAML12::CommonStates::CullNone,
					rtState,
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
				);

				m_dualTextureEffect = std::make_unique<DirectX::DXTKXAML12::DualTextureEffect>(device, EffectFlags::VertexColor || EffectFlags::Fog, pd);
				m_dualTextureEffect->SetAlpha(1.0f);
				float a = 1.0f;
				float r = 1.0f;
				float g = 1.0f;
				float b = 1.0f;
				XMVECTOR colorv = XMVectorSet(r, g, b, a);
				m_dualTextureEffect->SetDiffuseColor(colorv);
				m_dualTextureEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::DrawnObjectTexture1)), m_states->AnisotropicWrap());
				m_dualTextureEffect->SetTexture2(m_resourceDescriptors->GetGpuHandle(size_t(Descriptors::DrawnObjectTexture2)), m_states->AnisotropicWrap());
				float a1 = 0.5f;
				float r1 = 0.5f;
				float g1 = 0.4f;
				float b1 = 1.0f;
				XMVECTOR colorvf = XMVectorSet(r1, g1, b1, a1);

				bool m_bSetFog = false;

				if (m_bSetFog == true) {
					m_dualTextureEffect->SetFogColor(colorvf);
					m_dualTextureEffect->SetFogStart(0.0f);
					m_dualTextureEffect->SetFogEnd(10.0f);
				}
			}
			auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
			WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);

			m_dualTextureEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_sceneDeviceResources->WaitForGpu();
			loaded.then([this]()
				{
					m_loadingDrawnObjectComplete = true;
					m_bIsDualTextureModel = true;
					m_bIsBasicModel = false;
					m_bIsPBRModel = false;
					m_bIsVideoTextureModel = false;
					m_bIsSculptWireframe = false;
					m_bIsBasicModelColor = false;
					m_player = false;
					
				});
		}
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::InitDrawnObjectVideoTexture()
{
	if (m_loadingDrawnObjectComplete == false)
	{
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}
		
		m_videoTexture.Reset();// = nullptr;
		m_sharedVideoTexture = nullptr;
		m_videoWidth = 0;
		m_videoHeight = 0;
		m_shapeDrawnObjectVideoEffect.reset();// = nullptr;
		m_shapeDrawnObjectVideoTex.reset();// = nullptr;
				
		size_t cnt = vertices.size();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		GetUVPercent(m_textureDimension, cnt);
		vertexes.clear();
		vertexes.resize(cnt);

		for (size_t i = 0; i < cnt; i++)
		{
			XMVECTOR n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
			if (i > textureU.size()) { textureU.emplace_back(0.0f); }
			if (i > textureV.size()) { textureV.emplace_back(0.0f); }
			DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(textureU.at(i), textureV.at(i)) };
			vertexes.at(i) = vpnt;
		}

		if (vertices.size() <= 0)return;
		if (indices.size() == 0)return;
		m_shapeDrawnObjectVideoTex = GeometricPrimitive::CreateCustom(vertexes, indices, device);
		{
			DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);

			// Begin Resource Upload
			m_resourceUploadDrawnObject->BeginXaml();

			
			RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());

			// Each effect object must be proceeded by its own 
			// EffectPipelineStateDescription pd 
			// even if the EffectPipelineStateDescription pd is the same

			{
				EffectPipelineStateDescription pdSVT(
					&GeometricPrimitive::VertexType::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
					rtState);//,D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

				//Transparent materials
				EffectPipelineStateDescription pdAlphaSVTE(
					&GeometricPrimitive::VertexType::InputLayout,
					CommonStates::AlphaBlend,
					CommonStates::DepthDefault,
					GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
					rtState);

				// Vertex Position Color
				EffectPipelineStateDescription pdVPC(
					&DirectX::DXTKXAML12::VertexPositionColor::InputLayout,
					DirectX::DXTKXAML12::CommonStates::Opaque,
					DirectX::DXTKXAML12::CommonStates::DepthNone,
					DirectX::DXTKXAML12::CommonStates::CullNone,
					rtState,
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

				m_shapeDrawnObjectVideoEffect = std::make_unique<BasicEffect>(device, EffectFlags::Lighting | EffectFlags::Texture, pdSVT);
				//m_shapeDrawnObjectEffect = std::make_unique<BasicEffect>(device, EffectFlags::PerPixelLighting | EffectFlags::Texture, pdAlphaSDOE);
				//m_shapeDrawnObjectEffect = std::make_unique<BasicEffect>(device, EffectFlags::VertexColor, pdVPC);

				m_shapeDrawnObjectVideoEffect->EnableDefaultLighting();

				m_shapeDrawnObjectVideoEffect->SetTexture(m_resourceDescriptors->GetGpuHandle(static_cast<size_t>(Descriptors::VideoTexture)), m_states->AnisotropicWrap());
				m_shapeDrawnObjectVideoEffect->SetDiffuseColor(DirectX::XMVECTOR{ 1.0f,1.0f ,1.0f,1.0f });
				m_shapeDrawnObjectVideoEffect->SetAmbientLightColor(DirectX::XMVECTOR{ 1.0f,1.0f ,1.0f,1.0f });
				//m_shapeDrawnObjectEffect->SetLightDirection(0, DirectX::XMVECTOR{ 0.0f,1.0f,-20.0f,0.0f });
			}


			auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
			WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);

			m_shapeDrawnObjectVideoEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_sceneDeviceResources->WaitForGpu();

			loaded.then([this]()
				{
					auto device = m_sceneDeviceResources->GetD3DDevice();
					Size hw = m_sceneDeviceResources->GetOutputSize();
					m_player = std::make_unique<Hot3dxRotoDraw::MediaEnginePlayer>(m_sceneDeviceResources);
					m_player->Initialize(m_sceneDeviceResources->GetDXGIFactory(), device, DXGI_FORMAT_B8G8R8A8_UNORM);

					//Scenario7Vars^ vars7 = m_vars->GetDXPage()->m_Scene7Vars;
					//m_textureImageVideoFile = ref new Platform::String(vars7->GetVideoTextureImage1File()->Data());
					Scenario5TexVars^ vars = m_vars->GetDXPage()->m_Scene5TexVars;
					m_textureImageVideo2File = ref new Platform::String(vars->GetTextureImageVideo1File()->Data());
					Uri^ uri = ref new Uri(m_textureImageVideo2File);
					Windows::Media::Core::MediaSource^ source = Windows::Media::Core::MediaSource::CreateFromUri(uri);
					m_player->SetSource(m_textureImageVideo2File->Data());//uri->DisplayUri->Data());// 
					
					while (!m_player->IsInfoReady())
					{
						SwitchToThread();
					}
					m_player->GetNativeVideoSize(m_videoWidth, m_videoHeight);
					
					CD3DX12_RESOURCE_DESC desc(
						D3D12_RESOURCE_DIMENSION_TEXTURE2D,
						0,
						m_videoWidth,
						m_videoHeight,
						1,
						1,
						DXGI_FORMAT_B8G8R8A8_UNORM,
						1,
						0,
						D3D12_TEXTURE_LAYOUT_UNKNOWN,
						D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS);

					CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
					DirectXPage^ xpg = m_vars->GetDXPage();
									

					//ThrowIfFailedHot(
					device->CreateCommittedResource(
						&defaultHeapProperties,
						D3D12_HEAP_FLAG_SHARED,
						&desc,
						D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
						nullptr,
						IID_GRAPHICS_PPV_ARGS(&m_videoTexture));//.GetAddressOf()
					//);

					if (!m_videoTexture)
					{
						m_videoTexture.Reset();// = nullptr;
						device->CreateCommittedResource(
							&defaultHeapProperties,
							D3D12_HEAP_FLAG_SHARED,
							&desc,
							D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
							nullptr,
							IID_GRAPHICS_PPV_ARGS(&m_videoTexture));
						xpg->NotifyUser("Retry - Error occured m_videoTexture is null", NotifyType::StatusMessage);
					}
					
					if (!m_videoTexture)
					{
						m_player->IsFinished();
						m_player->Shutdown();
						ClearDrawnObject();
						
						xpg->NotifyUser("Clearing: Error occured m_videoTexture is null", NotifyType::StatusMessage);
						return;
					}
					CreateShaderResourceView(device, m_videoTexture.Get(), m_resourceDescriptors->GetCpuHandle(static_cast<size_t>(Descriptors::VideoTexture)), false);

					ThrowIfFailedHot(
						device->CreateSharedHandle(
							m_videoTexture.Get(),
							nullptr,
							GENERIC_ALL,
							nullptr,
							&m_sharedVideoTexture));
					m_loadingDrawnObjectComplete = true;
					m_bIsVideoTextureModel = true;
					m_bIsPlayer = true;
					m_bIsBasicModel = false;// true;
					m_bIsPBRModel = false;
					m_bIsDualTextureModel = false;
					m_bIsSculptWireframe = false;
					m_bIsBasicModelColor = false;
					
				});
		}
	}
}

void Hot3dxRotoDraw::RotoDrawSceneRender::InitDrawnObjectSculptWireframe()
{
	m_loadingDrawnObjectComplete = false;
	if (m_loadingDrawnObjectComplete == false)
	{
		auto device = m_sceneDeviceResources->GetD3DDevice();
		if (!device)
		{
			return;
		}
		m_shapeDrawnObjectEffectSculpt = nullptr;
		m_shapeDrawnObjectSculpt = nullptr;

		size_t cnt = vertices.size();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();

		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);

		m_shapeDrawnObjectSculpt = Hot3dxDrawnObject::CreateDrawnObjectColor(vertices, indices, device);
		{
			DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);

			// Begin Resource Upload
			m_resourceUploadDrawnObject->BeginXaml();

			LoadDDSOrWicTextureFile(m_sceneDeviceResources->GetD3DDevice(), *m_resourceUploadDrawnObject, m_textureImage1File->Data(), &m_DrawnMeshTexture1, GetMsgTypes(0), GetMessages(0));
			// If there is a failure here it is because the open file dialog is not in the project directory

			DirectX::DXTKXAML12::CreateShaderResourceView(device, m_DrawnMeshTexture1.Get(), m_resourceDescriptors->GetCpuHandle(size_t(Descriptors::DrawnObjectTexture1)));

			RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());

			// Each effect object must be proceeded by its own
			// EffectPipelineStateDescription pd
			// even if the EffectPipelineStateDescription pd is the same

			{
				EffectPipelineStateDescription pdSDOE(
					&GeometricPrimitive::VertexType::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					GetCommonStatesRasterizerDescp12(3),// Wireframe
					rtState,
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
				);

				m_shapeDrawnObjectEffectSculpt = std::make_unique<BasicEffect>(device, EffectFlags::PerPixelLighting, pdSDOE);
				//m_shapeDrawnObjectEffect = std::make_unique<BasicEffect>(device, EffectFlags::PerPixelLighting | EffectFlags::Texture, pdAlphaSDOE);
				//m_shapeDrawnObjectEffect = std::make_unique<BasicEffect>(device, EffectFlags::VertexColor, pdVPC);

				m_shapeDrawnObjectEffectSculpt->EnableDefaultLighting();
				m_shapeDrawnObjectEffectSculpt->SetDiffuseColor(XMVECTOR{ 1.0f,1.0f ,1.0f,1.0f });

			}
			auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
			WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);

			m_shapeDrawnObjectEffectSculpt->SetProjection(XMLoadFloat4x4(&m_projection4x4));
			m_sceneDeviceResources->WaitForGpu();
			loaded.then([this]()
				{
					m_loadingDrawnObjectComplete = true;
					m_bIsBasicModel = false;
					m_bIsSculptWireframe = true;
					m_bIsPBRModel = false;
					m_bIsDualTextureModel = false;
					m_bIsVideoTextureModel = false;
					m_bIsBasicModelColor = false;
					m_player = false;
				});
		}
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::InitDrawnObjectSingleColor()
{
	vertexes.resize(0);
	if (m_loadingDrawnObjectComplete == false)
	{
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}

		m_shapeDrawnObjectEffect.reset();
		m_shapeDrawnObjectColor.reset();

		size_t cnt = vertices.size();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		GetUVPercent(m_textureDimension, cnt);
		
		if (indices.size() == 0)return;
		m_shapeDrawnObjectColor = Hot3dxDrawnObject::CreateDrawnObjectColor(vertices, indices, device);

		DirectX::DXTKXAML12::ResourceUploadBatch* m_resourceUploadDrawnObject = new DirectX::DXTKXAML12::ResourceUploadBatch(device);

		// Begin Resource Upload
		m_resourceUploadDrawnObject->BeginXaml();

		RenderTargetState rtState(m_sceneDeviceResources->GetBackBufferFormat(), m_sceneDeviceResources->GetDepthBufferFormat());

		// Each effect object must be proceeded by its own 
		// EffectPipelineStateDescription pd 
		// even if the EffectPipelineStateDescription pd is the same

		EffectPipelineStateDescription pdSDOE(
			&GeometricPrimitive::VertexType::InputLayout,
			CommonStates::Opaque,
			CommonStates::DepthDefault,
			GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
			rtState,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		);

		//Transparent materials
		EffectPipelineStateDescription pdAlphaSDOE(
			&GeometricPrimitive::VertexType::InputLayout,
			CommonStates::AlphaBlend,
			CommonStates::DepthDefault,
			GetCommonStatesRasterizerDescp12(m_iCullNoneToWireframe),
			rtState);

		// Vertex Position Color
		EffectPipelineStateDescription pdVPC(
			&DirectX::DXTKXAML12::VertexPositionColor::InputLayout,
			DirectX::DXTKXAML12::CommonStates::Opaque,
			DirectX::DXTKXAML12::CommonStates::DepthNone,//DepthDefault, causes points only
			DirectX::DXTKXAML12::CommonStates::CullNone,
			rtState,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		m_shapeDrawnObjectEffect = std::make_unique<BasicEffect>(device, EffectFlags::VertexColor, pdVPC);

		m_shapeDrawnObjectEffect->EnableDefaultLighting();
		m_shapeDrawnObjectEffect->SetDiffuseColor(XMVECTOR{ 1.0f,1.0f ,1.0f, 1.0f });
		m_shapeDrawnObjectEffect->SetColorAndAlpha(FXMVECTOR{ 1.0f,1.0f ,1.0f,0.5f });
		m_shapeDrawnObjectEffect->SetProjection(XMLoadFloat4x4(&m_projection4x4));

		auto loaded = m_resourceUploadDrawnObject->EndXaml(m_sceneDeviceResources->GetCommandQueue());
		WaitForSingleObject(m_resourceUploadDrawnObject->GetGPUHandle(), INFINITE);

		m_sceneDeviceResources->WaitForGpu();
		loaded.then([this]()
			{
				m_loadingDrawnObjectComplete = true;
				m_bIsBasicModelColor = true;
				m_bIsBasicModel = false;
				m_bIsPBRModel = false;
				m_bIsDualTextureModel = false;
				m_bIsVideoTextureModel = false;
				m_bIsSculptWireframe = false;
				m_player = false;
			});

	}
}

/// <summary>
/// Checks the point count of the free draw line
/// To see if the buffer is overrun
/// </summary>

void Hot3dxRotoDraw::RotoDrawSceneRender::CheckMaximumPointCount(size_t vectorSize)
{
	if (m_iPointCount > vectorSize)
	{
		m_vars->GetDXPage()->NotifyUser("WARNING:\n Number of Points is Too Large:\nBuffer Auto Reset\nDraw A Line Withe Less Points", NotifyType::StatusMessage);
	}
	
}