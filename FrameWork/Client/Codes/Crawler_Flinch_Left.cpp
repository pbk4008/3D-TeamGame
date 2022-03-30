#include "pch.h"
#include "Crawler_Flinch_Left.h"

#include "Monster_Crawler.h"
#include "Animation.h"

CCrawler_Flinch_Left::CCrawler_Flinch_Left(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CCrawler_Flinch_Left::CCrawler_Flinch_Left(const CCrawler_Flinch_Left& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CCrawler_Flinch_Left::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCrawler_Flinch_Left::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Walk");
	}

	if (m_pMonster->Get_CurrentHp() <= 0)
		m_pStateController->Change_State(L"Death");

	return _int();
}

_int CCrawler_Flinch_Left::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CCrawler_Flinch_Left::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrawler_Flinch_Left::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;
	m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::FLINCH_LEFT);

	return S_OK;
}

HRESULT CCrawler_Flinch_Left::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CCrawler_Flinch_Left::Look_Player(void)
{

}

CCrawler_Flinch_Left* CCrawler_Flinch_Left::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CCrawler_Flinch_Left* pInstance = new CCrawler_Flinch_Left(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCrawler_Flinch_Left Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrawler_Flinch_Left::Free()
{
	__super::Free();
}
