#include "pch.h"
#include "Bastion_2HSword_Hit.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_2HSword.h"

CBastion_2HSword_Hit::CBastion_2HSword_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_2HSword_State(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Hit::CBastion_2HSword_Hit(const CBastion_2HSword_Hit& _rhs)
	: CBastion_2HSword_State(_rhs)
{
}

HRESULT CBastion_2HSword_Hit::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_Hit::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	//m_pTransform->Add_Velocity(XMLoadFloat3(&m_vDir) * (_float)_dDeltaTime);
	m_pTransform->Add_Velocity(CTransform::STATE_LOOK, -(_float)_dDeltaTime);

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBastion_2HSword_Hit::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_2HSword_Hit::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_Hit::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_FLINCH_LEFT);
	return S_OK;
}

HRESULT CBastion_2HSword_Hit::EnterState(void* _pArg)
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (_pArg)
	{
		_vector svDir = *(_vector*)_pArg;
		XMStoreFloat3(&m_vDir, svDir);
	}


	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_FLINCH_LEFT);
	return S_OK;
}

HRESULT CBastion_2HSword_Hit::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBastion_2HSword_Hit::Look_Player(void)
{

}

void CBastion_2HSword_Hit::Look_Monster(void)
{
	//CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	//if (pAnim->Is_Finished() && 0 < m_pMonster->Get_CurrentHp())
	//	m_pStateController->Change_State(L"Idle");
}

CBastion_2HSword_Hit* CBastion_2HSword_Hit::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_Hit* pInstance = new CBastion_2HSword_Hit(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_Hit Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_2HSword_Hit::Free()
{
	__super::Free();
}
