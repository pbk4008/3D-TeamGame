#include "pch.h"
#include "Wall.h"
#include "GameInstance.h"

CWall::CWall(ID3D11Device* pdevice, ID3D11DeviceContext* pdevicecontext)
	: CGameObject(pdevice,pdevicecontext)
{
}

CWall::CWall(const CWall& rhs)
	: CGameObject(rhs)
{
}

HRESULT CWall::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	MSGBOX("Failed to Wall Prototype");

	return S_OK;
}

HRESULT CWall::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID,_pArg)))	MSGBOX("Failed to Wall Clone Native");

	if(FAILED(Ready_Component())) MSGBOX("Failed to Wall ReadyComponent");

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.5738f, 1.f, 10.0221f, 1.f));
	m_pTransform->Scale_Up(XMVectorSet(30.f, 30.f, 1.f, 0.f));

	return S_OK;
}

_int CWall::Tick(_double _dDeltaTime)
{
	return _int();
}

_int CWall::LateTick(_double _dDeltaTime)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_DYDISTORTION, this);
		//m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	}

	return _int();
}

HRESULT CWall::Render()
{
	if (m_pRenderer == nullptr)
		return E_FAIL;

	m_delta += 0.01f;

	if (m_delta >= 1000.0f)
	{
		m_delta = 0.0f;
	}

	m_pRenderer->SetRenderButton(CRenderer::DISTORTION, true);
	_matrix world, view, proj;

	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_float4x4))))	MSGBOX("Wall ConstBuffer Worldmatrix Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_float4x4))))	MSGBOX("Wall ConstBuffer Viewmatrix Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_float4x4))))	MSGBOX("Wall ConstBuffer Projmatrix Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_deltatime", &m_delta, sizeof(_float)))) MSGBOX("Wall ConstBuffer delta Not Apply");
	if (FAILED(m_pbuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pdiffusetex)))	MSGBOX("Wall ConstBuffer DiffuseTex Not Apply");

	if (FAILED(m_pbuffer->Render(1))) MSGBOX("Failed To Wall Rendering");

	return S_OK;
}

HRESULT CWall::Ready_Component()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_Wall", L"wall_buffer", (CComponent**)&m_pbuffer)))
		MSGBOX("Failed to Create wall buffer");

	m_pdiffusetex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pdiffusetex->Change_Texture(L"WallBase");

	return S_OK;
}

CWall* CWall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContex)
{
	CWall* pInsance = new CWall(pDevice, pDeviceContex);
	if (FAILED(pInsance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Prototype Wall");
		Safe_Release(pInsance);
	}
	return pInsance;
}

CGameObject* CWall::Clone(_uint iSeneid, void* pArg)
{
	CWall* pInsance = new CWall(*this);
	if (FAILED(pInsance->NativeConstruct(iSeneid, pArg)))
	{
		MSGBOX("Failed to Creating Clone Wall");
		Safe_Release(pInsance);
	}
	return pInsance;
}

void CWall::Free()
{
	Safe_Release(m_pbuffer);
	Safe_Release(m_pdiffusetex);

	__super::Free();
}
