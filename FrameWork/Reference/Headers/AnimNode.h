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
	//AnimNode 태그, 애니메이션, 반복재생 여부, 중복방지를 위한 Animator에 있는 백터의 Index
	virtual HRESULT NativeConstruct_Prototype(_uint iTag, CAnimation* pAnim, _bool bLoop, _bool bRootAnim, _bool bTransformMove, ERootOption eOption);
public:
	//본인이 가지고 있는 링크벡터에 해당 이름을 가지고 있는 링크가 있는지 없는지 판단
	CAnimNode* Check_ConnectNode(_uint iIndex);
	//애니메이션 노드 연결
	void Conntect_AnimNode(CAnimNode* pNode);
	void Set_RootAnimValue(_bool& bUsingRootAnim, _bool& bUsingTransformAnim, ERootOption& eRootOption);
	void Change_Loop(_bool bChange);
public:
	//자식 노드들이 비어있는지 판단
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
	//AnimNode 태그, 애니메이션, 반복재생 여부, 중복방지를 위한 Animator에 있는 백터의 Index
	static CAnimNode* Create(_uint iTag, CAnimation* pAnim, _bool bLoop, _bool bRootAnim, _bool bTransformMove, ERootOption eOption);

private:
	virtual void Free() override;
private:
	CAnimation* m_pAnim;//찐 애니메이션
	_bool m_bLoop;//반복 재생
	_bool m_bTmpLoop;//반복 재생 보관용 변수
	vector<CAnimNode*> m_vecAnimNode;//현재 노드와 연결되어 있는 자식노드들
	_int m_iIndex;//중복 방지를 위한 Index(Animator의 벡터의 인덱스)
	_int m_iAutoIndex;//현재 AnimNode가 끝나면 자동으로 넘어갈 인덱스
	_bool m_bRootAnim;
	_bool m_bTransformMove;
	ERootOption m_eRootOption;
};
END
#endif