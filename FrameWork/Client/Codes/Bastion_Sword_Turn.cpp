#include "pch.h"
#include "Bastion_Sword_Turn.h"
#include "Monster_Bastion_Sword.h"

CBastion_Sword_Turn::CBastion_Sword_Turn(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CBastion_Sword_Turn::NativeConstruct(void* _pArg)
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

_int CBastion_Sword_Turn::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	m_pAnimator->Tick(_dDeltaTime);

	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Sword::ANIM_TYPE::IDLE)
		m_pStateController->Change_State(L"Idle");

	return _int();
}

_int CBastion_Sword_Turn::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CBastion_Sword_Turn::Render()
{
	return S_OK;
}

HRESULT CBastion_Sword_Turn::EnterState()
{
	if (!m_pAnimator)
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::TURN);

	return S_OK;
}

HRESULT CBastion_Sword_Turn::ExitState()
{

	return S_OK;
}

CBastion_Sword_Turn* CBastion_Sword_Turn::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Sword_Turn* pInstance = new CBastion_Sword_Turn(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Sword_Turn Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Sword_Turn::Free()
{
	CMonster_FSM::Free();
}
