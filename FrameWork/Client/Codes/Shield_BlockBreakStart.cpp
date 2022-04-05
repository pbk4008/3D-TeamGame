#include "pch.h"
#include "Shield_BlockBreakStart.h"

CShield_BlockBreakStart::CShield_BlockBreakStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Block(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_BlockBreakStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_BlockBreakStart::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_BlockBreakEnd")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

_int CShield_BlockBreakStart::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_BlockBreakStart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_BlockBreakStart::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Block_Break_Start", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	m_pSilvermane->Set_IsTrasceCamera(false);
	return S_OK;
}

HRESULT CShield_BlockBreakStart::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	return S_OK;
}

_int CShield_BlockBreakStart::Input(const _double& _dDeltaTime)
{
	//_int iProgress = __super::Input(_dDeltaTime);
	//if (NO_EVENT != iProgress)
	//	return iProgress;

	return _int();
}

CShield_BlockBreakStart* CShield_BlockBreakStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_BlockBreakStart* pInstance = new CShield_BlockBreakStart(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_BlockBreakStart Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShield_BlockBreakStart::Free()
{
	__super::Free();
}
