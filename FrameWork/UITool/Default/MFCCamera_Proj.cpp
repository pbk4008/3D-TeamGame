#include "framework.h"
#include "pch.h"
#include "MFCCamera_Proj.h"
#include "GameInstance.h"

#include "Mouse.h"

CMFCCamera_Proj::CMFCCamera_Proj(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMFCCamera_Proj::CMFCCamera_Proj(const CMFCCamera_Proj& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMFCCamera_Proj::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMFCCamera_Proj::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	CCamera::CAMERADESC Desc;
	Desc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	Desc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	Desc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	Desc.vAxisY = _float4(0.f, 1.f, 0.f, 1.f);

	Desc.fFovy = XMConvertToRadians(90.f);
	Desc.fAspect = _float(WINCX) / _float(WINCY);
	Desc.fNear = 0.01f;
	Desc.fFar = 300.f;

	Desc.pCameraTag = L"MFCCamera_Proj";

	m_pCameraCom = g_pGameInstance->Clone_Component<CCamera>(0, L"Camera", &Desc);

	if (FAILED(SetUp_Components(L"MFCCamera_Proj", m_pCameraCom)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CMFCCamera_Proj::Tick(_double TimeDelta)
{
	m_pCameraCom->Update_Matrix(m_pTransform->Get_WorldMatrix());

	if (g_pGameInstance->getkeyDown(DIK_W))
	{
		m_pTransform->Go_Straight(TimeDelta);
	}

	return 0;
}

_int CMFCCamera_Proj::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CMFCCamera_Proj::Render()
{
	return S_OK;
}

CMFCCamera_Proj* CMFCCamera_Proj::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMFCCamera_Proj* pInstance = new CMFCCamera_Proj(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMFCCamera_Proj");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMFCCamera_Proj::Clone(void* pArg)
{
	CMFCCamera_Proj* pInstance = new CMFCCamera_Proj(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CMFCCamera_Proj");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMFCCamera_Proj::Free()
{
	__super::Free();
}
