#ifndef GuideUIManager_h__
#define GuideUIManager_h__

#include "SingleTon.h"

BEGIN(Client)
class CUI_Guide_Background;
class CUI_Guide_Texture;
class CGuideUIManager final :  public CSingleTon<CGuideUIManager>
{
	friend CSingleTon;

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
	void Hide(void);

private:
	void Ready_UIObject(void);

private:
	CUI_Guide_Background* m_pBg = nullptr;
	CUI_Guide_Texture*	  m_pTex = nullptr;

private:
	virtual void Free() override;
};

END

#define SHOW_GUIDE()	g_pGuideManager->Show()
#define HIDE_GUIDE()	g_pGuideManager->Hide()

#endif // GuideUIManager_h__