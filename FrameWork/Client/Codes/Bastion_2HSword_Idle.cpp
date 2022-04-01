#include "pch.h"
#include "Bastion_2HSword_Idle.h"
#include "Monster_Bastion_2HSword.h"
#include "Animation.h"

CBastion_2HSword_Idle::CBastion_2HSword_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_2HSword_State(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Idle::CBastion_2HSword_Idle(const CBastion_2HSword_Idle& _rhs)
	: CBastion_2HSword_State(_rhs)
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

	if (m_pMonster->Get_Dead())
		return 0;

	m_pAnimator->Tick(_dDeltaTime);

	if (m_bTargetOn)
		m_pStateController->Change_State(L"Chaser");

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
	if (m_bAttackOn && !m_bRageOn)
	{
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
		m_pStateController->Change_State(L"Attack");
	}
}

void CBastion_2HSword_Idle::Look_Monster(void)
{

}

void CBastion_2HSword_Idle::OnTriggerEnter(CCollision& collision)
{
	if ((_uint)GAMEOBJECT::WEAPON == collision.pGameObject->getTag() && g_pObserver->IsAttack())
	{
		static_cast<CMonster_Bastion_2HSword*>(m_pMonster)->Set_CurrentHp(-1);
		m_pStateController->Change_State(L"Hit");
	}
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
