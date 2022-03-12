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
	virtual HRESULT NativeConstruct_Prototype(const wstring& pName, CAnimation* pAnim, _bool bLoop, _uint iIndex, _bool bRootAnim, _bool bTransformMove, ERootOption eOption);
public:
	//���� �̸��� ������ �Ǵ�
	_bool Check_AnimNodeName(const wstring& pName);
	//������ ������ �ִ� ��ũ���Ϳ� �ش� �̸��� ������ �ִ� ��ũ�� �ִ��� ������ �Ǵ�
	CAnimNode* Check_ConnectNode(const wstring& pName);
	//�ִϸ��̼� ��� ����
	void Conntect_AnimNode(CAnimNode* pNode);
	void Set_RootAnimValue(_bool& bUsingRootAnim, _bool& bUsingTransformAnim, ERootOption& eRootOption);
public:
	const wstring& Get_Name() { return m_wstrName; }
	//�ڽ� ������ ����ִ��� �Ǵ�
	const _bool  Is_LinkEmpty() { return m_vecAnimNode.empty(); }
	CAnimation* Get_Animation() { return m_pAnim; }
	_bool Get_Loop() { return m_bLoop; }
	_int Get_Index() { return m_iIndex; }
	_int Get_AutoIndex() { return m_iAutoIndex; }

public:
	void Set_AutoChangeIndex(_int ChangeIndex) { m_iAutoIndex = ChangeIndex; }
public:
	//AnimNode �±�, �ִϸ��̼�, �ݺ���� ����, �ߺ������� ���� Animator�� �ִ� ������ Index
	static CAnimNode* Create(const wstring& pName, CAnimation* pAnim, _bool bLoop, _uint iIndex, _bool bRootAnim, _bool bTransformMove, ERootOption eOption);
private:
	virtual void Free() override;
private:
	CAnimation* m_pAnim;//�� �ִϸ��̼�
	wstring m_wstrName;//Node �±�
	_bool m_bLoop;//�ݺ� ���
	vector<CAnimNode*> m_vecAnimNode;//���� ���� ����Ǿ� �ִ� �ڽĳ���
	_int m_iIndex;//�ߺ� ������ ���� Index(Animator�� ������ �ε���)
	_int m_iAutoIndex;//���� AnimNode�� ������ �ڵ����� �Ѿ �ε���
	_bool m_bRootAnim;
	_bool m_bTransformMove;
	ERootOption m_eRootOption;
};
END
#endif