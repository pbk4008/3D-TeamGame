#include "pch.h"
#include "1H_SwordSkill_3.h"

#include "MeshEffect_Test.h"

C1H_SwordSkill_3::C1H_SwordSkill_3(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordSkill_3::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordSkill_3::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();

	CActor* pTarget = m_pSilvermane->Get_TargetExecution();
	if (pTarget)
	{
		if (pTarget->Get_Dead())
		{
			pTarget = nullptr;
			m_pSilvermane->Set_TargetExecution(nullptr);
		}
	}
	if (pTarget && 20 > iCurKeyFrameIndex)
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

		if (!m_isAttack && 18 < iCurKeyFrameIndex)
		{
			//ATTACKDESC tAttackDesc = m_pSilvermane->Get_AttackDesc();
			//tAttackDesc.fDamage += 50.f;
			//tAttackDesc.pHitObject = m_pSilvermane->Get_CurerntWeapon();
			//pTarget->Hit(tAttackDesc);
			m_pSilvermane->RangeAttack(4.f);
			m_isAttack = true;

			//STOP_SOUND(CHANNEL::Player_Sword_Attack);
			//PLAY_SOUND(L"Player_Gap_Closer", CHANNEL::Player_Sword_Attack);
		}
	}

	if (20 < iCurKeyFrameIndex && 30 > iCurKeyFrameIndex)
	{
		m_pTransform->Rotation_Axis(CTransform::STATE_UP, (_float)_dDeltaTime * 10.f);

		_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

		svPos += XMLoadFloat3(&m_vDir) * (_float)_dDeltaTime * 16.f;
		_float3 vPos{}; XMStoreFloat3(&vPos, svPos);
		m_pSilvermane->Set_FootPosition(vPos);

		m_pAnimationController->Set_RootMotion(true, false);

		if (pTarget && !m_isAttack2 && 28 < iCurKeyFrameIndex)
		{
			//ATTACKDESC tAttackDesc = m_pSilvermane->Get_AttackDesc();
			//tAttackDesc.fDamage += 50.f;
			//tAttackDesc.pHitObject = m_pSilvermane->Get_CurerntWeapon();
			//pTarget->Hit(tAttackDesc);
			m_pSilvermane->RangeAttack(5.f);
			m_isAttack2 = true;

			STOP_SOUND(CHANNEL::Player_Sword_Attack);
			PLAY_SOUND(L"Player_Gap_Closer", CHANNEL::Player_Sword_Attack);
		}
	}

	if (18 < iCurKeyFrameIndex && 20 > iCurKeyFrameIndex ||
		28 < iCurKeyFrameIndex && 30 > iCurKeyFrameIndex)
	{
		if (!m_isShake)
		{
			CCameraShake::SHAKEEVENT tShakeEvent;
			tShakeEvent.fInnerRadius = 100.f;
			tShakeEvent.fOuterRadius = 100.f;
			tShakeEvent.fDuration = 0.4f;
			tShakeEvent.fBlendOutTime = 0.3f;
			tShakeEvent.tWaveX.fAmplitude = 0.1f;
			tShakeEvent.tWaveX.fFrequency = 10.f;
			tShakeEvent.tWaveY.fAmplitude = 0.1f;
			tShakeEvent.tWaveY.fFrequency = 6.f;
			tShakeEvent.tWaveZ.fAmplitude = 0.1f;
			tShakeEvent.tWaveZ.fFrequency = 8.f;
			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(tShakeEvent, vPos);

			m_isShake = true;
		}
	}
	else
		m_isShake = false;

	if (m_pAnimationController->Is_Finished())
		return ToIdle();

	return _int();
}

_int C1H_SwordSkill_3::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordSkill_3::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordSkill_3::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Normal_L1_R2_Offset_10deg", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	m_pAnimationController->Set_PlaySpeed(1.4f);
	m_pAnimationController->Mul_MoveSpeed(1.4f);

	m_pSilvermane->Add_SkillGuage(-50.f);
	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pSilvermane->Set_IsDash(true);
	m_pSilvermane->Set_IsSkill(true);

	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}

	m_iCutIndex = 50;

	_vector svLook = XMVector3Normalize(XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_LOOK), 0.f));
	_vector svRight = XMVector3Normalize(XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_RIGHT), 0.f));

	XMStoreFloat3(&m_vDir, XMVector3Normalize(svLook + svRight));

	CCameraShake::SHAKEEVENT tShakeEvent;
	tShakeEvent.fInnerRadius = 0.f;
	tShakeEvent.fOuterRadius = 100.f;
	tShakeEvent.fDuration = 2.5f;
	tShakeEvent.fBlendInTime = 0.5f;
	tShakeEvent.fBlendOutTime = 2.f;
	tShakeEvent.tWaveY.fAdditionalOffset = 3.f;
	tShakeEvent.tWaveY.fFrequency = 0.01f;
	tShakeEvent.tWaveY.fAmplitude = 0.01f;
	tShakeEvent.tWaveZ.fAdditionalOffset = -6.f;
	tShakeEvent.tWaveZ.fFrequency = 0.01f;
	tShakeEvent.tWaveZ.fAmplitude = 0.01f;

	_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	g_pShakeManager->Shake(tShakeEvent, vPos);

	g_pGameInstance->VolumeChange(CHANNEL::Player_Sword_Attack, 4.f);
	STOP_SOUND(CHANNEL::Player_Sword_Attack);
	PLAY_SOUND(L"Dash_Leap_01", CHANNEL::Player_Sword_Attack);
	return S_OK;
}

HRESULT C1H_SwordSkill_3::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PlaySpeed(1.f);
	m_pAnimationController->Div_MoveSpeed(1.4f);

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pSilvermane->Set_IsSkill(false);
	m_pSilvermane->Set_IsDash(false);
	m_pSilvermane->Set_IsSkill(false);

	m_isAttack = false;
	m_isAttack2 = false;

	m_isShake = false;

	if (m_pSilvermane->Get_RadialCheck() == true)
		m_pSilvermane->Set_Radial(false);

	g_pGameInstance->VolumeChange(CHANNEL::Player_Sword_Attack, 1.f);
	return S_OK;
}

_int C1H_SwordSkill_3::Input(const _double& _dDeltaTime)
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

C1H_SwordSkill_3* C1H_SwordSkill_3::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordSkill_3* pInstance = new C1H_SwordSkill_3(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordSkill_3 Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C1H_SwordSkill_3::Free()
{
	__super::Free();
}
