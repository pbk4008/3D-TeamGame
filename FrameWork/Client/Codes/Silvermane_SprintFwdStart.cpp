#include "pch.h"
#include "Silvermane_SprintFwdStart.h"

CSilvermane_SprintFwdStart::CSilvermane_SprintFwdStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Sprint(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_SprintFwdStart::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_SprintFwdStart::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CSilvermane_SprintFwdStart::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_SprintFwdStart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_SprintFwdStart::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Sprint_Fwd_Start_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	//if (m_pSilvermane->IsEquipWeapon())
	//{
	//	switch (m_pSilvermane->Get_WeaponType())
	//	{
	//	case CWeapon::EType::Sword_1H:
	//		if (FAILED(m_pStateController->Change_State(L"1H_SwordEquipOff")))
	//			return E_FAIL;
	//		break;
	//	case CWeapon::EType::Hammer_2H:
	//		if (FAILED(m_pStateController->Change_State(L"2H_HammerEquipOff")))
	//			return E_FAIL;
	//		break;
	//	}
	//	return STATE_CHANGE;
	//}
	
	m_iCutIndex = 57;
	return S_OK;
}

HRESULT CSilvermane_SprintFwdStart::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_SprintFwdStart::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
	{
		_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();

		if (g_pGameInstance->getkeyPress(DIK_W))
		{
			if (m_iCutIndex < iCurKeyFrameIndex)
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwd")))
					return -1;
				return STATE_CHANGE;
			}

			if (g_pGameInstance->getkeyPress(DIK_A))
			{
				Add_PlusAngle(EDir::LeftForward, _dDeltaTime);
			}
			else if (g_pGameInstance->getkeyPress(DIK_D))
			{
				Add_PlusAngle(EDir::RightForward, _dDeltaTime);
			}
			else
			{
				Add_PlusAngle(EDir::Forward, _dDeltaTime);
			}
		}
		else if (g_pGameInstance->getkeyPress(DIK_S))
		{
			if (m_iCutIndex < iCurKeyFrameIndex)
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwd")))
					return -1;
				return STATE_CHANGE;
			}
			if (g_pGameInstance->getkeyPress(DIK_A))
			{
				Add_PlusAngle(EDir::LeftBackward, _dDeltaTime);
			}
			else if (g_pGameInstance->getkeyPress(DIK_D))
			{
				Add_PlusAngle(EDir::RightBackward, _dDeltaTime);
			}
			else
			{
				Add_PlusAngle(EDir::Backward, _dDeltaTime);
			}
		}
		else if (g_pGameInstance->getkeyPress(DIK_A))
		{
			if (m_iCutIndex < iCurKeyFrameIndex)
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwd")))
					return -1;
				return STATE_CHANGE;
			}

			Add_PlusAngle(EDir::Left, _dDeltaTime);
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
			if (m_iCutIndex < iCurKeyFrameIndex)
			{
				if (FAILED(m_pStateController->Change_State(L"SprintFwd")))
					return -1;
				return STATE_CHANGE;
			}

			Add_PlusAngle(EDir::Right, _dDeltaTime);
		}
		else
		{
			if (FAILED(m_pStateController->Change_State(L"SprintFwdStop")))
				return -1;
			return STATE_CHANGE;
		}
	}
	else
	{
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
		else if (g_pGameInstance->getkeyPress(DIK_A))
		{
			if (FAILED(m_pStateController->Change_State(L"JogLeft")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
			if (FAILED(m_pStateController->Change_State(L"JogRight")))
				return -1;
			return STATE_CHANGE;
		}
		else
		{
			if (FAILED(m_pStateController->Change_State(L"SprintFwdStop")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

CSilvermane_SprintFwdStart* CSilvermane_SprintFwdStart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_SprintFwdStart* pInstance = new CSilvermane_SprintFwdStart(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_SprintFwdStart Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSilvermane_SprintFwdStart::Free()
{

	__super::Free();
}
