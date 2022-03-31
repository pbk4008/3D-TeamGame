#include "Effect.h"
#include "GameInstance.h"


CEffect::CEffect()
	:m_pTexture(nullptr)
{
}

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pTexture(nullptr)
{
}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs)
	, m_pTexture(rhs.m_pTexture)
{
	Safe_AddRef(m_pTexture);
}

HRESULT CEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (!m_pTexture)
		return E_FAIL;
	if (FAILED(SetUp_Components(L"Com_Texture", m_pTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(EFFECTDESC));
	}

	m_pTransform->Set_State(CTransform::STATE_POSITION, m_Desc.fMyPos);
	m_iObectTag = m_Desc.IDTag;
	return S_OK;
}

_int CEffect::Tick(_double TimeDelta)
{
	return _int();
}

_int CEffect::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CEffect::Render()
{
	return S_OK;
}

CEffect* CEffect::Copy()
{
	return nullptr;
}

_fvector CEffect::UsingGravity(_fvector vPos, _double dDeltaTime)
{
	_float fY = 0.f;
	if (m_Desc.bUsingGravity)
		fY = XMVectorGetY(vPos) + (-2 * 9.8f * (_float)dDeltaTime * (_float)dDeltaTime);
	return XMVectorSetY(vPos, fY);
}

void CEffect::Set_Reset(_bool bReset)
{
	//m_pRenderer->SetRenderButton(CRenderer::PARTICLE, true);
	m_bReset = false;
}

void CEffect::Free()
{
	Safe_Release(m_pTexture);
	CGameObject::Free();
}
