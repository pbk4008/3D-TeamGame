#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trapezium final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Trapezium(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_Trapezium(const CVIBuffer_Trapezium& rhs);
	virtual ~CVIBuffer_Trapezium() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	static CVIBuffer_Trapezium* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END