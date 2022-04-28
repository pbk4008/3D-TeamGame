#include "pch.h"
#include "..\Headers\Bastion_Sword_Attack.h"
#include "Monster_Bastion_Sword.h"
#include "Animation.h"

CBastion_Sword_Attack::CBastion_Sword_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
	, m_eAttackType(ATTACK_TYPE::ATTACK_END)
{
}

HRESULT CBastion_Sword_Attack::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMACTORDESC tDesc = (*(FSMACTORDESC*)_pArg);
	m_pAnimator = tDesc.pAnimator;
	m_pStateController = tDesc.pController;
	m_pMonster = tDesc.pActor;
	m_pTransform = m_pMonster->Get_Transform();

	m_wstrTag = tDesc.pName;

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Sword_Attack::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	Play_Sound();

	m_pAnimator->Tick(_dDeltaTime);

	if (!m_pAnimator->Get_IsLerp()
		&&(m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_SINGLE
		|| m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_DOUBLE))
	{
		if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
			m_pStateController->Change_State(L"Idle");
	}
	else if (!m_pAnimator->Get_IsLerp()
		&&m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_JUMPEND)
	{
		if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
			m_pStateController->Change_State(L"Idle");
	}

	/*if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		if (m_eAttackType != ATTACK_TYPE::ATTACK_JUMP)
			m_pStateController->Change_State(L"Idle");
		else
		{
			if(m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_JUMPEND)
				m_pStateController->Change_State(L"Idle");
		}
	}*/

	//keyframe에따라 데미지 다르게 들어가게 
	CMonster_Bastion_Sword* pMonster = (CMonster_Bastion_Sword*)m_pStateController->Get_GameObject();
	if (nullptr != pMonster)
	{
		_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

		if ((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_SINGLE == m_pAnimator->Get_CurrentAnimNode())
		{
			if (120 < iCurKeyFrameIndex && 150 > iCurKeyFrameIndex)
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

		else if ((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_DOUBLE == m_pAnimator->Get_CurrentAnimNode())
		{
			cout << "Double : " << iCurKeyFrameIndex << endl;

			if (80 < iCurKeyFrameIndex && 200 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);

				_float fDamage = 3.f;
				_uint iLevel = 1;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}
			/*else if (180 < iCurKeyFrameIndex && 200 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);

				_float fDamage = 5.f;
				_uint iLevel = 2;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}*/
			else
			{
				pMonster->Set_IsAttack(false);
			}
		}

		else if ((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_JUMPEND == m_pAnimator->Get_CurrentAnimNode())
		{
			if (50 < iCurKeyFrameIndex && 70 > iCurKeyFrameIndex)
			{
				pMonster->Set_IsAttack(true);

				_float fDamage = 5.f;
				_uint iLevel = 3;
				pMonster->Set_AttackDesc_Damaga(fDamage);
				pMonster->Set_AttackDesc_Level(iLevel);
			}
			else
				pMonster->Set_IsAttack(false);
		}
	}

	return _int();
}

_int CBastion_Sword_Attack::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CBastion_Sword_Attack::Render()
{
	return S_OK;
}

HRESULT CBastion_Sword_Attack::EnterState(void* pArg)
{
	m_eAttackType = (*(ATTACK_TYPE*)pArg);

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	switch (m_eAttackType)
	{
	case ATTACK_TYPE::ATTACK_SINGLE:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_SINGLE)))
			return E_FAIL;
		m_pAnimator->Get_AnimController()->Mul_MoveSpeed(3.5f);
		break;
	case ATTACK_TYPE::ATTACK_DOUBLE:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_DOUBLE)))
			return E_FAIL;
		m_pAnimator->Get_AnimController()->Mul_MoveSpeed(3.5f);
		break;
	case ATTACK_TYPE::ATTACK_JUMP:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_JUMPSTART)))
			return E_FAIL;
		m_pAnimator->Get_AnimController()->Mul_MoveSpeed(1.5f);
		break;
	}

	return S_OK;
}

HRESULT CBastion_Sword_Attack::ExitState(void* _pArg)
{
	m_pMonster->Set_IsAttack(false);

	//사운드관련 변수
	m_bSingle = false;
	m_bDouble = false;
	m_bJump = false;

	//m_pAnimator->Get_AnimController()->Div_MoveSpeed(3.5f);

	return S_OK;
}

void CBastion_Sword_Attack::Play_Sound()
{
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	switch (m_eAttackType)
	{
	case Client::CBastion_Sword_Attack::ATTACK_TYPE::ATTACK_SINGLE:
		if (iCurKeyFrameIndex >= m_iSingleAtt && iCurKeyFrameIndex <= m_iSingleAtt+1)
		{
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Sword1H_Attack_1);
			g_pGameInstance->Play_Shot(L"H1_Attack_1", CSoundMgr::CHANNELID::Sword1H_Attack_1);
		}
		break;
	case Client::CBastion_Sword_Attack::ATTACK_TYPE::ATTACK_DOUBLE:
		if (iCurKeyFrameIndex >= m_iDoubleAtt&& iCurKeyFrameIndex <= m_iDoubleAtt + 1)
		{
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Sword1H_Attack_1);
			g_pGameInstance->Play_Shot(L"H1_Attack_2", CSoundMgr::CHANNELID::Sword1H_Attack_1);
			m_bDouble = true;
		}
		break;
	case Client::CBastion_Sword_Attack::ATTACK_TYPE::ATTACK_JUMP:
		if (iCurKeyFrameIndex >= m_iJumpAtt && iCurKeyFrameIndex <= m_iJumpAtt + 1)
		{
			if (!m_bJump)
			{
				g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Sword1H_Attack_2);
				g_pGameInstance->Play_Shot(L"H1_Attack_3", CSoundMgr::CHANNELID::Sword1H_Attack_2);
				m_bJump = true;
			}
		}
		break;
	}

	if (m_bDouble)
	{
		_uint iCurKeyFrameIndex2 = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

		if (Client::CBastion_Sword_Attack::ATTACK_TYPE::ATTACK_DOUBLE == m_eAttackType)
		{
			if (iCurKeyFrameIndex2 >= m_iDoubleAtt + 100 && iCurKeyFrameIndex2 >= m_iDoubleAtt + 101)
			{
				g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Sword1H_Attack_2);
				g_pGameInstance->Play_Shot(L"H1_Attack_3", CSoundMgr::CHANNELID::Sword1H_Attack_2);
			}
		}
	}
}

HRESULT CBastion_Sword_Attack::EnterState()
{
	return S_OK;
}

HRESULT CBastion_Sword_Attack::ExitState()
{
	m_bJump = false;

	switch (m_eAttackType)
	{
	case ATTACK_TYPE::ATTACK_SINGLE:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_SINGLE)))
			return E_FAIL;
		m_pAnimator->Get_AnimController()->Div_MoveSpeed(3.5f);
		break;
	case ATTACK_TYPE::ATTACK_DOUBLE:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_DOUBLE)))
			return E_FAIL;
		m_pAnimator->Get_AnimController()->Div_MoveSpeed(3.5f);
		break;
	case ATTACK_TYPE::ATTACK_JUMP:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_JUMPSTART)))
			return E_FAIL;
		m_pAnimator->Get_AnimController()->Div_MoveSpeed(1.5f);
		break;
	}
	return S_OK;
}

CBastion_Sword_Attack* CBastion_Sword_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Sword_Attack* pInstance = new CBastion_Sword_Attack(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Sword_Attack Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Sword_Attack::Free()
{
	CMonster_FSM::Free();
}
