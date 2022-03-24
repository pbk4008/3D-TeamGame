#include "..\public\Navigation.h"
#include "Cell.h"
#include "VIBuffer_Triangle.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar* pDataFilePath)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;	

	if (nullptr == pDataFilePath)
		return S_OK;

	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	while (true)
	{
		_float3		vPoints[3];

		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, (_uint)m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
			}

			if (true == pDestCell->Compare(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
			}

			if (true == pDestCell->Compare(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

_bool CNavigation::Move_OnNavigation(_fvector vPosition)
{
	/* 1. 현재 객체가 움직일 수 있는지에 대한 판단 */		
	/* 2. 다른 쎌로 이동했다라면 현재 m_iCurrentCellIndex를 갱신. */

	CCell*		pNeighbor = nullptr;
	
	/* 현재 존재하던 셀안에서 움직였다. */
	if (true == m_Cells[m_iCurrentCellIndex]->isIn(vPosition, &pNeighbor))
	{
		return true;
	}

	/* 현재 존재하던 셀밖으로 움직였다. */
	else
	{
		/*  움직일수있는 경우. 이웃셀이 존재했다. */
		if (nullptr != pNeighbor)
		{
			while(true)
			{ 
				if (true == pNeighbor->isIn(vPosition, &pNeighbor))
				{		
					m_iCurrentCellIndex = pNeighbor->Get_Index();
					return true;
				}	
				int a = 10;
			}			
		}
		/*  움직일수없는 경우. */
		else
			return false;
	}
}
#ifdef _DEBUG
HRESULT CNavigation::Update_Buffer(_fvector pPosition)
{
	class CCell* pCell = nullptr;

	for (auto Finder : m_Cells)
	{
		for (int i = 0; i < CCell::POINT_END; ++i)
		{
			if (true == XMVector3Equal(pPosition, XMLoadFloat3(&(*Finder->m_pPoint[i]))))
			{
				Finder->m_vPoint[i] = *(Finder->m_pPoint[i]);
				pCell = Finder;
			}

			if (nullptr != pCell)
			{
				D3D11_MAPPED_SUBRESOURCE resource;
				m_pDeviceContext->Map(pCell->m_pVIBuffer->m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

				for (int j = 0; j < CCell::POINT_END; ++j)
					((VTXCOL*)resource.pData)[j].vPosition = *pCell->m_pPoint[j];

				m_pDeviceContext->Unmap((pCell)->m_pVIBuffer->m_pVB, 0);
			}
		}
	}
	return S_OK;
}
#endif // _DEBUG

/*RESULT CNavigation::Find_Cell(_fvector _vFindPoint, _fvector _vUpdatePos)
{
	for (auto Finder : m_Cells)
	{
		for (int i = 0; i <= CCell::POINT_END; ++i)
		{
			if (true == XMVector3Equal(_vFindPoint, XMLoadFloat3(&Finder->m_vPoint[i])))
			{
				m_pCell = Finder;
				m_iChangePointIndex = i;
				XMStoreFloat3(&Finder->m_vPoint[i], _vUpdatePos);
				break;
			}
			else
				m_iChangePointIndex = -1;
		}
		if(-1 != m_iChangePointIndex)
			break;
	}
	return Update_Point(_vUpdatePos);
}*/

#ifdef _DEBUG
HRESULT CNavigation::Render(const wstring& pCameraTag, _fmatrix WorldMatrix)
{
	for (auto& pCell : m_Cells)
	{
		pCell->Render(WorldMatrix, m_iCurrentCellIndex, pCameraTag);
	}
	return S_OK;
}
#endif // _DEBUG

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pDataFilePath /*= nullptr*/)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pDataFilePath)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*		pInstance = new CNavigation(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();
}
