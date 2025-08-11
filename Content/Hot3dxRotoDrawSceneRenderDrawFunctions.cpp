
#include "pch.h"
#include "DirectXPage.xaml.h"
#include "Hot3dxRotoDrawSceneRender.h"
#include "Hot3dxDrawnObject.h"
#include "Hot3dxObjectGeometry.h"

using namespace Hot3dxRotoDraw;

void Hot3dxRotoDraw::RotoDrawSceneRender::ResizePtGroups(size_t size)
{
	m_PtGroupList.clear();
	m_PtGroupList.resize(0);
	m_PtGroupList.resize(size);

	for (size_t i = 0; i < size; i++)
	{
		PtGroups^ ptGroups = ref new PtGroups(m_iPointCount);
		m_PtGroupList.at(i) = ptGroups;
	}
	
}

void Hot3dxRotoDraw::RotoDrawSceneRender::IncrementPtGroups()
{
	PtGroups^ ptGroups = ref new PtGroups(m_iPointCount);
	if (ptGroups){
		m_PtGroupList.emplace_back(ptGroups);
	}
	else {
		Concurrency::wait(10);
		m_PtGroupList.emplace_back(ptGroups);
	}
}

void Hot3dxRotoDraw::RotoDrawSceneRender::IncrementLinePtGroups()
{
	LinePtGrp^ lptGroups = ref new LinePtGrp(65);
	m_LinePtsList.emplace_back(lptGroups);
}

void Hot3dxRotoDraw::RotoDrawSceneRender::IncrementLinePtGroupsSize(unsigned int sizeOfLine)
{
	LinePtGrp^ lptGroups = ref new LinePtGrp(sizeOfLine);
	m_LinePtsList.emplace_back(lptGroups);
}

uint16_t Hot3dxRotoDraw::RotoDrawSceneRender::DrawObjectPoints(uint16_t n)
{
	if (m_iPointCount <= 0) { return 0; }
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	bool closed = page->m_Scene2Vars->GetOpenOrClosedChecked();
	m_drawMode = (int)RotoDrawDrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	m_fCamMove_degreeradian = 0.017453293005625408f;

	m_fPointDrawGroupAngle = page->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)((360.0f/ m_fPointDrawGroupAngle) - page->m_Scene2Vars->GetPartialRotateAngle());
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = page->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);
	
	m_fScale1stLineDrawnPts = 1.33f;
	uint16_t k = n;
	vertices.clear();
	vertices.resize(0);
	if (closed == true)
	{
		vertices.resize(static_cast<size_t>(m_iPointCount + m_iPointCount + (m_iPointCount * (cnt-1))));
		ResizePtGroups(static_cast<size_t>(cnt+1));
	}
	else {
		vertices.resize(static_cast<size_t>(m_iPointCount + (m_iPointCount * (cnt-1))));
		ResizePtGroups(static_cast<size_t>(cnt));
	}
		
		if (m_bIsYAxis)
		{   
			
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				m_PtGroupList.at(0)->SetPtList(i, k); 
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i) * m_fScale1stLineDrawnPts, posY->get(i), posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
				
				vertices.at(k)=vpc;
				k++;
			}
			
			for (unsigned int j = 1; j < cnt; j++)
			{				
				for (unsigned int i = 0; i < m_iPointCount; i++)
				{
					m_PtGroupList.at(j)->SetPtList(i, k);
					float aa = (float)j * a;
					float x = m_hot3dxRotate->xCoordofYRot3f(posX->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofYRot3f(posX->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x * m_fScale1stLineDrawnPts, posY->get(i), z * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;
					k++;
				} // eo for i
			}// eo for j

			if (closed == true)
			{
				if (m_bIsYAxis)
				{
					size_t sz = m_PtGroupList.size() - 1;
					for (unsigned int i = 0; i < m_iPointCount; i++)
					{
						m_PtGroupList.at(sz)->SetPtList(i, k); 
						DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i) * m_fScale1stLineDrawnPts, posY->get(i), posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
						vertices.at(k) = vpc;
						
						k++;
					}
				}
			}
		}
		else if (!m_bIsYAxis)
		{
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i) * m_fScale1stLineDrawnPts, posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
				vertices.at(k) = vpc;
				m_PtGroupList.at(0)->SetPtList(i, k);
				k++;
			}

			for (unsigned int j = 1; j < cnt; j++)
			{
				for (unsigned int i = 0; i < m_iPointCount; i++)
				{
					float aa = (float)j * a;
					float y = m_hot3dxRotate->yCoordofXRot3f(posY->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofXRot3f(posY->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), y * m_fScale1stLineDrawnPts, z * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;
					m_PtGroupList.at(j)->SetPtList(i, k);
					k++;
				} // eo for i
			}// eo for j

			if (closed == true)
			{
				if (!m_bIsYAxis)
				{
					size_t sz = m_PtGroupList.size() - 1;
					for (unsigned int i = 0; i < m_iPointCount; i++)
					{
						DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i) * m_fScale1stLineDrawnPts, posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
						vertices.at(k) = vpc;
						m_PtGroupList.at(sz)->SetPtList(i, k);
						k++;
					}
				}
			}// EO if (closed == true)
		}

		m_iTotalPointCount = k;
		if (vertices.size() < k)
		{
           Platform::String^ message = ref new Platform::String(L"Line 125 DrawPoints() vertices are less than the m_iTotalPointCount");

			m_vars->GetDXPage()->NotifyUser(message, NotifyType::ErrorMessage);
		}
		else if (vertices.size() > k)
		{
			size_t pcnt = vertices.size() - k;
			for (size_t i = 0; i < pcnt; i++)
			{
				vertices.pop_back();
			}
		}
		m_iGroupCount = (unsigned int)m_PtGroupList.size();
		return k;
}

