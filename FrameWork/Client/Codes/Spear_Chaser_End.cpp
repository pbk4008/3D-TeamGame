#include "pch.h"
#include "Spear_Chaser_End.h"
#include "Animation.h"
/* Monster List */
#include "Monster_Bastion_Spear.h"

CSpear_Chaser_End::CSpear_Chaser_End(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Chaser_End::CSpear_Chaser_End(const CSpear_Chaser_End& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Chaser_End::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Chaser_End::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CSpear_Chaser_End::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Chaser_End::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Chaser_End::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_WALK_FWD_ED);

	return S_OK;
}

HRESULT CSpear_Chaser_End::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	return S_OK;
}

void CSpear_Chaser_End::Look_Player(void)
{
}


void CSpear_Chaser_End::Look_Monster(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
		m_pStateController->Change_State(L"Idle");
}

CSpear_Chaser_End* CSpear_Chaser_End::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Chaser_End* pInstance = new CSpear_Chaser_End(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Chaser_End Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpear_Chaser_End::Free()
{
	__super::Free();
}
