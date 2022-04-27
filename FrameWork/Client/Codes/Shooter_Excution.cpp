#include "pch.h"
#include "Shooter_Excution.h"
#include "Monster_Bastion_Shooter.h"

CShooter_Excution::CShooter_Excution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CShooter_Excution::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMACTORDESC tDesc = (*(FSMACTORDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_pMonster = tDesc.pActor;
	m_pTransform = m_pMonster->Get_Transform();
	m_wstrTag = tDesc.pName;

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShooter_Excution::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	m_pAnimator->Tick(_dDeltaTime);
	
	return 0;
}

_int CShooter_Excution::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CShooter_Excution::Render()
{
	return S_OK;
}

HRESULT CShooter_Excution::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::EXCUTION);
	g_pObserver->Set_Exp(10.f);

	return S_OK;
}

HRESULT CShooter_Excution::ExitState()
{
	return S_OK;
}

HRESULT CShooter_Excution::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CShooter_Excution::ExitState(void* pArg)
{
	return S_OK;
}

CShooter_Excution* CShooter_Excution::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Excution* pInstance = new CShooter_Excution(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Excution Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShooter_Excution::Free()
{
	CMonster_FSM::Free();
}
