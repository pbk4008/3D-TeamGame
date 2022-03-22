#include "pch.h"
#include "Aberrant_Dash_Bwd.h"

#include "Monster_EarthAberrant.h"
#include "Animation.h"

CAberrant_Dash_Bwd::CAberrant_Dash_Bwd(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CAberrant_Dash_Bwd::CAberrant_Dash_Bwd(const CAberrant_Dash_Bwd& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Dash_Bwd::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAberrant_Dash_Bwd::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	CAnimationController* con = m_pAnimator->Get_AnimController();
	m_pAnimator->Get_AnimController()->Set_MoveSpeed(20.f);

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Run");
		//cout << "달리기로 변경" << endl;
	}

	return _int();
}

_int CAberrant_Dash_Bwd::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Dash_Bwd::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAberrant_Dash_Bwd::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	_uint irandom = rand() % 5;

	switch (irandom)
	{
	case 0:
		m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::DASH_BWD);
		break;
	case 1:
		m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::DASH_BWD);
		break;
	case 2:
		m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::DASH_BWD);
		break;
	case 3:
		m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::DASH_LEFT);
		break;
	case 4:
		m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::DASH_RIGHT);
		break;
	}

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CAberrant_Dash_Bwd::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.f);


	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CAberrant_Dash_Bwd::Look_Player(void)
{

}

CAberrant_Dash_Bwd* CAberrant_Dash_Bwd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Dash_Bwd* pInstance = new CAberrant_Dash_Bwd(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Dash_Bwd Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Dash_Bwd::Free()
{
	__super::Free();
}
