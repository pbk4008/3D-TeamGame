#include "pch.h"
#include "SubEnvironment.h"

CSubEnvironment::CSubEnvironment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLandScape(pDevice, pDeviceContext)
	, m_pInstanceMesh(nullptr)
	, m_iInstanceCnt(0)
{
}

CSubEnvironment::CSubEnvironment(const CSubEnvironment& rhs)
	: CLandScape(rhs)
	, m_pInstanceMesh(rhs.m_pInstanceMesh)
	, m_tEnvironmentDesc(rhs.m_tEnvironmentDesc)
	, m_vecUsingMatrix(rhs.m_vecUsingMatrix)
	, m_iInstanceCnt(rhs.m_iInstanceCnt)
{
	Safe_AddRef(m_pInstanceMesh);
}

HRESULT CSubEnvironment::NativeConstruct_Prototype()
{
	if (FAILED(CLandScape::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSubEnvironment::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	m_tEnvironmentDesc = (*(ENVIRONMENTDESC*)pArg);
	m_tEnvironmentDesc.tInstanceDesc.iNumInstance = (_uint)m_tEnvironmentDesc.tInstanceDesc.vecMatrix.size();

	m_iInstanceCnt = m_tEnvironmentDesc.tInstanceDesc.iNumInstance;

	m_vecUsingMatrix = m_tEnvironmentDesc.tInstanceDesc.vecMatrix;

	if (FAILED(CLandScape::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CSubEnvironment::Tick(_double TimeDelta)
{
	return _int();
}

_int CSubEnvironment::LateTick(_double TimeDelta)
{
	//if (FAILED(Culling()))
	//	return -1;

	if (m_pRenderer)
	{
		if (m_pRenderer->Get_RenderButton(CRenderer::SHADOW) == true)
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);

		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	}


	return _int();
}

HRESULT CSubEnvironment::Render()
{
	if (!m_pInstanceMesh)
		return E_FAIL;

	_float4 ClipPlane = _float4(0.f, 0.f, 0.f, 0.f);
	_matrix matWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix matView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix matProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector campos = g_pGameInstance->Get_CamPosition(wstrCamTag);

	m_pInstanceMesh->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_CamPos", &campos, sizeof(_vector));
	m_pInstanceMesh->SetUp_ValueOnShader("ClipPlane", &ClipPlane, sizeof(_float4));

	for (_uint i = 0; i < m_Nummeshcontainer; i++)
		m_pInstanceMesh->Render(i, 0);

	return S_OK;
}

HRESULT CSubEnvironment::Render_Shadow()
{
	_matrix world, lightview, ligtproj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	lightview = XMMatrixTranspose(m_LightDesc->mLightView);
	ligtproj = XMMatrixTranspose(m_LightDesc->mLightProj);
	_float3 lightpos = m_LightDesc->vPosition;

	m_pInstanceMesh->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_LightView", &lightview, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_LightProj", &ligtproj, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_LightPos", &lightpos, sizeof(_float3));

	for (_uint i = 0; i < m_Nummeshcontainer; i++)
		m_pInstanceMesh->Render(i, 2);

	return S_OK;

	return S_OK;
}

HRESULT CSubEnvironment::Ready_Component()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, m_tEnvironmentDesc.wstrInstaneTag, m_tEnvironmentDesc.wstrInstaneTag, (CComponent**)&m_pInstanceMesh, &m_tEnvironmentDesc.tInstanceDesc)))
		return E_FAIL;

	m_Nummeshcontainer = m_pInstanceMesh->Get_NumMeshContainer();

	m_LightDesc = g_pGameInstance->Get_LightDesc(0);

	return S_OK;
}

HRESULT CSubEnvironment::Culling()
{
	for (_uint i = 0; i < m_iInstanceCnt; i++)
	{
		_matrix matTmp = XMLoadFloat4x4(&m_tEnvironmentDesc.tInstanceDesc.vecMatrix[i]);
		_vector vPos = matTmp.r[3];
		if (!g_pGameInstance->isIn_WorldFrustum(vPos, 50.f))
			ZeroMemory(&m_vecUsingMatrix[i], sizeof(_float4x4));
		else
			m_vecUsingMatrix[i] = m_tEnvironmentDesc.tInstanceDesc.vecMatrix[i];
	}

	m_pInstanceMesh->Update_InstanceBuffer(m_vecUsingMatrix);

	return S_OK;
}

CSubEnvironment* CSubEnvironment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CSubEnvironment* pInstance = new CSubEnvironment(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CEnvironment Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CSubEnvironment* CSubEnvironment::Clone(const _uint _iSceneID, void* pArg)
{
	CSubEnvironment* pInstance = new CSubEnvironment(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CEnvironment Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSubEnvironment::Free()
{
	CLandScape::Free();
	Safe_Release(m_pInstanceMesh);
}
