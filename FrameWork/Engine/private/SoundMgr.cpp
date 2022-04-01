#include "SoundMgr.h"


CSoundMgr::CSoundMgr()
	: m_pSystem(nullptr)
{
	for (auto& i : m_pChannelArr)
		i = nullptr;
}

HRESULT CSoundMgr::Init_SoundManager()
{
	FMOD_RESULT hr = FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	if (hr != FMOD_OK)
		return E_FAIL;
	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	if (FMOD_OK != FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL))
		return E_FAIL;

	if (FAILED(LoadSoundFile()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSoundMgr::Add_Sound(const wstring& pSoundTag, const char* pSoundPath)
{
	FMOD_SOUND* pSound = nullptr;

	FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, pSoundPath, FMOD_LOOP_OFF, 0, &pSound);

	if (eRes == FMOD_OK)
		m_mapSound.emplace(pSoundTag, pSound);

	return S_OK;
}

void CSoundMgr::Play_Shot(const wstring& pSoundKey, CHANNELID eID)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE; 
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(const wstring& pSoundKey)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem,iter->second, nullptr, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0 ; i < MAXCHANNEL ; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::VolumeChange(const std::wstring& pSoundKey, _float fVolume)
{
	/*FMOD_RESULT Result;
	_bool bPlay = false;

	const _float OriginVolume = fVolume;

	auto iter = m_mapSound.find(pSoundKey);

	if(iter != std::end(m_mapSound))
	{
		iter;
	}*/
}

HRESULT CSoundMgr::LoadSoundFile()
{
	// BGM
	FMOD_SOUND* pSound = nullptr;
	FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem,
		"../bin/Resources/Sound/Bgm/BM_ZU_AveyronPeak_78_v3_body_38bars.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Stage1_BGM", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Swing_Short_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Swing_Short_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Swing_Short_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_3", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Swing_Long_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_4", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Power_Resonance_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_4_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Unsheath_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_UnEquip", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Unsheath_04.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Equip", pSound); else return E_FAIL;
	
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Blind_Attack_Cast_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_P1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Player_Weapon_Swap_Longsword_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_P2", pSound); else return E_FAIL;

	/* for.Monster */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/A_Attack_Liquid_Hit_Generic_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Hit", pSound); else return E_FAIL;

	/* for. Crawler */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Crawler/A_Vox_CrystalCrawler_Hit_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Crawler_Death", pSound); else return E_FAIL;

	/* for.Jump Node */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/JumpNode/A_Effect_JumpNode_Open_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"JumpNode_Open", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/JumpNode/A_Effect_JumpNode_Close_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"JumpNode_Close", pSound); else return E_FAIL;

	FMOD_System_Update(m_pSystem);

	return S_OK;
}


void CSoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
		FMOD_Sound_Release(Mypair.second);

	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
