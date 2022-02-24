#include "Actor.h"
#include "..\public\Actor.h"

CActor::CActor()
	:m_fSpeed(0.f)
	, m_bDead(false)
{
}

CActor::CActor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
	, m_fSpeed(0.f)
	, m_bDead(false)
{
}

CActor::CActor(const CActor& rhs)
	: CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_bDead(rhs.m_bDead)
{
}

HRESULT CActor::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CActor::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CActor::Tick(_double TimeDelta)
{
	return _int();
}

_int CActor::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CActor::Render()
{
	return S_OK;
}

void CActor::Free()
{
	CGameObject::Free();
}
