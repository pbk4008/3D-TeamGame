#include "pch.h"
#include "2H_HammerJogBwd.h"

#include "StateController.h"

C2H_HammerJogBwd::C2H_HammerJogBwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerJog(_pDevice, _pDeviceContext)
{
}

C2H_HammerJogBwd::C2H_HammerJogBwd(const C2H_HammerJogBwd& _rhs)
	: C2H_HammerJog(_rhs)
{
}

HRESULT C2H_HammerJogBwd::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerJogBwd::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	m_pSilvermane->Add_Velocity(CTransform::STATE_LOOK, -_dDeltaTime);

	return _int();
}

_int C2H_HammerJogBwd::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerJogBwd::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerJogBwd::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Loco_Jog_Bwd_Normal", true);
	m_pAnimationController->Set_RootMotion(true, false, ERootOption::XYZ);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pAnimationController->Set_PivotMatrix(smatPivot);

	return S_OK;
}

HRESULT C2H_HammerJogBwd::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int C2H_HammerJogBwd::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_S))
	{
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
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogBwdPivot180")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogRight")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerJogLeft")))
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

C2H_HammerJogBwd* C2H_HammerJogBwd::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerJogBwd* pInstance = new C2H_HammerJogBwd(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerJogBwd Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerJogBwd::Free()
{

	__super::Free();
}
