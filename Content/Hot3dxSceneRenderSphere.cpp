#include "pch.h"
#include "DirectXPage.xaml.h"
#include "Hot3dxRotoDrawSceneRender.h"

using namespace Hot3dxRotoDraw;
#define GetAValue(rgb)      (LOBYTE((rgb)>>24))

Hot3dxRotoDraw::CCameraXYMoveRotation::~CCameraXYMoveRotation() {}
//float  Hot3dxRotoDraw::CCameraXYMoveRotation::DegreesToRadians(float degree) { return degree; }
//void   Hot3dxRotoDraw::CCameraXYMoveRotation::InitSphereVars(void) {}
//void   Hot3dxRotoDraw::CCameraXYMoveRotation::CalculateSphere(int* count) { }
//void Hot3dxRotoDraw::CCameraXYMoveRotation::CalculateSphereCV(int* n, DWORD color){//return vertices;}
//void   Hot3dxRotoDraw::CCameraXYMoveRotation::MoveRotateCameraXY(int direction) {}
//void   Hot3dxRotoDraw::CCameraXYMoveRotation::MoveRotateCameraAny(float x, float y, float z) {}

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
							
							IncrementPtGroups();
							m_iGroupCount++;
							for (unsigned int j = 0; j < rep; j++)
							{
								float x = ox.at(i) = ox.at(i);// +150.0f;
								float y = (radius * (1.0f * cosf(zangle)));// +150.0f;
								float z = (radius * (1.0f * sinf(zangle)));
								DrawPointsSphere(XMFLOAT3(x, y, z));
								DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
								vertices.push_back(vpc);
								cnt++;
								zangle += addangle;
								m_PtGroupList.at(i)->SetPtList(j, k);
								
								k++;
							}// eo j for
							p++;
						}// eo if i < 0
						else
						{
							if (cnt == 0) {
								IncrementPtGroups();
								float x = ox.at(0) = camradius;// +150.0F;
								float y = 0.0f;// +150.0F;
								float z = 0.0F;
								DrawPointsSphere(XMFLOAT3(x, y, z));
								DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
								vertices.push_back(vpc);
								m_PtGroupList.at(p)->SetPtList(static_cast<unsigned int>(i), static_cast<unsigned int>(k));
								m_iGroupCount = p++;
								k++;
								cnt++;
							}
							
						} // eo else
						angle += m_fCamMove_anglerotation;
					}// eo i for

					IncrementPtGroups();

					// Last point point of the sphere x axis
					float x = ox.at(i-1);
					float y = 0.0f;// +150.0F;
					float z = 0.0F;
					DrawPointsSphere(XMFLOAT3(x,y,z));
					DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
					vertices.push_back(vpc);
					m_PtGroupList.at(p)->SetPtList(0, k);
					m_iGroupCount = p++;
					k++;
					cnt++;
					//size_t szv = vertices.size();

					m_iTotalPointCount = vertices.size();
					m_iGroupCount = (unsigned int)m_PtGroupList.size()-1;
					m_vSphereGroupsSizes->set(static_cast<unsigned int>(i)-1, 1);
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
						m_vSphereGroupsSizes->set(static_cast<unsigned int>(i), rep);
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

					IncrementPtGroups();
					m_iGroupCount++;
					for (unsigned int j = 0; j < rep; j++)
					{
						float x = (radius * (1.0f * cosf(zangle)));// +150.0f;
						float y = oy.at(i) = oy.at(i);// +150.0f;
						float z = (radius * (1.0f * sinf(zangle)));
						DrawPointsSphere(XMFLOAT3(x, y, z));
						DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
						vertices.push_back(vpc);
						cnt++;
						zangle += addangle;
						m_PtGroupList.at(i)->SetPtList(j, k);

						k++;
					}// eo j for
					p++;
				}// eo if i < 0
				else
				{
					if (cnt == 0) {
						IncrementPtGroups();
						float x = 0.0f;
						float y = oy.at(0) = camradius;// +150.0F;
						float z = 0.0F;
						DrawPointsSphere(XMFLOAT3(x, y, z));
						DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
						vertices.push_back(vpc);
						m_PtGroupList.at(p)->SetPtList(static_cast<unsigned int>(i), k);
						m_iGroupCount = p++;
						k++;
						cnt++;
					}

				} // eo else
				angle += m_fCamMove_anglerotation;
			}// eo i for

			IncrementPtGroups();

			// Last point point of the sphere x axis
			float x = 0.0f;// +150.0F;
			float y = oy.at(i - 1);
			float z = 0.0F;
			DrawPointsSphere(XMFLOAT3(x, y, z));
			DirectX::DXTKXAML12::VertexPositionColor vpc = { XMFLOAT3(x, y, z), XMFLOAT4(r,g,b,alpha) };
			vertices.push_back(vpc);
			m_PtGroupList.at(p)->SetPtList(0, k);
			m_iGroupCount = p++;
			k++;
			cnt++;
			//size_t szv = vertices.size();

			m_iTotalPointCount = vertices.size();
			m_iGroupCount = (unsigned int)m_PtGroupList.size() - 1;
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
	unsigned int modulus = m_iGroupCount % 2;
	for (s1; s1 < (m_iGroupCount - 2); s1++)
	{
		if (modulus == 0)
		{
			OutputDebugString(L"Begin Face Group: CalculateSphereFaces() if\n");
			if (s1 < (m_iGroupCount / 2))
			{
				OutputDebugString(L"In Face Group if: MidSphereGreaterLatitudeTopLeftFaces() if\n");
				MidSphereGreaterLatitudeTopLeftFaces(s1, m_vSphereGroupsSizes->get(s1), m_vSphereGroupsSizes->get(s1 + 1));
			}
			else {
				//OutputDebugString(L"In Face Group else:MidSphereLesserLatitudeTopLeftFaces() if\n");
				MidSphereLesserLatitudeTopLeftFaces(s1, m_vSphereGroupsSizes->get(s1), m_vSphereGroupsSizes->get(s1 + 1));
			}
		}
		else
		{
			OutputDebugString(L"Begin Face Group: CalculateSphereFaces() else\n");
			if (s1 < (m_iGroupCount / 2))
			{
				OutputDebugString(L"In Face Group if: MidSphereGreaterLatitudeTopLeftFaces() else\n");
				MidSphereGreaterLatitudeTopLeftFaces(s1, m_vSphereGroupsSizes->get(s1), m_vSphereGroupsSizes->get(s1 + 1));
			}
			else {
				OutputDebugString(L"In Face Group else:MidSphereLesserLatitudeTopLeftFaces() else\n");
				MidSphereLesserLatitudeTopLeftFaces(s1, m_vSphereGroupsSizes->get(s1), m_vSphereGroupsSizes->get(s1 + 1));
			}
			
		}
		OutputDebugString(L"\nEnd Face Group: \n");
	}

	EndpointSphereBottomRightFaces(m_vSphereGroupsSizes->get(m_iGroupCount - 2));
	InitDrawnObjectSingleTexture();
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
	//OutputDebugString(L"\nEndpointSphereTopLeftFaces Line Points 1: cnt: "); OutputDebugString(std::to_wstring(secondGroupPoints).c_str()); OutputDebugString(L"\n");
	unsigned int m_iPtCount = secondGroupPoints;
	unsigned int i = 0;
	unsigned int j = 0;
		for (j = 0; j < m_iPtCount; j++)
		{
			size_t posI = static_cast<size_t>(i);
			// First Face
			uint16_t a = 0;
			uint16_t b = m_PtGroupList.at(posI +1)->GetListPt(j);
			int n = j;
			if (j == m_iPtCount - 1)
			{
				n = -1;
			}
			uint16_t c = m_PtGroupList.at(posI + 1)->GetListPt(n + 1);
			//OutputDebugString(L"j: "); OutputDebugString(std::to_wstring(j).c_str());
			//OutputDebugString(L", "); OutputDebugString(std::to_wstring(a).c_str());
			//OutputDebugString(L", "); OutputDebugString(std::to_wstring(b).c_str());
			//OutputDebugString(L", "); OutputDebugString(std::to_wstring(c).c_str()); OutputDebugString(L"\n");
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c); faceCnt++; faceIndiceCnt += 3;
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
	//bool g1 = m_iPtCount1 % 2 == 1 ? false : true;
	//bool g2 = m_iPtCount2 % 2 == 1 ? false : true;

	std::vector<unsigned int> numLine1;
	std::vector<unsigned int> numLine2;
	OutputDebugString(L"\nLesserLat Line Points 1: cnt: "); OutputDebugString(std::to_wstring(m_iPtCount1).c_str()); OutputDebugString(L"\n");
	for (j; j < (m_iPtCount1); j++)
	{
		numLine1.push_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));
		OutputDebugString(L", "); OutputDebugString(std::to_wstring(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j)).c_str());
	}
	j = 0;
	numLine1.push_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));
	OutputDebugString(L", "); OutputDebugString(std::to_wstring(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j)).c_str());

	OutputDebugString(L"\nLine Points 2: cnt: "); OutputDebugString(std::to_wstring(m_iPtCount2).c_str()); OutputDebugString(L"\n");
	for (j; j < (m_iPtCount2); j++)
	{
		numLine2.push_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));
		OutputDebugString(L", "); OutputDebugString(std::to_wstring(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j)).c_str());
	}
	j = 0;
	numLine2.push_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));
	OutputDebugString(L", "); OutputDebugString(std::to_wstring(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j)).c_str());
	OutputDebugString(L"\n");
	
	//m_iPtCount1 = static_cast<unsigned int>(numLine1.size());
	//m_iPtCount2 = static_cast<unsigned int>(numLine2.size());
	unsigned int difCnt = 0;
	difCnt = static_cast<unsigned int>(numLine1.size()) - static_cast<unsigned int>(numLine2.size());
	//size_t posI = static_cast<size_t>(i);
	if (difCnt == 0)
	{

	}// eo if (difCnt == 0)
	else
	{
		for (k; k < (m_iPtCount1); k++)
		{
			if (j < (m_iPtCount2))
			{
				a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
				b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
				c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 1));
				d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j) + 1));

				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
				indices.push_back(c);
				indices.push_back(d);
				indices.push_back(a);
				j++;
				if (k + 2 < m_iPtCount2)
				{
					b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 2));
					indices.push_back(d);
					indices.push_back(c);
					indices.push_back(b);

				}
			}
			else
			{
				a = static_cast<uint16_t>(numLine2.at(m_iPtCount2));
				b = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
				c = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(k) + 1));
				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(b);
				j++;
			}
		}
	}// eo else (difCnt != 0)
	numLine1.resize(0);
	numLine2.resize(0);
}

