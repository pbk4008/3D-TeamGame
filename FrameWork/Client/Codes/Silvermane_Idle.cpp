#include "pch.h"
#include "Silvermane_Idle.h"

#include "StateController.h"

CSilvermane_Idle::CSilvermane_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

CSilvermane_Idle::CSilvermane_Idle(const CSilvermane_Idle& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT CSilvermane_Idle::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_Idle::Tick(const _double& TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CSilvermane_Idle::LateTick(const _double& TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CSilvermane_Idle::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
	{
		return E_FAIL;
	}

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Idle_Player", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	return S_OK;
}

HRESULT CSilvermane_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_Idle::KeyCheck(const _double& TimeDelta)
{
	if (g_pGameInstance->getkeyDown(DIK_UP))
	{
		m_pStateController->Change_State(L"JogFwdStart");
	}
	if (g_pGameInstance->getkeyDown(DIK_DOWN))
	{
		m_pStateController->Change_State(L"JogBwdStart");
	}
	if (g_pGameInstance->getkeyDown(DIK_RIGHT))
	{
		m_pStateController->Change_State(L"JogRightStart");
	}
	if (g_pGameInstance->getkeyDown(DIK_LEFT))
	{
		m_pStateController->Change_State(L"JogLeftStart");
	}

	return _int();
}

CSilvermane_Idle* CSilvermane_Idle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_Idle* pInstance = new CSilvermane_Idle(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_Idle::Free()
{

	__super::Free();
}
