#include "pch.h"
#include "DirectXPage.xaml.h"
#include "Hot3dxSceneRenderSphere.h"
#include "Hot3dxRotoDrawSceneRender.h"
#include "Hot3dxObjectGeometry.h"

using namespace Hot3dxRotoDraw;
#define GetAValue(rgb)      (LOBYTE((rgb)>>24))

Hot3dxRotoDraw::CCameraXYMoveRotation::~CCameraXYMoveRotation() {}
//float  Hot3dxRotoDraw::CCameraXYMoveRotation::DegreesToRadians(float degree) { return degree; }
//void   Hot3dxRotoDraw::CCameraXYMoveRotation::InitSphereVars(void) {}
//void   Hot3dxRotoDraw::CCameraXYMoveRotation::CalculateSphere(int* count) { }
//void Hot3dxRotoDraw::CCameraXYMoveRotation::CalculateSphereCV(int* n, DWORD color){//return vertices;}
//void   Hot3dxRotoDraw::CCameraXYMoveRotation::MoveRotateCameraXY(int direction) {}
//void   Hot3dxRotoDraw::CCameraXYMoveRotation::MoveRotateCameraAny(float x, float y, float z) {}
void OutputGroupIdandCount(int id, size_t count)
{
	 OutputDebugString(L"\n GroupId: ");
	 OutputDebugString(std::to_wstring(id).c_str());
	 OutputDebugString(L"Group Count: ");
	 OutputDebugString(std::to_wstring(count).c_str());
	 OutputDebugString(L"\n");
}

