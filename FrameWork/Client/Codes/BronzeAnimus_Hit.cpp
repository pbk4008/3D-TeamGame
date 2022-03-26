#include "pch.h"
#include "BronzeAnimus_Hit.h"
#include "Animation.h"

/* Monster List */
#include "Monster_BronzeAnimus.h"

CBronzeAnimus_Hit::CBronzeAnimus_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

CBronzeAnimus_Hit::CBronzeAnimus_Hit(const CBronzeAnimus_Hit& _rhs)
	: CBronzeAnimus_State(_rhs)
{
}

HRESULT CBronzeAnimus_Hit::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Hit::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBronzeAnimus_Hit::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Hit::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Hit::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if(FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_FLINCH)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Hit::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBronzeAnimus_Hit::Look_Player(void)
{

}

void CBronzeAnimus_Hit::Look_Monster(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished() && 0 < m_pMonster->Get_CurrentHp())
		m_pStateController->Change_State(L"A_Idle_Battle");
}

CBronzeAnimus_Hit* CBronzeAnimus_Hit::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Hit* pInstance = new CBronzeAnimus_Hit(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Hit Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBronzeAnimus_Hit::Free()
{
	__super::Free();
}
