#include "pch.h"
#include "Silvermane_JogBwd.h"

#include "StateController.h"

CSilvermane_JogBwd::CSilvermane_JogBwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogBwd::CSilvermane_JogBwd(const CSilvermane_JogBwd& _rhs)
	: CState_Silvermane(_rhs)
{
}

HRESULT CSilvermane_JogBwd::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSilvermane_JogBwd::Tick(const _double& TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CSilvermane_JogBwd::LateTick(const _double& TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CSilvermane_JogBwd::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane_JogBwd::EnterState()
{
	if (FAILED(__super::EnterState()))
	{
		return E_FAIL;
	}

	/*
	static_cast<CSilvermane*>(m_pGameObject)->Set_CurrentAnimation(m_pModel->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Bwd_Player"));
	m_pModel->Set_RootMotion(true, ERootOption::XYZ);
	m_pModel->Set_LoopNextAnim(true);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(180.f));
	static_cast<CSilvermane*>(m_pGameObject)->Set_AnimPivotMatrix(smatPivot);
	*/

	return S_OK;
}

HRESULT CSilvermane_JogBwd::ExitState()
{
	if (FAILED(__super::ExitState()))
	{
		return E_FAIL;
	}

	//static_cast<CSilvermane*>(m_pGameObject)->Set_AnimPivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CSilvermane_JogBwd::KeyCheck(const _double& TimeDelta)
{
	if (g_pGameInstance->getkeyPress(DIK_DOWN))
	{
	}
	else if (g_pGameInstance->getkeyPress(DIK_UP))
	{
		if (FAILED(m_pStateController->Change_State(L"JogBwdPivot180")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_RIGHT))
	{
		if (FAILED(m_pStateController->Change_State(L"JogRightStart")))
		{
			return -1;
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_LEFT))
	{
		if (FAILED(m_pStateController->Change_State(L"JogLeftStart")))
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

CSilvermane_JogBwd* CSilvermane_JogBwd::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogBwd* pInstance = new CSilvermane_JogBwd(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogBwd Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogBwd::Free()
{

	__super::Free();
}
