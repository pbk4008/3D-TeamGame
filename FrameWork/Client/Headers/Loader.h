#pragma once
#ifndef __LOADER_H__
#define __LOADER_H__
#include "Base.h"
#include "GameObject.h"
BEGIN(Engine)
class CTexutre;
END
BEGIN(Client)
class CLoader final : public CBase
{
private:
	explicit CLoader();
	explicit CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual~CLoader()=default;
public:
	HRESULT Init_Loader(SCENEID eID);
private:
	HRESULT LoadForScene();
private:
	HRESULT Ready_Logo();
	HRESULT Ready_Stage1();
	HRESULT Ready_Stage2();
	HRESULT Ready_Stage3();
	HRESULT Ready_Test_JS();
private:
	//stage1
	HRESULT SetUp_Stage1_Prototype();
	HRESULT SetUp_Stage1_Object();
	HRESULT Load_Stage1FBXLoad();
	HRESULT Load_Stage1Navi_SkyLoad();
	HRESULT Load_Stage1PlayerLoad();
	HRESULT Load_Stage1BossLoad();
	HRESULT Load_Stage1MonsterLoad();
	HRESULT Load_Stage1Meteor();
	HRESULT Load_Stage1StaticUILoad(); //툴에서 값 지정해주는 UI들은 여기서 미리 생성
	HRESULT Load_Stage1UILoad(); //직접 생성
	HRESULT Load_Stage1EffectLoad();
	HRESULT Load_Stage1JumpTrigger();		// 점프 트리거들 로더
	HRESULT Load_Stage1TriggerLod();
	HRESULT	Load_Stage1_TreasureChest_Load();
	HRESULT Load_Stage1_Cinema_Object();
private:
	//Stage2
	HRESULT Set_Stage2_Prototype();
	HRESULT Load_Stage2_Object();
private:
	//쓰레드에 로딩할 파일 추가(컴포넌트 태그, 로딩할 파일 패스, 타입(0 = Static, 1 = Ainm, 2 = Instancing_Static)
	void Add_LoadingThread(const wstring& pComponetTag, const wstring& pFilePath, _uint iType);
public:
	static _uint CALLBACK Thread_Main(void* pArg);
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, SCENEID eID);
private:
	virtual void Free();
public:
	CRITICAL_SECTION* getCritical() { return &m_Critical; }
	SCENEID getNextScene() { return m_eNextScene; }
	_bool getFinish() { return m_bFinish; }
private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_Critical;
	_bool m_bFinish;
	SCENEID m_eNextScene;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	/*vector<CTexutre*> m_vectexture;*/
};
END
#endif