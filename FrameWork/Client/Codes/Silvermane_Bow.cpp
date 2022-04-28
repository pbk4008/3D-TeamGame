#include "pch.h"
#include "Silvermane_Bow.h"

CSilvermane_Bow::CSilvermane_Bow(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_Bow::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_Bow::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_pAnimationController->Is_Finished())
		return ToIdle();

	return _int();
}

_int CSilvermane_Bow::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_Bow::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_Bow::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Emote_Bow_Player", true);
	m_pAnimationController->Set_RootMotion(false, false);

	_matrix smatPivot = m_pModel->Get_PivotMatrix();
	XMStoreFloat4x4(&m_matPrePivot, smatPivot);
	_matrix smatRot = XMMatrixRotationY(XMConvertToRadians(180.f));
	smatRot *= smatPivot;
	m_pModel->Set_PivotMatrix(smatRot);

	m_pSilvermane->Set_WeaponActive(false);

	m_pSilvermane->RimlightCheck(true, _float3(1.f, 1.f, 0.f));
	
	return S_OK;
}

HRESULT CSilvermane_Bow::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pModel->Set_PivotMatrix(XMLoadFloat4x4(&m_matPrePivot));
	m_pSilvermane->Set_WeaponActive(true);

	m_pSilvermane->RimlightCheck(false);
	return S_OK;
}

void CSilvermane_Bow::Hit(const ATTACKDESC& _tAttackDesc)
{
}

CSilvermane_Bow* CSilvermane_Bow::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_Bow* pInstance = new CSilvermane_Bow(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_Bow Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSilvermane_Bow::Free()
{
	__super::Free();
}
