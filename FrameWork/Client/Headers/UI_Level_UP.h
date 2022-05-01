#ifndef UI_Level_UP_h__
#define UI_Level_UP_h__

#include "Hud.h"

BEGIN(Client)
class CUI_LevelUP_Background;
class CUI_LevelUP_BG_Left;
class CUI_LevelUP_BG_Right;
class CUI_LevelUP_Fill_Lead_Left;
class CUI_LevelUP_Fill_Lead_Right;
class UI_LevelUP_Fill_Left;
class UI_LevelUP_Fill_Right;
class CUI_LevelHUD;

class CLevel_UP : public CUI
{
public:
	const std::wstring m_arrLevelTex[9]
	{
		L"T_HUD_2Level",
		L"T_HUD_2Level",
		L"T_HUD_3Level",
		L"T_HUD_4Level",
		L"T_HUD_5Level",
		L"T_HUD_6Level",
		L"T_HUD_7Level",
		L"T_HUD_8Level",
		L"T_HUD_9Level",
	};

public:
	explicit CLevel_UP(void) = default;
	explicit CLevel_UP(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CLevel_UP(const CUI & rhs);
	virtual ~CLevel_UP() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	HRESULT Ready_UIObject(void);

public:
	void Show(CPlayerData* pPlayerData);
	void Hide(void);
	void ShowUp(_double TimeDelta);
	void FadeOutAll(void);
	void FadeInAll(void);
	void ShowLevelUp_HUD(_int iLevel);
	void HideLevelUp_HUD(void);

public:
	void SetLevelBG(_int PlayerLevel);
	void FixPos(void);

public:
	static CLevel_UP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg) override;

private:
	CUI_LevelUP_Background*			m_pBg = nullptr;
	CUI_LevelUP_BG_Left*		    m_pBgLeft = nullptr;
	CUI_LevelUP_BG_Right*			m_pBgRight = nullptr;
	CUI_LevelUP_Fill_Lead_Left*		m_pFill_Lead_Left = nullptr;
	CUI_LevelUP_Fill_Lead_Right*	m_pFill_Lead_Right = nullptr;
	UI_LevelUP_Fill_Left*			m_pFIll_Left = nullptr;
	UI_LevelUP_Fill_Right*			m_pFIll_Right = nullptr;
	CUI_LevelHUD*				    m_pLevelUp_HUD = nullptr;

private:
	_float m_fInitYPos = -60.f;
	_float m_fEndYPos  = 0.f;

	_float m_fHideYInitPos = 0.f;
	_float m_fHideYEndPos = -60.f;

	_float m_fTimeLevelUp_HUD = 0.f;
private:
	virtual void Free() override;
};

END

#endif // UI_Level_UP_h__