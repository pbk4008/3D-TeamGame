#ifndef Client_Observer_h__
#define Client_Observer_h__

#include "SingleTon.h"

class CClient_Observer final :  public CSingleTon<CClient_Observer>
{
	friend CSingleTon;

protected:
	CClient_Observer(void);
	virtual ~CClient_Observer(void) = default;

public:
	void Set_PlayerWordlMat(_fmatrix _matWorld);
	void Set_PlayerPos(_fvector _vecPos);
	void Set_PlayerAttack(_bool _isAttack);

public:
	_float4x4 m_matWorld;
	_float3	  m_fPos;
	_bool	  m_bAttack = false;

public:
	virtual void Free(void) override;
};

#endif // Client_Observer_h__