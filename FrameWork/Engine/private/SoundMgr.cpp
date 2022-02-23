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

	//if (FAILED(LoadSoundFile()))
	//	return E_FAIL;

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


void CSoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
		FMOD_Sound_Release(Mypair.second);

	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
