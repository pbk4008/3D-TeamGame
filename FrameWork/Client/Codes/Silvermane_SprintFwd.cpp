#include "pch.h"
#include "Silvermane_SprintFwd.h"

CSilvermane_SprintFwd::CSilvermane_SprintFwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Sprint(_pDevice, _pDeviceContext)
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

	//m_pSilvermane->Add_Velocity(CTransform::STATE_LOOK, _dDeltaTime * 2.f);

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

	m_motiontrailidx = 0;

	return S_OK;
}

#pragma region 실패한 코드들
//_int CSilvermane_SprintFwd::Input(const _double& _dDeltaTime)
//{
//	_int iProgress = __super::Input(_dDeltaTime);
//	if (NO_EVENT != iProgress)
//		return iProgress;
//
//	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
//	{
//		_float3 vDir = m_pSilvermane->Get_Dir();
//
//		if(g_pGameInstance->getkeyPress(DIK_W))
//		{
//			if (1.f > vDir.z) m_pSilvermane->Add_Dir(0.f, (_float)_dDeltaTime);
//			if (g_pGameInstance->getkeyPress(DIK_A))
//			{
//				if (-1.f < vDir.x)
//					m_pSilvermane->Add_Dir((_float)-_dDeltaTime, 0.f);
//			}
//			else if (g_pGameInstance->getkeyPress(DIK_D))
//			{
//				if (1.f > vDir.x)
//					m_pSilvermane->Add_Dir((_float)_dDeltaTime, 0.f);
//			}
//			else
//			{
//				if (0.f > vDir.x)
//					m_pSilvermane->Add_Dir((_float)_dDeltaTime, 0.f);
//				if (0.f < vDir.x)
//					m_pSilvermane->Add_Dir((_float)-_dDeltaTime, 0.f);
//			}
//		}
//		else if (g_pGameInstance->getkeyPress(DIK_S))
//		{
//			if (-1.f < vDir.z) m_pSilvermane->Add_Dir(0.f, (_float)-_dDeltaTime);
//			if (g_pGameInstance->getkeyPress(DIK_A))
//			{
//				if (-1.f < vDir.x)
//					m_pSilvermane->Add_Dir((_float)-_dDeltaTime, 0.f);
//			}
//			else if (g_pGameInstance->getkeyPress(DIK_D))
//			{
//				if (1.f > vDir.x)
//					m_pSilvermane->Add_Dir((_float)_dDeltaTime, 0.f);
//			}
//			else
//			{
//				if (0.f > vDir.x)
//					m_pSilvermane->Add_Dir((_float)_dDeltaTime, 0.f);
//				if (0.f < vDir.x)
//					m_pSilvermane->Add_Dir((_float)-_dDeltaTime, 0.f);
//			}
//		}
//		else if (g_pGameInstance->getkeyPress(DIK_D))
//		{
//			if (1.f > vDir.x) m_pSilvermane->Add_Dir((_float)_dDeltaTime, 0.f);
//
//			if (0.f < vDir.z)
//				m_pSilvermane->Add_Dir(0.f, (_float)-_dDeltaTime);
//			if (0.f > vDir.z)
//				m_pSilvermane->Add_Dir(0.f, (_float)_dDeltaTime);
//		}
//		else if (g_pGameInstance->getkeyPress(DIK_A))
//		{
//#pragma region 실패1			
//			//// 1사분면
//			//if (-90.f < fPlusAngle && 0.f >= fPlusAngle)
//			//	m_pSilvermane->Add_PlusAngle(-_dDeltaTime);
//			//else if (270.f < fPlusAngle && 360.f >= fPlusAngle)
//			//	m_pSilvermane->Add_PlusAngle(-_dDeltaTime);
//
//
//			//// 2사분면
//			//else if (0.f <= fPlusAngle && 90.f > fPlusAngle)
//			//	m_pSilvermane->Add_PlusAngle(-_dDeltaTime);
//			//else if (-360.f <= fPlusAngle && -270.f > fPlusAngle)
//			//	m_pSilvermane->Add_PlusAngle(-_dDeltaTime);
//
//
//			//// 3사분면
//			//else if (-270.f < fPlusAngle && -180.f >= fPlusAngle)
//			//	m_pSilvermane->Add_PlusAngle(_dDeltaTime);
//			//else if (90.f < fPlusAngle && 180.f >= fPlusAngle)
//			//	m_pSilvermane->Add_PlusAngle(_dDeltaTime);
//
//
//			//// 4사분면
//			//else if (-90.f > fPlusAngle && -180.f < fPlusAngle)
//			//	m_pSilvermane->Add_PlusAngle(_dDeltaTime);
//			//else if(180.f < fPlusAngle && 270.f > fPlusAngle)
//			//	m_pSilvermane->Add_PlusAngle(_dDeltaTime);
//#pragma endregion
//#pragma region 실패2
//			//if (-90.f < fPlusAngle && 90.f >= fPlusAngle)
//			//{
//			//	m_pSilvermane->Add_PlusAngle(-_dDeltaTime);
//			//}
//			//else if (90.f <= fPlusAngle && 270.f > fPlusAngle)
//			//{
//			//	m_pSilvermane->Add_PlusAngle(_dDeltaTime);
//			//}
//			//else if (270.f < fPlusAngle)
//			//{
//			//	m_pSilvermane->Add_PlusAngle(-_dDeltaTime);
//			//}
//			//else if (-270.f <= fPlusAngle && -90.f > fPlusAngle)
//			//{
//			//	m_pSilvermane->Add_PlusAngle(_dDeltaTime);
//			//}
//			//else if (-270.f >= fPlusAngle)
//			//{
//			//	m_pSilvermane->Add_PlusAngle(-_dDeltaTime);
//			//}
//#pragma endregion
//			if (-1.f < vDir.x) m_pSilvermane->Add_Dir((_float)-_dDeltaTime, 0.f);
//			if (0.f < vDir.z)
//				m_pSilvermane->Add_Dir(0.f, (_float)-_dDeltaTime);
//			if (0.f > vDir.z)
//				m_pSilvermane->Add_Dir(0.f, (_float)_dDeltaTime);
//		}
//		else
//		{
//			if (FAILED(m_pStateController->Change_State(L"SprintFwdStop")))
//				return E_FAIL;
//			return STATE_CHANGE;
//		}
//	}
//	else
//	{
//		if (g_pGameInstance->getkeyPress(DIK_W))
//		{
//			if (FAILED(m_pStateController->Change_State(L"JogFwd")))
//				return E_FAIL;
//			return STATE_CHANGE;
//		}
//		else if (g_pGameInstance->getkeyPress(DIK_S))
//		{
//			if (FAILED(m_pStateController->Change_State(L"JogBwd")))
//				return E_FAIL;
//			return STATE_CHANGE;
//		}
//		else
//		{
//			if (FAILED(m_pStateController->Change_State(L"SprintFwdStop")))
//				return E_FAIL;
//			return STATE_CHANGE;
//		}
//	}
//
//	return _int();
//}
//_int CSilvermane_SprintFwd::Input(const _double& _dDeltaTime)
//{
//	_int iProgress = __super::Input(_dDeltaTime);
//	if (NO_EVENT != iProgress)
//		return iProgress;
//
//	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
//	{
//		_float fPlusAngle = m_pSilvermane->Get_PlusAngle();
//
//		if(g_pGameInstance->getkeyPress(DIK_W))
//		{
//			if (g_pGameInstance->getkeyPress(DIK_A))
//			{
//				if (-45.f < fPlusAngle && 135.f >= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//				else if (-45.f > fPlusAngle && -225.f <= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//				else if (135.f <= fPlusAngle && 315.f > fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//				else if (-225.f >= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//			}
//			else if (g_pGameInstance->getkeyPress(DIK_D))
//			{
//				if (45.f > fPlusAngle && -135.f <= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//				else if (45.f < fPlusAngle && 225.f >= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//				else if (-135.f >= fPlusAngle && -315.f < fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//				else if (225.f <= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//			}
//			else
//			{
//				if (0.f > fPlusAngle && -180.f <= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//				else if (0.f < fPlusAngle && 180.f >= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//				else if (-180.f >= fPlusAngle && -360.f < fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//				else if (180.f <= fPlusAngle && 360.f > fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//			}
//			
//		}
//		else if (g_pGameInstance->getkeyPress(DIK_S))
//		{
//			if (g_pGameInstance->getkeyPress(DIK_A))
//			{
//				if (45.f >= fPlusAngle && -135.f < fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//				else if (45.f <= fPlusAngle && 225.f > fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//				else if (-135.f > fPlusAngle && -315.f <= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//				else if (-315.f >= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//			}
//			else if (g_pGameInstance->getkeyPress(DIK_D))
//			{
//				if (-45.f <= fPlusAngle && 135.f > fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//				else if (-45.f >= fPlusAngle && -225.f < fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//				else if (135.f < fPlusAngle && 315.f >= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//				else if (315.f <= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//			}
//			else
//			{
//				if (0.f >= fPlusAngle && -180.f < fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//				else if (0.f <= fPlusAngle && 180.f > fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//				else if (-180.f > fPlusAngle && -360.f <= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//				}
//				else if (180.f < fPlusAngle && 360.f >= fPlusAngle)
//				{
//					m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//				}
//			}
//		}
//		else if (g_pGameInstance->getkeyPress(DIK_A))
//		{
//			if (90.f >= fPlusAngle && -90.f < fPlusAngle)
//			{
//				m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//			}
//			else if (90.f <= fPlusAngle && 270.f > fPlusAngle)
//			{
//				m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//			}
//			else if (-90.f >= fPlusAngle && -270.f < fPlusAngle)
//			{
//				m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//			}
//			else if (-270.f >= fPlusAngle)
//			{
//				m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//			}
//		}
//		else if (g_pGameInstance->getkeyPress(DIK_D))
//		{
//			if (-90.f <= fPlusAngle && 90.f > fPlusAngle)
//			{
//				m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//			}
//			else if (-90.f >= fPlusAngle && -270.f < fPlusAngle)
//			{
//				m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//			}
//			else if (90.f < fPlusAngle && 270.f >= fPlusAngle)
//			{
//				m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
//			}
//			else if (270.f <= fPlusAngle)
//			{
//				m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
//			}
//		}
//		else
//		{
//			if (FAILED(m_pStateController->Change_State(L"SprintFwdStop")))
//				return E_FAIL;
//			return STATE_CHANGE;
//		}
//	}
//	else
//	{
//		if (g_pGameInstance->getkeyPress(DIK_W))
//		{
//			if (FAILED(m_pStateController->Change_State(L"JogFwd")))
//				return E_FAIL;
//			return STATE_CHANGE;
//		}
//		else if (g_pGameInstance->getkeyPress(DIK_S))
//		{
//			if (FAILED(m_pStateController->Change_State(L"JogBwd")))
//				return E_FAIL;
//			return STATE_CHANGE;
//		}
//		else
//		{
//			if (FAILED(m_pStateController->Change_State(L"SprintFwdStop")))
//				return E_FAIL;
//			return STATE_CHANGE;
//		}
//	}
//
//	return _int();
//}
#pragma endregion

_int CSilvermane_SprintFwd::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
		{
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
			Add_PlusAngle(EDir::Left, _dDeltaTime);
		}
		else if (g_pGameInstance->getkeyPress(DIK_D))
		{
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
