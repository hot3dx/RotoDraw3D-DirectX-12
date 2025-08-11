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
#include <RenderTargetStateXaml12.h>
#include <EffectPipelineStateDescriptionXaml12.h>
#include <CommonStatesXaml12.h>
#include <GraphicsMemoryXaml12.h>
#include <VertexTypesXaml12.h>
#include <MyResourceUploadBatchXaml12.h>
#include <GeometricPrimitiveXaml12.h>
#include <GeometryXaml12.h>
#include <EffectsXaml12.h>
#include <EffectCommonXaml12.h>
#include <DDSTextureLoaderXaml12.h>
#include <WICTextureLoaderXaml12.h>
#include "FilesSave\ObjMaterialFile.h"
#include "FilesSave\Obj3DFile.h"
#include "FilesSave\Hot3dxTextFileSave.h"
#include <DirectXMath.h>

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

inline bool Hot3dxRotoDraw::RotoDrawSceneRender::MyIntersects(const SimpleMath::Plane& plane, _Out_ float& Dist, _Out_ XMVECTOR& v, XMFLOAT3 direction, XMFLOAT3 position)
{
	using namespace DirectX;

	XMVECTOR p = XMLoadFloat4(&plane);
	XMVECTOR dir = XMLoadFloat3(&direction);

	XMVECTOR nd = XMPlaneDotNormal(p, dir);
	v = nd;
	if (XMVector3LessOrEqual(XMVectorAbs(nd), g_RayEpsilon))
	{
		Dist = 0.f;
		return false;
	}
	else
	{
		// t = -(dot(n,origin) + D) / dot(n,dir)
		XMVECTOR pos1 = XMLoadFloat3(&position);
		//XMVECTOR 
		v = XMPlaneDotNormal(p, pos1);
		v = XMVectorAdd(v, XMVectorSplatW(p));
		v = XMVectorDivide(v, nd);
		float dist = -XMVectorGetX(v);
		if (dist < 0)
		{
			Dist = 0.f;
			return false;
		}
		else
		{
			Dist = dist;
			return true;
		}
	}
}
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::MouseCursor3dPoint(float positionX, float positionY)
{
	if (m_tracking)
	{
		if (m_loadingComplete)
		{
			point.y = positionY;
			D3D12_VIEWPORT rect = m_sceneDeviceResources->GetScreenViewport();

			XMVECTOR xAxis = XMVectorSet(10.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR yAxis = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
			XMVECTOR zAxis = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			float vrx = m_vars->GetDXPage()->Get_xRotateGridCam();
			float vry = m_vars->GetDXPage()->Get_yRotateGridCam();
			//float vrz = m_vars->GetDXPage()->Get_zRotateGridCam();// +20.0f;

			xAxis = RotateYawPitchPointXMV(xAxis, vry, vrx);
			yAxis = RotateYawPitchPointXMV(yAxis, vry, vrx);
			zAxis = RotateYawPitchPointXMV(zAxis, vry, vrx);

			// convert mouse points to number line 
			// plus/ minus coordinates
			// and convert to float
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float centerx;
			float centery;

			// convert mouse points to number line 
			// plus/ minus coordinates
			// and convert to float
			XMFLOAT3 camdir = XMFLOAT3(m_hot3dxGeo->directionBetweenPoints(XMFLOAT3(m_EyeX, m_EyeY, m_EyeZ), XMFLOAT3(m_LookAtZ, m_LookAtY, m_LookAtZ)));
			float cammag = (float)m_hot3dxGeo->Magnitude(camdir);
			float midWidth = rect.Width / 2.0f;
			float midHeight = rect.Height / 2.0f;
			float m_cameraDistanceWidthRatio = cammag / 1.43f;// 1.428571428571429f; // -20.0f / 14.0f (The original camera position divided by the height and width in DirectX units
			m_cameraDistanceRatio = cammag / 1.43f;// 1.052631578947368f; // for a perfectly square grid
			m_drawMouseWidthRatio = m_cameraDistanceWidthRatio / midWidth;// 14.0f //28.0f / Width would also work for the drawWidthRatio
			m_drawMouseHeightRatio = m_cameraDistanceRatio / midHeight;// 14.0f // 28.0f / Height would also work for the drawHeightRatio

			if (positionX > 0 || point.x < (rect.TopLeftX - rect.Width)
				|| point.y > 0 || point.y < (rect.TopLeftY - rect.Height))
			{
				centerx = midWidth;
				centery = midHeight;

				if (m_IsLeftHanded)
				{
					x = ((point.x - centerx) * m_drawMouseWidthRatio);// for left handed minus sign on x
				}
				else
				{
					x = -((point.x - centerx) * m_drawMouseWidthRatio);// for right handed remove minus sign on x
				}// eo if (m_IsLeftHanded)
				y = -((point.y - centery) * m_drawMouseHeightRatio);

				XMVECTOR xx, yy;// , zz;
				xx = XMVectorScale(xAxis, x);
				yy = XMVectorScale(yAxis, y);
				XMVECTOR mouse3dPos = XMVectorSet((XMVectorGetX(yy) - XMVectorGetX(xx)) + m_LookAtX, (XMVectorGetY(yy) - XMVectorGetY(xx)) + m_LookAtY, (XMVectorGetZ(yy) - XMVectorGetZ(xx)) + m_LookAtZ, 0.0f);

				float radians = XM_2PI * 2.0f * positionX / m_sceneDeviceResources->GetOutputSize().Width;
				Rotate(radians);

				XMVECTOR eye = XMVectorSet(m_EyeX, m_EyeY, m_EyeZ, 0.0f);
				XMVECTOR look = XMVectorSet(m_LookAtZ, m_LookAtY, m_LookAtZ, 0.0f);
				float camToFaceCenterDist = (float)m_hot3dxGeo->distanceBetweenPoints(XMFLOAT3(m_EyeX, m_EyeY, m_EyeZ), XMFLOAT3(m_LookAtZ, m_LookAtY, m_LookAtZ));
				if(camToFaceCenterDist == 0.0f){}
				XMFLOAT3 dir = XMFLOAT3(m_hot3dxGeo->directionBetweenPoints(XMFLOAT3(m_EyeX, m_EyeY, m_EyeZ), XMFLOAT3(m_LookAtZ, m_LookAtY, m_LookAtZ)));
				XMVECTOR lineDirection = XMVectorSet(dir.x * 20.f, dir.y * 20.f, dir.z * 20.f, 0.0f);
				XMVECTOR planeNormal = m_hot3dxGeo->FindPlaneNormalVec(xAxis, yAxis, zAxis);
				float planeConstant = (float)m_hot3dxGeo->FindPlaneConstantVec(planeNormal, xAxis);
				XMVECTOR intersect = m_hot3dxGeo->FindPointIntersectPlaneVec(planeNormal, eye, lineDirection, planeConstant);

				point.x = x;
				point.y = y;
				z = dir.z;

				DirectX::XMVectorSetX(mouse3dPos, XMVectorGetX(intersect));
				DirectX::XMVectorSetY(mouse3dPos, XMVectorGetY(intersect));
				DirectX::XMVectorSetZ(mouse3dPos, XMVectorGetZ(intersect));

				m_posX = XMVectorGetX(mouse3dPos);
				m_posY = XMVectorGetY(mouse3dPos);
				m_posZ = XMVectorGetZ(mouse3dPos);
			}
		}
	}// eo m_tracking if
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyLinePointsAnyAxis()
{
	unsigned int count = m_iPointCount;
	//unsigned int j = count - 1;
	posXCopy = ref new Platform::Array<float>(m_iPointCount);
	posYCopy = ref new Platform::Array<float>(m_iPointCount);
	posZCopy = ref new Platform::Array<float>(m_iPointCount);

	for (unsigned int i = 0; i < count; i++)
	{
		posXCopy->set(i, posX->get(i));// posCursor.x;
		posYCopy->set(i, posY->get(i));// posCursor.y;
		posZCopy->set(i, posZ->get(i));// posCursor.z;
	}
	m_iCopyPointCount = m_iPointCount;
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyLinePointsAnyAxis2()
{
	unsigned int count = m_iPointCount;
	//unsigned int j = count - 1;
	posXCopy2 = ref new Platform::Array<float>(count);
	posYCopy2 = ref new Platform::Array<float>(count);
	posZCopy2 = ref new Platform::Array<float>(count);

	for (unsigned int i = 0; i < count; i++)
	{
		posXCopy2->set(i, posX->get(i));// posCursor.x;
		posYCopy2->set(i, posY->get(i));// posCursor.y;
		posZCopy2->set(i, posZ->get(i));// posCursor.z;
	}
	m_iCopyPointCount2 = m_iPointCount;
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyLinePointsAnyAxis3()
{
	unsigned int count = m_iCopyPointCount3 = m_iPointCount;
	//unsigned int j = count - 1;
	posXCopy3 = ref new Platform::Array<float>(count);
	posYCopy3 = ref new Platform::Array<float>(count);
	posZCopy3 = ref new Platform::Array<float>(count);

	for (unsigned int i = 0; i < count; i++)
	{
		posXCopy3->set(i, posX->get(i));// posCursor.x;
		posYCopy3->set(i, posY->get(i));// posCursor.y;
		posZCopy3->set(i, posZ->get(i));// posCursor.z;
	}
	//m_iCopyPointCount3 = m_iPointCount;
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CopyLinePointsAnyAxis4()
{
	unsigned int count = m_iCopyPointCount4 = m_iPointCount;;
	//unsigned int j = count - 1;
	posXCopy4 = ref new Platform::Array<float>(count);
	posYCopy4 = ref new Platform::Array<float>(count);
	posZCopy4 = ref new Platform::Array<float>(count);

	for (unsigned int i = 0; i < count; i++)
	{
		posXCopy4->set(i, posX->get(i));// posCursor.x;
		posYCopy4->set(i, posY->get(i));// posCursor.y;
		posZCopy4->set(i, posZ->get(i));// posCursor.z;
	}
	//m_iCopyPointCount4 = m_iPointCount;
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetLinePointsObject()
{

	// Register our SwapChainPanel to get independent input pointer events
	// Register our SwapChainPanel to get independent input pointer events
	auto drawItemHandlerLines = ref new WorkItemHandler([this](IAsyncAction^ action)
		{
			critical_section::scoped_lock lock(m_drawCriticalSectionLines);
			if (m_loadingDrawnLineObjectComplete == true)
			{
				//CalculateLinesMeshFaces();
				CalculateDifLensLinesMeshFaces();

			}
			m_iDrawMode = 2;
		});

	// Run task on a dedicated high priority background thread.
	m_drawObjectWorkerLines = ThreadPool::RunAsync(drawItemHandlerLines, WorkItemPriority::High, WorkItemOptions::TimeSliced);
	m_drawObjectWorkerLines->Completed::get();
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetLinePoints(unsigned int lineId)
{
	// Begin Check to see
	size_t id = static_cast<size_t>(lineId - 2);
	XMFLOAT3 move = m_LinePtsList.at(id)->GetTranslateLine();
	XMFLOAT3 rotate = m_LinePtsList.at(id)->GetRotateLine();
	XMFLOAT3 scale = m_LinePtsList.at(id)->GetScaleLine();
	// End Check to see

	// Register our SwapChainPanel to get independent input pointer events
	auto drawItemHandler = ref new WorkItemHandler([this, move, rotate, scale](IAsyncAction^ action)
		{
			{
				critical_section::scoped_lock lock(m_drawCriticalSection);

				this->SetLinesObject(move, rotate, scale);
			}
		});

	// Run task on a dedicated high priority background thread.
	m_drawObjectWorker = ThreadPool::RunAsync(drawItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetCopyLineId(unsigned int lineId)
{
	m_icopyLineId = lineId;
}
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetCopyLinePoints(unsigned int lineId)
{
	XMFLOAT3 move = m_LinePtsList.at(static_cast<size_t>(lineId - 2))->GetTranslateLine();
	XMFLOAT3 rotate{};// = m_LinePtsList.at(lineId - 2)->GetRotateLine();
	XMFLOAT3 scale{};// = m_LinePtsList.at(lineId - 2)->GetScaleLine();
	XMFLOAT4 color{};// = m_LinePtsList.at(lineId - 2)->GetColorOfLine();

	// Register our SwapChainPanel to get independent input pointer events
	auto drawItemHandler = ref new WorkItemHandler([this, move, rotate, scale, color](IAsyncAction^ action)
		{
			{
				critical_section::scoped_lock lock(m_drawCriticalSection);

				this->SetCopyLinesObject(m_icopyLineId, move, rotate, scale, color);
			}
		});

	// Run task on a dedicated high priority background thread.
	m_drawObjectWorker = ThreadPool::RunAsync(drawItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CalculateAllLinesPointsTranslateDraw(float xMove, float yMove, float zMove)
{
	m_iLineCount = static_cast<unsigned int>(m_LinePtsList.size());

	for (size_t j = 0; j < m_iLineCount; ++j)
	{
		unsigned int cnt = m_LinePtsList.at(j)->GetLinePtCnt();
		for (unsigned int i = 0; i < cnt; ++i)
		{
			DirectX::DXTKXAML12::VertexPositionColor v1 =
				m_LinePtsList.at(j)->m_lineVerts.at(i);

			v1.position.x += xMove;
			v1.position.y += yMove;
			v1.position.z += zMove;

			m_LinePtsList.at(j)->m_lineVerts.at(i).position = v1.position;
		} // eofor(unsigned int i = 0;
	}
	m_vars->GetDXPage()->NotifyUser("Line Points Transormation Complete", NotifyType::StatusMessage);

	m_sceneDeviceResources->WaitForGpu();
	m_loadingDrawnLineObjectComplete = true;
}
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CalculateLinePointsTranslateDraw(float xMove, float yMove, float zMove)
{
	m_iLineCount = static_cast<unsigned int>(m_LinePtsList.size());
	size_t j = static_cast<size_t>(m_iLineCount - 1);

	unsigned int cnt = m_LinePtsList.at(j)->GetLinePtCnt();

	for (unsigned int i = 0; i < cnt; ++i)
	{
		DirectX::DXTKXAML12::VertexPositionColor v1 =
			m_LinePtsList.at(j)->m_lineVerts.at(i);

		v1.position.x += xMove;
		v1.position.y += yMove;
		v1.position.z += zMove;

		m_LinePtsList.at(j)->m_lineVerts.at(i).position = v1.position;
	} // eofor(unsigned int i = 0;

	m_vars->GetDXPage()->NotifyUser("Line Points Transormation Complete", NotifyType::StatusMessage);

	m_sceneDeviceResources->WaitForGpu();
	m_loadingDrawnLineObjectComplete = true;

	//m_vars->GetDXPage()->NotifyUser("\nReady to draw = true" + m_iLineCount, NotifyType::StatusMessage);
}
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CalculateLinePointsRotations(float xAngle, float yAngle, float zAngle)
{

	m_iGroupCount = m_iLineCount = static_cast<unsigned int>(m_LinePtsList.size());
	size_t j = static_cast<size_t>(m_iLineCount - 1);

	unsigned int cnt = m_LinePtsList.at(j)->GetLinePtCnt();

	for (unsigned int i = 0; i < cnt; ++i)
	{
		DirectX::DXTKXAML12::VertexPositionColor v1 =
			m_LinePtsList.at(j)->m_lineVerts.at(i);
		Hot3dx12Rotate rotater;
		/*
		float mag1 = (float)sqrtf(
			((float)v1.position.x * (float)v1.position.x)
			+ ((float)v1.position.y * (float)v1.position.y)
			+ ((float)v1.position.z * (float)v1.position.z));
		*/
		if (xAngle != 0.0f) {
			v1.position.y = rotater.yCoordofXRot3f(v1.position.y, v1.position.z, xAngle);
			v1.position.z = rotater.zCoordofXRot3f(v1.position.y, v1.position.z, xAngle);
		}
		if (yAngle != 0.0f) {
			v1.position.x = rotater.xCoordofYRot3f(v1.position.x, v1.position.z, yAngle);
			v1.position.z = rotater.zCoordofYRot3f(v1.position.x, v1.position.z, yAngle);
		}
		if (zAngle != 0.0f) {
			v1.position.x = rotater.xCoordofZRot3f(v1.position.x, v1.position.y, zAngle);
			v1.position.y = rotater.yCoordofZRot3f(v1.position.x, v1.position.y, zAngle);
		}
		v1.position.x;
		v1.position.y;
		v1.position.z;
		m_LinePtsList.at(j)->m_lineVerts.at(i).position = v1.position;
	} // eofor(unsigned int i = 0;

	m_vars->GetDXPage()->NotifyUser("Line Points Transormation Complete", NotifyType::StatusMessage);

	m_sceneDeviceResources->WaitForGpu();
	m_loadingDrawnLineObjectComplete = true;

	//m_vars->GetDXPage()->NotifyUser("\nReady to draw = true" + m_iLineCount, NotifyType::StatusMessage);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CalculateAllLinesPointsRotations(float xAngle, float yAngle, float zAngle)
{
	//float degreeradian = 0.017453293005625408f;

	m_iGroupCount = m_iLineCount = static_cast<unsigned int>(m_LinePtsList.size());

	for (size_t j = 0; j < m_iLineCount; ++j)
	{
		unsigned int cnt = m_LinePtsList.at(j)->GetLinePtCnt();
		for (unsigned int i = 0; i < cnt; ++i)
		{

			DirectX::DXTKXAML12::VertexPositionColor v1 =
				m_LinePtsList.at(j)->m_lineVerts.at(i);
			Hot3dx12Rotate rotater;
			/*
			float mag1 = (float)sqrtf(
				((float)v1.position.x * (float)v1.position.x)
				+ ((float)v1.position.y * (float)v1.position.y)
				+ ((float)v1.position.z * (float)v1.position.z));
			*/
			if (xAngle != 0) {


				XMVECTOR v = XMVector3Rotate(XMVectorSet(v1.position.x, v1.position.y, v1.position.z, 0.0f), XMVectorSet(0.0f, v1.position.y, 0.0f, xAngle));

				v1.position.y = rotater.yCoordofXRot3f(v1.position.y, v1.position.z, xAngle);
				v1.position.z = rotater.zCoordofXRot3f(v1.position.y, v1.position.z, xAngle);
			}
			if (yAngle != 0) {
				XMVECTOR v = XMVector3Rotate(XMVectorSet(v1.position.x, v1.position.y, v1.position.z, 0.0f), XMVectorSet(0.0f, v1.position.y, 0.0f, yAngle));

				v1.position.x = rotater.xCoordofYRot3f(v1.position.x, v1.position.z, yAngle);
				v1.position.z = rotater.zCoordofYRot3f(v1.position.x, v1.position.z, yAngle);
			}
			if (zAngle != 0) {
				XMVECTOR v = XMVector3Rotate(XMVectorSet(v1.position.x, v1.position.y, v1.position.z, 0.0f), XMVectorSet(0.0f, v1.position.y, 0.0f, zAngle));

				v1.position.x = rotater.xCoordofZRot3f(v1.position.x, v1.position.y, zAngle);
				v1.position.y = rotater.yCoordofZRot3f(v1.position.x, v1.position.y, zAngle);
			}
			v1.position.x;
			v1.position.y;
			v1.position.z;
			m_LinePtsList.at(j)->m_lineVerts.at(i).position = v1.position;
		} // eofor(unsigned int i = 0;
	}
	m_vars->GetDXPage()->NotifyUser("All Lines Points Rotations Complete", NotifyType::StatusMessage);

	m_sceneDeviceResources->WaitForGpu();
	m_loadingDrawnLineObjectComplete = true;

	//m_vars->GetDXPage()->NotifyUser("\nReady to draw = true" + m_iLineCount, NotifyType::StatusMessage);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetLinesObject(XMFLOAT3 move, XMFLOAT3 rotate, XMFLOAT3 scale)
{
	m_loadingDrawnLineObjectComplete = false;

	IncrementLinePtGroupsSize(m_iPointCount);

	float degreeradian = 0.017453293005625408f;
	Windows::UI::Color col = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (float)(col.A * 0.00390625f);
	float r = (float)(col.R * 0.00390625f);
	float g = (float)(col.G * 0.00390625f);
	float b = (float)(col.B * 0.00390625f);
	//GXMVECTOR colors = XMVectorSet(r, g, b, alpha);
	XMFLOAT4 colors = { r, g, b, alpha };
	// Turned on to see
	DirectXPage^ dxp = m_vars->GetDXPage();
    XMFLOAT4 rotate1 = { dxp->Get_xRotateDrawnObject(), dxp->Get_yRotateDrawnObject(), dxp->Get_zRotateDrawnObject(), degreeradian };
	move = { dxp->Get_xTranslateDrawnObject(), dxp->Get_yTranslateDrawnObject(), dxp->Get_zTranslateDrawnObject() };
	XMMATRIX translation = XMMatrixTranslation(move.x,move.y,move.z) * DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&rotate1));
	rotate.x = rotate1.x;
	rotate.y = rotate1.y;
	rotate.z = rotate1.z;
	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;
	// Turned on to see END

	m_iLineCount = static_cast<unsigned int>(m_LinePtsList.size());
	size_t j = static_cast<size_t>(m_iLineCount - 1);
	uint16_t k = 0;
	m_LinePtsList.at(j)->SetColorOfLineColor(XMFLOAT4{ r,g,b,alpha });
	m_LinePtsList.at(j)->SetLinePtCnt(m_iPointCount);
	unsigned int cnt = m_LinePtsList.at(j)->GetLinePtCnt();
	unsigned int arrSz = m_LinePtsList.at(j)->GetArraySize();
	if (cnt > arrSz) {
		cnt = arrSz;
		m_LinePtsList.at(j)->SetLinePtCnt(cnt);
		m_LinePtsList.at(j)->m_lineVerts.resize(cnt);
	}
	m_LinePtsList.at(j)->m_lineVerts.resize(cnt);
	for (unsigned int i = 0; i < cnt; ++i)
	{
		XMFLOAT3 p = { 0.0f,0.0f,0.0f };
		DirectX::DXTKXAML12::VertexPositionColor v(p, colors);
		m_LinePtsList.at(j)->m_lineVerts.at(i) = v;
	}

	for (unsigned int i = 0; i < cnt; ++i)
	{
		m_LinePtsList.at(j)->SetLinePositions(i, posX->get(i), posY->get(i), posZ->get(i));
		XMFLOAT3 position = { m_LinePtsList.at(j)->GetLinePosX(i), m_LinePtsList.at(j)->GetLinePosY(i), m_LinePtsList.at(j)->GetLinePosZ(i) };
		//position.x += move.x; position.y += move.y; position.z += move.z;
		//position.x += rotate.x; position.y += rotate.y; position.z += rotate.z;

		//XMStoreFloat3(&position, XMVector3Transform(XMLoadFloat3(&position), translation));

		DirectX::DXTKXAML12::VertexPositionColor v1(position, colors);
		m_LinePtsList.at(j)->m_lineVerts.at(i) = v1;
		m_LinePtsList.at(j)->IncrementPtCnt();
		m_LinePtsList.at(j)->SetPtList(i, k);
		k++;
		m_LinePtsList.at(j)->SetPtIndexList(i, m_iTotalLinePointsCount);
		m_iTotalLinePointsCount++;
		m_vars->GetDXPage()->NotifyUser("Line set: " + j + "\nPoint Count : " + m_LinePtsList.at(j)->m_lineVerts.size(), NotifyType::StatusMessage);

	} // eofor(unsigned int i = 0;

	m_iTotalPointCount += m_iPointCount;
	posX = ref new Platform::Array<float>(1000);
	posY = ref new Platform::Array<float>(1000);
	posZ = ref new Platform::Array<float>(1000);
	m_iPointCount = 0;
	m_iTempGroup = ref new Platform::Array<unsigned int>(10000);
	m_iTempMouseX = ref new Platform::Array<float>(1000);
	m_iTempMouseY = ref new Platform::Array<float>(1000);
	m_iTempGroupCount = 0;
	m_LinePtsList.at(j)->SetLinePtCnt(cnt);

	m_vars->GetDXPage()->NotifyUser("m_loadingDrawnLineObjectComplete = true" + m_iLineCount, NotifyType::StatusMessage);

	m_sceneDeviceResources->WaitForGpu();
	m_loadingDrawnLineObjectComplete = true;

	m_vars->GetDXPage()->NotifyUser("\nReady to draw = true" + m_iLineCount, NotifyType::StatusMessage);

}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetLinesObjectMatrix(XMFLOAT3 move, unsigned int lineId, XMFLOAT3 rotate, XMFLOAT3 scale, XMFLOAT4 color)
{
	size_t id = static_cast<size_t>(lineId);
	unsigned int lastpt = static_cast<unsigned int>(m_LinePtsList.size() - 1);
	m_LinePtsList.at(id)->SetLinePosX(lineId, move.x);
	m_LinePtsList.at(id)->SetLinePosY(lineId, move.y);
	m_LinePtsList.at(id)->SetLinePosZ(lineId, move.z);
	m_LinePtsList.at(id)->TranslateLine(0, lastpt, move);
	m_LinePtsList.at(id)->RotateLine(0, lastpt, rotate);
	m_LinePtsList.at(id)->ScaleLine(0, lastpt, scale);
	m_LinePtsList.at(id)->SetColorOfLineColor(color);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetLinesObjectMatrix2(XMFLOAT3 move, unsigned int lineId, XMFLOAT3 rotate, XMFLOAT3 scale, XMFLOAT4 color)
{
	size_t id = static_cast<size_t>(lineId);
	unsigned int lastpt = static_cast<unsigned int>(m_LinePtsList.size() - 1);
	m_LinePtsList.at(id)->SetLinePosX(lineId, move.x);
	m_LinePtsList.at(id)->SetLinePosY(lineId, move.y);
	m_LinePtsList.at(id)->SetLinePosZ(lineId, move.z);
	m_LinePtsList.at(id)->TranslateLine(0, lastpt, move);
	m_LinePtsList.at(id)->RotateLine(0, lastpt, rotate);
	m_LinePtsList.at(id)->ScaleLine(0, lastpt, scale);
	m_LinePtsList.at(id)->SetColorOfLineColor(color);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetCopyLinesObject(unsigned int copyLineId, XMFLOAT3 move, XMFLOAT3 rotate, XMFLOAT3 scale, XMFLOAT4 color)
{
	m_loadingDrawnLineObjectComplete = false;

	unsigned int copyPointCount = 0;
	switch (copyLineId)
	{
	case 1:
		copyPointCount = m_iCopyPointCount;
		break;
	case 2:
		copyPointCount = m_iCopyPointCount2;
		break;
	case 3:
		copyPointCount = m_iCopyPointCount3;
		break;
	case 4:
		copyPointCount = m_iCopyPointCount4;
		break;
	};

	IncrementLinePtGroupsSize(copyPointCount);

	float degreeradian = 0.017453293005625408f;
	Windows::UI::Color col = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (float)(col.A * 0.00390625f);
	float r = (float)(col.R * 0.00390625f);
	float g = (float)(col.G * 0.00390625f);
	float b = (float)(col.B * 0.00390625f);
	//GXMVECTOR colors = XMVectorSet(r, g, b, alpha);
	XMFLOAT4 colors = { r, g, b, alpha };
	color = { r, g, b, alpha };

	DirectXPage^ dxp = m_vars->GetDXPage();
	XMFLOAT4 rotate1 = { dxp->Get_xRotateGridCam(), dxp->Get_yRotateGridCam(), dxp->Get_zRotateGridCam(), degreeradian };
	XMFLOAT4 scale1 = {1.0f,1.0f,1.0f,1.0f};
	
	float m2[3] = { rotate.x, rotate.y, rotate.z };
	XMLoadFloat(m2);
	float m3[3] = { scale.x, scale.y, scale.z };
	XMLoadFloat(m3);

	move = { dxp->Get_xTranslateDrawnObject(), dxp->Get_yTranslateDrawnObject(), dxp->Get_zTranslateDrawnObject() };
	XMMATRIX translation = XMMatrixTranslation(move.x, move.y, move.z);// *DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&rotate1));
	m_iLineCount = static_cast<unsigned int>(m_LinePtsList.size());
	size_t j = static_cast<size_t>(m_iLineCount - 1);
	uint16_t k = 0;


	m_LinePtsList.at(j)->SetLinePtCnt(copyPointCount);
	unsigned int cnt = m_LinePtsList.at(j)->GetLinePtCnt();
	unsigned int arrSz = m_LinePtsList.at(j)->GetArraySize();
	if (cnt > arrSz) {
		cnt = arrSz;
		m_LinePtsList.at(j)->SetLinePtCnt(cnt);
		m_LinePtsList.at(j)->m_lineVerts.reserve(cnt);
	}
	for (unsigned int i = 0; i < cnt; ++i)
	{
		XMFLOAT3 p = { 0.0f,0.0f,0.0f };
		DirectX::DXTKXAML12::VertexPositionColor v(p, colors);
		m_LinePtsList.at(j)->m_lineVerts.emplace_back(v);
	}

	for (unsigned int i = 0; i < cnt; ++i)
	{
		XMFLOAT3 cpyPos{};
		switch (copyLineId)
		{
		case 1:
			cpyPos = XMFLOAT3(posXCopy->get(i), posYCopy->get(i), posZCopy->get(i));
			break;
		case 2:
			cpyPos = XMFLOAT3(posXCopy2->get(i), posYCopy2->get(i), posZCopy2->get(i));
			break;
		case 3:
			cpyPos = XMFLOAT3(posXCopy3->get(i), posYCopy3->get(i), posZCopy3->get(i));
			break;
		case 4:
			cpyPos = XMFLOAT3(posXCopy4->get(i), posYCopy4->get(i), posZCopy4->get(i));
			break;
		};
		cpyPos = RotateYawPitchPointXMFL3(cpyPos, rotate1.y, rotate1.x);
		m_LinePtsList.at(j)->SetLinePositions(i, cpyPos.x, cpyPos.y, cpyPos.z);
		XMFLOAT3 position(m_LinePtsList.at(j)->GetLinePosX(i), m_LinePtsList.at(j)->GetLinePosY(i), m_LinePtsList.at(j)->GetLinePosZ(i));
		position.x += move.x; position.y += move.y; position.z += move.z;
		//position.x += rotate.x; position.y += rotate.y; position.z += rotate.z;
		//XMVECTOR posit = RotateYawPitchPoint(position, rotate.x, rotate1.y);
		//XMStoreFloat3(&position, XMVector3Transform(posit, translation));

		DirectX::DXTKXAML12::VertexPositionColor v1(position, colors);
		m_LinePtsList.at(j)->m_lineVerts.at(i) = v1;
		m_LinePtsList.at(j)->IncrementPtCnt();
		m_LinePtsList.at(j)->SetPtList(i, k);
		k++;
		m_LinePtsList.at(j)->SetPtIndexList(i, m_iTotalLinePointsCount);
		m_iTotalLinePointsCount++;
		m_vars->GetDXPage()->NotifyUser("Line set: " + j + "\nPoint Count : " + m_LinePtsList.at(j)->m_lineVerts.size(), NotifyType::StatusMessage);

	} // eofor(unsigned int i = 0;
	m_iPointCount = copyPointCount;
	m_iTotalPointCount += m_iPointCount;

	posX = ref new Platform::Array<float>(1000);
	posY = ref new Platform::Array<float>(1000);
	posZ = ref new Platform::Array<float>(1000);
	m_iPointCount = 0;
	m_iTempGroup = ref new Platform::Array<unsigned int>(10000);
	m_iTempMouseX = ref new Platform::Array<float>(1000);
	m_iTempMouseY = ref new Platform::Array<float>(1000);
	m_iTempGroupCount = 0;
	m_LinePtsList.at(j)->SetLinePtCnt(cnt);

	m_vars->GetDXPage()->NotifyUser("m_loadingDrawnLineObjectComplete = true" + m_iLineCount, NotifyType::StatusMessage);

	m_sceneDeviceResources->WaitForGpu();
	m_loadingDrawnLineObjectComplete = true;

	m_vars->GetDXPage()->NotifyUser("\nReady to draw = true" + m_iLineCount, NotifyType::StatusMessage);

}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::SetLinesObjectMatrix(unsigned int lineId, XMFLOAT3 move, XMFLOAT3 rotate, XMFLOAT3 scale, XMFLOAT4 color)
{
	size_t id = static_cast<size_t>(lineId);
	unsigned int lastpt = static_cast<unsigned int>(m_LinePtsList.size() - 1);
	m_LinePtsList.at(id)->SetLinePosX(lineId, move.x);
	m_LinePtsList.at(id)->SetLinePosY(lineId, move.y);
	m_LinePtsList.at(id)->SetLinePosZ(lineId, move.z);
	m_LinePtsList.at(id)->TranslateLine(0, lastpt, move);
	m_LinePtsList.at(id)->RotateLine(0, lastpt, rotate);
	m_LinePtsList.at(id)->ScaleLine(0, lastpt, scale);
	m_LinePtsList.at(id)->SetColorOfLineColor(color);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawLinesObject()
{
	if (m_loadingDrawnLineObjectComplete == true)
	{
		m_drawMode = static_cast<int>(RotoDrawDrawMode::DrawLineOnlyObject);
		auto commandList = m_sceneDeviceResources->GetCommandList();
		PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw Lines");
		DirectXPage^ xpg = m_vars->GetDXPage();
		const XMVECTORF32 scale = { xpg->Get_xScaleDrawnObject(), xpg->Get_yScaleDrawnObject(), xpg->Get_zScaleDrawnObject() };
		const XMVECTORF32 translate = { xpg->Get_xTranslateDrawnObject(), xpg->Get_yTranslateDrawnObject(), xpg->Get_zTranslateDrawnObject() };
		XMVECTOR angles = { xpg->Get_xRotateDrawnObject(), xpg->Get_yRotateDrawnObject(), xpg->Get_zRotateDrawnObject() };// { XM_PI / 2.f, 0.f, -XM_PI / 2.f };
		XMVECTOR rotate = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles);
		// orig
		DirectX::XMMATRIX objectLineXY = XMLoadFloat4x4(&m_world4x4) * XMMatrixTranslation(0.0f, 0.0f, 0.0f);//XMMatrixTransformation(g_XMZero, DirectX::XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
		m_lineEffect->SetWorld(objectLineXY);
		m_lineEffect->Apply(commandList);
		m_linesDrawnBatch->Begin(commandList);

		size_t lcnt = m_iLineCount;
		for (size_t j = 0; j < lcnt; j++)
		{
			unsigned int cnts = 0;
			cnts = static_cast<unsigned int>(m_LinePtsList.at(static_cast<size_t>(j))->m_lineVerts.size());
			//cnts = static_cast<unsigned int>(m_LinePtsList.at(j)->m_lineVerts.size());

			for (unsigned int i = 1; i < cnts; ++i)
			{
				m_linesDrawnBatch->DrawLine(m_LinePtsList.at(j)->m_lineVerts.at(m_LinePtsList.at(j)->GetListPt(i - 1)), m_LinePtsList.at(j)->m_lineVerts.at(m_LinePtsList.at(j)->GetListPt(i)));
			} // eofor(unsigned int i = 0;

			m_vars->GetDXPage()->NotifyUser("Line: " + j + "\nPoint Count : " + cnts, NotifyType::StatusMessage);
		} // eo for (unsigned int j

		m_linesDrawnBatch->End();
		PIXEndEvent(commandList);
	}
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawLinesObject(XMFLOAT3 move1, XMFLOAT3 rotate1, XMFLOAT3 scale1, XMFLOAT4 color1)
{
	if (m_loadingDrawnLineObjectComplete == true)
	{
		m_drawMode = static_cast<int>(RotoDrawDrawMode::DrawLineOnlyObject);
		auto commandList = m_sceneDeviceResources->GetCommandList();
		PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Draw Lines");
		DirectXPage^ xpg = m_vars->GetDXPage();
		Windows::UI::Color col{};
		col.A = unsigned char(color1.w/ 0.00390625f);
		col.R = unsigned char(color1.x / 0.00390625f);
		col.G = unsigned char(color1.y / 0.00390625f);
		col.B = unsigned char(color1.z / 0.00390625f);
		xpg->SetFrontColorDXP(col);
		const XMVECTORF32 scale = { xpg->Get_xScaleDrawnObject(), xpg->Get_yScaleDrawnObject(), xpg->Get_zScaleDrawnObject() };
		const XMVECTORF32 translate = { xpg->Get_xTranslateDrawnObject(), xpg->Get_yTranslateDrawnObject(), xpg->Get_zTranslateDrawnObject() };
		XMVECTOR angles = { xpg->Get_xRotateDrawnObject(), xpg->Get_yRotateDrawnObject(), xpg->Get_zRotateDrawnObject() };// { XM_PI / 2.f, 0.f, -XM_PI / 2.f };
		XMVECTOR rotate = DirectX::XMQuaternionRotationRollPitchYawFromVector(angles);
		float m1[3] = { move1.x, move1.y, move1.z };
		XMStoreFloat(m1, translate);
		float m2[3] = { rotate1.x, rotate1.y, rotate1.z };
		XMStoreFloat(m2, rotate);
		float m3[3] = { scale1.x, scale1.y, scale1.z };
		XMStoreFloat(m3, scale);
		// orig
		DirectX::XMMATRIX objectLineXY = XMLoadFloat4x4(&m_world4x4) * XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		// Not used yet
		XMMatrixTransformation(g_XMZero, DirectX::XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
		m_lineEffect->SetWorld(objectLineXY);
		m_lineEffect->Apply(commandList);
		m_linesDrawnBatch->Begin(commandList);

		size_t lcnt = m_iLineCount;
		for (size_t j = 0; j < lcnt; j++)
		{
			unsigned int cnts = 0;
			cnts = static_cast<unsigned int>(m_LinePtsList.at(static_cast<size_t>(j))->m_lineVerts.size());

			for (unsigned int i = 1; i < cnts; ++i)
			{
				m_linesDrawnBatch->DrawLine(m_LinePtsList.at(static_cast<size_t>(j))->m_lineVerts.at(m_LinePtsList.at(static_cast<size_t>(j))->GetListPt(i - 1)),
					m_LinePtsList.at(static_cast<size_t>(j))->m_lineVerts.at(m_LinePtsList.at(static_cast<size_t>(j))->GetListPt(i)));
			} // eofor(unsigned int i = 0;

			m_vars->GetDXPage()->NotifyUser("Line: " + j + "\nPoint Count : " + cnts, NotifyType::StatusMessage);
		} // eo for (unsigned int j

		m_linesDrawnBatch->End();
		PIXEndEvent(commandList);
	}
	//m_vars->GetDXPage()->NotifyUser("\n\nLine Drawn: " + m_iLineCount, NotifyType::StatusMessage);

}