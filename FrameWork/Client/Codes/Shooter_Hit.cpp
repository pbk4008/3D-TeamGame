#include "pch.h"
#include "..\Headers\Shooter_Hit.h"
#include "Monster_Bastion_Shooter.h"

CShooter_Hit::CShooter_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
	, m_fCurHp(0)
{
}

HRESULT CShooter_Hit::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMACTORDESC tDesc = (*(FSMACTORDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_pStateController = tDesc.pController;
	m_wstrTag = tDesc.pName;
	m_pMonster = tDesc.pActor;

	//Safe_AddRef(m_pAnimator);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShooter_Hit::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;
	m_pAnimator->Tick(_dDeltaTime);


	if (m_fCurHp <= 0.f)
		m_pStateController->Change_State(L"Death");

	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Shooter::ANIM_TYPE::IDLE)
		m_pStateController->Change_State(L"Idle");

	return _int();
}

_int CShooter_Hit::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CShooter_Hit::Render()
{
	return S_OK;
}

HRESULT CShooter_Hit::EnterState()
{
	return S_OK;
}

HRESULT CShooter_Hit::ExitState()
{
	return S_OK;
}

HRESULT CShooter_Hit::EnterState(void* _pArg)
{
	HITDATA tData = (*(HITDATA*)_pArg);

	m_pAnimator->Change_AnyEntryAnimation(tData.iHitType);
	m_fCurHp = tData.fCurHp;

	//쏘다가 맞는애니메이션으로 들어오게되면 공격이 씹혀서 여기에 false처리 
	if (nullptr != m_pMonster)
	{
		static_cast<CMonster_Bastion_Shooter*>(m_pMonster)->Set_Shot(false);
	}

	return S_OK;
}

HRESULT CShooter_Hit::ExitState(void* _pArg)
{
	return S_OK;
}

CShooter_Hit* CShooter_Hit::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Hit* pInstance = new CShooter_Hit(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Hit Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShooter_Hit::Free()
{
	CMonster_FSM::Free();
}