void OutputFloat3(DirectX::DXTKXAML12::VertexPositionColor vpc)
{
	OutputDebugString(L"\n x: ");
	OutputDebugString(std::to_wstring(vpc.position.x).c_str());
	OutputDebugString(L", y: ");
	OutputDebugString(std::to_wstring(vpc.position.y).c_str());
	OutputDebugString(L", z: ");
	OutputDebugString(std::to_wstring(vpc.position.z).c_str());
	OutputDebugString(L"\n\n");
}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateSphereVPCXAxis(float camradius, float camrotation)
{
	m_fCamMove_degreeradian = 0.017453292384743690f;// ;
	m_fCamMove_anglerotation = camrotation * m_fCamMove_degreeradian;
	m_fCamMove_cameraradius = camradius;
    // all points on sphere at origin
	// x^2 + y^2 + z^2 = r^2 
	// x^2 + y^2 + z^2 - r^2 = 0
	// add x, y, z to position sphere in 3D space

	Windows::UI::Color color = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (color.A * 0.00390625f);
	float r = (color.R * 0.00390625f);
	float g = (color.G * 0.00390625f);
	float b = (color.B * 0.00390625f);
	int cnt = m_iPointCount = 0;
	float pi = 3.1415926535897932F;
	float repCount = (360.0F * m_fCamMove_degreeradian) / m_fCamMove_anglerotation;
	
	int num =  static_cast<int>(repCount);
	uint16_t k = 0;
	std::vector<float> ox;
	std::vector<float> oy;
	ox.assign(num, 0.0f);
	oy.assign(num, 0.0f);
	float angle = 0.0F;
	size_t i = 0;
	if (ox.size() > 0)
	{
		if (oy.size() > 0)
		{
            
			float distance;
			unsigned int half = 1 + num / 2;
			
			m_vSphereGroupsSizes->set(static_cast<unsigned int>(i), 1);
			for (i; i < half; i++)
			{
				ox.at(i) = m_fCamMove_cameraradius * (1.0f * cosf(angle));
				oy.at(i) = sqrtf(powf(m_fCamMove_cameraradius, 2.0f) - powf(ox.at(i), 2.0f));//*sin(angle);
				
				if (oy.at(i) != 0.0f)
				{
					distance = sqrtf(powf((ox.at(i - 1) - ox.at(i)), 2.0f)
						+ powf((oy.at(i - 1) - oy.at(i)), 2.0f));
					float radius = fabs(oy.at(i));
					float circum = 2.0f * pi * radius;
					int rep = static_cast<int>(circum / distance);
					float zangle = 0.0f;
					float addangle = (360.0f / rep) * m_fCamMove_degreeradian;
					if (rep > 0 ) {
						m_vSphereGroupsSizes->set(static_cast<unsigned int>(i), static_cast<unsigned int>(rep));
						//OutputGroupIdandCount(static_cast<int>(i), static_cast<size_t>(rep));
					}
					for (int j = 0; j < rep; j++)
					{
						cnt++;
						zangle += addangle;
					}// eo j for
				}// eo if i < 0
				else
				{
					cnt++;
					
				}
				angle += m_fCamMove_anglerotation;
				
			}// eo i for
			
			if (m_bArrayInit == true) {}
			if (cnt > 10000) { return; }
			///////////////////////////////////////
			m_fCamMove_degreeradian = 0.017453292384743690f;// m_cameraradius;
			m_fCamMove_anglerotation = camrotation * m_fCamMove_degreeradian;
			m_fCamMove_cameraradius = camradius;

				m_bArrayInit = true;
				angle = 0.0F;
				//m_iPointCount = cnt;
				cnt = 0;
				distance = 0.0F;
				//int cntP = 0;
				int p = 0; 
				i = 0;
				ResizePtGroups(half);
					for (i; i < half; i++)
					{
						ox.at(i) = (m_fCamMove_cameraradius * (1.0f * cos(angle)));
						oy.at(i) = (sqrtf(powf(m_fCamMove_cameraradius, 2.0f) - powf(ox.at(i), 2.0f)));//*sin(angle);
						
						if (i > 0)
						{
							distance = sqrtf(powf((ox.at(i - 1) - ox.at(i)), 2.0f)
								+ powf((oy.at(i - 1) - oy.at(i)), 2.0f));
							float radius = fabs(oy.at(i));
							float circum = 2.0f * pi * radius;
							unsigned int rep = (unsigned int)(circum / distance);
							float zangle = 0.0f;
							float addangle = (360.0f / rep) * m_fCamMove_degreeradian;
							// First point of the sphere x axis						
							unsigned int sz = m_vSphereGroupsSizes->get(static_cast<unsigned int>(i));
							m_PtGroupList.at(i)->SetPtListSize(sz);
							
							m_iGroupCount++;
							// OutputDebugString(L"\n New Group\n");
							for (unsigned int j = 0; j < rep; j++)
							{
								float x = ox.at(i) = ox.at(i);// +150.0f;
								float y = (radius * (1.0f * cosf(zangle)));// +150.0f;
								float z = (radius * (1.0f * sinf(zangle)));
								DrawPointsSphere(XMFLOAT3(x, y, z));
								DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
								vertices.emplace_back(vpc);
								cnt++;
								zangle += addangle;
								m_PtGroupList.at(i)->SetPtList(j, k);
								//OutputFloat3(vpc);
								k++;
							}// eo j for
							p++;
						}// eo if i < 0
						else
						{
							if (cnt == 0) {
								unsigned int sz = m_vSphereGroupsSizes->get(static_cast<unsigned int>(i));
								m_PtGroupList.at(p)->SetPtListSize(sz);
								
								float x = ox.at(0) = camradius;// +150.0F;
								float y = 0.0f;// +150.0F;
								float z = 0.0F;
								DrawPointsSphere(XMFLOAT3(x, y, z));
								DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
								vertices.emplace_back(vpc);
								m_PtGroupList.at(p)->SetPtList(static_cast<unsigned int>(i), k);
								m_iGroupCount = p++;
								k++;
								cnt++;
								//OutputFloat3(vpc);
							}
							
						} // eo else
						angle += m_fCamMove_anglerotation;
						m_iPointCount = 0;
					}// eo i for

					
					m_PtGroupList.at(static_cast<size_t>(p) - 1)->SetPtListSize(1);
					// Last point point of the sphere x axis
					float x = ox.at(i-1);
					float y = 0.0f;// +150.0F;
					float z = 0.0F;
					DrawPointsSphere(XMFLOAT3(x,y,z));
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
					vertices.emplace_back(vpc);
					m_PtGroupList.at(static_cast<size_t>(p) - 1)->SetPtList(0, k);
					m_iGroupCount = p++;
					k++;
					cnt++;
					//size_t szv = vertices.size();

					m_iTotalPointCount = vertices.size();
					m_iGroupCount = (unsigned int)m_PtGroupList.size();
					m_vSphereGroupsSizes->set(static_cast<unsigned int>(i)-1, 1);
					//OutputFloat3(vpc);
		}// eo oy != NULL
		
	}// eo ox != NULL
	
	ox.resize(0);
	oy.resize(0);

	CalculateSphereFaces();
}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateSphereVPCYAxis(float camradius, float camrotation)
{
	m_fCamMove_degreeradian = 0.017453292384743690f;// ;
	m_fCamMove_anglerotation = camrotation * m_fCamMove_degreeradian;
	m_fCamMove_cameraradius = camradius;
	// all points on sphere at origin
	// x^2 + y^2 + z^2 = r^2 
	// x^2 + y^2 + z^2 - r^2 = 0
	// add x, y, z to position sphere in 3D space
	Windows::UI::Color color = m_vars->GetDXPage()->GetFrontColorDXP();
	float alpha = (color.A * 0.00390625f);
	float r = (color.R * 0.00390625f);
	float g = (color.G * 0.00390625f);
	float b = (color.B * 0.00390625f);
	int cnt = m_iPointCount = 0;
	float pi = 3.1415926535897932F;
	float repCount = (360.0F * m_fCamMove_degreeradian) / m_fCamMove_anglerotation;

	int num =  static_cast<int>(repCount);
	uint16_t k = 0;
	std::vector<float> ox;
	std::vector<float> oy;
	ox.assign(num, 0.0f);
	oy.assign(num, 0.0f);
	float angle = 0.0F;
	size_t i = 0;
	if (oy.size() > 0)
	{
		if (ox.size() > 0)
		{

			float distance;
			unsigned int half = 1 + num / 2;

			m_vSphereGroupsSizes->set(static_cast<unsigned int>(i), 1);
			for (i; i < half; i++)
			{
				oy.at(i) = m_fCamMove_cameraradius * (1.0f * cosf(angle));
				ox.at(i) = sqrtf(powf(m_fCamMove_cameraradius, 2.0f) - powf(oy.at(i), 2.0f));//*sin(angle);

				if (ox.at(i) != 0.0f)
				{
					distance = sqrtf(powf((oy.at(i - 1) - oy.at(i)), 2.0f)
						+ powf((ox.at(i - 1) - ox.at(i)), 2.0f));
					float radius = fabs(ox.at(i));
					float circum = 2.0f * pi * radius;
					int rep =  static_cast<int>(circum / distance);
					float zangle = 0.0f;
					float addangle = (360.0f / rep) * m_fCamMove_degreeradian;
					if (rep > 0) {
						m_vSphereGroupsSizes->set(static_cast<unsigned int>(i), static_cast<unsigned int>(rep));
						//OutputGroupIdandCount(static_cast<int>(i), static_cast<size_t>(rep));
					}
					for (int j = 0; j < rep; j++)
					{
						cnt++;
						zangle += addangle;
					}// eo j for
				}// eo if i < 0
				else
				{
					cnt++;

				}
				angle += m_fCamMove_anglerotation;

			}// eo i for

			if (m_bArrayInit == true) {}
			if (cnt > 10000) { return; }
			///////////////////////////////////////
			m_fCamMove_degreeradian = 0.017453292384743690f;// m_cameraradius;
			m_fCamMove_anglerotation = camrotation * m_fCamMove_degreeradian;
			m_fCamMove_cameraradius = camradius;

			m_bArrayInit = true;
			angle = 0.0F;
			//m_iPointCount = cnt;
			cnt = 0;
			
			distance = 0.0F;
			//int cntP = 0;
			int p = 0;
			i = 0;
			ResizePtGroups(half);
			for (i; i < half; i++)
			{
				oy.at(i) = (m_fCamMove_cameraradius * (1.0f * cosf(angle)));
				ox.at(i) = (sqrtf(powf(m_fCamMove_cameraradius, 2.0f) - powf(oy.at(i), 2.0f)));//*sin(angle);

				if (i > 0)
				{
					distance = sqrtf(powf((oy.at(i - 1) - oy.at(i)), 2.0f)
						+ powf((ox.at(i - 1) - ox.at(i)), 2.0f));
					float radius = fabs(ox.at(i));
					float circum = 2.0f * pi * radius;
					unsigned int rep = (unsigned int)(circum / distance);
					float zangle = 00.0f;
					float addangle = (360.0f / rep) * m_fCamMove_degreeradian;
					// First point of the sphere x axis						

					//IncrementPtGroups();
					unsigned int sz = m_vSphereGroupsSizes->get(static_cast<unsigned int>(i));
					m_PtGroupList.at(i)->SetPtListSize(sz);
					m_iGroupCount++;
					// OutputDebugString(L"\n New Group: \nID: ");
					// OutputDebugString(std::to_wstring(i).c_str());
					// OutputDebugString(L"  cnt: ");
					// OutputDebugString(std::to_wstring(sz).c_str());
					// OutputDebugString(L"\n");

					for (unsigned int j = 0; j < rep; j++)
					{
						float x = (radius * (1.0f * cosf(zangle)));// +150.0f;
						float y = oy.at(i) = oy.at(i);// +150.0f;
						float z = (radius * (1.0f * sinf(zangle)));
						DrawPointsSphere(XMFLOAT3(x, y, z));
						DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
						vertices.emplace_back(vpc);
						cnt++;
						zangle += addangle;
						
						m_PtGroupList.at(i)->SetPtList(j, k);
						//OutputFloat3(vpc);
						k++;
					}// eo j for
					p++;
				}// eo if i < 0
				else
				{
					if (cnt == 0) {
						//IncrementPtGroups();
						unsigned int sz = m_vSphereGroupsSizes->get(static_cast<unsigned int>(i));
						m_PtGroupList.at(p)->SetPtListSize(sz);// cnt + 1);
						

						float x = 0.0f;
						float y = oy.at(0) = camradius;// +150.0F;
						float z = 0.0F;
						DrawPointsSphere(XMFLOAT3(x, y, z));
						DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
						vertices.emplace_back(vpc);
						//OutputFloat3(vpc);
						m_PtGroupList.at(p)->SetPtList(static_cast<unsigned int>(i), k);
						m_iGroupCount = p++;
						k++; 
						cnt++;
					}

				} // eo else
				angle += m_fCamMove_anglerotation;
			    m_iPointCount = 0;
			}// eo i for
			m_PtGroupList.at(static_cast<size_t>(p) - 1)->SetPtListSize(static_cast<unsigned int>(1));
			
			// Last point point of the sphere x axis
			float x = 0.0f;// +150.0F;
			float y = oy.at(i - 1);
			float z = 0.0F;
			DrawPointsSphere(XMFLOAT3(x, y, z));
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
			vertices.emplace_back(vpc);
			m_PtGroupList.at(static_cast<size_t>(p) - 1)->SetPtList(static_cast<unsigned int>(0), k);
			m_iGroupCount = p++;
			k++;
			cnt++;
			//size_t szv = vertices.size();
			//OutputFloat3(vpc);
			m_iTotalPointCount = vertices.size();
			m_iGroupCount = (unsigned int)m_PtGroupList.size();
			m_vSphereGroupsSizes->set(static_cast<unsigned int>(i) - 1, 1);
		}// eo oy != NULL

	}// eo ox != NULL

	ox.resize(0);
	oy.resize(0);

	CalculateSphereFaces();

}

