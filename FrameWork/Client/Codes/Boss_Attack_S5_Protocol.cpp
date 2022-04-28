#include "pch.h"
#include "Boss_Attack_S5_Protocol.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Attack_S5_Protocol::CBoss_Attack_S5_Protocol(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Attack_S5_Protocol::CBoss_Attack_S5_Protocol(const CBoss_Attack_S5_Protocol& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Attack_S5_Protocol::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Attack_S5_Protocol::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	//Play_Sound();
	m_pAnimator->Tick(TimeDelta);


	//자연스럽게 회전하면서 플레이어쪽으로뛰려고..
	_vector vBossPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vBossPos - g_pObserver->Get_PlayerPos();
	_vector vBossToPlayerLook = XMVector3Normalize(vDist);

	_vector vMyLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector vMyUp = m_pTransform->Get_State(CTransform::STATE_UP);

	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	_vector vecDot = XMVector3Dot(vMyLook, vBossToPlayerLook);
	_float CheckFWD = XMVectorGetX(vecDot); //음수 = 플레이어가 앞에있다 

	vDist = XMVector3Normalize(XMVectorSetY(vDist, 0.f));
	vMyLook = XMVector3Normalize(XMVectorSetY(vMyLook, 0.f));

	_vector vAngle = XMVector3AngleBetweenVectors(vMyLook, vDist);
	_float fRadian = 0.f;
	XMStoreFloat(&fRadian, vAngle);
	_vector vCross = XMVector3Cross(vMyLook, vDist);


	if (0 >= CheckFWD)
	{
		if (0.f > XMVectorGetY(vCross))
		{
			fRadian = XMConvertToRadians(XMConvertToRadians(180.f) - fRadian);

			CheckFWD = 1.f - CheckFWD;
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian * CheckFWD);
			//cout << "1 : 여기" << endl;
		}
		else if (0.f <= XMVectorGetY(vCross))
		{
			fRadian = XMConvertToRadians(XMConvertToRadians(180.f) - fRadian);

			//CheckFWD = CheckFWD;
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian * CheckFWD * 2.f);
			//cout << "1 : 아님" << endl;
		}
	}
	else if (0 < CheckFWD)
	{
		if (0.f > XMVectorGetY(vCross))
		{
			//fRadian = XMConvertToRadians(XMConvertToRadians(180.f) - fRadian);
			//
			//CheckFWD = 1.f - CheckFWD;
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian * CheckFWD);
			//cout << "2 : 여기" << endl;
		}
		else if (0.f <= XMVectorGetY(vCross))
		{
			fRadian = XMConvertToRadians(XMConvertToRadians(180.f) - fRadian);

			//CheckFWD = CheckFWD;
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian * CheckFWD);
			//cout << "2 : 아님" << endl;
		}
	}

	//점프하기직전까지 플레이어거리판단
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	if (40 <= iCurKeyFrameIndex && 44 >= iCurKeyFrameIndex && false == m_bFirstCheckDist)
	{
		m_pAnimator->Get_AnimController()->Set_MoveSpeed(fDistToPlayer * (1.f + (fDistToPlayer * 0.017f)));
		m_bFirstCheckDist = true;
	}


	//어택 체크
	_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector svRight = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT));

	if (CBoss_Solaris::M_BossAnimState::ATTACK_S5_SKEWER_PROTOCOL== m_pAnimator->Get_CurrentAnimNode())
	{	
		if (66 <= iCurKeyFrameIndex && 80 >= iCurKeyFrameIndex)
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

			if (66 < iCurKeyFrameIndex && 69 > iCurKeyFrameIndex && false == m_bShakeCheck)
			{
				//쉐이킹 & 이펙트
				CCameraShake::SHAKEEVENT tShakeEvent;
				tShakeEvent.fDuration = 0.4f;
				tShakeEvent.fBlendInTime = 0.1f;
				tShakeEvent.fBlendOutTime = 0.3f;
				tShakeEvent.tWaveX.fAmplitude = -0.4f;
				tShakeEvent.tWaveX.fFrequency = 12.f;
				tShakeEvent.tWaveY.fAmplitude = 0.4f;
				tShakeEvent.tWaveY.fFrequency = 4.f;
				tShakeEvent.tWaveZ.fAdditionalOffset = 0.2f;
				tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
				tShakeEvent.fInnerRadius = 20.f;
				tShakeEvent.fOuterRadius = 40.f;
				tShakeEvent.fDistanceRate = 4.f;

				g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

				m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND, svLook * 5.5f + svRight * 2.1f);
				m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE, XMVectorSet(0.f, -0.1f, 0.f, 0.f));
				m_pMonster->Active_Effect((_uint)EFFECT::EXPLOSION_ROCK_UP, svLook * 5.5f + svRight * 2.1f);
				m_pMonster->Active_Effect((_uint)EFFECT::ATTACK_GROUND, XMVectorSet(0.f, -0.1f, 0.f, 0.f));

				m_bShakeCheck = true;
			}

			m_pMonster->Set_IsAttack(true);

			_float fDamage = 6.f;
			_uint iLevel = 3;
			m_pMonster->Set_AttackDesc_Damaga(fDamage);
			m_pMonster->Set_AttackDesc_Level(iLevel);
		}

		if (151 < iCurKeyFrameIndex && 154 > iCurKeyFrameIndex && false == m_bEffectCheck)
		{
			//쉐이킹 & 이펙트
			CCameraShake::SHAKEEVENT tShakeEvent;
			tShakeEvent.fDuration = 0.3f;
			tShakeEvent.fBlendInTime = 0.2f;
			tShakeEvent.fBlendOutTime = 0.3f;
			tShakeEvent.tWaveX.fAmplitude = -0.1f;
			tShakeEvent.tWaveX.fFrequency = 12.f;
			tShakeEvent.tWaveY.fAmplitude = 0.1f;
			tShakeEvent.tWaveY.fFrequency = 4.f;
			tShakeEvent.tWaveZ.fAdditionalOffset = 0.2f;
			tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
			tShakeEvent.fInnerRadius = 20.f;
			tShakeEvent.fOuterRadius = 40.f;
			tShakeEvent.fDistanceRate = 4.f;

			g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

			//m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND, svLook * 5.5f + svRight * 2.1f);
			m_pMonster->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE, XMVectorSet(0.f, -0.1f, 0.f, 0.f));
			m_pMonster->Active_Effect((_uint)EFFECT::EXPLOSION_ROCK_UP, svLook * 5.5f + svRight * 2.1f);

			m_bEffectCheck = true;
		}
		else
		{
			m_pMonster->Set_IsAttack(false);
		}
	
		m_pMonster->Set_AttackDesc_Dir(EAttackDir::Forward);
	}

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		if (30.f <= fDistToPlayer)
		{
			cout << "protocol -> walk" << endl;

			m_pStateController->Change_State(L"Walk_Front");
		}
		else if (30.f > fDistToPlayer )
		{
			cout << "Protocol -> turn" << endl;

			m_pStateController->Change_State(L"Turn");
		}
	}

	return _int();
}

