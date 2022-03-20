#pragma once
#pragma once
#ifndef __STAGE2_H__
#define __STAGE2_H__
#include "Level.h"
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
	HRESULT Ready_MapObject();
	//HRESULT Ready_Camera(const _tchar* LayerTag);
	HRESULT Ready_Player(const _tchar* LayerTag);
	//HRESULT Ready_Boss(const _tchar* LayerTag);
	//HRESULT Ready_Monster(const _tchar* LayerTag);
	//HRESULT Ready_UI(const _tchar* LayerTag);
	//HRESULT Ready_LightDesc();


	//HRESULT Ready_Data_Effect(const _tchar* pDataFilePath);
	//HRESULT Ready_Data_UI(const _tchar* pDataFilePath);

private:
	/*HRESULT Ready_Trigger_Lod(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Light(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Scene(const _tchar* pDataFilePath);
	HRESULT Ready_Trigger_Monster(const _tchar* pDataFilePath);*/

public:
	static CStage2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;
};
END
#endif

