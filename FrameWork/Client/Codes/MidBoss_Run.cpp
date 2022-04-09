#include "pch.h"
#include "MidBoss_Run.h"

#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

CMidBoss_Run::CMidBoss_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CMidBoss_Run::CMidBoss_Run(const CMidBoss_Run& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CMidBoss_Run::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMidBoss_Run::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	//TODO : Attackµµ °Å¸® ¶È°°ÀÌ ¸ÂÃçÁà¾ßµÊ 
	if (6.f > fDistToPlayer )
	{
		m_pStateController->Change_State(L"Attack");
	}

	return _int();
}

_int CMidBoss_Run::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMidBoss_Run::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMidBoss_Run::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.f);

	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::JOG_FWD_START_H);
	
	//_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	//_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	//_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CMidBoss_Run::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CMidBoss_Run::Look_Player(void)
{
	
}

CMidBoss_Run* CMidBoss_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CMidBoss_Run* pInstance = new CMidBoss_Run(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMidBoss_Run Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMidBoss_Run::Free()
{
	__super::Free();
}
