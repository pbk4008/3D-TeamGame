#include "pch.h"
#include "State_Silvermane.h"

CState_Silvermane::CState_Silvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState(_pDevice, _pDeviceContext)
{
}

HRESULT CState_Silvermane::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CState_Silvermane::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	iProgress = KeyCheck(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CState_Silvermane::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CState_Silvermane::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Silvermane::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Silvermane::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CState_Silvermane::Set_Model(CModel* _pModel)
{
	m_pModel = _pModel;
}

void CState_Silvermane::Set_AnimationController(CAnimationController* _pAnimationController)
{
	m_pAnimationController = _pAnimationController;
}

const _int CState_Silvermane::Add_PlusAngle(const EDir _eDir, const _double& _dDeltaTime)
{
	_float fPlusAngle = m_pSilvermane->Get_PlusAngle();
	_float fValue = 4.f; // 고정시킬 각도의 여유를 주는값
	switch (_eDir)
	{
	case EDir::Forward:
		if (0.f - fValue > fPlusAngle && -180.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (0.f + fValue < fPlusAngle && 180.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (-180.f >= fPlusAngle && -360.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (180.f <= fPlusAngle && 360.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		break;
	case EDir::LeftForward:
		if (-45.f + fValue < fPlusAngle && 135.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (-45.f - fValue > fPlusAngle && -225.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (135.f <= fPlusAngle && 315.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-225.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (315.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		break;
	case EDir::RightForward:
		if (45.f - fValue > fPlusAngle && -135.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (45.f + fValue < fPlusAngle && 225.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (-135.f >= fPlusAngle && -315.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (225.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-315.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		break;
	case EDir::Backward:
		if (0.f >= fPlusAngle && -180.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (0.f <= fPlusAngle && 180.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-180.f - fValue > fPlusAngle && -360.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (180.f + fValue < fPlusAngle && 360.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		break;
	case EDir::LeftBackward:
		if (45.f >= fPlusAngle && -135.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (45.f <= fPlusAngle && 225.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-135.f - fValue > fPlusAngle && -315.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-315.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (225.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		break;
	case EDir::RightBackward:
		if (-45.f <= fPlusAngle && 135.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-45.f >= fPlusAngle && -225.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (135.f + fValue < fPlusAngle && 315.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (315.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-225.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		break;
	case EDir::Left:
		if (90.f >= fPlusAngle && -90.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (90.f <= fPlusAngle && 270.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-90.f - fValue > fPlusAngle && -270.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-270.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (270.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		break;
	case EDir::Right:
		if (-90.f <= fPlusAngle && 90.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-90.f >= fPlusAngle && -270.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (90.f + fValue < fPlusAngle && 270.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (270.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-270.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		break;
	}

	return _int();
}

void CState_Silvermane::Set_Silvermane(CSilvermane* _pSilvermane)
{
	m_pSilvermane = _pSilvermane;
}

void CState_Silvermane::Set_Transform(CTransform* _pTransform)
{
	m_pTransform = _pTransform;
}

_int CState_Silvermane::KeyCheck(const _double& _dDeltaTime)
{
	return _int();
}

void CState_Silvermane::Free()
{

	__super::Free();
}
