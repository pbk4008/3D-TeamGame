#include "pch.h"
#include "2H_HammerAttackR1_02.h"

#include "StateController.h"

C2H_HammerAttackR1_02::C2H_HammerAttackR1_02(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerAttackR1_02::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerAttackR1_02::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (iCurKeyFrameIndex == m_iShakeIndex)
	{
		if (!m_isShake2)
		{
			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(m_tShakeEvent2, vPos);
			m_isShake2 = true;
		}
	}


	if (38 < iCurKeyFrameIndex && !m_isRangeAttack)
	{
		m_pSilvermane->RangeAttack();
		m_isRangeAttack = true;
	}


	if (m_pAnimationController->Is_Finished())
	{
		m_pStateController->Change_State(L"2H_HammerIdle");
		return STATE_CHANGE;
	}

	return _int();
}

_int C2H_HammerAttackR1_02::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerAttackR1_02::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerAttackR1_02::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Attack_R1_02", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);


	m_iCutIndex = 40;
	m_pAnimationController->Set_PlaySpeed(1.2f);

	m_iAttackStartIndex = 30;
	m_iAttackEndIndex = 50;
	m_iShakeIndex = 36;
	return S_OK;
}

HRESULT C2H_HammerAttackR1_02::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PlaySpeed(1.f);
	return S_OK;
}

_int C2H_HammerAttackR1_02::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_03")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_04Swap")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C2H_HammerAttackR1_02* C2H_HammerAttackR1_02::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerAttackR1_02* pInstance = new C2H_HammerAttackR1_02(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerAttackR1_02 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerAttackR1_02::Free()
{

	__super::Free();
}
