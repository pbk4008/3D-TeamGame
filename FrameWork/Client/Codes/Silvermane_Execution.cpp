#include "pch.h"
#include "Silvermane_Execution.h"

CSilvermane_Execution::CSilvermane_Execution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_Execution::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_Execution::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CSilvermane_Execution::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_Execution::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_Execution::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_Execution::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CSilvermane_Execution::Hit(const ATTACKDESC& _tAttackDesc)
{
}

_int CSilvermane_Execution::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

void CSilvermane_Execution::Free()
{
	__super::Free();
}
