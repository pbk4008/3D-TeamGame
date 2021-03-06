#include "pch.h"
#include "Silvermane_JogFwd.h"

#include "StateController.h"

CSilvermane_JogFwd::CSilvermane_JogFwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Jog(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogFwd::CSilvermane_JogFwd(const CSilvermane_JogFwd& _rhs)
	: CSilvermane_Jog(_rhs)
{
}

HRESULT CSilvermane_JogFwd::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_JogFwd::Tick(const _double& _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	return _int();
}

_int CSilvermane_JogFwd::LateTick(const _double& _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	return _int();
}

HRESULT CSilvermane_JogFwd::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_JogFwd::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Fwd_Player", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);
	

	return S_OK;
}

HRESULT CSilvermane_JogFwd::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_JogFwd::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_W))
	{
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
		if (FAILED(m_pStateController->Change_State(L"JogRight")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"JogLeft")))
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

CSilvermane_JogFwd* CSilvermane_JogFwd::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogFwd* pInstance = new CSilvermane_JogFwd(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogFwd Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogFwd::Free()
{

	__super::Free();
}
