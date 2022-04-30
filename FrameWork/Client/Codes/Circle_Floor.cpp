#include "pch.h"
#include "Circle_Floor.h"

CCircle_Floor::CCircle_Floor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLandScape(pDevice, pDeviceContext)
	,m_pModel(nullptr)
	,m_pCrackModel(nullptr)
{
}

CCircle_Floor::CCircle_Floor(const CCircle_Floor& rhs)
	: CLandScape(rhs)
	, m_pModel(rhs.m_pModel)
	, m_pCrackModel(rhs.m_pCrackModel)
{
	Safe_AddRef(m_pModel);
	Safe_AddRef(m_pCrackModel);
}

HRESULT CCircle_Floor::NativeConstruct_Prototype()
{
	if (FAILED(CLandScape::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCircle_Floor::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(CLandScape::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCircle_Floor::Tick(_double TimeDelta)
{
	return _int();
}

_int CCircle_Floor::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CCircle_Floor::Render()
{
	return E_NOTIMPL;
}

HRESULT CCircle_Floor::Render_Shadow()
{
	return E_NOTIMPL;
}

HRESULT CCircle_Floor::Ready_Component()
{
	return E_NOTIMPL;
}

HRESULT CCircle_Floor::Culling()
{
	return E_NOTIMPL;
}

CCircle_Floor* CCircle_Floor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	return nullptr;
}

CCircle_Floor* CCircle_Floor::Clone(const _uint _iSceneID, void* pArg)
{
	return nullptr;
}

void CCircle_Floor::Free()
{
}
