#include "pch.h"
#include "2H_HammerAttackR2_Charge.h"

C2H_HammerAttackR2_Charge::C2H_HammerAttackR2_Charge(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerAttackR2_Charge::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 90;

	return S_OK;
}

_int C2H_HammerAttackR2_Charge::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_fRotTime += (_float)_dDeltaTime;
	Add_PlusAngle(EDir::Forward, -_dDeltaTime);

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (37 < iCurKeyFrameIndex && 40 > iCurKeyFrameIndex)
		m_pSilvermane->Set_IsTrasceCamera(false);
	else if (60 < iCurKeyFrameIndex && 65 > iCurKeyFrameIndex)
		m_pSilvermane->Set_IsTrasceCamera(true);
	else if (75 < iCurKeyFrameIndex)
		m_pSilvermane->Set_IsTrasceCamera(false);


	if (51 < iCurKeyFrameIndex && 53 > iCurKeyFrameIndex)
	{
		STOP_SOUND(CHANNEL::Player_Sword_Attack);
		PLAY_SOUND(L"Hammer_Swing", CHANNEL::Player_Sword_Attack);
	}
	if (70 < iCurKeyFrameIndex && 72 > iCurKeyFrameIndex)
	{
		STOP_SOUND(CHANNEL::PLAYER1);
		PLAY_SOUND(L"Hammer_Swing", CHANNEL::PLAYER1);
	}


	if (iCurKeyFrameIndex == m_iShakeIndex2)
	{
		if (!m_isShake2)
		{
			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(m_tShakeEvent2, vPos);
			m_isShake2 = true;
		}
	}


	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	return _int();
}

_int C2H_HammerAttackR2_Charge::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerAttackR2_Charge::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerAttackR2_Charge::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Attack_R2_Charge", false)))
		return E_FAIL;

	m_pAnimationController->Set_RootMotion(true, true);
	m_pAnimationController->Set_PlaySpeed(1.4f);

	m_pAnimationController->Add_TrackAcc(35.0);


	// ?????? ????
	//1
	m_tShakeEvent = CCameraShake::SHAKEEVENT();
	m_tShakeEvent.fDuration = 0.4f;
	m_tShakeEvent.fBlendOutTime = 0.3f;
	m_tShakeEvent.tWaveX.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveX.fFrequency = 10.f;
	m_tShakeEvent.tWaveY.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveY.fFrequency = 6.f;
	m_tShakeEvent.tWaveZ.fAmplitude = 0.02f;
	m_tShakeEvent.tWaveZ.fFrequency = 8.f;
	//2
	m_tShakeEvent2 = CCameraShake::SHAKEEVENT();
	m_tShakeEvent2.fDuration = 0.4f;
	m_tShakeEvent2.fBlendOutTime = 0.3f;
	m_tShakeEvent2.tWaveX.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveX.fFrequency = 10.f;
	m_tShakeEvent2.tWaveY.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveY.fFrequency = 6.f;
	m_tShakeEvent2.tWaveZ.fAmplitude = 0.02f;
	m_tShakeEvent2.tWaveZ.fFrequency = 8.f;


	m_iAttackStartIndex = 50;
	m_iAttackEndIndex = 90;
	m_iTrailStartIndex = 50;
	m_iTrailEndIndex = 90;

	m_iShakeIndex = 54;
	m_iShakeIndex2 = 73;

	return S_OK;
}

HRESULT C2H_HammerAttackR2_Charge::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PlaySpeed(1.f);

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_fRotTime = 0.f;

	m_isShake2 = false;
	return S_OK;
}

_int C2H_HammerAttackR2_Charge::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_01")))
				return E_FAIL;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR2_02")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C2H_HammerAttackR2_Charge* C2H_HammerAttackR2_Charge::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerAttackR2_Charge* pInstance = new C2H_HammerAttackR2_Charge(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerAttackR2_Charge Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerAttackR2_Charge::Free()
{

	__super::Free();
}
