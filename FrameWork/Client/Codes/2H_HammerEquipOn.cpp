#include "pch.h"
#include "2H_HammerEquipOn.h"

#include"Silvermane.h"

C2H_HammerEquipOn::C2H_HammerEquipOn(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerEquipOn::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 11;

	return S_OK;
}

_int C2H_HammerEquipOn::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (!m_pSilvermane->IsEquipWeapon())
		{
			m_pSilvermane->Set_EquipWeapon(true);
			m_pSilvermane->Set_WeaponFixedBone("weapon_r");
		}
	}

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
			return -1;
		return STATE_CHANGE;
	}


	return _int();
}

_int C2H_HammerEquipOn::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerEquipOn::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerEquipOn::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Equip_On", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_pAnimationController->Set_PlaySpeed(1.4f);

	return S_OK;
}

HRESULT C2H_HammerEquipOn::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PlaySpeed(1.f);

	return S_OK;
}

_int C2H_HammerEquipOn::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{

	}

	return _int();
}

C2H_HammerEquipOn* C2H_HammerEquipOn::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerEquipOn* pInstance = new C2H_HammerEquipOn(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerEquipOn Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerEquipOn::Free()
{
	
	__super::Free();
}
