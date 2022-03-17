#include "..\public\HierarchyNode.h"
#include "Channel.h"

HRESULT CHierarchyNode::NativeConstruct(char* pBoneName, _fmatrix TransformationMatrix, _uint iDepth, CHierarchyNode* pParent)
{
	strcpy_s(m_szBoneName, pBoneName);
	XMStoreFloat4x4(&m_CreateMatrix, TransformationMatrix);
	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	m_iDepth = iDepth;

	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	return S_OK;
}

HRESULT CHierarchyNode::NativeConstruct(char* pBoneName, _fmatrix OffsetMatrix, _fmatrix TransformationMatrix, _uint iDepth, CHierarchyNode* pParent)
{
	strcpy_s(m_szBoneName, pBoneName);
	XMStoreFloat4x4(&m_CreateMatrix, TransformationMatrix);
	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	m_iDepth = iDepth;

	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	return S_OK;
}

/* 모든 뼈의 TransformationMatrix가 애니메이션에 맞게 셋팅된 이후에 호출된다. */
/* 현재 노드의 CombineTransformationMatrix를 셋팅해놓기위한 함수다. */
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

void CHierarchyNode::Update_CombinedTransformationMatrix(const _uint _iAnimIndex, const _bool _isRootMotion, const ERootOption _eRootOption)
{
	if (nullptr != m_Channels[_iAnimIndex])
	{
		if (_isRootMotion && !strcmp(m_szBoneName, "root"))
		{
			_float3 vRot = QuaternionToEuler(m_Channels[_iAnimIndex]->Get_CurrentKeyFrame()->vRotation);
			_float3 vPos = m_Channels[_iAnimIndex]->Get_CurrentKeyFrame()->vPosition;
			_float3 vScale = m_Channels[_iAnimIndex]->Get_CurrentKeyFrame()->vScale;

			switch (_eRootOption)
			{
			case ERootOption::X:
				vPos.x = 0;
				vRot.x = 0;
				break;
			case ERootOption::Y:
				vPos.y = 0;
				vRot.y = 0;
				break;
			case ERootOption::Z:
				vPos.z = 0;
				vRot.z = 0;
				break;
			case ERootOption::XY:
				vPos.x = 0;
				vPos.y = 0;
				vRot.x = 0;
				vRot.y = 0;
				break;
			case ERootOption::XZ:
				vPos.x = 0;
				vPos.z = 0;
				vRot.x = 0;
				vRot.z = 0;
				break;
			case ERootOption::YZ:
				vPos.y = 0;
				vPos.z = 0;
				vRot.y = 0;
				vRot.z = 0;
				break;
			case ERootOption::XYZ:
				vPos.x = 0;
				vPos.y = 0;
				vPos.z = 0;
				vRot.x = 0;
				vRot.y = 0;
				vRot.z = 0;
				break;
			}

			XMStoreFloat4x4(&m_TransformationMatrix
				, XMMatrixScaling(vScale.x, vScale.y, vScale.z)
				* XMMatrixRotationRollPitchYaw(vRot.x, vRot.y, vRot.z)
				* XMMatrixTranslation(vPos.x, vPos.y, vPos.z));
		}
		else
			XMStoreFloat4x4(&m_TransformationMatrix, m_Channels[_iAnimIndex]->Get_TransformMatrix());
	}

	if (nullptr != m_pParent)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMMatrixIdentity());
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

CHierarchyNode* CHierarchyNode::Create(char* pBoneName, _fmatrix OffsetMatrix, _fmatrix TransformationMatrix, _uint iDepth, CHierarchyNode* pParent)
{
	CHierarchyNode* pInstance = new CHierarchyNode();
	if (FAILED(pInstance->NativeConstruct(pBoneName, OffsetMatrix, TransformationMatrix, iDepth, pParent)))
	{
		MSGBOX("CHierarchyNode Load Fail");
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
