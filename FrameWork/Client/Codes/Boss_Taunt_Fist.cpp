#include "pch.h"
#include "Boss_Taunt_Fist.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Taunt_Fist::CBoss_Taunt_Fist(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Taunt_Fist::CBoss_Taunt_Fist(const CBoss_Taunt_Fist& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Taunt_Fist::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Taunt_Fist::Tick(const _double& TimeDelta)
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

_int CBoss_Taunt_Fist::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Taunt_Fist::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Taunt_Fist::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	cout << "Taunt_Fist" << endl;

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(true);
	m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.0f);

	//g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//g_pGameInstance->Play_Shot(L"MidBoss_Roar", CSoundMgr::CHANNELID::MidBoss);
	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TAUNT_FIST);

	return S_OK;
}

HRESULT CBoss_Taunt_Fist::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CBoss_Taunt_Fist::Look_Player(void)
{

}

CBoss_Taunt_Fist* CBoss_Taunt_Fist::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Taunt_Fist* pInstance = new CBoss_Taunt_Fist(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Taunt_Fist Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Taunt_Fist::Free()
{
	__super::Free();
}
