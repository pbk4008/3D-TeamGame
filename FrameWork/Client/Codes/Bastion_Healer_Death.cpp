#include "pch.h"
#include "Bastion_Healer_Death.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Healer.h"

CBastion_Healer_Death::CBastion_Healer_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_Healer_State(_pDevice, _pDeviceContext)
{
}

CBastion_Healer_Death::CBastion_Healer_Death(const CBastion_Healer_Death& _rhs)
	: CBastion_Healer_State(_rhs)
{
}

HRESULT CBastion_Healer_Death::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Healer_Death::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);


	return _int();
}

_int CBastion_Healer_Death::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_Healer_Death::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_Healer_Death::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_DEATH);

	return S_OK;
}

HRESULT CBastion_Healer_Death::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBastion_Healer_Death::Look_Player(void)
{

}

void CBastion_Healer_Death::Look_Monster(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
	{
		static_cast<CMonster_Bastion_Healer*>(m_pMonster)->Set_Dead();
		static_cast<CMonster_Bastion_Healer*>(m_pMonster)->setActive(false);
	}
}

CBastion_Healer_Death* CBastion_Healer_Death::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Healer_Death* pInstance = new CBastion_Healer_Death(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Healer_Death Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_Healer_Death::Free()
{
	__super::Free();
}
