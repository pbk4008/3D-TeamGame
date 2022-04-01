#include "pch.h"
#include "..\Headers\Shooter_Death.h"
#include "Animation.h"
#include "Monster_Bastion_Shooter.h"

CShooter_Death::CShooter_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CShooter_Death::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMMOVEDESC tDesc = (*(FSMMOVEDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_pTransform = tDesc.pTransform;
	m_wstrTag = tDesc.pName;

	//Safe_AddRef(m_pAnimator);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShooter_Death::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	m_pAnimator->Tick(_dDeltaTime);

	return  0;
}

_int CShooter_Death::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CShooter_Death::Render()
{
	return S_OK;
}

HRESULT CShooter_Death::EnterState()
{
	g_pGameInstance->BlendSound(L"Bastion_Death", L"Monster_Death", CSoundMgr::CHANNELID::Shooter_Death, CSoundMgr::CHANNELID::MONSTER);
	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Shooter_Death, 7.0f);

	g_pShakeManager->Shake(CShakeManager::ETemplate::MonsterDeath, m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::DEATH);
	g_pMainApp->FreezeTime();

	return S_OK;
}

HRESULT CShooter_Death::ExitState()
{
	return S_OK;
}

HRESULT CShooter_Death::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CShooter_Death::ExitState(void* pArg)
{
	return S_OK;
}

CShooter_Death* CShooter_Death::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Death* pInstance = new CShooter_Death(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Death Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShooter_Death::Free()
{
	CMonster_FSM::Free();
}
