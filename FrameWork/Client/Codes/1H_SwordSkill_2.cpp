#include "pch.h"
#include "1H_SwordSkill_2.h"

#include "MeshEffect_Test.h"

C1H_SwordSkill_2::C1H_SwordSkill_2(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordSkill_2::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordSkill_2::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();

	CActor* pTarget = m_pSilvermane->Get_TargetExecution();
	if (pTarget)
	{
		CTransform* pTargetTransform = pTarget->Get_Transform();
		_vector svTargetPos = XMVectorSetY(pTargetTransform->Get_State(CTransform::STATE_POSITION), 0.f);
		_vector svPos = XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_POSITION), 0.f);

		_vector svLook = XMVector3Normalize(svTargetPos - svPos);
		_vector svRight = XMVector3Normalize(XMVector3Cross(_vector{ 0.f, 1.f, 0.f, 0.f }, svLook));
		_vector svUp = XMVector3Normalize(XMVector3Cross(svLook, svRight));

		svLook *= m_pTransform->Get_Scale(CTransform::STATE_LOOK);
		svRight *= m_pTransform->Get_Scale(CTransform::STATE_RIGHT);
		svUp *= m_pTransform->Get_Scale(CTransform::STATE_UP);

		m_pTransform->Set_State(CTransform::STATE_LOOK, svLook);
		m_pTransform->Set_State(CTransform::STATE_RIGHT, svRight);
		m_pTransform->Set_State(CTransform::STATE_UP, svUp);

		//if (16 < iCurKeyFrameIndex && 18 > iCurKeyFrameIndex ||
		//	26 < iCurKeyFrameIndex && 28 > iCurKeyFrameIndex ||
		//	38 < iCurKeyFrameIndex && 40 > iCurKeyFrameIndex ||
		//	52 < iCurKeyFrameIndex && 54 > iCurKeyFrameIndex)
		//{
		//	if (!m_isAttack)
		//	{
		//		ATTACKDESC tAttackDesc = m_pSilvermane->Get_AttackDesc();
		//		tAttackDesc.fDamage += 50.f;
		//		tAttackDesc.pHitObject = m_pSilvermane->Get_CurerntWeapon();
		//		pTarget->Hit(tAttackDesc);
		//		m_isAttack = true;

		//		CCameraShake::SHAKEEVENT tShakeEvent;
		//		tShakeEvent.fDuration = 0.4f;
		//		tShakeEvent.fBlendOutTime = 0.3f;
		//		tShakeEvent.tWaveX.fAmplitude = 0.06f;
		//		tShakeEvent.tWaveX.fFrequency = 10.f;
		//		tShakeEvent.tWaveY.fAmplitude = 0.06f;
		//		tShakeEvent.tWaveY.fFrequency = 6.f;
		//		tShakeEvent.tWaveZ.fAmplitude = 0.06f;
		//		tShakeEvent.tWaveZ.fFrequency = 8.f;
		//		_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		//		g_pShakeManager->Shake(tShakeEvent, vPos);
		//	}
		//}
		//else
		//	m_isAttack = false;
	}

	if (28 > iCurKeyFrameIndex)
	{
		m_fMTAcc += g_fDeltaTime;
		if (0.1f < m_fMTAcc)
		{
			m_pSilvermane->Create_MotionTrail(m_motiontrailidx);
			++m_motiontrailidx;
			m_fMTAcc = 0.f;
		}

		if (m_motiontrailidx >= 20)
			m_motiontrailidx = 0;
	}
	else
	{
		if (!m_isAttack)
		{
			m_pSilvermane->Set_LightCheck(true);
			m_pSilvermane->Set_LightColor(XMVectorSet(1.f, 0.f, 0.f, 1.f));
			m_pSilvermane->Set_LightOrigRange(5.f);
			m_pSilvermane->Set_LightAmbientSpecular(_float4(0.8f, 0.8f, 0.8f, 1.f), _float4(0.5f, 0.5f, 0.5f, 1.f));

			_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
			_vector svLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
			_vector svUp = m_pTransform->Get_State(CTransform::STATE_UP);
			CMeshEffect::DESC tMeshEffectDesc;
			XMStoreFloat3(&tMeshEffectDesc.vPosition, svPos + (svLook * 2.f) + (svUp * 0.1f));
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_pSilvermane->getSceneID(), L"Layer_Effect", L"Proto_GameObject_MeshEffect_Test", &tMeshEffectDesc)))
				return E_FAIL;
			// ÀÌÆåÆ®
			m_pSilvermane->Active_Effect((_uint)EFFECT::ATTACK_GROUND, (svLook * 2.f) + (svUp * 0.1f));
			m_pSilvermane->Active_Effect((_uint)EFFECT::ATTACK_GROUND_2, (svLook * 2.f) + (svUp * 0.1f));

			m_pSilvermane->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE, (svLook * 2.f) + (svUp * 0.1f));

			m_pSilvermane->RangeAttack();
			m_isAttack = true;
		}

		if (!m_isShake)
		{
			CCameraShake::SHAKEEVENT tShakeEvent;
			tShakeEvent.fDuration = 0.6f;
			tShakeEvent.fBlendInTime = 0.1f;
			tShakeEvent.fBlendOutTime = 0.3f;
			tShakeEvent.tWaveX.fAmplitude = 0.03f;
			tShakeEvent.tWaveX.fFrequency = 10.f;

			CCameraShake::SHAKEEVENT tShakeEvent2;
			tShakeEvent2.fDuration = 1.f;
			tShakeEvent2.fBlendInTime = 0.1f;
			tShakeEvent2.fBlendOutTime = 0.5f;
			tShakeEvent2.tWaveY.fAmplitude = 0.004f;
			tShakeEvent2.tWaveY.fFrequency = 1.f;
			tShakeEvent2.tWaveY.fAdditionalOffset = -1.f;

			_float3 vPos{}; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(tShakeEvent, vPos);
			g_pShakeManager->Shake(tShakeEvent2, vPos);
			m_isShake = true;
		}
	}

	if (m_pAnimationController->Is_Finished())
		return ToIdle();

	return _int();
}

