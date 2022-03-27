#include "pch.h"
#include "BronzeAnimus_Chaser.h"

/* Monster List */
#include "Monster_BronzeAnimus.h"

CBronzeAnimus_Chaser::CBronzeAnimus_Chaser(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

CBronzeAnimus_Chaser::CBronzeAnimus_Chaser(const CBronzeAnimus_Chaser& _rhs)
	: CBronzeAnimus_State(_rhs)
{
}

HRESULT CBronzeAnimus_Chaser::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Chaser::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	m_pTransform->Add_Velocity(m_pTransform->Chase_Pos(g_pObserver->Get_Transform(), _dDeltaTime));

	return _int();
}

_int CBronzeAnimus_Chaser::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Chaser::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Chaser::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_WALK_FWD_ST);

	return S_OK;
}

HRESULT CBronzeAnimus_Chaser::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	return S_OK;
}

void CBronzeAnimus_Chaser::Look_Player(void)
{
	if (!m_bTargetOn)
		m_pStateController->Change_State(L"A_Idle_Battle");
}

void CBronzeAnimus_Chaser::Look_Monster(void)
{
}

CBronzeAnimus_Chaser* CBronzeAnimus_Chaser::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Chaser* pInstance = new CBronzeAnimus_Chaser(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Chaser Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBronzeAnimus_Chaser::Free()
{
	__super::Free();
}
