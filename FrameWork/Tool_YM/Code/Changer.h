#ifndef Changer_h__
#define Changer_h__

#include "Engine_Defines.h"
#include "Tool_Defines.h"
#include "Base.h"

class CChanger final : public CBase
{
private:
	explicit CChanger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CChanger(void) = default;

public:
	HRESULT Ready_Changer(TAB_ID _eNextTab);

public:
	HRESULT Loading_For_Static(void);
	HRESULT Loading_ForMap_Tool(void);
	HRESULT Loading_ForCam_Tool(void);
	
public:
	CRITICAL_SECTION& Get_CriticalSection() { return m_CS; }

public:
	TAB_ID Get_NextTabID() const { return m_eNextTab;}
	_bool  Get_Finished() const  { return m_isFinished;}

public:
	ID3D11Device*		   m_pDevice = nullptr;
	ID3D11DeviceContext*   m_pDeviceContext = nullptr;

public:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_CS;

public:
	_tchar				m_szLoading[MAX_PATH] = TEXT("");
	_bool				m_isFinished = false;

public:
	TAB_ID				m_eNextTab = TAB_END;

public:
	static CChanger* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, TAB_ID _eNextTab);
	virtual void Free(void) override;
};

#endif // Changer_h__