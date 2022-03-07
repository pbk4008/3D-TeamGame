#ifndef Observer_h__
#define Observer_h__

#include "Base.h"
#include "Tool_Defines.h"

class CObserver final :  public CBase
{
	DECLARE_SINGLETON(CObserver)

public:
	enum MODE { MODE_MAP, MODE_NAV, MODE_END };
	enum ScaleMode { SCALE_X, SCALE_Y, SCALE_Z, SCALE_END };

public:
	_fvector Load_DebugingFile(const wstring& wstrFilePath = L"");

protected:
	CObserver(void);
	virtual ~CObserver(void) = default;

public:
	_float3				m_fPickPos = { 0.0f, 0.0f, 0.0f };
	_float3				m_fModelPos = { 0.0f, 0.0f, 0.0f };
	_float3				m_fModelScale = { 0.0f, 0.0f, 0.0f };
	_float4				m_DebugingPos;
	_bool				m_bPick = false;
	_bool				m_bPlanePick = false;
	_bool				m_bNavSpherePick = false;
public:
	MODE				m_eMode = MODE_END;
	ScaleMode			m_eScaleMode = SCALE_END;

public:
	class CStatic_Mesh* m_pModel = nullptr;
	class CNavSphere*	m_pNavSphere = nullptr;

public:
	virtual void Free(void) override;
};

#endif // Observer_h__
