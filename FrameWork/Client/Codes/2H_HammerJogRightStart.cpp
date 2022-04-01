#include "pch.h"
#include "2H_HammerJogRightStart.h"

#include "StateController.h"

C2H_HammerJogRightStart::C2H_HammerJogRightStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerJog(_pDevice, _pDeviceContext)
{
}

C2H_HammerJogRightStart::C2H_HammerJogRightStart(const C2H_HammerJogRightStart& _rhs)
	: C2H_HammerJog(_rhs)
{
}

HRESULT C2H_HammerJogRightStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerJogRightStart::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pSilvermane->Add_Velocity(CTransform::STATE_RIGHT, _dDeltaTime);

	return _int();
}

_int C2H_HammerJogRightStart::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerJogRightStart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerJogRightStart::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Loco_Jog_Right_Start_Normal", false);
	m_pAnimationController->Set_RootMotion(true, false, ERootOption::XYZ);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(90.f));
	m_pAnimationController->Set_PivotMatrix(smatPivot);
	

	return S_OK;
}

HRESULT C2H_HammerJogRightStart::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int C2H_HammerJogRightStart::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (31 < m_pAnimationController->Get_CurKeyFrameIndex())
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerJogRight")))
				return -1;
			return STATE_CHANGE;
		}

		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerJogFwd")))
				return -1;
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_S))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerJogBwd")))
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
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogRightPivot180")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogFwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogBwdStart")))
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

C2H_HammerJogRightStart* C2H_HammerJogRightStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerJogRightStart* pInstance = new C2H_HammerJogRightStart(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerJogRightStart Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerJogRightStart::Free()
{

	__super::Free();
}
