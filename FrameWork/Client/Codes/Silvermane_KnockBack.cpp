#include "pch.h"
#include "Silvermane_KnockBack.h"

CSilvermane_KnockBack::CSilvermane_KnockBack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Hit(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_KnockBack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_KnockBack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (m_pSilvermane->IsEquipWeapon())
		{
			switch (m_pSilvermane->Get_WeaponType())
			{
			case CWeapon::EType::Sword_1H:
				if (FAILED(m_pStateController->Change_State(L"1H_SwordIdle")))
					return -1;
				return STATE_CHANGE;
				break;
			case CWeapon::EType::Hammer_2H:
				if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
					return -1;
				return STATE_CHANGE;
				break;
			}
		}
		else
		{
			if (FAILED(m_pStateController->Change_State(L"Idle")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

_int CSilvermane_KnockBack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_KnockBack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_KnockBack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Knockdown_Bwd_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	m_pAnimationController->Mul_MoveSpeed(0.5f);

	if (!m_isShake)
	{
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 0.4f;
		tShakeEvent.fBlendInTime = 0.1f;
		tShakeEvent.fBlendOutTime = 0.1f;
		tShakeEvent.tWaveX.fAmplitude = 0.04f;
		tShakeEvent.tWaveX.fFrequency = 12.f;
		tShakeEvent.tWaveY.fAmplitude = 0.04f;
		tShakeEvent.tWaveY.fFrequency = 14.f;
		tShakeEvent.tWaveZ.fAmplitude = 0.04f;
		tShakeEvent.tWaveZ.fFrequency = 10.f;

		_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		g_pShakeManager->Shake(tShakeEvent, vPos);
		m_isShake = true;
	}

	m_iCutIndex = 70;
	return S_OK;
}

HRESULT CSilvermane_KnockBack::EnterState(void* _pArg)
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	EDir eDir = EDir::Max;
	if (_pArg)
		memcpy_s(&eDir, sizeof(EDir), _pArg, sizeof(EDir));

	switch (eDir)
	{
	case EDir::Forward:
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Knockdown_Bwd_Player", false)))
			return E_FAIL;
		break;
	case EDir::Backward:
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Knockdown_Fwd_Player", false)))
			return E_FAIL;
		break;
	}
	m_pAnimationController->Set_RootMotion(true, true);
	m_pAnimationController->Mul_MoveSpeed(0.5f);

	if (!m_isShake)
	{
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 0.4f;
		tShakeEvent.fBlendInTime = 0.1f;
		tShakeEvent.fBlendOutTime = 0.1f;
		tShakeEvent.tWaveX.fAmplitude = 0.04f;
		tShakeEvent.tWaveX.fFrequency = 12.f;
		tShakeEvent.tWaveY.fAmplitude = 0.04f;
		tShakeEvent.tWaveY.fFrequency = 14.f;
		tShakeEvent.tWaveZ.fAmplitude = 0.04f;
		tShakeEvent.tWaveZ.fFrequency = 10.f;

		_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		g_pShakeManager->Shake(tShakeEvent, vPos);
		m_isShake = true;
	}

	m_iCutIndex = 70;
	return S_OK;
}

HRESULT CSilvermane_KnockBack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	m_pAnimationController->Div_MoveSpeed(0.5f);


	return S_OK;
}

_int CSilvermane_KnockBack::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

CSilvermane_KnockBack* CSilvermane_KnockBack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_KnockBack* pInstance = new CSilvermane_KnockBack(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_KnockBack Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSilvermane_KnockBack::Free()
{
	__super::Free();
}
