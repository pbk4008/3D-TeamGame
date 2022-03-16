#ifndef Client_Observer_h__
#define Client_Observer_h__

#include "Base.h"

class CClient_Observer final :  public CBase
{
	DECLARE_SINGLETON(CClient_Observer)

protected:
	CClient_Observer(void);
	virtual ~CClient_Observer(void) = default;


public:
	virtual void Free(void) override;
};


#endif // Client_Observer_h__