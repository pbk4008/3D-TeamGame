#include "pch.h"
#include "..\Headers\Shooter_Chase.h"
#include "Monster_Bastion_Shooter.h"

CShooter_Chase::CShooter_Chase(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CMonster_FSM(_pDevice, _pDeviceContext)
	, m_fAngle(0.f)
{
}

HRESULT CShooter_Chase::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	CMonster_FSM::FSMMOVEDESC tDesc = (*(CMonster_FSM::FSMMOVEDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_pTransform = tDesc.pTransform;
	m_pStateController = tDesc.pController;
	m_wstrTag = tDesc.pName;

	Safe_AddRef(m_pAnimator);
	Safe_AddRef(m_pTransform);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;
	return S_OK;
}

_int CShooter_Chase::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator || !m_pStateController)
		return -1;

	//애니메이션 진행
	//쫓기 시작 -쫓기 루프 -쫓기 끝의 루프를 진행

	m_pAnimator->Tick(_dDeltaTime);
	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	//일정 거리가 되면 바로 공격
	_vector vPlayerPos = g_pObserver->Get_PlayerPos();
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_float fDist = XMVectorGetX(XMVector3Length(vPlayerPos - vPos));
	if (fDist < 5.f)
		m_pStateController->Change_State(L"Attack");
	//해당 애니메이션이 종착 애니메이션에 도달하면 상태머신의 상태 변경
	if (!m_pAnimator->Get_IsLerp() && m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Shooter::ANIM_TYPE::IDLE)
		m_pStateController->Change_State(L"Idle");

	return _int();
}

_int CShooter_Chase::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CShooter_Chase::Render()
{
	return S_OK;
}

HRESULT CShooter_Chase::EnterState()
{
	if (!m_pAnimator)
		return E_FAIL;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::RUN_START)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShooter_Chase::ExitState()
{
	return S_OK;
}

HRESULT CShooter_Chase::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CShooter_Chase::ExitState(void* pArg)
{
	return S_OK;
}

CShooter_Chase* CShooter_Chase::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Chase* pInstance = new CShooter_Chase(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Chase Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShooter_Chase::Free()
{
	CMonster_FSM::Free();
}
