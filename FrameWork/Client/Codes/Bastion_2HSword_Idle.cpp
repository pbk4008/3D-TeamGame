#include "pch.h"
#include "Bastion_2HSword_Idle.h"
#include "Monster_Bastion_2HSword.h"


CBastion_2HSword_Idle::CBastion_2HSword_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Idle::CBastion_2HSword_Idle(const CBastion_2HSword_Idle& _rhs)
	: CMonster_FSM(_rhs)
{
}

HRESULT CBastion_2HSword_Idle::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_Idle::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBastion_2HSword_Idle::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_2HSword_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CBastion_2HSword_Idle::Look_Player(void)
{
	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - XMLoadFloat3(&g_pObserver->m_fPos);
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (5.0f < fDistToPlayer && 15.0f > fDistToPlayer)
	{
		m_pTransform->Face_Target(XMLoadFloat3(&g_pObserver->m_fPos));
		m_pStateController->Change_State(L"Chaser");
	}

	if(5.0f > fDistToPlayer && TRUE == g_pObserver->m_bAttack)
		m_pStateController->Change_State(L"Dash");

	if(4.0f > fDistToPlayer)
		m_pStateController->Change_State(L"Attack");
}

CBastion_2HSword_Idle* CBastion_2HSword_Idle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_Idle* pInstance = new CBastion_2HSword_Idle(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_2HSword_Idle::Free()
{
	__super::Free();
}
