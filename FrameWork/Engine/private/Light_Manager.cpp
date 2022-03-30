#include "Light_Manager.h"
#include "Light.h"


CLight_Manager::CLight_Manager()
{
}

void CLight_Manager::UpdateLightCam(_uint LightIndx, _fvector playerpos)
{
	m_Lights[LightIndx]->UpdateLightCam(playerpos);
}

const LIGHTDESC* CLight_Manager::Get_LightDesc(_uint iIndex)
{
	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::CreateLightCam(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,LIGHTDESC& desc)
{
	_vector up = { 0, 1.f, 0,0 };
	_vector lookat = { -1.f,1.f,1.f,0.f };

	_float3 dir = _float3(-1.f, -1.f, 1.f);
	_vector vdir = XMVector3Normalize(XMLoadFloat3(&desc.vDirection));

	if(desc.vPosition.x == 0 )
		XMStoreFloat3(&desc.vPosition, (vdir * desc.mOrthinfo[0] * -1.f) + lookat);

	desc.mLightView = XMMatrixLookAtLH(XMLoadFloat3(&desc.vPosition), lookat, up);

	_vector origin = { 0,0,0,0 };
	_float3	forigin;

	origin = XMVector3TransformCoord(origin, desc.mLightView);
	XMStoreFloat3(&forigin, origin);


	desc.mOrthinfo[1] = forigin.x - desc.mOrthinfo[0];
	desc.mOrthinfo[2] = forigin.x + desc.mOrthinfo[0];
	desc.mOrthinfo[3] = forigin.y - desc.mOrthinfo[0];
	desc.mOrthinfo[4] = forigin.y + desc.mOrthinfo[0];

	desc.mLightProj = XMMatrixOrthographicLH(desc.mOrthinfo[2] - desc.mOrthinfo[1], desc.mOrthinfo[4] - desc.mOrthinfo[3], 0.1f, 500.f);

	if (FAILED(Add_Light(pDevice, pDeviceContext, desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLight_Manager::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC & LightDesc)
{
	CLight*		pLight = CLight::Create(pDevice, pDeviceContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);		

	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(const wstring& pCameraTag,_bool pbr, _bool shadow)
{
	for (auto& pLight : m_Lights)
		pLight->Render(pCameraTag, pbr, shadow);

	return S_OK;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
