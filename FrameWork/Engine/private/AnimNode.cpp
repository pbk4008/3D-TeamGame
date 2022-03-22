#include "AnimNode.h"
#include "Animator.h"


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


CAnimNode* CAnimNode::Check_ConnectNode(_uint iIndex, vector<_uint>* vecDuplicate)
{
	if (Is_LinkEmpty())
		return nullptr;

	//찾는 노드
	CAnimNode* pFind = nullptr;
	//모든 자식 노드 탐색

	//새로운 중복 방지벡터
	vector<_uint> vecNewDuplicate;
	for (auto& pNode : m_vecAnimNode)
	{
		//탐색 시작 체크를 하는 변수

		_bool bCheck = false;
		for (auto& pDuplicate : *vecDuplicate)
		{
			//중복 방지 벡터와 같은 Index가 있으면
			if (pDuplicate == pNode->m_iIndex)
			{
				//탐색 시작을 하고
				bCheck = true;
				//새로운 중복 방지 벡터에 넣지 않기 위해 continue로 건너뜀
				continue;
			}
			//같은 벡터가 없으면 새로운 중복 방지 벡터에 넣는다
			_bool bDoubleCheck = false;
			for (auto& pNewDuplicate : vecNewDuplicate)
			{
				if (pNewDuplicate == pDuplicate)
				{
					bDoubleCheck = true;
					break;
				}
			}
			if(!bDoubleCheck)
				vecNewDuplicate.emplace_back(pDuplicate);
		}
		//탐색 시작을 못하면(중복 방지 벡터에 걸리는게 없으면) null
		if (!bCheck)
			return nullptr;
		//자식의 index가 매개변수로 받은 index와 같으면
		if(pNode->m_iIndex == iIndex)
		{
			//노드를 가져오고 for 문을 벗어난다.
			pFind = pNode;
			break;
		}
		//자식의 index가 매개변수로 받은 index와 다르면
		//자식의 자식으로 찾는다.
		pFind = pNode->Check_ConnectNode(iIndex, &vecNewDuplicate);
		//자식의 자식으로 값을 받고 있으면 for문을 나간다
		if (pFind)
			break;
	}
	return pFind;
}

CAnimNode* CAnimNode::Check_ConnectNode(_uint iIndex)
{
	//자식 링크가 비어있는지 판단
	if (Is_LinkEmpty())
		return nullptr;

	//찾는 노드
	CAnimNode* pFind = nullptr;
	//모든 자식 노드 탐색
	for (auto& pNode : m_vecAnimNode)
	{
		if (pNode->m_iIndex == iIndex)
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

void CAnimNode::Change_Loop(_bool bChange)
{
	m_bTmpLoop = m_bLoop;
	m_bLoop = bChange;
}

HRESULT CAnimNode::Delete_Node(vector<_uint>& vecNode)
{
	_bool bCheck = false;
	for (auto iter = vecNode.begin(); iter != vecNode.end(); iter++)
	{
		if (*iter == m_iIndex)
		{
			bCheck = true;
			if (iter == vecNode.end() - 1)
			{
				vecNode.pop_back();
				break;
			}
			vecNode.erase(iter);
			break;
		}
	}
	if (!bCheck)
		return S_OK;

	for (auto& pNode : m_vecAnimNode)
	{
		for (auto& pNum : vecNode)
		{
			if (pNode)
			{
				if (pNum == pNode->m_iIndex)
				{
					pNode->Delete_Node(vecNode);
					Safe_Release(pNode);
				}
			}
		}
	}
	m_vecAnimNode.clear();

	return S_OK;
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
	
}
