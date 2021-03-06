#include "pch.h"
#include "Silvermane_Sprint.h"

CSilvermane_Sprint::CSilvermane_Sprint(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_Sprint::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_Sprint::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CSilvermane_Sprint::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_Sprint::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_Sprint::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pSilvermane->Set_IsMove(true);

	return S_OK;
}

HRESULT CSilvermane_Sprint::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsMove(false);

	return S_OK;
}

void CSilvermane_Sprint::OnTriggerEnter(CCollision& collision)
{
	//OnTriggerEnterHit(collision);
}

_int CSilvermane_Sprint::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyDown(DIK_Q))
	{
		if (m_pSilvermane->IsLootShield())
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	if (g_pGameInstance->getkeyPress(DIK_E))
	{
		return ToSkill();
	}

	if (g_pGameInstance->getkeyDown(DIK_SPACE))
	{
		if (g_pGameInstance->getkeyPress(DIK_A))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SidestepLeft")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyPress(DIK_S))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SidestepRight")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_DodgeSpin")))
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


	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		switch (m_pSilvermane->Get_WeaponType())
		{
		case CWeapon::EType::Sword_1H:
			if (FAILED(m_pStateController->Change_State(L"1H_SwordSupermanStab")))
				return -1;
			return STATE_CHANGE;
			break;
		case CWeapon::EType::Hammer_2H:
			if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackSprintR1")))
				return -1;
			return STATE_CHANGE;
			break;
		}
	}
	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (m_pSilvermane->IsLootShield())
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_SupermanPunchStraight")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

void CSilvermane_Sprint::Free()
{

	__super::Free();
}
