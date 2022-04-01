#include "pch.h"
#include "Bastion_Healer_Groggy.h"
#include "Monster_Bastion_Healer.h"

CBastion_Healer_Groggy::CBastion_Healer_Groggy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_Healer_State(_pDevice, _pDeviceContext)
{
}

HRESULT CBastion_Healer_Groggy::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Healer_Groggy::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	m_fGroggyTime -= (_float)_dDeltaTime;

	return _int();
}

_int CBastion_Healer_Groggy::LateTick(const _double& _dDeltaTime)
{				
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_Healer_Groggy::Render()
{
	return S_OK;
}

void CBastion_Healer_Groggy::Look_Player(void)
{
}

void CBastion_Healer_Groggy::Look_Monster(void)
{
	if (0 >= m_fGroggyTime)
		m_pStateController->Change_State(L"Groggy_End");
}

void CBastion_Healer_Groggy::OnTriggerEnter(CCollision& collision)
{
}

HRESULT CBastion_Healer_Groggy::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_STUN_ST);

	return S_OK;
}

HRESULT CBastion_Healer_Groggy::ExitState()
{
	m_fGroggyTime = 5.0f;
	return S_OK;
}

CBastion_Healer_Groggy* CBastion_Healer_Groggy::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Healer_Groggy* pInstance = new CBastion_Healer_Groggy(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Healer_Groggy Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Healer_Groggy::Free()
{
	__super::Free();
}
