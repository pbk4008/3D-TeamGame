#include "pch.h"
#include "Shield_WalkFwd.h"

#include "StateController.h"

CShield_WalkFwd::CShield_WalkFwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Walk(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_WalkFwd::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_WalkFwd::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	m_pSilvermane->Add_Velocity(CTransform::STATE_LOOK, _dDeltaTime * 0.5);

	return _int();
}

_int CShield_WalkFwd::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT > iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_WalkFwd::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_WalkFwd::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	
	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Walk_Fwd_Player", true);
	m_pAnimationController->Set_RootMotion(true, false, ERootOption::XYZ);
	

	return S_OK;
}

HRESULT CShield_WalkFwd::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CShield_WalkFwd::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_W))
	{
		//if (g_pGameInstance->getkeyPress(DIK_A))
		//{
		//	Add_PlusAngle(EDir::LeftForward, _dDeltaTime);
		//}
		//else if (g_pGameInstance->getkeyPress(DIK_D))
		//{
		//	Add_PlusAngle(EDir::RightForward, _dDeltaTime);
		//}
		//else
		//{
			Add_PlusAngle(EDir::Forward, _dDeltaTime);
		//}
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkBwdStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkRightStart")))
			return -1;
		return STATE_CHANGE;
	}
	else if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkLeftStart")))
			return -1;
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_WalkFwdStop")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

CShield_WalkFwd* CShield_WalkFwd::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_WalkFwd* pInstance = new CShield_WalkFwd(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_WalkFwd Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_WalkFwd::Free()
{

	__super::Free();
}
