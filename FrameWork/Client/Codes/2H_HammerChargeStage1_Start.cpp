#include "pch.h"
#include "2H_HammerChargeStage1_Start.h"

C2H_HammerChargeStage1_Start::C2H_HammerChargeStage1_Start(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerChargeStage1_Start::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerChargeStage1_Start::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerChargeStage1_Loop")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	return _int();
}

_int C2H_HammerChargeStage1_Start::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerChargeStage1_Start::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerChargeStage1_Start::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;


	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Charge_Stage_1_Start", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}
	m_pSilvermane->Set_IsTrasceCamera(false);

	return S_OK;
}

HRESULT C2H_HammerChargeStage1_Start::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);

	return S_OK;
}

_int C2H_HammerChargeStage1_Start::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getMouseKeyUp(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR2_01")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	return _int();
}

C2H_HammerChargeStage1_Start* C2H_HammerChargeStage1_Start::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerChargeStage1_Start* pInstance = new C2H_HammerChargeStage1_Start(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerChargeStage1_Start Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerChargeStage1_Start::Free()
{

	__super::Free();
}
