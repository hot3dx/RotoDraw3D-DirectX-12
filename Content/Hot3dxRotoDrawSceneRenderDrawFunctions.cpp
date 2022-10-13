
#include "pch.h"
#include "DirectXPage.xaml.h"
#include "Hot3dxRotoDrawSceneRender.h"

using namespace Hot3dxRotoDraw;

uint16_t Hot3dxRotoDraw::RotoDrawSceneRender::DrawObjectPoints(uint16_t n)
{
	if (m_iPointCount <= 0) { return 0; }
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	m_drawMode = (int)RotoDrawDrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	float m_fCamMove_degreeradian = 0.017453293005625408f;

	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = page->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)((360.0f - page->m_Scene2Vars->GetPartialRotateAngle()) / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = page->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);

	uint16_t k = n;
	for (unsigned int j = 0; j < 1; j++)
	{
		IncrementPtGroups();

		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(0)->SetPtList(i, k);
			k++;
		}
	}

	if (m_bIsYAxis)
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				//float pointRadius = posX->get(i);
				float aa = (float)j * a;
				float x = m_hot3dxRotate->xCoordofYRot3(posX->get(i), posZ->get(i), aa);
				float z = m_hot3dxRotate->zCoordofYRot3(posX->get(i), posZ->get(i), aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, posY->get(i), z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j

	}
	else
	{
		for (unsigned int j = 1; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				//float pointRadius = posX->get(i);
				float aa = (float)j * a;
				float y = m_hot3dxRotate->yCoordofXRot3(posY->get(i), posZ->get(i), aa);
				float z = m_hot3dxRotate->zCoordofXRot3(posY->get(i), posZ->get(i), aa);
				DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), y, z), XMFLOAT4(r,g,b,alpha) };
				vertices.push_back(vpc);
				m_PtGroupList.at(j)->SetPtList(i, k);
				k++;
			} // eo for i
		}// eo for j
	}


	IncrementPtGroups();

	size_t sz = m_PtGroupList.size() - 1;
	for (unsigned int i = 0; i < m_iPointCount; i++)
	{
		DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
		vertices.push_back(vpc);
		m_PtGroupList.at(sz)->SetPtList(i, k);
		k++;
	}
	size_t szv = vertices.size();

	m_iTotalPointCount = vertices.size();
	m_iGroupCount = (unsigned int)m_PtGroupList.size();
	return k;
}

void Hot3dxRotoDraw::RotoDrawSceneRender::DrawObjectPointsTop()
{
	if (m_iPointCount <= 0) { return; }
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	m_drawMode = (int)RotoDrawDrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	float m_fCamMove_degreeradian = 0.017453293005625408f;
	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = page->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)((360.0f - page->m_Scene2Vars->GetPartialRotateAngle()) / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = page->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);

	uint16_t k = 0;

	if (m_bIsYAxis)
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == 0)
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i + 1), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float x = m_hot3dxRotate->xCoordofYRot3(posX->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofYRot3(posX->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, posY->get(i), z), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j
		unsigned int j = cnt;
		IncrementPtGroups();
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(j)->SetPtList(i, k);
			k++;
		} // eo for i
	}
	else
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == 0)
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float y = m_hot3dxRotate->yCoordofXRot3(posY->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofXRot3(posY->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), y, z), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j
		unsigned int j = cnt;
		IncrementPtGroups();
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(j)->SetPtList(i, k);
			k++;
		} // eo for i
	}

	m_iTotalPointCount = vertices.size();
	m_iGroupCount = (unsigned int)m_PtGroupList.size();

}

