#include "pch.h"
#include "Aberrant_Flinch_Left.h"

#include "Monster_EarthAberrant.h"
#include "Animation.h"

CAberrant_Flinch_Left::CAberrant_Flinch_Left(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CAberrant_Flinch_Left::CAberrant_Flinch_Left(const CAberrant_Flinch_Left& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Flinch_Left::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAberrant_Flinch_Left::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Run");
	}

	return _int();
}

_int CAberrant_Flinch_Left::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Flinch_Left::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAberrant_Flinch_Left::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::FLINCH_LEFT);


	return S_OK;
}

HRESULT CAberrant_Flinch_Left::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;


	return S_OK;
}

void CAberrant_Flinch_Left::Look_Player(void)
{

}

CAberrant_Flinch_Left* CAberrant_Flinch_Left::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Flinch_Left* pInstance = new CAberrant_Flinch_Left(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Flinch_Left Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Flinch_Left::Free()
{
	__super::Free();
}
