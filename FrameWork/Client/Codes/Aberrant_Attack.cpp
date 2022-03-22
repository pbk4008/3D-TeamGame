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

	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	/*if (m_pAnimator->Get_AnimController()->Is_Finished() && 3 > m_iAttackCount)
	{
		++m_iAttackCount;
	}

	if (2 == m_iAttackCount)
	{
		m_pStateController->Change_State(L"Walk");

		cout << "워크으로변경" << endl;
	}*/

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Walk");
		cout << "워크으로변경" << endl;
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

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	_int iRandom = rand() % 2;

	while (m_iPreState_1 == iRandom)
	{
		iRandom = rand() % 2;
	}

	if (m_iPreState_1 != iRandom)
	{
		switch (iRandom)
		{
		case 0:
			m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_EarthAberrant::MON_STATE::ATTACK_R1);
			break;
		case 1:
			m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_EarthAberrant::MON_STATE::ATTACK_R2);
			break;
		}

		m_iPreState_1 = iRandom;
	}

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
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
