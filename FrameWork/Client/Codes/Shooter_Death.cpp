#include "pch.h"
#include "..\Headers\Shooter_Death.h"
#include "Animation.h"
#include "Monster_Bastion_Shooter.h"

CShooter_Death::CShooter_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CShooter_Death::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMDESC tDesc = (*(FSMDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_wstrTag = tDesc.pName;

	Safe_AddRef(m_pAnimator);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShooter_Death::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	m_pAnimator->Tick(_dDeltaTime);

	return  0;
}

_int CShooter_Death::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CShooter_Death::Render()
{
	return S_OK;
}

HRESULT CShooter_Death::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::DEATH);

	return S_OK;
}

HRESULT CShooter_Death::ExitState()
{
	return S_OK;
}

HRESULT CShooter_Death::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CShooter_Death::ExitState(void* pArg)
{
	return S_OK;
}

CShooter_Death* CShooter_Death::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Death* pInstance = new CShooter_Death(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Death Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShooter_Death::Free()
{
	CMonster_FSM::Free();
}
