#include "Effect.h"

CEffect::CEffect()
	:m_fMaxLifTime(0.f)
	,m_bUsingGravity(false)
	, m_iNumEffectCount(0)
{
}

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_fMaxLifTime(0.f)
	, m_bUsingGravity(false)
	, m_iNumEffectCount(0)
{
}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs)
	, m_fMaxLifTime(rhs.m_fMaxLifTime)
	, m_bUsingGravity(rhs.m_bUsingGravity)
	, m_iNumEffectCount(rhs.m_iNumEffectCount)
{
}

HRESULT CEffect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CEffect::NativeConstruct(void* pArg)
{
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

_fvector CEffect::UsingGravity(_fvector vPos, _double dDeltaTime)
{
	_float fY = 0.f;
	if (m_bUsingGravity)
		fY = XMVectorGetY(vPos) + (-2 * 9.8f * (_float)dDeltaTime * (_float)dDeltaTime);
	return XMVectorSetY(vPos, fY);
}

void CEffect::Free()
{
	CGameObject::Free();
}
