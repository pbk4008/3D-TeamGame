#include "pch.h"
#include "Shield_BlockEnd.h"

CShield_BlockEnd::CShield_BlockEnd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Block(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_BlockEnd::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 10;

	return S_OK;
}

_int CShield_BlockEnd::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
		m_pSilvermane->Set_EquipShield(false);

	if (m_pAnimationController->Is_Finished())
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
	}

	return _int();
}

_int CShield_BlockEnd::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_BlockEnd::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_BlockEnd::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;
	g_pGameInstance->BlendSound(L"Shield_Off", L"Needle_Attack_L_4_1", CSoundMgr::CHANNELID::Unequip_Shield, CSoundMgr::CHANNELID::Power_Resonance_01);
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Block_End", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	m_pSilvermane->Set_EquipShieldAnim(false);
	//m_pSilvermane->Set_IsTrasceCamera(false);
	return S_OK;
}

HRESULT CShield_BlockEnd::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_EquipShield(false);
	//m_pSilvermane->Set_IsTrasceCamera(true);
	m_pSilvermane->Set_BlockTime(0.f);
	return S_OK;
}

void CShield_BlockEnd::Block(const ATTACKDESC& _tAttackDesc)
{
	Hit(_tAttackDesc);
}

_int CShield_BlockEnd::Input(const _double& _dDeltaTime)
{
	//_int iProgress = __super::KeyCheck(_dDeltaTime);
	//if (NO_EVENT != iProgress)
	//	return iProgress;
	if (g_pGameInstance->getkeyDown(DIK_Q))
	{
		if (m_pSilvermane->IsLootShield())
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			//if (m_pSilvermane->IsEquipWeapon())
			//{
				switch (m_pSilvermane->Get_WeaponType())
				{
				case CWeapon::EType::Sword_1H:
					if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR1_01")))
						return -1;
					return STATE_CHANGE;
					break;
				case CWeapon::EType::Hammer_2H:
					if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_01")))
						return -1;
					return STATE_CHANGE;
					break;
				}
			//}
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (m_pSilvermane->IsEquipWeapon())
			{
				switch (m_pSilvermane->Get_WeaponType())
				{
				case CWeapon::EType::Sword_1H:
					if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_Start")))
						return -1;
					return STATE_CHANGE;
					break;
				case CWeapon::EType::Hammer_2H:
					if (FAILED(m_pStateController->Change_State(L"2H_HammerChargeStage1_Start")))
						return -1;
					return STATE_CHANGE;
					break;
				}
			}
		}

		if (g_pGameInstance->getkeyPress(DIK_W) ||
			g_pGameInstance->getkeyPress(DIK_S) ||
			g_pGameInstance->getkeyPress(DIK_D) ||
			g_pGameInstance->getkeyPress(DIK_A))
		{
			if (m_pSilvermane->IsEquipWeapon())
			{
				switch (m_pSilvermane->Get_WeaponType())
				{
				case CWeapon::EType::Sword_1H:
					if (FAILED(m_pStateController->Change_State(L"1H_SwordIdle")))
						return -1;
					return STATE_CHANGE;
					break;
				case CWeapon::EType::Hammer_2H:
					if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
						return -1;
					return STATE_CHANGE;
					break;
				}
			}
			else
			{
				if (FAILED(m_pStateController->Change_State(L"Idle")))
					return -1;
				return STATE_CHANGE;
			}
		}
	}

	return _int();
}

CShield_BlockEnd* CShield_BlockEnd::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_BlockEnd* pInstance = new CShield_BlockEnd(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_BlockEnd Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_BlockEnd::Free()
{

	__super::Free();
}
