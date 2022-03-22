#include "framework.h"
#include "Stage1.h"
#include "GameInstance.h"
#include "DataLoader.h"

CStage1::CStage1()
{
}

CStage1::CStage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CStage1::NativeConstruct()
{
	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;

	if(FAILED(Ready_LightDesc()))
		return E_FAIL;

	if (FAILED(Ready_Gameobject()))
		return E_FAIL;

	if (FAILED(Ready_LoadMapData()))
		return E_FAIL;

	return S_OK;
}

_int CStage1::Tick(_double TimeDelta)
{
	return _int();
}

HRESULT CStage1::Render()
{
	return S_OK;
}

HRESULT CStage1::Ready_LightDesc()
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

HRESULT CStage1::Ready_Gameobject()
{
	//g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"LayerFloor", L"Floor");

	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"LayerSky", L"Sky");

	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"LayerPlayer", L"Player");

	return S_OK;
}

HRESULT CStage1::Ready_LoadMapData()
{
	CDataLoader* pDataLoader = CDataLoader::Create(m_pDevice, m_pDeviceContext);

	if (!pDataLoader)
		return E_FAIL;

	if (FAILED(pDataLoader->LoadMapData(L"../bin/Data/Stage_1.dat")))
		return E_FAIL;

	Safe_Release(pDataLoader);

	return S_OK;
}

CStage1* CStage1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CStage1* pInstance = new CStage1(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CStage1 Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStage1::Free()
{
	CLevel::Free();
}
