#include "pch.h"
#include "Silvermane_JogFwdStop.h"

#include "StateController.h"

CSilvermane_JogFwdStop::CSilvermane_JogFwdStop(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogFwdStop::CSilvermane_JogFwdStop(const CSilvermane_JogFwdStop& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT CSilvermane_JogFwdStop::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogFwdStop::Tick(const _double& TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CSilvermane_JogFwdStop::LateTick(const _double& TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CSilvermane_JogFwdStop::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane_JogFwdStop::EnterState()
{
	if (FAILED(__super::EnterState()))
	{
		return E_FAIL;
	}

	/*
	static_cast<CSilvermane*>(m_pGameObject)->Set_CurrentAnimation(m_pModel->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Fwd_Stop_Player"));
	m_pModel->Set_RootMotion(true, ERootOption::XYZ);
	m_pModel->Set_LoopNextAnim(false);
	*/

	return S_OK;
}

HRESULT CSilvermane_JogFwdStop::ExitState()
{
	if (FAILED(__super::ExitState()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogFwdStop::KeyCheck(const _double& TimeDelta)
{
	if (g_pGameInstance->getkeyPress(DIK_UP))
	{
		if (FAILED(m_pStateController->Change_State(L"JogFwd")))
		{
			return -1;
		}
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
		//if (m_pModel->Is_AnimationFinished())
		{
			if (FAILED(m_pStateController->Change_State(L"Idle")))
			{
				return -1;
			}
		}
	}


	return _int();
}

CSilvermane_JogFwdStop* CSilvermane_JogFwdStop::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogFwdStop* pInstance = new CSilvermane_JogFwdStop(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogFwdStop Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogFwdStop::Free()
{

	__super::Free();
}
