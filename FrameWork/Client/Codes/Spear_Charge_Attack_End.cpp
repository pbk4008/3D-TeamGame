#include "pch.h"
#include "Spear_Charge_Attack_End.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Spear.h"

CSpear_Charge_Attack_End::CSpear_Charge_Attack_End(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Charge_Attack_End::CSpear_Charge_Attack_End(const CSpear_Charge_Attack_End& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Charge_Attack_End::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Charge_Attack_End::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);
	m_fTime -= (_float)_dDeltaTime;

	return _int();
}

_int CSpear_Charge_Attack_End::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Charge_Attack_End::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Charge_Attack_End::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_CHARGE_ED);

	return S_OK;
}

HRESULT CSpear_Charge_Attack_End::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_bChargeOn = false;
	m_fTime = 5.0f;

	CMonster_Bastion_Spear* pMonster = (CMonster_Bastion_Spear*)m_pStateController->Get_GameObject();
	if (nullptr != pMonster)
		pMonster->Set_IsAttack(true);

	return S_OK;
}

void CSpear_Charge_Attack_End::Look_Player(void)
{
}

void CSpear_Charge_Attack_End::Look_Monster(void)
{
	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished()/* && 0.0f >= m_fTime*/)
		m_pStateController->Change_State(L"Idle");
}

CSpear_Charge_Attack_End* CSpear_Charge_Attack_End::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Charge_Attack_End* pInstance = new CSpear_Charge_Attack_End(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Charge_Attack_End Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_Charge_Attack_End::Free()
{
	__super::Free();
}
