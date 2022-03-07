// UITool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "UITool.h"
#include "afxdialogex.h"
#include "ToolCamera.h"
#include "ToolMouse.h"

// CUITool 대화 상자

IMPLEMENT_DYNAMIC(CUITool, CDialog)

CUITool::CUITool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CUITool, pParent)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	,m_pOthoGraphiCam(nullptr)
{
	ZeroMemory(&m_vPos, sizeof(_float2));
	ZeroMemory(&m_vScale, sizeof(_float2));
}

CUITool::CUITool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse, CWnd* pParent)
	: CDialog(IDD_CUITool, pParent)
	, m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_pOthoGraphiCam(nullptr)
	, m_pMouse(pMouse)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pMouse);
	ZeroMemory(&m_vPos, sizeof(_float2));
	ZeroMemory(&m_vScale, sizeof(_float2));
}

CUITool::~CUITool()
{
	Release();
}

void CUITool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_tScale[0]);
	DDX_Control(pDX, IDC_EDIT2, m_tScale[1]);
	DDX_Control(pDX, IDC_EDIT10, m_tPos[0]);
	DDX_Control(pDX, IDC_EDIT11, m_tPos[1]);

	DDX_Text(pDX, IDC_EDIT1, m_vScale.x);
	DDX_Text(pDX, IDC_EDIT2, m_vScale.y);
	DDX_Text(pDX, IDC_EDIT10, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT11, m_vPos.y);
	DDX_Control(pDX, IDC_LIST1, m_tTextureList);
	DDX_Control(pDX, IDC_PICTURE, m_tPictureCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_tUITagComboBox);
	DDX_Control(pDX, IDC_LIST2, m_tUIList);
}

