#include "pch.h"
#include "1H_SwordAttackNormalR2_ReleaseStab.h"

C1H_SwordAttackNormalR2_ReleaseStab::C1H_SwordAttackNormalR2_ReleaseStab(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordAttackNormalR2_ReleaseStab::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordAttackNormalR2_ReleaseStab::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


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
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordAttackNormalR2_ReleaseStab::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordAttackNormalR2_ReleaseStab::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_ReleaseStab::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Player_Sword_Attack);
	g_pGameInstance->Play_Shot(L"Needle_Attack_R_Relese", CSoundMgr::CHANNELID::Player_Sword_Attack);

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Normal_R2_Release_Stab", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}


	m_iCutIndex = 26;
	m_iAttackStartIndex = 10;
	m_iAttackEndIndex = 20;
	//m_iTrailStartIndex = 10;
	//m_iTrailEndIndex = 25;


	// ½¦ÀÌÅ© ¿É¼Ç
	// 2
	m_tShakeEvent2.fDuration = 1.2f;
	m_tShakeEvent2.fBlendInTime = 0.2f;
	m_tShakeEvent2.fBlendOutTime = 0.4f;
	m_tShakeEvent2.tWaveY.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveY.fFrequency = 1.f;
	m_tShakeEvent2.tWaveY.fAdditionalOffset = -0.1f;
	m_tShakeEvent2.tWaveZ.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveZ.fFrequency = 1.f;
	m_tShakeEvent2.tWaveZ.fAdditionalOffset = 0.3f;

	m_pSilvermane->RimlightCheck(false);
	m_pSilvermane->Get_CurerntWeapon()->RimlightCheck(false);

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR2_ReleaseStab::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_isShake2 = false;

	return S_OK;
}

_int C1H_SwordAttackNormalR2_ReleaseStab::Input(const _double& _dDeltaTime)
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
			if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_02")))
				return E_FAIL;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C1H_SwordAttackNormalR2_ReleaseStab* C1H_SwordAttackNormalR2_ReleaseStab::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordAttackNormalR2_ReleaseStab* pInstance = new C1H_SwordAttackNormalR2_ReleaseStab(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordAttackNormalR1_01 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1H_SwordAttackNormalR2_ReleaseStab::Free()
{

	__super::Free();
}
