#include "pch.h"
#include "MidBoss_Stun.h"

#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

CMidBoss_Stun::CMidBoss_Stun(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CMidBoss_Stun::CMidBoss_Stun(const CMidBoss_Stun& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CMidBoss_Stun::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMidBoss_Stun::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	if (CBoss_Bastion_Judicator::M_BossAnimState::STUN_END == m_pAnimator->Get_CurrentAnimNode())
	{
		if (m_pAnimator->Get_AnimController()->Is_Finished())
		{
			m_pStateController->Change_State(L"Turn");
		}
	}

	return _int();
}

_int CMidBoss_Stun::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMidBoss_Stun::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMidBoss_Stun::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::STUN_START);
	
	return S_OK;
}

HRESULT CMidBoss_Stun::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;


	return S_OK;
}

void CMidBoss_Stun::Look_Player(void)
{
	
}

CMidBoss_Stun* CMidBoss_Stun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CMidBoss_Stun* pInstance = new CMidBoss_Stun(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMidBoss_Stun Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMidBoss_Stun::Free()
{
	__super::Free();
}
