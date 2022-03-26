#include "Actor.h"
#include "..\public\Actor.h"
#include "Transform.h"

CActor::CActor()
	:m_fSpeed(0.f)
	, m_bDead(false)
	, m_IsAttack(false)
	, m_fMaxHp(0.f)
	, m_fCurrentHp(0.f)
	, m_fGroggyGauge(0.f)
	, m_fMaxGroggyGauge(0.f)
	, m_bGroggy(false)
{
}

CActor::CActor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
	, m_fSpeed(0.f)
	, m_bDead(false)
	, m_IsAttack(false)
	, m_fMaxHp(0.f)
	, m_fCurrentHp(0.f)
	, m_fGroggyGauge(0.f)
	, m_fMaxGroggyGauge(0.f)
	, m_bGroggy(false)
{
}

CActor::CActor(const CActor& rhs)
	: CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_bDead(rhs.m_bDead)
	, m_IsAttack(rhs.m_IsAttack)
	, m_fMaxHp(rhs.m_fMaxHp)
	, m_fCurrentHp(rhs.m_fCurrentHp)
	, m_fGroggyGauge(rhs.m_fGroggyGauge)
	, m_fMaxGroggyGauge(rhs.m_fMaxGroggyGauge)
	, m_bGroggy(rhs.m_bGroggy)
{
}

HRESULT CActor::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CActor::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CActor::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CActor::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CActor::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

const _float CActor::Get_HpRatio()
{
	_float fRatio = m_fCurrentHp / m_fMaxHp;

	return fRatio;
}

const _float CActor::Get_GroggyGaugeRatio()
{
	return m_fGroggyGauge / m_fMaxGroggyGauge;
}

HRESULT CActor::Set_SpawnPosition(const _float3 vPoint)
{
	if (!m_pTransform)
		return E_FAIL;

	_vector vPos = XMLoadFloat3(&vPoint);
	vPos=XMVectorSetW(vPos, 1.f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}


void CActor::Free()
{
	CGameObject::Free();
}
