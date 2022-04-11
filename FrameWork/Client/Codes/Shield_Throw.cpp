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

	//if (m_iReverse)
	//{
	//	if (15 > iCurKeyFrameIndex)
	//	{
	//		m_pAnimationController->Set_PlaySpeed(1.f);
	//		m_iReverse = false;
	//	}
	//}

	if (m_pAnimationController->Is_Finished())
	{
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
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	m_pSilvermane->Set_IsTrasceCamera(false);
	return S_OK;
}

HRESULT CShield_Throw::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pSilvermane->Set_EquipShield(false);
	m_pSilvermane->Set_EquipShieldAnim(false);

	m_isThrow = false;
	return S_OK;
}

_int CShield_Throw::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (20 < iCurKeyFrameIndex)
	{
		if (!g_pGameInstance->getMousePress(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			m_pAnimationController->Set_PlaySpeed(1.f);
			if (!m_isThrow)
			{
				RaycastForThrow();
				m_isThrow = true;
			}
		}
		else
		{
			m_pAnimationController->Add_TrackAcc(-5.f);
			m_iReverse = true;
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

	svRayPos += svRayDir * 4.f;
	RAYCASTDESC tRaycastDesc;
	XMStoreFloat3(&tRaycastDesc.vOrigin, svRayPos);
	XMStoreFloat3(&tRaycastDesc.vDir, svRayDir);
	tRaycastDesc.fMaxDistance = 30.f;
	tRaycastDesc.filterData.flags = PxQueryFlag::eANY_HIT;
	CGameObject* pHitObject = nullptr;
	tRaycastDesc.ppOutHitObject = &pHitObject;
	if (g_pGameInstance->Raycast(tRaycastDesc))
	{
		if (pHitObject)
		{
			// 타겟이 있으면 그 위치를 받아오고 점 두개를 더 구해서 베지어 곡선을 구한다음에 해당 곡선을 따라 방패 던지기
			svTargetPos = pHitObject->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		}
	}
	else
	{
		svTargetPos = svRayPos += svRayDir * tRaycastDesc.fMaxDistance;
	}


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
