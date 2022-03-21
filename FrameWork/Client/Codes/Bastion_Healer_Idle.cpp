#include "pch.h"
#include "Bastion_Healer_Idle.h"
#include "Monster_Bastion_Healer.h"
#include "Animation.h"

CBastion_Healer_Idle::CBastion_Healer_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_Healer_State(_pDevice, _pDeviceContext)
{
}

CBastion_Healer_Idle::CBastion_Healer_Idle(const CBastion_Healer_Idle& _rhs)
	: CBastion_Healer_State(_rhs)
{
}

HRESULT CBastion_Healer_Idle::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Healer_Idle::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	//if (m_bTargetOn)
	//	m_pStateController->Change_State(L"Chaser");
	//else if (m_bRageOn)
	//	m_pStateController->Change_State(L"Rage");

	return _int();
}

_int CBastion_Healer_Idle::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_Healer_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_Healer_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_Healer_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CBastion_Healer_Idle::Look_Player(void)
{
	//if (m_bAttackOn && !m_bRageOn)
	//{
	//	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	//	m_pStateController->Change_State(L"Attack");
	//}
}

void CBastion_Healer_Idle::Look_Monster(void)
{

}

CBastion_Healer_Idle* CBastion_Healer_Idle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Healer_Idle* pInstance = new CBastion_Healer_Idle(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Healer_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_Healer_Idle::Free()
{
	__super::Free();
}
