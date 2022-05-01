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

	Play_Sound();

	m_pAnimator->Tick(TimeDelta);

	//어택 체크
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_JOG_H == m_pAnimator->Get_CurrentAnimNode())
	{
		//cout << "JOG : " << iCurKeyFrameIndex << endl;

		if (20 < iCurKeyFrameIndex && 35 > iCurKeyFrameIndex)
		{
			if (27 <= iCurKeyFrameIndex && 28 >= iCurKeyFrameIndex && false == m_bEffectCheck)
			{
				CCameraShake::SHAKEEVENT tShakeEvent;
				tShakeEvent.fDuration = 0.7f;
				tShakeEvent.fBlendInTime = 0.2f;
				tShakeEvent.fBlendOutTime = 0.5f;
				tShakeEvent.tWaveX.fAmplitude = 0.1f;
				tShakeEvent.tWaveX.fFrequency = -6.f;
				tShakeEvent.tWaveX.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveY.fAmplitude = 0.1f;
				tShakeEvent.tWaveY.fFrequency = 6.f;
				tShakeEvent.tWaveY.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveZ.fAmplitude = 0.5f;
				tShakeEvent.tWaveZ.fFrequency = 1.f;
				tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
				tShakeEvent.fInnerRadius = 10.f;
				tShakeEvent.fOuterRadius = 20.f;
				tShakeEvent.fDistanceRate = 10.f;

				g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

				//무기앞에생기는이펙트
				m_bEffectCheck = true;
				static_cast<CBoss_Bastion_Judicator*>(m_pMonster)->OnWeaponEffect();
			}

			m_pMonster->Set_IsAttack(true);

			_float fDamage = 6.f;
			_uint iLevel = 3;
			m_pMonster->Set_AttackDesc_Damaga(fDamage);
			m_pMonster->Set_AttackDesc_Level(iLevel);
		}
		else
		{
			m_pMonster->Set_IsAttack(false);
		}

		m_pMonster->Set_AttackDesc_Dir(EAttackDir::Forward);
	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_LEGACY_H == m_pAnimator->Get_CurrentAnimNode())
	{
		cout << "LEGACY : " << iCurKeyFrameIndex << endl;

		if (10 <= iCurKeyFrameIndex && 60 > iCurKeyFrameIndex)
		{
			m_pMonster->Set_IsAttack(true);

			_float fDamage = 8.f;
			_uint iLevel = 4;
			m_pMonster->Set_AttackDesc_Damaga(fDamage);
			m_pMonster->Set_AttackDesc_Level(iLevel);

			if (35 <= iCurKeyFrameIndex && 38 >= iCurKeyFrameIndex && false == m_bEffectCheck)
			{
				CCameraShake::SHAKEEVENT tShakeEvent;
				tShakeEvent.fDuration = 1.f;
				tShakeEvent.fBlendInTime = 0.2f;
				tShakeEvent.fBlendOutTime = 0.5f;
				tShakeEvent.tWaveX.fAmplitude = 0.2f;
				tShakeEvent.tWaveX.fFrequency = -9.f;
				tShakeEvent.tWaveX.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveY.fAmplitude = 0.2f;
				tShakeEvent.tWaveY.fFrequency = 9.f;
				tShakeEvent.tWaveY.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveZ.fAmplitude = 0.5f;
				tShakeEvent.tWaveZ.fFrequency = 0.6f;
				tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
				tShakeEvent.fInnerRadius = 10.f;
				tShakeEvent.fOuterRadius = 20.f;
				tShakeEvent.fDistanceRate = 20.f;

				g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

				//무기앞에생기는이펙트
				m_bEffectCheck = true;
				static_cast<CBoss_Bastion_Judicator*>(m_pMonster)->OnWeaponEffect();
			}
		}
		else 
			m_pMonster->Set_IsAttack(false);

		m_pMonster->Set_AttackDesc_Dir(EAttackDir::Forward);
	}

	else if (CBoss_Bastion_Judicator::M_BossAnimState::SPRINT_ATTACK_H == m_pAnimator->Get_CurrentAnimNode())
	{
		cout << "SPRINT : " << iCurKeyFrameIndex << endl;
		if (15 < iCurKeyFrameIndex && 27 > iCurKeyFrameIndex)
		{
			if (19 <= iCurKeyFrameIndex && 20 >= iCurKeyFrameIndex && false == m_bEffectCheck)
			{
				CCameraShake::SHAKEEVENT tShakeEvent;
				tShakeEvent.fDuration = 1.f;
				tShakeEvent.fBlendInTime = 0.2f;
				tShakeEvent.fBlendOutTime = 0.5f;
				tShakeEvent.tWaveX.fAmplitude = 0.2f;
				tShakeEvent.tWaveX.fFrequency = -9.f;
				tShakeEvent.tWaveX.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveY.fAmplitude = 0.2f;
				tShakeEvent.tWaveY.fFrequency = 9.f;
				tShakeEvent.tWaveY.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveZ.fAmplitude = 0.5f;
				tShakeEvent.tWaveZ.fFrequency = 0.6f;
				tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
				tShakeEvent.fInnerRadius = 10.f;
				tShakeEvent.fOuterRadius = 20.f;
				tShakeEvent.fDistanceRate = 20.f;

				g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

				//무기앞에생기는이펙트
				m_bEffectCheck = true;
				static_cast<CBoss_Bastion_Judicator*>(m_pMonster)->OnWeaponEffect();
			}

			m_pMonster->Set_IsAttack(true);

			_float fDamage = 6.f;
			_uint iLevel = 3;
			m_pMonster->Set_AttackDesc_Damaga(fDamage);
			m_pMonster->Set_AttackDesc_Level(iLevel);
		}
		else
			m_pMonster->Set_IsAttack(false);

		m_pMonster->Set_AttackDesc_Dir(EAttackDir::Forward);
	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_R1_H == m_pAnimator->Get_CurrentAnimNode())
	{
		cout << "R1_H : " << iCurKeyFrameIndex << endl;

		if (26 < iCurKeyFrameIndex && 40 > iCurKeyFrameIndex)
		{
			if (35 <= iCurKeyFrameIndex && 36 >= iCurKeyFrameIndex && false == m_bEffectCheck)
			{
				CCameraShake::SHAKEEVENT tShakeEvent;
				tShakeEvent.fDuration = 1.f;
				tShakeEvent.fBlendInTime = 0.2f;
				tShakeEvent.fBlendOutTime = 0.5f;
				tShakeEvent.tWaveX.fAmplitude = 0.2f;
				tShakeEvent.tWaveX.fFrequency = -9.f;
				tShakeEvent.tWaveX.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveY.fAmplitude = 0.2f;
				tShakeEvent.tWaveY.fFrequency = 9.f;
				tShakeEvent.tWaveY.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveZ.fAmplitude = 0.5f;
				tShakeEvent.tWaveZ.fFrequency = 0.6f;
				tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
				tShakeEvent.fInnerRadius = 10.f;
				tShakeEvent.fOuterRadius = 20.f;
				tShakeEvent.fDistanceRate = 20.f;

				g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

				//무기앞에생기는이펙트
				m_bEffectCheck = true;
				static_cast<CBoss_Bastion_Judicator*>(m_pMonster)->OnWeaponEffect();
			}

			m_pMonster->Set_IsAttack(true);

			_float fDamage = 6.f;
			_uint iLevel = 3;
			m_pMonster->Set_AttackDesc_Damaga(fDamage);
			m_pMonster->Set_AttackDesc_Level(iLevel);
		}
		else
			m_pMonster->Set_IsAttack(false);

		m_pMonster->Set_AttackDesc_Dir(EAttackDir::Forward);
	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_R1 == m_pAnimator->Get_CurrentAnimNode())
	{
		cout << "R1 : " << iCurKeyFrameIndex << endl;

		if (40 <= iCurKeyFrameIndex && 125 >= iCurKeyFrameIndex)
		{
			m_pMonster->Set_IsAttack(true);

			_float fDamage = 6.f;
			_uint iLevel = 3;
			m_pMonster->Set_AttackDesc_Damaga(fDamage);
			m_pMonster->Set_AttackDesc_Level(iLevel);

			if (40 <= iCurKeyFrameIndex && 60 >= iCurKeyFrameIndex)
			{
				if (false == m_bSideAttack1 && 60 <= iCurKeyFrameIndex)
				{
					m_bSideAttack1 = true;

					CCameraShake::SHAKEEVENT tShakeEvent;
					tShakeEvent.fDuration = 0.6f;
					tShakeEvent.fBlendInTime = 0.2f;
					tShakeEvent.fBlendOutTime = 0.4f;
					tShakeEvent.tWaveX.fAmplitude = 0.4f;
					tShakeEvent.tWaveX.fFrequency = 7.f;
					tShakeEvent.tWaveY.fAmplitude = -0.1f;
					tShakeEvent.tWaveY.fFrequency = 0.5f;
					tShakeEvent.tWaveZ.fAdditionalOffset = 0.6f;
					tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
					tShakeEvent.fInnerRadius = 10.f;
					tShakeEvent.fOuterRadius = 20.f;
					tShakeEvent.fDistanceRate = 30.f;

					g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));
				}
				m_pMonster->Set_AttackDesc_Dir(EAttackDir::Right);
			}
			if (80 <= iCurKeyFrameIndex && 120 >= iCurKeyFrameIndex)
			{
				if (false == m_bSideAttack2 && 110 <= iCurKeyFrameIndex)
				{
					m_bSideAttack2 = true;

					CCameraShake::SHAKEEVENT tShakeEvent;
					tShakeEvent.fDuration = 0.6f;
					tShakeEvent.fBlendInTime = 0.2f;
					tShakeEvent.fBlendOutTime = 0.4f;
					tShakeEvent.tWaveX.fAmplitude = 0.4f;
					tShakeEvent.tWaveX.fFrequency = 7.f;
					tShakeEvent.tWaveY.fAmplitude = -0.1f;
					tShakeEvent.tWaveY.fFrequency = 0.5f;
					tShakeEvent.tWaveZ.fAdditionalOffset = 0.6f;
					tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
					tShakeEvent.fInnerRadius = 10.f;
					tShakeEvent.fOuterRadius = 20.f;
					tShakeEvent.fDistanceRate = 30.f;

					g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));
				}

				m_pMonster->Set_AttackDesc_Dir(EAttackDir::Left);
			}
		}

		else
			m_pMonster->Set_IsAttack(false);

	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_S1 == m_pAnimator->Get_CurrentAnimNode())
	{
		cout << "S1 : " << iCurKeyFrameIndex << endl;

		//림라이트
		if (m_rimcheck == true)
		{
			m_pMonster->RimlightCheck(true, _float3(0.1f, 0, 0), 6.f);
			m_rimcheck = false;
		}

		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

		if (60 < iCurKeyFrameIndex && 90 > iCurKeyFrameIndex)
		{
			if (74 < iCurKeyFrameIndex && 76 > iCurKeyFrameIndex && false == m_bEffectCheck2) //변수다시만들어서넣어주기 
			{
				CCameraShake::SHAKEEVENT tShakeEvent;
				tShakeEvent.fDuration = 1.f;
				tShakeEvent.fBlendInTime = 0.2f;
				tShakeEvent.fBlendOutTime = 0.5f;
				tShakeEvent.tWaveX.fAmplitude = 0.2f;
				tShakeEvent.tWaveX.fFrequency = -9.f;
				tShakeEvent.tWaveX.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveY.fAmplitude = 0.2f;
				tShakeEvent.tWaveY.fFrequency = 9.f;
				tShakeEvent.tWaveY.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveZ.fAmplitude = 0.5f;
				tShakeEvent.tWaveZ.fFrequency = 0.6f;
				tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
				tShakeEvent.fInnerRadius = 10.f;
				tShakeEvent.fOuterRadius = 20.f;
				tShakeEvent.fDistanceRate = 20.f;

				g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

				//무기앞에생기는이펙트
				m_bEffectCheck2 = true;
				static_cast<CBoss_Bastion_Judicator*>(m_pMonster)->OnWeaponEffect();
			}

			m_pMonster->Set_IsAttack(true);

			_float fDamage = 6.f;
			_uint iLevel = 3;
			m_pMonster->Set_AttackDesc_Damaga(fDamage);
			m_pMonster->Set_AttackDesc_Level(iLevel);
		}
		else if (110 < iCurKeyFrameIndex && 140 > iCurKeyFrameIndex)
		{
			if (123 <= iCurKeyFrameIndex && 124 >= iCurKeyFrameIndex && false == m_bEffectCheck)
			{
				CCameraShake::SHAKEEVENT tShakeEvent;
				tShakeEvent.fDuration = 0.5f;
				tShakeEvent.fBlendInTime = 0.2f;
				tShakeEvent.fBlendOutTime = 0.5f;
				tShakeEvent.tWaveX.fAmplitude = 0.2f;
				tShakeEvent.tWaveX.fFrequency = -9.f;
				tShakeEvent.tWaveX.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveY.fAmplitude = 0.2f;
				tShakeEvent.tWaveY.fFrequency = 9.f;
				tShakeEvent.tWaveY.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveZ.fAmplitude = 0.5f;
				tShakeEvent.tWaveZ.fFrequency = 0.6f;
				tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
				tShakeEvent.fInnerRadius = 10.f;
				tShakeEvent.fOuterRadius = 20.f;
				tShakeEvent.fDistanceRate = 20.f;

				g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

				//무기앞에생기는이펙트
				m_bEffectCheck = true;
				static_cast<CBoss_Bastion_Judicator*>(m_pMonster)->OnWeaponEffect();
			}

			m_pMonster->Set_IsAttack(true);

			_float fDamage = 7.f;
			_uint iLevel = 4;
			m_pMonster->Set_AttackDesc_Damaga(fDamage);
			m_pMonster->Set_AttackDesc_Level(iLevel);
		}
		else
		{
			m_bEffectCheck = false;
			m_pMonster->Set_IsAttack(false);
		}

		m_pMonster->Set_AttackDesc_Dir(EAttackDir::Forward);
	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_S2 == m_pAnimator->Get_CurrentAnimNode())
	{
		cout << "S2 : " << iCurKeyFrameIndex << endl;

		//림라이트
		if (m_rimcheck == true)
		{
			m_pMonster->RimlightCheck(true, _float3(0.1f, 0, 0), 5.f);
			m_rimcheck = false;
		}

		if (10 < iCurKeyFrameIndex && 15 > iCurKeyFrameIndex && !m_bEnergy)
		{
			m_bEnergy = true;
			m_pMonster->Active_Effect((_uint)EFFECT::ENERGY_MIDBOSS);
		}

		if (35 < iCurKeyFrameIndex && 55 > iCurKeyFrameIndex)
		{
			OVERLAPDESC tOverlapDesc;
			tOverlapDesc.geometry = PxSphereGeometry(5.f);
			XMStoreFloat3(&tOverlapDesc.vOrigin, m_pTransform->Get_State(CTransform::STATE_POSITION));
			CGameObject* pHitObject = nullptr;
			tOverlapDesc.ppOutHitObject = &pHitObject;
			tOverlapDesc.filterData.flags = PxQueryFlag::eDYNAMIC;
			tOverlapDesc.layerMask = (1 << (_uint)ELayer::Player);
			if (g_pGameInstance->Overlap(tOverlapDesc))
			{
				_uint iSize = (_uint)tOverlapDesc.vecHitObjects.size();
				for (_uint i = 0; i < iSize; ++i)
				{
					CActor* pActor = static_cast<CActor*>(tOverlapDesc.vecHitObjects[i]);
					_uint iTag = tOverlapDesc.vecHitObjects[i]->getTag();
					switch (iTag)
					{
					case (_uint)GAMEOBJECT::PLAYER:
						ATTACKDESC tAttackDesc = m_pMonster->Get_AttackDesc();
						tAttackDesc.fDamage = 7.f;
						tAttackDesc.iLevel = 4;

						pActor->Hit(tAttackDesc);
						break;
					}
				}
			}

			if (37 < iCurKeyFrameIndex && 40 > iCurKeyFrameIndex && !m_bExplosion)
			{
				m_bExplosion = true;
				static_cast<CBoss_Bastion_Judicator*>(m_pMonster)->OnEff_MeshExplosion(true);

				CCameraShake::SHAKEEVENT tShakeEvent;
				tShakeEvent.fDuration = 0.5f;
				tShakeEvent.fBlendInTime = 0.2f;
				tShakeEvent.fBlendOutTime = 0.5f;
				tShakeEvent.tWaveX.fAmplitude = 0.2f;
				tShakeEvent.tWaveX.fFrequency = -9.f;
				tShakeEvent.tWaveX.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveY.fAmplitude = 0.2f;
				tShakeEvent.tWaveY.fFrequency = 9.f;
				tShakeEvent.tWaveY.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveZ.fAmplitude = 0.5f;
				tShakeEvent.tWaveZ.fFrequency = 0.6f;
				tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
				tShakeEvent.fInnerRadius = 10.f;
				tShakeEvent.fOuterRadius = 20.f;
				tShakeEvent.fDistanceRate = 20.f;

				g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));
			}
		}
		else
			m_pMonster->Set_IsAttack(false);
	}


	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Turn");
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

	m_bEffectCheck = false; //이펙트다시켜야됨 
	m_bAttackTypeFar = false;
	m_bEffectCheck2 = false;
	m_bEnergy = false;
	m_bExplosion = false;
	m_bSideAttack1 = false;
	m_bSideAttack2 = false;

	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	if (5.f > fDistToPlayer )
	{
		_int iRandom = rand() % 4;

		while (m_iPreState_1 == iRandom)
		{
			iRandom = rand() % 4;
		}

		if (m_iPreState_1 != iRandom)
		{
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
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
				break;
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

	CBoss_Bastion_Judicator* m_pMonster = (CBoss_Bastion_Judicator*)m_pStateController->Get_GameObject();

	m_bEffectCheck = false; //이펙트다시켜야됨 
	m_bAttackTypeFar = false;
	m_bEffectCheck2 = false;
	m_bEnergy = false;
	m_bExplosion = false;
	m_bSideAttack1 = false;
	m_bSideAttack2 = false;

	m_bAttackR1_H = false;
	m_bAttackR1 = false;
	m_bAttackS1 = false;
	m_bAttackS2 = false;
	m_bAttackJog = false;
	m_bAttackSprint = false;
	m_bAttackLegacy = false;
	m_bAttackLegacy_2 = false;
	m_rimcheck = true;

	return S_OK;
}

void CMidBoss_Attack::Look_Player(void)
{
}

void CMidBoss_Attack::Play_Sound(void)
{
	_vector Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_R1_H == m_pAnimator->Get_CurrentAnimNode() && false == m_bAttackR1_H)
	{
		if (34 == iCurKeyFrameIndex)
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXZ, Pos);

			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
			g_pGameInstance->Play_Shot(L"MidBoss_Roar_1", CSoundMgr::CHANNELID::MidBoss);
			g_pGameInstance->BlendSound(L"MidBoss_Swing_1", L"MidBoss_Attack_End_1", CSoundMgr::CHANNELID::MidBoss_Attack_1, CSoundMgr::CHANNELID::MidBoss_Attack_2);
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_2, 0.5f);
			m_bAttackR1_H = true;
		}
	}

	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_R1 == m_pAnimator->Get_CurrentAnimNode())
	{
		if (false == m_bAttackR1)
		{
			if (m_iAttackR1_1 == iCurKeyFrameIndex)
			{
				g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXYZ, Pos);

				g_pGameInstance->BlendSound(L"MidBoss_Roar_1", L"MidBoss_Swing_1", CSoundMgr::CHANNELID::MidBoss, CSoundMgr::CHANNELID::MidBoss_Attack_2);
				g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_2, 0.5f);
				m_bAttackR1 = true;
			}
		}
		if (m_bAttackR1)
		{
			if (m_iAttackR1_2 == iCurKeyFrameIndex)
			{
				g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXYZ, Pos);

				g_pGameInstance->BlendSound(L"MidBoss_Swing_2", L"MidBoss_Attack_End_2", CSoundMgr::CHANNELID::MidBoss_Attack_1, CSoundMgr::CHANNELID::MidBoss_Attack_2);
				g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_2, 0.5f);
			}
		}
	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_S1 == m_pAnimator->Get_CurrentAnimNode())
	{
		if (false == m_bAttackS1)
		{
			if (m_iAttackS1_1 == iCurKeyFrameIndex)
			{
				g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXZ, Pos);

				g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
				g_pGameInstance->Play_Shot(L"MidBoss_Roar_1", CSoundMgr::CHANNELID::MidBoss);
				g_pGameInstance->BlendSound(L"MidBoss_Swing_1", L"MidBoss_Attack_End_2", CSoundMgr::CHANNELID::MidBoss, CSoundMgr::CHANNELID::MidBoss_Attack_2);
				g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_2, 0.5f);
				m_bAttackS1 = true;
			}
		}
		if (m_bAttackS1)
		{
			if (m_iAttackS1_2 == iCurKeyFrameIndex)
			{
				g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXYZ, Pos);

				g_pGameInstance->BlendSound(L"MidBoss_Swing_2", L"MidBoss_Attack_End_3", CSoundMgr::CHANNELID::MidBoss_Attack_1, CSoundMgr::CHANNELID::MidBoss_Attack_2);
				g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_2, 0.5f);
			}
		}
	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_S2 == m_pAnimator->Get_CurrentAnimNode() && false == m_bAttackS2)
	{
		if (m_iAttackS2_1 == iCurKeyFrameIndex)
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossY, Pos);

			g_pGameInstance->BlendSound(L"MidBoss_Roar", L"MidBoss_Explode_End", CSoundMgr::CHANNELID::MidBoss_Attack_1, CSoundMgr::CHANNELID::MidBoss_Attack_2);
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_2, 0.5f);
			m_bAttackS2 = true;
		}
	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_JOG_H == m_pAnimator->Get_CurrentAnimNode() && false == m_bAttackJog)
	{
		if (m_iAttackJog == iCurKeyFrameIndex)
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossY, Pos);

			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss_Attack_1);
			g_pGameInstance->Play_Shot(L"MidBoss_Attack_Down", CSoundMgr::CHANNELID::MidBoss_Attack_1);
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_1, 0.5f);
			m_bAttackJog = true;
		}
	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::SPRINT_ATTACK_H == m_pAnimator->Get_CurrentAnimNode() && false == m_bAttackSprint)
	{
		if (m_iAttackSprint == iCurKeyFrameIndex)
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossY, Pos);

			g_pGameInstance->BlendSound(L"MidBoss_Jump", L"MidBoss_Attack_Down", CSoundMgr::CHANNELID::MidBoss, CSoundMgr::CHANNELID::MidBoss_Attack_1);
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_1, 0.5f);
			m_bAttackSprint = true;
		}
	}
	else if (CBoss_Bastion_Judicator::M_BossAnimState::ATTACK_LEGACY_H == m_pAnimator->Get_CurrentAnimNode())
	{
		if (false == m_bAttackLegacy)
		{
			if (m_iAttackLegacy_1 == iCurKeyFrameIndex)
			{
				g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss_Attack_1);
				g_pGameInstance->Play_Shot(L"MidBoss_Swing_1", CSoundMgr::CHANNELID::MidBoss_Attack_1);
				g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_1, 0.5f);
				m_bAttackLegacy = true;
			}
		}

		if (m_bAttackLegacy && false == m_bAttackLegacy_2)
		{
			if (m_iAttackLegacy_2 == iCurKeyFrameIndex)
			{
				g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
				g_pGameInstance->Play_Shot(L"MidBoss_Roar_3", CSoundMgr::CHANNELID::MidBoss);
				g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss_Attack_2);
				g_pGameInstance->Play_Shot(L"MidBoss_Swing_2", CSoundMgr::CHANNELID::MidBoss_Attack_2);
				g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_2, 0.5f);
				m_bAttackLegacy_2 = true;
			}
		}

		if (m_bAttackLegacy_2)
		{
			if (m_iAttackLegacy_3 == iCurKeyFrameIndex)
			{
				g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXYZ, Pos);

				g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
				g_pGameInstance->Play_Shot(L"MidBoss_Roar_1", CSoundMgr::CHANNELID::MidBoss);
				g_pGameInstance->BlendSound(L"MidBoss_Swing_1", L"MidBoss_Attack_End_2", CSoundMgr::CHANNELID::MidBoss, CSoundMgr::CHANNELID::MidBoss_Attack_3);
				g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss_Attack_2, 0.5f);
			}
		}
	}
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