void Hot3dxRotoDraw::RotoDrawSceneRender::CalculateSphereFaces()
{
	EndpointSphereTopLeftFaces(m_vSphereGroupsSizes->get(1));
	unsigned int s1 = 1;
		
	//unsigned int modulus = m_iGroupCount % 2;
	for (s1; s1 < (m_iGroupCount - 2); s1++)
	{
		CalculateSphereDifLensLinesMeshFaces(s1, m_vSphereGroupsSizes->get(s1), m_vSphereGroupsSizes->get(s1 + 1));
		/*
		if (modulus == 0)
		{
			if (s1 < (m_iGroupCount / 2))
			{
				MidSphereGreaterLatitudeTopLeftFaces(s1, m_vSphereGroupsSizes->get(s1), m_vSphereGroupsSizes->get(s1 + 1));
			}
			else {
				MidSphereLesserLatitudeTopLeftFaces(s1, m_vSphereGroupsSizes->get(s1), m_vSphereGroupsSizes->get(s1 + 1));
			}
		}
		else
		{
			if (s1 < (m_iGroupCount / 2))
			{
				MidSphereGreaterLatitudeTopLeftFaces(s1, m_vSphereGroupsSizes->get(s1), m_vSphereGroupsSizes->get(s1 + 1));
			}
			else {
				MidSphereLesserLatitudeTopLeftFaces(s1, m_vSphereGroupsSizes->get(s1), m_vSphereGroupsSizes->get(s1 + 1));
			}
			
		}
		*/
	}
	
	EndpointSphereBottomRightFaces(m_vSphereGroupsSizes->get(m_iGroupCount-2));
	InitSphere();

	m_iDrawMode = 2;

}


void Hot3dxRotoDraw::RotoDrawSceneRender::InitSphereVB2(float camradius, float camrotation)
{
	//Switchws between x axis and y axis drawing

	m_vSphereGroupsSizes = ref new Platform::Array<unsigned int>(1000);
	
	if (m_bIsYAxis)
	{
		CalculateSphereVPCYAxis(camradius, camrotation);
	}
	else
	{
		CalculateSphereVPCXAxis(camradius, camrotation);
	}
}


