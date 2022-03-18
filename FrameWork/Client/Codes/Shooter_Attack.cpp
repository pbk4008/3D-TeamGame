#include "pch.h"
#include "Shooter_Attack.h"
#include "Monster_Bastion_Shooter.h"

CShooter_Attack::CShooter_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CShooter_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShooter_Attack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CShooter_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();

}

HRESULT CShooter_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShooter_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_Animation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::ATTACK);

	return S_OK;
}

HRESULT CShooter_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimator->Change_Animation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::IDLE);

	return S_OK;
}

CShooter_Attack* CShooter_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Attack* pInstance = new CShooter_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShooter_Attack::Free()
{
	__super::Free();
}
