#include "pch.h"
#include "..\Headers\Bastion_Sword_Attack.h"
#include "Monster_Bastion_Sword.h"

CBastion_Sword_Attack::CBastion_Sword_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice,_pDeviceContext)
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

	/*Safe_AddRef(m_pAnimator);
	Safe_AddRef(m_pTransform);*/

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

	//무한 루프에 대한 조건 들어갈 자리

	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Sword::ANIM_TYPE::IDLE)
		m_pStateController->Change_State(L"Idle");


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
	m_pMonster->Set_IsAttack(true);
	m_eAttackType = (*(ATTACK_TYPE*)pArg);

	switch (m_eAttackType)
	{
	case ATTACK_TYPE::ATTACK_SINGLE:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_SINGLE)))
			return E_FAIL;
		break;
	case ATTACK_TYPE::ATTACK_DOUBLE:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_DOUBLE)))
			return E_FAIL;
		break;
	case ATTACK_TYPE::ATTACK_JUMP:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_JUMPSTART)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CBastion_Sword_Attack::ExitState(void* _pArg)
{
	m_pMonster->Set_IsAttack(false);

	m_bSingle = false;
	m_bDouble = false;
	m_bJump = false;

	return S_OK;
}

void CBastion_Sword_Attack::Play_Sound()
{
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	switch (m_eAttackType)
	{
	case Client::CBastion_Sword_Attack::ATTACK_TYPE::ATTACK_SINGLE:
		if (iCurKeyFrameIndex == m_iSingleAtt)
		{
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Sword1H_Attack_1);
			g_pGameInstance->Play_Shot(L"H1_Attack_1", CSoundMgr::CHANNELID::Sword1H_Attack_1);
		}
		break;
	case Client::CBastion_Sword_Attack::ATTACK_TYPE::ATTACK_DOUBLE:
		if (iCurKeyFrameIndex == m_iDoubleAtt)
		{
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Sword1H_Attack_1);
			g_pGameInstance->Play_Shot(L"H1_Attack_2", CSoundMgr::CHANNELID::Sword1H_Attack_1);
			m_bDouble = true;
		}
		break;
	case Client::CBastion_Sword_Attack::ATTACK_TYPE::ATTACK_JUMP:
		if (iCurKeyFrameIndex == m_iJumpAtt)
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
			if (iCurKeyFrameIndex2 == m_iDoubleAtt + 100)
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
