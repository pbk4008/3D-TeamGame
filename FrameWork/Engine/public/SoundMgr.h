#pragma once
#ifndef __SOUND_MANANGER_H__
#define __SOUND_MANANGER_H__

#include "SingleTon.h"
#include <unordered_map>
#include "../../Reference/Headers/fmod/fmod.hpp"

BEGIN(Engine)
class CSoundMgr final : public CSingleTon<CSoundMgr>
{
friend CSingleTon;
public:
	enum  CHANNELID { BGM, PLAYER1, PLAYER2, PLAYER3, MONSTER, EFFECT, UI,
		Player_Sword_Attack,
		Power_Resonance_01,
		Unequip_Needle, Equip_Needle,
		Unequip_Shield, Equip_Shield,
		Monster_Hit, Monster_Death, Monster_Attack, Monster_Attack_2,
		Earth_Hit, Earth_Death, Earth_Attack_1, Earth_Attack_2,
		Sword1H_Hit, Sword1H_Death, Sword1H_Attack_1, Sword1H_Attack_2,
		Sword2H_Hit, Sword2H_Death,
		Shooter_Hit, Shooter_Death, Shooter_Attack_1, Shooter_Attack_2,
		Healer_Hit, Healer_Death,
		Spear_Hit, Spear_Death, Spear_Attack_1, Spear_Attack_2,
		Bronz_Hit, Bronz_Death,
		MidBoss, MidBoss_Hit, MidBoss_Death, MidBoss_Attack_1, MidBoss_Attack_2, MidBoss_Attack_3,
		Jump_Node,
		Item_Drop, Item_Drop_2,
		Orin, Ravenna, Soras, Sanctum,
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
	_bool IsPlaying(CHANNELID eID);

public:
	void VolumeChange(CHANNELID eID,_float fVolume);
	void BlendSound(
		const std::wstring& pStartSoundKey, 
		const std::wstring& pEndSoundKey, 
		CHANNELID eStartID, 
		CHANNELID eEndID, 
		_float fBlendTiming);

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
	FMOD_CHANNELGROUP* m_pGroup;
};
END
#endif