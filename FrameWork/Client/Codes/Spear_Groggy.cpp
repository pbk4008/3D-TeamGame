#include "pch.h"
#include "Spear_Groggy.h"
#include "Monster_Bastion_Spear.h"

CSpear_Groggy::CSpear_Groggy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

HRESULT CSpear_Groggy::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Groggy::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	m_pAnimator->Tick(_dDeltaTime);

	m_fGroggyTime -= (_float)_dDeltaTime;

	return _int();
}

_int CSpear_Groggy::LateTick(const _double& _dDeltaTime)
{				
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Groggy::Render()
{
	return S_OK;
}

void CSpear_Groggy::Look_Player(void)
{
}

void CSpear_Groggy::Look_Monster(void)
{
	if (0 >= m_fGroggyTime)
		m_pStateController->Change_State(L"Groggy_End");
}

void CSpear_Groggy::OnTriggerEnter(CCollision& collision)
{
}

HRESULT CSpear_Groggy::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_STUN_ST);

	return S_OK;
}

HRESULT CSpear_Groggy::ExitState()
{
	m_fGroggyTime = 5.0f;
	return S_OK;
}

CSpear_Groggy* CSpear_Groggy::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Groggy* pInstance = new CSpear_Groggy(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Groggy Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpear_Groggy::Free()
{
	__super::Free();
}
