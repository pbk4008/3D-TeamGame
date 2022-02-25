#ifndef Tab_Changer_h__
#define Tab_Changer_h__

#include "Tool_Defines.h"
#include "Level.h"

class CTab_Changer final : public CLevel
{
public:
	explicit CTab_Changer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CTab_Changer(void) = default;

public:
	virtual HRESULT Ready_Level(TAB_ID _eNextTab);
	virtual _int	Update_Level(_double _dTimeDelta);
	virtual HRESULT Render_Level(void);

private:
	HRESULT			Ready_Layer_BackGround(const _tchar * _pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar * _pLayerTag);

private:
	TAB_ID			m_eNextTAB = TAB_END;
	class CChanger* m_pChanger = nullptr;

private:
	HRESULT			Set_ShowTab(void);

public:
	static CTab_Changer* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, TAB_ID _eNextTab);
	virtual void Free(void) override;
};

#endif // Tab_Changer_h__

