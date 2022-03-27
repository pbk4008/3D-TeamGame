#include "pch.h"
#include "BronzeAnimus_Battle_Idle.h"
#include "Monster_BronzeAnimus.h"
#include "Animation.h"

CBronzeAnimus_Battle_Idle::CBronzeAnimus_Battle_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

CBronzeAnimus_Battle_Idle::CBronzeAnimus_Battle_Idle(const CBronzeAnimus_Battle_Idle& _rhs)
	: CBronzeAnimus_State(_rhs)
{
}

HRESULT CBronzeAnimus_Battle_Idle::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Battle_Idle::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if(!m_bBattleOn)
		m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBronzeAnimus_Battle_Idle::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Battle_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Battle_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_IDLE_BATTLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Battle_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_bBattleOn = true;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CBronzeAnimus_Battle_Idle::Look_Player(void)
{
	if (m_bTargetOn)
	{
		m_bBattleOn = true;
		if(__super::m_bBattleOn)
			m_pStateController->Change_State(L"Chaser");
		else
			m_pStateController->Change_State(L"Roar");
	}
}

void CBronzeAnimus_Battle_Idle::Look_Monster(void)
{

}

CBronzeAnimus_Battle_Idle* CBronzeAnimus_Battle_Idle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Battle_Idle* pInstance = new CBronzeAnimus_Battle_Idle(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Battle_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBronzeAnimus_Battle_Idle::Free()
{
	__super::Free();
}
