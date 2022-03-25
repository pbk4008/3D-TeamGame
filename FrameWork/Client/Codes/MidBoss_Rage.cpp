#include "pch.h"
#include "MidBoss_Rage.h"

#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

CMidBoss_Rage::CMidBoss_Rage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CMidBoss_Rage::CMidBoss_Rage(const CMidBoss_Rage& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CMidBoss_Rage::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMidBoss_Rage::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		m_pStateController->Change_State(L"BattleCry");
	}

	return _int();
}

_int CMidBoss_Rage::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMidBoss_Rage::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMidBoss_Rage::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::RAGE);


	return S_OK;
}

HRESULT CMidBoss_Rage::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CMidBoss_Rage::Look_Player(void)
{

}

CMidBoss_Rage* CMidBoss_Rage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CMidBoss_Rage* pInstance = new CMidBoss_Rage(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMidBoss_Rage Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMidBoss_Rage::Free()
{
	__super::Free();
}
