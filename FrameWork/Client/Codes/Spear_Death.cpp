#include "pch.h"
#include "Spear_Death.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Spear.h"

CSpear_Death::CSpear_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Death::CSpear_Death(const CSpear_Death& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Death::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Death::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if(!m_bDeath)
		m_pAnimator->Tick(_dDeltaTime);


	return _int();
}

_int CSpear_Death::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Death::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Death::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pShakeManager->Shake(CShakeManager::ETemplate::MonsterDeath, _float3(0.f, 0.f, 0.f));
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_DEATH);
	g_pMainApp->FreezeTime();

	return S_OK;
}

HRESULT CSpear_Death::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CSpear_Death::Look_Player(void)
{

}

void CSpear_Death::Look_Monster(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
	{
		m_bDeath = true;
		static_cast<CMonster_Bastion_Spear*>(m_pMonster)->Set_Dead();
		static_cast<CMonster_Bastion_Spear*>(m_pMonster)->setActive(false);
	}
}

CSpear_Death* CSpear_Death::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Death* pInstance = new CSpear_Death(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Death Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_Death::Free()
{
	__super::Free();
}
