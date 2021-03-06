#include "pch.h"
#include "MidBoss_Execution.h"

#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

CMidBoss_Execution::CMidBoss_Execution(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CMidBoss_Execution::CMidBoss_Execution(const CMidBoss_Execution& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CMidBoss_Execution::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMidBoss_Execution::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	/*if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		m_pStateController->Change_State(L"Death");
	}*/

	return _int();
}

_int CMidBoss_Execution::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMidBoss_Execution::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMidBoss_Execution::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->BlendSound(L"MidBoss_Death", L"Monster_Death", CSoundMgr::CHANNELID::MidBoss, CSoundMgr::CHANNELID::MONSTER);
	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::MidBoss, 5.0f);
	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	//_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	//_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	//_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_pAnimator->Change_AnyEntryAnimation(CBoss_Bastion_Judicator::EXECUTION);

	return S_OK;
}

HRESULT CMidBoss_Execution::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CMidBoss_Execution::Look_Player(void)
{

}

CMidBoss_Execution* CMidBoss_Execution::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CMidBoss_Execution* pInstance = new CMidBoss_Execution(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMidBoss_Execution Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMidBoss_Execution::Free()
{
	__super::Free();
}