void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::DrawPointsSphere(XMFLOAT3 cursorPos)
{
			if (!posX || posX->Length < 10000)
			{
				posX = ref new Platform::Array<float>(10000);
			}
			if (!posY || posY->Length < 10000)
			{
				posY = ref new Platform::Array<float>(10000);
			}
			if (!posZ || posZ->Length < 10000)
			{
				posZ = ref new Platform::Array<float>(10000);
			}

			if (!m_iTempMouseX || m_iTempMouseX->Length < 10000)
			{
				m_iTempMouseX = ref new Platform::Array<float>(10000);
			}
			if (!m_iTempMouseY || m_iTempMouseY->Length < 10000)
			{
				this->m_iTempMouseY = ref new Platform::Array<float>(10000);
			}

			posX->set(m_iPointCount, cursorPos.x);// posCursor.x;
			posY->set(m_iPointCount, cursorPos.y);// posCursor.y;
			posZ->set(m_iPointCount, cursorPos.z);// posCursor.z;
			m_previousPosX = cursorPos.x;
			m_previousPosY = cursorPos.y;
			m_previousPosZ = cursorPos.z;
			
			m_iTempGroup->set(m_iTempGroupCount, m_iPointCount);
			m_iTempMouseX->set(m_iTempGroupCount, point.x);
			m_iTempMouseY->set(m_iTempGroupCount, point.y);
			
			m_iTempGroupCount++;
			m_iPointCount++;
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndpointSphereTopLeftFaces(unsigned int secondGroupPoints)
{
	unsigned int m_iPtCount = secondGroupPoints;
	unsigned int j = 0;
		for (j = 0; j < m_iPtCount; j++)
		{
			size_t posI = 0;
			// First Face
			uint16_t a = 0;
			uint16_t b = m_PtGroupList.at(posI +1)->GetListPt(j);
			int n = j;
			if (j == m_iPtCount - 1)
			{
				n = -1;
			}
			uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(n + 1);
				indices.emplace_back(a);
				indices.emplace_back(b);
				indices.emplace_back(c); faceCnt++; faceIndiceCnt += 3;
		}
}



void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::MidSphereLesserLatitudeTopLeftFaces(unsigned int groupNum, unsigned int firstGroupPoints, unsigned int secondGroupPoints)
{
	unsigned int m_iPtCount1 = firstGroupPoints;
	unsigned int m_iPtCount2 = secondGroupPoints;
	//unsigned int i = groupNum;
	uint16_t a = 0;
	uint16_t b = 0;
	uint16_t c = 0;
	uint16_t d = 0;
	unsigned int j = 0;
	unsigned int k = 0;

	std::vector<unsigned int> numLine1;
	std::vector<unsigned int> numLine2;

	for (j; j < (m_iPtCount1); j++)
	{
		numLine1.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));
	}
	j = 0;
	numLine1.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));

	for (j; j < (m_iPtCount2); j++)
	{
		numLine2.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));
	}
	j = 0;
	numLine2.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));

	unsigned int difCnt = 0;
	difCnt = static_cast<unsigned int>(numLine1.size()) - static_cast<unsigned int>(numLine2.size());

	if (difCnt == 0)
	{
		for (j = 0, k = 0; k < numLine2.size(); j++, k++)
		{
			a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
			b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
			c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1)));
			d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1)));


			indices.emplace_back(a);
			indices.emplace_back(b);
			indices.emplace_back(c);
			indices.emplace_back(c);
			indices.emplace_back(d);
			indices.emplace_back(a);
		}

	}// eo if (difCnt == 0)
	else if (m_iPtCount1 / m_iPtCount2 == 2 && m_iPtCount1 - m_iPtCount2 == 6)
	{
		for (j = 0, k = 0; j < numLine2.size() - 1; j++)
		{

			if (j < (numLine2.size()))
			{
				a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
				b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
				c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1)));
				d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1)));


				indices.emplace_back(a);
				indices.emplace_back(b);
				indices.emplace_back(c);
				indices.emplace_back(c);
				indices.emplace_back(d);
				indices.emplace_back(a);



				if (static_cast<unsigned long long>(k) + 2 < numLine1.size())
				{
					if (difCnt > 0)
					{
						b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k) + 2));
						indices.emplace_back(d);
						indices.emplace_back(c);
						indices.emplace_back(b);


						if (static_cast<unsigned long long>(k) + 3 < numLine1.size()) {
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k) + 2));
							b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k) + 3));
							indices.emplace_back(d);
							indices.emplace_back(c);
							indices.emplace_back(b);
							k++;
						}
						k++;
						difCnt -= 1;
					}
				}

			}
		}
	}
	else if (difCnt == 1)//m_iPtCount2 / m_iPtCount1 != 2 && m_iPtCount2 - m_iPtCount1 == 1)
	{
		for (j = 0, k = 0; j < numLine2.size() - 1; j++, k++)
		{
			a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
			b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
			c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1)));
			d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1)));


			indices.emplace_back(a);
			indices.emplace_back(b);
			indices.emplace_back(c);
			indices.emplace_back(c);
			indices.emplace_back(d);
			indices.emplace_back(a);

		}
		//if (k + 2 < m_iPtCount2)
		{
			d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
			c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
			b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
			indices.emplace_back(d);
			indices.emplace_back(c);
			indices.emplace_back(b);
		}
	}
	else if (difCnt == 2)
	{
		unsigned int dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))); // difCnt = 2.0f
		unsigned int modul = dif / 2;
		if (modul == 0)
		{
			dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt)));
		}
		else
		{
			dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))) - 1;
		}
		unsigned int difTrace = difCnt;
		j = 0;
		k = 0;

		for (; j < numLine2.size() - 1;)
		{
			for (unsigned int l = 0; l < dif; l++)
			{
				if (k < m_iPtCount1)
				{
					a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
					(k < numLine1.size()) ?
						c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
						c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
					(j < numLine2.size()) ?
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1))) :
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);
					j++; k++;
					if (l == dif - 1 && difTrace > 0)
					{
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
						(k < numLine1.size()) ?
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						indices.emplace_back(d);
						indices.emplace_back(b);
						indices.emplace_back(c);
						k++;
						difTrace--;
					}

				}

			}

		}

	}
	else if (difCnt == 3)
	{
		unsigned int dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))); // difCnt = 3.0f
		unsigned int modul = dif / 2;
		if (modul == 0)
		{
			dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt)));
		}
		else
		{
			dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))) - 1;
		}
		unsigned int difTrace = difCnt;
		j = 0;
		k = 0;

		for (; j < numLine2.size() - 1;)
		{
			for (unsigned int l = 0; l < dif; l++)
			{
				if (k < m_iPtCount1)
				{
					a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
					(k < numLine1.size()) ?
						c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
						c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
					(j < numLine1.size()) ?
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1))) :
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);
					j++; k++;
					if (l == dif - 1 && difTrace > 0)
					{
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
						(k < numLine1.size()) ?
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						indices.emplace_back(d);
						indices.emplace_back(b);
						indices.emplace_back(c);
						k++;
						difTrace--;
					}

				}

			}

		}
	}
	else if (difCnt == 4)
	{
		unsigned int dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))); // difCnt = 3.0f
		unsigned int modul = dif / 2;
		if (modul == 0)
		{
			dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt)));
		}
		else
		{
			dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))) - 1;
		}
		unsigned int difTrace = difCnt;
		j = 0;
		k = 0;

		for (; j < numLine2.size() - 1;)
		{
			for (unsigned int l = 0; l < dif; l++)
			{
				if (k < m_iPtCount1)
				{
					a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
					(k < numLine1.size()) ?
						c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
						c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
					(j < numLine2.size()) ?
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1))) :
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);
					j++; k++;
					if (l == dif - 1 && difTrace > 0)
					{
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
						(k < numLine1.size()) ?
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						indices.emplace_back(d);
						indices.emplace_back(b);
						indices.emplace_back(c);
						k++;
						difTrace--;
					}

				}

			}

		}
	}
	else if (difCnt >= 5)
	{
		unsigned int dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))); // difCnt = 5.0f
		unsigned int difTrace = difCnt;
		j = 0;
		k = 0;

		for (; j < numLine2.size() - 1;)
		{
			for (unsigned int l = 0; l < dif; l++)
			{
				if (k < m_iPtCount1)
				{
					a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
					(k < numLine1.size()) ?
						c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
						c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
					(j < numLine2.size()) ?
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1))) :
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);
					j++; k++;
					if (l == dif - 1 && difTrace > 0)
					{
						d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
						(k < numLine1.size()) ?
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						indices.emplace_back(d);
						indices.emplace_back(b);
						indices.emplace_back(c);
						k++;
						difTrace--;
					}

				}

			}

		}
	}
	// eo else (difCnt != 0)
	numLine1.resize(0);
	numLine2.resize(0);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::MidSphereGreaterLatitudeTopLeftFaces(unsigned int groupNum, unsigned int firstGroupPoints, unsigned int secondGroupPoints)
{
	unsigned int m_iPtCount1 = firstGroupPoints;
	unsigned int m_iPtCount2 = secondGroupPoints;
	//unsigned int i = groupNum;
	uint16_t a = 0;
	uint16_t b = 0;
	uint16_t c = 0;
	uint16_t d = 0;
	unsigned int j = 0;
	unsigned int k = 0;

	std::vector<unsigned int> numLine1;
	std::vector<unsigned int> numLine2;

	for (j; j < (m_iPtCount1); j++)
	{
		numLine1.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));
	}
	j = 0;
	numLine1.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));

	for (j; j < (m_iPtCount2); j++)
	{
		numLine2.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));
	}
	j = 0;
	numLine2.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));

	unsigned int difCnt = 0;
	difCnt = static_cast<unsigned int>(numLine2.size()) - static_cast<unsigned int>(numLine1.size());

	if (difCnt == 0)
	{
		for (j = 0, k = 0; k < numLine2.size(); j++, k++)
		{
			a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
			b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
			c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1)));
			d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1)));


			indices.emplace_back(a);
			indices.emplace_back(b);
			indices.emplace_back(c);
			indices.emplace_back(c);
			indices.emplace_back(d);
			indices.emplace_back(a);
		}

	}// eo if (difCnt == 0)
	else if (m_iPtCount2 / m_iPtCount1 == 2 && m_iPtCount2 - m_iPtCount1 == 6)
	{
		for (j = 0, k = 0; j < numLine1.size() - 1; j++)
		{

			if (j < (numLine1.size()))
			{
				a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
				b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
				c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1)));
				d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1)));


				indices.emplace_back(a);
				indices.emplace_back(b);
				indices.emplace_back(c);
				indices.emplace_back(c);
				indices.emplace_back(d);
				indices.emplace_back(a);



				if (static_cast<unsigned long long>(k) + 2 < numLine2.size())
				{
					if (difCnt > 0)
					{
						b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 2));
						indices.emplace_back(d);
						indices.emplace_back(c);
						indices.emplace_back(b);


						if (static_cast<unsigned long long>(k) + 3 < numLine2.size()) {
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 2));
							b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 3));
							indices.emplace_back(d);
							indices.emplace_back(c);
							indices.emplace_back(b);
							k++;
						}
						k++;
						difCnt -= 1;
					}
				}

			}
		}
	}
	else if (difCnt == 1)//m_iPtCount2 / m_iPtCount1 != 2 && m_iPtCount2 - m_iPtCount1 == 1)
	{
		for (j = 0, k = 0; j < numLine1.size() - 1; j++, k++)
		{
			a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
			b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
			c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1)));
			d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1)));


			indices.emplace_back(a);
			indices.emplace_back(b);
			indices.emplace_back(c);
			indices.emplace_back(c);
			indices.emplace_back(d);
			indices.emplace_back(a);

		}
		//if (k + 2 < m_iPtCount2)
		{
			d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
			c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
			b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
			indices.emplace_back(d);
			indices.emplace_back(c);
			indices.emplace_back(b);
		}
	}
	else if (difCnt == 2)
	{
		unsigned int dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))); // difCnt = 2.0f
		unsigned int modul = dif / 2;
		if (modul == 0)
		{
			dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt)));
		}
		else
		{
			dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))) - 1;
		}
		unsigned int difTrace = difCnt;
		j = 0;
		k = 0;

		for (; j < numLine1.size() - 1;)
		{
			for (unsigned int l = 0; l < dif; l++)
			{
				if (k < m_iPtCount2)
				{
					a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
					(k < numLine2.size()) ?
						c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
						c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
					(j < numLine1.size()) ?
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1))) :
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);
					j++; k++;
					if (l == dif - 1 && difTrace > 0)
					{
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
						(k < numLine2.size()) ?
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						indices.emplace_back(d);
						indices.emplace_back(b);
						indices.emplace_back(c);
						k++;
						difTrace--;
					}

				}

			}

		}

	}
	else if (difCnt == 3)
	{
		unsigned int dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))); // difCnt = 3.0f
		unsigned int modul = dif / 2;
		if (modul == 0)
		{
			dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt)));
		}
		else
		{
			dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))) - 1;
		}
		unsigned int difTrace = difCnt;
		j = 0;
		k = 0;

		for (; j < numLine1.size() - 1;)
		{
			for (unsigned int l = 0; l < dif; l++)
			{
				if (k < m_iPtCount2)
				{
					a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
					(k < numLine2.size()) ?
						c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
						c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
					(j < numLine1.size()) ?
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1))) :
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);
					j++; k++;
					if (l == dif - 1 && difTrace > 0)
					{
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
						(k < numLine2.size()) ?
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						indices.emplace_back(d);
						indices.emplace_back(b);
						indices.emplace_back(c);
						k++;
						difTrace--;
					}

				}

			}

		}
		}
	else if (difCnt == 4)
	{
		unsigned int dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))); // difCnt = 4
		unsigned int modul = dif / 2;
		if (modul == 0)
		{
			dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt)));
		}
		else
		{
			dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))) - 1;
		}

		unsigned int difTrace = difCnt;
		j = 0;
		k = 0;

		for (; j < numLine1.size() - 1;)
		{
			for (unsigned int l = 0; l < dif; l++)
			{
				if (k < m_iPtCount2)
				{
					a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
					(k < numLine2.size()) ?
						c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
						c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
					(j < numLine1.size()) ?
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1))) :
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);
					j++; k++;
					if (l == dif - 1 && difTrace > 0)
					{
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
						(k < numLine2.size()) ?
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						indices.emplace_back(d);
						indices.emplace_back(b);
						indices.emplace_back(c);
						k++;
						difTrace--;
					}

				}

			}

		}
		}
	else if (difCnt >= 5)
	{
		unsigned int dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))); // difCnt = 5.0f
		unsigned int difTrace = difCnt;
		j = 0;
		k = 0;

		for (; j < numLine1.size() - 1;)
		{
			for (unsigned int l = 0; l < dif; l++)
			{
				if (k < m_iPtCount2)
				{
					a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
					(k < numLine2.size()) ?
						c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
						c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
					(j < numLine1.size()) ?
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1))) :
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);
					j++; k++;
					if (l == dif - 1 && difTrace > 0)
					{
						d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
						(k < numLine2.size()) ?
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						indices.emplace_back(d);
						indices.emplace_back(b);
						indices.emplace_back(c);
						k++;
						difTrace--;
					}

				}

			}

		}

		
		}
		// eo else (difCnt != 0)
		numLine1.resize(0);
		numLine2.resize(0);
}

