#include "pch.h"
#include "Bastion_2HSword_Groggy.h"
#include "Monster_Bastion_2HSword.h"

CBastion_2HSword_Groggy::CBastion_2HSword_Groggy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_2HSword_State(_pDevice, _pDeviceContext)
{
}

HRESULT CBastion_2HSword_Groggy::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_Groggy::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	if (0 >= m_fGroggyTime)
	{
		m_pStateController->Change_State(L"Groggy_End");
		m_fGroggyTime = 5.0f;
	}
	else
		m_fGroggyTime -= (_float)_dDeltaTime;

	return _int();
}

_int CBastion_2HSword_Groggy::LateTick(const _double& _dDeltaTime)
{				
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_2HSword_Groggy::Render()
{
	return S_OK;
}

void CBastion_2HSword_Groggy::Look_Player(void)
{
}

void CBastion_2HSword_Groggy::Look_Monster(void)
{
	//if (0 >= m_fGroggyTime)
	//{
	//	m_pMonster->Set_Groggy(true);
	//	m_pStateController->Change_State(L"Groggy_End");
	//}
}

void CBastion_2HSword_Groggy::OnTriggerEnter(CCollision& collision)
{
}

HRESULT CBastion_2HSword_Groggy::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_STUN_ST);

	return S_OK;
}

HRESULT CBastion_2HSword_Groggy::ExitState()
{
	m_fGroggyTime = 5.0f;
	
	m_pMonster->Set_Groggy(false);

	return S_OK;
}

CBastion_2HSword_Groggy* CBastion_2HSword_Groggy::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_Groggy* pInstance = new CBastion_2HSword_Groggy(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_Groggy Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_2HSword_Groggy::Free()
{
	__super::Free();
}
