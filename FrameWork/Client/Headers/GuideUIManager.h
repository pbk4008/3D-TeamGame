#ifndef GuideUIManager_h__
#define GuideUIManager_h__

#include "SingleTon.h"

BEGIN(Client)
class CUI_Guide_Background;
class CUI_Guide_Texture;
class CUI_Death;
class CGuideUIManager final :  public CSingleTon<CGuideUIManager>
{
	friend CSingleTon;

public:
	enum class TriggerIdx
	{
		First,
		Second,
		Third,
		Fourth,
		EndIndex
	};

private:
	NO_COPY(CGuideUIManager);
	CGuideUIManager(void);
	~CGuideUIManager(void) = default;

public:
	HRESULT NativeConstruct(void);
	_int    Tick(_double dTimeDelta);
	_int	Late_Tick(_double dTimeDelta);
	HRESULT	Render(void);

public:
	void Show(void);
	void ShowNext(void);
	void Hide(void);
	void ShowDeath(void);
	void Respawn(void);

	_bool IsOpenGuideUI(void);
	_bool IsOpenDeathUI(void);

private:
	void Ready_UIObject(void);

private:
	CUI_Guide_Background* m_pBg = nullptr;
	CUI_Guide_Texture*	  m_pTex = nullptr;
	CUI_Guide_Texture*    m_pInvenGuide = nullptr;
	CUI_Death*			  m_pDeath = nullptr;
	CActor*				  m_pPlayer = nullptr;
	_uint				  m_iCurrentTrigerrIdx = 0;

private:
	const std::wstring arrFirst[1]
	{
		L"T_HUD_Guide_JumpNode"
	};
	const std::wstring arrSecond[3]
	{
		L"T_HUD_Guide_LifeStone_1",
		L"T_HUD_Guide_LifeStone_2",
		L"T_HUD_Guide_LifeStone_3",
	};
	const std::wstring arrThird[2]
	{
		L"T_HUD_Guide_Shield_1",
		L"T_HUD_Guide_Shield_2",
	};
	const std::wstring arrFourth[2]
	{
		L"T_HUD_Guide_Knockdown_1",
		L"T_HUD_Guide_Knockdown_2",
	};
	std::wstring m_szTextureName;

private:
	std::vector<CUI_Guide_Texture*> m_vecGuideTex;
	_uint m_arrStage1[4] = { 1, 3, 2, 2 };
	_bool m_bGuideUIShow = false;
	_uint m_iTexIdx = 0;
	_bool m_bInvenRender = true;
	_bool m_bDeathShow = false;

private:
	virtual void Free() override;
};

END

#define SHOW_GUIDE()	g_pGuideManager->Show()
#define HIDE_GUIDE()	g_pGuideManager->Hide()

#endif // GuideUIManager_h__