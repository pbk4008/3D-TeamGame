#include "pch.h"
#include "Boss_Attack_S6.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Attack_S6::CBoss_Attack_S6(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Attack_S6::CBoss_Attack_S6(const CBoss_Attack_S6& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Attack_S6::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Attack_S6::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	//Play_Sound();

	m_pAnimator->Tick(TimeDelta);

	//어택 체크
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();
	 
	if (59 <= iCurKeyFrameIndex && 123 >= iCurKeyFrameIndex)
	{
		m_pAnimator->Get_AnimController()->Set_PlaySpeed(0.4f);

		//쉐이킹 & 이펙트 //이펙트한테서 생성해줘야할듯 
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 0.1f;
		tShakeEvent.fBlendInTime = 0.02f;
		tShakeEvent.fBlendOutTime = 0.07f;
		tShakeEvent.tWaveX.fAmplitude = -0.1f;
		tShakeEvent.tWaveX.fFrequency = 20.f;
		tShakeEvent.tWaveY.fAmplitude = 0.1f;
		tShakeEvent.tWaveY.fFrequency = 6.f;
		tShakeEvent.tWaveZ.fAdditionalOffset = 0.2f;
		tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
		tShakeEvent.fInnerRadius = 10.f;
		tShakeEvent.fOuterRadius = 20.f;
		tShakeEvent.fDistanceRate = 10.f;

		if (66 < iCurKeyFrameIndex && 69 > iCurKeyFrameIndex && false == m_bShakeCheck)
		{
			g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

			m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND);
			m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE);

			m_bShakeCheck = true;
		}

		if (82 < iCurKeyFrameIndex && 85 > iCurKeyFrameIndex && false == m_bShakeCheck)
		{
			g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

			m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND);
			m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE);

			m_bShakeCheck = true;
		}

		if (100 < iCurKeyFrameIndex && 103 > iCurKeyFrameIndex && false == m_bShakeCheck)
		{
			g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

			m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND);
			m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE);

			m_bShakeCheck = true;
		}

		if (120 < iCurKeyFrameIndex && 123 > iCurKeyFrameIndex && false == m_bShakeCheck)
		{
			g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

			m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND);
			m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE);

			m_bShakeCheck = true;
		}

		m_bShakeCheck = false;

		m_pMonster->Set_IsAttack(true);

		_float fDamage = 6.f;
		_uint iLevel = 3;
		m_pMonster->Set_AttackDesc_Damaga(fDamage);
		m_pMonster->Set_AttackDesc_Level(iLevel);
	}

	else
	{
		m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);
		m_pMonster->Set_IsAttack(false);
	}

	m_pMonster->Set_AttackDesc_Dir(EAttackDir::Forward);

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Turn");
	}

	return _int();
}

_int CBoss_Attack_S6::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Attack_S6::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Attack_S6::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(false);

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_bShakeCheck = false;
	m_bEffectCheck = false;

 	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::ATTACK_S6);

	return S_OK;
}

HRESULT CBoss_Attack_S6::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	CBoss_Solaris* m_pMonster = (CBoss_Solaris*)m_pStateController->Get_GameObject();
	m_pMonster->RimlightCheck(false);

	m_bEffectCheck = false;

	return S_OK;
}

void CBoss_Attack_S6::Look_Player(void)
{
}

void CBoss_Attack_S6::Play_Sound(void)
{
	_vector Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	//if (m_iAttackR1_H == iCurKeyFrameIndex)
	//{
	//	g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXZ, Pos);
	//
	//	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->Play_Shot(L"MidBoss_Roar_1", CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->BlendSound(L"MidBoss_Swing_1", L"Boss_Attack_S6_End_1", CSoundMgr::CHANNELID::Boss_Attack_S6_1, CSoundMgr::CHANNELID::Boss_Attack_S6_2);
	//	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Boss_Attack_S6_2, 0.5f);
	//	m_bAttackR1_H = true;
	//}
}

CBoss_Attack_S6* CBoss_Attack_S6::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Attack_S6* pInstance = new CBoss_Attack_S6(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Attack_S6 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Attack_S6::Free()
{
	__super::Free();
}
