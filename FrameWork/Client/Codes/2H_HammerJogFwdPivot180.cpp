#include "pch.h"
#include "2H_HammerJogFwdPivot180.h"

#include "StateController.h"

C2H_HammerJogFwdPivot180::C2H_HammerJogFwdPivot180(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerJog(_pDevice, _pDeviceContext)
{
}

C2H_HammerJogFwdPivot180::C2H_HammerJogFwdPivot180(const C2H_HammerJogFwdPivot180& _rhs)
	: C2H_HammerJog(_rhs)
{
}

HRESULT C2H_HammerJogFwdPivot180::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerJogFwdPivot180::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

_int C2H_HammerJogFwdPivot180::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerJogFwdPivot180::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerJogFwdPivot180::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Loco_Jog_Fwd_Pivot_180_Normal", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);
	

	return S_OK;
}

HRESULT C2H_HammerJogFwdPivot180::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerJogFwdPivot180::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (m_pAnimationController->Is_Finished())
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerJogBwd")))
				return -1;
			return STATE_CHANGE;
		}

		if (g_pGameInstance->getkeyPress(DIK_A))
		{
			Add_PlusAngle(EDir::RightForward, _dDeltaTime);
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
			Add_PlusAngle(EDir::LeftForward, _dDeltaTime);
		}
		else
		{
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogBwdPivot180")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogRightStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogLeftStart")))
			return -1;
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

C2H_HammerJogFwdPivot180* C2H_HammerJogFwdPivot180::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerJogFwdPivot180* pInstance = new C2H_HammerJogFwdPivot180(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerJogFwdPivot180 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerJogFwdPivot180::Free()
{

	__super::Free();
}
