#include "pch.h"
#include "Shooter_Idle.h"
#include "Monster_Bastion_Shooter.h"

CShooter_Idle::CShooter_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CShooter_Idle::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMMOVEDESC tDesc = (*(FSMMOVEDESC*)_pArg);

	m_wstrTag = tDesc.pName;
	m_pAnimator = tDesc.pAnimator;
	m_pTransform = tDesc.pTransform;
	//Safe_AddRef(m_pAnimator);
	//Safe_AddRef(m_pTransform);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShooter_Idle::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CShooter_Idle::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShooter_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShooter_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;
	
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::IDLE);

	return S_OK;
}

HRESULT CShooter_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	return S_OK;
}

HRESULT CShooter_Idle::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CShooter_Idle::ExitState(void* pArg)
{
	return S_OK;
}

CShooter_Idle* CShooter_Idle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Idle* pInstance = new CShooter_Idle(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShooter_Idle::Free()
{
	__super::Free();
}
