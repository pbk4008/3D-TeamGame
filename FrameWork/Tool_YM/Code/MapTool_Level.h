#ifndef MapTool_Level_h__
#define MapTool_Level_h__

#include "Level.h"

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

public:
	static CMapTool_Level* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual void Free(void) override;
};


#endif // MapTool_Level_h__
