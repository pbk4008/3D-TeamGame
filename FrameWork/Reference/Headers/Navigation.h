#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	explicit CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pDataFilePath);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT SetUp_Neighbor();
	_bool Move_OnNavigation(_fvector vPosition);

#ifdef _DEBUG
public:
	HRESULT Render(_fmatrix WorldMatrix = XMMatrixIdentity());
#endif // _DEBUG

private:
	vector<class CCell*>			m_Cells;
	typedef vector<class CCell*>	CELLS;

private:
	/* 현재 네이베기엿ㄴㄴ을 이용하는 객체가 존재하는 셀의 인덱스 .*/
	_uint				m_iCurrentCellIndex = 0;

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END