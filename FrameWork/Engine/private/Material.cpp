#include "Material.h"

CMaterial::CMaterial()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pEffect(nullptr)
	, m_isCloned(false)
{
	
}

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_pEffect(nullptr)
	, m_isCloned(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CMaterial::CMaterial(const CMaterial& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDeviceContext(rhs.m_pDeviceContext)
	, m_pEffect(rhs.m_pEffect)
	, m_PassDesc(rhs.m_PassDesc)
	, m_isCloned(true)
	, m_pMeshTexture(rhs.m_pMeshTexture)
	, m_pMeshTextureName(rhs.)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	for (auto& pPassDesc : m_PassDesc)
	{
		Safe_AddRef(pPassDesc->pInputLayout);
		Safe_AddRef(pPassDesc->pPass);
	}
	Safe_AddRef(m_pEffect);

	for (auto& pTexture : m_pMeshTexture)
		Safe_AddRef(pTexture);
}

HRESULT CMaterial::Native_Construct_ProtoType()
{
	return S_OK;
}

HRESULT CMaterial::Render()
{
	return S_OK;
}

CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMaterial* pInstance = new CMaterial(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_ProtoType()))
	{
		MSGBOX("CMaterial Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}

void CMaterial::Free()
{
	if (false == m_isCloned)
	{
		for (auto& pPassDesc : m_PassDesc)
			Safe_Delete(pPassDesc);
	}
	m_PassDesc.clear();
	Safe_Release(m_pEffect);

	for (auto& pTexture : m_pMeshTexture)
		Safe_Release(pTexture);
	m_pMeshTexture.clear();
}
