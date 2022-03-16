#pragma once
#ifndef __ANIMNODE_H__
#define __ANIMNODE_H__

#include "Base.h"
BEGIN(Engine)
class CAnimation;
class CAnimNode final : public CBase
{
private:
	explicit CAnimNode();
	virtual ~CAnimNode() = default;
public:
	//AnimNode �±�, �ִϸ��̼�, �ݺ���� ����, �ߺ������� ���� Animator�� �ִ� ������ Index
	virtual HRESULT NativeConstruct_Prototype(_uint iTag, CAnimation* pAnim, _bool bLoop, _bool bRootAnim, _bool bTransformMove, ERootOption eOption);
public:
	//������ ������ �ִ� ��ũ���Ϳ� �ش� �̸��� ������ �ִ� ��ũ�� �ִ��� ������ �Ǵ�
	CAnimNode* Check_ConnectNode(_uint iIndex);
	//�ִϸ��̼� ��� ����
	void Conntect_AnimNode(CAnimNode* pNode);
	void Set_RootAnimValue(_bool& bUsingRootAnim, _bool& bUsingTransformAnim, ERootOption& eRootOption);
	void Change_Loop(_bool bChange);
public:
	//�ڽ� ������ ����ִ��� �Ǵ�
	const _bool  Is_LinkEmpty() { return m_vecAnimNode.empty(); }
	CAnimation* Get_Animation() { return m_pAnim; }
	_bool Get_Loop() { return m_bLoop; }
	_int Get_Index() { return m_iIndex; }
	_int Get_AutoIndex() { return m_iAutoIndex; }
	_bool Is_LoopChange() { return m_bLoop != m_bTmpLoop; }
	HRESULT Delete_Node(vector<_uint>& vecNode);
public:
	void Set_AutoChangeIndex(_int ChangeIndex) { m_iAutoIndex = ChangeIndex; }
public:
	//AnimNode �±�, �ִϸ��̼�, �ݺ���� ����, �ߺ������� ���� Animator�� �ִ� ������ Index
	static CAnimNode* Create(_uint iTag, CAnimation* pAnim, _bool bLoop, _bool bRootAnim, _bool bTransformMove, ERootOption eOption);

private:
	virtual void Free() override;
private:
	CAnimation* m_pAnim;//�� �ִϸ��̼�
	_bool m_bLoop;//�ݺ� ���
	_bool m_bTmpLoop;//�ݺ� ��� ������ ����
	vector<CAnimNode*> m_vecAnimNode;//���� ���� ����Ǿ� �ִ� �ڽĳ���
	_int m_iIndex;//�ߺ� ������ ���� Index(Animator�� ������ �ε���)
	_int m_iAutoIndex;//���� AnimNode�� ������ �ڵ����� �Ѿ �ε���
	_bool m_bRootAnim;
	_bool m_bTransformMove;
	ERootOption m_eRootOption;
};
END
#endif