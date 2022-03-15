#include "pch.h"
#include "1H_SwordSupermanStab.h"

#include "StateController.h"

C1H_SwordSupermanStab::C1H_SwordSupermanStab(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordSupermanStab::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 22;

	return S_OK;
}

_int C1H_SwordSupermanStab::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);

	if (m_pAnimationController->Is_Finished())
	{
		m_pStateController->Change_State(L"1H_SwordIdle");
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordSupermanStab::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordSupermanStab::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordSupermanStab::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Superman_Stab", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	if (!m_pSilvermane->Is_EquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}

	return S_OK;
}

HRESULT C1H_SwordSupermanStab::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordSupermanStab::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
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
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_Start")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C1H_SwordSupermanStab* C1H_SwordSupermanStab::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordSupermanStab* pInstance = new C1H_SwordSupermanStab(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordSupermanStab Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordSupermanStab::Free()
{

	__super::Free();
}
