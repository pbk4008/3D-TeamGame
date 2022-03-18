#include "pch.h"
#include "..\Headers\Bastion_Sword_Chase.h"
#include "Monster_Bastion_Sword.h"
#include "StateController.h"

CBastion_Sword_Chase::CBastion_Sword_Chase(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CBastion_Sword_Chase::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	CMonster_FSM::FSMMOVEDESC tDesc = (*(CMonster_FSM::FSMMOVEDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_pTransform = tDesc.pTransform;
	m_pStateController = tDesc.pController;

	Safe_AddRef(m_pAnimator);
	Safe_AddRef(m_pTransform);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;
	return S_OK;
}

_int CBastion_Sword_Chase::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator||!m_pStateController)
		return -1;

	//�ִϸ��̼� ����
	//�ѱ� ���� -�ѱ� ���� -�ѱ� ���� ������ ����
	m_pAnimator->Tick(_dDeltaTime);

	m_fAccTime += _dDeltaTime;
	//���� ����(ex : �÷��̾���� �Ÿ��� ���� �Ÿ��� �Ǹ�)�� �ش� �ִϸ��̼��� �������ϸ��̼��̸�
	if (m_fAccTime > 5.f&&!bChange)
	{
		bChange = true;
 		m_fAccTime = 0.f;
		//���� �ִϸ��̼ǿ� �ڵ����� �ű�°� ����Ǿ� ������ �ڵ����� �ű�� ������ �ִϸ��̼� ��ȯ
		m_pAnimator->Change_LoopAnim();
	}

	//�ش� �ִϸ��̼��� ���� �ִϸ��̼ǿ� �����ϸ� ���¸ӽ��� ���� ����
	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Sword::ANIM_TYPE::IDLE)
	{
		m_pStateController->Change_State(L"Idle");
	}

	return _int();
}

_int CBastion_Sword_Chase::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CBastion_Sword_Chase::Render()
{
	return S_OK;
}

HRESULT CBastion_Sword_Chase::EnterState()
{
	if (!m_pAnimator)
		return E_FAIL;
	//bChange = false;
	//m_fAccTime = 0.f;
	//������ ���� �ִϸ��̼� ���� ���� ��ġ�� RunStart
	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::RUN_START)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_Sword_Chase::ExitState()
{
	return S_OK;
}

CBastion_Sword_Chase* CBastion_Sword_Chase::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Sword_Chase* pInstance = new CBastion_Sword_Chase(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Sword_Chase Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Sword_Chase::Free()
{
	CMonster_FSM::Free();
}
