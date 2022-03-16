#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectViewPort final : public CVIBuffer
{
protected:
	explicit CVIBuffer_RectViewPort(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_RectViewPort(const CVIBuffer_RectViewPort& rhs);
	virtual ~CVIBuffer_RectViewPort() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(_float fX, _float fY, _float fSizeX, _float fSizeY, const _tchar* pShaderFilePath);
	virtual HRESULT NativeConstruct(void* pArg) override;
	HRESULT			Buffer_Resize(_float sizex, _float sizey);

private: D3D11_VIEWPORT				m_viewport;

public:
	static CVIBuffer_RectViewPort* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _float fX, _float fY, _float fSizeX, _float fSizeY, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END