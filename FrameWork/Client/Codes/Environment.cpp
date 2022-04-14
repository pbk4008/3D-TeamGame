#include "pch.h"
#include "Environment.h"
#include "MeshCollider.h"
#include "CullingBox.h"

CEnvironment::CEnvironment()
	: m_pInstanceMesh(nullptr)
{
}

CEnvironment::CEnvironment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLandScape(pDevice, pDeviceContext)
	, m_pInstanceMesh(nullptr)
	, m_iInstanceCnt(0)
{
}

CEnvironment::CEnvironment(const CEnvironment& rhs)
	: CLandScape(rhs)
	, m_pInstanceMesh(rhs.m_pInstanceMesh)
	, m_tEnvironmentDesc(rhs.m_tEnvironmentDesc)
	, m_vecUsingMatrix(rhs.m_vecUsingMatrix)
	, m_iInstanceCnt(rhs.m_iInstanceCnt)
{
	Safe_AddRef(m_pInstanceMesh);
}

HRESULT CEnvironment::NativeConstruct_Prototype()
{
	if (FAILED(CLandScape::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnvironment::NativeConstruct(const _uint _iSceneID, void* pArg)
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

_int CEnvironment::Tick(_double TimeDelta)
{
	//_vector vPos=g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	//cout << XMVectorGetX(vPos) << ", " << XMVectorGetY(vPos) << ", " << XMVectorGetZ(vPos) << endl;
	return _int();
}

_int CEnvironment::LateTick(_double TimeDelta)
{
	//if (FAILED(Culling()))
	//	return -1;
	//cout << "Hello World" << endl;

	if (m_pRenderer->Get_RenderButton(CRenderer::SHADOW) == true)
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_SHADOW, this))) return -1;
	}

	if(FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this))) return -1;

	//if (m_pRenderer->Get_RenderButton(CRenderer::VELOCITYBLUR) == true)
	//{
	//	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_VELOCITY, this)))
	//		return -1;
	//}

	return _int();
}

HRESULT CEnvironment::Render()
{
	if (!m_pInstanceMesh)
		return E_FAIL;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();

	_float4 ClipPlane = _float4(0.f, 0.f, 0.f, 0.f);
	_matrix matWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix matView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix matProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector campos = g_pGameInstance->Get_CamPosition(wstrCamTag);

	m_pInstanceMesh->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_CamPos", &campos, sizeof(_vector));
	m_pInstanceMesh->SetUp_ValueOnShader("ClipPlane", &ClipPlane, sizeof(_float4));

	//_matrix preworld, preview, preproj;
	//preworld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	//

	//if (FAILED(m_pInstanceMesh->SetUp_ValueOnShader("g_prvWmat", &preworld, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	//if (FAILED(m_pInstanceMesh->SetUp_ValueOnShader("g_prvVmat", &preview, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	//if (FAILED(m_pInstanceMesh->SetUp_ValueOnShader("g_prvPmat", &preproj, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	for (_uint i = 0; i < m_Nummeshcontainer; i++)
		m_pInstanceMesh->Render(i, 0);
	
	/*m_PreWroldMat = m_pTransform->Get_WorldMatrix();*/

	return S_OK;
}

HRESULT CEnvironment::Render_Shadow()
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
}

HRESULT CEnvironment::Render_Velocity()
{
	if (!m_pInstanceMesh)
		return E_FAIL;

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();

	_float4 ClipPlane = _float4(0.f, 0.f, 0.f, 0.f);
	_matrix matWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix matView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix matProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector campos = g_pGameInstance->Get_CamPosition(wstrCamTag);

	m_pInstanceMesh->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix));
	m_pInstanceMesh->SetUp_ValueOnShader("g_CamPos", &campos, sizeof(_vector));
	m_pInstanceMesh->SetUp_ValueOnShader("ClipPlane", &ClipPlane, sizeof(_float4));

	_float4x4 rot;
	XMStoreFloat4x4(&rot, m_pTransform->Get_WorldMatrix()
		* g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW)
		* g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	rot._11 = 1.0f; rot._22 = 1.0f; rot._33 = 1.0f;
	rot._41 = 0.0f; rot._42 = 0.0f; rot._43 = 0.0f;
	_matrix prewvp = XMMatrixTranspose(g_pGameInstance->GetPreViewProtj(m_PreWroldMat));
	_matrix rotmat = XMMatrixTranspose(XMLoadFloat4x4(&rot));

	if (FAILED(m_pInstanceMesh->SetUp_ValueOnShader("g_RotationMat", &rotmat, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pInstanceMesh->SetUp_ValueOnShader("g_PreWorldViewProj", &prewvp, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	for (_uint i = 0; i < m_Nummeshcontainer; i++)
		m_pInstanceMesh->Render(i, 3);

	m_PreWroldMat = m_pTransform->Get_WorldMatrix();

	return S_OK;
}

HRESULT CEnvironment::Ready_Component()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, m_tEnvironmentDesc.wstrInstaneTag, m_tEnvironmentDesc.wstrInstaneTag, (CComponent * *)& m_pInstanceMesh, &m_tEnvironmentDesc.tInstanceDesc)))
		return E_FAIL;

	m_Nummeshcontainer = m_pInstanceMesh->Get_NumMeshContainer();

	m_LightDesc = g_pGameInstance->Get_LightDesc(0);

	return S_OK; 
}

HRESULT CEnvironment::Culling()
{
	vector<CCullingBox*> vecCullingBox = m_pInstanceMesh->Get_CullingBox();

	for (_uint i = 0; i < m_iInstanceCnt; i++)
	{
		if (!g_pGameInstance->isIn_WorldFrustum(vecCullingBox[i]->Get_Points(), 50.f))
			ZeroMemory(&m_vecUsingMatrix[i], sizeof(_float4x4));
		else
			m_vecUsingMatrix[i] = m_tEnvironmentDesc.tInstanceDesc.vecMatrix[i];
	}

	//for (_uint i = 0; i < m_iInstanceCnt; i++)
	//{
	//	_matrix matTmp = XMLoadFloat4x4(&m_tEnvironmentDesc.tInstanceDesc.vecMatrix[i]);
	//	_vector vPos = matTmp.r[3];

	//	if (!g_pGameInstance->isIn_WorldFrustum(vPos, 50.f))
	//		ZeroMemory(&m_vecUsingMatrix[i], sizeof(_float4x4));
	//	else
	//		m_vecUsingMatrix[i] = m_tEnvironmentDesc.tInstanceDesc.vecMatrix[i];
	//}

	m_pInstanceMesh->Update_InstanceBuffer(m_vecUsingMatrix);

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

CGameObject* CEnvironment::Clone(const _uint _iSceneID, void* pArg)
{
	CEnvironment* pInstance = new CEnvironment(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CEnvironment Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnvironment::Free()
{
	CLandScape::Free();

	Safe_Release(m_pInstanceMesh);
}
