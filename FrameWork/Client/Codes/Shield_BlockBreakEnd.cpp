#include "pch.h"
#include "Shield_BlockBreakEnd.h"

CShield_BlockBreakEnd::CShield_BlockBreakEnd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Block(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_BlockBreakEnd::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_BlockBreakEnd::Tick(const _double& _dDeltaTime)
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

_int CShield_BlockBreakEnd::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_BlockBreakEnd::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_BlockBreakEnd::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Block_Break_End", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	m_iCutIndex = 40;

	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pSilvermane->Set_IsHit(true);
	return S_OK;
}

HRESULT CShield_BlockBreakEnd::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pSilvermane->Set_IsHit(false);
	return S_OK;
}

_int CShield_BlockBreakEnd::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

CShield_BlockBreakEnd* CShield_BlockBreakEnd::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_BlockBreakEnd* pInstance = new CShield_BlockBreakEnd(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_BlockBreakEnd Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShield_BlockBreakEnd::Free()
{
	__super::Free();
}
