#pragma once

#include "SingleTon.h"

BEGIN(Engine)

class CGraphic_Device final : public CSingleTon<CGraphic_Device>
{
	friend CSingleTon;
public:
	enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };
public:
	NO_COPY(CGraphic_Device);
	explicit CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	HRESULT Ready_Graphic_Device(HWND hWnd, WINMODE WinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT Clear_BackBuffer_View(XMFLOAT4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public:
	ID3D11Device*		 Get_Device(void) { return m_pDevice; }
	ID3D11DeviceContext* Get_DeviceContext(void) { return m_pDeviceContext; }

private:
	/* 메모리 할당. (정점버퍼, 인덱스버퍼, 텍스쳐로드) */
	ID3D11Device*			m_pDevice = nullptr;

	/* 바인딩작업. (정점버퍼를 SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	IDXGISwapChain*			m_pSwapChain = nullptr;	

	ID3D11RenderTargetView*		m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;

private:
	HRESULT Ready_SwapChain(HWND hWnd, WINMODE WinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
	virtual void Free() override;
};

END