#include "pch.h"
#include "..\Headers\Bastion_Sword_Walk.h"
#include "Monster_Bastion_Sword.h"

CBastion_Sword_Walk::CBastion_Sword_Walk(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CBastion_Sword_Walk::NativeConstruct(void* _pArg)
{
	if (!_pArg)
		return E_FAIL;

	FSMMOVEDESC tDesc = (*(FSMMOVEDESC*)_pArg);
	m_pAnimator = tDesc.pAnimator;
	m_pStateController = tDesc.pController;
	m_pTransform = tDesc.pTransform;

	Safe_AddRef(m_pAnimator);
	Safe_AddRef(m_pTransform);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;


	return S_OK;
}

_int CBastion_Sword_Walk::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	m_pAnimator->Tick(_dDeltaTime);
	return _int();
}

_int CBastion_Sword_Walk::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CBastion_Sword_Walk::Render()
{
	return E_NOTIMPL;
}

HRESULT CBastion_Sword_Walk::EnterState()
{
	if (!m_pAnimator)
		return E_FAIL;
	_uint iWalkDir = rand() % 4;
	
	switch (iWalkDir)
	{
	case 0: 
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::FORWARD_START);
		break;
	case 1:
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::BACKWARD_START);
		break;
	case 2:
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::LEFTWALK_START);
		break;
	case 3:
		m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::RIGHTWALK_START);
		break;
	}
	return S_OK;
}

HRESULT CBastion_Sword_Walk::ExitState()
{
	return E_NOTIMPL;
}

CBastion_Sword_Walk* CBastion_Sword_Walk::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	return nullptr;
}

void CBastion_Sword_Walk::Free()
{
}
