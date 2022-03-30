#include "pch.h"
#include "Bastion_2HSword_Death.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_2HSword.h"

CBastion_2HSword_Death::CBastion_2HSword_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_2HSword_State(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Death::CBastion_2HSword_Death(const CBastion_2HSword_Death& _rhs)
	: CBastion_2HSword_State(_rhs)
{
}

HRESULT CBastion_2HSword_Death::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_Death::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);


	return _int();
}

_int CBastion_2HSword_Death::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_2HSword_Death::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_Death::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_DEATH);

	g_pMainApp->FreezeTime();
	return S_OK;
}

HRESULT CBastion_2HSword_Death::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBastion_2HSword_Death::Look_Player(void)
{

}

void CBastion_2HSword_Death::Look_Monster(void)
{
	//CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	//if (pAnim->Is_Finished())
	//{
	//	static_cast<CMonster_Bastion_2HSword*>(m_pMonster)->Set_Dead();
	//	static_cast<CMonster_Bastion_2HSword*>(m_pMonster)->setActive(false);
	//}
}

CBastion_2HSword_Death* CBastion_2HSword_Death::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_Death* pInstance = new CBastion_2HSword_Death(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_Death Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_2HSword_Death::Free()
{
	__super::Free();
}
