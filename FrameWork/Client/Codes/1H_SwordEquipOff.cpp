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

	m_iCutIndex = 23;

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
			return -1;
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

	g_pGameInstance->Play_Shot(L"Needle_UnEquip", CSoundMgr::CHANNELID::Unequip_Needle);

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Equip_Off", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	return S_OK;
}

HRESULT C1H_SwordEquipOff::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Unequip_Needle);
	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Unequip_Needle, 1.5f);

	m_pSilvermane->Set_EquipWeapon(false);
	m_pSilvermane->Set_WeaponFixedBone("spine_03");

	return S_OK;
}

_int C1H_SwordEquipOff::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR1_01")))
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
