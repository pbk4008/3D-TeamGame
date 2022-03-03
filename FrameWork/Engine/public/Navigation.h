#pragma once

#include "Component.h"
#include "Cell.h"
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

public:
	HRESULT					Update_Buffer(_fvector pPosition);
	vector<class CCell*>	Get_vecCells(void) { return m_Cells; }

#ifdef _DEBUG
public:
	HRESULT Render(const wstring& pCameraTag, _fmatrix WorldMatrix = XMMatrixIdentity());
#endif // _DEBUG

public:
	vector<class CCell*>			m_Cells;
	typedef vector<class CCell*>	CELLS;
private:
	_uint				m_iCurrentCellIndex = 0;
	_int				m_iChangePointIndex = -1;

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pDataFilePath = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END