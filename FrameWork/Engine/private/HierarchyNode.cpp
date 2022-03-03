#include "..\public\HierarchyNode.h"
#include "Channel.h"

HRESULT CHierarchyNode::NativeConstruct(char* pBoneName, _fmatrix TransformationMatrix, _uint iDepth, CHierarchyNode* pParent)
{
	strcpy_s(m_szBoneName, pBoneName);
	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	m_iDepth = iDepth;

	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	return S_OK;
}

/* ��� ���� TransformationMatrix�� �ִϸ��̼ǿ� �°� ���õ� ���Ŀ� ȣ��ȴ�. */
/* ���� ����� CombineTransformationMatrix�� �����س������� �Լ���. */
void CHierarchyNode::Update_CombinedTransformationMatrix(_uint iAnimationIndex)
{
	if (nullptr != m_Channels[iAnimationIndex])
		XMStoreFloat4x4(&m_TransformationMatrix, m_Channels[iAnimationIndex]->Get_TransformMatrix());

	if (nullptr != m_pParent)	
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, 
			XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));

	else	
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix));	
}

CHierarchyNode * CHierarchyNode::Create(char * pBoneName, _fmatrix TransformationMatrix, _uint iDepth, CHierarchyNode * pParent)
{
	CHierarchyNode* pInstance = new CHierarchyNode();

	if (FAILED(pInstance->NativeConstruct(pBoneName, TransformationMatrix, iDepth, pParent)))
	{
		MSGBOX("Failed to Creating CMeshContainer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyNode::Free()
{
	Safe_Release(m_pParent);

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

}
