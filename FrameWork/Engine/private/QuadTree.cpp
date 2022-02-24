#include "..\public\QuadTree.h"
#include "Frustum.h"
#include "PipeLine.h"

CQuadTree::CQuadTree()
{

}

HRESULT CQuadTree::NativeConstruct(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iCorners[CORNER_LT] = iNumVerticesX * iNumVerticesZ - iNumVerticesX;
	m_iCorners[CORNER_RT] = iNumVerticesX * iNumVerticesZ - 1;
	m_iCorners[CORNER_RB] = iNumVerticesX - 1;
	m_iCorners[CORNER_LB] = 0;

	m_iCenter = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RB]) >> 1;

	_uint		iTC, iRC, iBC, iLC;

	iTC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RT]) >> 1;
	iRC = (m_iCorners[CORNER_RT] + m_iCorners[CORNER_RB]) >> 1;
	iBC = (m_iCorners[CORNER_LB] + m_iCorners[CORNER_RB]) >> 1;
	iLC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_LB]) >> 1;
	
	m_pChilds[CHILD_LT] = CQuadTree::Create(m_iCorners[CORNER_LT], iTC, m_iCenter, iLC);
	m_pChilds[CHILD_RT] = CQuadTree::Create(iTC, m_iCorners[CORNER_RT], iRC, m_iCenter);
	m_pChilds[CHILD_RB] = CQuadTree::Create(m_iCenter, iRC, m_iCorners[CORNER_RB], iBC);
	m_pChilds[CHILD_LB] = CQuadTree::Create(iLC, m_iCenter, iBC, m_iCorners[CORNER_LB]);
	
	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuadTree::NativeConstruct(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	m_iCorners[CORNER_LT] = iLT;
	m_iCorners[CORNER_RT] = iRT;
	m_iCorners[CORNER_RB] = iRB;
	m_iCorners[CORNER_LB] = iLB;

	m_iCenter = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RB]) >> 1;

	if (m_iCorners[CORNER_RT] - m_iCorners[CORNER_LT] == 1)
		return S_OK;

	_uint		iTC, iRC, iBC, iLC;

	iTC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RT]) >> 1;
	iRC = (m_iCorners[CORNER_RT] + m_iCorners[CORNER_RB]) >> 1;
	iBC = (m_iCorners[CORNER_LB] + m_iCorners[CORNER_RB]) >> 1;
	iLC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_LB]) >> 1;


	m_pChilds[CHILD_LT] = CQuadTree::Create(m_iCorners[CORNER_LT], iTC, m_iCenter, iLC);
	m_pChilds[CHILD_RT] = CQuadTree::Create(iTC, m_iCorners[CORNER_RT], iRC, m_iCenter);
	m_pChilds[CHILD_RB] = CQuadTree::Create(m_iCenter, iRC, m_iCorners[CORNER_RB], iBC);
	m_pChilds[CHILD_LB] = CQuadTree::Create(iLC, m_iCenter, iBC, m_iCorners[CORNER_LB]);

	return S_OK;
}

