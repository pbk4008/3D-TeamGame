#include "Material_Manager.h"

#include "Material.h"

CMaterial_Manager::CMaterial_Manager()
{
}

HRESULT CMaterial_Manager::NativeConstruct(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	m_pDevice = _pDevice;
	Safe_AddRef(m_pDevice);
	m_pDeviceContext = _pDeviceContext;
	Safe_AddRef(m_pDeviceContext);

	CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Mesh.hlsl");
	Add_Material(L"Default", pMtrl);

	return S_OK;
}

HRESULT CMaterial_Manager::Add_Material(const wstring& _wstrMtrlTag, CMaterial* _pMtrl)
{
	auto& iter_find = m_umapMaterials.find(_wstrMtrlTag);
	if (m_umapMaterials.end() != iter_find)
	{
		return E_FAIL;
	}
	m_umapMaterials.emplace(_wstrMtrlTag, _pMtrl);

	return S_OK;
}

CMaterial* CMaterial_Manager::Get_Material(const wstring& _wstrMtrlTag)
{
	auto& iter_find = m_umapMaterials.find(_wstrMtrlTag);
	if (m_umapMaterials.end() == iter_find)
	{
		return nullptr;
	}

	return iter_find->second;
}

void CMaterial_Manager::Free()
{
	for (auto& pair : m_umapMaterials)
	{
		Safe_Release(pair.second);
	}

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
