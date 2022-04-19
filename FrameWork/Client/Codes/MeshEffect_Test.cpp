#include "pch.h"
#include "MeshEffect_Test.h"

#include "Material.h"

CMeshEffect_Test::CMeshEffect_Test(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMeshEffect(_pDevice, _pDeviceContext)
{
}

CMeshEffect_Test::CMeshEffect_Test(const CMeshEffect_Test& _rhs)
	: CMeshEffect(_rhs)
{
}

HRESULT CMeshEffect_Test::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_MeshEffect", L"../bin/ShaderFile/Shader_MeshEffect.hlsl", CMaterial::EType::Static);

	return S_OK;
}

HRESULT CMeshEffect_Test::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fAlpha = 1.f;
	m_fFlowSpeedAlpha = 1.f;
	return S_OK;
}

_int CMeshEffect_Test::Tick(_double _dDeltaTime)
{
	_int iProcess = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	m_fAccTime += (_float)_dDeltaTime;

	if (g_pGameInstance->getkeyDown(DIK_COLON))
	{
		if (FAILED(m_pMaterial->Change_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, L"WispTrail_02")))
			return -1;
	}
	if (g_pGameInstance->getkeyDown(DIK_SEMICOLON))
	{
		Safe_Release(m_pModel);
		m_pModel = g_pGameInstance->Clone_Component<CModel>((_uint)SCENEID::SCENE_STATIC, L"Model_MagicShield");
		if (FAILED(m_pModel->Add_Material(m_pMaterial, 0)))
			return -1;
	}

	///////////////////////////////////// Alpha
	if(0.f < m_fAlpha)
		m_fAlpha += -2.f * (_float)_dDeltaTime;
	//m_fAlpha += m_fFlowSpeedAlpha * (_float)_dDeltaTime;
	//if (0.f >= m_fAlpha)
	//	m_fFlowSpeedAlpha = 1.f;
	//else if (1.f <= m_fAlpha)
	//	m_fFlowSpeedAlpha = -1.f;
	//m_fAlpha = 1.f;

	///////////////////////////////////// UV
	// X
	m_isFlowX = true;
	m_fFlowSpeedX = 0.5f;
	m_vPlusUV.x += m_fFlowSpeedX * (_float)_dDeltaTime;
	if (1.f < m_vPlusUV.x)
		m_vPlusUV.x = 0.f;
	// Y
	m_isFlowY = true;
	m_fFlowSpeedY = 0.1f;
	m_vPlusUV.y += m_fFlowSpeedY * (_float)_dDeltaTime;
	if (1.f < m_vPlusUV.y)
		m_vPlusUV.y = 0.f;

	/////////////////////////////////// Color
	//m_isCustomColor = false;
	//m_vColor = { 0.1f, 0.8f, 0.1f };


	//////////////////////////////////////////// Scale
	//m_vScale = { 0.01f, 0.01f, 0.01f };
	m_vScale.x += 0.1f * (_float)_dDeltaTime;
	//if (0.01f < m_vScale.x)
	//	m_vScale.x = 0.001f;
	m_vScale.y = 0.01f/* * (_float)_dDeltaTime*/;
	//if (0.01f < m_vScale.y)
	//	m_vScale.y = 0.001f;
	m_vScale.z += 0.1f * (_float)_dDeltaTime;
	//if (0.01f < m_vScale.z)
	//	m_vScale.z = 0.001f;
	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });

	//////////////////////////////////////////// Rotation
	//m_pTransform->SetUp_Rotation(_float3(0.f, 0.f, 0.f));

	///////////////////////////////////////////// Position
	//m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, 1.8f, 0.f, 1.f });

	return _int();
}

_int CMeshEffect_Test::LateTick(_double _dDeltaTime)
{
	_int iProcess = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	if (m_fAccTime > m_fLifeTime)
	{
		m_fAccTime = 0.f;
	}

	if (0.05f < m_vScale.x)
	{
		m_bRemove = true;
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CMeshEffect_Test::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 1)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMeshEffect_Test::Ready_Components()
{
	if (FAILED(m_pTexture->Change_Texture(L"WispTrail_02")))
		return E_FAIL;
	if (FAILED(m_pMaterial->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, m_pTexture, 0)))
		return E_FAIL;
	Safe_AddRef(m_pTexture);

	m_pModel = g_pGameInstance->Clone_Component<CModel>((_uint)SCENEID::SCENE_STATIC, L"Model_MagicShield");
	if (FAILED(m_pModel->Add_Material(m_pMaterial, 0)))
		return E_FAIL;

	return S_OK;
}

CMeshEffect_Test* CMeshEffect_Test::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMeshEffect_Test* pInstance = new CMeshEffect_Test(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMeshEffect_Test");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeshEffect_Test::Clone(const _uint _iSceneID, void* _pArg)
{
	CMeshEffect_Test* pInstance = new CMeshEffect_Test(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CMeshEffect_Test");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshEffect_Test::Free()
{
	__super::Free();
}
