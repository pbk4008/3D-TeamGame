#include "..\public\Frustum.h"
#include "PipeLine.h"


CFrustum::CFrustum()
{

}

HRESULT CFrustum::Ready_FrustumInProjSpace()
{
	m_vPoint[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _float3(1.f, 1.f, 0.f);
	m_vPoint[2] = _float3(1.f, -1.f, 0.f);
	m_vPoint[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _float3(1.f, 1.f, 1.f);
	m_vPoint[6] = _float3(1.f, -1.f, 1.f);
	m_vPoint[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

HRESULT CFrustum::Transform_ToWorldSpace(const wstring& pCameraTag)
{
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix			ProjMatrix = pPipeLine->Get_Transform(pCameraTag,TRANSFORMSTATEMATRIX::D3DTS_PROJECTION);
	ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);

	_matrix			ViewMatrix = pPipeLine->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	_vector		vPoint[8];

	for (_uint i = 0; i < 8; ++i)
	{
		/* 뷰스페이스로 옮긴다. */
		vPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPoint[i]), ProjMatrix);

		/* 월드스페이스로 옮긴다. */
		vPoint[i] = XMVector3TransformCoord(vPoint[i], ViewMatrix);

		vPoint[i] = XMVectorSetW(vPoint[i], 1.f);

		XMStoreFloat3(&m_vPointInWorld[i], vPoint[i]);
	}

	Make_PlaneInWorld(vPoint);

	RELEASE_INSTANCE(CPipeLine);
	
	return S_OK;
}

HRESULT CFrustum::Transform_ToLocalSpace(_fmatrix WorldMatrixInverse)
{
	_vector		vPoint[8];

	for (_uint i = 0; i < 8; ++i)
		vPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPointInWorld[i]), WorldMatrixInverse);

	Make_PlaneInLocal(vPoint);

	return S_OK;
}

_bool CFrustum::isInWorld(_fvector vPosition, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_PlaneInWorld[i]), vPosition)))
			return false;
	}
	return true;	
}

_bool CFrustum::isInWorld(_float4* vPoints, _float fRange)
{
	for (_uint i = 0; i < 8; i++)
	{
		_bool bCheck = true;
		_vector vPoint = XMLoadFloat4(&vPoints[i]);
		for (_uint j = 0; j < 6; j++)
		{
			_vector vPlane = XMLoadFloat4(&m_PlaneInWorld[j]);
			_float fDot = XMVectorGetX(XMPlaneDotCoord(vPlane, vPoint));
			if (fRange < fDot)
				bCheck = false;
		}
		if (bCheck)
			return true;
	}
	return false;
}

_bool CFrustum::isInLocal(_fvector vPosition, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_PlaneInLocal[i]), vPosition)))
			return false;
	}
	return true;
}

void CFrustum::Make_PlaneInWorld(_fvector* pPoints)
{
	XMStoreFloat4(&m_PlaneInWorld[0], XMPlaneFromPoints(pPoints[1], pPoints[5], pPoints[6]));
	XMStoreFloat4(&m_PlaneInWorld[1], XMPlaneFromPoints(pPoints[4], pPoints[0], pPoints[3]));

	XMStoreFloat4(&m_PlaneInWorld[2], XMPlaneFromPoints(pPoints[4], pPoints[5], pPoints[1]));
	XMStoreFloat4(&m_PlaneInWorld[3], XMPlaneFromPoints(pPoints[3], pPoints[2], pPoints[6]));

	XMStoreFloat4(&m_PlaneInWorld[4], XMPlaneFromPoints(pPoints[5], pPoints[4], pPoints[7]));
	XMStoreFloat4(&m_PlaneInWorld[5], XMPlaneFromPoints(pPoints[0], pPoints[1], pPoints[2]));
}

void CFrustum::Make_PlaneInLocal(_fvector* pPoints)
{
	XMStoreFloat4(&m_PlaneInLocal[0], XMPlaneFromPoints(pPoints[1], pPoints[5], pPoints[6]));
	XMStoreFloat4(&m_PlaneInLocal[1], XMPlaneFromPoints(pPoints[4], pPoints[0], pPoints[3]));

	XMStoreFloat4(&m_PlaneInLocal[2], XMPlaneFromPoints(pPoints[4], pPoints[5], pPoints[1]));
	XMStoreFloat4(&m_PlaneInLocal[3], XMPlaneFromPoints(pPoints[3], pPoints[2], pPoints[6]));

	XMStoreFloat4(&m_PlaneInLocal[4], XMPlaneFromPoints(pPoints[5], pPoints[4], pPoints[7]));
	XMStoreFloat4(&m_PlaneInLocal[5], XMPlaneFromPoints(pPoints[0], pPoints[1], pPoints[2]));
}

void CFrustum::Free()
{
}
