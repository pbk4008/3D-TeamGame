#include "pch.h"
#include "Silvermane_JogFwdStart.h"

#include "StateController.h"

CSilvermane_JogFwdStart::CSilvermane_JogFwdStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Jog(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogFwdStart::CSilvermane_JogFwdStart(const CSilvermane_JogFwdStart& _rhs)
	: CSilvermane_Jog(_rhs)
{
}

HRESULT CSilvermane_JogFwdStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_JogFwdStart::Tick(const _double& _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	return _int();
}

_int CSilvermane_JogFwdStart::LateTick(const _double& _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	return _int();
}

HRESULT CSilvermane_JogFwdStart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_JogFwdStart::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Fwd_Start_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);
	

	return S_OK;
}

HRESULT CSilvermane_JogFwdStart::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_JogFwdStart::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (m_pAnimationController->Is_Finished())
		{
			if (FAILED(m_pStateController->Change_State(L"JogFwd")))
				return -1;
			return STATE_CHANGE;
		}

		if (g_pGameInstance->getkeyPress(DIK_A))
		{
			Add_PlusAngle(EDir::LeftForward, _dDeltaTime);
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
			Add_PlusAngle(EDir::RightForward, _dDeltaTime);
		}
		else
		{
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"JogFwdPivot180")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"JogRightStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"JogLeftStart")))
			return -1;
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"JogFwdStop")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

CSilvermane_JogFwdStart* CSilvermane_JogFwdStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogFwdStart* pInstance = new CSilvermane_JogFwdStart(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogFwdStart Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogFwdStart::Free()
{

	__super::Free();
}
