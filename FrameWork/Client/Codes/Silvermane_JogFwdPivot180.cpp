#include "pch.h"
#include "Silvermane_JogFwdPivot180.h"

#include "StateController.h"

CSilvermane_JogFwdPivot180::CSilvermane_JogFwdPivot180(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogFwdPivot180::CSilvermane_JogFwdPivot180(const CSilvermane_JogFwdPivot180& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT CSilvermane_JogFwdPivot180::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogFwdPivot180::Tick(const _double& TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CSilvermane_JogFwdPivot180::LateTick(const _double& TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CSilvermane_JogFwdPivot180::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane_JogFwdPivot180::EnterState()
{
	if (FAILED(__super::EnterState()))
	{
		return E_FAIL;
	}

	/*
	static_cast<CSilvermane*>(m_pGameObject)->Set_CurrentAnimation(m_pModel->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Fwd_Pivot_180_Player"));
	m_pModel->Set_RootMotion(true, ERootOption::XYZ);
	m_pModel->Set_LoopNextAnim(false);
	*/

	return S_OK;
}

HRESULT CSilvermane_JogFwdPivot180::ExitState()
{
	if (FAILED(__super::ExitState()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogFwdPivot180::KeyCheck(const _double& TimeDelta)
{
	if (g_pGameInstance->getkeyPress(DIK_DOWN))
	{
		//if (m_pModel->Is_AnimationFinished())
		{
			if (FAILED(m_pStateController->Change_State(L"JogBwd")))
			{
				return -1;
			}
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_UP))
	{
		if (FAILED(m_pStateController->Change_State(L"JogBwdToJogFwd")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_RIGHT))
	{
		if (FAILED(m_pStateController->Change_State(L"IdleToJogRight")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_LEFT))
	{
		if (FAILED(m_pStateController->Change_State(L"IdleToJogLeft")))
		{
			return -1;
		}
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
		{
			return -1;
		}
	}

	return _int();
}

CSilvermane_JogFwdPivot180* CSilvermane_JogFwdPivot180::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogFwdPivot180* pInstance = new CSilvermane_JogFwdPivot180(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogFwdPivot180 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogFwdPivot180::Free()
{

	__super::Free();
}
