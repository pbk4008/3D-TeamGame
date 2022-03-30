#include "pch.h"
#include "Spear_Groggy_End.h"
#include "Monster_Bastion_Spear.h"
#include "Animation.h"

CSpear_Groggy_End::CSpear_Groggy_End(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

HRESULT CSpear_Groggy_End::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Groggy_End::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CSpear_Groggy_End::LateTick(const _double& _dDeltaTime)
{				
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Groggy_End::Render()
{
	return S_OK;
}

void CSpear_Groggy_End::Look_Player(void)
{
}

void CSpear_Groggy_End::Look_Monster(void)
{
	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
		m_pStateController->Change_State(L"Idle");
}

void CSpear_Groggy_End::OnTriggerEnter(CCollision& collision)
{
}

HRESULT CSpear_Groggy_End::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_STUN_ED);

	return S_OK;
}

HRESULT CSpear_Groggy_End::ExitState()
{
	return S_OK;
}

CSpear_Groggy_End* CSpear_Groggy_End::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Groggy_End* pInstance = new CSpear_Groggy_End(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Groggy_End Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpear_Groggy_End::Free()
{
	__super::Free();
}
