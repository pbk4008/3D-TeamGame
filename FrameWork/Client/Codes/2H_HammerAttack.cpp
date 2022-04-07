#include "pch.h"
#include "2H_HammerAttack.h"

C2H_HammerAttack::C2H_HammerAttack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerAttack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerAttack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (m_iAttackStartIndex < iCurKeyFrameIndex && m_iAttackEndIndex > iCurKeyFrameIndex)
	{
		m_pSilvermane->Set_IsAttack(true);
	}
	else
		m_pSilvermane->Set_IsAttack(false);

	if (iCurKeyFrameIndex == m_iShakeIndex)
	{
		if (!m_isShake)
		{
			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(m_tShakeEvent, vPos);
			m_isShake = true;
		}
	}

	if (4 < iCurKeyFrameIndex && 10 > iCurKeyFrameIndex)
		m_pSilvermane->Set_IsTrasceCamera(false);

	return _int();
}

_int C2H_HammerAttack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}
           
HRESULT C2H_HammerAttack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerAttack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	// ½¦ÀÌÅ© ¿É¼Ç
	// 1
	m_tShakeEvent.fDuration = 0.4f;
	m_tShakeEvent.fBlendOutTime = 0.3f;
	m_tShakeEvent.tWaveX.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveX.fFrequency = 10.f;
	m_tShakeEvent.tWaveY.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveY.fFrequency = 6.f;
	m_tShakeEvent.tWaveZ.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveZ.fFrequency = 8.f;
	// 2 
	m_tShakeEvent2.fDuration = 1.f;
	m_tShakeEvent2.fBlendInTime = 0.2f;
	m_tShakeEvent2.fBlendOutTime = 0.5f;
	m_tShakeEvent2.tWaveY.fAmplitude = 0.02f;
	m_tShakeEvent2.tWaveY.fFrequency = 1.f;
	m_tShakeEvent2.tWaveY.fAdditionalOffset = -0.3f;


	g_pObserver->Set_PlayerAttackAnimStart(true);
	return S_OK;
}

HRESULT C2H_HammerAttack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);

	m_isShake2 = false;
	m_isRangeAttack = false;
	return S_OK;
}

void C2H_HammerAttack::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (!m_pSilvermane->IsAttack())
		__super::Hit(_tAttackDesc);
}

_int C2H_HammerAttack::Input(const _double& _dDeltaTime)
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

		if (g_pGameInstance->getkeyPress(DIK_Q))
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
				return E_FAIL;
			return STATE_CHANGE;
		}

		if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
		{
			if (g_pGameInstance->getkeyPress(DIK_W) ||
				g_pGameInstance->getkeyPress(DIK_S) ||
				g_pGameInstance->getkeyPress(DIK_A) ||
				g_pGameInstance->getkeyPress(DIK_D))
			{
				if (!m_pSilvermane->IsEquipWeapon())
				{
					if (FAILED(m_pStateController->Change_State(L"SprintFwdStart")))
						return -1;
				}
				else
				{
					if (FAILED(m_pStateController->Change_State(L"2H_HammerEquipOff")))
						return -1;
				}
				return STATE_CHANGE;
			}
		}
	}


	return _int();
}

void C2H_HammerAttack::Free()
{

	__super::Free();
}
