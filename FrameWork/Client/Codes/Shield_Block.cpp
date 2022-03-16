#include "pch.h"
#include "Shield_Block.h"

CShield_Block::CShield_Block(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_Block::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_Block::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CShield_Block::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_Block::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_Block::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_Block::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CShield_Block::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyUp(DIK_Q))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_BlockEnd")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
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
	}

	if (m_pSilvermane->Is_EquipShield())
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_WalkFwdStart")))
				return -1;
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_S))
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_WalkBwdStart")))
				return -1;
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_D))
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_WalkRightStart")))
				return -1;
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_A))
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_WalkLeftStart")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

void CShield_Block::Free()
{

	__super::Free();
}
