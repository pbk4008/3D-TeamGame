#include "pch.h"
#include "2H_HammerAttackSprintR1.h"

#include "StateController.h"
#include "MeshEffect_Test.h"

C2H_HammerAttackSprintR1::C2H_HammerAttackSprintR1(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C2H_HammerAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C2H_HammerAttackSprintR1::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int C2H_HammerAttackSprintR1::Tick(const _double& _dDeltaTime)
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

	if (18 < iCurKeyFrameIndex)
	{
		if (!m_isShake3)
		{
			CCameraShake::SHAKEEVENT tShakeEvent;
			tShakeEvent.fDuration = 3.f;
			tShakeEvent.fBlendInTime = 0.5f;
			tShakeEvent.fBlendOutTime = 2.f;
			tShakeEvent.tWaveY.fAdditionalOffset = 2.f;
			tShakeEvent.tWaveY.fFrequency = 0.01f;
			tShakeEvent.tWaveY.fAmplitude = 0.01f;
			tShakeEvent.tWaveZ.fAdditionalOffset = -4.f;
			tShakeEvent.tWaveZ.fFrequency = 0.01f;
			tShakeEvent.tWaveZ.fAmplitude = 0.01f;

			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(tShakeEvent, vPos);
			m_isShake3 = true;
		}
	}

	if (iCurKeyFrameIndex <= 35)
	{
		m_fMTAcc += g_fDeltaTime;
		if (0.05f < m_fMTAcc)
		{
			m_pSilvermane->Create_MotionTrail(m_motiontrailidx);
			++m_motiontrailidx;
			m_fMTAcc = 0.f;
		}

		if (m_motiontrailidx >= 20)
			m_motiontrailidx = 0;
	}

	if (35 < iCurKeyFrameIndex && !m_isRangeAttack)
	{
		// ¸Þ½¬ ÀÌÆåÆ®
		m_pSilvermane->Set_LightCheck(true);
		m_pSilvermane->Set_LightColor(XMVectorSet(1.f, 0.f, 0.f, 1.f));
		m_pSilvermane->Set_LightOrigRange(5.f);
		m_pSilvermane->Set_LightAmbientSpecular(_float4(0.8f, 0.8f, 0.8f, 1.f), _float4(0.5f, 0.5f, 0.5f, 1.f));

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
		m_pStateController->Change_State(L"2H_HammerIdle");
		return STATE_CHANGE;
	}

	return _int();
}

_int C2H_HammerAttackSprintR1::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C2H_HammerAttackSprintR1::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C2H_HammerAttackSprintR1::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_2H_Hammer_Attack_SprintR1", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);
	if (!m_pSilvermane->IsEquipWeapon())
	{
		m_pSilvermane->Set_EquipWeapon(true);
		m_pSilvermane->Set_WeaponFixedBone("weapon_r");
	}

	m_tShakeEvent2.tWaveY.fAdditionalOffset = -0.8f;
	m_tShakeEvent2.tWaveZ.fAdditionalOffset = 0.6f;
	m_tShakeEvent2.tWaveZ.fFrequency = 1.f;
	m_tShakeEvent2.tWaveZ.fAmplitude = 0.04f;

	m_iCutIndex = 40;
	m_iAttackStartIndex = 30;
	m_iAttackEndIndex = 50;
	m_iShakeIndex = 36;
	return S_OK;
}

HRESULT C2H_HammerAttackSprintR1::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_isShake3 = false;
	return S_OK;
}

_int C2H_HammerAttackSprintR1::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	
	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackR1_02")))
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

C2H_HammerAttackSprintR1* C2H_HammerAttackSprintR1::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C2H_HammerAttackSprintR1* pInstance = new C2H_HammerAttackSprintR1(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C2H_HammerAttackSprintR1 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C2H_HammerAttackSprintR1::Free()
{

	__super::Free();
}
