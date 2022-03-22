#include "Monster_FSM.h"
#include "Transform.h"
#include "Model.h"
#include "Animator.h"
#include "Actor.h"

CMonster_FSM::CMonster_FSM(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState(_pDevice, _pDeviceContext)
{
}

HRESULT CMonster_FSM::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMonster_FSM::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Look_Player();
	Look_Monster();

	return _int();
}

_int CMonster_FSM::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMonster_FSM::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_FSM::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_FSM::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CMonster_FSM::Look_Player(void)
{
}

void CMonster_FSM::Look_Monster(void)
{
}

void CMonster_FSM::Free()
{
	__super::Free();
}
