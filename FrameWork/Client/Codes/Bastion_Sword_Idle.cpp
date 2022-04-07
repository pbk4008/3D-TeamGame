#include "pch.h"
#include "Bastion_Sword_Idle.h"
#include "Monster_Bastion_Sword.h"
#include "Animator.h"

CBastion_Sword_Idle::CBastion_Sword_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

HRESULT CBastion_Sword_Idle::NativeConstruct(void* _pArg) 
{
	//애니메이터 무조건 받아오게 끔
	if (!_pArg)
		return E_FAIL;


	FSMMOVEDESC tDesc = (*(FSMMOVEDESC*)_pArg);

	m_wstrTag = tDesc.pName;
	m_pAnimator = tDesc.pAnimator;
	m_pTransform = tDesc.pTransform;
	m_pStateController = tDesc.pController;
	//Safe_AddRef(m_pAnimator);
	//Safe_AddRef(m_pTransform);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Sword_Idle::Tick(const _double& _dDeltaTime)
{
	if (!m_pAnimator)
		return -1;

	//애니메이터 돌리기
	m_pAnimator->Tick(_dDeltaTime);

	_float fDist = g_pObserver->Get_Dist(m_pTransform->Get_State(CTransform::STATE_POSITION));

	if (fDist < 10.f)
		m_pStateController->Change_State(L"Chase");

	return _int();
}

_int CBastion_Sword_Idle::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CBastion_Sword_Idle::Render()
{
	return S_OK;
}

HRESULT CBastion_Sword_Idle::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Sword::ANIM_TYPE::IDLE);
	return S_OK;
}

HRESULT CBastion_Sword_Idle::ExitState()
{
	return S_OK;
}

HRESULT CBastion_Sword_Idle::EnterState(void* _pArg)
{
	return S_OK;
}

HRESULT CBastion_Sword_Idle::ExitState(void* _pArg)
{
	return S_OK;
}

CBastion_Sword_Idle* CBastion_Sword_Idle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Sword_Idle* pInstance = new CBastion_Sword_Idle(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Sword_Idle Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Sword_Idle::Free()
{
	CMonster_FSM::Free();
}
