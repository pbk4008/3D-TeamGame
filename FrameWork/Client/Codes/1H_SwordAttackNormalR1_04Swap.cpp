#include "pch.h"
#include "1H_SwordAttackNormalR1_04Swap.h"

#include "StateController.h"
#include "MeshEffect_Test.h"

C1H_SwordAttackNormalR1_04Swap::C1H_SwordAttackNormalR1_04Swap(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: C1H_SwordAttack(_pDevice, _pDeviceContext)
{
}

HRESULT C1H_SwordAttackNormalR1_04Swap::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_iCutIndex = 50;

	return S_OK;
}

_int C1H_SwordAttackNormalR1_04Swap::Tick(const _double& _dDeltaTime)
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


	if (42 < iCurkeyFrameIndex && !m_isRangeAttack)
	{
		// 이펙트
		_vector Pos = { 0.f, 0.03f, 0.f ,0.f };
		m_pSilvermane->Active_Effect((_uint)EFFECT::ATTACK_GROUND, Pos);
		m_pSilvermane->Active_Effect((_uint)EFFECT::ATTACK_GROUND_2, Pos); 

		Pos = { 0.f, 0.1f, 0.f ,0.f };
		m_pSilvermane->Active_Effect((_uint)EFFECT::HIT_GROUND_SMOKE, Pos);

		// 라이트
		m_pSilvermane->OnLight();

		// 메쉬 이펙트
		_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector svLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
		_vector svUp = m_pTransform->Get_State(CTransform::STATE_UP);
		CMeshEffect::DESC tMeshEffectDesc;
		XMStoreFloat3(&tMeshEffectDesc.vPosition, svPos + (svLook * 2.f) + (svUp * 0.1f));
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_pSilvermane->getSceneID(), L"Layer_Effect", L"Proto_GameObject_MeshEffect_Test", &tMeshEffectDesc)))
			return E_FAIL;

		// 범위 공격 
		m_pSilvermane->RangeAttack();
		m_isRangeAttack = true;
	}


	if (m_pAnimationController->Is_Finished())
	{
		m_pStateController->Change_State(L"1H_SwordIdle");
		return STATE_CHANGE;
	}

	return _int();
}

_int C1H_SwordAttackNormalR1_04Swap::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT C1H_SwordAttackNormalR1_04Swap::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT C1H_SwordAttackNormalR1_04Swap::EnterState()
{
	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Player_Sword_Attack);
	g_pGameInstance->Play_Shot(L"Needle_Attack_R_Slam_1", CSoundMgr::CHANNELID::Player_Sword_Attack);
	g_pGameInstance->Play_Shot(L"Needle_Attack_R_Slam_2", CSoundMgr::CHANNELID::PLAYER1);

	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_1H_Sword_Attack_Normal_R1_04_Swap", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	m_pAnimationController->Add_TrackAcc(15.0);
	m_iAttackStartIndex = 36;
	m_iAttackEndIndex = 42;

	m_iTrailStartIndex = 36;
	m_iTrailEndIndex = 42;

	// 쉐이크 옵션
	m_tShakeEvent.fDuration = 0.8f;
	m_tShakeEvent.fBlendInTime = 0.2f;
	m_tShakeEvent.fBlendOutTime = 0.6f;
	m_tShakeEvent.tWaveX.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveX.fFrequency = 12.f;
	m_tShakeEvent.tWaveY.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveY.fFrequency = 8.f;
	m_tShakeEvent.tWaveZ.fAmplitude = 0.04f;
	m_tShakeEvent.tWaveZ.fFrequency = 10.f;
	// 2
	m_tShakeEvent2.fDuration = 1.6f;
	m_tShakeEvent2.fBlendInTime = 0.2f;
	m_tShakeEvent2.fBlendOutTime = 0.4f;
	m_tShakeEvent2.tWaveX.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveX.fFrequency = 1.f;
	m_tShakeEvent2.tWaveX.fAdditionalOffset = -0.5f;
	m_tShakeEvent2.tWaveY.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveY.fFrequency = 1.f;
	m_tShakeEvent2.tWaveY.fAdditionalOffset = -1.f;
	m_tShakeEvent2.tWaveZ.fAmplitude = 0.04f;
	m_tShakeEvent2.tWaveZ.fFrequency = 1.f;
	m_tShakeEvent2.tWaveZ.fAdditionalOffset = 1.f;


	return S_OK;
}

HRESULT C1H_SwordAttackNormalR1_04Swap::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_isShake2 = false;
	m_isRangeAttack = false;
	return S_OK;
}

_int C1H_SwordAttackNormalR1_04Swap::Input(const _double& _dDeltaTime)
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

C1H_SwordAttackNormalR1_04Swap* C1H_SwordAttackNormalR1_04Swap::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	C1H_SwordAttackNormalR1_04Swap* pInstance = new C1H_SwordAttackNormalR1_04Swap(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("C1H_SwordAttackNormalR1_04Swap Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C1H_SwordAttackNormalR1_04Swap::Free()
{

	__super::Free();
}
