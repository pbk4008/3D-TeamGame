#ifndef TimerMgr_h__
#define TimerMgr_h__

#include "SingleTon.h"

BEGIN(Engine)

class CTimer_Manager : public CSingleTon<CTimer_Manager>
{
	friend CSingleTon;
private:
	NO_COPY(CTimer_Manager);
	explicit CTimer_Manager(void);
	virtual ~CTimer_Manager(void) = default;

public:
	_double					Get_TimeDelta(const wstring& pTimerTag);
	void					Update_TimeDelta(const wstring& pTimerTag);

public:
	HRESULT		Ready_Timer(const wstring& pTimerTag);

private:
	class CTimer*		Find_Timer(const wstring& pTimerTag);

private:
	map<wstring, class CTimer*>			m_mapTimer;

public:
	virtual void		Free(void);
};

END
#endif // TimerMgr_h__
