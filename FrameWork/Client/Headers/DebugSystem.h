#pragma once
#ifndef __DEBUG_SYSTEM_H__
#define __DEBUG_SYSTEM_H__

#include "SingleTon.h"
BEGIN(Engine)
class CRenderer;
END
BEGIN(Client)
class CDebugSystem final : public CSingleTon<CDebugSystem>
{
	friend CSingleTon;
private:
	NO_COPY(CDebugSystem);
	explicit CDebugSystem();
	virtual ~CDebugSystem() = default;
public:
	HRESULT Init_DebugSystem(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext, CRenderer* pRenderer);
public:
	HRESULT Input_Number();
public:
	void Set_LevelcMoveCheck(_bool bCheck);
public:
	_bool Get_LevelMoveCheck(_int& bLevel);
private:
	HRESULT  Debug_Functions();
	HRESULT  Level_Move();
	void Print_Level();
public:
	static _uint CALLBACK  Update_DebugSystem(void* pArg);
private:
	virtual void Free() override;
public:
	CRITICAL_SECTION* getCritical() { return &m_Critical; }
	static void Stop_DebugSystem() { m_pInstance->m_bExit = true; }
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_Critical;
	_int m_iNumber;
	_int m_iSelectLevel;
	_bool m_bLevelMove;
	_bool m_bExit;
	CRenderer* m_pRenderer;
};
END
#endif