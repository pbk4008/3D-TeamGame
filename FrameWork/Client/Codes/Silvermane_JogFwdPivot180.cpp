#include "pch.h"
#include "Silvermane_JogFwdPivot180.h"

#include "StateController.h"

CSilvermane_JogFwdPivot180::CSilvermane_JogFwdPivot180(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Jog(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogFwdPivot180::CSilvermane_JogFwdPivot180(const CSilvermane_JogFwdPivot180& _rhs)
	: CSilvermane_Jog(_rhs)
{
}

HRESULT CSilvermane_JogFwdPivot180::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_JogFwdPivot180::Tick(const _double& _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	return _int();
}

_int CSilvermane_JogFwdPivot180::LateTick(const _double& _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	return _int();
}

HRESULT CSilvermane_JogFwdPivot180::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_JogFwdPivot180::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Fwd_Pivot_180_Player", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);
	

	return S_OK;
}

HRESULT CSilvermane_JogFwdPivot180::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_JogFwdPivot180::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (m_pAnimationController->Is_Finished())
		{
			if (FAILED(m_pStateController->Change_State(L"JogBwd")))
				return -1;
			return STATE_CHANGE;
		}

		if (g_pGameInstance->getkeyPress(DIK_A))
		{
			Add_PlusAngle(EDir::RightForward, _dDeltaTime);
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
			Add_PlusAngle(EDir::LeftForward, _dDeltaTime);
		}
		else
		{
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"JogBwdPivot180")))
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
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

CSilvermane_JogFwdPivot180* CSilvermane_JogFwdPivot180::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogFwdPivot180* pInstance = new CSilvermane_JogFwdPivot180(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogFwdPivot180 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogFwdPivot180::Free()
{

	__super::Free();
}
