#include "..\public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"
#include "GameInstance.h"
#include "Transform.h"

CGameObject::CGameObject()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pTransform(nullptr)
	, m_pRenderer(nullptr)
	, m_bActive(false)
	, m_bCheckCollider(false)
{
}

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_pRenderer(nullptr)
	, m_pTransform(nullptr)
	, m_bActive(false)
	, m_bCheckCollider(false)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);	
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDeviceContext(rhs.m_pDeviceContext)
	, m_pRenderer(rhs.m_pRenderer)
	, m_Components(rhs.m_Components)
	, m_pTransform(nullptr)
	, m_bActive(rhs.m_bActive)
	, m_bCheckCollider(false)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);

	for (auto& pCom : m_Components)
		Safe_AddRef(pCom.second);

	Safe_AddRef(m_pRenderer);
}

CComponent * CGameObject::Get_Component(const wstring& pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	m_pRenderer = pInstance->Clone_Component<CRenderer>(0, L"Renderer");

	RELEASE_INSTANCE(CGameInstance);

	if (!m_pRenderer)
		return E_FAIL;

	if (FAILED(SetUp_Components(L"Renderer", m_pRenderer)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void * pArg)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	m_pTransform = pInstance->Clone_Component<CTransform>(0, L"Transform");

	RELEASE_INSTANCE(CGameInstance);

	if (!m_pTransform)
		return E_FAIL;

	if (FAILED(SetUp_Components(L"Transform", m_pTransform)))
		return E_FAIL;

	return S_OK;
}

_int CGameObject::Tick(_double TimeDelta)
{
	return _int();
}

_int CGameObject::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::OnCollisionEnter(CGameObject* pCol)
{
}

void CGameObject::OnCollisionStay(CGameObject* pCol)
{
}

void CGameObject::OnCollisionExit(CGameObject* pCol)
{
}

void CGameObject::OnTriggerEnter(CGameObject* pCol)
{
}

void CGameObject::OnTriggerExit(CGameObject* pCol)
{
}

void CGameObject::setActive(_bool bActive)
{
	m_bActive = bActive;
	for (auto& pCom : m_Components)
		pCom.second->setActive(m_bActive);
}

HRESULT CGameObject::SetUp_Components(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring&pComponentTag, CComponent** ppOut, void* pArg)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));

	if (iter != m_Components.end())
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent*		pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	/* 맵에도 보관. + 자식의 변수에도 저장. */
	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CGameObject::SetUp_Components(const wstring& pComponentTag, CComponent* pClone)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));

	if (iter != m_Components.end())
		return E_FAIL;
	
	m_Components.emplace(pComponentTag, pClone);
	Safe_AddRef(pClone);

	return S_OK;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
}
