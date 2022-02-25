#ifndef Static_h__
#define Static_h__

#include "TooL_Defines.h"
#include "Level.h"

class CStatic_Level final : public CLevel
{
public:
	explicit CStatic_Level(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CStatic_Level(void) = default;

public:
	virtual HRESULT		NativeConstruct(void);
	virtual _int		Tick(_double _dTimeDelta);
	virtual HRESULT		Render(void);

public:
	static CStatic_Level* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual void Free(void) override;
};

#endif //Static_h__
