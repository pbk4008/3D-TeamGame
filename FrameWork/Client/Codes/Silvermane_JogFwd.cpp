#include "pch.h"
#include "Silvermane_JogFwd.h"

#include "StateController.h"

CSilvermane_JogFwd::CSilvermane_JogFwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogFwd::CSilvermane_JogFwd(const CSilvermane_JogFwd& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT CSilvermane_JogFwd::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogFwd::Tick(const _double& TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CSilvermane_JogFwd::LateTick(const _double& TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CSilvermane_JogFwd::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane_JogFwd::EnterState()
{
	if (FAILED(__super::EnterState()))
	{
		return E_FAIL;
	}

	/*
	static_cast<CSilvermane*>(m_pGameObject)->Set_CurrentAnimation(m_pModel->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Fwd_Player"));
	m_pModel->Set_RootMotion(true, ERootOption::XYZ);
	m_pModel->Set_LoopNextAnim(true);
	*/

	return S_OK;
}

HRESULT CSilvermane_JogFwd::ExitState()
{
	if (FAILED(__super::ExitState()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogFwd::KeyCheck(const _double& TimeDelta)
{
	if (g_pGameInstance->getkeyPress(DIK_UP))
	{
	}
	else if (g_pGameInstance->getkeyPress(DIK_DOWN))
	{
		if (FAILED(m_pStateController->Change_State(L"JogFwdPivot180")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_RIGHT))
	{
		if (FAILED(m_pStateController->Change_State(L"JogRightStart")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_LEFT))
	{
		if (FAILED(m_pStateController->Change_State(L"JogLeftStart")))
		{
			return -1;
		}
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"JogFwdStop")))
		{
			return -1;
		}
	}

	return _int();
}

CSilvermane_JogFwd* CSilvermane_JogFwd::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogFwd* pInstance = new CSilvermane_JogFwd(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogFwd Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogFwd::Free()
{

	__super::Free();
}
