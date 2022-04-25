#include "pch.h"
#include "Boss_Flinch_Right.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Flinch_Right::CBoss_Flinch_Right(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Flinch_Right::CBoss_Flinch_Right(const CBoss_Flinch_Right& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Flinch_Right::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Flinch_Right::Tick(const _double& TimeDelta)
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

_int CBoss_Flinch_Right::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Flinch_Right::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Flinch_Right::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	cout << "Flinch_Right" << endl;

	m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.0f);

	//g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//g_pGameInstance->Play_Shot(L"MidBoss_Roar", CSoundMgr::CHANNELID::MidBoss);
	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::FLINCH_RIGHT);

	return S_OK;
}

HRESULT CBoss_Flinch_Right::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CBoss_Flinch_Right::Look_Player(void)
{

}

CBoss_Flinch_Right* CBoss_Flinch_Right::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Flinch_Right* pInstance = new CBoss_Flinch_Right(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Flinch_Right Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Flinch_Right::Free()
{
	__super::Free();
}
