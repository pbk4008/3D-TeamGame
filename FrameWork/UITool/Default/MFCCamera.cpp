#include "framework.h"
#include "pch.h"
#include "MFCCamera.h"
#include "GameInstance.h"

#include "Mouse.h"

CMFCCamera::CMFCCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMFCCamera::CMFCCamera(const CMFCCamera& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMFCCamera::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	

	return S_OK;
}

HRESULT CMFCCamera::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	CCamera::CAMERADESC Desc;
	Desc.eType = CCamera::CAMERATYPE::CAMERA_ORTHO;
	Desc.fWinCX = WINCX;
	Desc.fWinCY = WINCY;
	Desc.fNear = 0.01f;
	Desc.fFar = 1.f;
	Desc.pCameraTag = L"MFCCamera";

	m_pCameraCom = g_pGameInstance->Clone_Component<CCamera>(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Proto_Component_Camera", &Desc);

	if (FAILED(SetUp_Components(L"MFCCamera", m_pCameraCom)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CMFCCamera::Tick(_double TimeDelta)
{
	m_pCameraCom->Update_Matrix(m_pTransform->Get_WorldMatrix());

	return 0;
}

_int CMFCCamera::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CMFCCamera::Render()
{
	return S_OK;
}

CMFCCamera* CMFCCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMFCCamera* pInstance = new CMFCCamera(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMFCCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMFCCamera::Clone(void* pArg)
{
	CMFCCamera* pInstance = new CMFCCamera(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CMFCCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMFCCamera::Free()
{
	Safe_Release(m_pCameraCom);
	__super::Free();
}
