#include "pch.h"
#include "1H_SwordNormalSidestepLeft_V3.h"

#include "StateController.h"

C1H_SwordNormalSidestepLeft_V3::C1H_SwordNormalSidestepLeft_V3(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_Dash(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordNormalSidestepLeft_V3::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordNormalSidestepLeft_V3::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(m_eDir, _dDeltaTime);

	if (m_pAnimationController->Is_Finished())
	{
 		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordNormalSidestepLeft_V3::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordNormalSidestepLeft_V3::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordNormalSidestepLeft_V3::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Normal_Sidestep_Left_V3", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	if (g_pGameInstance->getkeyPress(DIK_W))
		m_eDir = EDir::RightForward;
	else if (g_pGameInstance->getkeyPress(DIK_S))
		m_eDir = EDir::LeftForward;
	else
		m_eDir = EDir::Forward;


	m_iCutIndex = 15;
	return S_OK;
}

HRESULT C1H_SwordNormalSidestepLeft_V3::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordNormalSidestepLeft_V3::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getkeyDown(DIK_SPACE))
		{
			if (g_pGameInstance->getkeyPress(DIK_A))
			{
				m_pAnimationController->Reset_Animation();

				if (g_pGameInstance->getkeyPress(DIK_W))
					m_eDir = EDir::RightForward;
				else if (g_pGameInstance->getkeyPress(DIK_S))
					m_eDir = EDir::LeftForward;
				else
					m_eDir = EDir::Forward;
			}
			else if (g_pGameInstance->getkeyPress(DIK_S))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
					return -1;
				return STATE_CHANGE;
			}
			else if (g_pGameInstance->getkeyPress(DIK_D))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepRight")))
					return -1;
				return STATE_CHANGE;
			}
			else if (g_pGameInstance->getkeyPress(DIK_W))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_DodgeSpin")))
					return -1;
				return STATE_CHANGE;
			}
			else
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
					return -1;
				return STATE_CHANGE;
			}
		}
	}
	
	return _int();
}

C1H_SwordNormalSidestepLeft_V3* C1H_SwordNormalSidestepLeft_V3::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordNormalSidestepLeft_V3* pInstance = new C1H_SwordNormalSidestepLeft_V3(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordNormalSidestepLeft_V3 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordNormalSidestepLeft_V3::Free()
{

	__super::Free();
}
