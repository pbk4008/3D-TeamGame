#include "pch.h"
#include "1H_SwordEquipOn.h"

#include"Silvermane.h"

C1H_SwordEquipOn::C1H_SwordEquipOn(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordEquipOn::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 11;

	return S_OK;
}

_int C1H_SwordEquipOn::Tick(const _double& _dDeltaTime)
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
		if (FAILED(m_pStateController->Change_State(L"1H_SwordIdle")))
			return -1;
		return STATE_CHANGE;
	}


	return _int();
}

_int C1H_SwordEquipOn::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordEquipOn::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordEquipOn::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->Play_Shot(L"Needle_Equip", CSoundMgr::CHANNELID::Equip_Needle);

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Equip_On", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	return S_OK;
}

HRESULT C1H_SwordEquipOn::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Equip_Needle);


	return S_OK;
}

_int C1H_SwordEquipOn::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{

	}

	return _int();
}

C1H_SwordEquipOn* C1H_SwordEquipOn::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordEquipOn* pInstance = new C1H_SwordEquipOn(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordEquipOn Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordEquipOn::Free()
{
	
	__super::Free();
}
