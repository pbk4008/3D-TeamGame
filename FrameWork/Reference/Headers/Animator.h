#pragma once
#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__

#include "Component.h"
BEGIN(Engine)
class CAnimationController;
class CAnimNode;
class ENGINE_DLL CAnimator final : public CComponent
{
/*
	애니메이션이 재생되다가 끝나면 자동으로 전환되는 애니메이터
	몬스터나 로직(키보드 조작을 통한 로직이 아닌)을 통해서 움직이는 객체들이 사용하면 됨
*/
public:
	typedef struct tagAnimatorDesc
	{
		class CModel* pModel;
		class CTransform* pTransform;
	}ANIMATORDESC;
public:
	explicit CAnimator() = default;
	explicit CAnimator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CAnimator(const CAnimator& rhs);
	virtual ~CAnimator() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double dDeltaTime);
	virtual _int LateTick(_double dDeltaTime);
public:
	//애니메이션 추가(추가하는 애니메이션 태그, 연결하고자 하는 애니메이션 태그, 애니메이션, 이중연결할건지 안할건지)
	HRESULT Insert_Animation(_uint iTag, _uint iConnectTag, class CAnimation* pAnim, _bool bRootAnim, _bool bTransFrom, _bool bLoop, ERootOption eOption,_bool bDouble=false);
	//애니메이션 연결(연결하고자 하는 애니메이션태그, 애니메이션 노드, 이중연결할건지 안할건지)
	HRESULT Connect_Animation(_uint iConnectTag, CAnimNode* pNode, _bool bDouble);
	//애니메이션 연결(연결하는 애니메이션 태그, 연결하고자 하는 애니메이션 태그, 이중연결 할건지 안할건지
	HRESULT Connect_Animation(_uint iTag, _uint iConnectTag, _bool bDouble);
	//애니메이션 변경(변경하고자하는 애니메이션 태그)
	HRESULT Change_Animation(_uint iTag);
	//AnyEntry애니메이션으로 변경(변경하고자 하는 애니메이션 태그)
	HRESULT Change_AnyEntryAnimation(_uint iTag);
	//현재 애니메이션이 루프애니메이션일때만 사용
	//루프 애니메이션이 끝나면 자동으로 연결되는 애니메이션이 있을경우 자동으로 변경하도록 함
	HRESULT Change_LoopAnim();
	//애니메이션 끝나면 자동으로 넘어갈 애니메이션 연결
	HRESULT Set_UpAutoChangeAnimation(_uint iTag, _uint iEndTag);
	//애니메이션 어떤 조건을 통해서 들어오는 애니메이션 추가시 사용
	//이미 만들어진 애니메이션을 AnyEntry에 담아서 조건에 따라 변경하면서 사용
	HRESULT Insert_AnyEntryAnimation(_uint iTag);

	void Set_AnimSpeed(_float fSpeed) { m_fPlaySpeed = fSpeed; };
	void Set_PivotMat(const _fmatrix& matPivot);

public:
	const _uint Get_CurrentAnimNode();
	CAnimation* Get_CurrentAnimation();
	CAnimationController* Get_AnimController() { return m_pController; };
public:
	vector<_uint> Get_Nodes() { return m_vecAnimNode; }
private:
	//애니메이션 찾기
	CAnimNode* Find_Animation(_uint itTag, CAnimNode* pNode=nullptr);
	//AnyEntry애니메이션 찾기
	CAnimNode* Find_AnyEntryAnim(_uint iTag);
	//AnimNode 중복 체크
	_bool Get_DuplicateTag(_uint iAnimTag);
	_bool Get_AnyEntryDuplicateTag(_uint iAnimTag);
public:
	static CAnimator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	CAnimationController* m_pController;
	CAnimNode* m_pHead;//링크드 리스트 Head 노드
	CAnimNode* m_pCulAnimNode;//현재 노드
	CAnimNode* m_pChangeNode;//바꿀 노드

	vector<_uint> m_vecAnimNode;

	vector<CAnimNode*> m_vecAnyEntryNode;

	_float m_fPlaySpeed = 1.f;
};
END
#endif