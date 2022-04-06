#include "pch.h"
#include "NoiseFire.h"

CNoiseFire::CNoiseFire(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CNoiseFire::CNoiseFire(CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CNoiseFire::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) return E_FAIL;

	return S_OK;
}

HRESULT CNoiseFire::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID,pArg))) return E_FAIL;
	if (FAILED(ReadyComponent())) return E_FAIL;
	
	_vector pos = *((_vector*)(pArg));
	pos += XMVectorSet(0, 1.8f, 0, 0);
	m_pTransform->Set_State(CTransform::STATE_POSITION, pos);

	//m_pRenderer->SetRenderButton(CRenderer::PARTICLE, true);

	return S_OK;
}

_int CNoiseFire::Tick(_double deltatime)
{
	m_deltatime += 0.01f;

	if (m_deltatime >= 1000.0f)
	{
		m_deltatime = 0.0f;
	}

	UpdateBillboard();

	return _int();
}

_int CNoiseFire::LateTick(_double deltatime)
{
	if (m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA,this);

	return _int();
}

HRESULT CNoiseFire::Render()
{
	if (FAILED(BindConstBuffer())) MSGBOX("NoiseFire Render BindConstBuffer Error");
	
	m_pbuffer->Render(0);

	return S_OK;
}

HRESULT CNoiseFire::BindConstBuffer()
{
	//m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 3.3f, 0.0f, 1.f));
	m_pTransform->Scale_Up(XMVectorSet(2.0f, 1.f, 1.f, 0.f));

	_matrix world, view, proj;

	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_float4x4))))	MSGBOX("NoiseFire ConstBuffer Worldmatrix Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_float4x4)))) MSGBOX("NoiseFire ConstBuffer Viewmatrix Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_float4x4)))) MSGBOX("NoiseFire ConstBuffer Projmatrix Not Apply");

	// Nosie Buffer
	_float3 scrollspeeds = _float3(1.5f, 2.6f, 2.8f); //_float3(1.3f, 2.1f, 2.3f);
	_float3 scales = _float3(1.0f, 2.0f, 3.0f);//_float3(1.0f, 2.0f, 3.0f);
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_frametime", &m_deltatime, sizeof(_float)))) MSGBOX("NoiseFire ConstBuffer NosieBuffer Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_scrollspeeds", &scrollspeeds, sizeof(_float3)))) MSGBOX("NoiseFire ConstBuffer NosieBuffer Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_scales", &scales, sizeof(_float3)))) MSGBOX("NoiseFire ConstBuffer NosieBuffer Not Apply");

	// Distorition buffer 
	_float2 distoriton1 = _float2(0.1f, 0.5f);
	_float2 distoriton2 = _float2(0.1f, 0.6f);
	_float2 distoriton3 = _float2(0.1f, 0.4f);
	_float distoritonScale = 0.8f;
	_float distoritonBiase = 0.2f;

	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_distortion1", &distoriton1, sizeof(_float2)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_distortion2", &distoriton2, sizeof(_float2)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_distortion3", &distoriton3, sizeof(_float2)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_distortionscale", &distoritonScale, sizeof(_float)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	if (FAILED(m_pbuffer->SetUp_ValueOnShader("g_distortionbias", &distoritonBiase, sizeof(_float)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");

	// Texture
	if (FAILED(m_pbuffer->SetUp_TextureOnShader("g_FireTexture", m_vectexture[0])))		MSGBOX("NoiseFire ConstBuffer FireTexture Not Apply");
	if (FAILED(m_pbuffer->SetUp_TextureOnShader("g_AlphaTexture", m_vectexture[1])))	MSGBOX("NoiseFire ConstBuffer NoiseTexture Not Apply");
	if (FAILED(m_pbuffer->SetUp_TextureOnShader("g_NoiseTexture", m_vectexture[2])))	MSGBOX("NoiseFire ConstBuffer AlphaTexture Not Apply");

	//  BOOL DepthEnable;
	//	D3D11_DEPTH_WRITE_MASK DepthWriteMask;
	//	D3D11_COMPARISON_FUNC DepthFunc;
	//	BOOL StencilEnable;
	//	UINT8 StencilReadMask;
	//	UINT8 StencilWriteMask;
	//	D3D11_DEPTH_STENCILOP_DESC FrontFace;
	//	D3D11_DEPTH_STENCILOP_DESC BackFace;
	return S_OK;
}

void CNoiseFire::UpdateBillboard()
{
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);
}

HRESULT CNoiseFire::ReadyComponent()
{
	CTransform::TRANSFORMDESC TransDesc;

	TransDesc.fRotationPerSec = XMConvertToRadians(90.f);
	TransDesc.fSpeedPerSec = 20.f;

	m_pTransform->Set_TransformDesc(TransDesc);

	//if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Prototype_Component_NoiseFire", L"com_buffer", (CComponent**)&m_pbuffer)))
	//	MSGBOX("Failed To NoiseFire ReadyComponent Buffer");
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Prototype_Component_NoiseFire", L"com_buffer", (CComponent**)&m_pbuffer)))
		MSGBOX("Failed To NoiseFire ReadyComponent Buffer");

	CTexture* texture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	texture->Change_Texture(L"FireTexture");
	if (!texture) MSGBOX("Failed To Get FireTexture");

	m_vectexture.emplace_back(texture);

	texture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	texture->Change_Texture(L"FireAlphaTexture");
	if (!texture) MSGBOX("Failed To Get FireAlphaTexture");

	m_vectexture.emplace_back(texture);

	texture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	texture->Change_Texture(L"FireNoiseTexture");
	if (!texture) MSGBOX("Failed To Get FireNoiseTexture");

	m_vectexture.emplace_back(texture);

	return S_OK;
}

CNoiseFire* CNoiseFire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CNoiseFire* pinst = new CNoiseFire(pDevice, pDeviceContext);

	if (FAILED(pinst->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed To Creating NoiseFire ProtoType");
		Safe_Release(pinst);
	}

	return pinst;
}

CNoiseFire* CNoiseFire::Clone(const _uint iSceneID, void* pArg)
{
	CNoiseFire* pinst = new CNoiseFire(*this);

	if (FAILED(pinst->NativeConstruct(iSceneID,pArg)))
	{
		MSGBOX("Failed To Creating NoiseFire Clone");
		Safe_Release(pinst);
	}

	return pinst;
}

void CNoiseFire::Free()
{
	__super::Free();

	for (auto& iter : m_vectexture)
		Safe_Release(iter);

	m_vectexture.clear();

	Safe_Release(m_pbuffer);
}
