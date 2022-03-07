// CNavigationTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "MainFrm.h"
#include "Form.h"
#include "MapTool.h"
#include "NavigationTool.h"
#include "afxdialogex.h"
#include "ToolMouse.h"
#include "ToolTerrain.h"
#include "ToolCell.h"
#include "ToolPoint.h"


// CNavigationTool 대화 상자

IMPLEMENT_DYNAMIC(CNavigationTool, CDialog)

CNavigationTool::CNavigationTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CNavigationTool, pParent)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pMouse(nullptr)
	, m_bModeCheck(false)
	, m_iSelectCell(-1)
	,m_iMouseSelect(-1)
	, m_iDragCell(-1)
	, m_bPointClick(false)
	, m_pStaticObjects(nullptr)
{
	ZeroMemory(&m_vPosition, sizeof(_float3));
}

CNavigationTool::CNavigationTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse, CWnd* pParent)
	: CDialog(IDD_CNavigationTool, pParent)
	, m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_pMouse(pMouse)
	, m_bModeCheck(false)
	, m_iSelectCell(-1)
	, m_iMouseSelect(-1)
	, m_iDragCell(-1)
	, m_bPointClick(false)
	, m_pStaticObjects(nullptr)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pMouse);
	ZeroMemory(&m_vPosition, sizeof(_float3));
}

CNavigationTool::~CNavigationTool()
{
	Release();
}

void CNavigationTool::Release()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pMouse);
	Safe_Release(m_pTerrain);

	for_each(m_vecToolCell.begin(), m_vecToolCell.end(), CDeleteObj());
	m_vecToolCell.clear();
	m_vecToolCell.shrink_to_fit();

	for_each(m_vecTmpPoints.begin(), m_vecTmpPoints.end(), CDeleteObj());
	m_vecTmpPoints.clear();
	m_vecTmpPoints.shrink_to_fit();
}

_int CNavigationTool::Update_NavigationTool(_float fDeltaTime)
{
	
	if (!m_pTerrain)
	{
		CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
		CMapTool* pMapTool = static_cast<CMapTool*>(pMainFrm->getForm()->getTools(CForm::TOOLS::TOOL_MAP));
		m_pTerrain = pMapTool->getTerrain();
		m_pStaticObjects = pMapTool->getStaticObjectVector();
		Safe_AddRef(m_pTerrain);
	}
	if (NullEditCheck())
		UpdateData(true);

	Update_PositionValue();

	m_pTerrain->setUsingNaviTool(true);
	CheckMode();

	if (FAILED(Click_Mouse()))
		return -1;

	for (auto& pPoint : m_vecTmpPoints)
		pPoint->Update(fDeltaTime);


	m_pTerrain->Update_Terrain(fDeltaTime);
	for (auto& pStatic : *m_pStaticObjects)
		pStatic->Update_GameObject(fDeltaTime);

	return 0;
}

HRESULT CNavigationTool::Render_NavigationTool()
{
	for (auto& pPoint : m_vecTmpPoints)
		pPoint->Render();
	return S_OK;
}

HRESULT CNavigationTool::Ready_CellTree()
{
	CString str = L"Cell";
	HTREEITEM tRoot = m_tNaviMeshTree.InsertItem(str, nullptr, nullptr);
	return S_OK;
}

_bool CNavigationTool::NullEditCheck()
{
	for (_int i = 0; i < 3; i++)
	{
		if (!m_tPosition[i].GetWindowTextLengthW())
			return false;
		else
		{
			if (!Check_MinusSymbol(m_tPosition[i]))
				return false;
		}
	}
	return true;
}

void CNavigationTool::CheckMode()
{
	if (m_tCreateMode.GetCheck())
	{
		m_bModeCheck = false;
		m_tModifyMode.SetCheck(false);
	}
	if (m_tModifyMode.GetCheck())
	{
		m_bModeCheck = true;
		m_tCreateMode.SetCheck(false);
	}
}

