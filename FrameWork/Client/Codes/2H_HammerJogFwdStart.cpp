#include "pch.h"
#include "2H_HammerJogFwdStart.h"

#include "StateController.h"

C2H_HammerJogFwdStart::C2H_HammerJogFwdStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerJog(_pDevice, _pDeviceContext)
{
}

C2H_HammerJogFwdStart::C2H_HammerJogFwdStart(const C2H_HammerJogFwdStart& _rhs)
	: C2H_HammerJog(_rhs)
{
}

HRESULT C2H_HammerJogFwdStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerJogFwdStart::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	m_pSilvermane->Add_Velocity(CTransform::STATE_LOOK, _dDeltaTime);

	return _int();
}

_int C2H_HammerJogFwdStart::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerJogFwdStart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerJogFwdStart::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Loco_Jog_Fwd_Start_Normal", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, false, ERootOption::XYZ);
	

	return S_OK;
}

HRESULT C2H_HammerJogFwdStart::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerJogFwdStart::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (42 < m_pAnimationController->Get_CurKeyFrameIndex())
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerJogFwd")))
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
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogFwdPivot180")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogRightStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogLeftStart")))
			return -1;
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

C2H_HammerJogFwdStart* C2H_HammerJogFwdStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerJogFwdStart* pInstance = new C2H_HammerJogFwdStart(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerJogFwdStart Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerJogFwdStart::Free()
{

	__super::Free();
}
