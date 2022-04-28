#include "pch.h"
#include "MeshEffect_Boss_Shield.h"

#include "Material.h"

CMeshEffect_Boss_Shield::CMeshEffect_Boss_Shield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMeshEffect(_pDevice, _pDeviceContext)
{
}

CMeshEffect_Boss_Shield::CMeshEffect_Boss_Shield(const CMeshEffect_Boss_Shield& _rhs)
	: CMeshEffect(_rhs)
{
}

HRESULT CMeshEffect_Boss_Shield::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_MeshEffect", L"../bin/ShaderFile/Shader_MeshEffect.hlsl", CMaterial::EType::Static);

	return S_OK;
}

HRESULT CMeshEffect_Boss_Shield::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fAlpha = 2.f;
	m_fFlowSpeedAlpha = 1.f;

	m_vScale = { 42.f, 42.f, 42.f };
	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });

	//////////////////////////////////////////// Rotation
	m_pTransform->SetUp_Rotation(_float3(90.f, 0.f, 0.f));

	///////////////////////////////////////////// Position
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, 0.f, 0.f, 1.f });

	m_fAlpha = 1.f;
	return S_OK;
}

_int CMeshEffect_Boss_Shield::Tick(_double _dDeltaTime)
{
	_int iProcess = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	m_fAccTime += (_float)_dDeltaTime;


	m_isReverse = false;
	m_vTiling.x = 15.f;
	m_vTiling.y = 15.f;
	
	m_isCustomColor = true;	
	m_vColor = { 0.1f, 0.3f, 1.f };
	
	/*if (false == m_bBeSmall)
	{
		m_vScale.x += 250.f * (_float)_dDeltaTime;
		m_vScale.y += 250.f * (_float)_dDeltaTime;
		m_vScale.z += 250.f * (_float)_dDeltaTime;

		m_fAlpha = 3.0f;
	}

	if (90.f < m_vScale.x)
	{
		m_bBeSmall = true;
	}

	if (m_bBeSmall)
	{
		m_fAlpha = 1.0f;

		m_vScale.x -= 250.f * (_float)_dDeltaTime;
		m_vScale.y -= 250.f * (_float)_dDeltaTime;
		m_vScale.z -= 250.f * (_float)_dDeltaTime;
	}*/

	CTransform* pBossTransform = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Boss")->front()->Get_Transform();

	_vector svPos = pBossTransform->Get_State(CTransform::STATE_POSITION);
	_vector svRight = XMVector3Normalize(pBossTransform->Get_State(CTransform::STATE_RIGHT));
	_vector svLook = XMVector3Normalize(pBossTransform->Get_State(CTransform::STATE_LOOK));

	if (pBossTransform)
	{
		_vector BossPos = pBossTransform->Get_State(CTransform::STATE_POSITION);
		_vector NewPos = { XMVectorGetX(BossPos), XMVectorGetY(BossPos) + 2.f, XMVectorGetZ(BossPos), 1.f };
		m_pTransform->Set_State(CTransform::STATE_POSITION, NewPos + svLook * 0.f);
	}

	return _int();
}

_int CMeshEffect_Boss_Shield::LateTick(_double _dDeltaTime)
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

HRESULT CMeshEffect_Boss_Shield::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_float Weight = 1.f;
	m_pModel->SetUp_ValueOnShader("g_Weight", &Weight, sizeof(_float));

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 6)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMeshEffect_Boss_Shield::Ready_Components()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Texture", L"MaskTexture", (CComponent**)&m_pMaskTexture)))
		return E_FAIL;

	m_pMaskTexture->Change_Texture(L"T_ShieldPattern2");
	if (FAILED(m_pMaterial->Set_Texture("g_MaskTexture", TEXTURETYPE::TEX_MASK, m_pMaskTexture, 0)))
		return E_FAIL;

	if (FAILED(m_pTexture->Change_Texture(L"T_ShieldPattern2")))
		return E_FAIL;
	if (FAILED(m_pMaterial->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, m_pTexture, 0)))
		return E_FAIL;
	Safe_AddRef(m_pTexture);


	m_pModel = g_pGameInstance->Clone_Component<CModel>((_uint)SCENEID::SCENE_STATIC, L"Model_Sphere2");
	if (FAILED(m_pModel->Add_Material(m_pMaterial, 0)))
		return E_FAIL;

	return S_OK;
}

CMeshEffect_Boss_Shield* CMeshEffect_Boss_Shield::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMeshEffect_Boss_Shield* pInstance = new CMeshEffect_Boss_Shield(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMeshEffect_Boss_Shield");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeshEffect_Boss_Shield::Clone(const _uint _iSceneID, void* _pArg)
{
	CMeshEffect_Boss_Shield* pInstance = new CMeshEffect_Boss_Shield(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CMeshEffect_Boss_Shield");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshEffect_Boss_Shield::Free()
{
	__super::Free();
	Safe_Release(m_pMaskTexture);
}
