#include "pch.h"
#include "1H_SwordEquipOff.h"

#include "Silvermane.h"

C1H_SwordEquipOff::C1H_SwordEquipOff(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordEquipOff::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordEquipOff::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordEquipOff::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordEquipOff::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordEquipOff::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Equip_Off", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	return S_OK;
}

HRESULT C1H_SwordEquipOff::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_EquipWeapon(false);
	CHierarchyNode* pWeaponBone = m_pModel->Get_BoneMatrix("spine_03");
	m_pSilvermane->Set_WeaponFixedBone(pWeaponBone);

	return S_OK;
}

_int C1H_SwordEquipOff::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR1_01")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (23 <= m_pAnimationController->Get_CurKeyFrameIndex())
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwdStart")))
					return E_FAIL;
				return STATE_CHANGE;
			}
		}
	}

	return _int();
}

C1H_SwordEquipOff* C1H_SwordEquipOff::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordEquipOff* pInstance = new C1H_SwordEquipOff(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordEquipOff Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordEquipOff::Free()
{
	
	__super::Free();
}
