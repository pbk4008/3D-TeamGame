#include "pch.h"
#include "..\Headers\Bastion_Sword_Attack.h"
#include "Monster_Bastion_Sword.h"

CBastion_Sword_Attack::CBastion_Sword_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice,_pDeviceContext)
{
}

HRESULT CBastion_Sword_Attack::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMDESC tDesc = (*(FSMDESC*)_pArg);
	m_pAnimator = tDesc.pAnimator;
	m_pStateController = tDesc.pController;

	Safe_AddRef(m_pAnimator);
	Safe_AddRef(m_pStateController);


	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Sword_Attack::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

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

HRESULT CBastion_Sword_Attack::EnterState()
{
	_uint iAttackType = rand() % 3;
	switch (iAttackType)
	{
	case 0:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_SINGLE)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_DOUBLE)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::ATTACK_JUMPSTART)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CBastion_Sword_Attack::ExitState()
{
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
