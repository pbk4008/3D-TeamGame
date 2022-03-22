#include "pch.h"
#include "Aberrant_Walk.h"

#include "Monster_EarthAberrant.h"
#include "Animation.h"

CAberrant_Walk::CAberrant_Walk(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CAberrant_Walk::CAberrant_Walk(const CAberrant_Walk& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Walk::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAberrant_Walk::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (5.f < fDistToPlayer)
	{
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	}

	if (5.f > fDistToPlayer)
	{
		m_pStateController->Change_State(L"Attack");
		cout << "공격으로 변경" << endl;
	}

	return _int();
}

_int CAberrant_Walk::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Walk::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAberrant_Walk::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::WALK_FWD_START);

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CAberrant_Walk::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CAberrant_Walk::Look_Player(void)
{

}

CAberrant_Walk* CAberrant_Walk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Walk* pInstance = new CAberrant_Walk(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Walk Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Walk::Free()
{
	__super::Free();
}