void XM_CALLCONV  Hot3dxRotoDraw::RotoDrawSceneRender::CalculateSphereDifLensLinesMeshFaces(unsigned int groupNum, unsigned int firstGroupPoints, unsigned int secondGroupPoints)
{

	
	unsigned int m_iPtCount1 = firstGroupPoints;
	unsigned int m_iPtCount2 = secondGroupPoints;
	//unsigned int i = groupNum;
	uint16_t a = 0;
	uint16_t b = 0;
	uint16_t c = 0;
	uint16_t d = 0;
	unsigned int j = 0;
	unsigned int k = 0;

	std::vector<unsigned int> numLine1;
	std::vector<unsigned int> numLine2;

	for (j; j < (m_iPtCount1); j++)
	{
		numLine1.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));
	}
	j = 0;
	numLine1.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));

	for (j; j < (m_iPtCount2); j++)
	{
		numLine2.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));
	}
	j = 0;
	numLine2.emplace_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));

	unsigned int difCnt = 0;
	
	if (static_cast<unsigned int>(numLine2.size()) == static_cast<unsigned int>(numLine1.size()))
	{
		for (j = 0, k = 0; k < numLine2.size(); j++, k++)
		{
			if (j < (numLine1.size() - 1))
			{
				a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
				b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
				c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1)));
				d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1)));


				indices.emplace_back(a);
				indices.emplace_back(b);
				indices.emplace_back(c);
				indices.emplace_back(c);
				indices.emplace_back(d);
				indices.emplace_back(a);
			}
		}

	}// eo if (difCnt == 0)
	
	if (static_cast<unsigned int>(numLine2.size()) > static_cast<unsigned int>(numLine1.size()))
	{
		////////////////////////////////////////////////////////
		//
		//  Greater or Top Latitude
		//
		////////////////////////////////////////////////////////
		difCnt = static_cast<unsigned int>(numLine2.size()) - static_cast<unsigned int>(numLine1.size());
		

		if (m_iPtCount2 / m_iPtCount1 == 2 && m_iPtCount2 - m_iPtCount1 == 6)
		{
			for (j = 0, k = 0; j < numLine1.size() - 1; j++)
			{

				if (j < (numLine1.size()))
				{
					a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
					c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1)));
					d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1)));


					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);



					if (static_cast<unsigned long long>(k) + 2 < numLine2.size())
					{
						if (difCnt > 0)
						{
							b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 2));
							indices.emplace_back(d);
							indices.emplace_back(c);
							indices.emplace_back(b);


							if (static_cast<unsigned long long>(k) + 3 < numLine2.size()) {
								c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 2));
								b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 3));
								indices.emplace_back(d);
								indices.emplace_back(c);
								indices.emplace_back(b);
								k++;
							}
							k++;
							difCnt -= 1;
						}
					}

				}
			}
		}
		else if (difCnt == 1)//m_iPtCount2 / m_iPtCount1 != 2 && m_iPtCount2 - m_iPtCount1 == 1)
		{
			for (j = 0, k = 0; j < numLine1.size() - 1; j++, k++)
			{
				a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
				b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
				c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1)));
				d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1)));


				indices.emplace_back(a);
				indices.emplace_back(b);
				indices.emplace_back(c);
				indices.emplace_back(c);
				indices.emplace_back(d);
				indices.emplace_back(a);

			}
			//if (k + 2 < m_iPtCount2)
			{
				d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
				c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
				b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
				indices.emplace_back(d);
				indices.emplace_back(c);
				indices.emplace_back(b);
			}
		}
		else if (difCnt == 2)
		{
			unsigned int dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))); // difCnt = 2.0f
			unsigned int modul = dif / 2;
			if (modul == 0)
			{
				dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt)));
			}
			else
			{
				dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))) - 1;
			}
			unsigned int difTrace = difCnt;
			j = 0;
			k = 0;

			for (; j < numLine1.size() - 1;)
			{
				for (unsigned int l = 0; l < dif; l++)
				{
					if (k < m_iPtCount2)
					{
						a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
						(k < numLine2.size()) ?
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						(j < numLine1.size()) ?
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1))) :
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(c);
						indices.emplace_back(d);
						indices.emplace_back(a);
						j++; k++;
						if (l == dif - 1 && difTrace > 0)
						{
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
							b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
							(k < numLine2.size()) ?
								c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
								c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
							indices.emplace_back(d);
							indices.emplace_back(b);
							indices.emplace_back(c);
							k++;
							difTrace--;
						}

					}

				}

			}

		}
		else if (difCnt == 3)
		{
			unsigned int dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))); // difCnt = 3.0f
			unsigned int modul = dif / 2;
			if (modul == 0)
			{
				dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt)));
			}
			else
			{
				dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))) - 1;
			}
			unsigned int difTrace = difCnt;
			j = 0;
			k = 0;

			for (; j < numLine1.size() - 1;)
			{
				for (unsigned int l = 0; l < dif; l++)
				{
					if (k < m_iPtCount2)
					{
						a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
						(k < numLine2.size()) ?
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						(j < numLine1.size()) ?
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1))) :
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(c);
						indices.emplace_back(d);
						indices.emplace_back(a);
						j++; k++;
						if (l == dif - 1 && difTrace > 0)
						{
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
							b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
							(k < numLine2.size()) ?
								c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
								c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
							indices.emplace_back(d);
							indices.emplace_back(b);
							indices.emplace_back(c);
							k++;
							difTrace--;
						}

					}

				}

			}
		}
		else if (difCnt == 4)
		{
			unsigned int dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))); // difCnt = 4
			unsigned int modul = dif / 2;
			if (modul == 0)
			{
				dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt)));
			}
			else
			{
				dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))) - 1;
			}

			unsigned int difTrace = difCnt;
			j = 0;
			k = 0;

			for (; j < numLine1.size() - 1;)
			{
				for (unsigned int l = 0; l < dif; l++)
				{
					if (k < m_iPtCount2)
					{
						a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
						(k < numLine2.size()) ?
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						(j < numLine1.size()) ?
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1))) :
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(c);
						indices.emplace_back(d);
						indices.emplace_back(a);
						j++; k++;
						if (l == dif - 1 && difTrace > 0)
						{
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
							b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
							(k < numLine2.size()) ?
								c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
								c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
							indices.emplace_back(d);
							indices.emplace_back(b);
							indices.emplace_back(c);
							k++;
							difTrace--;
						}

					}

				}

			}
		}
		else if (difCnt >= 5)
		{
			unsigned int dif = static_cast<unsigned int>(fabs(numLine1.size() / static_cast<float>(difCnt))); // difCnt = 5.0f
			
			unsigned int difTrace = difCnt;
			j = 0;
			k = 0;

			for (; j < numLine1.size() - 1;)
			{
				for (unsigned int l = 0; l < dif; l++)
				{
					if (k < m_iPtCount2)
					{
						a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
						(k < numLine2.size()) ?
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						(j < numLine1.size()) ?
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j + 1))) :
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(c);
						indices.emplace_back(d);
						indices.emplace_back(a);
						j++; k++;
						if (l == dif - 1 && difTrace > 0)
						{
							d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
							b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
							(k < numLine2.size()) ?
								c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k + 1))) :
								c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
							indices.emplace_back(d);
							indices.emplace_back(b);
							indices.emplace_back(c);
							k++;
							difTrace--;
						}

					}

				}

			}
		}
	} // eo if (If_Longer_or_Shorter_First_Line == false)
	else if (static_cast<unsigned int>(numLine2.size()) < static_cast<unsigned int>(numLine1.size()))
	{
		difCnt = static_cast<unsigned int>(numLine1.size()) - static_cast<unsigned int>(numLine2.size());

		////////////////////////////////////////////////////////
		//
		//  Greater or Top Latitude
		//
		////////////////////////////////////////////////////////

		if (m_iPtCount1 / m_iPtCount2 == 2 && m_iPtCount1 - m_iPtCount2 == 6)
		{
			for (j = 0, k = 0; j < numLine2.size() - 1; j++)
			{

				if (j < (numLine2.size()))
				{
					a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
					b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
					c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1)));
					d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1)));


					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
					indices.emplace_back(c);
					indices.emplace_back(d);
					indices.emplace_back(a);



					if (static_cast<unsigned long long>(k) + 2 < numLine1.size())
					{
						if (difCnt > 0)
						{
							b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k) + 2));
							indices.emplace_back(d);
							indices.emplace_back(c);
							indices.emplace_back(b);


							if (static_cast<unsigned long long>(k) + 3 < numLine1.size()) {
								c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k) + 2));
								b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k) + 3));
								indices.emplace_back(d);
								indices.emplace_back(c);
								indices.emplace_back(b);
								k++;
							}
							k++;
							difCnt -= 1;
						}
					}

				}
			}
		}
		else if (difCnt == 1)//m_iPtCount2 / m_iPtCount1 != 2 && m_iPtCount2 - m_iPtCount1 == 1)
		{
			for (j = 0, k = 0; j < numLine2.size() - 1; j++, k++)
			{
				a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
				b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
				c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1)));
				d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1)));


				indices.emplace_back(a);
				indices.emplace_back(b);
				indices.emplace_back(c);
				indices.emplace_back(c);
				indices.emplace_back(d);
				indices.emplace_back(a);

			}
			//if (k + 2 < m_iPtCount2)
			{
				d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
				c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
				b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
				indices.emplace_back(d);
				indices.emplace_back(c);
				indices.emplace_back(b);
			}
		}
		else if (difCnt == 2)
		{
			unsigned int dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))); // difCnt = 2.0f
			unsigned int modul = dif / 2;
			if (modul == 0)
			{
				dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt)));
			}
			else
			{
				dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))) - 1;
			}
			unsigned int difTrace = difCnt;
			j = 0;
			k = 0;

			for (; j < numLine2.size() - 1;)
			{
				for (unsigned int l = 0; l < dif; l++)
				{
					if (k < m_iPtCount1)
					{
						a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
						(k < numLine1.size()) ?
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						(j < numLine2.size()) ?
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1))) :
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(c);
						indices.emplace_back(d);
						indices.emplace_back(a);
						j++; k++;
						if (l == dif - 1 && difTrace > 0)
						{
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
							b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
							(k < numLine1.size()) ?
								c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
								c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
							indices.emplace_back(d);
							indices.emplace_back(b);
							indices.emplace_back(c);
							k++;
							difTrace--;
						}

					}

				}

			}

		}
		else if (difCnt == 3)
		{
			unsigned int dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))); // difCnt = 3.0f
			unsigned int modul = dif / 2;
			if (modul == 0)
			{
				dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt)));
			}
			else
			{
				dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))) - 1;
			}
			unsigned int difTrace = difCnt;
			j = 0;
			k = 0;

			for (; j < numLine2.size() - 1;)
			{
				for (unsigned int l = 0; l < dif; l++)
				{
					if (k < m_iPtCount1)
					{
						a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
						(k < numLine1.size()) ?
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						(j < numLine1.size()) ?
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1))) :
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(c);
						indices.emplace_back(d);
						indices.emplace_back(a);
						j++; k++;
						if (l == dif - 1 && difTrace > 0)
						{
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
							b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
							(k < numLine1.size()) ?
								c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
								c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
							indices.emplace_back(d);
							indices.emplace_back(b);
							indices.emplace_back(c);
							k++;
							difTrace--;
						}

					}

				}

			}
		}
		else if (difCnt == 4)
		{
			unsigned int dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))); // difCnt = 3.0f
			unsigned int modul = dif / 2;
			if (modul == 0)
			{
				dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt)));
			}
			else
			{
				dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))) - 1;
			}
			unsigned int difTrace = difCnt;
			j = 0;
			k = 0;

			for (; j < numLine2.size() - 1;)
			{
				for (unsigned int l = 0; l < dif; l++)
				{
					if (k < m_iPtCount1)
					{
						a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
						(k < numLine1.size()) ?
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						(j < numLine2.size()) ?
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1))) :
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(c);
						indices.emplace_back(d);
						indices.emplace_back(a);
						j++; k++;
						if (l == dif - 1 && difTrace > 0)
						{
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
							b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
							(k < numLine1.size()) ?
								c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
								c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
							indices.emplace_back(d);
							indices.emplace_back(b);
							indices.emplace_back(c);
							k++;
							difTrace--;
						}

					}

				}

			}
		}
		else if (difCnt >= 5)
		{
			unsigned int dif = static_cast<unsigned int>(fabs(numLine2.size() / static_cast<float>(difCnt))); // difCnt = 5.0f
			unsigned int difTrace = difCnt;
			j = 0;
			k = 0;

			for (; j < numLine2.size() - 1;)
			{
				for (unsigned int l = 0; l < dif; l++)
				{
					if (k < m_iPtCount1)
					{
						a = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
						b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
						(k < numLine1.size()) ?
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
							c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
						(j < numLine2.size()) ?
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j + 1))) :
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(0)));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(c);
						indices.emplace_back(d);
						indices.emplace_back(a);
						j++; k++;
						if (l == dif - 1 && difTrace > 0)
						{
							d = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
							b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k)));
							(k < numLine1.size()) ?
								c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k + 1))) :
								c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(0)));
							indices.emplace_back(d);
							indices.emplace_back(b);
							indices.emplace_back(c);
							k++;
							difTrace--;
						}

					}

				}

			}
		}
	}
	// eo else (difCnt != 0)
	numLine1.resize(0);
	numLine2.resize(0);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::CalculateDifLensLinesMeshFacesSphere(unsigned int groupNum, unsigned int firstGroupPoints, unsigned int secondGroupPoints)
{
	indices.clear();
	indices.resize(0);
	if( -1 != groupNum){}
	/////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	uint16_t k = 0;
	for (size_t i = 0; i < static_cast<size_t>(m_iGroupCount - 2); i++)
	{
		unsigned int cnts = firstGroupPoints = m_vSphereGroupsSizes->get(static_cast<unsigned int>(i));
		unsigned int cnts2 = secondGroupPoints = m_vSphereGroupsSizes->get(static_cast<unsigned int>(i) + 1);
		if (static_cast<size_t>(m_iGroupCount - 1))
		{
			cnts = firstGroupPoints; // static_cast<unsigned int>(m_PtGroupList.at(i)->m_lineVerts.size());
			cnts2 = secondGroupPoints; // static_cast<unsigned int>(m_PtGroupList.at(i + 1)->m_lineVerts.size());
			if (cnts == cnts2)
			{
				for (unsigned int j = 0; j < cnts - 1; j++)
				{
					// First Face
					size_t posI = i;
					// First Face
					uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
					uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
					uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
					uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
					indices.emplace_back(a); k++;
					indices.emplace_back(b); k++;
					indices.emplace_back(c); k++;
					// Second Face
					indices.emplace_back(a); k++;
					indices.emplace_back(c); k++;
					indices.emplace_back(d); k++;
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
						uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
						uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
						uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
						uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
						indices.emplace_back(a); k++;
						indices.emplace_back(b); k++;
						indices.emplace_back(c); k++;
						// Second Face
						indices.emplace_back(a); k++;
						indices.emplace_back(c); k++;
						indices.emplace_back(d); k++;
					}
					else
					{
						uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
						uint16_t b = m_PtGroupList.at(posI + 1)->GetListPt(cnts2 - 1);
						uint16_t c = m_PtGroupList.at(posI)->GetListPt(j + 1);

						(indices.size() < k) ? indices.emplace_back(a) : indices.emplace_back(a); k++;
						(indices.size() < k) ? indices.emplace_back(b) : indices.emplace_back(b); k++;
						(indices.size() < k) ? indices.emplace_back(c) : indices.emplace_back(c); k++;
					}
				}


			} // eo if (cnts > cnts2)
			else if (cnts < cnts2)
			{
				size_t posI = i + 1;
				unsigned int j = 0;
				for (j = 0; j < cnts2 - 1; j++)
				{
					// First Face
					if (j < cnts - 1)
					{
						// First Face
						uint16_t a = m_PtGroupList.at(posI)->GetListPt(j);
						uint16_t b = m_PtGroupList.at(posI)->GetListPt(j + 1);
						uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);
						uint16_t d = m_PtGroupList.at(posI + 1)->GetListPt(j);
						indices.emplace_back(a); k++;
						indices.emplace_back(b); k++;
						indices.emplace_back(c); k++;
						// Second Face
						indices.emplace_back(a); k++;
						indices.emplace_back(c); k++;
						indices.emplace_back(d); k++;
					}
					else
					{
						uint16_t a = m_PtGroupList.at(posI + 1)->GetListPt(j);
						uint16_t b = 0;
						(j  < cnts - 1 ? b = m_PtGroupList.at(posI)->GetListPt(j + 1) : b = m_PtGroupList.at(posI)->GetListPt(0));
						// orig b = m_PtGroupList.at(posI)->GetListPt(cnts - 1);
						uint16_t c =
							(j + 1 < cnts - 1 ? c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1) : c = m_PtGroupList.at(posI + 1)->GetListPt(0));
						// orig uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(j + 1);

						(indices.size() < k) ? indices.emplace_back(a) : indices.emplace_back(a); k++;
						(indices.size() < k) ? indices.emplace_back(b) : indices.emplace_back(b); k++;
						(indices.size() < k) ? indices.emplace_back(c) : indices.emplace_back(c); k++;
					}
				}

			} // eo if (cnts < cnts2)
		} // eo i for
	} // eo if m_iLineCount
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndpointSphereBottomRightFaces(unsigned int secondFromEndGroupPoints)
{
	unsigned int m_iPtCount = secondFromEndGroupPoints;
	unsigned int i = m_PtGroupList.at(static_cast<size_t>(m_iGroupCount - 1))->GetListPt(0);// m_iPointCount - 1;
	unsigned int j = 0;
	for (j = 0; j < m_iPtCount; j++)
	{
		size_t posI = static_cast<size_t>(m_iGroupCount - 1);
		// First Face
		uint16_t a = static_cast<uint16_t>(i);
		uint16_t b = m_PtGroupList.at(posI - 1)->GetListPt(j);
		int n = j;
		if (j == m_iPtCount - 1)
		{
			n = -1;
		}
		uint16_t c = m_PtGroupList.at(posI - 1)->GetListPt(n+1 );
		
			indices.emplace_back(b);
			indices.emplace_back(a);
			indices.emplace_back(c);
	}
}

