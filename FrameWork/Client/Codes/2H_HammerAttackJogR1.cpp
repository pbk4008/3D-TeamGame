#include "pch.h"
#include "2H_HammerAttackJogR1.h"

#include "StateController.h"

#include "MeshEffect_Test.h"

C2H_HammerAttackJogR1::C2H_HammerAttackJogR1(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerAttackJogR1::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerAttackJogR1::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);



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

	if (26 < iCurKeyFrameIndex && !m_isRangeAttack)
	{
		// ¸Þ½¬ ÀÌÆåÆ®
		_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector svLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
		_vector svUp = m_pTransform->Get_State(CTransform::STATE_UP);
		CMeshEffect::DESC tMeshEffectDesc;
		XMStoreFloat3(&tMeshEffectDesc.vPosition, svPos + (svLook * 2.f) + (svUp * 0.1f));
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_pSilvermane->getSceneID(), L"Layer_Effect", L"Proto_GameObject_MeshEffect_Test", &tMeshEffectDesc)))
			return E_FAIL;
		// ÀÌÆåÆ®
		m_pSilvermane->Active_Effect((_uint)EFFECT::ATTACK_GROUND, (svLook * 2.f) + (svUp * 0.1f));
		m_pSilvermane->Active_Effect((_uint)EFFECT::ATTACK_GROUND_2, (svLook * 2.f) + (svUp * 0.1f));

		m_pSilvermane->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE, (svLook * 2.f) + (svUp * 0.1f));

		m_pSilvermane->RangeAttack();
		m_isRangeAttack = true;
	}


	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

_int C2H_HammerAttackJogR1::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerAttackJogR1::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerAttackJogR1::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Attack_JogR1", false)))
		return -1;
	m_pAnimationController->Set_RootMotion(true, true);
	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}


	m_iCutIndex = 10;
	m_pAnimationController->Set_PlaySpeed(1.2f);

	m_iAttackStartIndex = 20;
	m_iAttackEndIndex = 30;
	m_iShakeIndex = 26;
	return S_OK;
}

HRESULT C2H_HammerAttackJogR1::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_PlaySpeed(1.f);
	m_isShake2 = false;
	return S_OK;
}

_int C2H_HammerAttackJogR1::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_01")))
				return -1;
			return STATE_CHANGE;
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerChargeStage1_Start")))
				return -1;
			return STATE_CHANGE;
		}
	}

	return _int();
}

C2H_HammerAttackJogR1* C2H_HammerAttackJogR1::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerAttackJogR1* pInstance = new C2H_HammerAttackJogR1(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerAttackJogR1 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerAttackJogR1::Free()
{

	__super::Free();
}
