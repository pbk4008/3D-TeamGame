#include "pch.h"
#include "1H_SwordAttackNormalR2_04.h"

#include "StateController.h"

C1H_SwordAttackNormalR2_04::C1H_SwordAttackNormalR2_04(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordAttackNormalR2_04::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 40;

	return S_OK;
}

_int C1H_SwordAttackNormalR2_04::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Is_Finished())
	{
		m_pStateController->Change_State(L"Idle");
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordAttackNormalR2_04::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordAttackNormalR2_04::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_04::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Normal_R2_04", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_04::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordAttackNormalR2_04::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_ReleaseStab")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C1H_SwordAttackNormalR2_04* C1H_SwordAttackNormalR2_04::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordAttackNormalR2_04* pInstance = new C1H_SwordAttackNormalR2_04(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordAttackNormalR2_04 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordAttackNormalR2_04::Free()
{

	__super::Free();
}
