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

	if (m_pMonster->Get_Dead())
		return 0;

	m_pAnimator->Tick(_dDeltaTime);
	CMonster_Bastion_2HSword* pMonster = (CMonster_Bastion_2HSword*)m_pStateController->Get_GameObject();

	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		m_pStateController->Change_State(L"Chaser");
		pMonster->Set_IsAttack(false);
	}
	else
		pMonster->Set_IsAttack(true);


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
	
	if(!m_bFirstAttack)
		m_bFirstAttack = true;

	_int randAtt = rand() % 4;
	//_int randAtt = 0;
	switch (randAtt)
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

HRESULT CBastion_2HSword_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

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
