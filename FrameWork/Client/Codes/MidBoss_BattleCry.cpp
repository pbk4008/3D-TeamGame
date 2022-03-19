#include "pch.h"
#include "MidBoss_BattleCry.h"

#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

CMidBoss_BattleCry::CMidBoss_BattleCry(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CMidBoss_BattleCry::CMidBoss_BattleCry(const CMidBoss_BattleCry& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CMidBoss_BattleCry::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMidBoss_BattleCry::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	if (CBoss_Bastion_Judicator::M_BossAnimState::BATTLECRY_END == m_pAnimator->Get_CurrentAnimNode())
	{
		if (m_pAnimator->Get_AnimController()->Is_Finished())
		{
			m_pStateController->Change_State(L"Turn");
		}
	}

	return _int();
}

_int CMidBoss_BattleCry::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMidBoss_BattleCry::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMidBoss_BattleCry::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::BATTLECRY_START);
	
	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	_vector vec = { 0.f, 1.f, 0.f,0.f };
	m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CMidBoss_BattleCry::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	_vector vec = { 0.f, 1.f, 0.f,0.f };
	m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CMidBoss_BattleCry::Look_Player(void)
{
	
}

CMidBoss_BattleCry* CMidBoss_BattleCry::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CMidBoss_BattleCry* pInstance = new CMidBoss_BattleCry(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMidBoss_BattleCry Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMidBoss_BattleCry::Free()
{
	__super::Free();
}
