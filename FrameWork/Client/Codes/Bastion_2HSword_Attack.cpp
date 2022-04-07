#include "pch.h"
#include "Bastion_2HSword_Attack.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_2HSword.h"

CBastion_2HSword_Attack::CBastion_2HSword_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_2HSword_State(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Attack::CBastion_2HSword_Attack(const CBastion_2HSword_Attack& _rhs)
	: CBastion_2HSword_State(_rhs)
{
}

HRESULT CBastion_2HSword_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_Attack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	cout << "Attack" << endl;
	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Tick(_dDeltaTime);

	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished() && !m_pAnimator->Get_IsLerp())
	{
		//m_pOwner->set_Attack(false);
		m_pOwner->set_RandAttack(-1);
		m_pStateController->Change_State(L"Idle");
	}

	//keyframe에따라 데미지 다르게 들어가게 
	CMonster_Bastion_2HSword* pMonster = (CMonster_Bastion_2HSword*)m_pStateController->Get_GameObject();
	if (nullptr != pMonster)
	{
		//cout << pMonster->IsAttack() << endl;

		_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

		if ((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_R1 == m_pAnimator->Get_CurrentAnimNode())
		{
			if (53 < iCurKeyFrameIndex && 62 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);

				_float fDamage = 4.f;
				_uint iLevel = 1;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}
			
			else if (105 < iCurKeyFrameIndex && 119 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);

				_float fDamage = 6.f;
				_uint iLevel = 2;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}

			else
			{
				pMonster->Set_IsAttack(false);
			}
		}

		else if ((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_R2 == m_pAnimator->Get_CurrentAnimNode())
		{
			if (73 < iCurKeyFrameIndex && 84 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);

				_float fDamage = 6.f;
				_uint iLevel = 2;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}
			else
			{
				pMonster->Set_IsAttack(false);
			}
		}

		else if ((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_S1 == m_pAnimator->Get_CurrentAnimNode())
		{
			if (54 < iCurKeyFrameIndex && 93 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);

				_float fDamage = 7.f;
				_uint iLevel = 3;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}
			else
				pMonster->Set_IsAttack(false);
		}

		else if ((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_S3== m_pAnimator->Get_CurrentAnimNode())
		{
			if (38 < iCurKeyFrameIndex && 48 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);

				_float fDamage = 4.f;
				_uint iLevel = 2;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}
			else
				pMonster->Set_IsAttack(false);
		}
	}

	return _int();
}

_int CBastion_2HSword_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_2HSword_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CBastion_2HSword_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);

	return S_OK;
}

HRESULT CBastion_2HSword_Attack::EnterState(void* pArg)
{
	_int iRand = (*(_int*)pArg);
	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.4f);
	switch (iRand)
	{
	case 0:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_R1)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_R2)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_S1)))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_S3)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CBastion_2HSword_Attack::ExitState(void* pArg)
{
	return S_OK;
}

void CBastion_2HSword_Attack::Look_Player(void)
{
}

void CBastion_2HSword_Attack::Look_Monster(void)
{

}

CBastion_2HSword_Attack* CBastion_2HSword_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_Attack* pInstance = new CBastion_2HSword_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_2HSword_Attack::Free()
{
	__super::Free();
}
