#pragma once

#include "SingleTon.h"

BEGIN(Engine)

class CLight_Manager final : public CSingleTon<CLight_Manager>
{
	friend CSingleTon;
public:
	NO_COPY(CLight_Manager);
	CLight_Manager();
	virtual ~CLight_Manager() = default;
public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex = 0);

public:
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);
	HRESULT Render_Lights();
private:
	list<class CLight*>				m_Lights;
	typedef list<class CLight*>		LIGHTS;

public:	
	virtual void Free();
};

END