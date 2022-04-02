#include "pch.h"
#include "MidBoss_Idle.h"

#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

CMidBoss_Idle::CMidBoss_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CMidBoss_Idle::CMidBoss_Idle(const CMidBoss_Idle& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CMidBoss_Idle::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMidBoss_Idle::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_pAnimator->Tick(TimeDelta);
	
	if ( 15.f > fDistToPlayer)
	{
		m_pStateController->Change_State(L"Rage");
	}

	return _int();
}

_int CMidBoss_Idle::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMidBoss_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMidBoss_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	_vector vec = { 0.f, 1.f, 0.f,0.f };
	m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	
	return S_OK;
}

HRESULT CMidBoss_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CMidBoss_Idle::Look_Player(void)
{

}

CMidBoss_Idle* CMidBoss_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CMidBoss_Idle* pInstance = new CMidBoss_Idle(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMidBoss_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMidBoss_Idle::Free()
{
	__super::Free();
}
