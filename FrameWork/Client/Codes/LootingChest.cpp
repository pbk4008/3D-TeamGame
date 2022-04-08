#include "pch.h"
#include "LootingChest.h"

#include "DropBox.h"

CLootingChest::CLootingChest(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CLootingChest::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CLootingChest::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	//m_pSilvermane->Raycast_DropBox(_dDeltaTime);

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (68 < iCurKeyFrameIndex)
	{
		CDropBox* pDropBox = m_pSilvermane->Get_TargetDropBox();
		if (pDropBox && !pDropBox->IsOpen())
		{
			pDropBox->Focus();
		}
	}

	if (!m_pSilvermane->Get_TargetDropBox() || m_pAnimationController->Is_Finished())
	{
		return ToIdle();
	}
	
	return _int();
}

_int CLootingChest::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CLootingChest::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLootingChest::EnterState(void* _pArg)
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Looting_Chest_Legendary_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	CDropBox* pDropBox = m_pSilvermane->Get_TargetDropBox();
	pDropBox->FocusEnter();
	m_pSilvermane->Set_IsBoxOpen(true);

	m_pSilvermane->Set_IsTrasceCamera(false);
	return S_OK;
}

HRESULT CLootingChest::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsBoxOpen(false);

	m_pSilvermane->Set_IsTrasceCamera(true);
	return S_OK;
}

_int CLootingChest::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyUp(DIK_C))
	{
		return ToIdle();
	}
	
	return _int();
}

_int CLootingChest::ToIdle()
{
	if (m_pSilvermane->IsEquipWeapon())
	{
		switch (m_pSilvermane->Get_WeaponType())
		{
		case CWeapon::EType::Sword_1H:
			if (FAILED(m_pStateController->Change_State(L"1H_SwordIdle")))
				return -1;
			break;
		case CWeapon::EType::Hammer_2H:
			if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
				return -1;
			break;
		}
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

CLootingChest* CLootingChest::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CLootingChest* pInstance = new CLootingChest(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CLootingChest Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLootingChest::Free()
{
	__super::Free();
}
