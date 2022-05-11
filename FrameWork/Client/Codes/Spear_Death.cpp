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

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_DEATH);

	g_pGameInstance->BlendSound(L"Bastion_Death", L"Monster_Death", CSoundMgr::CHANNELID::Spear_Death, CSoundMgr::CHANNELID::MONSTER);
	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Spear_Death, 7.0f);
	g_pShakeManager->Shake(CShakeManager::ETemplate::MonsterDeath, m_pTransform->Get_State(CTransform::STATE_POSITION));
	g_pMainApp->FreezeTime();

	g_pObserver->Set_Exp(2.f);
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
