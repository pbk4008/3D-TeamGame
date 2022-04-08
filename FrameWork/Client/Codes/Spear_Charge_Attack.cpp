#include "pch.h"
#include "Spear_Charge_Attack.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Spear.h"

CSpear_Charge_Attack::CSpear_Charge_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Charge_Attack::CSpear_Charge_Attack(const CSpear_Charge_Attack& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Charge_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Charge_Attack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Check_Attack();
	Play_Sound();
	m_pAnimator->Tick(_dDeltaTime);

	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_CHARGE
		&&m_pOwner->Get_Attack()
		&&!m_pAnimator->Get_IsLerp())
		m_pAnimator->Change_LoopAnim();
	else if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_CHARGE_ED
		&& !m_pAnimator->Get_IsLerp()
		&& m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		m_pOwner->Set_ChargeOn(true);
		m_pOwner->Set_Attack(false);
		m_pOwner->Set_Target(false);
		m_pOwner->Attack(false);
		m_pStateController->Change_State(L"Idle");
	}
	return _int();
}

_int CSpear_Charge_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Charge_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Charge_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	//m_bChargeOn = true;
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_CHARGE_ST);
	
	return S_OK;
}

HRESULT CSpear_Charge_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CSpear_Charge_Attack::Look_Player(void)
{
	/*if (7.0f >= m_fDistance && m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		m_bAttack = true;
		m_pStateController->Change_State(L"Charge_Attack_End");
	}*/
}

void CSpear_Charge_Attack::Look_Monster(void)
{

}

void CSpear_Charge_Attack::Play_Sound()
{

}

void CSpear_Charge_Attack::Check_Attack()
{
	if (m_pOwner)
	{
		_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();
		if ((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_CHARGE_ED == m_pAnimator->Get_CurrentAnimNode())
		{
			if (iCurKeyFrameIndex >= 45 && iCurKeyFrameIndex < 60)
			{
				m_pOwner->Attack(true);
				_float fDamage = 15.f;
				_uint iLevel = 3;
				m_pOwner->Set_AttackDesc_Damaga(fDamage);
				m_pOwner->Set_AttackDesc_Level(iLevel);
			}
			else
				m_pOwner->Attack(false);
		}
	}
}

CSpear_Charge_Attack* CSpear_Charge_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Charge_Attack* pInstance = new CSpear_Charge_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Charge_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_Charge_Attack::Free()
{
	__super::Free();
}
