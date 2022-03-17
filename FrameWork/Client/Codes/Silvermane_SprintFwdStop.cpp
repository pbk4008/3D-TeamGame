#include "pch.h"
#include "Silvermane_SprintFwdStop.h"

CSilvermane_SprintFwdStop::CSilvermane_SprintFwdStop(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Sprint(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_SprintFwdStop::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 40;

	return S_OK;
}

_int CSilvermane_SprintFwdStop::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CSilvermane_SprintFwdStop::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_SprintFwdStop::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_SprintFwdStop::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Sprint_Fwd_Stop_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	return S_OK;
}

HRESULT CSilvermane_SprintFwdStop::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_SprintFwdStop::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
	{
		if( g_pGameInstance->getkeyPress(DIK_W) ||
			g_pGameInstance->getkeyPress(DIK_A) ||
			g_pGameInstance->getkeyPress(DIK_D) ||
			g_pGameInstance->getkeyPress(DIK_S))
		{
			if(FAILED(m_pStateController->Change_State(L"SprintFwdStart")))
				return -1;
			return STATE_CHANGE;
		}
		else
		{
			if (m_pAnimationController->Is_Finished())
			{
				if (FAILED(m_pStateController->Change_State(L"Idle")))
					return -1;
				return STATE_CHANGE;
			}
		}
	}
	else
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (FAILED(m_pStateController->Change_State(L"JogFwdStart")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyPress(DIK_S))
		{
			if (FAILED(m_pStateController->Change_State(L"JogBwdStart")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyPress(DIK_A))
		{
			if (FAILED(m_pStateController->Change_State(L"JogLeftStart")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
			if (FAILED(m_pStateController->Change_State(L"JogRightStart")))
				return -1;
			return STATE_CHANGE;
		}
		else
		{
			if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
			{
				if (FAILED(m_pStateController->Change_State(L"Idle")))
					return -1;
				return STATE_CHANGE;
			}
		}
	}

	return _int();
}

CSilvermane_SprintFwdStop* CSilvermane_SprintFwdStop::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_SprintFwdStop* pInstance = new CSilvermane_SprintFwdStop(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_SprintFwdStop Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_SprintFwdStop::Free()
{

	__super::Free();
}
