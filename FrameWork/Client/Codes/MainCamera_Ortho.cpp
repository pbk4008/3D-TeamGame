#include "pch.h"
#include "MainCamera_Ortho.h"

CMainCamera_Ortho::CMainCamera_Ortho()
	: m_pCamera(nullptr)
{
}

CMainCamera_Ortho::CMainCamera_Ortho(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pCamera(nullptr)
{
}

CMainCamera_Ortho::CMainCamera_Ortho(const CMainCamera_Ortho& rhs)
	: CGameObject(rhs)
	, m_pCamera(rhs.m_pCamera)
{
	Safe_AddRef(m_pCamera);
}

HRESULT CMainCamera_Ortho::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CMainCamera_Ortho::NativeConstruct(void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMainCamera_Ortho::Tick(_double fDeltaTime)
{
	m_pCamera->Update_Matrix(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CMainCamera_Ortho::LateTick(_double fDeltaTime)
{
	return _int();
}

HRESULT CMainCamera_Ortho::Render()
{
	return S_OK;
}

CGameObject* CMainCamera_Ortho::Clone(void* pArg)
{
	CMainCamera_Ortho* pInstance = new CMainCamera_Ortho(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMainCamera_Ortho Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMainCamera_Ortho* CMainCamera_Ortho::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMainCamera_Ortho* pInstance = new CMainCamera_Ortho(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMainCamera_Ortho Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

HRESULT CMainCamera_Ortho::Ready_GameObject(void* pArg)
{
	CCamera::CAMERADESC tDesc = (*(CCamera::CAMERADESC*)pArg);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Camera", L"MainOrthoCamera", (CComponent * *)& m_pCamera, &tDesc)))
		return E_FAIL;

	return S_OK;
}

void CMainCamera_Ortho::Free()
{
	CGameObject::Free();
	Safe_Release(m_pCamera);
}
