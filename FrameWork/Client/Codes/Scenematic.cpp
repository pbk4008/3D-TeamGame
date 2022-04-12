#include "pch.h"
#include "..\Headers\Scenematic.h"

CScenematic::CScenematic()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_bActive(true)
	, m_bCinemaEnd(false)
{
}

CScenematic::CScenematic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_bActive(true)
	, m_bCinemaEnd(false)
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pDeviceContext);
}

HRESULT CScenematic::NativeContruct()
{
	return S_OK;
}

_int CScenematic::Tick(_double dDeltaTime)
{
	if (!m_bActive)
		return 1;
	if (m_bCinemaEnd)
	{
		Set_Active(false);
		End_Cinema();
		return 1;
	}

	return _int();
}

_int CScenematic::LateTick(_double dDeltaTime)
{
	if (!m_bActive)
		return 1;

	return _int();
}

void CScenematic::Set_Active(_bool bCheck)
{
	m_bActive = bCheck;
	for (auto& pCom : m_vecScenemaComponents)
		pCom->setActive(bCheck);
}

void CScenematic::End_Cinema()
{
	if (m_bCinemaEnd)
		g_pGameInstance->Change_BaseCamera(L"Camera_Silvermane");
}

HRESULT CScenematic::Set_UpComponents(CComponent* pComponent)
{
	if (!pComponent)
		return E_FAIL;
	m_vecScenemaComponents.emplace_back(pComponent);
	Safe_AddRef(pComponent);

	return S_OK;
}

void CScenematic::Free()
{
	for (auto& pCom : m_vecScenemaComponents)
		Safe_AddRef(pCom);

	m_vecScenemaComponents.clear();
}
