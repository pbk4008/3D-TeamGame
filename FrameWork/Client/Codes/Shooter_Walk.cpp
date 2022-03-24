#include "pch.h"
#include "..\Headers\Shooter_Walk.h"
#include "Monster_Bastion_Shooter.h"

CShooter_Walk::CShooter_Walk(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CShooter_Walk::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMMOVEDESC tDesc = (*(FSMMOVEDESC*)_pArg);
	m_pAnimator = tDesc.pAnimator;
	m_pStateController = tDesc.pController;
	m_pTransform = tDesc.pTransform;
	m_wstrTag = tDesc.pName;

	Safe_AddRef(m_pAnimator);
	Safe_AddRef(m_pTransform);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShooter_Walk::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CShooter_Walk::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CShooter_Walk::Render()
{
	return S_OK;
}

HRESULT CShooter_Walk::EnterState()
{
	if (!m_pAnimator)
		return E_FAIL;
	_uint iWalkDir = rand() % 4;

	switch (iWalkDir)
	{
	case 0:
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::FORWARD_START);
		break;
	case 1:
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::BACKWARD_START);
		break;
	case 2:
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::LEFTWALK_START);
		break;
	case 3:
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::RIGHTWALK_START);
		break;
	}
	return S_OK;
}

HRESULT CShooter_Walk::ExitState()
{
	return S_OK;
}

HRESULT CShooter_Walk::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CShooter_Walk::ExitState(void* pArg)
{
	return S_OK;
}

CShooter_Walk* CShooter_Walk::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Walk* pInstance = new CShooter_Walk(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Walk Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShooter_Walk::Free()
{
	CMonster_FSM::Free();
}
