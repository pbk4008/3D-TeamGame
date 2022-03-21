#include "pch.h"
#include "Bastion_Healer_Attack.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Healer.h"

CBastion_Healer_Attack::CBastion_Healer_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_Healer_State(_pDevice, _pDeviceContext)
{
}

CBastion_Healer_Attack::CBastion_Healer_Attack(const CBastion_Healer_Attack& _rhs)
	: CBastion_Healer_State(_rhs)
{
}

HRESULT CBastion_Healer_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Healer_Attack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBastion_Healer_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_Healer_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_Healer_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_ATTACK_BLIND);

	return S_OK;
}

HRESULT CBastion_Healer_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBastion_Healer_Attack::Look_Player(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
		m_pStateController->Change_State(L"Idle");
}

void CBastion_Healer_Attack::Look_Monster(void)
{

}

CBastion_Healer_Attack* CBastion_Healer_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Healer_Attack* pInstance = new CBastion_Healer_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Healer_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_Healer_Attack::Free()
{
	__super::Free();
}
