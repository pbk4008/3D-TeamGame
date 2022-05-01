#include "pch.h"
#include "Boss_Attack_R1.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Attack_R1::CBoss_Attack_R1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Attack_R1::CBoss_Attack_R1(const CBoss_Attack_R1& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Attack_R1::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Attack_R1::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	//Play_Sound();

	m_pAnimator->Tick(TimeDelta);

	//¾îÅÃ Ã¼Å©
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();
	 
	cout << iCurKeyFrameIndex << endl;

	//m_pMonster->RimlightCheck(true, _float3(1, 0, 0));

	_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector svRight = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT));

	_vector BossPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_vector BossLook = XMVector3Normalize(m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	_vector BossRight = XMVector3Normalize(m_pMonster->Get_Transform()->Get_State(CTransform::STATE_RIGHT));

#pragma region ±¸°£º°·Î³ª´®
	//if (50 <= iCurKeyFrameIndex && 67 >= iCurKeyFrameIndex)
	//{
	//	if (54 < iCurKeyFrameIndex && 57 > iCurKeyFrameIndex && false == m_bShakeCheckLeft)
	//	{
	//		//½¦ÀÌÅ· & ÀÌÆåÆ® //ÀÌÆåÆ®ÇÑÅ×¼­ »ý¼ºÇØÁà¾ßÇÒµí 
	//		CCameraShake::SHAKEEVENT tShakeEvent;
	//		tShakeEvent.fDuration = 0.3f;
	//		tShakeEvent.fBlendInTime = 0.1f;
	//		tShakeEvent.fBlendOutTime = 0.2f;
	//		tShakeEvent.tWaveX.fAmplitude = 0.3f;
	//		tShakeEvent.tWaveX.fFrequency = 7.f;
	//		tShakeEvent.tWaveY.fAmplitude = -0.1f;
	//		tShakeEvent.tWaveY.fFrequency = 0.3f;
	//		tShakeEvent.tWaveZ.fAdditionalOffset = 0.6f;
	//		tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
	//		tShakeEvent.fInnerRadius = 10.f;
	//		tShakeEvent.fOuterRadius = 20.f;
	//		tShakeEvent.fDistanceRate = 20.f;

	//		g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));
	//		_matrix matPos = m_pTransform->Get_WorldMatrix();
	//		matPos.r[3] = XMVectorSetY(matPos.r[3], XMVectorGetY(matPos.r[3]) + 2.f);
	//		m_pMonster->Active_Effect_Target((_uint)EFFECT::EXPLOSION_ROCK_LEFT, matPos);

	//		m_bShakeCheckLeft = true;
	//	}
	//	

	//	m_pMonster->Set_IsAttack(true);

	//	_float fDamage = 6.f;
	//	_uint iLevel = 3;
	//	m_pMonster->Set_AttackDesc_Damaga(fDamage);
	//	m_pMonster->Set_AttackDesc_Level(iLevel);
	//	m_pMonster->Set_AttackDesc_Dir(EAttackDir::Left);
	//}

	//if (114 <= iCurKeyFrameIndex && 135 >= iCurKeyFrameIndex)
	//{
	//	if (117 < iCurKeyFrameIndex && 119 > iCurKeyFrameIndex && false == m_bShakeCheckRight)
	//	{
	//		//½¦ÀÌÅ· & ÀÌÆåÆ® //ÀÌÆåÆ®ÇÑÅ×¼­ »ý¼ºÇØÁà¾ßÇÒµí 
	//		CCameraShake::SHAKEEVENT tShakeEvent;
	//		tShakeEvent.fDuration = 0.3f;
	//		tShakeEvent.fBlendInTime = 0.1f;
	//		tShakeEvent.fBlendOutTime = 0.2f;
	//		tShakeEvent.tWaveX.fAmplitude = 0.3f;
	//		tShakeEvent.tWaveX.fFrequency = 7.f;
	//		tShakeEvent.tWaveY.fAmplitude = -0.1f;
	//		tShakeEvent.tWaveY.fFrequency = 0.3f;
	//		tShakeEvent.tWaveZ.fAdditionalOffset = 0.6f;
	//		tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
	//		tShakeEvent.fInnerRadius = 10.f;
	//		tShakeEvent.fOuterRadius = 20.f;
	//		tShakeEvent.fDistanceRate = 20.f;

	//		g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

	//		_matrix matPos = m_pTransform->Get_WorldMatrix();
	//		matPos.r[3] = XMVectorSetY(matPos.r[3], XMVectorGetY(matPos.r[3]) + 2.f);
	//		m_pMonster->Active_Effect_Target((_uint)EFFECT::EXPLOSION_ROCK_RIGHT, matPos);

	//		m_bShakeCheckRight = true;
	//	}

	//	m_pMonster->Set_IsAttack(true);

	//	_float fDamage = 6.f;
	//	_uint iLevel = 3;
	//	m_pMonster->Set_AttackDesc_Damaga(fDamage);
	//	m_pMonster->Set_AttackDesc_Level(iLevel);
	//	m_pMonster->Set_AttackDesc_Dir(EAttackDir::Right);
	//}
#pragma endregion

	if (50 <= iCurKeyFrameIndex && 135 >= iCurKeyFrameIndex)
	{
		if (54 < iCurKeyFrameIndex && 57 > iCurKeyFrameIndex && false == m_bShakeCheckLeft)
		{
			//½¦ÀÌÅ· & ÀÌÆåÆ® //ÀÌÆåÆ®ÇÑÅ×¼­ »ý¼ºÇØÁà¾ßÇÒµí 
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

			//Rock
			_matrix matPos = m_pTransform->Get_WorldMatrix();
			matPos.r[3] = XMVectorSetY(matPos.r[3], XMVectorGetY(matPos.r[3]) + 2.f);
			m_pMonster->Active_Effect_Target((_uint)EFFECT::EXPLOSION_ROCK_LEFT, matPos);

			//Smoke
			_matrix mat = m_pMonster->Get_Transform()->Get_WorldMatrix();
			mat = XMMatrixRotationY(XMConvertToRadians(45.f))
				* XMMatrixTranslation(XMVectorGetX(BossRight) * -1.f, 0.f, 0.f) * mat;
			m_pMonster->Active_Effect_Target((_uint)EFFECT::BOSS_SIDE_ATTACK_SMOKE, mat);
			m_pMonster->Active_Effect_Target((_uint)EFFECT::BOSS_SIDE_ATTACK, mat);

			m_bShakeCheckLeft = true;
			m_pMonster->Set_AttackDesc_Dir(EAttackDir::Left);

		}

		if (117 < iCurKeyFrameIndex && 119 > iCurKeyFrameIndex && false == m_bShakeCheckRight)
		{
			//½¦ÀÌÅ· & ÀÌÆåÆ® //ÀÌÆåÆ®ÇÑÅ×¼­ »ý¼ºÇØÁà¾ßÇÒµí 
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

			//Rock
			_matrix matPos = m_pTransform->Get_WorldMatrix();
			matPos.r[3] = XMVectorSetY(matPos.r[3], XMVectorGetY(matPos.r[3]) + 2.f);
			m_pMonster->Active_Effect_Target((_uint)EFFECT::EXPLOSION_ROCK_RIGHT, matPos);

			//Smoke
			_matrix mat = m_pMonster->Get_Transform()->Get_WorldMatrix();
			mat = XMMatrixRotationY(XMConvertToRadians(-70.f))
				* XMMatrixTranslation(XMVectorGetX(BossRight) * 1.f, 0.f, 0.f) * mat;
			m_pMonster->Active_Effect_Target((_uint)EFFECT::BOSS_SIDE_ATTACK_SMOKE, mat);
			m_pMonster->Active_Effect_Target((_uint)EFFECT::BOSS_SIDE_ATTACK, mat);

			m_pMonster->Set_AttackDesc_Dir(EAttackDir::Right);

			m_bShakeCheckRight = true;
		}


		if (50 <= iCurKeyFrameIndex && 65 >= iCurKeyFrameIndex)
		{
			m_pMonster->Set_IsAttack(true);
		}
		else if (115 <= iCurKeyFrameIndex && 130 >= iCurKeyFrameIndex)
		{
			m_pMonster->Set_IsAttack(true);
		}
		else
		{
			m_pMonster->Set_IsAttack(false);
		}

		_float fDamage = 6.f;
		_uint iLevel = 4;
		m_pMonster->Set_AttackDesc_Damaga(fDamage);
		m_pMonster->Set_AttackDesc_Level(iLevel);
	}

	else
	{
		m_pMonster->Set_IsAttack(false);
	}

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		cout << "r1 -> turn" << endl;

		m_pStateController->Change_State(L"Turn");
	}

	return _int();
}