_int C1H_SwordSkill_2::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordSkill_2::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordSkill_2::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Prestige_KingArthur", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pSilvermane->Set_IsDash(true);
	m_pSilvermane->Set_IsSkill(true);
	m_pSilvermane->Add_SkillGuage(-50.f);

	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}

	m_iCutIndex = 35;

	CCameraShake::SHAKEEVENT tShakeEvent;
	tShakeEvent.fDuration = 1.5f;
	tShakeEvent.fBlendInTime = 0.5f;
	tShakeEvent.fBlendOutTime = 1.f;
	tShakeEvent.tWaveY.fAdditionalOffset = 2.f;
	tShakeEvent.tWaveY.fFrequency = 0.01f;
	tShakeEvent.tWaveY.fAmplitude = 0.01f;
	tShakeEvent.tWaveZ.fAdditionalOffset = -2.f;
	tShakeEvent.tWaveZ.fFrequency = 0.01f;
	tShakeEvent.tWaveZ.fAmplitude = 0.01f;

	_float3 vPos{}; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	g_pShakeManager->Shake(tShakeEvent, vPos);

	return S_OK;
}

HRESULT C1H_SwordSkill_2::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pSilvermane->Set_IsSkill(false);
	m_pSilvermane->Set_IsDash(false);
	m_pSilvermane->Set_IsSkill(false);

	m_isAttack = false;
	return S_OK;
}

_int C1H_SwordSkill_2::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (m_iCutIndex < iCurKeyFrameIndex)
	{
		if (g_pGameInstance->getkeyDown(DIK_SPACE))
		{
			if (g_pGameInstance->getkeyPress(DIK_W))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_DodgeSpin")))
					return -1;
				return STATE_CHANGE;
			}
			else if (g_pGameInstance->getkeyPress(DIK_A))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepLeft")))
					return -1;
				return STATE_CHANGE;
			}
			else if (g_pGameInstance->getkeyPress(DIK_D))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepRight")))
					return -1;
				return STATE_CHANGE;
			}
			else
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
					return -1;
				return STATE_CHANGE;
			}
		}
		if (g_pGameInstance->getkeyPress(DIK_Q))
		{
			if (m_pSilvermane->IsLootShield())
			{
				if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
					return E_FAIL;
				return STATE_CHANGE;
			}
		}

		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR1_01")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_Start")))
				return -1;
			return STATE_CHANGE;
		}

		if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
		{
			if (g_pGameInstance->getkeyPress(DIK_W) ||
				g_pGameInstance->getkeyPress(DIK_S) ||
				g_pGameInstance->getkeyPress(DIK_A) ||
				g_pGameInstance->getkeyPress(DIK_D))
			{
				if (!m_pSilvermane->IsEquipWeapon())
				{
					if (FAILED(m_pStateController->Change_State(L"SprintFwdStart")))
						return -1;
				}
				else
				{
					if (FAILED(m_pStateController->Change_State(L"1H_SwordEquipOff")))
						return -1;
				}
				return STATE_CHANGE;
			}
		}
		else
		{
			if (g_pGameInstance->getkeyPress(DIK_W))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SwordJogFwdStart")))
					return -1;
				return STATE_CHANGE;
			}
			if (g_pGameInstance->getkeyPress(DIK_S))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SwordJogBwdStart")))
					return -1;
				return STATE_CHANGE;
			}
			if (g_pGameInstance->getkeyPress(DIK_D))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SwordJogRightStart")))
					return -1;
				return STATE_CHANGE;
			}
			if (g_pGameInstance->getkeyPress(DIK_A))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SwordJogLeftStart")))
					return -1;
				return STATE_CHANGE;
			}
		}
	}

	return _int();
}

C1H_SwordSkill_2* C1H_SwordSkill_2::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordSkill_2* pInstance = new C1H_SwordSkill_2(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordSkill_2 Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C1H_SwordSkill_2::Free()
{
	__super::Free();
}
