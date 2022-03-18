#include "State.h"
#include "StateController.h"

CState::CState(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: m_pDevice(_pDevice)
	, m_pDeviceContext(_pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CState::CState(const CState& _rhs)
	: m_pDevice(_rhs.m_pDevice)
	, m_pDeviceContext(_rhs.m_pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CState::NativeConstruct(void* _pArg)
{
	return S_OK;
}

_int CState::Tick(const _double& _dDeltaTime)
{
	return _int();
}

_int CState::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CState::Render()
{
	return S_OK;
}

HRESULT CState::EnterState()
{
	return S_OK;
}

HRESULT CState::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CState::ExitState()
{
	return S_OK;
}

HRESULT CState::ExitState(void* pArg)
{
	return S_OK;
}

const wstring& CState::Get_Tag() const
{
	return m_wstrTag;
}

void CState::Set_StateController(CStateController* _pStateController)
{
	m_pStateController = _pStateController;
}

void CState::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
