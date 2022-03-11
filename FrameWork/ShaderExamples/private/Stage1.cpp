#include "framework.h"
#include "Stage1.h"
#include "GameInstance.h"

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
	LightDesc.vDirection = _float3(1.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vPosition = _float3(-100.f, 100.f, -100.f);

	_float3 up = _float3(0, 1.f, 0);
	_float3 lookat = _float3(10.f, 1.f, 10.f);

	_vector		vPosition = XMLoadFloat3(&LightDesc.vPosition);
	vPosition = XMVectorSetW(vPosition, 1.f);

	_vector		vLook = XMLoadFloat3(&lookat) - XMLoadFloat3(&LightDesc.vPosition);
	vLook = XMVector3Normalize(vLook);

	_vector		vRight = XMVector3Cross(XMLoadFloat3(&up), vLook);
	vRight = XMVector3Normalize(vRight);

	_vector		vUp = XMVector3Cross(vLook, vRight);
	vUp = XMVector3Normalize(vUp);

	_matrix lightcam;
	lightcam.r[0] = vRight;
	lightcam.r[1] = vUp;
	lightcam.r[2] = vLook;
	lightcam.r[3] = vPosition;

	_vector origin = { 0,0,0,0 };
	_float3	forigin;
	_float fleft, fright, ftop, fbottom, fradius;

	fradius = 10.f;

	LightDesc.mLightView = XMMatrixInverse(nullptr, lightcam);

	origin = XMVector3TransformCoord(origin, LightDesc.mLightView);
	XMStoreFloat3(&forigin, origin);

	fleft = forigin.x - fradius;
	fright = forigin.x + fradius;
	fbottom = forigin.y - fradius;
	ftop = forigin.y + fradius;

	LightDesc.mLightProj = XMMatrixOrthographicLH(fright - fleft, ftop - fbottom, 0.1f, 1000.f);

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Gameobject()
{
	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"LayerFloor", L"Floor");

	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"LayerSky", L"Sky");

	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"LayerPlayer", L"Player");
	
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
