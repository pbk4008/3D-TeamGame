#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectInstance final : public CVIBuffer
{
protected:
	explicit CVIBuffer_RectInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_RectInstance(const CVIBuffer_RectInstance& rhs);
	virtual ~CVIBuffer_RectInstance() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, _uint iNumInstance);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render(_uint iPassIndex);

public:
	void Update(_double TimeDelta);

private:
	_uint						m_iNumInstance = 0;

	ID3D11Buffer*				m_pVBInstance = nullptr;
	D3D11_BUFFER_DESC			m_VBInstDesc;
	D3D11_SUBRESOURCE_DATA		m_VBInstSubresourceData;
	_uint						m_iInstStride = 0; /*정점하나의 바이트 크기. */
	_uint						m_iInstNumVertices = 0;


public:
	static CVIBuffer_RectInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END