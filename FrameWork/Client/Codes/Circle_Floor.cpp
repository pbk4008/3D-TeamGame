#include "pch.h"
#include "Circle_Floor.h"

CCircle_Floor::CCircle_Floor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLandScape(pDevice, pDeviceContext)
	,m_pModel(nullptr)
{
}

CCircle_Floor::CCircle_Floor(const CCircle_Floor& rhs)
	: CLandScape(rhs)
	, m_pModel(rhs.m_pModel)
{
	Safe_AddRef(m_pModel);
}

HRESULT CCircle_Floor::NativeConstruct_Prototype()
{
	if (FAILED(CLandScape::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCircle_Floor::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(CLandScape::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	if (pArg)
	{
		_float4x4 fTransform = (*(_float4x4*)pArg);
		_matrix matTransform = XMLoadFloat4x4(&fTransform);

		m_pTransform->Set_WorldMatrix(matTransform);
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CCircle_Floor::Tick(_double TimeDelta)
{
	return _int();
}

_int CCircle_Floor::LateTick(_double TimeDelta)
{

	if (m_pRenderer)
	{
		if (m_pRenderer->Get_RenderButton(CRenderer::SHADOW) == true)
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);

		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	}

	return _int();
}

HRESULT CCircle_Floor::Render()
{
	_float4 ClipPlane = _float4(0.f, 0.f, 0.f, 0.f);
	_matrix matWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix matView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix matProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector campos = g_pGameInstance->Get_CamPosition(wstrCamTag);

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_CamPos", &campos, sizeof(_vector));
	m_pModel->SetUp_ValueOnShader("ClipPlane", &ClipPlane, sizeof(_float4));

	for (_uint i = 0; i < m_Nummeshcontainer; i++)
		m_pModel->Render(i, 0);


	return S_OK;
}

HRESULT CCircle_Floor::Render_Shadow()
{
	_matrix world, lightview, ligtproj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	lightview = XMMatrixTranspose(m_LightDesc->mLightView);
	ligtproj = XMMatrixTranspose(m_LightDesc->mLightProj);
	_float3 lightpos = m_LightDesc->vPosition;

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_LightView", &lightview, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_LightProj", &ligtproj, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_LightPos", &lightpos, sizeof(_float3));

	for (_uint i = 0; i < m_Nummeshcontainer; i++)
		m_pModel->Render(i, 2);

	return S_OK;
}

HRESULT CCircle_Floor::Ready_Component()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"PavementDecor_01_Lod0.fbx", L"Model",(CComponent**)&m_pModel)))
		return E_FAIL;

	m_Nummeshcontainer = m_pModel->Get_NumMeshContainer();

	m_LightDesc = g_pGameInstance->Get_LightDesc(0);

	return S_OK;
}

CCircle_Floor* CCircle_Floor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CCircle_Floor* pInstance = new CCircle_Floor(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCircle_Floor Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CCircle_Floor* CCircle_Floor::Clone(const _uint _iSceneID, void* pArg)
{
	CCircle_Floor* pInstance = new CCircle_Floor(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CCircle_Floor Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCircle_Floor::Free()
{
	CLandScape::Free();
	Safe_Release(m_pModel);
}
