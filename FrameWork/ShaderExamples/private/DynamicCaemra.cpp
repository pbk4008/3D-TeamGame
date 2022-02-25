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
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamicCaemra::NativeConstruct(void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CDynamicCaemra::Tick(_double TimeDelta)
{


	return _int();
}

CDynamicCaemra* CDynamicCaemra::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CDynamicCaemra* pInstance = new CDynamicCaemra(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CDynamicCaemra");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDynamicCaemra::Clone(void* pArg)
{
	CDynamicCaemra* pInstance = new CDynamicCaemra(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating  Clone CDynamicCaemra");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CDynamicCaemra::SetUp_Components()
{

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

	if (FAILED(__super::SetUp_Components((_uint)LEVEL::LEVEL_STATIC, TEXT("Camera"), TEXT("CamCom"), (CComponent**)&m_pCamCom, &camdesc)))
		return E_FAIL;

	return S_OK;
}

void CDynamicCaemra::Free()
{
	Safe_Release(m_pCamCom);
}