#include "pch.h"
#include "Shield_Throw.h"

CShield_Throw::CShield_Throw(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_Throw::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_Throw::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();

	if (22 < iCurKeyFrameIndex && 33 > iCurKeyFrameIndex)
	{
		m_pSilvermane->Set_IsTrasceCamera(true);
	}

	if (33 < iCurKeyFrameIndex && 40 > iCurKeyFrameIndex)
	{
		if (!m_isThrow)
		{
			RaycastForThrow();
			m_pSilvermane->Set_IsShieldThrow(true);
			m_pSilvermane->Set_EquipShield(false);
			m_pSilvermane->Set_IsTrasceCamera(false);
			m_isThrow = true;
		}
	}

	if (m_pSilvermane->IsShieldThrow())
	{
		//if (m_pSilvermane->IsShieldReturn())
		//{
			if (65 < iCurKeyFrameIndex && 73 > iCurKeyFrameIndex)
			{
				m_pAnimationController->Set_PlaySpeed(0.1f);
			}
		//}
	}

	if (73 < iCurKeyFrameIndex)
	{
		m_pSilvermane->Set_EquipShield(true);
		m_pAnimationController->Set_PlaySpeed(1.4f);
	}

	if (m_pAnimationController->Is_Finished())
	{
		if (g_pGameInstance->getkeyPress(DIK_Q))
		{
			if (m_pStateController->Change_State(L"Shield_BlockLoop"))
				return -1;
			return STATE_CHANGE;
		}

		m_pSilvermane->Set_EquipShield(false);
		m_pSilvermane->Set_EquipShieldAnim(false);
		return ToIdle();
	}

	return _int();
}

_int CShield_Throw::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_Throw::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_Throw::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Block_Throw_NoCharge", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, false, ERootOption::XYZ);

	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pAnimationController->Set_PlaySpeed(1.4f);

	m_iCutIndex = 40;
	return S_OK;
}

HRESULT CShield_Throw::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	
	m_isTrigger = false;
	m_isThrow = false;
	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pAnimationController->Set_PlaySpeed(1.f);

	m_pSilvermane->Set_IsShieldThrow(false);
	m_pSilvermane->Set_IsShieldReturn(false);
	m_pSilvermane->Set_BlockTime(0.f);
	return S_OK;
}

_int CShield_Throw::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (!m_isTrigger)
	{
		if (20 < iCurKeyFrameIndex)
		{
			if (!g_pGameInstance->getMousePress(CInputDev::MOUSESTATE::MB_RBUTTON))
			{
				m_pAnimationController->Set_PlaySpeed(1.f);
				m_isTrigger = true;
			}
			else
			{
				m_pAnimationController->Set_PlaySpeed(0.f);
				//STOP_SOUND(CHANNEL::PLAYER2);
				PLAY_SOUND(L"Shield_Charge", CHANNEL::PLAYER2);
			}
		}
	}

	if (m_iCutIndex < iCurKeyFrameIndex)
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			m_pSilvermane->Set_EquipShield(false);
			m_pSilvermane->Set_EquipShieldAnim(false);
			return ToAttack();
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			m_pSilvermane->Set_EquipShield(false);
			m_pSilvermane->Set_EquipShieldAnim(false);
			return ToChargeStart();
		}

		if (m_pSilvermane->IsShieldThrow())
		{
			if (g_pGameInstance->getkeyDown(DIK_Q))
			{
				if (m_pSilvermane->IsLootShield())
				{
					m_pSilvermane->Set_EquipShieldAnim(false);
					if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
						return E_FAIL;
					return STATE_CHANGE;
				}
			}

			if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
			{
				if (g_pGameInstance->getkeyPress(DIK_W) ||
					g_pGameInstance->getkeyPress(DIK_S) ||
					g_pGameInstance->getkeyPress(DIK_A) ||
					g_pGameInstance->getkeyPress(DIK_D))
				{
					m_pSilvermane->Set_EquipShieldAnim(false);
					return ToSprint();
				}
			}
			else
			{
				if (g_pGameInstance->getkeyPress(DIK_W))
				{
					m_pSilvermane->Set_EquipShieldAnim(false);
					return ToJogFwd();
				}
				if (g_pGameInstance->getkeyPress(DIK_S))
				{
					m_pSilvermane->Set_EquipShieldAnim(false);
					return ToJogBwd();
				}
				if (g_pGameInstance->getkeyPress(DIK_D))
				{
					m_pSilvermane->Set_EquipShieldAnim(false);
					return ToJogRight();
				}
				if (g_pGameInstance->getkeyPress(DIK_A))
				{
					m_pSilvermane->Set_EquipShieldAnim(false);
					return ToJogLeft();
				}
			}
		}
	}

	return _int();
}

