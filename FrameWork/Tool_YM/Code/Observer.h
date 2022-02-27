#ifndef Observer_h__
#define Observer_h__

#include "Base.h"
#include "Tool_Defines.h"

class CObserver final :  public CBase
{
	DECLARE_SINGLETON(CObserver)

protected:
	CObserver(void);
	virtual ~CObserver(void) = default;

public:
	_float3		 m_fPickPos = { 0.0f, 0.0f, 0.0f };

public:
	virtual void Free(void) override;
};

#endif // Observer_h__
