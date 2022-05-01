#include "pch.h"
#include "VoiceManager.h"
#include "Subtitles.h"

CVoiceManager::CVoiceManager(void)
{
}

HRESULT CVoiceManager::NativeConstruct(SCENEID _iSceneID)
{
	m_iSSceneID = (_int)_iSceneID;

	Ready_TriggerFile();

	return S_OK;
}

_int CVoiceManager::Tick(_double dTimeDelta)
{
	ShowNext();

	m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->Tick(dTimeDelta);
	
	return _int();
}

_int CVoiceManager::Late_Tick(_double dTimeDelta)
{
	if (m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->getActive())
	{
		if (false == m_bPlayOne)
		{
 			g_pGameInstance->Play_Shot(m_szSoundKey, m_EChannel);
			g_pGameInstance->VolumeChange(m_EChannel, 1.4f);

			m_bPlayOne = true;
			m_bIsPlaying = true;
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::BGM, 0.5f);
		}

		if (false == g_pGameInstance->IsPlaying(m_EChannel))
		{
			m_bIsPlaying = false;
			m_bPlayOne = false;
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::BGM, 1.0f);

		}

		if (L"TheMonolithIs" == m_szSoundKey || L"HeNearlyHas" == m_szSoundKey ||
			L"HowDidYou" == m_szSoundKey || L"WhatDoYou" == m_szSoundKey)
			m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->UpScaleing();
		else
			m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->DownScaleing();

		m_bSubtileShow = true;
		m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->LateTick(dTimeDelta);
	}

	return _int();
}

HRESULT CVoiceManager::Render(void)
{
	return S_OK;
}

