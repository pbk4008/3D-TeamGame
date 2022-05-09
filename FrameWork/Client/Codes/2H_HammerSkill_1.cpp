#include "pch.h"
#include "2H_HammerSkill_1.h"

#include "MeshEffect_Test.h"

C2H_HammerSkill_1::C2H_HammerSkill_1(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerSkill_1::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerSkill_1::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if(22 < iCurKeyFrameIndex)
		m_pSilvermane->Set_IsTrail(false);
	if (35 < iCurKeyFrameIndex)
	{
		if (!m_isAttack)
		{		
			// ¸Þ½¬ ÀÌÆåÆ®
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

			m_pSilvermane->RangeAttack(3.f);
			m_isAttack = true;

			STOP_SOUND(CHANNEL::PLAYER1);
			PLAY_SOUND(L"Warhammer_R1_Impact_01", CHANNEL::PLAYER1);
		}
		m_pSilvermane->Set_IsAttack(false);
	}

	if (14 < iCurKeyFrameIndex && 16 > iCurKeyFrameIndex)
	{
		if (!m_isShake)
		{
			CCameraShake::SHAKEEVENT tShakeEvent;
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

			STOP_SOUND(CHANNEL::PLAYER2);
			PLAY_SOUND(L"Hammer_Swing", CHANNEL::PLAYER2);
		}
	}
	else if (35 < iCurKeyFrameIndex && 37 > iCurKeyFrameIndex)
	{
		if (!m_isShake)
		{
			CCameraShake::SHAKEEVENT tShakeEvent;
			tShakeEvent.fDuration = 0.4f;
			tShakeEvent.fBlendOutTime = 0.3f;
			tShakeEvent.tWaveX.fAmplitude = 0.1f;
			tShakeEvent.tWaveX.fFrequency = 10.f;
			tShakeEvent.tWaveY.fAmplitude = 0.1f;
			tShakeEvent.tWaveY.fFrequency = 6.f;
			tShakeEvent.tWaveZ.fAmplitude = 0.1f;
			tShakeEvent.tWaveZ.fFrequency = 8.f;
			CCameraShake::SHAKEEVENT tShakeEvent2;
			tShakeEvent2.fDuration = 1.f;
			tShakeEvent2.fBlendInTime = 0.2f;
			tShakeEvent2.fBlendOutTime = 0.5f;
			tShakeEvent2.tWaveY.fAmplitude = 0.02f;
			tShakeEvent2.tWaveY.fFrequency = 1.f;
			tShakeEvent2.tWaveY.fAdditionalOffset = -0.8f;
			tShakeEvent2.tWaveZ.fAdditionalOffset = 0.6f;
			tShakeEvent2.tWaveZ.fFrequency = 1.f;
			tShakeEvent2.tWaveZ.fAmplitude = 0.04f;

			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(tShakeEvent, vPos);
			g_pShakeManager->Shake(tShakeEvent2, vPos);

			m_isShake = true;
		}
	}
	else
		m_isShake = false;

	if (m_pAnimationController->Is_Finished())
		return ToIdle();

	return _int();
}

_int C2H_HammerSkill_1::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerSkill_1::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerSkill_1::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Attack_L1_R2_Legacy", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	m_pAnimationController->Mul_MoveSpeed(0.8f);

	m_pSilvermane->Add_SkillGuage(-50.f);
	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pSilvermane->Set_IsAttack(true);
	m_pSilvermane->Set_IsTrail(true);
	m_pSilvermane->Set_IsSkill(true);

	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}

	m_iCutIndex = 40;

	CCameraShake::SHAKEEVENT tShakeEvent;
	tShakeEvent.fDuration = 0.4f;
	tShakeEvent.fBlendOutTime = 0.3f;
	tShakeEvent.tWaveX.fAmplitude = 0.08f;
	tShakeEvent.tWaveX.fFrequency = 10.f;
	tShakeEvent.tWaveY.fAmplitude = 0.08f;
	tShakeEvent.tWaveY.fFrequency = 6.f;
	tShakeEvent.tWaveZ.fAmplitude = 0.08f;
	tShakeEvent.tWaveZ.fFrequency = 8.f;
	_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	g_pShakeManager->Shake(tShakeEvent, vPos);

	STOP_SOUND(CHANNEL::PLAYER1);
	PLAY_SOUND(L"Hammer_Swing", CHANNEL::PLAYER1);

	return S_OK;
}

HRESULT C2H_HammerSkill_1::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Div_MoveSpeed(0.8f);

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pSilvermane->Set_IsSkill(false);
	m_pSilvermane->Set_IsTrail(false);

	m_isAttack = false;
	return S_OK;
}

_int C2H_HammerSkill_1::Input(const _double& _dDeltaTime)
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

		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_01")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerChargeStage1_Start")))
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
					if (FAILED(m_pStateController->Change_State(L"2H_HammerEquipOff")))
						return -1;
				}
				return STATE_CHANGE;
			}
		}
		else
		{
			if (g_pGameInstance->getkeyPress(DIK_W))
			{
				m_pStateController->Change_State(L"2H_HammerJogFwdStart");
				return STATE_CHANGE;
			}
			if (g_pGameInstance->getkeyPress(DIK_S))
			{
				m_pStateController->Change_State(L"2H_HammerJogBwdStart");
				return STATE_CHANGE;
			}
			if (g_pGameInstance->getkeyPress(DIK_D))
			{
				m_pStateController->Change_State(L"2H_HammerJogRightStart");
				return STATE_CHANGE;
			}
			if (g_pGameInstance->getkeyPress(DIK_A))
			{
				m_pStateController->Change_State(L"2H_HammerJogLeftStart");
				return STATE_CHANGE;
			}
		}
	}

	return _int();
}

C2H_HammerSkill_1* C2H_HammerSkill_1::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerSkill_1* pInstance = new C2H_HammerSkill_1(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerSkill_1 Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C2H_HammerSkill_1::Free()
{
	__super::Free();
}
