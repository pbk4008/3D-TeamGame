#include "pch.h"
#include "Bastion_2HSword_Attack_Rage.h"
#include "Animation.h"
/* Monster List */
#include "Monster_Bastion_2HSword.h"

CBastion_2HSword_Attack_Rage::CBastion_2HSword_Attack_Rage(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_2HSword_Rage(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Attack_Rage::CBastion_2HSword_Attack_Rage(const CBastion_2HSword_Attack_Rage& _rhs)
	: CBastion_2HSword_Rage(_rhs)
{
}

HRESULT CBastion_2HSword_Attack_Rage::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_Attack_Rage::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pTransform->Chase_Target(g_pObserver->m_pPlayerTrans, _dDeltaTime);
	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBastion_2HSword_Attack_Rage::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_2HSword_Attack_Rage::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_Attack_Rage::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_BATTLECRY_ED);

	return S_OK;
}

HRESULT CBastion_2HSword_Attack_Rage::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBastion_2HSword_Attack_Rage::Look_Player(void)
{
	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - XMLoadFloat3(&g_pObserver->m_fPos);
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (2.9f < fDistToPlayer && 15.0f > fDistToPlayer)
	{
		m_pTransform->Face_Target(XMLoadFloat3(&g_pObserver->m_fPos));
		m_pStateController->Change_State(L"Chaser");
	}

	if (TRUE == g_pObserver->m_bAttack)
	{
		m_pStateController->Change_State(L"Dash");
		dynamic_cast<CMonster_Bastion_2HSword*>(m_pMonster)->m_iHp -= 1;
	}
}

void CBastion_2HSword_Attack_Rage::Look_Monster(void)
{
}

CBastion_2HSword_Attack_Rage* CBastion_2HSword_Attack_Rage::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_Attack_Rage* pInstance = new CBastion_2HSword_Attack_Rage(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_Attack_Rage Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_2HSword_Attack_Rage::Free()
{
	__super::Free();
}
