#include "AnimNode.h"

CAnimNode::CAnimNode()
	: m_pAnim(nullptr)
	, m_bLoop(false)
	, m_iIndex(-1)
	, m_iAutoIndex(-1)
	, m_bRootAnim(false)
	, m_bTransformMove(false)
	, m_bTmpLoop(false)
	, m_eRootOption(ERootOption::Max)
{
}

HRESULT CAnimNode::NativeConstruct_Prototype(_uint iTag, CAnimation* pAnim, _bool bLoop,  _bool bRootAnim, _bool bTransformMove, ERootOption eOption)
{
	m_pAnim = pAnim;
	m_bLoop = bLoop;
	m_iIndex = iTag;
	m_bRootAnim = bRootAnim;
	m_bTransformMove = bTransformMove;
	m_eRootOption = eOption;

	return S_OK;
}


CAnimNode* CAnimNode::Check_ConnectNode(_uint iIndex)
{
	if (Is_LinkEmpty())
		return nullptr;

	CAnimNode* pFind = nullptr;

	for (auto& pNode : m_vecAnimNode)
	{
		if(pNode->m_iIndex == iIndex)
		{
			pFind = pNode;
			break;
		}
		pFind = pNode->Check_ConnectNode(iIndex);
		if (pFind)
			break;
	}

	return pFind;
}

void CAnimNode::Conntect_AnimNode(CAnimNode* pNode)
{
	m_vecAnimNode.emplace_back(pNode);
}

void CAnimNode::Set_RootAnimValue(_bool& bUsingRootAnim, _bool& bUsingTransformAnim, ERootOption& eRootOption)
{
	bUsingRootAnim = m_bRootAnim;
	bUsingTransformAnim = m_bTransformMove;
	eRootOption = m_eRootOption;
}

void CAnimNode::Change_Loop(_bool bChange)
{
	m_bTmpLoop = m_bLoop;
	m_bLoop = bChange;
}

CAnimNode* CAnimNode::Create(_uint iTag, CAnimation* pAnim, _bool bLoop, _bool bRootAnim, _bool bTransformMove, ERootOption eOption)
{
	CAnimNode* pInstance = new CAnimNode();
	if (FAILED(pInstance->NativeConstruct_Prototype(iTag, pAnim, bLoop, bRootAnim, bTransformMove,eOption)))
	{
		MSGBOX("CAnimNode Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimNode::Free()
{
	for (auto& pNode : m_vecAnimNode)
		Safe_Release(pNode);

	m_vecAnimNode.clear();
}
