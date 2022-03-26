#include "pch.h"
#include "Shield_WalkLeft.h"

#include "StateController.h"

CShield_WalkLeft::CShield_WalkLeft(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Walk(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_WalkLeft::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_WalkLeft::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	m_pSilvermane->Add_Velocity(CTransform::STATE_RIGHT, -_dDeltaTime * 0.5);

	return _int();
}

_int CShield_WalkLeft::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_WalkLeft::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_WalkLeft::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Walk_Left_Player", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(-90.f));
	m_pAnimationController->Set_PivotMatrix(smatPivot);
	

	return S_OK;
}

HRESULT CShield_WalkLeft::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CShield_WalkLeft::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_A))
	{
		//if (g_pGameInstance->getkeyPress(DIK_W))
		//{
		//	if (FAILED(m_pStateController->Change_State(L"Shield_WalkFwdStart")))
		//		return -1;
		//	return STATE_CHANGE;
		//}
		//if (g_pGameInstance->getkeyPress(DIK_S))
		//{
		//	if (FAILED(m_pStateController->Change_State(L"Shield_WalkBwdStart")))
		//		return -1;
		//	return STATE_CHANGE;
		//}
		//else
		//{
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
		//}
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkRightStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkFwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkBwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkLeftStop")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

CShield_WalkLeft* CShield_WalkLeft::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_WalkLeft* pInstance = new CShield_WalkLeft(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_WalkLeft Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_WalkLeft::Free()
{

	__super::Free();
}
