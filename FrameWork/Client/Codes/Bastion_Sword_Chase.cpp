#include "pch.h"
#include "..\Headers\Bastion_Sword_Chase.h"
#include "Monster_Bastion_Sword.h"
#include "StateController.h"

CBastion_Sword_Chase::CBastion_Sword_Chase(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CMonster_FSM(_pDevice, _pDeviceContext)
	, m_fAngle(0.f)
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
	m_wstrTag = tDesc.pName;

	//Safe_AddRef(m_pAnimator);
	//Safe_AddRef(m_pTransform);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;
	return S_OK;
}

_int CBastion_Sword_Chase::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator||!m_pStateController) 
		return -1;

	//애니메이션 진행
	//쫓기 시작 -쫓기 루프 -쫓기 끝의 루프를 진행
	m_pAnimator->Tick(_dDeltaTime);
	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	//m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	//일정 거리가 되면 바로 공
	_float fDist = g_pObserver->Get_Dist(m_pTransform->Get_State(CTransform::STATE_POSITION));
	_uint iAtkType = rand() % 3/*(_uint)MathUtils::ReliableRandom(0, 3)*/;
	if (fDist >= 10.f)
		m_pStateController->Change_State(L"Idle");
	else
	{
		if (iAtkType == 2)
		{
			if (fDist < 4.f)
				m_pStateController->Change_State(L"Attack", &iAtkType);
		}
		else
		{
			if (fDist < 2.f)
				m_pStateController->Change_State(L"Attack", &iAtkType);
		}
	}
	//if (iAtkType == 2)
	//{
	//	if (g_pObserver->Get_Dist(m_pTransform->Get_State(CTransform::STATE_POSITION)) < 4.f)
	//	{
	//		m_pStateController->Change_State(L"Attack",&iAtkType);
	//		return 0;
	//	}
	//}
	//else
	//{
	//	if (g_pObserver->Get_Dist(m_pTransform->Get_State(CTransform::STATE_POSITION)) < 2.f)
	//	{
	//		m_pStateController->Change_State(L"Attack", &iAtkType);
	//		return 0;
	//	}
	//}
	//해당 애니메이션이 종착 애니메이션에 도달하면 상태머신의 상태 변경
	/*if (!m_pAnimator->Get_IsLerp() && m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Sword::ANIM_TYPE::IDLE)
		m_pStateController->Change_State(L"Idle");*/

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

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::RUN_START)))
		return E_FAIL;

	//m_pAnimator->Get_AnimController()->Mul_MoveSpeed(2.f);

	//_matrix matRotate = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	//m_pAnimator->Set_PivotMatrix(matRotate);

	return S_OK;
}

HRESULT CBastion_Sword_Chase::ExitState()
{
	/*_matrix matIdentity = XMMatrixIdentity();
	m_pAnimator->Set_PivotMatrix(matIdentity);*/

	//m_pAnimator->Get_AnimController()->Div_MoveSpeed(2.f);

	return S_OK;
}

HRESULT CBastion_Sword_Chase::EnterState(void* pArg)
{
	//if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::RUN_START)))
	//	return E_FAIL;

	//_matrix matRotate = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	 
	//m_pAnimator->Set_PivotMatrix(matRotate);
	return S_OK;
}

HRESULT CBastion_Sword_Chase::ExitState(void* pArg)
{
	//_matrix matIdentity = XMMatrixIdentity();
	//m_pAnimator->Set_PivotMatrix(matIdentity);

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
