#include "pch.h"
#include "Silvermane_SprintFwd.h"

CSilvermane_SprintFwd::CSilvermane_SprintFwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_SprintFwd::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_SprintFwd::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CSilvermane_SprintFwd::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_SprintFwd::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_SprintFwd::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Sprint_Fwd_Player", true)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	return S_OK;
}

HRESULT CSilvermane_SprintFwd::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_SprintFwd::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
	{
		if(g_pGameInstance->getkeyPress(DIK_W))
		{
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
		}
		else if (g_pGameInstance->getkeyPress(DIK_A))
		{
		}
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"SprintFwdStop")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	return _int();
}

CSilvermane_SprintFwd* CSilvermane_SprintFwd::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_SprintFwd* pInstance = new CSilvermane_SprintFwd(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_SprintFwd Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilvermane_SprintFwd::Free()
{

	__super::Free();
}
