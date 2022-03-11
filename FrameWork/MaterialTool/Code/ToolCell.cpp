#include "pch.h"
#include "ToolCell.h"
#include "ToolPoint.h"

CToolCell::CToolCell()
	:m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{
	ZeroMemory(m_pToolPoints, sizeof(CToolPoint*) * 3);
}

CToolCell::CToolCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	ZeroMemory(m_pToolPoints, sizeof(CToolPoint*) * 3);
}

HRESULT CToolCell::Init_Cell(CToolPoint** pPoints)
{
	for (_uint i = 0; i < 3; i++)
	{
		m_pToolPoints[i] = pPoints[i];
		Safe_AddRef(m_pToolPoints[i]);
	}
	return S_OK;
}

HRESULT CToolCell::Render()
{
	for (auto& pPoint : m_pToolPoints)
		pPoint->Render();

	return S_OK;
}

_fvector CToolCell::getPos(_uint iIndex)
{
	return m_pToolPoints[iIndex]->getPos();
}

CToolCell* CToolCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,CToolPoint** pPoints)
{
	CToolCell* pInstace = new CToolCell(pDevice, pDeviceContext);
	if (FAILED(pInstace->Init_Cell(pPoints)))
	{
		MSG_BOX(L"CToolCell Create Fail");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CToolCell::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	for (auto& pPoint : m_pToolPoints)
		Safe_Release(pPoint);
}
