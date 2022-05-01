#include "pch.h"
#include "Bastion_Sword_Hit.h"
#include	"Monster_Bastion_Sword.h"
#include "Animation.h"

CBastion_Sword_Hit::CBastion_Sword_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
	, m_fCurHp(0)
{
}

HRESULT CBastion_Sword_Hit::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMDESC tDesc = (*(FSMDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_pStateController = tDesc.pController;
	m_wstrTag = tDesc.pName;

	//Safe_AddRef(m_pAnimator);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Sword_Hit::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;
	m_pAnimator->Tick(_dDeltaTime);

	if (m_pAnimator->Get_CurrentAnimation())
	{
		if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
			m_pStateController->Change_State(L"Idle");
	}

	return _int();
}

_int CBastion_Sword_Hit::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CBastion_Sword_Hit::Render()
{
	return S_OK;
}

HRESULT CBastion_Sword_Hit::EnterState()
{
	return S_OK;
}

HRESULT CBastion_Sword_Hit::ExitState()
{
	return S_OK;
}

HRESULT CBastion_Sword_Hit::EnterState(void* _pArg)
{
	HITDATA tData = (*(HITDATA*)_pArg);

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Sword1H_Hit);
	g_pGameInstance->Play_Shot(L"Monster_Hit_6", CSoundMgr::CHANNELID::Sword1H_Hit);

	m_pAnimator->Change_AnyEntryAnimation(tData.iHitType);
	m_fCurHp = tData.fCurHp;

	return S_OK;
}

HRESULT CBastion_Sword_Hit::ExitState(void* _pArg)
{
	return S_OK;
}

CBastion_Sword_Hit* CBastion_Sword_Hit::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Sword_Hit* pInstance = new CBastion_Sword_Hit(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Sword_Hit Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Sword_Hit::Free()
{
	CMonster_FSM::Free();
}
