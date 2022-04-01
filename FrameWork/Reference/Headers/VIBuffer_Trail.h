#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail : public CVIBuffer
{
private:
	explicit CVIBuffer_Trail(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CVIBuffer_Trail(const CVIBuffer_Trail& _rhs);
	virtual ~CVIBuffer_Trail() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* _pShaderFilePath, const _uint _iMaxVertices);
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	HRESULT Render_Curve(const _uint _iPassIndex = 0);

public:
	HRESULT Set_VertexTrail(const _float3 _arrPos[], const _float3 _arrDir[], const _float _fLength, const DWORD _dwCount);

public:
	static CVIBuffer_Trail* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, const _tchar* _pShaderFilePath, const _uint _iMaxVertices);
	virtual CComponent* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END