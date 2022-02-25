#pragma once
#ifndef MFCMainApp_h__
#define MFCMainApp_h__

#include "UI_Tool_Define.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

class CMFCMainApp final : public CBase
{
private:
	CMFCMainApp();
	virtual ~CMFCMainApp() = default;

public:
	HRESULT NativeConstruct(); //initialize
	_int Tick(_double TimeDelta); //update
	HRESULT Render();

private:
	CGameInstance* m_pGameInstance = nullptr;
	CRenderer* m_pRenderer = nullptr;

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	HRESULT SetUp_StartLevel(TOOL_LEVEL eLevel);

	/* For.Level_Static */
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();

public:
	static CMFCMainApp* Create();
	virtual void Free() override;
};

#endif // MFCMainApp_h__


