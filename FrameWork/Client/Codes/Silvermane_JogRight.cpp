#include "pch.h"
#include "Silvermane_JogRight.h"

#include "StateController.h"

CSilvermane_JogRight::CSilvermane_JogRight(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogRight::CSilvermane_JogRight(const CSilvermane_JogRight& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT CSilvermane_JogRight::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogRight::Tick(const _double& TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CSilvermane_JogRight::LateTick(const _double& TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CSilvermane_JogRight::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane_JogRight::EnterState()
{
	if (FAILED(__super::EnterState()))
	{
		return E_FAIL;
	}

	/*
	static_cast<CSilvermane*>(m_pGameObject)->Set_CurrentAnimation(m_pModel->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Right_Player"));
	m_pModel->Set_RootMotion(true, ERootOption::XYZ);
	m_pModel->Set_LoopNextAnim(true);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(90.f));
	static_cast<CSilvermane*>(m_pGameObject)->Set_AnimPivotMatrix(smatPivot);
	*/

	return S_OK;
}

HRESULT CSilvermane_JogRight::ExitState()
{
	if (FAILED(__super::ExitState()))
	{
		return E_FAIL;
	}

	//static_cast<CSilvermane*>(m_pGameObject)->Set_AnimPivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CSilvermane_JogRight::KeyCheck(const _double& TimeDelta)
{
	if (g_pGameInstance->getkeyPress(DIK_RIGHT))
	{
	}
	else if (g_pGameInstance->getkeyPress(DIK_LEFT))
	{
		if (FAILED(m_pStateController->Change_State(L"JogRightToJogLeft")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_UP))
	{
		if (FAILED(m_pStateController->Change_State(L"IdleToJogFwd")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_UP))
	{
		if (FAILED(m_pStateController->Change_State(L"IdleToJogBwd")))
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
