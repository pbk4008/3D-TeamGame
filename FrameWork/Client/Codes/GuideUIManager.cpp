#include "pch.h"
#include "GuideUIManager.h"
#include "UI_Guide_Background.h"
#include "UI_Guide_Texture.h"
CGuideUIManager::CGuideUIManager(void)
{
}

HRESULT CGuideUIManager::NativeConstruct(void)
{
	Ready_UIObject();

	return S_OK;
}

_int CGuideUIManager::Tick(_double dTimeDelta)
{

	return _int();
}

_int CGuideUIManager::Late_Tick(_double dTimeDelta)
{
	//if (m_pBg->getActive())
	//{
	//	m_pBg->LateTick(dTimeDelta);
	//	m_pTex->LateTick(dTimeDelta);
	//}

	return _int();
}

HRESULT CGuideUIManager::Render(void)
{
	//if (m_pBg->getActive())
	//{
	//	m_pBg->Render();
	//	m_pTex->Render();
	//}

	return S_OK;
}

void CGuideUIManager::Ready_UIObject(void)
{
	m_pBg = (CUI_Guide_Background*) static_cast<CUI*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Guide_Background"));
	assert(m_pBg);
	m_pBg->setActive(false);

	 m_pTex = (CUI_Guide_Texture*) static_cast<CUI*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Guide_Texture"));
	assert(m_pTex);
	m_pTex->setActive(false);
}

void CGuideUIManager::Show()
{
	m_pBg->setActive(true);
	m_pTex->setActive(true);
}

void CGuideUIManager::Hide(void)
{
	if(m_pBg->getActive())
		m_pBg->setActive(false);
}

void CGuideUIManager::Free()
{
	Safe_Release(m_pBg);
	Safe_Release(m_pTex);
}
