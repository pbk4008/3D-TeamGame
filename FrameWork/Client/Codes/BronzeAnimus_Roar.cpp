#include "pch.h"
#include "BronzeAnimus_Roar.h"
#include "Animation.h"

/* Monster List */
#include "Monster_BronzeAnimus.h"

CBronzeAnimus_Roar::CBronzeAnimus_Roar(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

CBronzeAnimus_Roar::CBronzeAnimus_Roar(const CBronzeAnimus_Roar& _rhs)
	: CBronzeAnimus_State(_rhs)
{
}

HRESULT CBronzeAnimus_Roar::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Roar::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if(!m_bBattleOn)
		m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBronzeAnimus_Roar::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Roar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Roar::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_ROAR);

	return S_OK;
}

HRESULT CBronzeAnimus_Roar::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_bBattleOn = true;

	return S_OK;
}

void CBronzeAnimus_Roar::Look_Player(void)
{

}

void CBronzeAnimus_Roar::Look_Monster(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
	{
		m_bBattleOn = true;
		m_pStateController->Change_State(L"Chaser");
	}
}

CBronzeAnimus_Roar* CBronzeAnimus_Roar::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Roar* pInstance = new CBronzeAnimus_Roar(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Roar Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBronzeAnimus_Roar::Free()
{
	__super::Free();
}
