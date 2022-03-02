#include "pch.h"
#include "State_Silvermane.h"

CState_Silvermane::CState_Silvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState(_pDevice, _pDeviceContext)
{
}

CState_Silvermane::CState_Silvermane(const CState_Silvermane& _rhs)
	: CState(_rhs)
{
}

HRESULT CState_Silvermane::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CState_Silvermane::Tick(const _double& _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}

	if (0 > KeyCheck(_dDeltaTime))
	{
		return -1;
	}

	return _int();
}

_int CState_Silvermane::LateTick(const _double& _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
	}

	return _int();
}

HRESULT CState_Silvermane::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CState_Silvermane::EnterState()
{
	if (FAILED(__super::EnterState()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CState_Silvermane::ExitState()
{
	if (FAILED(__super::ExitState()))
	{
		return E_FAIL;
	}

	return E_NOTIMPL;
}

void CState_Silvermane::Set_Model(CModel* _pModel)
{
	m_pModel = _pModel;
}

void CState_Silvermane::Set_Silvermane(CSilvermane* _pSilvermane)
{
	m_pSilvermane = _pSilvermane;
}

void CState_Silvermane::Set_Transform(CTransform* _pTransform)
{
	m_pTransform = _pTransform;
}

_int CState_Silvermane::KeyCheck(const _double& _dDeltaTime)
{
	return _int();
}

void CState_Silvermane::Free()
{

	__super::Free();
}
