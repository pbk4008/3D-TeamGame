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
	�ִϸ��̼��� ����Ǵٰ� ������ �ڵ����� ��ȯ�Ǵ� �ִϸ�����
	���ͳ� ����(Ű���� ������ ���� ������ �ƴ�)�� ���ؼ� �����̴� ��ü���� ����ϸ� ��
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
	//�ִϸ��̼� �߰�(�߰��ϴ� �ִϸ��̼� �±�, �����ϰ��� �ϴ� �ִϸ��̼� �±�, �ִϸ��̼�, ���߿����Ұ��� ���Ұ���)
	HRESULT Insert_Animation(_uint iTag, _uint iConnectTag, class CAnimation* pAnim, _bool bRootAnim, _bool bTransFrom, _bool bLoop, ERootOption eOption,_bool bDouble=false);
	//�ִϸ��̼� ����(�����ϰ��� �ϴ� �ִϸ��̼��±�, �ִϸ��̼� ���, ���߿����Ұ��� ���Ұ���)
	HRESULT Connect_Animation(_uint iConnectTag, CAnimNode* pNode, _bool bDouble);
	//�ִϸ��̼� ����(�����ϴ� �ִϸ��̼� �±�, �����ϰ��� �ϴ� �ִϸ��̼� �±�, ���߿��� �Ұ��� ���Ұ���
	HRESULT Connect_Animation(_uint iTag, _uint iConnectTag, _bool bDouble);
	//�ִϸ��̼� ����(�����ϰ����ϴ� �ִϸ��̼� �±�)
	HRESULT Change_Animation(_uint iTag);
	//AnyEntry�ִϸ��̼����� ����(�����ϰ��� �ϴ� �ִϸ��̼� �±�)
	HRESULT Change_AnyEntryAnimation(_uint iTag);
	//���� �ִϸ��̼��� �����ִϸ��̼��϶��� ���
	//���� �ִϸ��̼��� ������ �ڵ����� ����Ǵ� �ִϸ��̼��� ������� �ڵ����� �����ϵ��� ��
	HRESULT Change_LoopAnim();
	//�ִϸ��̼� ������ �ڵ����� �Ѿ �ִϸ��̼� ����
	HRESULT Set_UpAutoChangeAnimation(_uint iTag, _uint iEndTag);
	//�ִϸ��̼� � ������ ���ؼ� ������ �ִϸ��̼� �߰��� ���
	//�̹� ������� �ִϸ��̼��� AnyEntry�� ��Ƽ� ���ǿ� ���� �����ϸ鼭 ���
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
	//�ִϸ��̼� ã��
	CAnimNode* Find_Animation(_uint itTag, CAnimNode* pNode=nullptr);
	//AnyEntry�ִϸ��̼� ã��
	CAnimNode* Find_AnyEntryAnim(_uint iTag);
	//AnimNode �ߺ� üũ
	_bool Get_DuplicateTag(_uint iAnimTag);
	_bool Get_AnyEntryDuplicateTag(_uint iAnimTag);
public:
	static CAnimator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	CAnimationController* m_pController;
	CAnimNode* m_pHead;//��ũ�� ����Ʈ Head ���
	CAnimNode* m_pCulAnimNode;//���� ���
	CAnimNode* m_pChangeNode;//�ٲ� ���

	vector<_uint> m_vecAnimNode;

	vector<CAnimNode*> m_vecAnyEntryNode;

	_float m_fPlaySpeed = 1.f;
};
END
#endif