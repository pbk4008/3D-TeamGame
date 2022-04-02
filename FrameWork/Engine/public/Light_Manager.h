#pragma once

#include "SingleTon.h"

BEGIN(Engine)

class CTarget_Manager;

class CLight_Manager final : public CSingleTon<CLight_Manager>
{
	friend CSingleTon;
public:
	NO_COPY(CLight_Manager);
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public: void UpdateLightCam(_uint LightIndx, _fvector playerpos);

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex = 0);

public: HRESULT	CreateLightCam(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,LIGHTDESC& desc);

public:
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);
	HRESULT Render_Lights(CTarget_Manager* pTarget_Manager, const wstring& pCameraTag, _bool pbr, _bool shadow);
private:
	vector<class CLight*>				m_Lights;
	typedef vector<class CLight*>		LIGHTS;

public:	
	virtual void Free();
};

END