void Hot3dxRotoDraw::RotoDrawSceneRender::DrawObjectPointsTop()
{
	if (m_iPointCount <= 0) { return; }
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	bool closed = page->m_Scene2Vars->GetOpenOrClosedChecked();
	m_drawMode = (int)RotoDrawDrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	m_fCamMove_degreeradian = 0.017453293005625408f;
	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = page->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)((360.0f / m_fPointDrawGroupAngle) - page->m_Scene2Vars->GetPartialRotateAngle());

	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = page->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);
	m_fScale1stLineDrawnPts = 1.33f;
	uint16_t k = 0;
	vertices.clear();
	vertices.resize(0);
	
	if (closed == true)
	{
		vertices.resize(static_cast<size_t>(m_iPointCount + (m_iPointCount * (cnt+1))));
		ResizePtGroups(static_cast<size_t>(cnt +1));
	}
	else {
		vertices.resize(static_cast<size_t>(m_iPointCount * (cnt)));
		ResizePtGroups(static_cast<size_t>(cnt));
	}
	
	if (m_bIsYAxis)
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == 0)
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i) * m_fScale1stLineDrawnPts, posY->get(i), posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float x = m_hot3dxRotate->xCoordofYRot3f(posX->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofYRot3f(posX->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x * m_fScale1stLineDrawnPts, posY->get(i), z * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j

		if (closed == true)
		{
			unsigned int j = cnt;
			
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i) * m_fScale1stLineDrawnPts, posY->get(i), posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
				vertices.at(k) = vpc;
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// EO if (closed == true)
	}
	else if (!m_bIsYAxis)
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == 0)
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i) * m_fScale1stLineDrawnPts, posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float y = m_hot3dxRotate->yCoordofXRot3f(posY->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofXRot3f(posY->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), y * m_fScale1stLineDrawnPts, z * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j

		if (closed == true)
		{
			unsigned int j = cnt;
			
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i) * m_fScale1stLineDrawnPts, posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
				vertices.at(k) = vpc;
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// EO if (closed == true)

	}

	m_iTotalPointCount = vertices.size();
	m_iGroupCount = (unsigned int)m_PtGroupList.size();
}

