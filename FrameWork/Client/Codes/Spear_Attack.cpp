#include "pch.h"
#include "Spear_Attack.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Spear.h"

CSpear_Attack::CSpear_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Attack::CSpear_Attack(const CSpear_Attack& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Attack::Tick(const _double& _dDeltaTime)
{
	cout << "Attack" << endl;
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Tick(_dDeltaTime);

	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		m_pOwner->Set_Attack(false);
		m_pStateController->Change_State(L"Idle");
	}
	//CMonster_Bastion_Spear* pMonster = (CMonster_Bastion_Spear*)m_pStateController->Get_GameObject();
	//if (nullptr != pMonster)
	//	pMonster->Set_IsAttack(true);

	return _int();
}

_int CSpear_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	//m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_R1);
	//Play_Sound();

	return S_OK;
}

HRESULT CSpear_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_bAttack1=false;
	m_bAttack2=false;
	return S_OK;
}

HRESULT CSpear_Attack::EnterState(void* pArg)
{
	_uint iRand = (*(_uint*)pArg);

	switch (iRand)
	{
	case 0:
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_R1);
		break;
	case 1:
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_R2);
		break;
	}

	Play_Sound();

	return S_OK;
}

HRESULT CSpear_Attack::ExitState(void* pArg)
{
	m_bAttack1 = false;
	m_bAttack2 = false;

	return S_OK;
}

void CSpear_Attack::Look_Player(void)
{
	//CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	//if (pAnim->Is_Finished())
	//	m_pStateController->Change_State(L"Idle");
}

void CSpear_Attack::Look_Monster(void)
{

}

void CSpear_Attack::Play_Sound(void)
{
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	if (m_iAttack1Frame == iCurKeyFrameIndex)
	{
		if (!m_bAttack1)
		{
			g_pGameInstance->BlendSound(L"Spear_Swing", L"Spear_Attack_1", CSoundMgr::CHANNELID::Spear_Attack_1, CSoundMgr::CHANNELID::Sword1H_Attack_2);
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Spear_Attack_2, 0.02f);
			m_bAttack1 = true;
		}
	}
	if (m_bAttack1)
	{
		if (m_iAttack2Frame == iCurKeyFrameIndex)
		{
			if (!m_bAttack2)
			{
				g_pGameInstance->BlendSound(L"Spear_Roar", L"Spear_Attack_2", CSoundMgr::CHANNELID::Spear_Attack_1, CSoundMgr::CHANNELID::Sword1H_Attack_2);
				g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Spear_Attack_1, 0.3f);
				g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Spear_Attack_2, 0.2f);
				m_bAttack2 = true;
			}
		}
	}
}

CSpear_Attack* CSpear_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Attack* pInstance = new CSpear_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_Attack::Free()
{
	__super::Free();
}
