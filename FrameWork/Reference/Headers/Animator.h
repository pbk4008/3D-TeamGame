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
	HRESULT Insert_Animation(const _tchar* pName, const wstring& pConnectName, class CAnimation* pAnim, _bool bRootAnim, _bool bTransFrom, _bool bLoop, ERootOption eOption,_bool bDouble=false);
	//애니메이션 연결(연결하고자 하는 애니메이션태그, 애니메이션 노드, 이중연결할건지 안할건지)
	HRESULT Connect_Animation(const wstring& pConnectName, CAnimNode* pNode, _bool bDouble);
	//애니메이션 변경(변경하고자하는 애니메이션 태그)
	HRESULT Change_Animation(const wstring& pName);
	//현재 애니메이션이 루프애니메이션일때만 사용
	//루프 애니메이션이 끝나면 자동으로 연결되는 애니메이션이 있을경우 자동으로 변경하도록 함
	HRESULT Change_LoopAnim();
	//애니메이션 끝나면 자동으로 넘어갈 애니메이션 연결
	HRESULT Set_UpAutoChangeAnimation(const wstring& pAnim, const wstring& pEndAnim);
public:
	const wstring& Get_CurrentAnim();
private:
	//애니메이션 찾기
	CAnimNode* Find_Animation(const wstring& pConnectName, CAnimNode* pNode=nullptr);
	//AnimNode 중복 체크
	_bool Get_DuplicateTag(const wstring& pName);
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
	vector<wstring> m_vecAnimNodeName;//이름 저장소(노드를 만들때 넣어줘서 중복체크 하는 벡터)
};
END
#endif