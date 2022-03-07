#include"pch.h"
#include "Environment.h"
#include "MeshCollider.h"

CEnvironment::CEnvironment()
	: m_pNaviMesh(nullptr)
	, m_pInstanceMesh(nullptr)
{
	ZeroMemory(&m_tEnvironmentDesc, sizeof(ENVIRONMENTDESC));
}

CEnvironment::CEnvironment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLandScape(pDevice, pDeviceContext)
	, m_pInstanceMesh(nullptr)
	, m_pNaviMesh(nullptr)
{
	ZeroMemory(&m_tEnvironmentDesc, sizeof(ENVIRONMENTDESC));
}

CEnvironment::CEnvironment(const CEnvironment& rhs)
	: CLandScape(rhs)
	, m_pNaviMesh(rhs.m_pNaviMesh)
	, m_pInstanceMesh(rhs.m_pInstanceMesh)
	, m_tEnvironmentDesc(rhs.m_tEnvironmentDesc)
{
	Safe_AddRef(m_pNaviMesh);
	Safe_AddRef(m_pInstanceMesh);
}

HRESULT CEnvironment::NativeConstruct_Prototype()
{
	if (FAILED(CLandScape::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnvironment::NativeConstruct(void* pArg)
{
	m_tEnvironmentDesc = (*(ENVIRONMENTDESC*)pArg);
	m_tEnvironmentDesc.tInstanceDesc.iNumInstance = (_uint)m_tEnvironmentDesc.tInstanceDesc.vecMatrix.size();
	if (FAILED(CLandScape::NativeConstruct(pArg)))
		return E_FAIL;
	
	if (FAILED(Ready_Component()))
		return E_FAIL;
	return S_OK;
}

_int CEnvironment::Tick(_double TimeDelta)
{
	return _int();
}

_int CEnvironment::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
	return _int();
}

HRESULT CEnvironment::Render()
{
	if (!m_pInstanceMesh)
		return E_FAIL;

	_matrix matWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix matView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix matProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pInstanceMesh->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix));

	_uint iNumMeshCnt = m_pInstanceMesh->Get_NumMeshContainer();
	for (_uint i = 0; i < iNumMeshCnt; i++)
	{
		m_pInstanceMesh->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pInstanceMesh->Render(i, 0);
	}
//
//#ifdef _DEBUG
//	m_pNaviMesh->Render(L"MainCamera");
//#endif
	return S_OK;
}

HRESULT CEnvironment::Ready_Component()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STAGE1, m_tEnvironmentDesc.wstrInstaneTag, m_tEnvironmentDesc.wstrInstaneTag, (CComponent * *)& m_pInstanceMesh, &m_tEnvironmentDesc.tInstanceDesc)))
		return E_FAIL;

	return S_OK; 
}

CEnvironment* CEnvironment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEnvironment* pInstance = new CEnvironment(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CEnvironment Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEnvironment::Clone(void* pArg)
{
	CEnvironment* pInstance = new CEnvironment(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CEnvironment Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnvironment::Free()
{
	CLandScape::Free();
	Safe_Release(m_pNaviMesh);
	Safe_Release(m_pInstanceMesh);
}
