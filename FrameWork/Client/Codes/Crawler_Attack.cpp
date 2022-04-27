#include "pch.h"
#include "Crawler_Attack.h"

#include "Monster_Crawler.h"
#include "Animation.h"

CCrawler_Attack::CCrawler_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CCrawler_Attack::CCrawler_Attack(const CCrawler_Attack& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CCrawler_Attack::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCrawler_Attack::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	if (m_iAttackFrame == iCurKeyFrameIndex)
	{
		if (!m_bPlay)
		{
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Monster_Attack_2);
			g_pGameInstance->Play_Shot(L"Crawler_Attack_3", CSoundMgr::CHANNELID::Monster_Attack_2);
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Monster_Attack_2, 0.5f);
			m_bPlay = true;
		}
	}

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	CMonster_Crawler* pMonster = (CMonster_Crawler*)m_pStateController->Get_GameObject();
	if (nullptr != pMonster)
	{
		if (20 < iCurKeyFrameIndex && 45 > iCurKeyFrameIndex)
			pMonster->Set_IsAttack(true);
		else 
			pMonster->Set_IsAttack(false);
	}
	
	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished() && m_pStateController->Get_PreState()->Get_Tag() != L"Idle")
	{
		m_pStateController->Change_State(L"Idle");
		m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::IDLE);
	}

	return _int();
}

_int CCrawler_Attack::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CCrawler_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrawler_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Monster_Attack);
	_int randAtt = rand() % 3;

	if(randAtt)
		g_pGameInstance->Play_Shot(L"Crawler_Attack_1", CSoundMgr::CHANNELID::Monster_Attack);
	else
		g_pGameInstance->Play_Shot(L"Crawler_Attack_2", CSoundMgr::CHANNELID::Monster_Attack);

	//m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::ATTACK_R1);

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CCrawler_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_bPlay = false;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CCrawler_Attack::Look_Player(void)
{

}

CCrawler_Attack* CCrawler_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CCrawler_Attack* pInstance = new CCrawler_Attack(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCrawler_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrawler_Attack::Free()
{
	__super::Free();
}
