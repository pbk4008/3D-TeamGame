#include "pch.h"
#include "2H_HammerEquipOff.h"

#include "Silvermane.h"

C2H_HammerEquipOff::C2H_HammerEquipOff(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerEquipOff::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 23;

	return S_OK;
}

_int C2H_HammerEquipOff::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

_int C2H_HammerEquipOff::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerEquipOff::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerEquipOff::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Equip_Off", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	return S_OK;
}

HRESULT C2H_HammerEquipOff::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_EquipWeapon(false);
	m_pSilvermane->Set_WeaponFixedBone("spine_03");

	return S_OK;
}

_int C2H_HammerEquipOff::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_01")))
			return -1;
		return STATE_CHANGE;
	}

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
		{
			if (g_pGameInstance->getkeyPress(DIK_W) ||
				g_pGameInstance->getkeyPress(DIK_S) ||
				g_pGameInstance->getkeyPress(DIK_A) ||
				g_pGameInstance->getkeyPress(DIK_D))
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwdStart")))
					return -1;
				return STATE_CHANGE;
			}
		}
	}

	return _int();
}

C2H_HammerEquipOff* C2H_HammerEquipOff::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerEquipOff* pInstance = new C2H_HammerEquipOff(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerEquipOff Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerEquipOff::Free()
{
	
	__super::Free();
}
