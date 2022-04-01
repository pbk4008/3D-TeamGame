#include "pch.h"
#include "Aberrant_Attack.h"

#include "Monster_EarthAberrant.h"
#include "Animation.h"

CAberrant_Attack::CAberrant_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CAberrant_Attack::CAberrant_Attack(const CAberrant_Attack& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Attack::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAberrant_Attack::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	CMonster_EarthAberrant* pMonster = (CMonster_EarthAberrant*)m_pStateController->Get_GameObject();

	if (nullptr != pMonster)
	{
		pMonster->Set_IsAttack(true);
	}

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
		m_pStateController->Change_State(L"Dash_Bwd");
	}
	return _int();
}

_int CAberrant_Attack::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAberrant_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;
}


HRESULT CAberrant_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_iAttackCount = 0;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

HRESULT CAberrant_Attack::EnterState(void* pArg)
{
	_uint iAttackType = (*(_uint*)pArg);

	if(iAttackType == 0)
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_EarthAberrant::MON_STATE::ATTACK_R1);
	else
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_EarthAberrant::MON_STATE::ATTACK_R2);

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	return S_OK;
}

void CAberrant_Attack::Look_Player(void)
{

}

CAberrant_Attack* CAberrant_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Attack* pInstance = new CAberrant_Attack(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Attack::Free()
{
	__super::Free();
}
