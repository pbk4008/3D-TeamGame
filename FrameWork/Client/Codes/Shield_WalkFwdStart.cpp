#include "pch.h"
#include "Shield_WalkFwdStart.h"

#include "StateController.h"

CShield_WalkFwdStart::CShield_WalkFwdStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Walk(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_WalkFwdStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_WalkFwdStart::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

_int CShield_WalkFwdStart::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_WalkFwdStart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_WalkFwdStart::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Walk_Fwd_Start_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);
	

	return S_OK;
}

HRESULT CShield_WalkFwdStart::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CShield_WalkFwdStart::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (m_pAnimationController->Is_Finished())
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_WalkFwd")))
				return -1;
			return STATE_CHANGE;
		}

		//if (g_pGameInstance->getkeyPress(DIK_A))
		//{
		//	Add_PlusAngle(EDir::LeftForward, _dDeltaTime);
		//}
		//else if (g_pGameInstance->getkeyPress(DIK_D))
		//{
		//	Add_PlusAngle(EDir::RightForward, _dDeltaTime);
		//}
		//else
		//{
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
		//}
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkBwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkRightStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkLeftStart")))
			return -1;
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_BlockLoop")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

CShield_WalkFwdStart* CShield_WalkFwdStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_WalkFwdStart* pInstance = new CShield_WalkFwdStart(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_WalkFwdStart Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_WalkFwdStart::Free()
{

	__super::Free();
}
