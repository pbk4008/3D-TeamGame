#pragma once

#include "Base.h"

BEGIN(Engine)
class CCSTarget final : public CBase
{
public: enum CSType
{
	DOWNSAMPLE, GSBLUR, CSTYPE_END
};

private: CCSTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CCSTarget() = default;

public:	ID3D11RenderTargetView* Get_RTV() { return m_CSGroup.CSRTV; }
public: ID3D11ShaderResourceView* Get_SRV() { return m_CSGroup.CSSRV; }
public: ID3D11UnorderedAccessView* Get_UAV() { return m_CSGroup.CSUAV; }

public: HRESULT	Compile_CSShaderFile(const _tchar * pShaderFilePath, const char* pEntryPoitn);
public: HRESULT	Create_CSBuffer(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CSType etype);
public: HRESULT	RunCSDownSample(ID3D11ShaderResourceView * ppShaderResourceView);
public: HRESULT	RunCSBlur(ID3D11ShaderResourceView * pDownSample, ID3D11UnorderedAccessView * pDownUAV);

public:	HRESULT NativeConstruct(const _tchar * pShaderFilePath, const char* pEntryPoitn, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CSType eType);
public: HRESULT Clear();

#ifdef _DEBUG

public:	HRESULT Ready_Debug_Buffer(_float fX, _float fY, _float fSizeX, _float fSizeY);
public: HRESULT Render_Debug_Buffer();

#endif // _DEBUG


private: ID3D11Device*				m_pDevice = nullptr;
private: ID3D11DeviceContext*		m_pDeviceContext = nullptr;
private: CSRT						m_CSGroup;
private: ID3D11Buffer*				m_pConstantBuffer = nullptr;
private: ID3D11Buffer*				m_pBlurBuffer = nullptr;
private: THRESHOLD					m_Threshold;
private: BLURPARAM					m_BlurParam;
private: CS							m_pCS;
private: _float4					m_vClearColor = { 0,0,0,0 };


#ifdef _DEBUG
private: class CVIBuffer_RectViewPort* m_pDebugBuffer = nullptr;
#endif // _DEBUG

public: static CCSTarget*	Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pShaderFilePath, const char* pEntryPoitn, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CSType etype);
public: virtual void		Free() override;
};
END