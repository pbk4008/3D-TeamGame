#include "pch.h"
#include "Material_Level.h"
#include "MaterialCamera.h"
#include "ModelObject.h"
#include "Mouse.h"

CMaterial_Level::CMaterial_Level()
{
}

CMaterial_Level::CMaterial_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CMaterial_Level::NativeConstruct()
{
	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;
	if (FAILED(Init_Camera()))
		return E_FAIL;
	if (FAILED(Init_MaterialObject()))
		return E_FAIL;
	
	return S_OK;
}

_int CMaterial_Level::Tick(_double fDeltaTime)
{
	return _uint();
}

HRESULT CMaterial_Level::Render()
{
	return S_OK;
}

HRESULT CMaterial_Level::Init_Camera()
{
	g_pGameInstance->Add_Prototype(L"MaterialCamera", CMaterialCamera::Create(m_pDevice, m_pDeviceContext));

	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"MainCamera";
	tDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	tDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	tDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);
	tDesc.fFovy = XMConvertToRadians(60.f);;
	tDesc.fFar = 300.f;
	tDesc.fNear = 0.1f;
	tDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	
	g_pGameInstance->Add_GameObjectToLayer(0, L"Static", L"MaterialCamera", &tDesc);

	return S_OK;
}

HRESULT CMaterial_Level::Init_MaterialObject()
{
	if (FAILED(g_pGameInstance->Add_Prototype(L"Object", CModelObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	return S_OK;
}


CMaterial_Level* CMaterial_Level::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMaterial_Level* pInstance = new CMaterial_Level(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CMaterial_Level Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMaterial_Level::Free()
{
	CLevel::Free();
}