void Hot3dxRotoDraw::RotoDrawSceneRender::DrawObjectPointsBottom()
{
	if (m_iPointCount <= 0) { return; }
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	bool closed = page->m_Scene2Vars->GetOpenOrClosedChecked();
	m_drawMode = (int)RotoDrawDrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	m_fCamMove_degreeradian = 0.017453293005625408f;

	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = page->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)((360.0f / m_fPointDrawGroupAngle) - page->m_Scene2Vars->GetPartialRotateAngle());
	//unsigned int cnt = (unsigned int)((360.0f - page->m_Scene2Vars->GetPartialRotateAngle()) / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = page->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);
	m_fScale1stLineDrawnPts = 1.33f;
	vertices.clear();
	vertices.resize(0);
	if (closed == true)
	{
		vertices.resize(static_cast<size_t>(m_iPointCount + (m_iPointCount * (cnt + 1))));
		ResizePtGroups(static_cast<size_t>(cnt + 1));
	}
	else {
		vertices.resize(static_cast<size_t>(m_iPointCount * (cnt)));
		ResizePtGroups(static_cast<size_t>(cnt));
	}
	uint16_t k = 0;

	if (m_bIsYAxis)
	{
		

		for (unsigned int j = 0; j < cnt; j++)
		{
			

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == (m_iPointCount - 1))
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i) * m_fScale1stLineDrawnPts, posY->get(i), posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;// emplace_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float x = m_hot3dxRotate->xCoordofYRot3f(posX->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofYRot3f(posX->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x * m_fScale1stLineDrawnPts, posY->get(i), z * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;// emplace_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j
		
		if (closed == true)
		{
			unsigned int j = cnt;
			
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i) * m_fScale1stLineDrawnPts, posY->get(i), posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
				vertices.at(k) = vpc;// emplace_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// EO if (closed == true)
	} // eo  if (m_bIsYAxis)
	else // if (m_bIsYAxis)
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == m_iPointCount - 1)
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i) * m_fScale1stLineDrawnPts, posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;// emplace_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float y = m_hot3dxRotate->yCoordofXRot3f(posY->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofXRot3f(posY->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), y * m_fScale1stLineDrawnPts, z * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;// emplace_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j

		if (closed == true)
		{
			unsigned int j = cnt;
			
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i) * m_fScale1stLineDrawnPts, posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
				vertices.at(k) = vpc;// emplace_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// EO if (closed == true)

	} //eo else (m_bIsYAxis)

	m_iTotalPointCount = vertices.size();
	m_iGroupCount = (unsigned int)m_PtGroupList.size();

}

