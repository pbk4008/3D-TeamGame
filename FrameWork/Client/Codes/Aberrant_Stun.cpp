#include "pch.h"
#include "Aberrant_Stun.h"

#include "Monster_EarthAberrant.h"
#include "Animation.h"

CAberrant_Stun::CAberrant_Stun(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CAberrant_Stun::CAberrant_Stun(const CAberrant_Stun& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Stun::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAberrant_Stun::Tick(const _double& TimeDelta)
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

_int CAberrant_Stun::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Stun::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAberrant_Stun::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::STUN_START);

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CAberrant_Stun::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CAberrant_Stun::Look_Player(void)
{

}

CAberrant_Stun* CAberrant_Stun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Stun* pInstance = new CAberrant_Stun(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Stun Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Stun::Free()
{
	__super::Free();
}
