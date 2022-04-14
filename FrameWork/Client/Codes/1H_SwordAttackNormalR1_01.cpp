#include "pch.h"
#include "1H_SwordAttackNormalR1_01.h"

#include "StateController.h"

C1H_SwordAttackNormalR1_01::C1H_SwordAttackNormalR1_01(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordAttackNormalR1_01::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C1H_SwordAttackNormalR1_01::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurkeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();

	if (m_pAnimationController->Is_Finished())
	{
		m_pStateController->Change_State(L"1H_SwordIdle");
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordAttackNormalR1_01::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordAttackNormalR1_01::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR1_01::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Player_Sword_Attack);
	g_pGameInstance->Play_Shot(L"Needle_Attack_L_1", CSoundMgr::CHANNELID::Player_Sword_Attack);

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Normal_R1_01", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		CHierarchyNode* pWeaponBone = m_pModel->Get_BoneMatrix("weapon_r");
		m_pSilvermane->Set_WeaponFixedBone(pWeaponBone);
	}
	//m_pAnimationController->Set_PlaySpeed(1.2f);
	m_pAnimationController->Set_PlaySpeed(1.4f);

	m_iCutIndex = 18;

	m_iAttackStartIndex = 15;
	m_iAttackEndIndex = 21;

	m_iTrailStartIndex = 15;
	m_iTrailEndIndex = 25;

	m_pSilvermane->Set_AttackDesc_Dir(EAttackDir::Left);
	return S_OK;
}

HRESULT C1H_SwordAttackNormalR1_01::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PlaySpeed(1.f);
	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Player_Sword_Attack);

	return S_OK;
}

_int C1H_SwordAttackNormalR1_01::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		g_pObserver->Set_IsAttack(false);
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			m_pStateController->Change_State(L"1H_SwordAttackNormalR1_02");
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

C1H_SwordAttackNormalR1_01* C1H_SwordAttackNormalR1_01::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordAttackNormalR1_01* pInstance = new C1H_SwordAttackNormalR1_01(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordAttackNormalR1_01 Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C1H_SwordAttackNormalR1_01::Free()
{

	__super::Free();
}
