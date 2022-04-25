#include "pch.h"
#include "1H_KnockBack.h"

C1H_KnockBack::C1H_KnockBack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Hit(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_KnockBack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_KnockBack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (m_iCutIndex < iCurKeyFrameIndex)
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

	if (!m_isShake && 15 < iCurKeyFrameIndex)
	{
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 0.4f;
		tShakeEvent.fBlendInTime = 0.1f;
		tShakeEvent.fBlendOutTime = 0.2f;
		tShakeEvent.tWaveX.fAmplitude = 0.04f;
		tShakeEvent.tWaveX.fFrequency = 8.f;
		tShakeEvent.tWaveY.fAmplitude = 0.4f;
		tShakeEvent.tWaveY.fFrequency = 1.f;

		_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		g_pShakeManager->Shake(tShakeEvent, vPos);
		m_isShake = true;
	}

	return _int();
}

_int C1H_KnockBack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_KnockBack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_KnockBack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	//uniform_int_distribution<_uint> iRange(0, 1);
	//switch (iRange(g_random))
	//{
	//case 0:
	//	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_KnockBack_Clash_Player", false)))
	//		return E_FAIL;
	//	break;
	//case 1:
	//	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_KnockBack_Land_Player", false)))
	//		return E_FAIL;
	//	break;
	//}

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_KnockBack_Land_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	m_pAnimationController->Mul_MoveSpeed(0.5f);

	m_iCutIndex = 70;

	m_pSilvermane->Set_IsTrasceCamera(false);
	return S_OK;
}

HRESULT C1H_KnockBack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	m_pAnimationController->Div_MoveSpeed(0.5f);

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pSilvermane->Set_Radial(false);

	return S_OK;
}

_int C1H_KnockBack::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

C1H_KnockBack* C1H_KnockBack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_KnockBack* pInstance = new C1H_KnockBack(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_KnockBack Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C1H_KnockBack::Free()
{
	__super::Free();
}
