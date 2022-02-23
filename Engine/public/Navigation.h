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
	/* ���� ���̺��⿳������ �̿��ϴ� ��ü�� �����ϴ� ���� �ε��� .*/
	_uint				m_iCurrentCellIndex = 0;

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END