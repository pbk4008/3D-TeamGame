#include "pch.h"
#include "Shield_Parry.h"

CShield_Parry::CShield_Parry(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_Parry::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_Parry::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_BlockLoop")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

_int CShield_Parry::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_Parry::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_Parry::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Parry_V2", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	m_pSilvermane->Set_EquipShield(true);
	m_pSilvermane->Set_IsHit(true);

	m_iCutIndex = 25;
	return S_OK;
}

HRESULT CShield_Parry::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsHit(false);
	return S_OK;
}

_int CShield_Parry::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurkeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (m_iCutIndex < iCurkeyFrameIndex)
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			m_pSilvermane->Set_EquipShield(false);
			m_pSilvermane->Set_EquipShieldAnim(false);
			m_pSilvermane->Set_BlockTime(0.f);
			return ToAttack();
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			m_pSilvermane->Set_EquipShield(false);
			m_pSilvermane->Set_EquipShieldAnim(false);
			m_pSilvermane->Set_BlockTime(0.f);
			return ToChargeStart();
		}
	}
			
	return _int();
}

CShield_Parry* CShield_Parry::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void * _pArg)
{
	CShield_Parry* pInstance = new CShield_Parry(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_Parry Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShield_Parry::Free()
{
	__super::Free();
}