_int CUITool::Update_UITool(_float fDeltaTime)
{
	if (NullEditCheck())
		UpdateData(true);
	
	m_pOthoGraphiCam->Update_GameObject(fDeltaTime);
	_int iIndex = -1;
	iIndex = m_tTextureList.GetCurSel();
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	_vector vMouse = m_pMouse->UI_Picking();
	if (!XMVector3Equal(vMouse,XMVectorZero()))
	{
		_uint iClickIndex = -1;
		_bool bClicked = false;
		for (auto& pUI : m_vecUI)
		{
			iClickIndex++;
			if (pUI->Click_UI(vMouse))
			{
				_uint iUITag = -1;
				pUI->setClick(true);
				pUI->Change_ToolValue(&m_vScale, &m_vPos,&iUITag);
				m_tUIList.SetCurSel(iClickIndex);
				m_tUITagComboBox.SetCurSel(iUITag);
				bClicked = true;
				UpdateData(false);
				continue;
			}
			else
				pUI->setClick(false);
		}
		if (!bClicked)
		{
			m_tUIList.SetCurSel(-1);
			m_tUITagComboBox.SetCurSel(-1);
		}
	}
	else
	{
		_uint iSelIndex = -1;
		iSelIndex = m_tUIList.GetCurSel();
		if (iSelIndex != -1)
		{
			if (!m_vecUI[iSelIndex]->getClick())
			{
				for (auto& pUI : m_vecUI)
					pUI->setClick(false);

				m_vecUI[iSelIndex]->setClick(true);
				_uint iTagIndex = -1;
				m_vecUI[iSelIndex]->Change_ToolValue(&m_vScale, &m_vPos, &iTagIndex);
				m_tUITagComboBox.SetCurSel(iTagIndex);
				UpdateData(false);
			}
		}
	}
	if (iIndex > -1)
	{
		CString str;
		m_tTextureList.GetText(iIndex, str);
		if (FAILED(SetUITexture(str)))
			return -1;
	}

	if (!m_vecUI.empty())
	{
		_vector vScale = XMVectorSet(m_vScale.x, m_vScale.y, 0.f,0.f);
		_vector vPosition = XMVectorSet(m_vPos.x, m_vPos.y, 0.f, 1.f);
		for (auto& pUI : m_vecUI)
		{
			pUI->Update_GameObject(fDeltaTime);
			if (pUI->getClick())
			{
				pUI->Change_Tranform(vScale, vPosition);
				pUI->setTag((CToolUI::UITAG)m_tUITagComboBox.GetCurSel());
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

HRESULT CUITool::Render_UITool()
{
	return S_OK;
}

_bool CUITool::NullEditCheck()
{
	for (_int i = 0; i < 2; i++)
	{
		if (!m_tScale[i].GetWindowTextLengthW())
			return false;
		else
		{
			if (!Check_MinusSymbol(m_tScale[i]))
				return false;
		}
		if (!m_tPos[i].GetWindowTextLengthW())
			return false;
		else
		{
			if (!Check_MinusSymbol(m_tPos[i]))
				return false;
		}
	}
	return true;
}

void CUITool::ClassyfyUITag(CToolUI::UITAG eTag, CString& pStr)
{
	switch (eTag)
	{
	case CToolUI::UITAG::UI_BUTTON:
		pStr = L"Button";
		break;
	case CToolUI::UITAG::UI_HPBAR:
		pStr = L"HpBar";
		break;
	case CToolUI::UITAG::UI_PRINT:
		pStr = L"Print";
		break;
	case CToolUI::UITAG::UI_LIFE:
		pStr = L"Life";
		break;
	default:
		break;
	}
}

HRESULT CUITool::Ready_UITexture()
{
	CFileFind finder;

	CString ForderPath = L"../../Client/bin/Resources/Texture/UI/*.*";
	_bool bFind = finder.FindFile(ForderPath);
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	while (bFind)
	{
		bFind = finder.FindNextFile();
		CString FilePath = finder.GetFilePath();
		if (finder.IsDots())
			continue;
		if (FAILED(pInstance->Add_Texture(m_pDevice, finder.GetFileName(), FilePath)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}
		m_tTextureList.AddString(finder.GetFileName());
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUITool::SetUITexture(CString pStr)
{
	if (pStr.IsEmpty())
		return E_FAIL;

	CString FilePath = L"../../Client/bin/Resources/Texture/UI/";
	FilePath += pStr;

	CRect rect;

	m_tPictureCtrl.GetWindowRect(rect);
	CDC* dc = nullptr;
	dc = m_tPictureCtrl.GetDC();
	CImage image;
	if (FAILED(image.Load(FilePath)))
		return E_FAIL;

	image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	ReleaseDC(dc);

	return S_OK;
}

void CUITool::ResetIndex(CGameObject::COMINDEX* pIndex)
{
	for (_uint i = 0; i < (_uint)COMPONENTID::COMPONENT_END; i++)
	{
		pIndex[i].iStartIndex = -1;
		pIndex[i].iAccumIndex = 0;
	}
}



void CUITool::Release()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pMouse);
	for_each(m_vecUI.begin(), m_vecUI.end(), CDeleteObj());
	m_vecUI.clear();
	m_vecUI.shrink_to_fit();
	Safe_Release(m_pOthoGraphiCam);
}


BEGIN_MESSAGE_MAP(CUITool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUITool::OnUIAddBtnClick)
	ON_BN_CLICKED(IDC_BUTTON11, &CUITool::OnUIDeleteBtnClick)
	ON_BN_CLICKED(IDC_BUTTON12, &CUITool::OnUISaveBtnClick)
	ON_BN_CLICKED(IDC_BUTTON13, &CUITool::OnUILoadBtnClick)
END_MESSAGE_MAP()


// CUITool 메시지 처리기


BOOL CUITool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	for (_uint i = 0; i < (_uint)CToolUI::UITAG::UI_END; i++)
	{
		CString str;
		ClassyfyUITag((CToolUI::UITAG)i, str);
		m_tUITagComboBox.InsertString(i, str);
	}
	if (FAILED(Ready_UITexture()))
		return false;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CGameObject::COMINDEX* pIndex = new CGameObject::COMINDEX[(_uint)COMPONENTID::COMPONENT_END];

	_uint iIndex = 0;

	CCamera::CAMERADESC pDesc;
	ZeroMemory(&pDesc, sizeof(CCamera::CAMERADESC));
	lstrcpy(pDesc.pCameraTag, L"SubCamera");
	pDesc.eType = CCamera::CAMERATYPE::CAMERA_ORTHO;;
	pDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	pDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	pDesc.vAxisY = _float4(0.f, 1.f, 0.f, 1.f);

	pDesc.fWinCX = g_iToolCnX;
	pDesc.fWinCY = g_iToolCnY;
	pDesc.fFar = 1.f;
	pDesc.fNear = 0.f;

	pInstance->Init_ComDeForm((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::CAMERA, &iIndex, &pDesc);
	pIndex[(_uint)COMPONENTID::CAMERA].iStartIndex = iIndex;

	m_pOthoGraphiCam = static_cast<CToolCamera*>(pInstance->Clone_GameObject((_uint)TOOLOBJID::TOOL_CAMERA, pIndex));

	ResetIndex(pIndex);

	if (FAILED(pInstance->Init_GameObject((_uint)TOOLOBJID::TOOL_UI, CToolUI::Create(m_pDevice, m_pDeviceContext, pIndex))))
		return false;

	Safe_Delete_Arr(pIndex);
	RELEASE_INSTANCE(CGameInstance);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUITool::OnUIAddBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_int iUIIndex = -1;
	iUIIndex = m_tTextureList.GetCurSel();
	if (iUIIndex < 0)
	{
		MSG_BOX(L"UI텍스쳐를 선택해주세요");
		return;
	}

	_int iTagIndex = -1;
	iTagIndex = m_tUITagComboBox.GetCurSel();
	if (iTagIndex < 0)
	{
		MSG_BOX(L"UITag를 선택해주세요");
		return;
	}

	CGameObject::COMINDEX* pIndex = new CGameObject::COMINDEX[(_uint)COMPONENTID::COMPONENT_END];

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	_uint iComIndex = 0;
	pInstance->Init_ComDeForm((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::VIBUFFER_RECT, &iComIndex);
	pIndex[(_uint)COMPONENTID::VIBUFFER_RECT].iStartIndex = iComIndex;

	_tchar strTexturTag[MAX_PATH];
	m_tTextureList.GetText(iUIIndex, strTexturTag);
	pInstance->Init_ComDeForm((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::TEXTURE, &iComIndex,&strTexturTag);
	pIndex[(_uint)COMPONENTID::TEXTURE].iStartIndex = iComIndex;

	CToolUI* pObj = static_cast<CToolUI*>(pInstance->Clone_GameObject((_uint)TOOLOBJID::TOOL_UI, pIndex));
	pObj->setTag((CToolUI::UITAG)iTagIndex);
	m_vecUI.emplace_back(pObj);

	m_tUIList.AddString(strTexturTag);

	Safe_Delete_Arr(pIndex);
	RELEASE_INSTANCE(CGameInstance);
}


void CUITool::OnUIDeleteBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	_int iIndex = -1;
	iIndex = m_tUIList.GetCurSel();
	if (iIndex < 0)
		return;

	auto iter = m_vecUI.begin();
	Safe_Release(m_vecUI[iIndex]);

	m_vecUI.erase(iter + iIndex);

	m_tUIList.DeleteString(iIndex);
	UpdateData(false);
}


void CUITool::OnUISaveBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(false, L"UI", L"*.UI"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.UI", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	vector<UIDATA> vecUIData;
	vecUIData.reserve((_uint)m_vecUI.size());

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	for (auto& pUI : m_vecUI)
	{
		UIDATA tData;
		ZeroMemory(&tData, sizeof(UIDATA));
		
		tData.iUITag = (_uint)pUI->getUITag();
		_matrix matTransform = pUI->getMatirx();

		_vector vPos = matTransform.r[3];
		XMStoreFloat2(&tData.vPos, vPos);

		_float fX = XMVectorGetX(XMVector2Length(matTransform.r[0]));
		_float fY = XMVectorGetX(XMVector2Length(matTransform.r[1]));

		tData.vScale = _float2(fX, fY);

		lstrcpy(tData.szTextureTag, pUI->getUITexutreName());

		vecUIData.emplace_back(tData);
	}

	pInstance->SaveFile<UIDATA>(&vecUIData, Dlg.GetPathName());

	RELEASE_INSTANCE(CGameInstance);
}


void CUITool::OnUILoadBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(true, L"UI", L"*.UI"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.UI", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	vector<UIDATA> vecUIData;
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	pInstance->LoadFile<UIDATA>(vecUIData, Dlg.GetPathName());

	CGameObject::COMINDEX* pIndex = new CGameObject::COMINDEX[(_uint)COMPONENTID::COMPONENT_END];
	for (auto& pUIData : vecUIData)
	{
		ZeroMemory(pIndex, sizeof(CGameObject::COMINDEX) * (_uint)COMPONENTID::COMPONENT_END);

		_uint iComIndex = 0;

		pInstance->Init_ComDeForm((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::VIBUFFER_RECT, &iComIndex);
		pIndex[(_uint)COMPONENTID::VIBUFFER_RECT].iStartIndex = iComIndex;

		pInstance->Init_ComDeForm((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::TEXTURE, &iComIndex, &pUIData.szTextureTag);
		pIndex[(_uint)COMPONENTID::TEXTURE].iStartIndex = iComIndex;

		CToolUI* pObj = static_cast<CToolUI*>(pInstance->Clone_GameObject((_uint)TOOLOBJID::TOOL_UI, pIndex));
		
		pObj->Change_Tranform(XMLoadFloat2(&pUIData.vScale), XMLoadFloat2(&pUIData.vPos));

		pObj->setTag((CToolUI::UITAG)pUIData.iUITag);
		m_vecUI.emplace_back(pObj);

		m_tUIList.AddString(pUIData.szTextureTag);
	}
	Safe_Delete_Arr(pIndex);
	RELEASE_INSTANCE(CGameInstance);
}
