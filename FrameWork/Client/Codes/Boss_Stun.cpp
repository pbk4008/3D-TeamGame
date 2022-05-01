#include "pch.h"
#include "Boss_Stun.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Stun::CBoss_Stun(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Stun::CBoss_Stun(const CBoss_Stun& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Stun::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Stun::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	//cout << iCurKeyFrameIndex << endl;

	if (CBoss_Solaris::M_BossAnimState::STUN_END == m_pAnimator->Get_CurrentAnimNode())
	{
		static_cast<CBoss_Solaris*>(m_pMonster)->OnEff_MeshShield(false);

		if (m_pAnimator->Get_AnimController()->Is_Finished())
		{
			m_pStateController->Change_State(L"Turn");
		}
	}

	return _int();
}

_int CBoss_Stun::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Stun::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Stun::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pMonster->Set_IsAttack(false);

	cout << "Boss Stun" << endl;

	static_cast<CBoss_Solaris*>(m_pMonster)->OnEff_MeshShield(true);

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(false);
	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::STUN_START);

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_ShieldDissolveOff();
	return S_OK;
}

HRESULT CBoss_Stun::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_ShieldDissolveOn();

	return S_OK;
}

void CBoss_Stun::Look_Player(void)
{
}

CBoss_Stun* CBoss_Stun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Stun* pInstance = new CBoss_Stun(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Stun Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Stun::Free()
{
	__super::Free();
}
