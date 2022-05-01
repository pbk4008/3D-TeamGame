#include "pch.h"
#include "2H_HammerChargeStage1_Loop.h"

C2H_HammerChargeStage1_Loop::C2H_HammerChargeStage1_Loop(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerChargeStage1_Loop::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerChargeStage1_Loop::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_fHoldTime += (_float)_dDeltaTime;

	return _int();
}

_int C2H_HammerChargeStage1_Loop::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerChargeStage1_Loop::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerChargeStage1_Loop::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	// ¶óÀÌÆ®
	m_pSilvermane->Set_LightCheck(true);

	_vector Pos = { 0.f, 0.03f, 0.f ,0.f };
	m_pSilvermane->Active_Effect((_uint)EFFECT::ATTACK_GROUND, Pos);
	m_pSilvermane->Active_Effect((_uint)EFFECT::ATTACK_GROUND_2, Pos);

	Pos = { 0.f, 0.1f, 0.f ,0.f };
	m_pSilvermane->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE, Pos);

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Charge_Stage_1_Loop", true)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_pSilvermane->Set_IsTrasceCamera(false);


	m_tShakeEvent.fDuration = 0.4f;
	m_tShakeEvent.fBlendOutTime = 0.3f;
	m_tShakeEvent.tWaveX.fAmplitude = 0.02f;
	m_tShakeEvent.tWaveX.fFrequency = 10.f;
	m_tShakeEvent.tWaveY.fAmplitude = 0.02f;
	m_tShakeEvent.tWaveY.fFrequency = 6.f;
	m_tShakeEvent.tWaveZ.fAmplitude = 0.02f;
	m_tShakeEvent.tWaveZ.fFrequency = 8.f;


	m_iShakeIndex = 0;

	STOP_SOUND(CHANNEL::PLAYER2);
	PLAY_SOUND(L"Hammer_L2_ChargeIgnite", CHANNEL::PLAYER2);

	return S_OK;
}

HRESULT C2H_HammerChargeStage1_Loop::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_fHoldTime = 0.f;

	return S_OK;
}

_int C2H_HammerChargeStage1_Loop::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getMouseKeyUp(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (0.1f < m_fHoldTime)
		{
			if (m_pStateController->Change_State(L"2H_HammerAttackR2_Charge"))
				return E_FAIL;
			return STATE_CHANGE;
		}
		else
		{
			if (m_pStateController->Change_State(L"2H_HammerAttackR2_01"))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C2H_HammerChargeStage1_Loop* C2H_HammerChargeStage1_Loop::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerChargeStage1_Loop* pInstance = new C2H_HammerChargeStage1_Loop(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerChargeStage1_Loop Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerChargeStage1_Loop::Free()
{

	__super::Free();
}
