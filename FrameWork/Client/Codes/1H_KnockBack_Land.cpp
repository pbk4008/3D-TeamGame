#include "pch.h"
#include "1H_KnockBack_Land.h"

C1H_KnockBack_Land::C1H_KnockBack_Land(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Hit(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_KnockBack_Land::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_KnockBack_Land::Tick(const _double& _dDeltaTime)
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

_int C1H_KnockBack_Land::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_KnockBack_Land::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_KnockBack_Land::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_KnockBack_Land_Player", false)))
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

HRESULT C1H_KnockBack_Land::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	m_pAnimationController->Div_MoveSpeed(0.5f);


	return S_OK;
}

_int C1H_KnockBack_Land::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

C1H_KnockBack_Land* C1H_KnockBack_Land::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_KnockBack_Land* pInstance = new C1H_KnockBack_Land(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_KnockBack_Land Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C1H_KnockBack_Land::Free()
{
	__super::Free();
}
