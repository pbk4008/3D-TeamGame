#include "pch.h"
#include "1H_Dash.h"

C1H_Dash::C1H_Dash(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_Dash::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_Dash::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();

	if (m_iCutIndex < iCurKeyFrameIndex)
	{
		m_pSilvermane->Set_IsDash(false);
	}

	if (m_pAnimationController->Is_Finished())
	{
		return ToIdle();
	}

	return _int();
}

_int C1H_Dash::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_Dash::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_Dash::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pSilvermane->Set_IsMove(true);
	m_pSilvermane->Set_EquipShield(false);
	m_pSilvermane->Set_IsDash(true);
	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pAnimationController->Set_PlaySpeed(1.2f);

	g_pGameInstance->BlendSound(L"Player_Dash", L"Player_Dash_1", CSoundMgr::CHANNELID::PLAYER1, CSoundMgr::CHANNELID::PLAYER2);

	return S_OK;
}

HRESULT C1H_Dash::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsMove(false);
	m_pSilvermane->Set_IsDash(false);
	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pAnimationController->Set_PlaySpeed(1.f);

	return S_OK;
}

_int C1H_Dash::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (6 < iCurKeyFrameIndex && iCurKeyFrameIndex < m_iCutIndex)
	{
		if (g_pGameInstance->getkeyDown(DIK_SPACE))
		{
			
			EDir eDir = EDir::Max;
			if (g_pGameInstance->getkeyPress(DIK_A))
			{
				if (g_pGameInstance->getkeyPress(DIK_W))
					eDir = EDir::LeftForward;
				else if (g_pGameInstance->getkeyPress(DIK_S))
					eDir = EDir::LeftBackward;
				else
					eDir = EDir::Left;

			}
			else if (g_pGameInstance->getkeyPress(DIK_D))
			{
				if (g_pGameInstance->getkeyPress(DIK_W))
					eDir = EDir::RightForward;
				else if (g_pGameInstance->getkeyPress(DIK_S))
					eDir = EDir::RightBackward;
				else
					eDir = EDir::Right;
			}
			else if (g_pGameInstance->getkeyPress(DIK_S))
			{
				eDir = EDir::Backward;
			}
			else if (g_pGameInstance->getkeyPress(DIK_W))
			{
				eDir = EDir::Forward;
			}
			else
			{
				eDir = EDir::Backward;
			}

			if (FAILED(m_pStateController->Change_State(L"DodgeSlide", &eDir)))
				return -1;
			return STATE_CHANGE;
		}
	}

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (m_pSilvermane->IsLootShield())
		{
			if (g_pGameInstance->getkeyPress(DIK_Q))
			{
				if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
					return E_FAIL;
				return STATE_CHANGE;
			}
		}

		if (g_pGameInstance->getkeyPress(DIK_E))
		{
			return ToSkill();
		}

		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			return ToDashAttack();
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			return ToChargeStart();
		}

		if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
		{
			if (g_pGameInstance->getkeyPress(DIK_W) ||
				g_pGameInstance->getkeyPress(DIK_S) ||
				g_pGameInstance->getkeyPress(DIK_A) ||
				g_pGameInstance->getkeyPress(DIK_D))
			{
				return ToSprint();
			}
		}
	}


	return _int();
}

void C1H_Dash::Free()
{

	__super::Free();
}
