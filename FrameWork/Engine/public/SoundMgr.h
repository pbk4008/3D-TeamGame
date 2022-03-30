#pragma once
#ifndef __SOUND_MANANGER_H__
#define __SOUND_MANANGER_H__

#include "SingleTon.h"
#include <unordered_map>
#include "fmod.hpp"

BEGIN(Engine)
class CSoundMgr final : public CSingleTon<CSoundMgr>
{
friend CSingleTon;
public:
	enum CHANNELID { BGM, PLAYER1, PLAYER2,MONSTER, EFFECT, UI, 
		Player_Sword_Attack,
		Power_Resonance_01,
		Unequip_Needle, Equip_Needle,
		Monster_Hit, Monster_Death,
		Jump_Node,
		MAXCHANNEL};
private:
	NO_COPY(CSoundMgr);
	explicit CSoundMgr();
	virtual ~CSoundMgr() = default;
public:
	HRESULT Init_SoundManager(); 
	HRESULT Add_Sound(const wstring& pSoundTag, const char* pSoundPath);
public:
	void Play_Shot(const wstring& pSoundKey, CHANNELID eID);
	void PlayBGM(const wstring& pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

public:
	void VolumeChange(const std::wstring& pSoundKey, _float fVolume);

	using SoundType = std::pair<FMOD::Sound*, FMOD::Channel*>;

private:
	HRESULT LoadSoundFile();

private:
	virtual void Free();

private:
	std::unordered_map<std::wstring, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 
};
END
#endif