_int CBoss_Attack_R1::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Attack_R1::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Attack_R1::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_bShakeCheckLeft = false;
	m_bShakeCheckRight = false;

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(false);

	cout << "Attack_R1" << endl;

	//¸²¶óÀÌÆ®
	m_pMonster->RimlightCheck(true, _float3(0.1f, 0, 0), 6.f);

	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);
	m_pAnimator->Get_AnimController()->Set_MoveSpeed(1.f);

 	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::ATTACK_R1);

	return S_OK;
}

HRESULT CBoss_Attack_R1::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_bShakeCheckLeft = false;
	m_bShakeCheckRight = false;
	return S_OK;
}

void CBoss_Attack_R1::Look_Player(void)
{
}

void CBoss_Attack_R1::Play_Sound(void)
{
	_vector Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	//if (m_iAttackR1_H == iCurKeyFrameIndex)
	//{
	//	g_pShakeManager->Shake(CShakeManager::ETemplate::MidBossXZ, Pos);
	//
	//	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->Play_Shot(L"MidBoss_Roar_1", CSoundMgr::CHANNELID::MidBoss);
	//	g_pGameInstance->BlendSound(L"MidBoss_Swing_1", L"Boss_Attack_R1_End_1", CSoundMgr::CHANNELID::Boss_Attack_R1_1, CSoundMgr::CHANNELID::Boss_Attack_R1_2);
	//	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Boss_Attack_R1_2, 0.5f);
	//	m_bAttackR1_H = true;
	//}
}

CBoss_Attack_R1* CBoss_Attack_R1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Attack_R1* pInstance = new CBoss_Attack_R1(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Attack_R1 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Attack_R1::Free()
{
	__super::Free();
}
