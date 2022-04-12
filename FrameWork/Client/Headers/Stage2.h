#pragma once
#pragma once
#ifndef __STAGE2_H__
#define __STAGE2_H__
#include "Level.h"
#include "TriggerSystem.h"

BEGIN(Client)
class CStage2 final : public CLevel
{
private:
	explicit CStage2() = default;
	explicit CStage2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CStage2() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
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


	//HRESULT Ready_Data_Effect(const _tchar* pDataFilePath);
	//HRESULT Ready_Data_UI(const _tchar* pDataFilePath);

private:
	HRESULT Ready_TriggerSystem(const _tchar* pTriggerFile);
	HRESULT Ready_TriggerFunctionSetting();
private:
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

	/*HRESULT Ready_Trigger_Lod(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Light(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Scene(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Monster(const _tchar* pDataFilePath);*/
public:
	void Minus_MonsterCount() { m_iCountMonster--; }
public:
	static CStage2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;
private:
	CTriggerSystem<CStage2>* m_pTriggerSystem;
	_bool m_bDebug;
	_uint m_iCountMonster;
	_bool m_bFirst;
};
END
#endif

