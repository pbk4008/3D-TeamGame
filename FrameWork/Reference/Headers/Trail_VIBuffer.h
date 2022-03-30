#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTrail_VIBuffer final : public CVIBuffer
{
protected:
	explicit CTrail_VIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CTrail_VIBuffer(const CTrail_VIBuffer& rhs);
	virtual ~CTrail_VIBuffer() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar * pShaderFilePath,_uint MaxVtex);
	virtual HRESULT NativeConstruct(void* pArg) override;

public: void				RenderBuffer(list<pair<_fvector, _fvector>>*pTrailList,_uint passIndx, _uint InterpoleCnt = 3);

public: HRESULT				Add_Vertex(_fvector Startpos, _fvector Endpos);
public: HRESULT				Clear_Vertex();
public: HRESULT				Add_Vertex_CatmullRom(list<pair<_fvector,_fvector>>* pTrailList, _uint InterpoleCnt);

private: _ushort				m_dwUsingVtxcnt = 0;
private: _ushort				m_dwUsingTriCnt = 0;


public:
	static CTrail_VIBuffer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pShaderFilePath,_uint MaxVtex);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END


