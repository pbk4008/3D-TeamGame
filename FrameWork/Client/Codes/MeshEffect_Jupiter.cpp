#include "pch.h"
#include "MeshEffect_Jupiter.h"

#include "Material.h"

CMeshEffect_Jupiter::CMeshEffect_Jupiter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMeshEffect(_pDevice, _pDeviceContext)
{
}

CMeshEffect_Jupiter::CMeshEffect_Jupiter(const CMeshEffect_Jupiter& _rhs)
	: CMeshEffect(_rhs)
{
}

HRESULT CMeshEffect_Jupiter::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_MeshEffect", L"../bin/ShaderFile/Shader_MeshEffect.hlsl", CMaterial::EType::Static);

	return S_OK;
}

HRESULT CMeshEffect_Jupiter::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (_pArg)
	{
		DESC tDesc = *static_cast<DESC*>(_pArg);
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&tDesc.vPos), 1.f));
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fAlpha = 1.f;
	m_fFlowSpeedAlpha = 1.f;
	return S_OK;
}

_int CMeshEffect_Jupiter::Tick(_double _dDeltaTime)
{
	_int iProcess = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	m_fAccTime += (_float)_dDeltaTime;

	if (g_pGameInstance->getkeyDown(DIK_COLON))
	{
		if (FAILED(m_pMaterial->Change_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, L"TrailBase")))
			return -1;
	}

	///////////////////////////////////// Alpha
	m_fAlpha = 1.f;

	///////////////////////////////////// UV
	m_vTiling.x = 1.f;
	m_vTiling.y = 6.f;
	// X
	m_isFlowX = true;
	m_fFlowSpeedX = 0.2f;
	m_vPlusUV.x += m_fFlowSpeedX * (_float)_dDeltaTime;
	if (1.f < m_vPlusUV.x)
		m_vPlusUV.x = 0.f;
	// Y
	m_isFlowY = true;
	m_fFlowSpeedY = 0.2f;
	m_vPlusUV.y += m_fFlowSpeedY * (_float)_dDeltaTime;
	if (1.f < m_vPlusUV.y)
		m_vPlusUV.y = 0.f;

	///////////////////////////////// Color
	m_isCustomColor = true;
	m_vColor = { 0.156f, 0.76f, 1.f };


	//////////////////////////////////////////// Scale
	m_vScale = { 6.f, 6.f, 6.f };
	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });

	//////////////////////////////////////////// Rotation
	m_pTransform->SetUp_Rotation(_float3(90.f, 0.f, 0.f));

	///////////////////////////////////////////// Position
	//_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	//m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(svPos, 0.3f));

	return _int();
}

_int CMeshEffect_Jupiter::LateTick(_double _dDeltaTime)
{
	_int iProcess = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	if (m_fAccTime > m_fLifeTime)
	{
		m_fAccTime = 0.f;
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CMeshEffect_Jupiter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_float weight = 0.3f;
	m_pModel->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float));

	_float empower = 0.35f;
	m_pModel->SetUp_ValueOnShader("g_empower", &empower, sizeof(_float));

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 1)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMeshEffect_Jupiter::Ready_Components()
{
	if (FAILED(m_pTexture->Change_Texture(L"Venus_Trail")))
		return E_FAIL;

	if (FAILED(m_pMaterial->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, m_pTexture, 0)))
		return E_FAIL;
	Safe_AddRef(m_pTexture);

	m_pModel = g_pGameInstance->Clone_Component<CModel>((_uint)SCENEID::SCENE_STATIC, L"Model_Sphere2");
	if (FAILED(m_pModel->Add_Material(m_pMaterial, 0)))
		return E_FAIL;

	return S_OK;
}

CMeshEffect_Jupiter* CMeshEffect_Jupiter::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMeshEffect_Jupiter* pInstance = new CMeshEffect_Jupiter(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMeshEffect_Jupiter");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeshEffect_Jupiter::Clone(const _uint _iSceneID, void* _pArg)
{
	CMeshEffect_Jupiter* pInstance = new CMeshEffect_Jupiter(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CMeshEffect_Jupiter");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshEffect_Jupiter::Free()
{
	__super::Free();
}
