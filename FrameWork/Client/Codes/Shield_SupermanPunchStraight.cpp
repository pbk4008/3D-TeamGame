#include "pch.h"
#include "Shield_SupermanPunchStraight.h"

#include "StateController.h"

CShield_SupermanPunchStraight::CShield_SupermanPunchStraight(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_SupermanPunchStraight::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 25;

	return S_OK;
}

_int CShield_SupermanPunchStraight::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);

	if (m_pAnimationController->Is_Finished())
	{
		m_pStateController->Change_State(L"Shield_BlockEnd");
		return STATE_CHANGE;
	}

	return _int();
}

_int CShield_SupermanPunchStraight::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_SupermanPunchStraight::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_SupermanPunchStraight::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Superman_Punch_Straight", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_pSilvermane->Set_EquipShield(true);
	return S_OK;
}

HRESULT CShield_SupermanPunchStraight::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CShield_SupermanPunchStraight::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{

	}

	return _int();
}

CShield_SupermanPunchStraight* CShield_SupermanPunchStraight::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_SupermanPunchStraight* pInstance = new CShield_SupermanPunchStraight(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_SupermanPunchStraight Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_SupermanPunchStraight::Free()
{

	__super::Free();
}
