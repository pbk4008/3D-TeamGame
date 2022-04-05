#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager;

class CLight final : public CBase
{
private:
	explicit CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLight() = default;

public:
	const LIGHTDESC* Get_LightDesc() const {
		return &m_LightDesc;
	}
public:
	HRESULT NativeConstruct(const LIGHTDESC& LightDesc);
	void	Tick();
	HRESULT Render(CTarget_Manager* pTarget_Manager,const wstring& pCameraTag, _bool PBRHDRcheck,_bool Shadow);
	HRESULT RenderVolumetric(CTarget_Manager* pTarget_Manager, const wstring& pCameraTag);

private:
	HRESULT	Render_Directional(CTarget_Manager* pTarget_Manager, const wstring& pCameraTag, _bool PBRHDRcheck, _bool Shadow,_uint PassIndx);
	HRESULT	Render_PointLight(CTarget_Manager* pTarget_Manager, const wstring& pCameraTag, _bool PBRHDRcheck, _uint PassIndx);
	HRESULT	Ready_PBRLighting(CTarget_Manager* pTarget_Manager, const wstring& pCameraTag,LIGHTDESC::TYPE Type);

public: void UpdateLightCam(_fvector playerpos);

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;

private:
	LIGHTDESC						m_LightDesc;
	class CVIBuffer_RectViewPort*	m_pVIBuffer = nullptr;

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);	
	virtual void Free() override;
};

END