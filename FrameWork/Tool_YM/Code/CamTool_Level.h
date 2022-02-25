#ifndef CamTool_Level_h__
#define CamTool_Level_h__

#include "Level.h"

class CCamTool_Level final : public CLevel
{
protected:
	explicit CCamTool_Level(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CCamTool_Level(void) = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual _int Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CCamTool_Level* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext);
	virtual void Free(void) override;
};

#endif // CamTool_Level_h__


