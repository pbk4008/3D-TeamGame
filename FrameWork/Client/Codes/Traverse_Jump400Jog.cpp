#include "pch.h"
#include "Traverse_Jump400Jog.h"

CTraverse_Jump400Jog::CTraverse_Jump400Jog(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CTraverse_Jump400Jog::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CTraverse_Jump400Jog::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	return _int();
}

_int CTraverse_Jump400Jog::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CTraverse_Jump400Jog::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTraverse_Jump400Jog::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Traverse_Jump_400_Jog_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	
	m_pSilvermane->Set_IsTrasceCamera(false);
	return S_OK;
}

HRESULT CTraverse_Jump400Jog::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	return S_OK;
}

_int CTraverse_Jump400Jog::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

CTraverse_Jump400Jog* CTraverse_Jump400Jog::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CTraverse_Jump400Jog* pInstance = new CTraverse_Jump400Jog(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CTraverse_Jump400Jog Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTraverse_Jump400Jog::Free()
{

	__super::Free();
}