void Hot3dxRotoDraw::RotoDrawSceneRender::DrawObjectPointsTopBottom()
{
	if (m_iPointCount <= 0) { return; }
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	bool closed = page->m_Scene2Vars->GetOpenOrClosedChecked();
	m_drawMode = (int)RotoDrawDrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	m_fCamMove_degreeradian = 0.017453293005625408f;
	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = m_vars->GetDXPage()->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)((360.0f / m_fPointDrawGroupAngle) - page->m_Scene2Vars->GetPartialRotateAngle());
	//unsigned int cnt = (unsigned int)(360.0f / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);
	m_fScale1stLineDrawnPts = 1.33f;
	vertices.clear();
	vertices.resize(0);
	if (closed == true)
	{
		vertices.resize(static_cast<size_t>(m_iPointCount + (m_iPointCount * (cnt))));
		ResizePtGroups(static_cast<size_t>(cnt + 1));
	}
	else {
		vertices.resize(static_cast<size_t>(m_iPointCount * (cnt)));
		ResizePtGroups(static_cast<size_t>(cnt));
	}
	uint16_t k = 0;

	if (m_bIsYAxis)
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == (m_iPointCount - 1))
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i) * m_fScale1stLineDrawnPts, posY->get(i), posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;// emplace_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float x = m_hot3dxRotate->xCoordofYRot3f(posX->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofYRot3f(posX->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x * m_fScale1stLineDrawnPts, posY->get(i), z * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;// emplace_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j

		if (closed == true)
		{
			unsigned int j = cnt;
			
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i) * m_fScale1stLineDrawnPts, posY->get(i), posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
				vertices.at(k) = vpc;// emplace_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// EO if (closed == true)
	}
	else  // if (m_bIsYAxis)
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == m_iPointCount - 1)
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i) * m_fScale1stLineDrawnPts, posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;// emplace_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float y = m_hot3dxRotate->yCoordofXRot3f(posY->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofXRot3f(posY->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), y * m_fScale1stLineDrawnPts, z * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
					vertices.at(k) = vpc;// emplace_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j

		if (closed == true)
		{
			unsigned int j = cnt;
			
			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i) * m_fScale1stLineDrawnPts, posZ->get(i) * m_fScale1stLineDrawnPts), XMFLOAT4(r,g,b,alpha) };
				vertices.at(k) = vpc;// emplace_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// EO if (closed == true)

	}

	m_iTotalPointCount = vertices.size();
	m_iGroupCount = (unsigned int)m_PtGroupList.size();

}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateMeshFaces()
{
	indices.clear();
	indices.resize(0);
	if (m_vars->GetDXPage()->m_Scene2Vars->GetOpenOrClosedChecked() == true)
	{
		uint16_t k = 0;
		k = ((((uint16_t)m_iGroupCount - 1) * ((uint16_t)m_iPointCount - 1) * 6) + (((uint16_t)m_iPointCount - 1) * 6));
		indices.resize(static_cast<size_t>(k));
	}
	else {
		uint16_t k = 0;
		k = ((((uint16_t)m_iGroupCount - 1) * ((uint16_t)m_iPointCount - 1) * 6));
		indices.resize(static_cast<size_t>(k));
	}
	uint16_t k = 0;
	for (unsigned int i = 0; i < m_iGroupCount - 1; i++)
	{
		for (unsigned int j = 0; j < m_iPointCount - 1; j++)
		{
			size_t posI = static_cast<size_t>(i);
			// First Face
			uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
			uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
			uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
			uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
			indices.at(k) = a; k++;
			indices.at(k) = b; k++;
			indices.at(k) = c; k++;
			// Second Face
			indices.at(k) = a; k++;
			indices.at(k) = c; k++;
			indices.at(k) = d; k++;
		}
	}
	if (m_vars->GetDXPage()->m_Scene2Vars->GetOpenOrClosedChecked())
	{
		// From Last Group to First Group
		unsigned int i = m_iGroupCount - 1;
		for (unsigned int j = 0; j < m_iPointCount - 1; j++)
		{
			size_t posI = static_cast<size_t>(i);
			// First Face
			uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
			uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
			uint16_t c = m_PtGroupList.at(0)->GetListPt(j + 1);
			uint16_t d = m_PtGroupList.at(0)->GetListPt(j);
			indices.at(k) = a; k++;
			indices.at(k) = b; k++;
			indices.at(k) = c; k++;
			// Second Face
			indices.at(k) = a; k++;
			indices.at(k) = c; k++;
			indices.at(k) = d; k++;
		}
	}
	indices.resize(k);
}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateMeshFacesTopBottom()
{
	indices.clear();
	indices.resize(0);
	uint16_t k = 0;
	for (unsigned int i = 0; i < m_iGroupCount - 1; i++)
	{
		for (unsigned int j = 0; j < m_iPointCount - 1; j++)
		{
			// First Face
			if (j == 0)
			{
				k += 3;
			}
			else if (j == m_iPointCount - 2)
			{
				k += 3;
			}
			else
			{
				k += 6;
			}
		}
	}
	indices.resize(k);
	///////////////
	k = 0;
	for (unsigned int i = 0; i < m_iGroupCount - 1; i++)
	{
		for (unsigned int j = 0; j < m_iPointCount - 1; j++)
		{
			// First Face
			size_t posI = static_cast<size_t>(i);
			uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
			uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
			uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
			uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
			if (j == 0)
			{
				if (static_cast<size_t>(k) + 3 > indices.size())
				{
					size_t sz = indices.size() - (static_cast<size_t>(k) + 3);
					for (uint16_t l = 0; l < sz; l++)
						indices.emplace_back(static_cast<uint16_t>(k + l));
				}
				indices.at(k) = a; k++;
				indices.at(k) = b; k++;
				indices.at(k) = c; k++;
			}
			else if (j == m_iPointCount - 2)
			{
				if (static_cast<size_t>(k) + 3 > indices.size())
				{
					size_t sz = indices.size() - (static_cast<size_t>(k) + 3);
					for (uint16_t l = 0; l < sz; l++)
						indices.emplace_back(static_cast<uint16_t>(k + l));
				}
				indices.at(k) = a; k++;
				indices.at(k) = b; k++;
				indices.at(k) = d; k++;
			}
			else
			{
				if (static_cast<size_t>(k) + 6 > indices.size())
				{
					size_t sz = indices.size() - (static_cast<size_t>(k) + 6);
					for (uint16_t l = 0; l < sz; l++)
						indices.emplace_back(static_cast<uint16_t>(k + l));
				}
				indices.at(k) = a; k++;
				indices.at(k) = b; k++;
				indices.at(k) = c; k++;
				// Second Face
				indices.at(k) = a; k++;
				indices.at(k) = c; k++;
				indices.at(k) = d; k++;
			}
		}
	}
	indices.resize(k);
		
}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateDifLensLinesMeshFaces()
{
	size_t indicesSize = 0;
	indicesSize = USHRT_MAX;
	IndexCollectionColor findexes;
	findexes.resize(indicesSize);

	/////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	uint16_t k = 0;
	for (size_t i = 0; i < static_cast<size_t>(m_iLineCount - 1); i++)
	{
		unsigned int cnts = 0;
		unsigned int cnts2 = 0;
		if (static_cast<size_t>(m_iLineCount - 1))
		{
			cnts = static_cast<unsigned int>(m_LinePtsList.at(i)->m_lineVerts.size());
			cnts2 = static_cast<unsigned int>(m_LinePtsList.at(i + 1)->m_lineVerts.size());
			if (cnts == cnts2)
			{
				for (unsigned int j = 0; j < cnts - 1; j++)
				{
					// First Face
					size_t posI = i;
					// First Face
					uint16_t a = m_LinePtsList.at(posI)->GetListPtIndex(j);
					uint16_t b = m_LinePtsList.at(posI)->GetListPtIndex(j + 1);
					uint16_t c = m_LinePtsList.at(posI + 1)->GetListPtIndex(j + 1);
					uint16_t d = m_LinePtsList.at(posI + 1)->GetListPtIndex(j);
					findexes.at(k) = a; k++;
					findexes.at(k) = b; k++;
					findexes.at(k) = c; k++;
					// Second Face
					findexes.at(k) = a; k++;
					findexes.at(k) = c; k++;
					findexes.at(k) = d; k++;
				}
			} // eo if (cnts == cnts2)
			else if (cnts > cnts2)
			{
				size_t posI = i;
				unsigned int j = 0;
				for (j = 0; j < cnts - 1; j++)
				{
					// First Face
					if (j < cnts2 - 1)
					{
						// First Face
						uint16_t a = m_LinePtsList.at(posI)->GetListPtIndex(j);
						uint16_t b = m_LinePtsList.at(posI)->GetListPtIndex(j + 1);
						uint16_t c = m_LinePtsList.at(posI + 1)->GetListPtIndex(j + 1);
						uint16_t d = m_LinePtsList.at(posI + 1)->GetListPtIndex(j);
						findexes.at(k) = a; k++;
						findexes.at(k) = b; k++;
						findexes.at(k) = c; k++;
						// Second Face
						findexes.at(k) = a; k++;
						findexes.at(k) = c; k++;
						findexes.at(k) = d; k++;
					}
					else
					{
						uint16_t a = m_LinePtsList.at(posI)->GetListPtIndex(j);
						uint16_t b = m_LinePtsList.at(posI + 1)->GetListPtIndex(cnts2 - 1);
						uint16_t c = m_LinePtsList.at(posI)->GetListPtIndex(j + 1);

						(findexes.size() < k) ? findexes.emplace_back(a) : findexes.at(k) = a; k++;
						(findexes.size() < k) ? findexes.emplace_back(b) : findexes.at(k) = b; k++;
						(findexes.size() < k) ? findexes.emplace_back(c) : findexes.at(k) = c; k++;
					}
				}


			} // eo if (cnts > cnts2)
			else if (cnts < cnts2)
			{
				size_t posI = i;
				unsigned int j = 0;
				for (j = 0; j < cnts2 - 1; j++)
				{
					// First Face
					if (j < cnts - 1)
					{
						// First Face
						uint16_t a = m_LinePtsList.at(posI)->GetListPtIndex(j);
						uint16_t b = m_LinePtsList.at(posI)->GetListPtIndex(j + 1);
						uint16_t c = m_LinePtsList.at(posI + 1)->GetListPtIndex(j + 1);
						uint16_t d = m_LinePtsList.at(posI + 1)->GetListPtIndex(j);
						findexes.at(k) = a; k++;
						findexes.at(k) = b; k++;
						findexes.at(k) = c; k++;
						// Second Face
						findexes.at(k) = a; k++;
						findexes.at(k) = c; k++;
						findexes.at(k) = d; k++;
					}
					else
					{
						uint16_t a = m_LinePtsList.at(posI + 1)->GetListPtIndex(j);
						uint16_t b = m_LinePtsList.at(posI)->GetListPtIndex(cnts - 1);
						uint16_t c = m_LinePtsList.at(posI + 1)->GetListPtIndex(j + 1);

						(findexes.size() < k) ? findexes.emplace_back(a) : findexes.at(k) = a; k++;
						(findexes.size() < k) ? findexes.emplace_back(b) : findexes.at(k) = b; k++;
						(findexes.size() < k) ? findexes.emplace_back(c) : findexes.at(k) = c; k++;
					}
				}

			} // eo if (cnts < cnts2)
		} // eo i for
		indices.resize(k);
	} // eo if m_iLineCount
}
void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndpointTopLeftFaces()
{
	indices.clear();
	indices.resize(0);
	uint16_t k = 0;
	k = (uint16_t)(((m_iGroupCount - 2) * (m_iPointCount - 1) * 6) + ((m_iGroupCount - 2) * 3));
	indices.resize(k);
	///////////////
	k = 0;
	for (unsigned int i = 0; i < m_iGroupCount - 1; i++)
	{
		for (unsigned int j = 0; j < m_iPointCount - 1; j++)
		{
			// First Face
			size_t posI = static_cast<size_t>(i);
			uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
			uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
			uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
			uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
			if (j == 0)
			{
				if (static_cast<size_t>(k) + 3 > indices.size())
				{   
					size_t sz = indices.size() - (static_cast<size_t>(k) + 3);
					for (uint16_t l = 0; l < sz; l++)
						indices.emplace_back(static_cast<uint16_t>(k + l));
				}
				indices.at(k) = a; k++;
				indices.at(k) = b; k++;
				indices.at(k) = c; k++;
			}
			else
			{
				if (static_cast<size_t>(k) + 6 > indices.size())
				{
					size_t sz = indices.size() - (static_cast<size_t>(k) + 6);
					for (uint16_t l = 0; l < sz; l++)
						indices.emplace_back(static_cast<uint16_t>(k + l));
				}
				indices.at(k) = a; k++;
				indices.at(k) = b; k++;
				indices.at(k) = c; k++;
				// Second Face
				indices.at(k) = a; k++;
				indices.at(k) = c; k++;
				indices.at(k) = d; k++;
			}
		}
	}
	indices.resize(k);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndpointBottomRightFaces()
{
	indices.clear();
	indices.resize(0);
	uint16_t k = 0;
	
	k = (uint16_t)(((m_iGroupCount - 2) * (m_iPointCount - 1) * 6) + ((m_iGroupCount - 2) * 3));
	indices.resize(k);
	///////////////
	k = 0;
	for (unsigned int i = 0; i < m_iGroupCount - 1; i++)
	{
		for (unsigned int j = 0; j < m_iPointCount - 1; j++)
		{
			// First Face
			size_t posI = static_cast<size_t>(i);
			uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
			uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
			uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
			uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
			if (j == m_iPointCount - 2)
			{
				if (static_cast<size_t>(k) + 3 > indices.size())
				{
					size_t sz = indices.size() - (static_cast<size_t>(k) + 3);
					for (uint16_t l = 0; l < sz; l++)
						indices.emplace_back(static_cast<uint16_t>(k + l));
				}
				indices.at(k) = a; k++;
				indices.at(k) = b; k++;
				indices.at(k) = d; k++;
			}
			else
			{
				if (static_cast<size_t>(k) + 6 > indices.size())
				{
					size_t sz = indices.size() - (static_cast<size_t>(k) + 6);
					for (uint16_t l = 0; l < sz; l++)
						indices.emplace_back(static_cast<uint16_t>(k + l));
				}
				indices.at(k) = a; k++;
				indices.at(k) = b; k++;
				indices.at(k) = c; k++;
				// Second Face
				indices.at(k) = a; k++;
				indices.at(k) = c; k++;
				indices.at(k) = d; k++;
			}
		}
	}
	indices.resize(k);
}

Platform::String^ Hot3dxRotoDraw::RotoDrawSceneRender::GetMsgTypes(unsigned int i)
{
	Platform::String^ msgType = nullptr;
	switch (i)
	{
	case 0:
		msgType = L"Error Message: Directory Not Accessible  ";
		break;
	default:
		msgType = L"Error Message: Unknown";
		break;
	}
	return msgType;
}

Platform::String^ Hot3dxRotoDraw::RotoDrawSceneRender::GetMessages(unsigned int i)
{
	Platform::String^ message = nullptr;
	switch (i)
	{
	case 0:
		message = L"All textures must be chosen from the x64\\Release or Debug\\Hot3dxRotoDraw\\AppX\\Assets\\(Folder or sub-Folders \nPress the Clear Button after Dialog Closes\n1)Go To: Textures\n2) Add Texture1 Button from the proper doirectory\nThe directory is also in the status box lower left)";
		break;
	default:
		message = L"Something Horrible Has Happpened and one does not know what it is.";
	}
	return message;
}

