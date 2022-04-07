#include "pch.h"
#include "BronzeAnimus_Attack.h"
#include "Animation.h"

/* Monster List */
#include "Monster_BronzeAnimus.h"

CBronzeAnimus_Attack::CBronzeAnimus_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

CBronzeAnimus_Attack::CBronzeAnimus_Attack(const CBronzeAnimus_Attack& _rhs)
	: CBronzeAnimus_State(_rhs)
{
}

HRESULT CBronzeAnimus_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Attack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	cout << "Attack" << endl;
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Tick(_dDeltaTime);

	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished()&&!m_pAnimator->Get_IsLerp())
	{
		m_pOwner->Set_AttackOn(false);
		m_pStateController->Change_State(L"A_Idle_Battle");
	}
	return _int();
}

_int CBronzeAnimus_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);

	return S_OK;
}

HRESULT CBronzeAnimus_Attack::EnterState(void* pArg)
{
	_int randAtt = (*(_int*)pArg);

	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.5f);
	switch (randAtt)
	{
	case 0:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_ATTACK_R1)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_ATTACK_R2)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_ATTACK_S1)))
			return E_FAIL;
		break;
	}


	return S_OK;
}

HRESULT CBronzeAnimus_Attack::ExitState(void* pArg)
{
	return S_OK;
}

void CBronzeAnimus_Attack::Look_Player(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
	{
		m_bAttackEnd = true;
		m_pStateController->Change_State(L"A_Idle_Battle");
	}
}

void CBronzeAnimus_Attack::Look_Monster(void)
{

}

CBronzeAnimus_Attack* CBronzeAnimus_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Attack* pInstance = new CBronzeAnimus_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBronzeAnimus_Attack::Free()
{
	__super::Free();
}
