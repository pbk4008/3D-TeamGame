#include "pch.h"
#include "1H_SwordIdle.h"

#include "StateController.h"

C1H_SwordIdle::C1H_SwordIdle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

C1H_SwordIdle::C1H_SwordIdle(const C1H_SwordIdle& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT C1H_SwordIdle::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordIdle::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_fHoldTime += (_float)_dDeltaTime;
	Add_PlusAngle(EDir::Forward, _dDeltaTime);

	return _int();
}

_int C1H_SwordIdle::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordIdle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordIdle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Pose_Normal", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	return S_OK;
}

HRESULT C1H_SwordIdle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_fHoldTime = 0.f;

	return S_OK;
}

_int C1H_SwordIdle::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyDown(DIK_COLON))
	{
		if (m_pSilvermane->Change_Weapon(L"Needle"))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordEquipOn")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}
	else if (g_pGameInstance->getkeyDown(DIK_SEMICOLON))
	{
		if (m_pSilvermane->Change_Weapon(L"Fury"))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerEquipOn")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	if (g_pGameInstance->getkeyDown(DIK_Q))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR1_01")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_Start")))
			return -1;
		return STATE_CHANGE;
	}

	if (g_pGameInstance->getkeyDown(DIK_SPACE))
	{
		if (g_pGameInstance->getkeyDown(DIK_W))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_DodgeSpin")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyDown(DIK_A))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SidestepLeft")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyDown(DIK_D))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SidestepRight")))
				return -1;
			return STATE_CHANGE;
		}
		else
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
				return -1;
			return STATE_CHANGE;
		}
	}

	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
	{
		if (g_pGameInstance->getkeyPress(DIK_W) ||
			g_pGameInstance->getkeyPress(DIK_S) ||
			g_pGameInstance->getkeyPress(DIK_A) ||
			g_pGameInstance->getkeyPress(DIK_D))
		{
			if (!m_pSilvermane->Is_EquipWeapon())
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwdStart")))
					return -1;
			}
			else
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SwordEquipOff")))
					return -1;
			}
			return STATE_CHANGE;
		}
	}
	else
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			m_pStateController->Change_State(L"1H_SwordJogFwdStart");
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_S))
		{
			m_pStateController->Change_State(L"1H_SwordJogBwdStart");
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_D))
		{
			m_pStateController->Change_State(L"1H_SwordJogRightStart");
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_A))
		{
			m_pStateController->Change_State(L"1H_SwordJogLeftStart");
			return STATE_CHANGE;
		}
	}

	return _int();
}

C1H_SwordIdle* C1H_SwordIdle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordIdle* pInstance = new C1H_SwordIdle(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordIdle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordIdle::Free()
{

	__super::Free();
}
