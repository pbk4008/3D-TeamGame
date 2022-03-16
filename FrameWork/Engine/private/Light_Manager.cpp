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

HRESULT CLight_Manager::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC & LightDesc)
{
	CLight*		pLight = CLight::Create(pDevice, pDeviceContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);		

	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(const wstring& pCameraTag)
{
	for (auto& pLight : m_Lights)
		pLight->Render(pCameraTag, m_bPBRHDR);

	return S_OK;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
