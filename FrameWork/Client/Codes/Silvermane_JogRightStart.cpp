#include "pch.h"
#include "Silvermane_JogRightStart.h"

#include "StateController.h"

CSilvermane_JogRightStart::CSilvermane_JogRightStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogRightStart::CSilvermane_JogRightStart(const CSilvermane_JogRightStart& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT CSilvermane_JogRightStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogRightStart::Tick(const _double& TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CSilvermane_JogRightStart::LateTick(const _double& TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CSilvermane_JogRightStart::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane_JogRightStart::EnterState()
{
	if (FAILED(__super::EnterState()))
	{
		return E_FAIL;
	}

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Right_Start_Player", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(90.f));
	m_pAnimationController->Set_PivotMatrix(smatPivot);
	

	return S_OK;
}

HRESULT CSilvermane_JogRightStart::ExitState()
{
	if (FAILED(__super::ExitState()))
	{
		return E_FAIL;
	}

	m_pAnimationController->Set_PivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CSilvermane_JogRightStart::KeyCheck(const _double& TimeDelta)
{
	if (g_pGameInstance->getkeyPress(DIK_RIGHT))
	{
		if (m_pAnimationController->Is_Finished())
		{
			if (FAILED(m_pStateController->Change_State(L"JogRight")))
			{
				return -1;
			}
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_LEFT))
	{
		if (FAILED(m_pStateController->Change_State(L"JogLeftPivot180")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_UP))
	{
		if (FAILED(m_pStateController->Change_State(L"JogFwdStart")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_DOWN))
	{
		if (FAILED(m_pStateController->Change_State(L"JogBwdStart")))
		{
			return -1;
		}
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
		{
			return -1;
		}
	}

	return _int();
}

CSilvermane_JogRightStart* CSilvermane_JogRightStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogRightStart* pInstance = new CSilvermane_JogRightStart(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogRightStart Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogRightStart::Free()
{

	__super::Free();
}
