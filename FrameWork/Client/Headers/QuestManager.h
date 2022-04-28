#ifndef QuestManager_h__
#define QuestManager_h__

#include "SingleTon.h"

BEGIN(Client)
class CQuest;
class CUI_QuestHeadText;
class CQuestManager final :  public CSingleTon<CQuestManager>
{
	friend CSingleTon;

private:
	NO_COPY(CQuestManager);
	CQuestManager(void);
	~CQuestManager(void) = default;

public:
	HRESULT NativeConstruct(void);
	_int    Tick(_double dTimeDelta);
	_int	Late_Tick(_double dTimeDelta);
	HRESULT	Render(void);

public:
	void	Start_Quest(EQuestHeaderType eQuestHead, std::wstring szQuestText);
	void	End_Quest(std::wstring szQuestText);

private:
	void	Ready_Quest(void);
	void    PullingQuest(void);

public:
	void	SetRender(_bool bOnOff) { m_bRender = bOnOff; }

private:
	CUI_QuestHeadText*  m_pQuesthead = nullptr;

private:
	std::vector<std::pair<std::wstring, CQuest*> > m_vecQuestList; /* 담아두는 용도 */
	std::vector<CQuest*> m_vecQuest;							   /* 틱 용 */

private:
	_bool	m_bRender = true;


private:
	virtual void Free() override;
};

#define  START_QUEST(Type, Key) g_pQuestManager->Start_Quest(Type, Key)
#define	 CLEAR_QUEST(Key)		g_pQuestManager->End_Quest(Key)
//#define  CLEAR_QUEST() 
END
#endif // QuestManager_h__
