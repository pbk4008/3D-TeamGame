#include "pch.h"
#include "Silvermane_SprintFwdStart.h"

CSilvermane_SprintFwdStart::CSilvermane_SprintFwdStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_SprintFwdStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_SprintFwdStart::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CSilvermane_SprintFwdStart::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_SprintFwdStart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_SprintFwdStart::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Sprint_Fwd_Start_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	return S_OK;
}

HRESULT CSilvermane_SprintFwdStart::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_SprintFwdStart::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (m_pAnimationController->Is_Finished())
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwd")))
					return E_FAIL;
				return STATE_CHANGE;
			}

			if (g_pGameInstance->getkeyPress(DIK_A))
			{
				Add_PlusAngle(EDir::LeftForward, _dDeltaTime);
			}
			else if (g_pGameInstance->getkeyPress(DIK_D))
			{
				Add_PlusAngle(EDir::RightForward, _dDeltaTime);
			}
			else
			{
				Add_PlusAngle(EDir::Forward, _dDeltaTime);
			}
		}
		else if (g_pGameInstance->getkeyPress(DIK_S))
		{
			if (m_pAnimationController->Is_Finished())
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwd")))
					return E_FAIL;
				return STATE_CHANGE;
			}
			if (g_pGameInstance->getkeyPress(DIK_A))
			{
				Add_PlusAngle(EDir::LeftBackward, _dDeltaTime);
			}
			else if (g_pGameInstance->getkeyPress(DIK_D))
			{
				Add_PlusAngle(EDir::RightBackward, _dDeltaTime);
			}
			else
			{
				Add_PlusAngle(EDir::Backward, _dDeltaTime);
			}
		}
		else if (g_pGameInstance->getkeyPress(DIK_A))
		{
			if (m_pAnimationController->Is_Finished())
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwd")))
					return E_FAIL;
				return STATE_CHANGE;
			}

			Add_PlusAngle(EDir::Left, _dDeltaTime);
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
			if (m_pAnimationController->Is_Finished())
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwd")))
					return E_FAIL;
				return STATE_CHANGE;
			}

			Add_PlusAngle(EDir::Right, _dDeltaTime);
		}
		else
		{
			if (FAILED(m_pStateController->Change_State(L"SprintFwdStop")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}
	else
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (FAILED(m_pStateController->Change_State(L"JogFwd")))
				return E_FAIL;
			return STATE_CHANGE;
		}
		else
		{
			if (FAILED(m_pStateController->Change_State(L"SprintFwdStop")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	return _int();
}

CSilvermane_SprintFwdStart* CSilvermane_SprintFwdStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_SprintFwdStart* pInstance = new CSilvermane_SprintFwdStart(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_SprintFwdStart Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_SprintFwdStart::Free()
{

	__super::Free();
}
