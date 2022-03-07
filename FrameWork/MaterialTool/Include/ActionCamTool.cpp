// ActionCamTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "ActionCamTool.h"
#include "afxdialogex.h"
#include "ToolMouse.h"


// CActionCamTool 대화 상자

IMPLEMENT_DYNAMIC(CActionCamTool, CDialog)

CActionCamTool::CActionCamTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CActionCamTool, pParent)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_fSpeed(0)
{
	ZeroMemory(&m_vPos, sizeof(_float3));
}

CActionCamTool::CActionCamTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse, CWnd* pParent)
	: CDialog(IDD_CActionCamTool, pParent)
	,m_pDevice(pDevice)
	,m_pDeviceContext(pDeviceContext)
	,m_pMouse(pMouse)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pMouse);
	ZeroMemory(&m_vPos, sizeof(_float3));
}

CActionCamTool::~CActionCamTool()
{
	Release();
}

void CActionCamTool::Release()
{
	for (auto& pCameraInfo : m_vecCameraInfo)
		Safe_Delete(pCameraInfo);
	m_vecCameraInfo.clear();
	m_vecCameraInfo.shrink_to_fit();
	
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pMouse);
}

_bool CActionCamTool::NullEditCheck()
{
	for (_int i = 0; i < 3; i++)
	{
		if (!m_tPos[i].GetWindowTextLengthW())
			return false;
		else
		{
			if (!Check_MinusSymbol(m_tPos[i]))
				return false;
		}
	}
	if (m_tSpeed.GetWindowTextLengthW())
		return false;
	else
	{
		if (!Check_MinusSymbol(m_tSpeed))
			return false;
	}

	return true;
}

HRESULT CActionCamTool::Init_CameraInfo(_int iIndex)
{
	if (iIndex < 0)
		return E_FAIL;
	HTREEITEM tParent = getParent(iIndex);
	if (!tParent)
		return E_FAIL;
	CAMERAINFO* pInfo = new CAMERAINFO;
	m_vecCameraInfo.emplace_back(pInfo);
	m_tCameraTree.InsertItem(_T("Pos"), tParent, nullptr);
	m_tCameraTree.InsertItem(_T("Target"), tParent, nullptr);
	return S_OK;
}

HTREEITEM CActionCamTool::getParent(_int iIndex)
{
	HTREEITEM tRoot=m_tCameraTree.GetRootItem();
	CString str;
	str.Format(L"%d", iIndex);
	while (tRoot)
	{
		if (str == m_tCameraTree.GetItemText(tRoot))
			break;
		tRoot = m_tCameraTree.GetNextItem(tRoot, TVGN_NEXT);
	}
	return tRoot;
}

_int CActionCamTool::Update_ActionCamTool(_float fDeltaTime)
{
	if (NullEditCheck())
		UpdateData(true);
	return _int();
}

HRESULT CActionCamTool::Render_ActionCamTool()
{
	return S_OK;
}

void CActionCamTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tCameraTree);
	DDX_Control(pDX, IDC_EDIT1, m_tPos[0]);
	DDX_Control(pDX, IDC_EDIT2, m_tPos[1]);
	DDX_Control(pDX, IDC_EDIT3, m_tPos[2]);
	DDX_Control(pDX, IDC_EDIT10, m_tSpeed);
	DDX_Text(pDX, IDC_EDIT1, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT2, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT3, m_vPos.z);
	DDX_Text(pDX, IDC_EDIT10, m_fSpeed);
}


BEGIN_MESSAGE_MAP(CActionCamTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CActionCamTool::OnCameraCreateBtn)
	ON_BN_CLICKED(IDC_BUTTON16, &CActionCamTool::OnCameraDeleteBtn)
END_MESSAGE_MAP()


// CActionCamTool 메시지 처리기



BOOL CActionCamTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//카메라 생성

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CActionCamTool::OnCameraCreateBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*if (!m_pSampleCamera)
		m_pSampleCamera = CCamera::Create(m_pDevice, m_pDeviceContext);*/
	_int iIndex = m_vecCameraInfo.size();
	CString str;
	str.Format(L"%d", iIndex);
	m_tCameraTree.InsertItem(str, nullptr, nullptr);
	if (FAILED(Init_CameraInfo(iIndex)))
		return;
}


void CActionCamTool::OnCameraDeleteBtn()
{
	/*if (!m_tCameraTree.GetCount())
		Safe_Release(m_pSampleCamera);*/
	HTREEITEM tParent = m_tCameraTree.GetSelectedItem();
	if (!tParent)
		return;
	CString tParentIndex = m_tCameraTree.GetItemText(tParent);

	_int iIndex = stoi(tParentIndex.GetString(), nullptr, 10);

	Safe_Delete(m_vecCameraInfo[iIndex]);
	auto iter = m_vecCameraInfo.begin();

	m_vecCameraInfo.erase(iter + iIndex);
	HTREEITEM tParentNext = m_tCameraTree.GetNextItem(tParent, TVGN_NEXT);
	m_tCameraTree.DeleteItem(tParent);

	while (tParentNext)
	{
		m_tCameraTree.SetItemText(tParentNext, tParentIndex);
		tParentIndex.Format(L"%d", iIndex += 1);
		tParentNext = m_tCameraTree.GetNextItem(tParentNext, TVGN_NEXT);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
