#pragma once

#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END
	
BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT NativeConstruct();
	_int Tick(_double TimeDelta);
	HRESULT Render();
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	CRenderer*				m_pRenderer = nullptr;

private:
	_double				m_TimeAcc = 0.0;
	_uint				m_iNumRender = 0;
	_tchar				m_szFPS[MAX_PATH] = TEXT("");

private:
	HRESULT SetUp_StartLevel(SCENEID eLevel);

	/* For. Level_static*/
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();

	HRESULT Ready_Fonts();
	
public:
	static CMainApp* Create();
	virtual void Free() override;
};

END