#include "pch.h"
#include "Silvermane_JogLeft.h"

#include "StateController.h"

CSilvermane_JogLeft::CSilvermane_JogLeft(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogLeft::CSilvermane_JogLeft(const CSilvermane_JogLeft& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT CSilvermane_JogLeft::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogLeft::Tick(const _double& TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CSilvermane_JogLeft::LateTick(const _double& TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CSilvermane_JogLeft::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane_JogLeft::EnterState()
{
	if (FAILED(__super::EnterState()))
	{
		return E_FAIL;
	}

	/*
	static_cast<CSilvermane*>(m_pGameObject)->Set_CurrentAnimation(m_pModel->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Left_Player"));
	m_pModel->Set_RootMotion(true, ERootOption::XYZ);
	m_pModel->Set_LoopNextAnim(true);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(-90.f));
	static_cast<CSilvermane*>(m_pGameObject)->Set_AnimPivotMatrix(smatPivot);
	*/

	return S_OK;
}

HRESULT CSilvermane_JogLeft::ExitState()
{
	if (FAILED(__super::ExitState()))
	{
		return E_FAIL;
	}

	//static_cast<CSilvermane*>(m_pGameObject)->Set_AnimPivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CSilvermane_JogLeft::KeyCheck(const _double& TimeDelta)
{
	if (g_pGameInstance->getkeyPress(DIK_LEFT))
	{
	}
	else if (g_pGameInstance->getkeyPress(DIK_RIGHT))
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
	else if (g_pGameInstance->getkeyPress(DIK_UP))
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

CSilvermane_JogLeft* CSilvermane_JogLeft::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogLeft* pInstance = new CSilvermane_JogLeft(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogLeft Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogLeft::Free()
{

	__super::Free();
}
