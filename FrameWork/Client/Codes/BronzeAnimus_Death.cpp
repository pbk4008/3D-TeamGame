#include "pch.h"
#include "BronzeAnimus_Death.h"
#include "Animation.h"

/* Monster List */
#include "Monster_BronzeAnimus.h"

CBronzeAnimus_Death::CBronzeAnimus_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

CBronzeAnimus_Death::CBronzeAnimus_Death(const CBronzeAnimus_Death& _rhs)
	: CBronzeAnimus_State(_rhs)
{
}

HRESULT CBronzeAnimus_Death::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Death::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);


	return _int();
}

_int CBronzeAnimus_Death::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Death::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Death::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pShakeManager->Shake(CShakeManager::ETemplate::MonsterDeath, _float3(0.f, 0.f, 0.f));
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_DEATH);
	g_pMainApp->FreezeTime();

	return S_OK;
}

HRESULT CBronzeAnimus_Death::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBronzeAnimus_Death::Look_Player(void)
{

}

void CBronzeAnimus_Death::Look_Monster(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
	{
		static_cast<CMonster_BronzeAnimus*>(m_pMonster)->Set_Dead();
		static_cast<CMonster_BronzeAnimus*>(m_pMonster)->setActive(false);
	}
}

CBronzeAnimus_Death* CBronzeAnimus_Death::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Death* pInstance = new CBronzeAnimus_Death(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Death Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBronzeAnimus_Death::Free()
{
	__super::Free();
}
