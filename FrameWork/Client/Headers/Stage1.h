#pragma once
#ifndef __STAGE1_H__
#define __STAGE1_H__
#include "Level.h"
#include "TriggerSystem.h"
BEGIN(Client)
class CScenematicManager;
class CScenematic;
class CMeteor;
class CIndicator_Manager;
class CDropBoxData;
class CWall;
class CPot;
class CStage1 final : public CLevel
{
private:
	explicit CStage1();
	explicit CStage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CStage1() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT Ready_MapObject();
	HRESULT Ready_Camera(const _tchar* LayerTag);
	HRESULT Ready_Player(const _tchar* LayerTag);
	HRESULT Ready_Boss(const _tchar* LayerTag);
	HRESULT Ready_Monster(const _tchar* LayerTag);
	HRESULT Ready_UI(const _tchar* LayerTag);
	HRESULT Ready_Light();
	HRESULT Ready_GameManager(void);
	HRESULT Ready_Data_Effect();
	HRESULT Ready_Data_UI(const _tchar* pDataFilePath);
	HRESULT Ready_Indicator();

	HRESULT Ready_TriggerSystem(const _tchar* pTriggerFile);
	HRESULT Ready_TriggerFunctionSetting();

	HRESULT Ready_Cinema();
	HRESULT Ready_Wall();
	HRESULT Ready_Pot();
	HRESULT Ready_Obstacle();

public:
	void Minus_MonsterCount() { m_iCountMonster--; }
public:
	void Trgger_Function1();//1번 트리거가 작동할 함수
	void Trgger_Function2();//2번 트리거가 작동할 함수
	void Trgger_Function3();//3번 트리거가 작동할 함수
	void Trgger_Function4();//4번 트리거가 작동할 함수
	void Trgger_Function5();//5번 트리거가 작동할 함수
	void Trgger_Function7();//7번 트리거가 작동할 함수
	void Trgger_Function8();//8번 트리거가 작동할 함수
	void Trgger_Function9();//9번 트리거가 작동할 함수
	void Trgger_Function10();//10번 트리거가 작동할 함수
	void Trgger_Function11();//11번 트리거가 작동할 함수
	void Trgger_FunctionBoss();//보스 트리거가 작동할 함수
	/// ////////////////////////////////////
	HRESULT Ready_Trigger_Lod(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Light(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Scene(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Monster(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Quest(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Jump();
private:
	HRESULT	Ready_Treasure_Chest();
private:
	HRESULT Ready_Meteor();
	void Shoot_Meteor(_double dDeltaTime);
	CMeteor* Find_Meteor();
private:
	HRESULT Ready_Portal();
	void Open_Potal(_fvector vPos, _uint iMonTag);
	void CheckTriggerForQuest(void);
private:
	void Portal_Spot1();
	void Portal_Spot2();
	void Portal_Spot3();
	void Portal_Spot4();
	void Portal_Spot5();
	void Portal_Spot6();
private:
	void Open_Wall();
public:
	static CStage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;
private:
	CTriggerSystem<CStage1>* m_pTriggerSystem;
	CScenematicManager* m_pScenemaManager;
	CIndicator_Manager* m_pIndicatorManager = nullptr;

	std::vector<CDropBoxData*> m_pDumyDropData;
	_bool m_bDebug;
	_uint m_iCountMonster;
	_bool m_bFirst;
	_bool test = false;
private:/*for Meteor*/
	vector<CMeteor*> m_vecMeteor;
	vector<_float4> m_vecMeteorPos;
	_float m_fAccMeteorSpawn;
	_float m_fRandomMeteorSpawnTime;
	_float m_fAccMeteorStartTime;
private:
	_int m_iPortalCount;
	_bool m_bPortalClear = false;

private:
	_bool m_bBossClear=false;
};
#endif

END