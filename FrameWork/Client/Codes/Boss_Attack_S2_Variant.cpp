#include "pch.h"
#include "Boss_Attack_S2_Variant.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Attack_S2_Variant::CBoss_Attack_S2_Variant(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Attack_S2_Variant::CBoss_Attack_S2_Variant(const CBoss_Attack_S2_Variant& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Attack_S2_Variant::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Attack_S2_Variant::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	//Play_Sound();

	m_pAnimator->Tick(TimeDelta);

	//어택 체크
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();
	 
	cout << iCurKeyFrameIndex << endl;

	//m_pMonster->RimlightCheck(true, _float3(1, 0, 0));

	if (42 <= iCurKeyFrameIndex && 44 >= iCurKeyFrameIndex && false == m_bShakeCheck)
	{
		//쉐이킹 & 이펙트 //이펙트한테서 생성해줘야할듯 
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 0.3f;
		tShakeEvent.fBlendInTime = 0.1f;
		tShakeEvent.fBlendOutTime = 0.2f;
		tShakeEvent.tWaveX.fAmplitude = 0.3f;
		tShakeEvent.tWaveX.fFrequency = 10.f;
		tShakeEvent.tWaveY.fAmplitude = -0.3f;
		tShakeEvent.tWaveY.fFrequency = 6.f;
		tShakeEvent.tWaveZ.fAdditionalOffset = 0.2f;
		tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
		tShakeEvent.fInnerRadius = 10.f;
		tShakeEvent.fOuterRadius = 20.f;
		tShakeEvent.fDistanceRate = 10.f;

		g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

		m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE);
		m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND);
		m_pMonster->Active_Effect((_uint)EFFECT::EXPLOSION_ROCK_UP);

		m_bShakeCheck = true;

		m_pMonster->Set_IsAttack(true);

		_float fDamage = 6.f;
		_uint iLevel = 3;
		m_pMonster->Set_AttackDesc_Damaga(fDamage);
		m_pMonster->Set_AttackDesc_Level(iLevel);
		m_pMonster->Set_AttackDesc_Dir(EAttackDir::Forward);
	}

	else
		m_pMonster->Set_IsAttack(false);

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Turn");
	}

	return _int();
}

_int CBoss_Attack_S2_Variant::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Attack_S2_Variant::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Attack_S2_Variant::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_bShakeCheck = false;
	m_bEffectCheck = false;

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(false);
	//림라이트
	//m_pMonster->RimlightCheck(true);

	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);
 	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::ATTACK_S2_VARIANT);

	return S_OK;
}

HRESULT CBoss_Attack_S2_Variant::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pMonster->RimlightCheck(false);

	m_bEffectCheck = false;

	return S_OK;
}

void CBoss_Attack_S2_Variant::Look_Player(void)
{
}

void CBoss_Attack_S2_Variant::Play_Sound(void)
{
	_vector Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	//if (m_iAttackR1_H == iCurKeyFrameIndex)
	//{
	//	g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXZ, Pos);
	//
	//	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->Play_Shot(L"MidBoss_Roar_1", CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->BlendSound(L"MidBoss_Swing_1", L"Boss_Attack_S2_Variant_End_1", CSoundMgr::CHANNELID::Boss_Attack_S2_Variant_1, CSoundMgr::CHANNELID::Boss_Attack_S2_Variant_2);
	//	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Boss_Attack_S2_Variant_2, 0.5f);
	//	m_bAttackR1_H = true;
	//}
}

CBoss_Attack_S2_Variant* CBoss_Attack_S2_Variant::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Attack_S2_Variant* pInstance = new CBoss_Attack_S2_Variant(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Attack_S2_Variant Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Attack_S2_Variant::Free()
{
	__super::Free();
}
