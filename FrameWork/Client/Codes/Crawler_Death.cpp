#include "pch.h"
#include "Crawler_Death.h"
#include "CameraShake.h"
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

	if(m_pMonster->Get_Dead() == false)
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	m_pAnimator->Tick(TimeDelta);

	//if (m_pAnimator->Get_AnimController()->Is_Finished())
	//{
	//	cout << "Á×À½" << endl;
	//}
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

	g_pGameInstance->BlendSound(L"Crawler_Death", L"Monster_Death", CSoundMgr::CHANNELID::Monster_Death, CSoundMgr::CHANNELID::MONSTER);
	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Monster_Death, 1.0f);

	m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::DEATH);
	m_pAnimator->Get_AnimController()->Set_MoveSpeed(15.f);
	//m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	g_pShakeManager->Shake(CShakeManager::ETemplate::MonsterDeath, m_pTransform->Get_State(CTransform::STATE_POSITION));

	g_pObserver->Set_Exp(10.f);

	return S_OK;
}

HRESULT CCrawler_Death::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Monster_Death);

	return S_OK;
}

void CCrawler_Death::Look_Player(void)
{

}

void CCrawler_Death::Look_Monster(void)
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
