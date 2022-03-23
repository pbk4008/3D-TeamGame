#include "pch.h"
#include "2H_HammerIdle.h"

#include "StateController.h"

C2H_HammerIdle::C2H_HammerIdle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

C2H_HammerIdle::C2H_HammerIdle(const C2H_HammerIdle& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT C2H_HammerIdle::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerIdle::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	m_fHoldTime += (_float)_dDeltaTime;
	Add_PlusAngle(EDir::Forward, _dDeltaTime);

	return _int();
}

_int C2H_HammerIdle::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerIdle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerIdle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Pose_Normal", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	return S_OK;
}

HRESULT C2H_HammerIdle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_fHoldTime = 0.f;

	return S_OK;
}

_int C2H_HammerIdle::KeyCheck(const _double& _dDeltaTime)
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

	if (g_pGameInstance->getkeyDown(DIK_Q))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_01")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerChargeStage1_Start")))
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
			if (!m_pSilvermane->IsEquipWeapon())
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwdStart")))
					return -1;
			}
			else
			{
				if (FAILED(m_pStateController->Change_State(L"2H_HammerEquipOff")))
					return -1;
			}
			return STATE_CHANGE;
		}
	}
	else
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			m_pStateController->Change_State(L"2H_HammerJogFwdStart");
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_S))
		{
			m_pStateController->Change_State(L"2H_HammerJogBwdStart");
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_D))
		{
			m_pStateController->Change_State(L"2H_HammerJogRightStart");
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_A))
		{
			m_pStateController->Change_State(L"2H_HammerJogLeftStart");
			return STATE_CHANGE;
		}
	}

	return _int();
}

C2H_HammerIdle* C2H_HammerIdle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerIdle* pInstance = new C2H_HammerIdle(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerIdle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerIdle::Free()
{

	__super::Free();
}
