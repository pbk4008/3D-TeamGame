#include "pch.h"
#include "Healer_Execution.h"

#include "Monster_Bastion_Healer.h"

CHealer_Execution::CHealer_Execution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)\
	: CBastion_Healer_State(_pDevice, _pDeviceContext)
{
}

HRESULT CHealer_Execution::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CHealer_Execution::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CHealer_Execution::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CHealer_Execution::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHealer_Execution::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_Execution);
	g_pMainApp->FreezeTime();

	return S_OK;
}

HRESULT CHealer_Execution::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CHealer_Execution::Look_Player(void)
{
}

void CHealer_Execution::Look_Monster(void)
{
}

CHealer_Execution* CHealer_Execution::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CHealer_Execution* pInstance = new CHealer_Execution(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CHealer_Execution Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHealer_Execution::Free()
{
	__super::Free();
}
