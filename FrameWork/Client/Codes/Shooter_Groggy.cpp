#include "pch.h"
#include "..\Headers\Shooter_Groggy.h"
#include "Monster_Bastion_Shooter.h"

CShooter_Groggy::CShooter_Groggy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CShooter_Groggy::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMDESC tDesc = (*(FSMDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_pStateController = tDesc.pController;
	m_wstrTag = tDesc.pName;

	Safe_AddRef(m_pAnimator);


	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShooter_Groggy::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	m_pAnimator->Tick(_dDeltaTime);

	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Shooter::ANIM_TYPE::IDLE)
		m_pStateController->Change_State(L"Idle");

	return _int();
}

_int CShooter_Groggy::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CShooter_Groggy::Render()
{
	return S_OK;
}

HRESULT CShooter_Groggy::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::GROGGY_START);

	return S_OK;
}

HRESULT CShooter_Groggy::ExitState()
{
	return S_OK;
}

HRESULT CShooter_Groggy::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CShooter_Groggy::ExitState(void* pArg)
{
	return S_OK;
}

CShooter_Groggy* CShooter_Groggy::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Groggy* pInstance = new CShooter_Groggy(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Groggy Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShooter_Groggy::Free()
{
	CMonster_FSM::Free();
}
