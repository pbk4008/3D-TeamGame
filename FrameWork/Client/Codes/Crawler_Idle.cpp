#include "pch.h"
#include "Crawler_Idle.h"
#include "Monster_Crawler.h"
#include "Animation.h"

CCrawler_Idle::CCrawler_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CCrawler_Idle::CCrawler_Idle(const CCrawler_Idle& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CCrawler_Idle::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCrawler_Idle::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	_float dist = MathUtils::Length(m_pTransform->Get_State(CTransform::STATE::STATE_POSITION), g_pObserver->Get_PlayerPos());

	m_timer += g_fDeltaTime;

	if (m_timer >= 0.5f)
	{
		if (2.f < dist && 5.f > dist)
		{
			m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
			m_pStateController->Change_State(L"Walk");
			m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::WALK_FWD);
			//cout << "°È±â·Î º¯°æ" << endl;
		}
		else if (2.f > dist)
		{

			m_pStateController->Change_State(L"Attack");
			m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::ATTACK_R1);
		}
		m_timer = 0.f;
	}


	return _int();
}

_int CCrawler_Idle::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CCrawler_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrawler_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));
	//m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::IDLE);

	return S_OK;
}

HRESULT CCrawler_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CCrawler_Idle::Look_Player(void)
{

}

CCrawler_Idle* CCrawler_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CCrawler_Idle* pInstance = new CCrawler_Idle(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCrawler_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrawler_Idle::Free()
{
	__super::Free();
}
