#include "pch.h"
#include "SwordTrail.h"
#include "GameInstance.h"

CSwordTrail::CSwordTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice,pDeviceContext)
{
}

CSwordTrail::CSwordTrail(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSwordTrail::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSwordTrail::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID,pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CSwordTrail::Tick(_double TimeDelta)
{
	return _int();
}

_int CSwordTrail::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer && m_bOnOff)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CSwordTrail::Render()
{
	if (m_pTextureCom == nullptr)
		return E_FAIL;

	_matrix world, view, proj;
	world = XMMatrixTranspose(XMMatrixIdentity());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pViBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pViBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pViBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));
	m_pViBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTextureCom);

	m_pViBufferCom->RenderBuffer(&m_TrailList,2);

	return S_OK;
}

void CSwordTrail::AddVertex(const _fvector pToppos, const _vector pBottompos)
{
	m_bOnOff = true;

	m_TrailList.emplace_back(pToppos, pBottompos);

	if (m_TrailList.size() > 10)
		m_TrailList.pop_front();
}

void CSwordTrail::Clear_Vertex()
{
	m_bOnOff = false;

	m_TrailList.clear();

	m_pViBufferCom->Clear_Vertex();
}

HRESULT CSwordTrail::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"), TEXT("Com_VIBuffer"), (CComponent**)&m_pViBufferCom)))
		return E_FAIL;

	m_pTextureCom = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pTextureCom->Change_Texture(L"TrailBase");
	
	return S_OK;
}

CSwordTrail* CSwordTrail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CSwordTrail* pInstance = new CSwordTrail(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed To Creating Prototype CSwordTrail");
	}
	return pInstance;
}

CGameObject* CSwordTrail::Clone(const _uint _iSceneID, void* pArg)
{
	CSwordTrail* pInstance = new CSwordTrail(*this);

	if (FAILED(pInstance->NativeConstruct(_iSceneID,pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed To Creating Clone CSwordTrail");
	}
	return pInstance;
}

void CSwordTrail::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pViBufferCom);
}
