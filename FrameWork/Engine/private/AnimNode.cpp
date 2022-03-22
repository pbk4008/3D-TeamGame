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

	//ã�� ���
	CAnimNode* pFind = nullptr;
	//��� �ڽ� ��� Ž��

	//���ο� �ߺ� ��������
	vector<_uint> vecNewDuplicate;
	for (auto& pNode : m_vecAnimNode)
	{
		//Ž�� ���� üũ�� �ϴ� ����

		_bool bCheck = false;
		for (auto& pDuplicate : *vecDuplicate)
		{
			//�ߺ� ���� ���Ϳ� ���� Index�� ������
			if (pDuplicate == pNode->m_iIndex)
			{
				//Ž�� ������ �ϰ�
				bCheck = true;
				//���ο� �ߺ� ���� ���Ϳ� ���� �ʱ� ���� continue�� �ǳʶ�
				continue;
			}
			//���� ���Ͱ� ������ ���ο� �ߺ� ���� ���Ϳ� �ִ´�
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
		//Ž�� ������ ���ϸ�(�ߺ� ���� ���Ϳ� �ɸ��°� ������) null
		if (!bCheck)
			return nullptr;
		//�ڽ��� index�� �Ű������� ���� index�� ������
		if(pNode->m_iIndex == iIndex)
		{
			//��带 �������� for ���� �����.
			pFind = pNode;
			break;
		}
		//�ڽ��� index�� �Ű������� ���� index�� �ٸ���
		//�ڽ��� �ڽ����� ã�´�.
		pFind = pNode->Check_ConnectNode(iIndex, &vecNewDuplicate);
		//�ڽ��� �ڽ����� ���� �ް� ������ for���� ������
		if (pFind)
			break;
	}
	return pFind;
}

CAnimNode* CAnimNode::Check_ConnectNode(_uint iIndex)
{
	//�ڽ� ��ũ�� ����ִ��� �Ǵ�
	if (Is_LinkEmpty())
		return nullptr;

	//ã�� ���
	CAnimNode* pFind = nullptr;
	//��� �ڽ� ��� Ž��
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