void Hot3dxRotoDraw::RotoDrawSceneRender::DrawObjectPointsBottom()
{
	if (m_iPointCount <= 0) { return; }
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	m_drawMode = (int)RotoDrawDrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	float m_fCamMove_degreeradian = 0.017453293005625408f;

	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = page->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)((360.0f - page->m_Scene2Vars->GetPartialRotateAngle()) / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = page->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);

	uint16_t k = 0;

	if (m_bIsYAxis)
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == (m_iPointCount - 1))
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float x = m_hot3dxRotate->xCoordofYRot3(posX->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofYRot3(posX->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, posY->get(i), z), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j
		unsigned int j = cnt;
		IncrementPtGroups();
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(j)->SetPtList(i, k);
			k++;
		} // eo for i
	}
	else
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == m_iPointCount - 1)
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float y = m_hot3dxRotate->yCoordofXRot3(posY->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofXRot3(posY->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), y, z), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j
		unsigned int j = cnt;
		IncrementPtGroups();
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(j)->SetPtList(i, k);
			k++;
		} // eo for i
	}

	m_iTotalPointCount = vertices.size();
	m_iGroupCount = (unsigned int)m_PtGroupList.size();

}

void Hot3dxRotoDraw::RotoDrawSceneRender::DrawObjectPointsTopBottom()
{
	if (m_iPointCount <= 0) { return; }
	m_drawMode = (int)RotoDrawDrawMode::DrawSelectWithTetras;// 1;
	//float radian = 57.29577791868204900000f; 
	float m_fCamMove_degreeradian = 0.017453293005625408f;

	//float degree = (2 * PI * radian) / 360.0f;
	m_fPointDrawGroupAngle = m_vars->GetDXPage()->GetPointDrawGroupAngleDXP();
	unsigned int cnt = (unsigned int)(360.0f / m_fPointDrawGroupAngle);
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	float a = m_fPointDrawGroupAngle;
	Windows::UI::Color color = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (float)(color.A * 0.00390625f);
	float r = (float)(color.R * 0.00390625f);
	float g = (float)(color.G * 0.00390625f);
	float b = (float)(color.B * 0.00390625f);

	uint16_t k = 0;

	if (m_bIsYAxis)
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == (m_iPointCount - 1))
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float x = m_hot3dxRotate->xCoordofYRot3(posX->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofYRot3(posX->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, posY->get(i), z), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j
		unsigned int j = cnt;
		IncrementPtGroups();
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(j)->SetPtList(i, k);
			k++;
		} // eo for i
	}
	else
	{
		for (unsigned int j = 0; j < cnt; j++)
		{
			IncrementPtGroups();

			for (unsigned int i = 0; i < m_iPointCount; i++)
			{
				if (i == m_iPointCount - 1)
				{
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				else
				{
					float aa = (float)j * a;
					float y = m_hot3dxRotate->yCoordofXRot3(posY->get(i), posZ->get(i), aa);
					float z = m_hot3dxRotate->zCoordofXRot3(posY->get(i), posZ->get(i), aa);
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), y, z), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(j)->SetPtList(i, k);
				}
				k++;
			} // eo for i
		}// eo for j
		unsigned int j = cnt;
		IncrementPtGroups();
		for (unsigned int i = 0; i < m_iPointCount; i++)
		{
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(posX->get(i), posY->get(i), posZ->get(i)), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(j)->SetPtList(i, k);
			k++;
		} // eo for i
	}

	m_iTotalPointCount = vertices.size();
	m_iGroupCount = (unsigned int)m_PtGroupList.size();

}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateMeshFaces()
{

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
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			// Second Face
			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(d);
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
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			// Second Face
			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(d);
		}
	}
	//EndPointSetFaceValues();
	InitDrawnObjectSingleTexture();
	m_iDrawMode = 2;

}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateMeshFacesTopBottom()
{

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
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
			}
			else if (j == m_iPointCount - 2)
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(d);
			}
			else
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
				// Second Face
				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(d);
			}
		}
	}


	//EndPointSetFaceValues();
	InitDrawnObjectSingleTexture();
	m_iDrawMode = 2;

}


void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndpointTopLeftFaces()
{

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
			if (j == 0)
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
			}
			else
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
				// Second Face
				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(d);
			}
		}
	}
	InitDrawnObjectSingleTexture();
	m_iDrawMode = 2;
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndpointBottomRightFaces()
{

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
			if (j == m_iPointCount - 2)
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(d);
			}
			else
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
				// Second Face
				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(d);
			}
		}
	}

	InitDrawnObjectSingleTexture();
	m_iDrawMode = 2;
}