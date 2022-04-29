#include "pch.h"
#include "Silvermane_Heal.h"

CSilvermane_Heal::CSilvermane_Heal(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_Heal::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_Heal::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if(20 < iCurKeyFrameIndex && 80 > iCurKeyFrameIndex)
		m_pSilvermane->Add_HP(m_fValue * (_float)_dDeltaTime);

	if (m_pAnimationController->Is_Finished())
	{
		return ToIdle();
	}

	return _int();
}

_int CSilvermane_Heal::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_Heal::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_Heal::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Heal01_Player", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	m_pSilvermane->RimlightCheck(true, _float3(0, 0.5f, 0), 3.5f);
	m_pSilvermane->Set_HealActive(true);
	m_fValue = m_pSilvermane->Get_MaxHp() * 0.2f;

	m_pSilvermane->Get_InvenData()->UsingPotion();

	return S_OK;
}

HRESULT CSilvermane_Heal::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_HealActive(false);

	return S_OK;
}

_int CSilvermane_Heal::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

CSilvermane_Heal* CSilvermane_Heal::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_Heal* pInstance = new CSilvermane_Heal(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_Heal Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSilvermane_Heal::Free()
{
	__super::Free();
}
