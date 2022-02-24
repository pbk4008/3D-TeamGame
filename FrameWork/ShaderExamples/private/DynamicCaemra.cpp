#include "DynamicCaemra.h"
#include "GameInstance.h"

CDynamicCaemra::CDynamicCaemra(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice,pDeviceContext)
{
}

CDynamicCaemra::CDynamicCaemra(const CDynamicCaemra& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDynamicCaemra::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CDynamicCaemra::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CDynamicCaemra::Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

CDynamicCaemra* CDynamicCaemra::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	return nullptr;
}

CGameObject* CDynamicCaemra::Clone(void* pArg)
{
	return nullptr;
}

void CDynamicCaemra::Free()
{
}

HRESULT CDynamicCaemra::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 7.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::SetUp_Components((_uint)LEVEL::LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	CCamera::CAMERADESC camdesc;
	camdesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	camdesc.pCameraTag = L"Dynamic";
	camdesc.vEye = _float4(0.0f, 0.0f, -1.0f, 0.f);
	camdesc.vAt = _float4(0.0f, 0.0f, 0.0f, 0.f);
	camdesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);
	camdesc.fFovy = XMConvertToRadians(60.f);
	camdesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	camdesc.fNear = 0.2f;
	camdesc.fFar = 300.f;

	if (FAILED(__super::SetUp_Components((_uint)LEVEL::LEVEL_STATIC, TEXT("PtotocomponentCamera"), TEXT("CamCom"), (CComponent**)&m_pCamCom, &camdesc)))
		return E_FAIL;

	return S_OK;
}
