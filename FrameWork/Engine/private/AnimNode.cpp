#include "AnimNode.h"

CAnimNode::CAnimNode()
	: m_pAnim(nullptr)
	, m_bLoop(false)
	, m_iIndex(-1)
	, m_iAutoIndex(-1)
	, m_wstrName(L"")
	, m_bRootAnim(false)
	, m_bTransformMove(false)
	, m_eRootOption(ERootOption::Max)
{
}

HRESULT CAnimNode::NativeConstruct_Prototype(const wstring& pName, CAnimation* pAnim, _bool bLoop, _uint iIndex, _bool bRootAnim, _bool bTransformMove, ERootOption eOption)
{
	m_wstrName = pName;
	m_pAnim = pAnim;
	m_bLoop = bLoop;
	m_iIndex = iIndex;

	return S_OK;
}

_bool CAnimNode::Check_AnimNodeName(const wstring& pName)
{
	if (pName==m_wstrName)
		return true;

	return false;
}

CAnimNode* CAnimNode::Check_ConnectNode(const wstring& pName)
{
	if (Is_LinkEmpty())
		return nullptr;

	CAnimNode* pFind = nullptr;


	for (auto& pNode : m_vecAnimNode)
	{
		if (pNode->Check_AnimNodeName(pName))
		{
			pFind = pNode;
			break;
		}
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

CAnimNode* CAnimNode::Create(const wstring& pName, CAnimation* pAnim, _bool bLoop, _uint iIndex, _bool bRootAnim, _bool bTransformMove, ERootOption eOption)
{
	CAnimNode* pInstance = new CAnimNode();
	if (FAILED(pInstance->NativeConstruct_Prototype(pName, pAnim, bLoop, iIndex,bRootAnim, bTransformMove,eOption)))
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
