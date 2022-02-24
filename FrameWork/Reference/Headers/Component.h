#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{ 
public:
	explicit CComponent() = default;
	explicit CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	void setActive(_bool bActive) { m_bActive = bActive; }
	_bool getActive() { return m_bActive; }
protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
protected:
	_bool		m_isCloned = false;
	_bool		m_bActive = false;
public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END