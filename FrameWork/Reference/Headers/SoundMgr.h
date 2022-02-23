#pragma once
#ifndef __SOUND_MANANGER_H__
#define __SOUND_MANANGER_H__

#include "SingleTon.h"
BEGIN(Engine)
class CSoundMgr final : public CSingleTon<CSoundMgr>
{
friend CSingleTon;
public:
	enum CHANNELID {BGM, PLAYER1, PLAYER2,MONSTER, EFFECT, UI, MAXCHANNEL};
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
private:
	virtual void Free();
private:
	map<wstring, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem; 
};
END
#endif