#include "pch.h"
#include "Spear_Hit.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Spear.h"
#include "Stage2.h"

CSpear_Hit::CSpear_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Hit::CSpear_Hit(const CSpear_Hit& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Hit::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Hit::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (0 >= m_pMonster->Get_CurrentHp())
		m_pStateController->Change_State(L"Death");

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CSpear_Hit::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Hit::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Hit::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_FLINCH);

	return S_OK;
}

HRESULT CSpear_Hit::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CSpear_Hit::Look_Player(void)
{
}

void CSpear_Hit::Look_Monster(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished() && 0 < m_pMonster->Get_CurrentHp())
		m_pStateController->Change_State(L"Idle");

	else if (0 >= m_pMonster->Get_CurrentHp() && !m_pMonster->Get_Dead())
	{
		static_cast<CMonster_Bastion_Spear*>(m_pMonster)->Set_Dead();
		static_cast<CMonster_Bastion_Spear*>(m_pMonster)->Remove_Collider();

		CLevel* pLevel = g_pGameInstance->getCurrentLevelScene();
		if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE2)
			static_cast<CStage2*>(pLevel)->Minus_MonsterCount();

		m_pStateController->Change_State(L"Death");
	}

	if (15.0f >= m_pMonster->Get_CurrentHp())
		m_pStateController->Change_State(L"Bwd_Dash");
}

CSpear_Hit* CSpear_Hit::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Hit* pInstance = new CSpear_Hit(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Hit Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpear_Hit::Free()
{
	__super::Free();
}
