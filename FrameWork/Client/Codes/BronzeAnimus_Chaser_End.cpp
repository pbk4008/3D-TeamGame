#include "pch.h"
#include "BronzeAnimus_Chaser_End.h"
#include "Animation.h"
/* Monster List */
#include "Monster_BronzeAnimus.h"

CBronzeAnimus_Chaser_End::CBronzeAnimus_Chaser_End(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

CBronzeAnimus_Chaser_End::CBronzeAnimus_Chaser_End(const CBronzeAnimus_Chaser_End& _rhs)
	: CBronzeAnimus_State(_rhs)
{
}

HRESULT CBronzeAnimus_Chaser_End::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Chaser_End::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBronzeAnimus_Chaser_End::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Chaser_End::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Chaser_End::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_WALK_FWD_ED);

	return S_OK;
}

HRESULT CBronzeAnimus_Chaser_End::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	return S_OK;
}

void CBronzeAnimus_Chaser_End::Look_Player(void)
{
}


void CBronzeAnimus_Chaser_End::Look_Monster(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
		m_pStateController->Change_State(L"A_Idle_Battle");
}

CBronzeAnimus_Chaser_End* CBronzeAnimus_Chaser_End::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Chaser_End* pInstance = new CBronzeAnimus_Chaser_End(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Chaser_End Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBronzeAnimus_Chaser_End::Free()
{
	__super::Free();
}
