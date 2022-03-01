#pragma once

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	explicit CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCell() = default;
public:
	_fvector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoint[ePoint]);
	}

	_uint Get_Index() const {
		return m_iIndex;
	}

	void Set_Neighbor(LINE eLine, CCell* pNeighbor) {
		m_pNeighbor[eLine] = pNeighbor;		
	}
public:
	HRESULT NativeConstruct(_float3* pPoints, _uint iIndex);
public:
	_bool Compare(_fvector SourPoint, _fvector DestPoint);
	_bool isIn(_fvector vPosition, CCell** ppOutNeighbor);

#ifdef _DEBUG
public:
	HRESULT Ready_DebugBuffer();
	HRESULT Render(_fmatrix WorldMatrix, _uint iCurrentIndex, const wstring& pCameraTag);
#endif // _DEBUG

public:
	_float3					m_vPoint[POINT_END];
	_uint					m_iIndex = 0;

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	_float3					m_vLine[LINE_END];
	_float3					m_vNormal[LINE_END];

	CCell*					m_pNeighbor[LINE_END];



#ifdef _DEBUG
public:
	class CVIBuffer_Triangle*			m_pVIBuffer = nullptr;
#endif // _DEBUG



public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END