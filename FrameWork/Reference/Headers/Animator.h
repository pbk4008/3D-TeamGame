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
	HRESULT Insert_Animation(const _tchar* pName, const wstring& pConnectName, class CAnimation* pAnim, _bool bRootAnim, _bool bTransFrom, _bool bLoop, ERootOption eOption,_bool bDouble=false);
	//�ִϸ��̼� ����(�����ϰ��� �ϴ� �ִϸ��̼��±�, �ִϸ��̼� ���, ���߿����Ұ��� ���Ұ���)
	HRESULT Connect_Animation(const wstring& pConnectName, CAnimNode* pNode, _bool bDouble);
	//�ִϸ��̼� ����(�����ϰ����ϴ� �ִϸ��̼� �±�)
	HRESULT Change_Animation(const wstring& pName);
	//���� �ִϸ��̼��� �����ִϸ��̼��϶��� ���
	//���� �ִϸ��̼��� ������ �ڵ����� ����Ǵ� �ִϸ��̼��� ������� �ڵ����� �����ϵ��� ��
	HRESULT Change_LoopAnim();
	//�ִϸ��̼� ������ �ڵ����� �Ѿ �ִϸ��̼� ����
	HRESULT Set_UpAutoChangeAnimation(const wstring& pAnim, const wstring& pEndAnim);
public:
	const wstring& Get_CurrentAnim();
private:
	//�ִϸ��̼� ã��
	CAnimNode* Find_Animation(const wstring& pConnectName, CAnimNode* pNode=nullptr);
	//AnimNode �ߺ� üũ
	_bool Get_DuplicateTag(const wstring& pName);
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
	vector<wstring> m_vecAnimNodeName;//�̸� �����(��带 ���鶧 �־��༭ �ߺ�üũ �ϴ� ����)
};
END
#endif