#include "pch.h"
#include "1H_SwordJogLeft.h"

#include "StateController.h"

C1H_SwordJogLeft::C1H_SwordJogLeft(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordJog(_pDevice, _pDeviceContext)
{
}

C1H_SwordJogLeft::C1H_SwordJogLeft(const C1H_SwordJogLeft& _rhs)
	: C1H_SwordJog(_rhs)
{
}

HRESULT C1H_SwordJogLeft::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordJogLeft::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	m_pSilvermane->Add_Velocity(CTransform::STATE_RIGHT, -_dDeltaTime);

	return _int();
}

_int C1H_SwordJogLeft::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordJogLeft::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordJogLeft::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Loco_Jog_Left_Normal", true);
	m_pAnimationController->Set_RootMotion(true, false, ERootOption::XYZ);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(-90.f));
	m_pAnimationController->Set_PivotMatrix(smatPivot);
	

	return S_OK;
}

HRESULT C1H_SwordJogLeft::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int C1H_SwordJogLeft::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if(FAILED(m_pStateController->Change_State(L"1H_SwordJogFwd")))
				return -1;
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_S))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordJogBwd")))
				return -1;
			return STATE_CHANGE;
		}
		else
		{
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
		}
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordJogLeftPivot180")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordJogFwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SwordJogBwdStart")))
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

C1H_SwordJogLeft* C1H_SwordJogLeft::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordJogLeft* pInstance = new C1H_SwordJogLeft(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordJogLeft Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordJogLeft::Free()
{

	__super::Free();
}
