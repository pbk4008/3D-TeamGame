#pragma once
#ifndef __STAGE1_H__
#define __STAGE1_H__
#include "Level.h"
#include "TriggerSystem.h"

BEGIN(Client)
class CStage1 final : public CLevel
{
private:
	explicit CStage1();
	explicit CStage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CStage1() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT Ready_MapObject();
	HRESULT Ready_Camera(const _tchar* LayerTag);
	HRESULT Ready_Player(const _tchar* LayerTag);
	HRESULT Ready_Boss(const _tchar* LayerTag);
	HRESULT Ready_Monster(const _tchar* LayerTag);
	HRESULT Ready_UI(const _tchar* LayerTag);
	HRESULT Ready_Light();

	HRESULT Ready_Data_Effect();
	HRESULT Ready_Data_UI(const _tchar* pDataFilePath);

	HRESULT Ready_TriggerSystem(const _tchar* pTriggerFile);
	HRESULT Ready_TriggerFunctionSetting();
public:
	void Minus_MonsterCount() { m_iCountMonster--; }
public:
	void Trgger_Function1();//1�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function2();//2�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function3();//3�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function4();//4�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function5();//5�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function6();//6�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function7();//7�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function8();//8�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function9();//9�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function10();//10�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_Function11();//11�� Ʈ���Ű� �۵��� �Լ�
	void Trgger_FunctionBoss();//���� Ʈ���Ű� �۵��� �Լ�
	/// ////////////////////////////////////
	HRESULT Ready_Trigger_Lod(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Light(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Scene(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Monster(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Quest(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Jump();
private:
	HRESULT	Ready_Treasure_Chest();
public:
	static CStage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;
private:
	CTriggerSystem<CStage1>* m_pTriggerSystem;
	_bool m_bDebug;
	_uint m_iCountMonster;
	_bool m_bFirst;
};
#endif

END