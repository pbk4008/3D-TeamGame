#include "pch.h"
#include "1H_SwordAttackNormalR2_Loop.h"

C1H_SwordAttackNormalR2_Loop::C1H_SwordAttackNormalR2_Loop(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordAttackNormalR2_Loop::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordAttackNormalR2_Loop::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_fHoldTime += (_float)_dDeltaTime;

	return _int();
}

_int C1H_SwordAttackNormalR2_Loop::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordAttackNormalR2_Loop::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_Loop::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Normal_R2_Loop", true)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pSilvermane->Set_IsAttack(false);

	if (!m_isShake)
	{
		_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		g_pShakeManager->Shake(m_tShakeEvent, vPos);
		m_isShake = true;
	}
	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_Loop::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	g_pGameInstance->Play_Shot(L"Needle_Attack_R_P2", CSoundMgr::CHANNELID::Player_Sword_Attack);
	m_pSilvermane->Set_IsTrasceCamera(true);
	m_fHoldTime = 0.f;

	m_isShake = false;
	return S_OK;
}

_int C1H_SwordAttackNormalR2_Loop::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getMouseKeyUp(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (0.1f < m_fHoldTime)
		{
			if (m_pStateController->Change_State(L"1H_SwordAttackNormalR2_ReleaseDoubleSwing"))
				return E_FAIL;
			return STATE_CHANGE;
		}
		else
		{
			if (m_pStateController->Change_State(L"1H_SwordAttackNormalR2_ReleaseStab"))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C1H_SwordAttackNormalR2_Loop* C1H_SwordAttackNormalR2_Loop::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordAttackNormalR2_Loop* pInstance = new C1H_SwordAttackNormalR2_Loop(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordAttackNormalR2_Loop Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordAttackNormalR2_Loop::Free()
{

	__super::Free();
}