//size_t faceCnt = 0;
//size_t faceIndiceCnt = 0;

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
	//bool g1 = m_iPtCount1 % 2 == 1 ? false : true;
	//bool g2 = m_iPtCount2 % 2 == 1 ? false : true;

	std::vector<unsigned int> numLine1;
	std::vector<unsigned int> numLine2;
	OutputDebugString(L"\nGreaterLat Line Points 1: cnt: "); OutputDebugString(std::to_wstring(m_iPtCount1).c_str()); OutputDebugString(L"\n");

	for (j; j < (m_iPtCount1); j++)
	{
		numLine1.push_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));
		OutputDebugString(L", "); OutputDebugString(std::to_wstring(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j)).c_str());
	}
	j = 0;
	numLine1.push_back(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j));
	OutputDebugString(L", "); OutputDebugString(std::to_wstring(m_PtGroupList.at(static_cast<size_t>(groupNum))->GetListPt(j)).c_str());
	
	OutputDebugString(L"\nLine Points 2: cnt: "); OutputDebugString(std::to_wstring(m_iPtCount2).c_str()); OutputDebugString(L"\n");
	for (j; j < (m_iPtCount2); j++)
	{
		numLine2.push_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));
		OutputDebugString(L", "); OutputDebugString(std::to_wstring(m_PtGroupList.at(static_cast<size_t>(groupNum+1))->GetListPt(j)).c_str());
	}
	j = 0;
	numLine2.push_back(m_PtGroupList.at(static_cast<size_t>(groupNum + 1))->GetListPt(j));
	OutputDebugString(L", "); OutputDebugString(std::to_wstring(m_PtGroupList.at(static_cast<size_t>(groupNum+1))->GetListPt(j)).c_str());
	OutputDebugString(L"\n");
	j = 0;

	unsigned int difCnt = 0;
		difCnt = static_cast<unsigned int>(numLine2.size()) - static_cast<unsigned int>(numLine1.size());
	
	//size_t posI = static_cast<size_t>(i);
	for (k; k < (m_iPtCount2); k++)
	{
		if (j < (m_iPtCount1))
		{
			a = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j)));
			b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k)));
			c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 1));
			d = static_cast<uint16_t>(numLine1.at(static_cast<size_t>(j) + 1));
			
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
				OutputDebugString(L"Face1: ");
				OutputDebugString(std::to_wstring(indices.at(faceIndiceCnt)).c_str());
				OutputDebugString(L", ");
				OutputDebugString(std::to_wstring(indices.at(faceIndiceCnt + 1)).c_str());
				OutputDebugString(L", ");
				OutputDebugString(std::to_wstring(indices.at(faceIndiceCnt + 2)).c_str());
				OutputDebugString(L"\n"); faceCnt++; faceIndiceCnt += 3;
			
			indices.push_back(c);
			indices.push_back(d);
			indices.push_back(a);
			j++;
			OutputDebugString(L"Face2: ");
			OutputDebugString(std::to_wstring(indices.at(faceIndiceCnt)).c_str());
			OutputDebugString(L", ");
			OutputDebugString(std::to_wstring(indices.at(faceIndiceCnt + 1)).c_str());
			OutputDebugString(L", ");
			OutputDebugString(std::to_wstring(indices.at(faceIndiceCnt + 2)).c_str());
			OutputDebugString(L"\n"); faceCnt++; faceIndiceCnt += 3;
			
			if (k + 2 < difCnt)
			{
				b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 2));
				indices.push_back(d);
				indices.push_back(c);
				indices.push_back(b);

			}
		}
		else
		{
			a = static_cast<uint16_t>(numLine1.at(m_iPtCount1));
			b = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(j)));
			c = static_cast<uint16_t>(numLine2.at(static_cast<size_t>(k) + 1));
			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(b);
			j++;
		}
	}
	numLine1.resize(0);
	numLine2.resize(0);
}

void XM_CALLCONV Hot3dxRotoDraw::RotoDrawSceneRender::EndpointSphereBottomRightFaces(unsigned int secondFromEndGroupPoints)
{
	unsigned int m_iPtCount = secondFromEndGroupPoints;
	unsigned int i = m_iPointCount-1;
	unsigned int j = 0;
	for (j = 0; j < m_iPtCount; j++)
	{
		size_t posI = static_cast<size_t>(m_iGroupCount-1);
		// First Face
		uint16_t a = static_cast<uint16_t>(i);
		uint16_t b = m_PtGroupList.at(posI - 1)->GetListPt(j);
		int n = j;
		if (j == m_iPtCount - 1)
		{
			n = -1;
		}
		uint16_t c = m_PtGroupList.at(posI - 1)->GetListPt(n + 1);
		
			indices.push_back(b);
			indices.push_back(a);
			indices.push_back(c);
	}
}