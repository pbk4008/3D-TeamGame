#include "pch.h"
#include "Shield_BlockStart.h"

CShield_BlockStart::CShield_BlockStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Block(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_BlockStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 10;

	return S_OK;
}

_int CShield_BlockStart::Tick(const _double& _dDeltaTime)
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

_int CShield_BlockStart::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_BlockStart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_BlockStart::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Block_Start", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	m_pSilvermane->Set_EquipShield(true);
	m_pSilvermane->Set_EquipShieldAnim(true);

	//m_pSilvermane->Set_IsTrasceCamera(false);

	return S_OK;
}

HRESULT CShield_BlockStart::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//m_pSilvermane->Set_IsTrasceCamera(true);
	return S_OK;
}

_int CShield_BlockStart::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

CShield_BlockStart* CShield_BlockStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_BlockStart* pInstance = new CShield_BlockStart(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_BlockStart Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_BlockStart::Free()
{

	__super::Free();
}
