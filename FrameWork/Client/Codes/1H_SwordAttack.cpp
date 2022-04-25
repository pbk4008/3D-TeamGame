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


	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (m_iAttackStartIndex < iCurKeyFrameIndex && m_iAttackEndIndex > iCurKeyFrameIndex)
	{
		m_pSilvermane->Set_IsAttack(true);
		if (!m_isShake)
		{
			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(m_tShakeEvent, vPos);
			m_isShake = true;
		}
		m_pSilvermane->Set_IsTrasceCamera(false);
	}
	else
		m_pSilvermane->Set_IsAttack(false);

	if (m_iTrailStartIndex < iCurKeyFrameIndex && m_iTrailEndIndex > iCurKeyFrameIndex)
		m_pSilvermane->Set_IsTrail(true);
	else
		m_pSilvermane->Set_IsTrail(false);


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
	m_tShakeEvent.tWaveX.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveX.fFrequency = 10.f;
	m_tShakeEvent.tWaveY.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveY.fFrequency = 6.f;
	m_tShakeEvent.tWaveZ.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveZ.fFrequency = 8.f;

	m_pAnimationController->Set_PlaySpeed(1.2f);
	g_pObserver->Set_PlayerAttackAnimStart(true);

	m_pAnimationController->Mul_MoveSpeed(0.5f);
	return S_OK;
}

HRESULT C1H_SwordAttack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	if(m_pSilvermane->IsAttack())
		m_pSilvermane->Set_IsAttack(false);
	m_pSilvermane->Set_IsTrail(false);

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pAnimationController->Set_PlaySpeed(1.f);
	g_pObserver->Set_PlayerAttackAnimStart(false);

	m_pAnimationController->Div_MoveSpeed(0.5f);
	return S_OK;
}

void C1H_SwordAttack::OnTriggerEnter(CCollision& collision)
{
}

void C1H_SwordAttack::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (!m_pSilvermane->IsAttack())
		__super::Hit(_tAttackDesc);
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
			if (m_pSilvermane->IsLootShield())
			{
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