void Hot3dxRotoDraw::RotoDrawSceneRender::InitSphere()
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
		size_t cnt = vertices.size();

		InitDimensionsBox();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		textureU.clear();
		textureV.clear();

		float g = m_textureDimension / static_cast<float>(m_iGroupCount - 1);
		float p = m_textureDimension / static_cast<float>(71 - 1);
		float pt = 0.0f;
		float gt = m_textureDimension;

		if (m_iEndPointOneCount == 1)
		{
			textureU.emplace_back(gt);
			textureV.emplace_back(pt);
		}

		// unsigned int tp = m_iTotalPointCount;
		for (unsigned int i = 0; i < (m_iGroupCount); i++)
		{

			unsigned int count = m_vSphereGroupsSizes->get(i);
			for (unsigned int j = 0; j < count; j++)
			{
				p = m_textureDimension / static_cast<float>(count);
				textureU.emplace_back(gt);
				textureV.emplace_back(pt);
				pt = pt + p;
			}
			gt = gt - g;
			pt = 0.0f;
		}
		if (m_iEndPointTwoCount == 1)
		{
			textureU.emplace_back(gt);
			textureV.emplace_back(pt);
		}
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
		Concurrency::task createLoadTask = DX::ReadDataAsync(L"\\Assets\\Textures\\Marble.DDS").then([this](std::vector<mybyte>& fileData) {
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
				m_bIsPBRModel = false;
				m_bIsDualTextureModel = false;
				m_bIsVideoTextureModel = false;
				m_bIsSculptWireframe = false;
				m_bIsBasicModelColor = false;
				m_player = false;
			});

	}
}


