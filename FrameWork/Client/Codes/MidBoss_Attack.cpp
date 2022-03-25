#include "pch.h"
#include "MidBoss_Attack.h"

#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

CMidBoss_Attack::CMidBoss_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CMidBoss_Attack::CMidBoss_Attack(const CMidBoss_Attack& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CMidBoss_Attack::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMidBoss_Attack::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Turn");
	}

	//���� üũ
	CBoss_Bastion_Judicator* pBoss = (CBoss_Bastion_Judicator*)m_pStateController->Get_GameObject();
	if (nullptr != pBoss)
	{
		if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_S2 == m_pAnimator->Get_CurrentAnimNode())
		{
			//TODO : ���� �������� �ݶ��̴� ���ܼ� �װɷ� �浹ó���ؾߵ�
		}

		pBoss->Set_IsAttack(true);
	}

	return _int();
}

_int CMidBoss_Attack::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMidBoss_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMidBoss_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());


	//TODO : ����������ؾ��� �Ÿ���� 
	if (5.f > fDistToPlayer /*4.f > XMVectorGetX(vDist) && 4.f > XMVectorGetZ(vDist) && 4.f > XMVectorGetY(vDist)*/)
	{
		_int iRandom = rand() % 4;

		while (m_iPreState_1 == iRandom)
		{
			iRandom = rand() % 4;
		}

		if (m_iPreState_1 != iRandom)
		{
			switch (iRandom)
			{
			case 0:
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_R1_H);
				break;
			case 1:
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_R1);
				break;
			case 2:
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_S1);
				break;
			case 3:
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_S2);
				break;
			}

			m_iPreState_1 = iRandom;
		}
	}

	else if (5.f <= fDistToPlayer)
	{
		_int iRandom = rand() % 3;

		while (m_iPreState_2 == iRandom)
		{
			iRandom = rand() % 3;
		}

		if (m_iPreState_2 != iRandom)
		{
			switch (iRandom)
			{
			case 0:
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_JOG_H);
				break;
			case 1:
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::SPRINT_ATTACK_H);
				break;
			case 2:
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_LEGACY_H);
			}

			m_iPreState_2 = iRandom;
		}
	}

	return S_OK;
}

HRESULT CMidBoss_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	return S_OK;
}

void CMidBoss_Attack::Look_Player(void)
{
}

CMidBoss_Attack* CMidBoss_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CMidBoss_Attack* pInstance = new CMidBoss_Attack(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMidBoss_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMidBoss_Attack::Free()
{
	__super::Free();
}
