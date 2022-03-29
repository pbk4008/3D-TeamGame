#include "pch.h"
#include "1H_SwordAttack.h"
#include "Camera_Silvermane.h"

C1H_SwordAttack::C1H_SwordAttack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordAttack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordAttack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	_uint iCurkeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (m_iAttackStartIndex < iCurkeyFrameIndex && m_iAttackEndIndex > iCurkeyFrameIndex)
	{
		m_pSilvermane->Set_IsAttack(true);
		if (!m_isShake)
		{
			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(m_tShakeEvent, vPos);
			m_isShake = true;
		}
	}
	else
		m_pSilvermane->Set_IsAttack(false);


	return _int();
}

_int C1H_SwordAttack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordAttack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordAttack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_tShakeEvent.fDuration = 0.4f;
	m_tShakeEvent.fBlendOutTime = 0.3f;
	m_tShakeEvent.tWaveX.fAmplitude = 0.06f;
	m_tShakeEvent.tWaveX.fFrequency = 10.f;
	m_tShakeEvent.tWaveY.fAmplitude = 0.06f;
	m_tShakeEvent.tWaveY.fFrequency = 6.f;
	m_tShakeEvent.tWaveZ.fAmplitude = 0.02f;
	m_tShakeEvent.tWaveZ.fFrequency = 8.f;

	return S_OK;
}

HRESULT C1H_SwordAttack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	if(m_pSilvermane->IsAttack())
		m_pSilvermane->Set_IsAttack(false);

	return S_OK;
}

void C1H_SwordAttack::OnTriggerEnter(CCollision& collision)
{
	if (m_iCutIndex > m_pAnimationController->Get_CurKeyFrameIndex())
		return;
}

_int C1H_SwordAttack::Input(const _double& _dDeltaTime)
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
					if (FAILED(m_pStateController->Change_State(L"1H_SwordEquipOff")))
						return -1;
				}
				return STATE_CHANGE;
			}
		}
	}


	return _int();
}

void C1H_SwordAttack::Free()
{

	__super::Free();
}
