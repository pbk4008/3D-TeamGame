#pragma once

#include "Base.h"

BEGIN(Engine)

class CQuadTree final : public CBase
{
public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_RB, CHILD_LB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END };
	enum NEIGHBOR {	NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END};
private:
	CQuadTree();
	virtual ~CQuadTree() = default;
public:
	HRESULT NativeConstruct(_uint iNumVerticesX, _uint iNumVerticesZ);
	HRESULT NativeConstruct(_uint iLT, _uint iRT, _uint iRB, _uint iLB);

public:
	HRESULT Culling(class CFrustum* pFrustum, VTXNORTEX* pVertices, FACEINDICES32* pIndices, _uint* pNumFaces, _fmatrix WorldMatrixInverse);
	_bool isLOD(VTXNORTEX* pVertices, _fmatrix WorldMatrixInverse);

private:
	CQuadTree*			m_pChilds[CHILD_END] = { nullptr };
	CQuadTree*			m_pNeighbor[NEIGHBOR_END] = { nullptr };
	_uint				m_iCorners[CORNER_END] = { 0 };
	_uint				m_iCenter = 0;
	_float				m_fRadius = 0.f;

private:
	HRESULT SetUp_Neighbor();

public:
	static CQuadTree* Create(_uint iNumVerticesX, _uint iNumVerticesZ);
	static CQuadTree* Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	virtual void Free() override;
};

END