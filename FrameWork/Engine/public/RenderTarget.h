#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTarget final : public CBase
{
public: enum class RTT // Render Target Type
{
	SHADOWMAP, SHADOWDEPTH, DEFFURED, LIGHTING, RTT_END
};

private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderTarget() = default;
public:
	ID3D11RenderTargetView* Get_RTV() {
		return m_pRTV;
	}
	ID3D11ShaderResourceView* Get_STV() {
		return m_pSRV;
	}
public:
	HRESULT NativeConstruct(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, RTT eType);
	HRESULT Clear();

#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer();
#endif // _DEBUG

private: HRESULT			CreateDSV(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	ID3D11Texture2D* m_pTexture = nullptr;
	ID3D11Texture2D* m_pDepthStencilTex = nullptr;
	ID3D11RenderTargetView* m_pRTV = nullptr;
	ID3D11ShaderResourceView* m_pSRV = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	_float4						m_vClearColor;

private: RTT					m_eType = RTT::RTT_END;

#ifdef _DEBUG
private:
	class CVIBuffer_RectViewPort* m_pDebugBuffer = nullptr;
#endif // _DEBUG

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, RTT eType);
	virtual void Free() override;
};

END