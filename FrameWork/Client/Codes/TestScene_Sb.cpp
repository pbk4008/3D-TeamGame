#include "pch.h"
#include "TestScene_Sb.h"

CTestScene_Sb::CTestScene_Sb()
{
}

CTestScene_Sb::CTestScene_Sb(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CTestScene_Sb::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct())) return E_FAIL;

	if (FAILED(Ready_LightDesc())) return E_FAIL;

	if (FAILED(Ready_Gameobject())) return E_FAIL;

	return S_OK;
}

_int CTestScene_Sb::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta)) return -1;
		

	return _int();
}

HRESULT CTestScene_Sb::Render()
{
	return S_OK;
}

HRESULT CTestScene_Sb::Ready_Gameobject()
{
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Plane", L"Proto_GameObject_Plane_Test")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Silvermane", L"Proto_GameObject_Silvermane")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Camera", L"Proto_GameObject_Camera_Silvermane")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Monster", L"Monster_Bastion_Marksman")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_SkyBox", L"Proto_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestScene_Sb::Ready_LightDesc()
{
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(-1.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);

	_vector up = { 0, 1.f, 0,0 };
	_vector lookat = { 0.f, 1.f, 0.f, 1.f };

	LightDesc.mOrthinfo[0] = 50.f;

	XMStoreFloat3(&LightDesc.vPosition, ((XMLoadFloat3(&LightDesc.vDirection) * LightDesc.mOrthinfo[0] * -1.f) + lookat));
	LightDesc.mLightView = XMMatrixLookAtLH(XMLoadFloat3(&LightDesc.vPosition), lookat, up);

	_vector origin = { 0,0,0,0 };
	_float3	forigin;

	origin = XMVector3TransformCoord(origin, LightDesc.mLightView);
	XMStoreFloat3(&forigin, origin);

	LightDesc.mOrthinfo[1] = forigin.x - LightDesc.mOrthinfo[0];
	LightDesc.mOrthinfo[2] = forigin.x + LightDesc.mOrthinfo[0];
	LightDesc.mOrthinfo[3] = forigin.y - LightDesc.mOrthinfo[0];
	LightDesc.mOrthinfo[4] = forigin.y + LightDesc.mOrthinfo[0];

	LightDesc.mLightProj = XMMatrixOrthographicLH(LightDesc.mOrthinfo[2] - LightDesc.mOrthinfo[1], LightDesc.mOrthinfo[4] - LightDesc.mOrthinfo[3], 0.1f, 300.f);

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	return S_OK;
}

CTestScene_Sb* CTestScene_Sb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTestScene_Sb* pInstance = new CTestScene_Sb(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CTestScene_Sb Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestScene_Sb::Free()
{

	__super::Free();
}