#pragma once
#ifndef __TOOLCELL_H__
#define __TOOLCELL_H__
#include "Base.h"
BEGIN(Tool)
class CToolPoint;
class CToolCell final : public CBase 
{
private:
	explicit CToolCell();
	explicit CToolCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CToolCell() = default;
public:
	HRESULT Init_Cell(CToolPoint** pPoints);
	HRESULT Render();
	_fvector getPos(_uint iIndex);
public:
	static CToolCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolPoint** pPoints);
private:
	virtual void Free();
public:
	
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	CToolPoint* m_pToolPoints[3];
};
END
#endif