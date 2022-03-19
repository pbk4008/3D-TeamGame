#include "pch.h"
#include "..\Headers\Bastion_Sword_Paring.h"
#include "Monster_Bastion_Sword.h"

CBastion_Sword_Paring::CBastion_Sword_Paring(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CBastion_Sword_Paring::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMDESC tDesc = (*(FSMDESC*)_pArg);
	m_pAnimator = tDesc.pAnimator;
	m_pStateController = tDesc.pController;
	m_wstrTag = tDesc.pName;
	Safe_AddRef(m_pAnimator);


	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Sword_Paring::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		m_pAnimator->Tick(_dDeltaTime);

	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Sword::ANIM_TYPE::IDLE)
		m_pStateController->Change_State(L"Idle");

	return _int();
}

_int CBastion_Sword_Paring::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CBastion_Sword_Paring::Render()
{
	return S_OK;
}

HRESULT CBastion_Sword_Paring::EnterState()
{
	if (!m_pAnimator)
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::PARING);

	return S_OK;
}

HRESULT CBastion_Sword_Paring::ExitState()
{
	return S_OK;
}

CBastion_Sword_Paring* CBastion_Sword_Paring::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Sword_Paring* pInstance = new CBastion_Sword_Paring(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Sword_Paring Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CBastion_Sword_Paring::Free()
{
	CMonster_FSM::Free();
}
