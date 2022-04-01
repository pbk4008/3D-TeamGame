#include "pch.h"
#include "..\Headers\Bastion_Sword_Death.h"
#include "Animation.h"
#include "Monster_Bastion_Sword.h"

CBastion_Sword_Death::CBastion_Sword_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice,_pDeviceContext)
{
}

HRESULT CBastion_Sword_Death::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;


	FSMDESC tDesc = (*(FSMDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_wstrTag = tDesc.pName;

	//Safe_AddRef(m_pAnimator);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Sword_Death::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	m_pAnimator->Tick(_dDeltaTime);

	//if(m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	

	return _int();
}

_int CBastion_Sword_Death::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CBastion_Sword_Death::Render()
{
	return S_OK;
}

HRESULT CBastion_Sword_Death::EnterState()
{
	g_pGameInstance->BlendSound(L"Bastion_Death", L"Monster_Death", CSoundMgr::CHANNELID::Sword1H_Death, CSoundMgr::CHANNELID::MONSTER);
	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Sword1H_Death, 7.0f);
	g_pShakeManager->Shake(CShakeManager::ETemplate::MonsterDeath, m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::DEATH);
	g_pMainApp->FreezeTime();

	return S_OK;
}

HRESULT CBastion_Sword_Death::ExitState()
{
	return S_OK;
}

CBastion_Sword_Death* CBastion_Sword_Death::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Sword_Death* pInstance = new CBastion_Sword_Death(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Sword_Death Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Sword_Death::Free()
{
	CMonster_FSM::Free();
}
