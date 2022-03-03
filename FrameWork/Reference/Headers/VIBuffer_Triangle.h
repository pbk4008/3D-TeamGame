#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Triangle final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Triangle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_Triangle(const CVIBuffer_Triangle& rhs);
	virtual ~CVIBuffer_Triangle() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, _float3* pPoints[]);
	virtual HRESULT NativeConstruct_Prototype(const _tchar * pShaderFilePath, _float3 * pPoints);
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	void* getVertices() { return m_pVertices; }
public:
	static CVIBuffer_Triangle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, _float3* pPoints[] = nullptr);
	static CVIBuffer_Triangle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, _float3* pPoints = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END