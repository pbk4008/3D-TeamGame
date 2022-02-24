#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	explicit CLevel();
	explicit CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);	
	virtual ~CLevel() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual HRESULT Render();
protected:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
public:
	virtual void Free() override;
};

END