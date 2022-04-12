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
	if (nullptr != m_pRenderer && m_bOnOff)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CSwordTrail::LateTick(_double TimeDelta)
{

	return _int();
}

HRESULT CSwordTrail::Render()
{
	//_matrix world, view, proj;
	//world = XMMatrixTranspose(XMMatrixIdentity());
	//view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	//proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	//m_pViBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	//m_pViBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	//m_pViBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));
	//m_pViBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTextureCom);

	//m_Frametime += 0.01f;

	//if (m_Frametime >= 1000.0f)
	//{
	//	m_Frametime = 0.0f;
	//}

	_matrix world, view, proj;

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_float4x4))))	MSGBOX("NoiseFire ConstBuffer Worldmatrix Not Apply");
	if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_float4x4)))) MSGBOX("NoiseFire ConstBuffer Viewmatrix Not Apply");
	if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_float4x4)))) MSGBOX("NoiseFire ConstBuffer Projmatrix Not Apply");

	//_float3 scrollspeeds = _float3(1.5f, 2.6f, 2.8f); //_float3(1.3f, 2.1f, 2.3f);
	//_float3 scales = _float3(1.0f, 2.0f, 3.0f);//_float3(1.0f, 2.0f, 3.0f);
	//if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_frametime", &m_Frametime, sizeof(_float)))) MSGBOX("NoiseFire ConstBuffer NosieBuffer Not Apply");
	//if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_scrollspeeds", &scrollspeeds, sizeof(_float3)))) MSGBOX("NoiseFire ConstBuffer NosieBuffer Not Apply");
	//if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_scales", &scales, sizeof(_float3)))) MSGBOX("NoiseFire ConstBuffer NosieBuffer Not Apply");

	//_float2 distoriton1 = _float2(0.1f, 0.5f);
	//_float2 distoriton2 = _float2(0.1f, 0.6f);
	//_float2 distoriton3 = _float2(0.1f, 0.4f);
	//_float distoritonScale = 0.8f;
	//_float distoritonBiase = 0.2f;

	//if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_distortion1", &distoriton1, sizeof(_float2)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	//if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_distortion2", &distoriton2, sizeof(_float2)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	//if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_distortion3", &distoriton3, sizeof(_float2)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	//if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_distortionscale", &distoritonScale, sizeof(_float)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	//if (FAILED(m_pViBufferCom->SetUp_ValueOnShader("g_distortionbias", &distoritonBiase, sizeof(_float)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");

	// Texture
	if (FAILED(m_pViBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture)))		MSGBOX("NoiseFire ConstBuffer DiffuseTexture Not Apply");
	if (FAILED(m_pViBufferCom->SetUp_TextureOnShader("g_DistortionTex", m_pTexture1)))	MSGBOX("NoiseFire ConstBuffer DistortionTex Not Apply");
	if (FAILED(m_pViBufferCom->SetUp_TextureOnShader("g_DistorionMaskTex", m_pTexture2)))	MSGBOX("NoiseFire ConstBuffer DistortionMaskTex Not Apply");

	m_pViBufferCom->RenderBuffer(&m_TrailList, 1);

	return S_OK;
}

void CSwordTrail::AddVertex(const _fvector pToppos, const _vector pBottompos)
{
	m_bOnOff = true;

	m_TrailList.emplace_back(pToppos, pBottompos);

	if (m_TrailList.size() > 15)
		m_TrailList.pop_front();

	std::cout << "Trail Size : " << m_TrailList.size() << endl;
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

	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pTexture->Change_Texture(L"TrailBase");

	m_pTexture1 = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");;
	m_pTexture1->Change_Texture(L"DistortionBase");

	m_pTexture2 = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");;
	m_pTexture2->Change_Texture(L"DistortionMask");

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

	Safe_Release(m_pTexture);
	Safe_Release(m_pTexture1);
	Safe_Release(m_pTexture2);
	Safe_Release(m_pViBufferCom);
}
