#include "pch.h"
#include "Spear_Bwd_Dash.h"
#include "Monster_Bastion_Spear.h"
#include "Animation.h"

CSpear_Bwd_Dash::CSpear_Bwd_Dash(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Bwd_Dash::CSpear_Bwd_Dash(const CSpear_Bwd_Dash& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Bwd_Dash::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Bwd_Dash::Tick(const _double& _dDeltaTime)
{
 	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CSpear_Bwd_Dash::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Bwd_Dash::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Bwd_Dash::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_BWD_DASH)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Bwd_Dash::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CSpear_Bwd_Dash::Look_Player(void)
{
}

void CSpear_Bwd_Dash::Look_Monster(void)
{
	//if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	//	m_pStateController->Change_State(L"Charge_Attack");
}

CSpear_Bwd_Dash* CSpear_Bwd_Dash::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Bwd_Dash* pInstance = new CSpear_Bwd_Dash(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Bwd_Dash Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_Bwd_Dash::Free()
{
	__super::Free();
}
