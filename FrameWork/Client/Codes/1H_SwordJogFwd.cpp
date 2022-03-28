#include "pch.h"
#include "1H_SwordJogFwd.h"

#include "StateController.h"

C1H_SwordJogFwd::C1H_SwordJogFwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordJog(_pDevice, _pDeviceContext)
{
}

C1H_SwordJogFwd::C1H_SwordJogFwd(const C1H_SwordJogFwd& _rhs)
	: C1H_SwordJog(_rhs)
{
}

HRESULT C1H_SwordJogFwd::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordJogFwd::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

_int C1H_SwordJogFwd::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordJogFwd::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordJogFwd::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Loco_Jog_Fwd_Normal", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);
	

	return S_OK;
}

HRESULT C1H_SwordJogFwd::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordJogFwd::Input(const _double& _dDeltaTime)
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
		if (FAILED(m_pStateController->Change_State(L"1H_SwordJogFwdPivot180")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordJogRight")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordJogLeft")))
			return -1;
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordIdle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

C1H_SwordJogFwd* C1H_SwordJogFwd::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordJogFwd* pInstance = new C1H_SwordJogFwd(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordJogFwd Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordJogFwd::Free()
{

	__super::Free();
}
