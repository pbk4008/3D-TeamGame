#include "pch.h"
#include "Shield_WalkLeftStart.h"

#include "StateController.h"

CShield_WalkLeftStart::CShield_WalkLeftStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Walk(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_WalkLeftStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_WalkLeftStart::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	//m_pSilvermane->Add_Velocity(CTransform::STATE_RIGHT, -_dDeltaTime * 0.5);

	return _int();
}

_int CShield_WalkLeftStart::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_WalkLeftStart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_WalkLeftStart::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Walk_Left_Start_Player", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	_matrix smatPivot = XMMatrixRotationY(XMConvertToRadians(-90.f));
	m_pAnimationController->Set_PivotMatrix(smatPivot);
	
	m_iCutIndex = 33;
	return S_OK;
}

HRESULT CShield_WalkLeftStart::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PivotMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CShield_WalkLeftStart::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_WalkLeft")))
				return -1;
			return STATE_CHANGE;
		}

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
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkBwdStart")))
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
		if (FAILED(m_pStateController->Change_State(L"Shield_BlockLoop")))
			return -1;
		return STATE_CHANGE;
	}


	return _int();
}

CShield_WalkLeftStart* CShield_WalkLeftStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_WalkLeftStart* pInstance = new CShield_WalkLeftStart(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_WalkLeftStart Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_WalkLeftStart::Free()
{

	__super::Free();
}
