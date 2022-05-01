#include "pch.h"
#include "Boss_Stagger_Right.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Stagger_Right::CBoss_Stagger_Right(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Stagger_Right::CBoss_Stagger_Right(const CBoss_Stagger_Right& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Stagger_Right::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Stagger_Right::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		static_cast<CBoss_Solaris*>(m_pMonster)->Set_Random_AttackAnim();
	}

	return _int();
}

_int CBoss_Stagger_Right::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Stagger_Right::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Stagger_Right::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	cout << "Stagger_Right" << endl;

	m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.0f);

	//g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//g_pGameInstance->Play_Shot(L"MidBoss_Roar", CSoundMgr::CHANNELID::MidBoss);
	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::STAGGER_RIGHT);

	STOP_SOUND(CHANNEL::Boss_Skill1);
	PLAY_SOUND(L"Intermission_Hit_01", CHANNEL::Boss_Skill1);

	return S_OK;
}

HRESULT CBoss_Stagger_Right::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CBoss_Stagger_Right::Look_Player(void)
{

}

CBoss_Stagger_Right* CBoss_Stagger_Right::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Stagger_Right* pInstance = new CBoss_Stagger_Right(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Stagger_Right Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Stagger_Right::Free()
{
	__super::Free();
}