HRESULT CQuadTree::Culling(CFrustum* pFrustum, VTXNORTEX* pVertices, FACEINDICES32* pIndices, _uint* pNumFaces, _fmatrix WorldMatrixInverse, const wstring& pCameraTag)
{
	if (nullptr == m_pChilds[CHILD_LT] || 
		true == isLOD(pCameraTag,pVertices, WorldMatrixInverse))
	{

		_bool		isDraw[NEIGHBOR_END] = { true, true, true, true };

		for (_uint i = 0; i < NEIGHBOR_END; ++i)
		{
			if (nullptr != m_pNeighbor[i])
				isDraw[i] = m_pNeighbor[i]->isLOD(pCameraTag,pVertices, WorldMatrixInverse);
		}

		/* 현재 쿼드트리를 구성하는 네개 코너 인덱스가 절두체와 충돌했느지 확인하다. */
		/* 그려야할 삼각형들을 인덱스로서 추가해준다. */

		/* 쿼드트리를 구성하고있는(가장작은) 코너네개가 절두체 안에 있나없나? */
		_bool		isIn[CORNER_END] = {
			pFrustum->isInLocal(XMLoadFloat3(&pVertices[m_iCorners[CORNER_LT]].vPosition)), 
			pFrustum->isInLocal(XMLoadFloat3(&pVertices[m_iCorners[CORNER_RT]].vPosition)),
			pFrustum->isInLocal(XMLoadFloat3(&pVertices[m_iCorners[CORNER_RB]].vPosition)),
			pFrustum->isInLocal(XMLoadFloat3(&pVertices[m_iCorners[CORNER_LB]].vPosition))
		};

		if (true == isDraw[NEIGHBOR_LEFT] &&
			true == isDraw[NEIGHBOR_TOP] &&
			true == isDraw[NEIGHBOR_RIGHT] &&
			true == isDraw[NEIGHBOR_BOTTOM])
		{
			/* 오른쪽 위삼각형ㅇ르 그려야하냐? */
			if (true == isIn[CORNER_LT] ||
				true == isIn[CORNER_RT] ||
				true == isIn[CORNER_RB])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCorners[CORNER_RT];
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_RB];
				++(*pNumFaces);
			}

			/* 왼쪽 하단 삼각형ㅇ르 그려야하냐? */
			if (true == isIn[CORNER_LT] ||
				true == isIn[CORNER_RB] ||
				true == isIn[CORNER_LB])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCorners[CORNER_RB];
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_LB];
				++(*pNumFaces);
			}

			return S_OK;
		}

		_uint		iTC, iRC, iBC, iLC;

		iTC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RT]) >> 1;
		iRC = (m_iCorners[CORNER_RT] + m_iCorners[CORNER_RB]) >> 1;
		iBC = (m_iCorners[CORNER_LB] + m_iCorners[CORNER_RB]) >> 1;
		iLC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_LB]) >> 1;

		if (true == isIn[CORNER_LT] ||
			true == isIn[CORNER_RB] ||
			true == isIn[CORNER_LB])
		{
			if (false == isDraw[NEIGHBOR_LEFT])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = iLC;
				++(*pNumFaces);

				pIndices[*pNumFaces]._0 = iLC;
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_LB];
				++(*pNumFaces);
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_LB];
				++(*pNumFaces);
			}
			if (false == isDraw[NEIGHBOR_BOTTOM])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LB];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = iBC;
				++(*pNumFaces);

				pIndices[*pNumFaces]._0 = iBC;
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_RB];
				++(*pNumFaces);
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LB];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_RB];
				++(*pNumFaces);
			}

		}
		if (true == isIn[CORNER_LT] ||
			true == isIn[CORNER_RT] ||
			true == isIn[CORNER_RB])
		{
			if (false == isDraw[NEIGHBOR_TOP])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = iTC;
				pIndices[*pNumFaces]._2 = m_iCenter;
				++(*pNumFaces);

				pIndices[*pNumFaces]._0 = m_iCenter;
				pIndices[*pNumFaces]._1 = iTC;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_RT];
				++(*pNumFaces);
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCorners[CORNER_RT];
				pIndices[*pNumFaces]._2 = m_iCenter;
				++(*pNumFaces);

			}

			if (false == isDraw[NEIGHBOR_RIGHT])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_RT];
				pIndices[*pNumFaces]._1 = iRC;
				pIndices[*pNumFaces]._2 = m_iCenter;
				++(*pNumFaces);

				pIndices[*pNumFaces]._0 = m_iCenter;
				pIndices[*pNumFaces]._1 = iRC;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_RB];
				++(*pNumFaces);
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_RT];
				pIndices[*pNumFaces]._1 = m_iCorners[CORNER_RB];
				pIndices[*pNumFaces]._2 = m_iCenter;
				++(*pNumFaces);
			}
		}		
		
		return S_OK;
	}


	/* 절두체와 쿼드트리가 충돌했다면? */
	_float		fRadius = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVertices[m_iCorners[CORNER_LT]].vPosition) - XMLoadFloat3(&pVertices[m_iCenter].vPosition)));
	
	if (true == pFrustum->isInLocal(XMLoadFloat3(&pVertices[m_iCenter].vPosition), fRadius))
	{
		for (_uint i = 0; i < CHILD_END; ++i)
		{
			m_pChilds[i]->Culling(pFrustum, pVertices, pIndices, pNumFaces, WorldMatrixInverse, pCameraTag);
		}
	}


	return S_OK;
}

