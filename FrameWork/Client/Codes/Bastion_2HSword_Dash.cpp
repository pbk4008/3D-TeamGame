#include "pch.h"
#include "Bastion_2HSword_Dash.h"

/* Monster List */
#include "Monster_Bastion_2HSword.h"

CBastion_2HSword_Dash::CBastion_2HSword_Dash(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Dash::CBastion_2HSword_Dash(const CBastion_2HSword_Dash& _rhs)
	: CMonster_FSM(_rhs)
{
}

HRESULT CBastion_2HSword_Dash::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_Dash::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBastion_2HSword_Dash::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_2HSword_Dash::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_Dash::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_DASH_BWD);

	return S_OK;
}

HRESULT CBastion_2HSword_Dash::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBastion_2HSword_Dash::Look_Player(void)
{
	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (5.0f < fDistToPlayer && 15.0f > fDistToPlayer)
	{
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
		m_pStateController->Change_State(L"Chaser");
	}

}

CBastion_2HSword_Dash* CBastion_2HSword_Dash::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_Dash* pInstance = new CBastion_2HSword_Dash(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_Dash Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_2HSword_Dash::Free()
{
	__super::Free();
}
