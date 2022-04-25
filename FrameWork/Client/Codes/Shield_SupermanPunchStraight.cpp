#include "pch.h"
#include "Shield_SupermanPunchStraight.h"

#include "StateController.h"

CShield_SupermanPunchStraight::CShield_SupermanPunchStraight(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_SupermanPunchStraight::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 25;

	return S_OK;
}

_int CShield_SupermanPunchStraight::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (16 < iCurKeyFrameIndex)
	{
		if (!m_isShake)
		{
			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(m_tShakeEvent, vPos);
			m_isShake = true;
		}
	}
	if (34 < iCurKeyFrameIndex)
	{
		m_pSilvermane->Set_EquipShieldAnim(false);
	}
	if (m_pAnimationController->Is_Finished())
	{
		if(FAILED(m_pStateController->Change_State(L"Idle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

_int CShield_SupermanPunchStraight::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_SupermanPunchStraight::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_SupermanPunchStraight::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Superman_Punch_Straight", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_pSilvermane->Set_EquipShield(true);
	m_pSilvermane->Set_EquipShieldAnim(true);

	m_pSilvermane->Set_IsShieldAttack(true);

	m_tShakeEvent.fDuration = 1.2f;
	m_tShakeEvent.fBlendInTime = 0.2f;
	m_tShakeEvent.fBlendOutTime = 0.4f;
	m_tShakeEvent.tWaveX.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveX.fFrequency = 1.f;
	m_tShakeEvent.tWaveX.fAdditionalOffset = -0.1f;
	m_tShakeEvent.tWaveY.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveY.fFrequency = 1.f;
	m_tShakeEvent.tWaveY.fAdditionalOffset = -0.5f;
	m_tShakeEvent.tWaveZ.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveZ.fFrequency = 1.f;
	m_tShakeEvent.tWaveZ.fAdditionalOffset = 0.4f;

	return S_OK;
}

HRESULT CShield_SupermanPunchStraight::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_EquipShield(false);
	m_pSilvermane->Set_EquipShieldAnim(false);

	m_pSilvermane->Set_IsShieldAttack(false);
	return S_OK;
}

_int CShield_SupermanPunchStraight::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{

	}

	return _int();
}

CShield_SupermanPunchStraight* CShield_SupermanPunchStraight::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_SupermanPunchStraight* pInstance = new CShield_SupermanPunchStraight(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_SupermanPunchStraight Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield_SupermanPunchStraight::Free()
{

	__super::Free();
}
