#include "pch.h"
#include "Bastion_2HSword_Rage.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_2HSword.h"

CBastion_2HSword_Rage::CBastion_2HSword_Rage(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_2HSword_State(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Rage::CBastion_2HSword_Rage(const CBastion_2HSword_Rage& _rhs)
	: CBastion_2HSword_State(_rhs)
{
}

HRESULT CBastion_2HSword_Rage::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_Rage::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Tick(_dDeltaTime);
	cout << m_pAnimator->Get_CurrentAnimation()->Get_CurrentKeyFrameIndex() << endl;

	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_BATTLECRY)
	{
		CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();
		if (pAnim)
		{
			_uint iFrame = pAnim->Get_CurrentKeyFrameIndex();
			if (iFrame >= 150 && iFrame < 151)
				g_pGameInstance->Play_Shot(L"MidBoss_Roar_2", CHANNEL::Sword2H_Attack);
		}
		if (!m_pAnimator->Get_IsLerp())
		{
			if (m_pOwner->get_Attack())
			{
				m_pAnimator->Change_LoopAnim();
				m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.4f);
				m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.f);
			}
		}
		else
			m_pAnimator->Get_AnimController()->Set_MoveSpeed(60.f);
	}
	else if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_BATTLECRY_ED)
	{
		CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();
		if (pAnim)
		{
			_uint iFrame = pAnim->Get_CurrentKeyFrameIndex();
			if (iFrame >= 150 && iFrame < 151)
				g_pGameInstance->Play_Shot(L"MidBoss_Attack_End_3", CHANNEL::Sword2H_Attack);
		}
		if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
		{
			m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);
			m_pOwner->set_Attack(false);
			m_pOwner->set_RageOn(false);
			m_pOwner->set_RandAttack(-1);
		}
	}
	return _int();
}

_int CBastion_2HSword_Rage::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_2HSword_Rage::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_Rage::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_TAUNT_ROAR);

	return S_OK;
}

HRESULT CBastion_2HSword_Rage::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);
	return S_OK;
}

void CBastion_2HSword_Rage::Look_Player(void)
{
}

void CBastion_2HSword_Rage::Look_Monster(void)
{
	
}

CBastion_2HSword_Rage* CBastion_2HSword_Rage::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_Rage* pInstance = new CBastion_2HSword_Rage(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_Rage Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_2HSword_Rage::Free()
{
	__super::Free();
}
