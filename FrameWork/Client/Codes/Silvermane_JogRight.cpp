#include "pch.h"
#include "Silvermane_JogRight.h"

#include "StateController.h"

CSilvermane_JogRight::CSilvermane_JogRight(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Jog(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogRight::CSilvermane_JogRight(const CSilvermane_JogRight& _rhs)
	: CSilvermane_Jog(_rhs)
{
}

HRESULT CSilvermane_JogRight::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_JogRight::Tick(const _double& _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	return _int();
}

_int CSilvermane_JogRight::LateTick(const _double& _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	return _int();
}

HRESULT CSilvermane_JogRight::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_JogRight::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Right_Player", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(90.f));
	m_pAnimationController->Set_PivotMatrix(smatPivot);
	
	m_pAnimationController->Add_TrackAcc(3.0);
	return S_OK;
}

HRESULT CSilvermane_JogRight::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CSilvermane_JogRight::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (FAILED(m_pStateController->Change_State(L"JogFwd")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyPress(DIK_S))
		{
			if (FAILED(m_pStateController->Change_State(L"JogBwd")))
				return -1;
			return STATE_CHANGE;
		}
		else
		{
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"JogRightPivot180")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"JogFwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"JogBwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return -1;
		return STATE_CHANGE;
	}
	
	return _int();
}

CSilvermane_JogRight* CSilvermane_JogRight::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogRight* pInstance = new CSilvermane_JogRight(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogRight Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogRight::Free()
{

	__super::Free();
}
