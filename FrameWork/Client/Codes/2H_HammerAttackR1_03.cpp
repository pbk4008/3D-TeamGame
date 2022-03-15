#include "pch.h"
#include "2H_HammerAttackR1_03.h"

#include "StateController.h"

C2H_HammerAttackR1_03::C2H_HammerAttackR1_03(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerAttackR1_03::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 40;

	return S_OK;
}

_int C2H_HammerAttackR1_03::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Is_Finished())
	{
		m_pStateController->Change_State(L"2H_HammerIdle");
		return STATE_CHANGE;
	}

	return _int();
}

_int C2H_HammerAttackR1_03::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerAttackR1_03::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerAttackR1_03::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Attack_R1_03", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_iCutIndex = 50;
	return S_OK;
}

HRESULT C2H_HammerAttackR1_03::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerAttackR1_03::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_04")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerChargeStage1_Start")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C2H_HammerAttackR1_03* C2H_HammerAttackR1_03::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerAttackR1_03* pInstance = new C2H_HammerAttackR1_03(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerAttackR1_03 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerAttackR1_03::Free()
{

	__super::Free();
}
