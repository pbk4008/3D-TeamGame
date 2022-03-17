#include "pch.h"
#include "1H_SwordAttackNormalR2_Start.h"

C1H_SwordAttackNormalR2_Start::C1H_SwordAttackNormalR2_Start(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordAttackNormalR2_Start::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 20;

	return S_OK;
}

_int C1H_SwordAttackNormalR2_Start::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_Loop")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	m_iCutIndex = 20;
	return _int();
}

_int C1H_SwordAttackNormalR2_Start::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordAttackNormalR2_Start::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_Start::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;


	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Normal_R2_Start", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_pSilvermane->Set_TrasceCamera(false);

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_Start::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_TrasceCamera(true);

	return S_OK;
}

_int C1H_SwordAttackNormalR2_Start::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getMouseKeyUp(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_ReleaseStab")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	return _int();
}

C1H_SwordAttackNormalR2_Start* C1H_SwordAttackNormalR2_Start::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordAttackNormalR2_Start* pInstance = new C1H_SwordAttackNormalR2_Start(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordAttackNormalR2_Start Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordAttackNormalR2_Start::Free()
{

	__super::Free();
}
