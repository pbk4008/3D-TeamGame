#include "pch.h"
#include "1H_SwordAttackNormalR2_03.h"

#include "StateController.h"

C1H_SwordAttackNormalR2_03::C1H_SwordAttackNormalR2_03(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordAttackNormalR2_03::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordAttackNormalR2_03::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);


	_uint iCurkeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();


	if (m_pAnimationController->Is_Finished())
	{
		m_pStateController->Change_State(L"Idle");
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordAttackNormalR2_03::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordAttackNormalR2_03::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_03::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;
	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Player_Sword_Attack);
	g_pGameInstance->Play_Shot(L"Needle_Attack_R_2", CSoundMgr::CHANNELID::Player_Sword_Attack);

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Normal_R2_03", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);


	m_iCutIndex = 22;
	//m_pAnimationController->Set_PlaySpeed(1.2f);
	m_pAnimationController->Set_PlaySpeed(1.4f);
	m_iAttackStartIndex = 15;
	m_iAttackEndIndex = 22;

	m_iTrailStartIndex = 14;
	m_iTrailEndIndex = 27;

	m_tShakeEvent.tWaveX.fAmplitude = 0.1f;
	m_tShakeEvent.tWaveX.fFrequency = 10.f;
	m_tShakeEvent.tWaveY.fAmplitude = 0.1f;
	m_tShakeEvent.tWaveY.fFrequency = 6.f;
	m_tShakeEvent.tWaveZ.fAmplitude = 0.1f;
	m_tShakeEvent.tWaveZ.fFrequency = 8.f;

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_03::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PlaySpeed(1.f);
	return S_OK;
}

_int C1H_SwordAttackNormalR2_03::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR1_01")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_04")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C1H_SwordAttackNormalR2_03* C1H_SwordAttackNormalR2_03::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordAttackNormalR2_03* pInstance = new C1H_SwordAttackNormalR2_03(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordAttackNormalR2_03 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordAttackNormalR2_03::Free()
{

	__super::Free();
}
