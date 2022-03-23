#include "pch.h"
#include "Silvermane_Idle.h"

#include "StateController.h"
#include "Silvermane.h"

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
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_Idle::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);

	return _int();
}

_int CSilvermane_Idle::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Idle_Player", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	return S_OK;
}

HRESULT CSilvermane_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_fHoldTime = 0.f;

	return S_OK;
}

_int CSilvermane_Idle::KeyCheck(const _double& _dDeltaTime)
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
		if (m_pSilvermane->IsEquipWeapon())
		{
			switch (m_pSilvermane->Get_WeaponType())
			{
			case CWeapon::EType::Sword_1H:
				if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR1_01")))
					return -1;
				return STATE_CHANGE;
				break;
			case CWeapon::EType::Hammer_2H:
				if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackNormalR1_01")))
					return -1;
				return STATE_CHANGE;
				break;
			}
		}
		else
		{
			switch (m_pSilvermane->Get_WeaponType())
			{
			case CWeapon::EType::Sword_1H:
				if (FAILED(m_pStateController->Change_State(L"1H_SwordEquipOn")))
					return -1;
				return STATE_CHANGE;
				break;
			case CWeapon::EType::Hammer_2H:
				if (FAILED(m_pStateController->Change_State(L"2H_HammerEquipOn")))
					return -1;
				return STATE_CHANGE;
				break;
			}
		}
	}
	else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (m_pSilvermane->IsEquipWeapon())
		{
			switch (m_pSilvermane->Get_WeaponType())
			{
			case CWeapon::EType::Sword_1H:
				if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_Start")))
					return -1;
				return STATE_CHANGE;
				break;
			case CWeapon::EType::Hammer_2H:
				if (FAILED(m_pStateController->Change_State(L"2H_HammerChargeStage1_Start")))
					return -1;
				return STATE_CHANGE;
				break;
			}
		}
		else
		{
			switch (m_pSilvermane->Get_WeaponType())
			{
			case CWeapon::EType::Sword_1H:
				if (FAILED(m_pStateController->Change_State(L"1H_SwordEquipOn")))
					return -1;
				return STATE_CHANGE;
				break;
			case CWeapon::EType::Hammer_2H:
				if (FAILED(m_pStateController->Change_State(L"2H_HammerEquipOn")))
					return -1;
				return STATE_CHANGE;
				break;
			}
		}
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
			m_pStateController->Change_State(L"JogFwdStart");
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_S))
		{
			m_pStateController->Change_State(L"JogBwdStart");
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_D))
		{
			m_pStateController->Change_State(L"JogRightStart");
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_A))
		{
			m_pStateController->Change_State(L"JogLeftStart");
			return STATE_CHANGE;
		}
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
