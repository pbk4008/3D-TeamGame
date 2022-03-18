#include "pch.h"
#include "Bastion_2HSword_Attack.h"

/* Monster List */
#include "Monster_Bastion_2HSword.h"

CBastion_2HSword_Attack::CBastion_2HSword_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Attack::CBastion_2HSword_Attack(const CBastion_2HSword_Attack& _rhs)
	: CMonster_FSM(_rhs)
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

	m_pAnimator->Tick(_dDeltaTime);

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

	_int randAtt = rand() % 4;

	switch (randAtt)
	{
	case 0:
		if (FAILED(m_pAnimator->Change_Animation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_R1)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(m_pAnimator->Change_Animation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_R2)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(m_pAnimator->Change_Animation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_S1)))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(m_pAnimator->Change_Animation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_ATTACK_S3)))
			return E_FAIL;
		break;
	}


	return S_OK;
}

HRESULT CBastion_2HSword_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBastion_2HSword_Attack::Look_Player(void)
{
	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (5.0f < fDistToPlayer && 15.0f > fDistToPlayer)
	{
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
		m_pStateController->Change_State(L"Chaser");
	}

	/*if (TRUE == g_pObserver->m_bAttack)
		m_pStateController->Change_State(L"Dash");*/
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
