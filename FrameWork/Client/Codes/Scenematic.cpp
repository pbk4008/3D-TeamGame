#include "pch.h"
#include "..\Headers\Scenematic.h"
#include "CinemaActor.h"
#include "CinemaWeapon.h"

CScenematic::CScenematic()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_bActive(true)
	, m_bCinemaEnd(false)
	, m_iSceneID(0)
{
}

CScenematic::CScenematic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_bActive(true)
	, m_bCinemaEnd(false)
	, m_iSceneID(0)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CScenematic::NativeContruct(_uint iSceneID)
{
	m_iSceneID = iSceneID;

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
		m_bCinemaEnd = false;
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

	OnOffPlayerWithUI(!bCheck);
}

void CScenematic::End_Cinema()
{
	if (m_bCinemaEnd)
	{
		OnOffPlayerWithUI(true);
		g_pGameInstance->Change_BaseCamera(L"Camera_Silvermane");
		g_pMainApp->Change_RenderCamTag(L"Camera_Silvermane");
		g_pMainApp->Set_RenderBtn(CRenderer::RENDERBUTTON::FADEOUT, true);
	}
}

HRESULT CScenematic::Set_UpComponents(CComponent* pComponent)
{
	if (!pComponent)
		return E_FAIL;
	m_vecScenemaComponents.emplace_back(pComponent);

	return S_OK;
}

HRESULT CScenematic::Ready_Actor(CCinemaActor** pOut, _uint iActorTag)
{
	*pOut = g_pGameInstance->Clone_GameObject<CCinemaActor>(m_iSceneID, L"Proto_GameObject_CinemaActor", &iActorTag);
	if (*pOut == nullptr)
	{
		MSGBOX("Ready Actor Fail");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CScenematic::Ready_Weapon(CCinemaWeapon** pOut, _uint iWeaponTag)
{
	*pOut = g_pGameInstance->Clone_GameObject<CCinemaWeapon>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_CinemaWeapon", &iWeaponTag);

	if (*pOut == nullptr)
	{
		MSGBOX("Ready Weapon Fail");
		return E_FAIL;
	}
	return S_OK;
}

void CScenematic::OnOffPlayerWithUI(_bool bCheck)
{
	g_pObserver->Player_Active(bCheck);
	list<CGameObject*>* pUIList = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_Green");
	for (auto& pUI : *pUIList)
		pUI->setActive(bCheck);
	pUIList = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI");
	for (auto& pUI : *pUIList)
		pUI->setActive(bCheck);

	g_pQuestManager->SetRender(bCheck);
	g_pInvenUIManager->SetRender(bCheck);

	pUIList = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_BlankC");
	for (auto& pUI : *pUIList)
		pUI->setActive(bCheck);

	pUIList = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_FillC");
	for (auto& pUI : *pUIList)
		pUI->setActive(bCheck);
}

void CScenematic::Free()
{
	//for (auto& pCom : m_vecScenemaComponents)
	//	Safe_Release(pCom);

	m_vecScenemaComponents.clear();

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
