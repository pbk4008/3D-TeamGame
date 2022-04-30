#pragma once
#ifndef __STAGE2_H__
#define __STAGE2_H__
#include "Level.h"
#include "TriggerSystem.h"

BEGIN(Client)
class CDropBoxData;
class CStage2 final : public CLevel
{
private:
	explicit CStage2() = default;
	explicit CStage2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CStage2() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT	Ready_Light();
	HRESULT Ready_NaviMesh();
	HRESULT Ready_MapObject();
	//HRESULT Ready_Camera(const _tchar* LayerTag);
	HRESULT Ready_Player(const _tchar* LayerTag);
	//HRESULT Ready_Boss(const _tchar* LayerTag);
	//HRESULT Ready_Monster(const _tchar* LayerTag);
	HRESULT Ready_UI(const _tchar* LayerTag);
	HRESULT Ready_Data_UI(const _tchar* pDataFilePath);
	//HRESULT Ready_LightDesc();
	HRESULT Ready_JumpTrigger();
	HRESULT Ready_Portal();

	HRESULT Ready_Data_Effect(const _tchar* pDataFilePath);
	//HRESULT Ready_Data_UI(const _tchar* pDataFilePath);

private:
	HRESULT Ready_TriggerSystem(const _tchar* pTriggerFile);
	HRESULT Ready_TriggerFunctionSetting();
public:
	void Trgger_Function1();
	void Trgger_Function2();
	void Trgger_Function3();
	void Trgger_Function4();
	void Trgger_Function5();
	void Trgger_Function6();
	void Trgger_Function7();
	void Trgger_Function8();
	void Trgger_Function9();
	void Trgger_FunctionBoss();
private:
	void Portal_Spot1();
	void Portal_Spot2();
	void Portal_Spot3();
	void Portal_Spot4();
	void Portal_Spot5();
	
private:
	HRESULT Ready_Treasure_Chest(void);
public:
	void Minus_MonsterCount() { m_iCountMonster--; }
private:
	void Open_Potal(_fvector vPos, _uint iMonTag);
public:
	static CStage2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;

private:
	std::vector<CDropBoxData*> m_pDumyDropData;

private:
	CTriggerSystem<CStage2>* m_pTriggerSystem;
	_bool m_bDebug;
	_uint m_iCountMonster;
	_bool m_bFirst;
	_uint m_iPortalCount = 0;
	_bool m_bPortalClear = false;
};
END
#endif

