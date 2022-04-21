#include "pch.h"
#include "Bastion_Sword_Excution.h"
#include "Monster_Bastion_Sword.h"

CBastion_Sword_Excution::CBastion_Sword_Excution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CBastion_Sword_Excution::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMACTORDESC tDesc = (*(FSMACTORDESC*)_pArg);

	m_pAnimator = tDesc.pAnimator;
	m_pMonster = tDesc.pActor;
	m_pTransform = m_pMonster->Get_Transform();
	m_wstrTag = tDesc.pName;

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Sword_Excution::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	m_pAnimator->Tick(_dDeltaTime);
	
	return 0;
}

_int CBastion_Sword_Excution::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CBastion_Sword_Excution::Render()
{
	return S_OK;
}

HRESULT CBastion_Sword_Excution::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::EXCUTION);

	return S_OK;
}

HRESULT CBastion_Sword_Excution::ExitState()
{
	return S_OK;
}

HRESULT CBastion_Sword_Excution::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CBastion_Sword_Excution::ExitState(void* pArg)
{
	return S_OK;
}

CBastion_Sword_Excution* CBastion_Sword_Excution::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Sword_Excution* pInstance = new CBastion_Sword_Excution(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Sword_Excution Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Sword_Excution::Free()
{
	CMonster_FSM::Free();
}
