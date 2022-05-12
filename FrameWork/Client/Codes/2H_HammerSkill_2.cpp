#include "pch.h"
#include "2H_HammerSkill_2.h"

C2H_HammerSkill_2::C2H_HammerSkill_2(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerSkill_2::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerSkill_2::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (68 < iCurKeyFrameIndex)
	{
		m_pSilvermane->Set_IsTrail(false);
		m_pSilvermane->Set_IsAttack(false);
	}

	if (37 < iCurKeyFrameIndex && 68 > iCurKeyFrameIndex)
	{
		m_pAnimationController->Set_PlaySpeed(1.f);
		m_pSilvermane->Set_IsTrail(true);
		m_fMTAcc += g_fDeltaTime;
		if (0.025f < m_fMTAcc)
		{
			m_pSilvermane->Create_MotionTrail(m_motiontrailidx);
			++m_motiontrailidx;
			m_fMTAcc = 0.f;
		}

		if (m_motiontrailidx >= 20)
			m_motiontrailidx = 0;
	}

	if (36 < iCurKeyFrameIndex && 38 > iCurKeyFrameIndex ||
		46 < iCurKeyFrameIndex && 48 > iCurKeyFrameIndex ||
		56 < iCurKeyFrameIndex && 58 > iCurKeyFrameIndex || 
		66 < iCurKeyFrameIndex && 68 > iCurKeyFrameIndex)
	{
		if (!m_isAttack)
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

			m_pSilvermane->RangeAttack(4.f);
			m_isAttack = true;
		}
	}
	else
		m_isAttack = false;

	if (36 < iCurKeyFrameIndex && 38 > iCurKeyFrameIndex)
	{
		STOP_SOUND(CHANNEL::PLAYER1);
		PLAY_SOUND(L"JustFrame_Throw_01", CHANNEL::PLAYER1);
	}
	if (67 < iCurKeyFrameIndex && 69 > iCurKeyFrameIndex)
	{
		STOP_SOUND(CHANNEL::PLAYER1);
		PLAY_SOUND(L"JustFrame_Catch_01", CHANNEL::PLAYER1);
	}

	if (m_pAnimationController->Is_Finished())
		return ToIdle();

	return _int();
}

_int C2H_HammerSkill_2::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerSkill_2::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerSkill_2::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Attack_S9", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_pSilvermane->Add_SkillGuage(-50.f);
	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pSilvermane->Set_IsAttack(true);
	m_pSilvermane->Set_IsSkill(true);
	m_pAnimationController->Set_PlaySpeed(1.6f);

	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}

	CCameraShake::SHAKEEVENT tShakeEvent;
	tShakeEvent.fInnerRadius = 4.f;
	tShakeEvent.fDuration = 4.f;
	tShakeEvent.fBlendInTime = 0.5f;
	tShakeEvent.fBlendOutTime = 2.f;
	tShakeEvent.tWaveY.fAdditionalOffset = 2.f;
	tShakeEvent.tWaveY.fFrequency = 0.01f;
	tShakeEvent.tWaveY.fAmplitude = 0.01f;
	tShakeEvent.tWaveZ.fAdditionalOffset = -5.f;
	tShakeEvent.tWaveZ.fFrequency = 0.01f;
	tShakeEvent.tWaveZ.fAmplitude = 0.01f;

	_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	g_pShakeManager->Shake(tShakeEvent, vPos);

	m_iCutIndex = 80;

	STOP_SOUND(CHANNEL::PLAYER1);
	PLAY_SOUND(L"Hammer_L2_ChargeBuild", CHANNEL::PLAYER1);

	return S_OK;
}

HRESULT C2H_HammerSkill_2::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pSilvermane->Set_IsSkill(false);
	m_pSilvermane->Set_IsTrail(false);
	m_pAnimationController->Set_PlaySpeed(1.f);

	m_isAttack = false;

	return S_OK;
}

_int C2H_HammerSkill_2::Input(const _double& _dDeltaTime)
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

C2H_HammerSkill_2* C2H_HammerSkill_2::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerSkill_2* pInstance = new C2H_HammerSkill_2(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerSkill_2 Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C2H_HammerSkill_2::Free()
{
	__super::Free();
}
