#include "pch.h"
#include "Spear_Attack.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Spear.h"

CSpear_Attack::CSpear_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Attack::CSpear_Attack(const CSpear_Attack& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Attack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	CMonster_Bastion_Spear* pMonster = (CMonster_Bastion_Spear*)m_pStateController->Get_GameObject();
	if (nullptr != pMonster)
		pMonster->Set_IsAttack(true);

	return _int();
}

_int CSpear_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_ATTACK_R1);

	return S_OK;
}

HRESULT CSpear_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CSpear_Attack::Look_Player(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
		m_pStateController->Change_State(L"Idle");
}

void CSpear_Attack::Look_Monster(void)
{

}

CSpear_Attack* CSpear_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Attack* pInstance = new CSpear_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_Attack::Free()
{
	__super::Free();
}