_bool CQuadTree::isLOD(const wstring& pCamerTag, VTXNORTEX* pVertices, _fmatrix WorldMatrixInverse)
{
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_vector		vCamPosition = pPipeLine->Get_CamPosition(pCamerTag);
	vCamPosition = XMVector4Transform(vCamPosition, WorldMatrixInverse);
	_float		fDistance = XMVectorGetX(XMVector3Length(vCamPosition - XMLoadFloat3(&pVertices[m_iCenter].vPosition)));
	_float		fWidth = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVertices[m_iCorners[CORNER_RT]].vPosition) - XMLoadFloat3(&pVertices[m_iCorners[CORNER_LT]].vPosition)));

	RELEASE_INSTANCE(CPipeLine);

	if (fDistance * 0.1f > fWidth)
		return true;	

	return false;
}

HRESULT CQuadTree::SetUp_Neighbor()
{
	if (nullptr == m_pChilds[CHILD_LT])
		return S_OK;

	m_pChilds[CHILD_LT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChilds[CHILD_RT];
	m_pChilds[CHILD_LT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChilds[CHILD_LB];

	m_pChilds[CHILD_RT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChilds[CHILD_LT];
	m_pChilds[CHILD_RT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChilds[CHILD_RB];

	m_pChilds[CHILD_RB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChilds[CHILD_LB];
	m_pChilds[CHILD_RB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChilds[CHILD_RT];

	m_pChilds[CHILD_LB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChilds[CHILD_RB];
	m_pChilds[CHILD_LB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChilds[CHILD_LT];

	if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
	{
		m_pChilds[CHILD_LT]->m_pNeighbor[NEIGHBOR_LEFT] =
			m_pNeighbor[NEIGHBOR_LEFT]->m_pChilds[CHILD_RT];
		m_pChilds[CHILD_LB]->m_pNeighbor[NEIGHBOR_LEFT] =
			m_pNeighbor[NEIGHBOR_LEFT]->m_pChilds[CHILD_RB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
	{
		m_pChilds[CHILD_RT]->m_pNeighbor[NEIGHBOR_RIGHT] =
			m_pNeighbor[NEIGHBOR_RIGHT]->m_pChilds[CHILD_LT];
		m_pChilds[CHILD_RB]->m_pNeighbor[NEIGHBOR_RIGHT] =
			m_pNeighbor[NEIGHBOR_RIGHT]->m_pChilds[CHILD_LB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
	{
		m_pChilds[CHILD_LB]->m_pNeighbor[NEIGHBOR_BOTTOM] =
			m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChilds[CHILD_LT];
		m_pChilds[CHILD_RB]->m_pNeighbor[NEIGHBOR_BOTTOM] =
			m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChilds[CHILD_RT];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
	{
		m_pChilds[CHILD_LT]->m_pNeighbor[NEIGHBOR_TOP] =
			m_pNeighbor[NEIGHBOR_TOP]->m_pChilds[CHILD_LB];
		m_pChilds[CHILD_RT]->m_pNeighbor[NEIGHBOR_TOP] =
			m_pNeighbor[NEIGHBOR_TOP]->m_pChilds[CHILD_RB];
	}

	for (auto& pChild : m_pChilds)
		pChild->SetUp_Neighbor();

	return S_OK;
}

CQuadTree * CQuadTree::Create(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	CQuadTree*		pInstance = new CQuadTree();

	if (FAILED(pInstance->NativeConstruct(iNumVerticesX, iNumVerticesZ)))
	{
		MSGBOX("Failed to Creating CQuadTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CQuadTree * CQuadTree::Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	CQuadTree*		pInstance = new CQuadTree();

	if (FAILED(pInstance->NativeConstruct(iLT, iRT, iRB, iLB)))
	{
		MSGBOX("Failed to Creating CQuadTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuadTree::Free()
{
	for (auto& pQuadTree : m_pChilds)
		Safe_Release(pQuadTree);
}
