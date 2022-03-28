#include "pch.h"
#include "Shield_WalkRight.h"

#include "StateController.h"

CShield_WalkRight::CShield_WalkRight(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Walk(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_WalkRight::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_WalkRight::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

_int CShield_WalkRight::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_WalkRight::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_WalkRight::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Walk_Right_Player", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(90.f));
	m_pAnimationController->Set_PivotMatrix(smatPivot);
	
	return S_OK;
}

HRESULT CShield_WalkRight::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CShield_WalkRight::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_D))
	{
		//if (g_pGameInstance->getkeyPress(DIK_W))
		//{
		//	if (FAILED(m_pStateController->Change_State(L"Shield_WalkFwdStart")))
		//		return -1;
		//	return STATE_CHANGE;
		//}
		//if (g_pGameInstance->getkeyPress(DIK_S))
		//{
		//	if (FAILED(m_pStateController->Change_State(L"Shield_WalkBwdStart")))
		//		return -1;
		//	return STATE_CHANGE;
		//}
		//else
		//{
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
		//}
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkLeftStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkFwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkBwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkRightStop")))
			return -1;
		return STATE_CHANGE;
	}
	
	return _int();
}

CShield_WalkRight* CShield_WalkRight::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_WalkRight* pInstance = new CShield_WalkRight(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_WalkRight Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_WalkRight::Free()
{

	__super::Free();
}
