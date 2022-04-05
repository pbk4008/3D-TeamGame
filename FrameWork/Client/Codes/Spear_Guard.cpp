#include "pch.h"
#include "Spear_Guard.h"
#include "Monster_Bastion_Spear.h"
#include "Animation.h"

CSpear_Guard::CSpear_Guard(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
	, m_bGuardEnd(false)
{
}

CSpear_Guard::CSpear_Guard(const CSpear_Guard& _rhs)
	: CSpear_State(_rhs)
	, m_bGuardEnd(_rhs.m_bGuardEnd)
{
}

HRESULT CSpear_Guard::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Guard::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Tick(_dDeltaTime);


	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_GUARD)
	{
		if (m_pAnimator->Get_CurrentAnimation()->Get_CurrentKeyFrameIndex() >= 90)
			m_pAnimator->Get_AnimController()->Set_PlaySpeed(0.f);
		if (m_pOwner->Get_GuardCount() <= 0)
		{
			m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);
			m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_RICOCHET);
		}
		if (!g_pObserver->Get_PlayerAttackAnimStart())
		{
			m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);
			m_pOwner->Set_Guard(false);
		}
	}
	else
	{
		if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
		{
			m_pStateController->Change_State(L"Idle");
			m_pOwner->Set_Guard(false);
		}
	}

	return _int();
}

_int CSpear_Guard::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Guard::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Guard::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->BlendSound(L"Spear_Guard", L"Spear_Guard_1", CSoundMgr::CHANNELID::Spear_Attack_1, CSoundMgr::CHANNELID::Spear_Attack_2);

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_GUARD)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Guard::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CSpear_Guard::Look_Player(void)
{
	//if (!g_pObserver->IsAttack() && m_pAnimator->Get_AnimController()->Is_Finished())
	//	m_pStateController->Change_State(L"Chaser");
}

void CSpear_Guard::Look_Monster(void)
{

}



CSpear_Guard* CSpear_Guard::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Guard* pInstance = new CSpear_Guard(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Guard Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_Guard::Free()
{
	__super::Free();
}
