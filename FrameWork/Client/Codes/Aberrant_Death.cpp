#include "pch.h"
#include "Aberrant_Death.h"

#include "Monster_EarthAberrant.h"
#include "Animation.h"

CAberrant_Death::CAberrant_Death(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CAberrant_Death::CAberrant_Death(const CAberrant_Death& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Death::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAberrant_Death::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	return _int();
}

_int CAberrant_Death::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Death::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAberrant_Death::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Earth_Death);
	g_pGameInstance->Play_Shot(L"Earth_Death", CSoundMgr::CHANNELID::Earth_Death);
	g_pShakeManager->Shake(CShakeManager::ETemplate::MonsterDeath, m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::DEATH);

	g_pObserver->Set_Exp(2.f);

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CAberrant_Death::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;


	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CAberrant_Death::Look_Player(void)
{

}

CAberrant_Death* CAberrant_Death::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Death* pInstance = new CAberrant_Death(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Death Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Death::Free()
{
	__super::Free();
}
