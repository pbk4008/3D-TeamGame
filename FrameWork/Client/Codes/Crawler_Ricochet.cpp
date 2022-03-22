#include "pch.h"
#include "Crawler_Ricochet.h"

#include "Monster_Crawler.h"
#include "Animation.h"

CCrawler_Ricochet::CCrawler_Ricochet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CCrawler_Ricochet::CCrawler_Ricochet(const CCrawler_Ricochet& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CCrawler_Ricochet::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCrawler_Ricochet::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Walk");
	}

	return _int();
}

_int CCrawler_Ricochet::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CCrawler_Ricochet::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrawler_Ricochet::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::RICOCHET);


	return S_OK;
}

HRESULT CCrawler_Ricochet::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;


	return S_OK;
}

void CCrawler_Ricochet::Look_Player(void)
{

}

CCrawler_Ricochet* CCrawler_Ricochet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CCrawler_Ricochet* pInstance = new CCrawler_Ricochet(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCrawler_Ricochet Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrawler_Ricochet::Free()
{
	__super::Free();
}
