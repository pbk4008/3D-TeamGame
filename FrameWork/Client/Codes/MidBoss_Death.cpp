#include "pch.h"
#include "MidBoss_Death.h"

#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

CMidBoss_Death::CMidBoss_Death(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CMidBoss_Death::CMidBoss_Death(const CMidBoss_Death& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CMidBoss_Death::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMidBoss_Death::Tick(const _double& TimeDelta)
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

_int CMidBoss_Death::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMidBoss_Death::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMidBoss_Death::EnterState()
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

	m_pAnimator->Change_AnyEntryAnimation(CBoss_Bastion_Judicator::DEATH);
	g_pObserver->Set_Exp(20.f);

	return S_OK;
}

HRESULT CMidBoss_Death::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CMidBoss_Death::Look_Player(void)
{

}

CMidBoss_Death* CMidBoss_Death::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CMidBoss_Death* pInstance = new CMidBoss_Death(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMidBoss_Death Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMidBoss_Death::Free()
{
	__super::Free();
}
