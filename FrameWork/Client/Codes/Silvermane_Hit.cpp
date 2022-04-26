#include "pch.h"
#include "Silvermane_Hit.h"
#include "Camera_Silvermane.h"

CSilvermane_Hit::CSilvermane_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_Hit::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_Hit::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		m_pSilvermane->Set_IsHit(false);
	}

	return _int();
}

_int CSilvermane_Hit::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_Hit::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_Hit::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pSilvermane->Set_IsHit(true);

	m_pSilvermane->Active_Effect((_uint)EFFECT::PLAYER_HIT);
	m_pSilvermane->Set_Radial(false);

	CCameraShake::SHAKEEVENT tShakeEvent;
	tShakeEvent.fDuration = 0.4f;
	tShakeEvent.fBlendInTime = 0.1f;
	tShakeEvent.fBlendOutTime = 0.2f;
	tShakeEvent.tWaveX.fAmplitude = 0.1f;
	tShakeEvent.tWaveX.fFrequency = 10.f;
	tShakeEvent.tWaveY.fAmplitude = 0.1f;
	tShakeEvent.tWaveY.fFrequency = 12.f;
	tShakeEvent.tWaveZ.fAmplitude = 0.1f;
	tShakeEvent.tWaveZ.fFrequency = 8.f;

	_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	g_pShakeManager->Shake(tShakeEvent, vPos);

	return S_OK;
}

HRESULT CSilvermane_Hit::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsHit(false);
	m_isShake = false;
	return S_OK;
}

_int CSilvermane_Hit::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getkeyDown(DIK_SPACE))
		{
			if (g_pGameInstance->getkeyPress(DIK_A))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepLeft")))
					return -1;
				return STATE_CHANGE;
			}
			else if (g_pGameInstance->getkeyPress(DIK_S))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
					return -1;
				return STATE_CHANGE;
			}
			else if (g_pGameInstance->getkeyPress(DIK_D))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepRight")))
					return -1;
				return STATE_CHANGE;
			}
			else if (g_pGameInstance->getkeyPress(DIK_W))
			{
				if (FAILED(m_pStateController->Change_State(L"1H_DodgeSpin")))
					return -1;
				return STATE_CHANGE;
			}
			else
			{
				if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
					return -1;
				return STATE_CHANGE;
			}
		}
	}
	
	return _int();
}

void CSilvermane_Hit::Free()
{

	__super::Free();
}
