#ifndef Timer_h__
#define Timer_h__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	CTimer(void);
	virtual ~CTimer(void) = default;

public:
	_double					Get_TimeDelta(void) const { return m_fTimeDelta; }

public:
	HRESULT					Ready_Timer(void);
	void					Update_Timer(void);

private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;

private:
	_double					m_fTimeDelta;

public:
	static CTimer* Create(void);
	virtual void Free(void) override;

};

END
#endif // Timer_h__
