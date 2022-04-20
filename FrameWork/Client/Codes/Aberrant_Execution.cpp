#include "pch.h"
#include "Aberrant_Execution.h"

#include "Monster_EarthAberrant.h"
#include "Animation.h"

CAberrant_Execution::CAberrant_Execution(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CAberrant_Execution::CAberrant_Execution(const CAberrant_Execution& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Execution::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAberrant_Execution::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	return _int();
}

_int CAberrant_Execution::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Execution::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAberrant_Execution::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Earth_Death); //처형소리로 변경해야함 
	g_pGameInstance->Play_Shot(L"Earth_Death", CSoundMgr::CHANNELID::Earth_Death);
	//g_pShakeManager->Shake(CShakeManager::ETemplate::MonsterDeath, m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::ATTACK_EXECUTION);

	m_pMonster->Execution();

	//g_pMainApp->FreezeTime();
	return S_OK;
}

HRESULT CAberrant_Execution::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CAberrant_Execution::Look_Player(void)
{

}

CAberrant_Execution* CAberrant_Execution::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Execution* pInstance = new CAberrant_Execution(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Execution Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Execution::Free()
{
	__super::Free();
}
