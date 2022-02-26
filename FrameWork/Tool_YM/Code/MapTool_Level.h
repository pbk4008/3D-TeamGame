#ifndef MapTool_Level_h__
#define MapTool_Level_h__

#include "Level.h"
BEGIN(Engine)
class CMouse;
END

class CMapTool_Level final :  public CLevel
{
protected:
	explicit CMapTool_Level(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CMapTool_Level(void) = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual _int Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Layer_Plane(const wstring& _pLayerTag);
	HRESULT Ready_Layer_Camera(const wstring& _pLayerTag);
	HRESULT Ready_Later_Mouse(const wstring& _pLayerTag);


public:
	class CMouse* m_pMouse = nullptr;
	class CPlane* m_pPlane = nullptr;
	_int m_iHitIndex = 0;

public:
	static CMapTool_Level* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual void Free(void) override;
};


#endif // MapTool_Level_h__
