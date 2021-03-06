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

	CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Default", L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", CMaterial::EType::Static);
	Add_Material(L"Default", pMtrl);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Default_Anim", L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", CMaterial::EType::Anim);
	Add_Material(L"Default_Anim", pMtrl);

	return S_OK;
}

HRESULT CMaterial_Manager::Add_Material(const wstring& _wstrMtrlTag, CMaterial* _pMtrl)
{
	auto& iter_find = m_umapMaterials.find(_wstrMtrlTag);
	if (m_umapMaterials.end() != iter_find)
	{
		Safe_Release(_pMtrl);
		return S_OK;
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
