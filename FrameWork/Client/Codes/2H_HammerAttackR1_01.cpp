#include "pch.h"
#include "2H_HammerAttackR1_01.h"

#include "StateController.h"

C2H_HammerAttackR1_01::C2H_HammerAttackR1_01(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerAttackR1_01::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerAttackR1_01::Tick(const _double& _dDeltaTime)
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

_int C2H_HammerAttackR1_01::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerAttackR1_01::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerAttackR1_01::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Attack_R1_01", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	if (!m_pSilvermane->Is_EquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		CHierarchyNode* pWeaponBone = m_pModel->Get_BoneMatrix("weapon_r");
		m_pSilvermane->Set_WeaponFixedBone(pWeaponBone);
	}


	m_iCutIndex = 33;
	m_pAnimationController->Set_PlaySpeed(1.2f);
	return S_OK;
}

HRESULT C2H_HammerAttackR1_01::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PlaySpeed(1.f);
	return S_OK;
}

_int C2H_HammerAttackR1_01::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			m_pStateController->Change_State(L"2H_HammerAttackR1_02");
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

C2H_HammerAttackR1_01* C2H_HammerAttackR1_01::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerAttackR1_01* pInstance = new C2H_HammerAttackR1_01(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerAttackR1_01 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerAttackR1_01::Free()
{

	__super::Free();
}
