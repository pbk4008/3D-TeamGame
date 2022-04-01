#include "pch.h"
#include "1H_SwordSupermanStab.h"

#include "StateController.h"

C1H_SwordSupermanStab::C1H_SwordSupermanStab(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordSupermanStab::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 22;

	return S_OK;
}

_int C1H_SwordSupermanStab::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);

	_uint iCurkeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();


	if (m_iAttackStartIndex < iCurkeyFrameIndex && m_iAttackEndIndex > iCurkeyFrameIndex)
	{
		if (!m_isShake2)
		{
			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(m_tShakeEvent2, vPos);
			m_isShake2 = true;
		}
	}


	if (m_pAnimationController->Is_Finished())
	{
		m_pStateController->Change_State(L"1H_SwordIdle");
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordSupermanStab::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordSupermanStab::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordSupermanStab::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->BlendSound(L"Needle_Attack_R_Slam_2", L"Needle_Attack_L_4_1", CSoundMgr::CHANNELID::Player_Sword_Attack, CSoundMgr::CHANNELID::PLAYER1);
	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Superman_Stab", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}


	m_iAttackStartIndex = 14;
	m_iAttackEndIndex = 28;


	// Ω¶¿Ã≈© ø…º«
	// 2
	m_tShakeEvent2.fDuration = 1.2f;
	m_tShakeEvent2.fBlendInTime = 0.2f;
	m_tShakeEvent2.fBlendOutTime = 0.4f;
	m_tShakeEvent2.tWaveX.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveX.fFrequency = 1.f;
	m_tShakeEvent2.tWaveX.fAdditionalOffset = -0.1f;
	m_tShakeEvent2.tWaveY.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveY.fFrequency = 1.f;
	m_tShakeEvent2.tWaveY.fAdditionalOffset = -0.5f;
	m_tShakeEvent2.tWaveZ.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveZ.fFrequency = 1.f;
	m_tShakeEvent2.tWaveZ.fAdditionalOffset = 0.3f;


	return S_OK;
}

HRESULT C1H_SwordSupermanStab::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_isShake2 = false;
	return S_OK;
}

_int C1H_SwordSupermanStab::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR1_01")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_Start")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C1H_SwordSupermanStab* C1H_SwordSupermanStab::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordSupermanStab* pInstance = new C1H_SwordSupermanStab(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordSupermanStab Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordSupermanStab::Free()
{

	__super::Free();
}
