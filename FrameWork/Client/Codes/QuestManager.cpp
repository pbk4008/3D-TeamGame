#include "pch.h"
#include "QuestManager.h"
#include "Quest.h"
#include "UI_QuestHeadText.h"

CQuestManager::CQuestManager(void)
{
}

HRESULT CQuestManager::NativeConstruct(void)
{
	Ready_Quest();

	CUI_QuestHeadText::Desc desc;
	desc.EHeadText = EQuestHeaderType::FirestStep;

	m_pQuesthead = (CUI_QuestHeadText*) static_cast<CUI*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_QuestHeadText", &desc));
	assert(m_pQuesthead);

	return S_OK;
}

_int CQuestManager::Tick(_double dTimeDelta)
{
	PullingQuest();

	for (_int i = 0; i < m_vecQuest.size(); ++i)
	{
		m_vecQuest[i]->Tick(dTimeDelta);
	}

	return _int();
}

_int CQuestManager::Late_Tick(_double dTimeDelta)
{
	
	for (_int i = 0; i < m_vecQuest.size(); ++i)
	{
		m_vecQuest[i]->LateTick(dTimeDelta);
	}
	m_pQuesthead->LateTick(dTimeDelta);

	return _int();
}

HRESULT CQuestManager::Render(void)
{
	if (!g_pInvenUIManager->IsOpenModal())
	{

		for (_int i = 0; i < m_vecQuest.size(); ++i)
		{
			m_vecQuest[i]->Render();
		}
		m_pQuesthead->Render();
	}
	return S_OK;
}

void CQuestManager::Ready_Quest(void)
{
	CQuest::Desc desc;
	_int j = 0;

	for (_int i = 0; i < (_int)EQuestTextType::QuestText_END; ++i)
	{
		desc.EQuestText = (EQuestTextType)j;
		j++;

		CQuest* pQuest = static_cast<CQuest*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Quest", &desc));
		assert(pQuest);

		m_vecQuestList.push_back(std::make_pair(g_arrQuestTextTex[(_int)desc.EQuestText], pQuest));
	}
}

void CQuestManager::PullingQuest(void)
{
	if (0 == m_vecQuest.size())
		return;
	else
	{
		for (auto iter = m_vecQuest.begin(); iter != m_vecQuest.end();)
		{
			if ((*iter)->IsAlive() == false)
			{
				/* 현재 iter 뒤의 모든 Quest 객체들의 위치값을 올리는 옵션 On */
				iter = m_vecQuest.erase(iter);
				for (; iter != m_vecQuest.end(); ++iter)
				{
					(*iter)->SetPosUp();
				}
			}
			else
				iter++;
		}
	}
}

void CQuestManager::Start_Quest(EQuestHeaderType eQuestHead, std::wstring szQuestText)
{
	CQuest* pQuest = nullptr;

	for (auto QuestText : m_vecQuestList)
	{
		if (0 == QuestText.first.compare(szQuestText))
		{
			pQuest = QuestText.second;
			pQuest->SetIndex(m_vecQuest.size());
			break;
		}
	}
	assert(pQuest);

	m_vecQuest.push_back(pQuest);

	m_pQuesthead->SetIcon(g_arrQuestHeaderTex[(_int)eQuestHead]);
}

void CQuestManager::End_Quest(std::wstring szQuestText)
{
	if (0 == m_vecQuest.size())
		return;

	for (auto QuestText : m_vecQuest)
	{
		if (0 == (QuestText)->GetQuestName().compare(szQuestText))
		{
			(QuestText)->ActiveQuestClear(true);
		}
	}
}

void CQuestManager::Free()
{
	for (auto& iter : m_vecQuestList)
		Safe_Release(iter.second);
	m_vecQuestList.clear();

	m_vecQuest.clear();

	Safe_Release(m_pQuesthead);
}
