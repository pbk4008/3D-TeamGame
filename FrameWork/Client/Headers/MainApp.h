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
	HRESULT Destroy();

public: void Change_RenderCamTag(const wstring& tag) { m_pRenderer->SetCameraTag(tag); }

public: void Set_RenderBtn(CRenderer::RENDERBUTTON type, _bool check) { m_pRenderer->SetRenderButton(type, check); }

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	CRenderer*				m_pRenderer = nullptr;

private:
	_double				m_TimeAcc = 0.0;
	_uint				m_iNumRender = 0;
	_tchar				m_szFPS[MAX_PATH] = TEXT("");

	_bool				m_isPause = false;
	_bool				m_bHalfView = true;
	_bool				m_isRender = true;
	_bool				m_bHDR = true;
	_bool				m_bDBG = true;
	_bool				m_bShadow = true;
	_bool				m_bOutline = true;
	_bool				m_bMotionblur = true;
	_bool				m_bMotiontrail = true;

	_bool				m_isFreeze = false;
	_bool				m_isDeltaTimeZero = false;
	_double				m_dFreezeTimeAcc = 0.0;

private:
	HRESULT SetUp_StartLevel(SCENEID eLevel);
	HRESULT Ready_GameManager(void);

	/* For. Level_static*/
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Load_Texture();
	HRESULT Init_Camera();
	HRESULT Ready_Fonts();
	void	RenderingBnt();

public:
	const _bool IsFreeze() const;
	const _bool IsDeltaTimeZero() const;
	void FreezeTime();
	void Set_DeltaTimeZero(const _bool _isDetaTimeZero);
	
public:
	static CMainApp* Create();
	virtual void Free() override;
};

END