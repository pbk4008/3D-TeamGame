#ifndef Observer_h__
#define Observer_h__

#include "Base.h"
#include "Tool_Defines.h"

class CObserver final :  public CBase
{
	DECLARE_SINGLETON(CObserver)

public:
	enum MODE { MODE_MAP, MODE_NAV, MODE_END };

protected:
	CObserver(void);
	virtual ~CObserver(void) = default;

public:
	_float3				m_fPickPos = { 0.0f, 0.0f, 0.0f };
	_float3				m_fModelPos = { 0.0f, 0.0f, 0.0f };
	_float3				m_fModelScale = { 0.0f, 0.0f, 0.0f };
	_bool				m_bPick = false;
	_bool				m_bPlanePick = false;
	_bool				m_bNavSpherePick = false;
public:
	MODE				m_eMode = MODE_END;

public:
	class CStatic_Mesh* m_pModel = nullptr;
	class CNavSphere*	m_pNavSphere = nullptr;

public:
	virtual void Free(void) override;
};

#endif // Observer_h__
