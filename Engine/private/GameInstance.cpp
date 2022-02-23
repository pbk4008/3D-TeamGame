#include "..\public\GameInstance.h"
#include "GameObject.h"
#include "Light_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pFrustum(CFrustum::GetInstance())
	, m_pFont_Manager(CFont_Manager::GetInstance())
{
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, HWND hWnd, _uint iNumLevel, CGraphic_Device::WINMODE eWinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pObject_Manager || 
		nullptr == m_pFrustum)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(hWnd, eWinMode, iWinCX, iWinCY, ppDeviceOut, ppDeviceContextOut)))
		return E_FAIL;	

	if (FAILED(m_pInput_Device->Ready_Input_Device(hInst, hWnd)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevel)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevel)))
		return E_FAIL;	

	if (FAILED(m_pFrustum->Ready_FrustumInProjSpace()))
		return E_FAIL;

	return S_OK;
}

_int CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager || 
		nullptr == m_pPipeLine ||
		nullptr == m_pInput_Device || 
		nullptr == m_pFrustum)
		return -1;

	_int	iProgress = 0;

	if (FAILED(m_pInput_Device->SetUp_InputDeviceState()))
		return -1;

	iProgress = m_pLevel_Manager->Tick(TimeDelta);
	if (0 > iProgress)
		return -1;

	iProgress = m_pObject_Manager->Tick(TimeDelta);
	if (0 > iProgress)
		return -1;

	m_pPipeLine->Update_PipeLine();

	m_pFrustum->Transform_ToWorldSpace();
	
	iProgress = m_pObject_Manager->LateTick(TimeDelta);
	if (0 > iProgress)
		return -1;

	return _int();
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pLevel_Manager->Render();	

	return S_OK;
}

HRESULT CGameInstance::Clear_Engine(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pObject_Manager->Clear_Object_Manager(iLevelIndex);

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(XMFLOAT4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);;
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

_double CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0;

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

void CGameInstance::Update_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	return m_pTimer_Manager->Update_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Ready_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Ready_Timer(pTimerTag);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pOpenLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pOpenLevel);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObjectToLayer(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObjectToLayer(iLevelIndex, pLayerTag, pPrototypeTag, pArg);	
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

_fmatrix CGameInstance::Get_Transform(CPipeLine::TRANSFORMSTATEMATRIX eType)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform(eType);
}

_fvector CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return XMVectorZero();

	return m_pPipeLine->Get_CamPosition();
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATEMATRIX eType, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eType, TransformMatrix);
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID) const
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKState(byKeyID);	
}

_long CGameInstance::Get_MouseMoveState(CInput_Device::MOUSEMOVESTATE eMoveState) const
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_MouseMoveState(eMoveState);
}

_byte CGameInstance::Get_MouseButtonState(CInput_Device::MOUSEBUTTONSTATE eButtonState) const
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_MouseButtonState(eButtonState);
}

const LIGHTDESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;
	
	return m_pLight_Manager->Add_Light(pDevice, pDeviceContext, LightDesc);	
}

HRESULT CGameInstance::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontTag, const _tchar * pFontPath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(pDevice, pDeviceContext, pFontTag, pFontPath);
}

HRESULT CGameInstance::Render_Font(const _tchar * pFontTag, _fvector vColor, const _tchar * pString)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render_Font(pFontTag, vColor, pString);
}

_bool CGameInstance::isIn_WorldFrustum(_fvector vPosition, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isInWorld(vPosition, fRange);
}

_bool CGameInstance::isIn_LocalFrustum(_fvector vPosition, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isInLocal(vPosition, fRange);
}

void CGameInstance::Release_Engine()
{

	if (0 != CGameInstance::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CGameInstance");	

	if (0 != CLevel_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CLevel_Manager");

	if (0 != CObject_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CObject_Manager");

	if (0 != CComponent_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CComponent_Manager");

	if (0 != CPipeLine::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CPipeLine");

	if (0 != CTimer_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CTimer_Manager");

	if (0 != CInput_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CInput_Device");

	if (0 != CLight_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CLight_Manager");

	if (0 != CTarget_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CTarget_Manager");

	if (0 != CFrustum::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CFrustum");

	if (0 != CFont_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CFont_Manager");

	if (0 != CGraphic_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CGraphic_Device");
}

void CGameInstance::Free()
{
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
}


