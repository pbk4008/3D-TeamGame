#include "..\public\PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{

}

void CPipeLine::Update_PipeLine()
{
	/* ī�޶��� ��ġ�� ����� ���´٤�. */
	_matrix		CamWorldMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrix[D3DTS_VIEW]));

	XMStoreFloat4(&m_vWorldCamPosition, CamWorldMatrix.r[3]);
}

void CPipeLine::Free()
{
}
