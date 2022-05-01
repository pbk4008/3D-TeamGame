#include "pch.h"
#include "Boss_Attack_Agg.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Attack_Agg::CBoss_Attack_Agg(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Attack_Agg::CBoss_Attack_Agg(const CBoss_Attack_Agg& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Attack_Agg::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Attack_Agg::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	//Play_Sound();

	m_pAnimator->Tick(TimeDelta);

	//어택 체크
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();
	//cout << iCurKeyFrameIndex << endl;

	_vector BossPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_vector BossLook = XMVector3Normalize(m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	_vector BossRight = XMVector3Normalize(m_pMonster->Get_Transform()->Get_State(CTransform::STATE_RIGHT));

	_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector svRight = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT));

	if(52 <= iCurKeyFrameIndex && 70 >= iCurKeyFrameIndex)
	{ 
		if (53 <= iCurKeyFrameIndex && 55 >= iCurKeyFrameIndex && false == m_bShakeCheck)
		{
			CCameraShake::SHAKEEVENT tShakeEvent;
			tShakeEvent.fDuration = 0.6f;
			tShakeEvent.fBlendInTime = 0.2f;
			tShakeEvent.fBlendOutTime = 0.4f;
			tShakeEvent.tWaveX.fAmplitude = 0.7f;
			tShakeEvent.tWaveX.fFrequency = 7.f;
			tShakeEvent.tWaveY.fAmplitude = -0.1f;
			tShakeEvent.tWaveY.fFrequency = 0.5f;
			tShakeEvent.tWaveZ.fAdditionalOffset = 0.6f;
			tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
			tShakeEvent.fInnerRadius = 10.f;
			tShakeEvent.fOuterRadius = 20.f;
			tShakeEvent.fDistanceRate = 30.f;

			g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

			_matrix mat = m_pMonster->Get_Transform()->Get_WorldMatrix();
			mat = XMMatrixRotationY(XMConvertToRadians(45.f))
				* XMMatrixTranslation(XMVectorGetX(BossRight) * -1.f , 0.f, 0.f) * mat;
			m_pMonster->Active_Effect_Target((_uint)EFFECT::BOSS_SIDE_ATTACK_SMOKE, mat);
			m_pMonster->Active_Effect_Target((_uint)EFFECT::BOSS_SIDE_ATTACK, mat);
			m_pMonster->Active_Effect((_uint)EFFECT::EXPLOSION_ROCK_RIGHT);
			m_bShakeCheck = true;
		}
		m_pMonster->Set_IsAttack(true);

		_float fDamage = 6.f;
		_uint iLevel = 3;
		m_pMonster->Set_AttackDesc_Damaga(fDamage);
		m_pMonster->Set_AttackDesc_Level(iLevel);
		m_pMonster->Set_AttackDesc_Dir(EAttackDir::Right);
	}
	else
		m_pMonster->Set_IsAttack(false);

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		if (20.f <= fDistToPlayer)
		{
			cout << "agg -> Walk" << endl;

			m_pStateController->Change_State(L"Walk_Front");
		}
		else if (20.f > fDistToPlayer)
		{
			cout << "agg -> turn" << endl;

			m_pStateController->Change_State(L"Turn");
		}
	}

	return _int();
}

_int CBoss_Attack_Agg::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Attack_Agg::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Attack_Agg::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(false);

	cout << "Attack_Agg" << endl;

	m_bShakeCheck = false;
	m_bEffectCheck = false;

	//림라이트
	m_pMonster->RimlightCheck(true, _float3(0.1f, 0, 0), 6.f);

	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);
 	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::ATTACK_AGG_SPIN_360);

	return S_OK;
}

HRESULT CBoss_Attack_Agg::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	CBoss_Solaris* m_pMonster = (CBoss_Solaris*)m_pStateController->Get_GameObject();

	m_bEffectCheck = false;

	return S_OK;
}

void CBoss_Attack_Agg::Look_Player(void)
{
}

void CBoss_Attack_Agg::Play_Sound(void)
{
	_vector Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	//if (m_iAttackR1_H == iCurKeyFrameIndex)
	//{
	//	g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXZ, Pos);
	//
	//	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->Play_Shot(L"MidBoss_Roar_1", CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->BlendSound(L"MidBoss_Swing_1", L"Boss_Attack_Agg_End_1", CSoundMgr::CHANNELID::Boss_Attack_Agg_1, CSoundMgr::CHANNELID::Boss_Attack_Agg_2);
	//	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Boss_Attack_Agg_2, 0.5f);
	//	m_bAttackR1_H = true;
	//}
}

CBoss_Attack_Agg* CBoss_Attack_Agg::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Attack_Agg* pInstance = new CBoss_Attack_Agg(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Attack_Agg Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Attack_Agg::Free()
{
	__super::Free();
}
