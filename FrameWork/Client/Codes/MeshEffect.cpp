#include "pch.h"
#include "MeshEffect.h"

#include "Material.h"

CMeshEffect::CMeshEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CMeshEffect::CMeshEffect(const CMeshEffect& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CMeshEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_MeshEffect", L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/Effect/T_Hexgrid.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_MeshEffect", pMtrl);

	return S_OK;
}

HRESULT CMeshEffect::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (_pArg)
	{
		m_tDesc = *static_cast<DESC*>(_pArg);
	}

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMeshEffect::Tick(_double _dDeltaTime)
{
	_int iProcess = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	return _int();
}

_int CMeshEffect::LateTick(_double _dDeltaTime)
{
	_int iProcess = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CMeshEffect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshEffect::Ready_Components()
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_tDesc.vPosition), 1.f));

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_MeshEffect", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;



	return S_OK;
}

CMeshEffect* CMeshEffect::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMeshEffect* pInstance = new CMeshEffect(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMeshEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeshEffect::Clone(const _uint _iSceneID, void* _pArg)
{
	CMeshEffect* pInstance = new CMeshEffect(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CMonster_Crawler");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshEffect::Free()
{
	__super::Free();
}
