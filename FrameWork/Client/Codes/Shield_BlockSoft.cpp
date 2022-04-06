#include "pch.h"
#include "Shield_BlockSoft.h"

CShield_BlockSoft::CShield_BlockSoft(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Block(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_BlockSoft::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_BlockSoft::Tick(const _double& _dDeltaTime)
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

_int CShield_BlockSoft::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_BlockSoft::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_BlockSoft::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Block_Soft", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	m_iCutIndex = 15;
	return S_OK;
}

HRESULT CShield_BlockSoft::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CShield_BlockSoft::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

CShield_BlockSoft* CShield_BlockSoft::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_BlockSoft* pInstance = new CShield_BlockSoft(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_BlockSoft Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShield_BlockSoft::Free()
{
	__super::Free();
}
