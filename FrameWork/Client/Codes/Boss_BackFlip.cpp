#include "pch.h"
#include "Boss_BackFlip.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_BackFlip::CBoss_BackFlip(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_BackFlip::CBoss_BackFlip(const CBoss_BackFlip& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_BackFlip::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_BackFlip::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	if (29 < iCurKeyFrameIndex && 31 > iCurKeyFrameIndex)
	{
		STOP_SOUND(CHANNEL::Boss_Skill1);
		PLAY_SOUND(L"BeamMassacre_End_01", CHANNEL::Boss_Skill1);
	}

	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		static_cast<CBoss_Solaris*>(m_pMonster)->Set_Random_AttackAnim();
	}

	return _int();
}

_int CBoss_BackFlip::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_BackFlip::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_BackFlip::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	cout << "BackFlip" << endl;

	m_pAnimator->Get_AnimController()->Set_MoveSpeed(35.0f);
	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.5f);

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(true);

	//g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//g_pGameInstance->Play_Shot(L"MidBoss_Roar", CSoundMgr::CHANNELID::MidBoss);
	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::DASH_BACK_FLIP);

	STOP_SOUND(CHANNEL::Boss_Skill1);
	PLAY_SOUND(L"BeamMassacre_FlyUp_01", CHANNEL::Boss_Skill1);

	return S_OK;
}

HRESULT CBoss_BackFlip::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CBoss_BackFlip::Look_Player(void)
{

}

CBoss_BackFlip* CBoss_BackFlip::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_BackFlip* pInstance = new CBoss_BackFlip(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_BackFlip Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_BackFlip::Free()
{
	__super::Free();
}
