#include "pch.h"
#include "Spear_Charge_Attack.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Spear.h"

CSpear_Charge_Attack::CSpear_Charge_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Charge_Attack::CSpear_Charge_Attack(const CSpear_Charge_Attack& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Charge_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Charge_Attack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Play_Sound();
	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CSpear_Charge_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Charge_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Charge_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_bChargeOn = true;
	if(!m_bAttack)
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_CHARGE_ST);

	return S_OK;
}

HRESULT CSpear_Charge_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CSpear_Charge_Attack::Look_Player(void)
{
	if (7.0f >= m_fDistance && m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		m_bAttack = true;
		m_pStateController->Change_State(L"Charge_Attack_End");
	}
}

void CSpear_Charge_Attack::Look_Monster(void)
{

}

void CSpear_Charge_Attack::Play_Sound()
{

}

CSpear_Charge_Attack* CSpear_Charge_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Charge_Attack* pInstance = new CSpear_Charge_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Charge_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_Charge_Attack::Free()
{
	__super::Free();
}
