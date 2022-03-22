#pragma once
#ifndef __STAGE1_H__
#define __STAGE1_H__
#include "Level.h"
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


	HRESULT Ready_Data_Effect(const _tchar* pDataFilePath);
	HRESULT Ready_Data_UI(const _tchar* pDataFilePath);

private:
	HRESULT Ready_Trigger_Lod(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Light(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Scene(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Monster(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Quest(const _tchar* pDataFilePath);

private:
	HRESULT	Ready_Treasure_Chest();

public:
	static CStage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;
};
#endif

END