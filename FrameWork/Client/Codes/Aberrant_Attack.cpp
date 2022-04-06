#include "pch.h"
#include "Aberrant_Attack.h"

#include "Monster_EarthAberrant.h"
#include "Animation.h"

CAberrant_Attack::CAberrant_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CAberrant_Attack::CAberrant_Attack(const CAberrant_Attack& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Attack::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAberrant_Attack::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	Play_Sound();

	m_pAnimator->Tick(TimeDelta);


	CMonster_EarthAberrant* pMonster = (CMonster_EarthAberrant*)m_pStateController->Get_GameObject();
	if (nullptr != pMonster)
	{
		_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

		if (CMonster_EarthAberrant::ATTACK_R1 == m_pAnimator->Get_CurrentAnimNode())
		{
			if (42 < iCurKeyFrameIndex && 55 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);
				
				_float fDamage = 5.f;
				_uint iLevel = 1;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}
			else
				pMonster->Set_IsAttack(false);
		}
		if (CMonster_EarthAberrant::ATTACK_R2 == m_pAnimator->Get_CurrentAnimNode())
		{
			if (13 < iCurKeyFrameIndex && 30 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);

				_float fDamage = 3.f;
				_uint iLevel = 1;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}
			else
				pMonster->Set_IsAttack(false);
		}
	}

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
		m_pStateController->Change_State(L"Dash_Bwd");
	}
	return _int();
}

_int CAberrant_Attack::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

//HRESULT CAberrant_Attack::EnterState()
//{
//	if (FAILED(__super::EnterState()))
//		return E_FAIL;
//
//	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
//	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
//	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));
//
//	
//	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
//
//	_uint iRandom = rand() % 2;
//
//	switch (iRandom)
//	{
//	case 0:
//		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_EarthAberrant::MON_STATE::ATTACK_R1);
//		m_iAttackType = 0;
//		break;
//	case 1:
//		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_EarthAberrant::MON_STATE::ATTACK_R2);
//		m_iAttackType = 1;
//		break;
//	}
//
//	return S_OK;
//}


HRESULT CAberrant_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.f);

	return S_OK;
}

HRESULT CAberrant_Attack::EnterState(void* pArg)
{
	_uint iAttackType = (*(_uint*)pArg);

	if (iAttackType == 0)
	{
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_EarthAberrant::MON_STATE::ATTACK_R1);
		m_pAnimator->Get_AnimController()->Set_MoveSpeed(30.f);
	}
	else
	{
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_EarthAberrant::MON_STATE::ATTACK_R2);
		m_pAnimator->Get_AnimController()->Set_MoveSpeed(35.f);
	}

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	return S_OK;
}

void CAberrant_Attack::Look_Player(void)
{

}

void CAberrant_Attack::Play_Sound(void)
{
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	if (!m_iAttackType)
	{
		if (m_iAttack1Frame == iCurKeyFrameIndex)
		{
			g_pGameInstance->BlendSound(L"Earth_Attack", L"Earth_Attack_2", CSoundMgr::CHANNELID::Earth_Attack_1, CSoundMgr::CHANNELID::Earth_Attack_2);
		}
	}
	else
	{
		if (m_iAttack2Frame == iCurKeyFrameIndex)
		{
			g_pGameInstance->BlendSound(L"Earth_Attack", L"Earth_Attack_3", CSoundMgr::CHANNELID::Earth_Attack_1, CSoundMgr::CHANNELID::Earth_Attack_2);
		}
	}
}

CAberrant_Attack* CAberrant_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Attack* pInstance = new CAberrant_Attack(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Attack::Free()
{
	__super::Free();
}
