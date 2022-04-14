#include "pch.h"
#include "SkyBox.h"

CSkyBox::CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CSkyBox::CSkyBox(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyBox::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyBox::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID,pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CSkyBox::Tick(_double TimeDelta)
{
	return _int();
}

_int CSkyBox::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_SKYBOX, this);
	}

	m_pTransform->Set_State(CTransform::STATE_POSITION, g_pGameInstance->Get_CamPosition(L"Camera_Silvermane"));

	return _int();
}

HRESULT CSkyBox::Render()
{
	_vector campos = g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_campos", &campos, sizeof(_vector));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pVIBufferCom->Render(0);

	return S_OK;
}

HRESULT CSkyBox::SetUp_Components()
{
	CTransform::TRANSFORMDESC TransDesc;

	TransDesc.fRotationPerSec = XMConvertToRadians(90.f);
	TransDesc.fSpeedPerSec = 20.f;

	m_pTransform->Set_TransformDesc(TransDesc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, TEXT("VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom))) return E_FAIL;

	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if(m_iSceneID == (_uint)SCENEID::SCENE_STAGE1)
		m_pTexture->Change_Texture(L"Sky_Texture1");
	else if(m_iSceneID == (_uint)SCENEID::SCENE_STAGE2)
		m_pTexture->Change_Texture(L"Sky_Texture2");

	if (m_pTexture == nullptr)
		MSGBOX("Failed To Creating SkyTexture");

	return S_OK;
}

CSkyBox* CSkyBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CSkyBox* pInstance = new CSkyBox(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating SkyBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkyBox::Clone(const _uint _iSceneID, void* pArg)
{
	CSkyBox* pInstance = new CSkyBox(*this);

	if (FAILED(pInstance->NativeConstruct(_iSceneID,pArg)))
	{
		MSGBOX("Failed to Creating SkyBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBufferCom);
}
