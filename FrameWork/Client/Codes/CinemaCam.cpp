#include "pch.h"
#include "CinemaCam.h"

CCinemaCam::CCinemaCam()
	:m_pCamera(nullptr)
{
}

CCinemaCam::CCinemaCam(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pCamera(nullptr)
{
}

CCinemaCam::CCinemaCam(const CCinemaCam& rhs)
	: CGameObject(rhs)
	, m_pCamera(rhs.m_pCamera)
{
	Safe_AddRef(m_pCamera);
}

HRESULT CCinemaCam::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;
		

	return S_OK;
}

HRESULT CCinemaCam::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	CCamera::CAMERADESC tDesc = (*(CCamera::CAMERADESC*)pArg);

	m_pCamTag = tDesc.pCameraTag;
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Camera", L"CameraCom", (CComponent**)&m_pCamera, &tDesc)))
		return E_FAIL;

	return S_OK;
}

_int CCinemaCam::Tick(_double TimeDelta)
{
	m_pCamera->Update_Matrix(m_pTransform->Get_WorldMatrix());
	return _int();
}

_int CCinemaCam::LateTick(_double TimeDelta)
{
	return _int();
}

void CCinemaCam::Change_CurrentCam()
{
	g_pGameInstance->Change_BaseCamera(m_pCamTag);
}



CCinemaCam* CCinemaCam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CCinemaCam* pInstance = new CCinemaCam(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCinemaCam Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCinemaCam::Clone(const _uint iSceneID, void* pArg)
{
	CCinemaCam* pInstance = new CCinemaCam(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CCinemaCam Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCinemaCam::Free()
{
	CGameObject::Free();
	Safe_Release(m_pCamera);
}
