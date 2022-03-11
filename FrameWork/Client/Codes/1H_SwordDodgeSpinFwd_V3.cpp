#include "pch.h"
#include "1H_SwordDodgeSpinFwd_V3.h"

#include "StateController.h"

#include "Silvermane.h"

C1H_SwordDodgeSpinFwd_V3::C1H_SwordDodgeSpinFwd_V3(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordDodgeSpinFwd_V3::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordDodgeSpinFwd_V3::Tick(const _double& _dDeltaTime)
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

_int C1H_SwordDodgeSpinFwd_V3::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordDodgeSpinFwd_V3::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordDodgeSpinFwd_V3::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_DodgeSpinFwd_V3", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	
	return S_OK;
}

HRESULT C1H_SwordDodgeSpinFwd_V3::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordDodgeSpinFwd_V3::KeyCheck(const _double& _dDeltaTime)
{
	_int iProgress = __super::KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyDown(DIK_SPACE))
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (20 < m_pAnimationController->Get_CurKeyFrameIndex())
			{
				m_pAnimationController->Reset_Animation();
			}
		}
		else if (g_pGameInstance->getkeyDown(DIK_A))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SidestepLeft")))
				return E_FAIL;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyDown(DIK_D))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SidestepRight")))
				return E_FAIL;
			return STATE_CHANGE;
		}
		else
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C1H_SwordDodgeSpinFwd_V3* C1H_SwordDodgeSpinFwd_V3::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordDodgeSpinFwd_V3* pInstance = new C1H_SwordDodgeSpinFwd_V3(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordDodgeSpinFwd_V3 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordDodgeSpinFwd_V3::Free()
{

	__super::Free();
}
