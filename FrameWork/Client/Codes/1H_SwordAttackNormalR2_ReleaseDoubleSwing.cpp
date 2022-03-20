#include "pch.h"
#include "1H_SwordAttackNormalR2_ReleaseDoubleSwing.h"

C1H_SwordAttackNormalR2_ReleaseDoubleSwing::C1H_SwordAttackNormalR2_ReleaseDoubleSwing(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordAttackNormalR2_ReleaseDoubleSwing::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 60;

	return S_OK;
}

_int C1H_SwordAttackNormalR2_ReleaseDoubleSwing::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_fRotTime += (_float)_dDeltaTime;
	Add_PlusAngle(EDir::Forward, -_dDeltaTime);

	if (25 < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		m_pSilvermane->Set_IsTrasceCamera(false);
	}

	//if (0.05f < m_fRotTime)
	//{
	//	m_pSilvermane->Set_IsTrasceCamera(false);
	//}

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordAttackNormalR2_ReleaseDoubleSwing::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordAttackNormalR2_ReleaseDoubleSwing::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_ReleaseDoubleSwing::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Normal_R2_Release_DoubleSwing", false)))
		return E_FAIL;

	m_pAnimationController->Set_RootMotion(true, true);
	m_pAnimationController->Set_PlaySpeed(1.4f);
	m_pAnimationController->Add_TrackAcc(20.0);

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_ReleaseDoubleSwing::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PlaySpeed(1.f);

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_fRotTime = 0.f;

	return S_OK;
}

_int C1H_SwordAttackNormalR2_ReleaseDoubleSwing::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR1_01")))
				return E_FAIL;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_02")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C1H_SwordAttackNormalR2_ReleaseDoubleSwing* C1H_SwordAttackNormalR2_ReleaseDoubleSwing::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordAttackNormalR2_ReleaseDoubleSwing* pInstance = new C1H_SwordAttackNormalR2_ReleaseDoubleSwing(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordAttackNormalR2_ReleaseDoubleSwing Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordAttackNormalR2_ReleaseDoubleSwing::Free()
{

	__super::Free();
}
