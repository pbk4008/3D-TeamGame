#include "pch.h"
#include "Wall.h"
#include "GameInstance.h"

CWall::CWall(ID3D11Device* pdevice, ID3D11DeviceContext* pdevicecontext)
	: CGameObject(pdevice,pdevicecontext)
{
	ZeroMemory(&m_desc, sizeof(WALLDESC));
}

CWall::CWall(const CWall& rhs)
	: CGameObject(rhs)
{
	ZeroMemory(&m_desc, sizeof(WALLDESC));
}

HRESULT CWall::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	MSGBOX("Failed to Wall Prototype");

	return S_OK;
}

HRESULT CWall::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID,_pArg)))	MSGBOX("Failed to Wall Clone Native");

	if (_pArg != nullptr)
		memcpy(&m_desc, _pArg, sizeof(WALLDESC));

	if(FAILED(Ready_Component())) MSGBOX("Failed to Wall ReadyComponent");

	

	return S_OK;
}

_int CWall::Tick(_double _dDeltaTime)
{

	m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CWall::LateTick(_double _dDeltaTime)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHANB, this);
		//m_pRenderer->Add_RenderGroup(CRenderer::RENDER_DYDISTORTION, this);
		//m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	}

	return _int();
}

HRESULT CWall::Render()
{
	if (m_pRenderer == nullptr)
		return E_FAIL;

	m_delta += 0.1f;

	if (m_delta >= 1000.0f)
	{
		m_delta = 0.0f;
	}

	if (FAILED(DissolveOn(0.35f))) MSGBOX("Failed to Dissovle Shader");

	_matrix world, view, proj;

	wstring wstrCam = g_pGameInstance->Get_BaseCameraTag();
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCam, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCam, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_bdissolve", &m_bdissolve, sizeof(_bool)))) MSGBOX("Failed to Apply dissolvetime");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_float4x4))))	MSGBOX("Wall ConstBuffer Worldmatrix Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_float4x4))))	MSGBOX("Wall ConstBuffer Viewmatrix Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_float4x4))))	MSGBOX("Wall ConstBuffer Projmatrix Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_deltatime", &m_delta, sizeof(_float)))) MSGBOX("Wall ConstBuffer delta Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_color", &m_desc.color, sizeof(_float4)))) MSGBOX("Wall ConstBuffer delta Not Apply");
	if (FAILED(m_pbuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pdiffusetex)))	MSGBOX("Wall ConstBuffer DiffuseTex Not Apply");

	if (FAILED(m_pbuffer->Render(1))) MSGBOX("Failed To Wall Rendering");

	return S_OK;
}

void CWall::set_DissolveRest()
{
	m_bdissolve = false;
	m_lifetime = 0.f;
}

void CWall::Destroy()
{
	m_bdissolve = true;
	m_pCollider->Remove_ActorFromScene();
}

HRESULT CWall::Ready_Component()
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_desc.pos));
	m_pTransform->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_desc.radian));
	m_pTransform->Scaling(XMVectorSet(m_desc.scale.x, m_desc.scale.y, 1.f, 0.f));

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_Wall", L"wall_buffer", (CComponent**)&m_pbuffer)))
		MSGBOX("Failed to Create wall buffer");


	CCollider::DESC tColliderDesc;
	tColliderDesc.eRigidType = ERigidType::Static;
	tColliderDesc.isSceneQuery = true;
	tColliderDesc.isTrigger = false;
	tColliderDesc.pGameObject = this;
	tColliderDesc.isVisualization= true;
	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { m_desc.scale.x, m_desc.scale.y, 1.f };

	if(FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC,L"Proto_Component_BoxCollider",L"Collider", (CComponent**)&m_pCollider,&tBoxColliderDesc)))
		return E_FAIL;
	//_matrix matPivot = XMMatrixTranslation(0.f, m_desc.scale.y, 0.f);
	//m_pCollider->setPivotMatrix(matPivot);

	m_pdiffusetex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (FAILED(m_pdiffusetex->Change_Texture(L"WallBase"))) MSGBOX("Failed to Change Texture DiffuseTex");

	m_dissolveTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (FAILED(m_dissolveTex->Change_Texture(L"DissovleBase"))) MSGBOX("Failed to Change Texture DissovleTex");



	return S_OK;
}

HRESULT CWall::DissolveOn(_float dissolveSpeed)
{
	if (m_bdissolve == true)
	{
		m_lifetime += (g_fDeltaTime * dissolveSpeed);
		if (m_lifetime >= 1.f)
		{
			m_lifetime = 1.f;
		}
		if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_dissolvetime", &m_lifetime, sizeof(_float)))) MSGBOX("Failed to Apply dissolvetime");
		if (FAILED(m_pbuffer->SetUp_TextureOnShader("g_DissolveTex", m_dissolveTex, 0))) MSGBOX("Failed to Apply dissolveTex");
	}

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
	__super::Free();

	Safe_Release(m_pbuffer);
	Safe_Release(m_pdiffusetex);
	Safe_Release(m_dissolveTex);
	Safe_Release(m_pCollider);
}
