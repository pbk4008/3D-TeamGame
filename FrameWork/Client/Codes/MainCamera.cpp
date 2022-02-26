#include "pch.h"
#include "MainCamera.h"

CMainCamera::CMainCamera()
	: m_pCamera(nullptr)
{
}

CMainCamera::CMainCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pCamera(nullptr)
{
}

CMainCamera::CMainCamera(const CMainCamera& rhs)
	: CGameObject(rhs)
	, m_pCamera(rhs.m_pCamera)
{
	Safe_AddRef(m_pCamera);
}

HRESULT CMainCamera::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMainCamera::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CMainCamera::Tick(_double fDeltaTime)
{
	return _int();
}

_int CMainCamera::LateTick(_double fDeltaTime)
{
	return _int();
}

HRESULT CMainCamera::Render()
{
	return S_OK;
}

CGameObject* CMainCamera::Clone(void* pArg)
{
	CMainCamera* pInstance = new CMainCamera(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMainCamera Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMainCamera* CMainCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMainCamera* pInstance = new CMainCamera(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMainCamera Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

HRESULT CMainCamera::Ready_GameObject()
{
	return S_OK;
}

void CMainCamera::Free()
{
	CGameObject::Free();
	Safe_Release(m_pCamera);
}
