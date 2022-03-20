#include "pch.h"
#include "Crawler_Attack.h"

#include "Monster_Crawler.h"
#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

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

	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (m_pAnimator->Get_AnimController()->Is_Finished() && 3 > m_iAttackCount)
	{
		++m_iAttackCount;
	}

	if (2 == m_iAttackCount)
	{
		m_pStateController->Change_State(L"Ricochet");
		
		cout << "리코챗으로변경" << endl;
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

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Change_AnyEntryAnimation(CMonster_Crawler::MON_STATE::ATTACK_R1);

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CCrawler_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_iAttackCount = 0;

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
