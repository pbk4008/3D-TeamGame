#include "pch.h"
#include "Bastion_Healer_Chaser.h"
#include "Animation.h"
/* Monster List */
#include "Monster_Bastion_Healer.h"

CBastion_Healer_Chaser::CBastion_Healer_Chaser(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_Healer_State(_pDevice, _pDeviceContext)
{
}

CBastion_Healer_Chaser::CBastion_Healer_Chaser(const CBastion_Healer_Chaser& _rhs)
	: CBastion_Healer_State(_rhs)
{
}

HRESULT CBastion_Healer_Chaser::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Healer_Chaser::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_WALK_FWD);
	m_pAnimator->Tick(_dDeltaTime);

	if(m_bTargetOn)
		m_pTransform->Add_Velocity(m_pTransform->Chase_Pos(g_pObserver->Get_Transform(), _dDeltaTime));

	m_fChaserDelay -= _dDeltaTime;

	return _int();
}

_int CBastion_Healer_Chaser::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_Healer_Chaser::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_Healer_Chaser::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_fChaserDelay = 0.5f;
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_WALK_FWD_ST);

	return S_OK;
}

HRESULT CBastion_Healer_Chaser::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	return S_OK;
}

void CBastion_Healer_Chaser::Look_Player(void)
{
	if (false == m_bTargetOn)
	{
		if (0 > m_fChaserDelay)
			m_pStateController->Change_State(L"Chaser_End");
	}
}


void CBastion_Healer_Chaser::Look_Monster(void)
{
}

CBastion_Healer_Chaser* CBastion_Healer_Chaser::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Healer_Chaser* pInstance = new CBastion_Healer_Chaser(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Healer_Chaser Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Healer_Chaser::Free()
{
	__super::Free();
}