HRESULT CNavigationTool::Click_Mouse()
{
	_vector vPos = XMVectorZero();
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	if (pInstance->GetMouseState(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		if (!m_bClick)
		{
			m_bClick = true;
			_int iIndex = 0;
			vPos = m_pMouse->Terrain_Picking(m_pTerrain->getVertices(), m_pTerrain->getWorldMatrx(),
				m_pTerrain->getVtxX(), m_pTerrain->getVtxZ(), iIndex);
			if (!m_bModeCheck)//Create모드
			{
				if (!XMVector3Equal(vPos, XMVectorZero()))
				{
					if (FAILED(Create_Point(vPos)))
					{
						RELEASE_INSTANCE(CGameInstance);
						return E_FAIL;
					}
				}
			}
			else
				Check_SamePointClick();
		}
	}
	else
	{
		m_bPointClick = false;
		m_bClick = false;
	}
	if (m_bModeCheck)
	{
		if (m_bPointClick)
		{
			_long MouseMove = 0;
			if (MouseMove = pInstance->GetMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y))
			{
				_vector vPos = XMLoadFloat3(&m_vPosition);
				_float fCurrentY = XMVectorGetY(vPos);
				fCurrentY -= MouseMove*0.1f;
				vPos=XMVectorSetY(vPos, fCurrentY);
				XMStoreFloat3(&m_vPosition, vPos);
				m_vecTmpPoints[m_iDragCell]->Change_Pos(vPos);
				m_pTerrain->Change_CellPos(m_iDragCell, vPos);
				UpdateData(false);
			}
		}
		else
			m_iDragCell = -1;
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CNavigationTool::Create_Point(_fvector vPos)
{
	if (!Check_SamePointClick())
	{
		CToolPoint* pPoint = CToolPoint::Create(m_pDevice, m_pDeviceContext, vPos);
		if (!pPoint)
			return E_FAIL;

		m_vecTmpPoints.emplace_back(pPoint);
	}
	if (m_vecTmpPoints.size()%3 == 0)
	{
		//소팅 후 셀로 저장
		if (FAILED(Create_Cell()))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CNavigationTool::Create_Cell()
{
	CToolPoint* tmpCell[3];
	ZeroMemory(tmpCell, sizeof(CToolPoint*) * 3);

	_uint iIndex = m_vecToolCell.size()*3;
	tmpCell[0] = m_vecTmpPoints[iIndex];

	_vector StandardPos = tmpCell[0]->getPos();
	_vector Point1 = m_vecTmpPoints[iIndex+1]->getPos();
	_vector Point2 = m_vecTmpPoints[iIndex+2]->getPos();

	_uint iWidth1, iWidth2, iWidth3;// 0: 기준이 제일 작을때 1: 기준이 중간 2: 기준이 제일 클때
	
	_float fMax = max(XMVectorGetX(StandardPos), XMVectorGetX(Point1));
	fMax = max(fMax, XMVectorGetX(Point2));

	if (fMax == XMVectorGetX(StandardPos))
	{
		iWidth1 = 0;
		if (XMVectorGetX(Point1) < XMVectorGetX(Point2))
		{
			iWidth2 = 2;
			iWidth3 = 1;
		}
		else
		{
			iWidth2 = 1;
			iWidth3 = 2;
		}
	}
	else if (fMax == XMVectorGetX(Point1))
	{
		iWidth2 = 0;
		if (XMVectorGetX(StandardPos) < XMVectorGetX(Point2))
		{
			iWidth1 = 2;
			iWidth3 = 1;
		}
		else
		{
			iWidth1 = 1;
			iWidth3 = 2;
		}
	}
	else
	{
		iWidth3 = 0;
		if (XMVectorGetX(StandardPos) < XMVectorGetX(Point1))
		{
			iWidth1 = 2;
			iWidth2 = 1;
		}
		else
		{
			iWidth1 = 1;
			iWidth2 = 2;
		}
	}
	_uint iHeight1, iHeight2, iHeight3;// 0: 기준이 제일 작을때 1: 기준이 중간 2: 기준이 제일 클때
	fMax = max(XMVectorGetZ(StandardPos), XMVectorGetZ(Point1));
	fMax = max(fMax, XMVectorGetZ(Point2));

	if (fMax == XMVectorGetZ(StandardPos))
	{
		iHeight1 = 0;
		if (XMVectorGetZ(Point1) < XMVectorGetZ(Point2))
		{
			iHeight2 = 2;
			iHeight3 = 1;
		}
		else
		{
			iHeight2 = 1;
			iHeight3 = 2;
		}
	}
	else if (fMax == XMVectorGetZ(Point1))
	{
		iHeight2 = 0;
		if (XMVectorGetZ(StandardPos) < XMVectorGetZ(Point2))
		{
			iHeight1 = 2;
			iHeight3 = 1;
		}
		else
		{
			iHeight1 = 1;
			iHeight3 = 2;
		}
	}
	else
	{
		iHeight3 = 0;
		if (XMVectorGetZ(StandardPos) < XMVectorGetZ(Point1))
		{
			iHeight1 = 2;
			iHeight2 = 1;
		}
		else
		{
			iHeight1 = 1;
			iHeight2 = 2;
		}
	}
	
	switch (iWidth1)
	{
	case 0:
		if (iHeight1 == 0)
		{
			if (iWidth2 == 1 && iHeight2 == 1)
			{
				_vector vDir = StandardPos-Point2;
				vDir=XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(Point1-StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
				
			}
			else if (iWidth2 == 1 && iHeight2 == 2)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
			else if (iWidth2 == 2 && iHeight2 == 1)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
			else if (iWidth2 == 2 && iHeight2 == 2)
			{
				_vector vDir = StandardPos-Point1;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(Point2-StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
			}
		}
		else if (iHeight1 == 1)
		{
			if (iWidth2 == 1 && iHeight2 == 0)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
			if (iWidth2 == 1 && iHeight2 == 2)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
			if (iWidth2 == 2 && iHeight2 == 0)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
			if (iWidth2 == 2 && iHeight2 == 2)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
		}
		else
		{
			if (iWidth2 == 1 && iHeight2 == 0)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
			if (iWidth2 == 1 && iHeight2 == 1)
			{
				_vector vDir = Point2-StandardPos;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(Point1-StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
			}
			if (iWidth2 == 2 && iHeight2 == 0)
			{
				_vector vDir = Point1-StandardPos;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(StandardPos - Point2);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex +1];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
			}
			if (iWidth2 == 2 && iHeight2 == 1)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
		}
		break;
	case 1:
		if (iHeight1 == 0)
		{
			if (iWidth2 == 0 && iHeight2 == 1)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
			else if (iWidth2 == 0 && iHeight2 == 2)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
			else if (iWidth2 == 2 && iHeight2 == 1)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
			else if (iWidth2 == 2 && iHeight2 == 2)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
		}
		else if (iHeight1 == 1)
		{
			if (iWidth2 == 0 && iHeight2 == 0)
			{
				_vector vDir = Point2-Point1;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(Point2 - StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
			}
			else if (iWidth2 == 0 && iHeight2 == 2)
			{
				_vector vDir = Point2 - Point1;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize( Point2 - StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
			}
			else if (iWidth2 == 2 && iHeight2 == 0)
			{
				_vector vDir = Point1-Point2;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(Point2- StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
			}
			else if (iWidth2 == 2 && iHeight2 == 2)
			{
				_vector vDir = Point2 - Point1;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(StandardPos - Point2);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
			} 
		}
		else
		{
			if (iWidth2 == 0 && iHeight2 == 0)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
			else if (iWidth2 == 0 && iHeight2 == 1)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
			else if (iWidth2 == 2 && iHeight2 == 0)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
			else if (iWidth2 == 2 && iHeight2 == 1)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
		}
		break;
	case 2:
		if (iHeight1 == 0)
		{
			if (iWidth2 == 0 && iHeight2 == 1)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
			else if (iWidth2 == 0 && iHeight2 == 2)
			{
				_vector vDir = Point1 - StandardPos;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(Point2 - StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
			}
			else if (iWidth2 == 1 && iHeight2 == 1)
			{
				_vector vDir = Point2 - StandardPos;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(Point1 - StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
			}
			else if (iWidth2 == 1 && iHeight2 == 2)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
		}
		else if (iHeight1 == 1)
		{
			if (iWidth2 == 0 && iHeight2 == 0)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
			else if (iWidth2 == 0 && iHeight2 == 2)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
			else if (iWidth2 == 1 && iHeight2 == 0)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
			else if (iWidth3 == 1 && iHeight3 == 2)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
		}
		else
		{
			if (iWidth2 == 0 && iHeight2 == 0)
			{
				_vector vDir = Point1 - StandardPos;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(Point2 - StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
			}
			else if (iWidth2 == 0 && iHeight2 == 1)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 2];
				tmpCell[2] = m_vecTmpPoints[iIndex + 1];
			}
			else if (iWidth2 == 1 && iHeight2 == 0)
			{
				tmpCell[1] = m_vecTmpPoints[iIndex + 1];
				tmpCell[2] = m_vecTmpPoints[iIndex + 2];
			}
			else if (iWidth2 == 1 && iHeight2 == 1)
			{
				_vector vDir = Point2 - StandardPos;
				vDir = XMVector3Normalize(vDir);
				_vector vNorm = XMVectorSet(-XMVectorGetZ(vDir), XMVectorGetY(vDir), XMVectorGetX(vDir), 0.f);
				_vector vTmpDir = XMVector3Normalize(Point1-StandardPos);

				_float fDot = XMVectorGetX(XMVector3Dot(vNorm, vTmpDir));
				if (fDot < 0)
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 2];
					tmpCell[2] = m_vecTmpPoints[iIndex + 1];
				}
				else
				{
					tmpCell[1] = m_vecTmpPoints[iIndex + 1];
					tmpCell[2] = m_vecTmpPoints[iIndex + 2];
				}
			}
		}
		break;
	}




	m_vecTmpPoints[iIndex] = tmpCell[0];
	m_vecTmpPoints[iIndex+1] = tmpCell[1];
	m_vecTmpPoints[iIndex+2] = tmpCell[2];

	CToolCell* pCell = CToolCell::Create(m_pDevice, m_pDeviceContext, tmpCell);

	if (!pCell)
		return E_FAIL;

	m_vecToolCell.emplace_back(pCell);
	m_pTerrain->Add_Cell(pCell);

	HTREEITEM tRoot = m_tNaviMeshTree.GetRootItem();
	CString str=L"%d";
	str.Format(str.GetString(), m_vecToolCell.size()-1);
	HTREEITEM tParent = m_tNaviMeshTree.InsertItem(str,tRoot,nullptr);

	for (_int i = 0; i < 3; i++)
	{
		CString tmp = L"%d";
		tmp.Format(tmp.GetString(), i);
		m_tNaviMeshTree.InsertItem(tmp, tParent, nullptr);
	}

	return S_OK;
}

_bool CNavigationTool::Check_SamePointClick()
{
	if (m_vecTmpPoints.empty())
		return false;

	if (m_bModeCheck)
	{
		for (auto& pPoint : m_vecTmpPoints)
			pPoint->Reset_Point();
	}
	for(_uint i=0; i<(_uint)m_vecTmpPoints.size(); i++)
	{
		if (m_vecTmpPoints[i]->Collision_Point(m_pMouse->getRayPos(), m_pMouse->getRayDir()))
		{
			if (!m_bModeCheck)
				m_vecTmpPoints.emplace_back(m_vecTmpPoints[i]->Clone());
			else
			{
				m_bPointClick = true;
				m_iMouseSelect = i;
				m_iDragCell = i;
				CellTreeClick(i);
			}
			return true;
		}
	}
	
	return false;
}

void CNavigationTool::Update_PositionValue()
{
	HTREEITEM tSelectItem = m_tNaviMeshTree.GetSelectedItem();
	_int iIndex = getIndex(m_tNaviMeshTree, tSelectItem);
	if (iIndex < 0)
		return;
	if (m_iSelectCell != iIndex || m_iMouseSelect != -1)
	{
		m_iMouseSelect = -1;
		for (auto& pPoint : m_vecTmpPoints)
			pPoint->Reset_Point();
		m_iSelectCell = iIndex;
		m_vecTmpPoints[iIndex]->Click_Point();
		_vector vPos = m_vecTmpPoints[iIndex]->getPos();
		XMStoreFloat3(&m_vPosition, vPos);
		UpdateData(false);
	}
	m_vecTmpPoints[iIndex]->Change_Pos(XMLoadFloat3(&m_vPosition));
	m_pTerrain->Change_CellPos(iIndex, XMLoadFloat3(&m_vPosition));
}

void CNavigationTool::CellTreeClick(_uint iIndex)
{
	_int ParentIndex = iIndex / 3;
	_int CellIndex = iIndex % 3;

	HTREEITEM tRoot = m_tNaviMeshTree.GetRootItem();
	HTREEITEM tParent = m_tNaviMeshTree.GetChildItem(tRoot);
	for (_int i = 0; i < ParentIndex; i++)
		tParent = m_tNaviMeshTree.GetNextSiblingItem(tParent);

	HTREEITEM tCell = m_tNaviMeshTree.GetChildItem(tParent);

	for (_int i = 0; i < CellIndex; i++)
		tCell = m_tNaviMeshTree.GetNextSiblingItem(tCell);

	m_tNaviMeshTree.SelectItem(tCell);
	m_tNaviMeshTree.SelectDropTarget(tCell);
}



void CNavigationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_tPosition[0]);
	DDX_Control(pDX, IDC_EDIT2, m_tPosition[1]);
	DDX_Control(pDX, IDC_EDIT3, m_tPosition[2]);
	DDX_Text(pDX, IDC_EDIT1, m_vPosition.x);
	DDX_Text(pDX, IDC_EDIT2, m_vPosition.y);
	DDX_Text(pDX, IDC_EDIT3, m_vPosition.z);
	DDX_Control(pDX, IDC_TREE1, m_tNaviMeshTree);
	DDX_Control(pDX, IDC_RADIO4, m_tCreateMode);
	DDX_Control(pDX, IDC_RADIO1, m_tModifyMode);
}


BEGIN_MESSAGE_MAP(CNavigationTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CNavigationTool::OnCellDeleteBtnClick)
	ON_BN_CLICKED(IDC_BUTTON1, &CNavigationTool::OnNavimeshSaveBtnClick)
	ON_BN_CLICKED(IDC_BUTTON12, &CNavigationTool::OnNaviMeshLoadBtnClick)
END_MESSAGE_MAP()


// CNavigationTool 메시지 처리기


BOOL CNavigationTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if(FAILED(Ready_CellTree()))
		return false;

	if (!m_bModeCheck)
	{
		m_tCreateMode.SetCheck(true);
		m_tModifyMode.SetCheck(false);
	}
	else
	{
		m_tCreateMode.SetCheck(true);
		m_tModifyMode.SetCheck(false);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNavigationTool::OnCellDeleteBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM SelItem = m_tNaviMeshTree.GetSelectedItem();
	_int iPointIndex = getIndex(m_tNaviMeshTree,SelItem);
	if (iPointIndex < 0)
		return;
	_uint iCellIndex = iPointIndex / 3;
	Safe_Release(m_vecToolCell[iCellIndex]);
	auto Celliter = m_vecToolCell.begin();
	m_vecToolCell.erase(Celliter + iCellIndex);

	if (FAILED(m_pTerrain->Delete_Cell(iCellIndex)))
		return;

	Safe_Release(m_vecTmpPoints[iCellIndex* 3]);
	Safe_Release(m_vecTmpPoints[iCellIndex*3 +1]);
	Safe_Release(m_vecTmpPoints[iCellIndex*3 +2]);

	auto Pointsiter = m_vecTmpPoints.begin();
	
	if (iCellIndex == 0 && m_vecTmpPoints.size() < 3)
	{
		m_vecTmpPoints.clear();
	}
	else
	{
		m_vecTmpPoints.erase(Pointsiter + iCellIndex*3 + 2);
		m_vecTmpPoints.erase(Pointsiter + iCellIndex*3 + 1);
		m_vecTmpPoints.erase(Pointsiter + iCellIndex*3);
	}
	
	HTREEITEM tCell = m_tNaviMeshTree.GetChildItem(m_tNaviMeshTree.GetRootItem());

	for(_uint i=0; i<iCellIndex; i++)
		tCell = m_tNaviMeshTree.GetNextSiblingItem(tCell);

	m_tNaviMeshTree.DeleteItem(tCell);
}


void CNavigationTool::OnNavimeshSaveBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(false, L"NaviMesh", L"*.NaviMesh"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.NaviMesh", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	vector<CELLDATA> vecSave;
	for (auto& pCell : m_vecToolCell)
	{
		CELLDATA tSaveData;
		ZeroMemory(&tSaveData, sizeof(CELLDATA));
		XMStoreFloat3(&tSaveData.p1Position, pCell->getPos(0));
		XMStoreFloat3(&tSaveData.p2Position, pCell->getPos(1));
		XMStoreFloat3(&tSaveData.p3Position, pCell->getPos(2));
		vecSave.emplace_back(tSaveData);
	}
	if (FAILED(pInstance->SaveFile<CELLDATA>(&vecSave, Dlg.GetPathName())))
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	RELEASE_INSTANCE(CGameInstance);
}


void CNavigationTool::OnNaviMeshLoadBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(true, L"NaviMesh", L"*.NaviMesh"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.NaviMesh", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	vector<CELLDATA> vecData;
	if (FAILED(pInstance->LoadFile<CELLDATA>(vecData, Dlg.GetPathName())))
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	for (auto& pData : vecData)
	{
		if (FAILED(Create_Point(XMLoadFloat3(&pData.p1Position))))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		if (FAILED(Create_Point(XMLoadFloat3(&pData.p2Position))))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		if (FAILED(Create_Point(XMLoadFloat3(&pData.p3Position))))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}
