#include "pch.h"
#include "Shield_WalkFwdStop.h"

#include "StateController.h"

CShield_WalkFwdStop::CShield_WalkFwdStop(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Walk(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_WalkFwdStop::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_WalkFwdStop::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	//Add_PlusAngle(EDir::Forward, _dDeltaTime);

	return _int();
}

_int CShield_WalkFwdStop::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_WalkFwdStop::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_WalkFwdStop::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Walk_Fwd_Stop_Player", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);
	

	return S_OK;
}

HRESULT CShield_WalkFwdStop::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CShield_WalkFwdStop::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkBwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkFwdStart")))
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
		if (m_pAnimationController->Is_Finished())
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_BlockLoop")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

CShield_WalkFwdStop* CShield_WalkFwdStop::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_WalkFwdStop* pInstance = new CShield_WalkFwdStop(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_WalkFwdStop Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_WalkFwdStop::Free()
{

	__super::Free();
}