_int CBoss_Attack_S5_Protocol::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Attack_S5_Protocol::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Attack_S5_Protocol::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	cout << "Attack_S5_Protocol" << endl;

	//m_pTransform->Face_Target()
	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(false);

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_pAnimator->Get_AnimController()->Set_MoveSpeed(fDistToPlayer * (1.f + (fDistToPlayer * 0.016f)));
 	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::ATTACK_S5_SKEWER_PROTOCOL);


	m_bEffectCheck = false;
	m_bFirstCheckDist = false;
	m_bShakeCheck = false;

	return S_OK;
}

HRESULT CBoss_Attack_S5_Protocol::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	CBoss_Solaris* m_pMonster = (CBoss_Solaris*)m_pStateController->Get_GameObject();
	m_pMonster->RimlightCheck(false);

	m_bEffectCheck = false;
	m_bFirstCheckDist = false;
	m_bShakeCheck = false;

	return S_OK;
}

void CBoss_Attack_S5_Protocol::Look_Player(void)
{
}

void CBoss_Attack_S5_Protocol::Play_Sound(void)
{
	_vector Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	//if (m_iAttackR1_H == iCurKeyFrameIndex)
	//{
	//	g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXZ, Pos);
	//
	//	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->Play_Shot(L"MidBoss_Roar_1", CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->BlendSound(L"MidBoss_Swing_1", L"Boss_Attack_S5_Protocol_End_1", CSoundMgr::CHANNELID::Boss_Attack_S5_Protocol_1, CSoundMgr::CHANNELID::Boss_Attack_S5_Protocol_2);
	//	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Boss_Attack_S5_Protocol_2, 0.5f);
	//	m_bAttackR1_H = true;
	//}
}

CBoss_Attack_S5_Protocol* CBoss_Attack_S5_Protocol::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Attack_S5_Protocol* pInstance = new CBoss_Attack_S5_Protocol(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Attack_S5_Protocol Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Attack_S5_Protocol::Free()
{
	__super::Free();
}
