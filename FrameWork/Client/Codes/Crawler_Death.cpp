#include "pch.h"
#include "Crawler_Death.h"

#include "Monster_Crawler.h"
#include "Animation.h"

CCrawler_Death::CCrawler_Death(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CCrawler_Death::CCrawler_Death(const CCrawler_Death& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CCrawler_Death::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCrawler_Death::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	return _int();
}

_int CCrawler_Death::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CCrawler_Death::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrawler_Death::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	//m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::DEATH);
	g_pMainApp->FreezeTime();
	return S_OK;
}

HRESULT CCrawler_Death::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CCrawler_Death::Look_Player(void)
{

}

CCrawler_Death* CCrawler_Death::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CCrawler_Death* pInstance = new CCrawler_Death(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCrawler_Death Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrawler_Death::Free()
{
	__super::Free();
}
