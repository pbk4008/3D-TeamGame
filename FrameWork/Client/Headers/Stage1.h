#pragma once
#ifndef __STAGE1_H__
#define __STAGE1_H__
#include "Level.h"
class CStage1 final : public CLevel
{
public:
	typedef struct tagEnvironMentLoadData
	{
		_tchar FolderName[MAX_PATH];
		_tchar FileName[MAX_PATH];
		_tchar Tag[MAX_PATH];
		_tchar Name[MAX_PATH];
		_float4x4 WorldMat;
		_float3 fInitPos;
		LEVEL_ID eLevel;
		_uint	  iType;
	}ENVIRONMENTLOADDATA;
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
	HRESULT Ready_Monster(const _tchar* LayerTag);
	HRESULT Ready_UI(const _tchar* LayerTag);


	HRESULT Ready_Data_Effect(const _tchar* pDataFilePath);
	HRESULT Ready_Data_UI(const _tchar* pDataFilePath);
public:
	static CStage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;
};
#endif