void Hot3dxRotoDraw::RotoDrawSceneRender::InitSphereGeo(float diameter, size_t tesselation)
{
	//auto device = m_sceneDeviceResources->GetD3DDevice();
	//m_hot3dxDrawnObject = Hot3dxDrawnObject::CreateGeoSphere();
	//const VertexCollectionColor verticesi;
	//const IndexCollectionColor indicesi;
	// m_hot3dxDrawnObject->GetVerticesI(verticesi);
	// m_hot3dxDrawnObject->GetIndicesI(indicesi);
	//m_hot3dxDrawnObject = Hot3dxDrawnObject::CreateDrawnObjectColor(verticesi, indicesi, device);

	ComputeGeoSphereColor(vertices, indices, textureCoordinates, diameter, tesselation, true);
	InitBasicEffectSphere();
}

void Hot3dxRotoDraw::RotoDrawSceneRender::InitLatLongSphere(float diameter, size_t tesselation)
{
	m_hot3dxDrawnObject->CreateSphere(vertices, indices, textureCoordinates, diameter, tesselation, true);
	InitBasicEffectSphere();
}

void Hot3dxRotoDraw::RotoDrawSceneRender::InitBasicEffectSphere()
{
	Hot3dxRotoDraw::DirectXPage^ page = m_vars->GetDXPage();
	m_fPointDrawGroupAngle = page->GetPointDrawGroupAngleDXP();
	unsigned int grcnt = (unsigned int)((360.0f / m_fPointDrawGroupAngle) - page->m_Scene2Vars->GetPartialRotateAngle());
	m_fPointDrawGroupAngle = m_fPointDrawGroupAngle * m_fCamMove_degreeradian;
	m_iGroupCount = grcnt +1;
	
	if (m_loadingDrawnObjectComplete == false)
	{
		auto device = m_sceneDeviceResources->GetD3DDevice();

		if (!device)
		{
			return;
		}

		m_DrawnMeshTexture1.Reset();
		m_shapeDrawnObjectEffect.reset();// = nullptr;
		//	m_hot3dxDrawnObject = Hot3dxDrawnObject::CreateGeoSphere(camradius, 3Ui64, true,  m_sceneDeviceResources->GetD3DDevice());
		size_t cnt = vertices.size();
		m_iPointCount = static_cast<unsigned int>(cnt)/grcnt;
		InitDimensionsBox();
		DirectX::DXTKXAML12::VertexPositionColor* vpc = vertices.data();
		// Makes the Box Frame Dimensions float* box is created 
		InitDimensionsBox();
		CreateDimensions(&vpc->position, cnt);
		//GetUVPercentMySphere(m_textureDimension, m_iPointCount);
		//GetUVPercent(m_textureDimension, cnt);
		//textureU.clear();
		//textureV.clear();
		//unsigned int cnt = 0;

		
		vertexes.clear();
		vertexes.resize(cnt);
		if (textureCoordinates.size() == cnt)
			for (size_t i = 0; i < cnt; i++)
			{
				XMVECTOR n = DirectX::XMVector3Normalize(XMVectorSet(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z, 0.0f));
				DirectX::DXTKXAML12::VertexPositionNormalTexture vpnt = { XMFLOAT3(vertices.at(i).position.x,vertices.at(i).position.y,vertices.at(i).position.z), XMFLOAT3(XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n)), XMFLOAT2(textureCoordinates.at(i).x, textureCoordinates.at(i).y) };
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
				m_bIsPBRModel = false;
				m_bIsDualTextureModel = false;
				m_bIsVideoTextureModel = false;
				m_bIsSculptWireframe = false;
				m_bIsBasicModelColor = false;
				m_player = false;
			});

	}
}