void CShield_Throw::RaycastForThrow()
{
	_matrix smatView;
	smatView = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	smatView = XMMatrixInverse(nullptr, smatView);

	if (XMMatrixIsNaN(smatView))
		return;

	_vector svRayPos, svRayDir, svTargetPos;

	memcpy_s(&svRayPos, sizeof(_vector), &smatView.r[3], sizeof(_vector));
	memcpy_s(&svRayDir, sizeof(_vector), &smatView.r[2], sizeof(_vector));
	svRayDir = XMVector3Normalize(svRayDir);

	svRayPos += svRayDir * 3.f;
	// ????????
	SWEEPDESC tSweepDesc;
	tSweepDesc.geometry = PxSphereGeometry(1.f);
	XMStoreFloat3(&tSweepDesc.vOrigin, svRayPos);
	XMStoreFloat3(&tSweepDesc.vDir, svRayDir);
	tSweepDesc.fMaxDistance = 20.f;
	tSweepDesc.filterData.flags |= PxQueryFlag::eANY_HIT;
	tSweepDesc.layerMask = (1 << (_uint)ELayer::Monster);// + (1 << (_uint)ELayer::Enviroment);
	CGameObject* pHitObject = nullptr;
	tSweepDesc.ppOutHitObject = &pHitObject;
	// ??????
	RAYCASTDESC tRaycastDesc;
	XMStoreFloat3(&tRaycastDesc.vOrigin, svRayPos);
	XMStoreFloat3(&tRaycastDesc.vDir, svRayDir);
	tRaycastDesc.fMaxDistance = 20.f;
	tRaycastDesc.filterData.flags |= PxQueryFlag::eANY_HIT;
	tRaycastDesc.layerMask = (1 << (_uint)ELayer::Enviroment);
	tRaycastDesc.ppOutHitObject = &pHitObject;

	if (g_pGameInstance->Sweep(tSweepDesc))
	{
		//for (_uint i = 0; i < tSweepDesc.iHitNum; ++i)
		//{
		//	svTargetPos = XMLoadFloat3(&tSweepDesc.vecHitPositions[i]);
		//	if ((_uint)GAMEOBJECT::ENVIRONMENT != tSweepDesc.vecHitObjects[i]->getTag())
		//	{
		//		break;
		//	}
		//}
		if (pHitObject)
		{
			// ?????? ?????? ?? ?????? ???????? ?? ?????? ?? ?????? ?????? ?????? ?????????? ???? ?????? ???? ???? ??????
			//svTargetPos = pHitObject->Get_Transform()->Get_State(CTransform::STATE_POSITION);
			svTargetPos = XMLoadFloat3(&tSweepDesc.vHitPos);
		}
	}
	else if (g_pGameInstance->Raycast(tRaycastDesc))
	{
		if (pHitObject)
		{
			svTargetPos = XMLoadFloat3(&tRaycastDesc.vHitPos);
		}
	}
	else
	{
		svTargetPos = svRayPos += svRayDir * tSweepDesc.fMaxDistance;
	}

	m_pSilvermane->ThrowShield(svTargetPos);
}

CShield_Throw* CShield_Throw::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_Throw* pInstance = new CShield_Throw(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_Throw Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShield_Throw::Free()
{
	__super::Free();
}