void CVoiceManager::Ready_TriggerFile(void)
{
	if (3 == m_iSSceneID)
	{
		vector<TRIGGER> vecPonit;
		if (FAILED(g_pGameInstance->LoadFile<TRIGGER>(vecPonit, L"../bin/SaveData/Trigger/Stage1_Voice.dat")))
			MSGBOX(L"자막트리거 데이터 못읽음");
		if (0 == vecPonit.size())
			MSGBOX(L"자막트리거 데이터 못읽음");

		CSubtitles* pSubTitle = nullptr;
		m_vecSubtiltesTex.reserve(vecPonit.size());

		for (_int i = 0; i < vecPonit.size(); ++i)
		{
			CSubtitles::Desc desc;
			desc.fPos = vecPonit[i].fTrigger_Point;
			desc.iNumTex = m_arrStage1[i];

			pSubTitle = (CSubtitles*)static_cast<CUI*>(
				g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Subtitle", &desc));
			assert(pSubTitle);

			pSubTitle->setActive(false);
			m_vecSubtiltesTex.push_back(pSubTitle);
		}
	}
	else if (4 == m_iSSceneID)
	{
		vector<TRIGGER> vecPonit;
		if (FAILED(g_pGameInstance->LoadFile<TRIGGER>(vecPonit, L"../bin/SaveData/Trigger/Stage2_Voice.dat")))
			MSGBOX(L"자막트리거 데이터  못읽음");
		if (0 == vecPonit.size())
			MSGBOX(L"자막트리거 데이터 못읽음");

		CSubtitles* pSubTitle = nullptr;
		m_vecSubtiltesTex.reserve(vecPonit.size());

		for (_int i = 0; i < vecPonit.size(); ++i)
		{
			CSubtitles::Desc desc;
			desc.fPos = vecPonit[i].fTrigger_Point;
			desc.iNumTex = m_arrStage2[i];

			pSubTitle = (CSubtitles*)static_cast<CUI*>(
				g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Subtitle", &desc));
			assert(pSubTitle);

			pSubTitle->setActive(false);
			m_vecSubtiltesTex.push_back(pSubTitle);
		}
	}
	else if (5 == m_iSSceneID)
	{
		vector<TRIGGER> vecPonit;
		if (FAILED(g_pGameInstance->LoadFile<TRIGGER>(vecPonit, L"../bin/SaveData/Trigger/Stage3_Voice.dat")))
			MSGBOX(L"자막트리거 데이터  못읽음");
		if (0 == vecPonit.size())
			MSGBOX(L"자막트리거 데이터 못읽음");

		CSubtitles* pSubTitle = nullptr;
		m_vecSubtiltesTex.reserve(vecPonit.size());

		for (_int i = 0; i < vecPonit.size(); ++i)
		{
			CSubtitles::Desc desc;
			desc.fPos = vecPonit[i].fTrigger_Point;
			desc.iNumTex = m_arrStage3[i];

			pSubTitle = (CSubtitles*)static_cast<CUI*>(
				g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Subtitle", &desc));
			assert(pSubTitle);

			pSubTitle->setActive(false);
			m_vecSubtiltesTex.push_back(pSubTitle);
		}
	}
}

void CVoiceManager::ShowNext(void)
{
	/* 기본 이미지, 사운드 설정 */
	if (3 == m_iSSceneID)
	{
		switch (m_iCurrentTrigerrIdx)
		{
		case 0:
			m_szTextureName = arrFirst[m_iTexIdx];
			m_szSoundKey = arrVoiceFirst[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1); /* 사운드 파일명 맨뒤 _ 문자기준으로 Voice 주인 찾아냄 */
			break;
		case 1:
			m_szTextureName = arrSecond[m_iTexIdx];
			m_szSoundKey = arrVoiceSecond[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
			break;
		case 2:
			m_szTextureName = arrThird[m_iTexIdx];
			m_szSoundKey = arrVoiceThird[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
			break;
		case 3:
			m_szTextureName = arrFourth[m_iTexIdx];
			m_szSoundKey = arrVoiceFourth[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
			break;
		}
		m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->SetImage(m_szTextureName);

		if (L"Orin" == szTemp)
			m_EChannel = CSoundMgr::CHANNELID::Orin;
		else if (L"Ravenna" == szTemp)
			m_EChannel = CSoundMgr::CHANNELID::Ravenna;
		else if (L"Soras" == szTemp)
			m_EChannel = CSoundMgr::CHANNELID::Soras;
		else if (L"Sanctum" == szTemp)
			m_EChannel = CSoundMgr::CHANNELID::Sanctum;

		/* 이전 재생 중인 채널이 끝난 경우 */
		if (true == m_bSubtileShow && false == m_bIsPlaying)
		{
			if (m_iTexIdx + 1 < m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->GetNumTex())
			{
				++m_iTexIdx;

				switch (m_iCurrentTrigerrIdx)
				{
				case 0:
					/*T_Voice_ItsSluggish_Orin*/
					m_szTextureName = arrFirst[m_iTexIdx];
					/* IsYourArmor */
					m_szSoundKey = arrVoiceFirst[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1); /* 사운드 파일명 맨뒤 _ 문자기준으로 Voice 주인 찾아냄 */
					break;
				case 1:
					m_szTextureName = arrSecond[m_iTexIdx];
					m_szSoundKey = arrVoiceSecond[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
					break;
				case 2:
					m_szTextureName = arrThird[m_iTexIdx];
					m_szSoundKey = arrVoiceThird[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
					break;
				case 3:
					m_szTextureName = arrFourth[m_iTexIdx];
					m_szSoundKey = arrVoiceFourth[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
					break;
				}
				m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->SetImage(m_szTextureName);

				if (L"Orin" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Orin;
				else if (L"Ravenna" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Ravenna;
				else if (L"Soras" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Soras;
				else if (L"Sanctum" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Sanctum;
			}
			else
			{
				/* 현재 트리거에서 재생될 분량이 끝난 경우 */
				m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->setActive(false);
				m_iTexIdx = 0;

				if (m_iCurrentTrigerrIdx < 4)
					++m_iCurrentTrigerrIdx;

				if (4 == m_iCurrentTrigerrIdx)
					m_iCurrentTrigerrIdx = 0;

				m_bSubtileShow = false;
			}
		}
	}

	if (4 == m_iSSceneID)
	{
		switch (m_iCurrentTrigerrIdx)
		{
		case 0:
			m_szTextureName = arrFirst2[m_iTexIdx];
			m_szSoundKey = arrVoiceFirst2[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1); /* 사운드 파일명 맨뒤 _ 문자기준으로 Voice 주인 찾아냄 */
			break;
		case 1:
			m_szTextureName = arrSecond2[m_iTexIdx];
			m_szSoundKey = arrVoiceSecond2[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
			break;
		case 2:
			m_szTextureName = arrThird2[m_iTexIdx];
			m_szSoundKey = arrVoiceThird2[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
			break;
		case 3:
			m_szTextureName = arrFourth2[m_iTexIdx];
			m_szSoundKey = arrVoiceFourth2[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
			break;
		case 4:
			m_szTextureName = arrFifth2[m_iTexIdx];
			m_szSoundKey = arrVoiceFifth2[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
			break;
		}
		m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->SetImage(m_szTextureName);

		if (L"Orin" == szTemp)
			m_EChannel = CSoundMgr::CHANNELID::Orin;
		else if (L"Sanctum" == szTemp)
			m_EChannel = CSoundMgr::CHANNELID::Sanctum;

		/* 이전 재생 중인 채널이 끝난 경우 */
		if (true == m_bSubtileShow && false == m_bIsPlaying)
		{
			m_szTextureName = L"";
			m_szSoundKey = L"";

			if (m_iTexIdx + 1 < m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->GetNumTex())
			{
				++m_iTexIdx;

				switch (m_iCurrentTrigerrIdx)
				{
				case 0:
					/*T_Voice_ItsSluggish_Orin*/
					m_szTextureName = arrFirst2[m_iTexIdx];
					/* IsYourArmor */
					m_szSoundKey = arrVoiceFirst2[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1); /* 사운드 파일명 맨뒤 _ 문자기준으로 Voice 주인 찾아냄 */
					break;
				case 1:
					m_szTextureName = arrSecond2[m_iTexIdx];
					m_szSoundKey = arrVoiceSecond2[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
					break;
				case 2:
					m_szTextureName = arrThird2[m_iTexIdx];
					m_szSoundKey = arrVoiceThird2[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
					break;
				case 3:
					m_szTextureName = arrFourth2[m_iTexIdx];
					m_szSoundKey = arrVoiceFourth2[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
					break;
				case 4:
					m_szTextureName = arrFifth2[m_iTexIdx];
					m_szSoundKey = arrVoiceFifth2[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1);
					break;
				}
				m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->SetImage(m_szTextureName);

				if (L"Orin" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Orin;
				else if (L"Ravenna" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Ravenna;
				else if (L"Soras" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Soras;
				else if (L"Sanctum" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Sanctum;
			}
			else
			{
				/* 현재 트리거에서 재생될 분량이 끝난 경우 */
				m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->setActive(false);
				m_iTexIdx = 0;

				if (m_iCurrentTrigerrIdx < 5)
					++m_iCurrentTrigerrIdx;

				if (5 == m_iCurrentTrigerrIdx)
					m_iCurrentTrigerrIdx = 0;

				m_bSubtileShow = false;
			}
		}
	}

	if (5 == m_iSSceneID)
	{
		switch (m_iCurrentTrigerrIdx)
		{
		case 0:
			m_szTextureName = arrFirst3[m_iTexIdx];
			m_szSoundKey = arrVoiceFirst3[m_iTexIdx];
			szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1); /* 사운드 파일명 맨뒤 _ 문자기준으로 Voice 주인 찾아냄 */
			break;
		}
		m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->SetImage(m_szTextureName);

		if (L"Orin" == szTemp)
			m_EChannel = CSoundMgr::CHANNELID::Orin;
		else if (L"Sanctum" == szTemp)
			m_EChannel = CSoundMgr::CHANNELID::Sanctum;

		/* 이전 재생 중인 채널이 끝난 경우 */
		if (true == m_bSubtileShow && false == m_bIsPlaying)
		{
			m_szTextureName = L"";
			m_szSoundKey = L"";

			if (m_iTexIdx + 1 < m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->GetNumTex())
			{
				++m_iTexIdx;

				switch (m_iCurrentTrigerrIdx)
				{
				case 0:
					/*T_Voice_ItsSluggish_Orin*/
					m_szTextureName = arrFirst3[m_iTexIdx];
					/* IsYourArmor */
					m_szSoundKey = arrVoiceFirst3[m_iTexIdx];
					szTemp = m_szTextureName.substr(m_szTextureName.rfind(L"_") + 1); /* 사운드 파일명 맨뒤 _ 문자기준으로 Voice 주인 찾아냄 */
					break;
				}
				m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->SetImage(m_szTextureName);

				if (L"Orin" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Orin;
				else if (L"Sanctum" == szTemp)
					m_EChannel = CSoundMgr::CHANNELID::Sanctum;
			}
			else
			{
				/* 현재 트리거에서 재생될 분량이 끝난 경우 */
				m_vecSubtiltesTex[m_iCurrentTrigerrIdx]->setActive(false);
				m_iTexIdx = 0;

				if (m_iCurrentTrigerrIdx < 1)
					++m_iCurrentTrigerrIdx;

				if (1 == m_iCurrentTrigerrIdx)
					m_iCurrentTrigerrIdx = 0;

				m_bSubtileShow = false;
			}
		}
	}
}


void CVoiceManager::Free()
{
	for (auto iter : m_vecSubtiltesTex)
		Safe_Release(iter);
	m_vecSubtiltesTex.clear();
}
