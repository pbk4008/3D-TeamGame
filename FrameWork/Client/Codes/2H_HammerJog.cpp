#include "pch.h"
#include "2H_HammerJog.h"

C2H_HammerJog::C2H_HammerJog(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerJog::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerJog::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int C2H_HammerJog::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerJog::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerJog::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pSilvermane->Set_IsMove(true);

	return S_OK;
}

HRESULT C2H_HammerJog::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsMove(false);

	return S_OK;
}

_int C2H_HammerJog::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyDown(DIK_Q))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
			return E_FAIL;
		return STATE_CHANGE;
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
		if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackJogR1")))
			return -1;
		return STATE_CHANGE;
	}

	if (g_pGameInstance->getkeyDown(DIK_LSHIFT))
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

	return _int();
}

void C2H_HammerJog::Free()
{

	__super::Free();
}
