#include "pch.h"
#include "Silvermane_JogLeftPivot180.h"

#include "StateController.h"

CSilvermane_JogLeftPivot180::CSilvermane_JogLeftPivot180(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Jog(_pDevice, _pDeviceContext)
{
}

CSilvermane_JogLeftPivot180::CSilvermane_JogLeftPivot180(const CSilvermane_JogLeftPivot180& _rhs)
	: CSilvermane_Jog(_rhs)
{
}

HRESULT CSilvermane_JogLeftPivot180::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_JogLeftPivot180::Tick(const _double& _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	return _int();
}

_int CSilvermane_JogLeftPivot180::LateTick(const _double& _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	return _int();
}

HRESULT CSilvermane_JogLeftPivot180::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_JogLeftPivot180::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Loco_Jog_Left_Pivot_180_Player", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(-90.f));
	m_pAnimationController->Set_PivotMatrix(smatPivot);
	

	return S_OK;
}

HRESULT CSilvermane_JogLeftPivot180::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CSilvermane_JogLeftPivot180::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (46 < m_pAnimationController->Get_CurKeyFrameIndex())
		{
			if (FAILED(m_pStateController->Change_State(L"JogRight")))
				return -1;
			return STATE_CHANGE;
		}
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (FAILED(m_pStateController->Change_State(L"JogFwd")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyPress(DIK_S))
		{
			if (FAILED(m_pStateController->Change_State(L"JogBwd")))
				return -1;
			return STATE_CHANGE;
		}

		Add_PlusAngle(EDir::Forward, _dDeltaTime);
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"JogRightPivot180")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"JogFwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"JogBwdStart")))
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

CSilvermane_JogLeftPivot180* CSilvermane_JogLeftPivot180::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_JogLeftPivot180* pInstance = new CSilvermane_JogLeftPivot180(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_JogLeftPivot180 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_JogLeftPivot180::Free()
{

	__super::Free();
}
