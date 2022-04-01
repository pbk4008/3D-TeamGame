#include "pch.h"
#include "Spear_Idle.h"
#include "Monster_Bastion_Spear.h"
#include "Animation.h"

CSpear_Idle::CSpear_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Idle::CSpear_Idle(const CSpear_Idle& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Idle::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Idle::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	m_fChaserDelay -= _dDeltaTime;

	return _int();
}

_int CSpear_Idle::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_fChaserDelay = 0.5f;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CSpear_Idle::Look_Player(void)
{
	if (m_bTargetOn && 0 > m_fChaserDelay)
		m_pStateController->Change_State(L"Chaser");
}

void CSpear_Idle::Look_Monster(void)
{
	if (15 >= m_pMonster->Get_CurrentHp())
		m_pStateController->Change_State(L"Bwd_Dash");
}

CSpear_Idle* CSpear_Idle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Idle* pInstance = new CSpear_Idle(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_Idle::Free()
{
	__super::Free();
}
