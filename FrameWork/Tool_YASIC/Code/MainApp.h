#pragma once

BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

BEGIN(Tool)

class CMainApp final : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT NativeConstruct();
	_int Tick(_double _dDeltaTime);
	HRESULT Render();

public:
	HRESULT SetUp_StartScene(ELevel _eScene);

private: /* For.Scene_Static */
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();

public:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	CGameInstance* m_pGameInstance = nullptr;
	CRenderer* m_pRenderer = nullptr;

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END