#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_PointInstance final : public CVIBuffer
{
protected:
	explicit CVIBuffer_PointInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_PointInstance(const CVIBuffer_PointInstance& rhs);
	virtual ~CVIBuffer_PointInstance() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, _uint iNumInstance);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render(_uint iPassIndex);

public:
	void Update(_double TimeDelta);

private:
	_uint			m_iNumInstance = 0;

	ID3D11Buffer*				m_pVBInstance = nullptr;
	D3D11_BUFFER_DESC			m_VBInstDesc;
	D3D11_SUBRESOURCE_DATA		m_VBInstSubresourceData;
	_uint						m_iInstStride = 0; /*정점하나의 바이트 크기. */
	_uint						m_iInstNumVertices = 0;
	_double*					m_pDropSpeed = nullptr;


public:
	static CVIBuffer_PointInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END