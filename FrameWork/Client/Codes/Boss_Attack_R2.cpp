#include "pch.h"
#include "Boss_Attack_R2.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Attack_R2::CBoss_Attack_R2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Attack_R2::CBoss_Attack_R2(const CBoss_Attack_R2& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Attack_R2::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Attack_R2::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	//Play_Sound();

	m_pAnimator->Tick(TimeDelta);

	//어택 체크
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();
	 
	//cout << iCurKeyFrameIndex << endl;

	if (15 <= iCurKeyFrameIndex && 38 >= iCurKeyFrameIndex )
	{
		m_pAnimator->Get_AnimController()->Set_PlaySpeed(0.4f);
	}

	if (9 <= iCurKeyFrameIndex && 12 >= iCurKeyFrameIndex && false == m_bEffectCheck)
	{
		m_pMonster->Active_Effect((_uint)EFFECT::ENERGY);
		m_bEffectCheck = true;
	}

	if (39 <= iCurKeyFrameIndex && 42 >= iCurKeyFrameIndex && false == m_bShakeCheck)
	{
		m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);

		//쉐이킹 & 이펙트 //이펙트한테서 생성해줘야할듯 
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 0.6f;
		tShakeEvent.fBlendInTime = 0.3f;
		tShakeEvent.fBlendOutTime = 0.5f;
		tShakeEvent.tWaveX.fAmplitude = 0.5f;
		tShakeEvent.tWaveX.fFrequency = 10.f;
		tShakeEvent.tWaveY.fAmplitude = 0.3f;
		tShakeEvent.tWaveY.fFrequency = 10.f;
		tShakeEvent.tWaveZ.fAdditionalOffset = 0.2f;
		tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
		tShakeEvent.fInnerRadius = 20.f;
		tShakeEvent.fOuterRadius = 30.f;
		tShakeEvent.fDistanceRate = 30.0f;

		g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));
		m_bShakeCheck = true;

		//이펙트위치지정
		_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
		_vector svRight = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT));

		m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE);
		m_pMonster->Active_Effect((_uint)EFFECT::EXPLOSION_ROCK_UP);
		static_cast<CBoss_Solaris*>(m_pMonster)->OnEff_MeshExplosion(true);


		m_pMonster->Set_IsAttack(true);

		_float fDamage = 6.f;
		_uint iLevel = 3;
		m_pMonster->Set_AttackDesc_Damaga(fDamage);
		m_pMonster->Set_AttackDesc_Level(iLevel);
		m_pMonster->Set_AttackDesc_Dir(EAttackDir::Forward);
	}

	//범위공격
	if (38 < iCurKeyFrameIndex && 50 > iCurKeyFrameIndex)
	{
		OVERLAPDESC tOverlapDesc;
		tOverlapDesc.geometry = PxSphereGeometry(7.f);
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
	}
	else
		m_pMonster->Set_IsAttack(false);

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		cout << "r2 -> turn" << endl;
		m_pStateController->Change_State(L"Turn");
	}

	return _int();
}

_int CBoss_Attack_R2::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Attack_R2::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Attack_R2::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	cout << "Attack_R2" << endl;

	m_bShakeCheck = false;
	m_bEffectCheck = false;

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(false);

	//림라이트
	m_pMonster->RimlightCheck(true, _float3(0.1f, 0, 0), 6.f);

	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);

 	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::ATTACK_R2);

	return S_OK;
}

HRESULT CBoss_Attack_R2::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_bEffectCheck = false;

	return S_OK;
}

void CBoss_Attack_R2::Look_Player(void)
{
}

void CBoss_Attack_R2::Play_Sound(void)
{
	_vector Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	//if (m_iAttackR1_H == iCurKeyFrameIndex)
	//{
	//	g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXZ, Pos);
	//
	//	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->Play_Shot(L"MidBoss_Roar_1", CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->BlendSound(L"MidBoss_Swing_1", L"Boss_Attack_R2_End_1", CSoundMgr::CHANNELID::Boss_Attack_R2_1, CSoundMgr::CHANNELID::Boss_Attack_R2_2);
	//	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Boss_Attack_R2_2, 0.5f);
	//	m_bAttackR1_H = true;
	//}
}

CBoss_Attack_R2* CBoss_Attack_R2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Attack_R2* pInstance = new CBoss_Attack_R2(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Attack_R2 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Attack_R2::Free()
{
	__super::Free();
}
