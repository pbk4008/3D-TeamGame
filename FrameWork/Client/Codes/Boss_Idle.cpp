#include "pch.h"
#include "Boss_Idle.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Idle::CBoss_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Idle::CBoss_Idle(const CBoss_Idle& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Idle::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Idle::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_pAnimator->Tick(TimeDelta);
	
	/*if ( 15.f > fDistToPlayer)
	{
		m_pStateController->Change_State(L"Rage");
	}*/
	
	if (15.f >= fDistToPlayer)
	{
		int a = 0;
	}

	return _int();
}

_int CBoss_Idle::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(true);


	m_pAnimator->Change_AnyEntryAnimation(CBoss_Solaris::M_BossAnimState::IDLE);
	
	return S_OK;
}

HRESULT CBoss_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBoss_Idle::Look_Player(void)
{

}

CBoss_Idle* CBoss_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Idle* pInstance = new CBoss_Idle(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Idle::Free()
{
	__super::Free();
}
