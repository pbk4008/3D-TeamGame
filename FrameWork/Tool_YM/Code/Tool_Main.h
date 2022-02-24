#ifndef Tool_Main_h__
#define Tool_Main_h__

#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

class CMenu_Form;
class CInspector_Form;
class CToolYMView;
class CMainFrame;
class CTool_Main : public CBase
{
public:
	explicit CTool_Main(void);
	virtual ~CTool_Main(void) = default;

public:
	HRESULT Tool_Main_Ready(void);
	_int	Tool_Main_Update(const _double _dTimeDelta);
	HRESULT Tool_Main_Render(void);

public:
	HRESULT Ready_Component_Prototype(void);
	HRESULT Ready_GameObject_Prototype(void);
	//HRESULT Set_Start_Tab(CMenu_Form::TAB_ID _eTab);

public:
	CMainFrame*		 m_pMainFrm = nullptr;
	CToolYMView*	 m_pToolView = nullptr;
	CMenu_Form*		 m_pMenu_Form = nullptr;
	CInspector_Form* m_pInspec_Form = nullptr;

public:
	CGameInstance* m_pGameInst = nullptr;
	CRenderer*	   m_pRenderer = nullptr;

public:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	virtual void Free() override;

};


#endif // Tool_Main_h__


