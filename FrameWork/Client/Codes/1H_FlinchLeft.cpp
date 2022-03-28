#include "pch.h"
#include "1H_FlinchLeft.h"

C1H_FlinchLeft::C1H_FlinchLeft(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Hit(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_FlinchLeft::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_FlinchLeft::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_FlinchLeft::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_FlinchLeft::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_FlinchLeft::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Flinch_Left_1_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_iCutIndex = 20;
	return S_OK;
}

HRESULT C1H_FlinchLeft::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int C1H_FlinchLeft::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

C1H_FlinchLeft* C1H_FlinchLeft::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_FlinchLeft* pInstance = new C1H_FlinchLeft(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_FlinchLeft Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C1H_FlinchLeft::Free()
{
	
	__super::Free();
}
