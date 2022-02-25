#include "..\public\PipeLine.h"

CPipeLine::CPipeLine()
{
}

HRESULT CPipeLine::Add_Camera(const wstring& pCameraTag)
{
	if (Find_Camera(pCameraTag))
		return S_OK;

	CAMERA* tCamera = new CAMERA;
	ZeroMemory(tCamera, sizeof(CAMERA));

	m_mapPipeLine.emplace(pCameraTag, tCamera);

	return S_OK;
}

void CPipeLine::Update_PipeLine()
{
	/* 카메라의 위치를 계산해 놓는다ㅏ. */
	for (auto& pair : m_mapPipeLine)
	{
		_matrix CamWorldMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&pair.second->matView));

		XMStoreFloat4(&pair.second->vCamPos, CamWorldMatrix.r[3]);
	}
}

void CPipeLine::Delete_Camera()
{
	for (auto& pair : m_mapPipeLine)
		Safe_Delete(pair.second);
	m_mapPipeLine.clear();
}

_fmatrix CPipeLine::Get_Transform(const wstring& pCameraTag, TRANSFORMSTATEMATRIX eType)
{
	auto tCamera = Find_Camera(pCameraTag);
	if (!tCamera)
		return XMMatrixIdentity();

	_matrix matTransform = XMMatrixIdentity();
	switch (eType)
	{
	case TRANSFORMSTATEMATRIX::D3DTS_VIEW:
		matTransform = XMLoadFloat4x4(&tCamera->matView);
		break;
	case TRANSFORMSTATEMATRIX::D3DTS_PROJECTION:
		matTransform = XMLoadFloat4x4(&tCamera->matProj);
		break;
	}
	return matTransform;
}

_fvector CPipeLine::Get_CamPosition(const wstring& pCameraTag)
{
 	auto tCamera = Find_Camera(pCameraTag);
	if (!tCamera)
		return XMVectorZero();
	return XMLoadFloat4(&tCamera->vCamPos);
}

void CPipeLine::Set_Transform(const wstring& pCameraTag, TRANSFORMSTATEMATRIX eType, _fmatrix TransformMatrix)
{
	auto tCamera = Find_Camera(pCameraTag);
	if (!tCamera)
		return;
	switch (eType)
	{
	case TRANSFORMSTATEMATRIX::D3DTS_VIEW:
		XMStoreFloat4x4(&tCamera->matView, TransformMatrix);
		break;
	case TRANSFORMSTATEMATRIX::D3DTS_PROJECTION:
		XMStoreFloat4x4(&tCamera->matProj, TransformMatrix);
		break;
	}
}

const wstring& CPipeLine::getBaseCamera()
{
	if (m_mapPipeLine.empty())
		return L"";

	auto& iter = m_mapPipeLine.begin();

	return iter->first;
}

CAMERA* CPipeLine::Find_Camera(const wstring& pCameraTag)
{
	auto pCamera = find_if(m_mapPipeLine.begin(), m_mapPipeLine.end(), CTag_Finder(pCameraTag));
	if (pCamera == m_mapPipeLine.end())
		return nullptr;

	return pCamera->second;
}

void CPipeLine::Free()
{
	for (auto& pair : m_mapPipeLine)
		Safe_Delete(pair.second);
	m_mapPipeLine.clear();
}
