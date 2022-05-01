#include "pch.h"
#include "DodgeSlide.h"

CDodgeSlide::CDodgeSlide(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CDodgeSlide::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CDodgeSlide::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (iCurKeyFrameIndex >= 10)
	{
		if (m_radialcnt > 1)
		{
			m_radialcnt--;
			m_pSilvermane->Set_RadialCnt(m_radialcnt);
		}
		else
			m_pSilvermane->Set_Radial(false);
	}
	else
	{
		if (m_radialcnt < 8)
		{
			m_radialcnt++;
			m_pSilvermane->Set_RadialCnt(m_radialcnt);
		}
	}

	switch (m_eDir)
	{
	case EDir::Forward:
	case EDir::Left:
	case EDir::Right:
	case EDir::Backward:
		Add_PlusAngle(EDir::Forward, _dDeltaTime);
		break;
	case EDir::LeftForward:
	case EDir::RightBackward:
		Add_PlusAngle(EDir::RightForward, _dDeltaTime);
		break;
	case EDir::LeftBackward:
	case EDir::RightForward:
		Add_PlusAngle(EDir::LeftForward, _dDeltaTime);
		break;
	}

	if (m_pAnimationController->Is_Finished())
		return ToIdle();

	return _int();
}

_int CDodgeSlide::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CDodgeSlide::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDodgeSlide::EnterState(void* _pArg)
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (_pArg)
		memcpy_s(&m_eDir, sizeof(EDir), _pArg, sizeof(EDir));

	_vector Pos = { 0.f, 0.5f, 0.f, 0.f };

	CCameraShake::SHAKEEVENT tShakeEvent;
	tShakeEvent.fDuration = 1.f;
	tShakeEvent.fBlendInTime = 0.2f;
	tShakeEvent.fBlendOutTime = 0.8f;
	tShakeEvent.tWaveY.fAmplitude = 1.f;
	tShakeEvent.tWaveY.fFrequency = 0.1f;
	tShakeEvent.tWaveY.fAdditionalOffset = 0.2f;
	tShakeEvent.tWaveZ.fAmplitude = 1.f;
	tShakeEvent.tWaveZ.fFrequency = 1.f;
	tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;

	_matrix newmat = g_pObserver->Get_PlayerWorldMatrix();
	newmat.r[3] = XMVectorSetY(newmat.r[3], XMVectorGetY(newmat.r[3]) - 0.8f);

	switch (m_eDir)
	{
	case EDir::Forward:
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Fwd_Player_NEW", false)))
			return E_FAIL;

		m_pSilvermane->Active_Effect_Target((_uint)EFFECT::DASH, XMMatrixRotationY(XMConvertToRadians(180.f)) * newmat);

		break;
	case EDir::Backward:
		tShakeEvent.fDuration = 1.4f;
		tShakeEvent.fBlendOutTime = 0.3f;
		tShakeEvent.tWaveY.fFrequency = 0.4f;
		tShakeEvent.tWaveZ.fAdditionalOffset = -4.f;
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Back_Player_NEW", false)))
			return E_FAIL;

		m_pSilvermane->Active_Effect_Target((_uint)EFFECT::DASH, XMMatrixRotationY(XMConvertToRadians(0.f)) * newmat);

		break;
	case EDir::Left:
		tShakeEvent.tWaveZ.fAdditionalOffset = -2.f;
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Left_Player_NEW", false)))
			return E_FAIL;

		m_pSilvermane->Active_Effect_Target((_uint)EFFECT::DASH, XMMatrixRotationY(XMConvertToRadians(90.f)) * newmat);

		break;
	case EDir::LeftForward:
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Left_Player_NEW", false)))
			return E_FAIL;

		m_pSilvermane->Active_Effect_Target((_uint)EFFECT::DASH, XMMatrixRotationY(XMConvertToRadians(90.f)) * newmat);

		break;
	case EDir::LeftBackward:
		tShakeEvent.fDuration = 1.4f;
		tShakeEvent.fBlendOutTime = 0.3f;
		tShakeEvent.tWaveY.fFrequency = 0.4f;
		tShakeEvent.tWaveZ.fAdditionalOffset = -4.f;
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Left_Player_NEW", false)))
			return E_FAIL;

		m_pSilvermane->Active_Effect_Target((_uint)EFFECT::DASH, XMMatrixRotationY(XMConvertToRadians(90.f)) * newmat);

		break;
	case EDir::Right:
		tShakeEvent.tWaveZ.fAdditionalOffset = -2.f;
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Right_Player_NEW", false)))
			return E_FAIL;

		m_pSilvermane->Active_Effect_Target((_uint)EFFECT::DASH, XMMatrixRotationY(XMConvertToRadians(270.f)) * newmat);

		break;
	case EDir::RightForward:
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Right_Player_NEW", false)))
			return E_FAIL;

		m_pSilvermane->Active_Effect_Target((_uint)EFFECT::DASH, XMMatrixRotationY(XMConvertToRadians(270.f)) * newmat);

		break;
	case EDir::RightBackward:
		tShakeEvent.fDuration = 1.4f;
		tShakeEvent.fBlendOutTime = 0.3f;
		tShakeEvent.tWaveY.fFrequency = 0.4f;
		tShakeEvent.tWaveZ.fAdditionalOffset = -4.f;
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Right_Player_NEW", false)))
			return E_FAIL;

		m_pSilvermane->Active_Effect_Target((_uint)EFFECT::DASH, XMMatrixRotationY(XMConvertToRadians(270.f)) * newmat);

		break;
	}
	m_pAnimationController->Set_RootMotion(true, true);

	m_iCutIndex = 18;

	m_pSilvermane->Set_IsMove(true);
	m_pSilvermane->Set_IsDash(true);
	g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_pSilvermane->Set_Radial(true);
	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pAnimationController->Set_PlaySpeed(1.2f);

	g_pGameInstance->BlendSound(L"Player_Dash", L"Player_Dash_1", CSoundMgr::CHANNELID::PLAYER1, CSoundMgr::CHANNELID::PLAYER2);
	return S_OK;
}

HRESULT CDodgeSlide::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsMove(false);
	m_pSilvermane->Set_IsDash(false);
	m_pSilvermane->Set_IsTrasceCamera(true);
	m_pAnimationController->Set_PlaySpeed(1.f);
	return S_OK;
}

_int CDodgeSlide::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_iCutIndex < m_pAnimationController->Get_CurKeyFrameIndex())
	{
		if (g_pGameInstance->getkeyPress(DIK_Q))
		{
			if (FAILED(m_pStateController->Change_State(L"Shield_BlockStart")))
				return E_FAIL;
			return STATE_CHANGE;
		}

		if (g_pGameInstance->getkeyPress(DIK_E))
		{
			return ToSkill();
		}

		if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			return ToDashAttack();
		}
		else if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			return ToChargeStart();
		}

		if (g_pGameInstance->getkeyDown(DIK_SPACE))
		{
			//Reset();
			return ToDash();
		}

		if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
		{
			if (g_pGameInstance->getkeyPress(DIK_W) ||
				g_pGameInstance->getkeyPress(DIK_S) ||
				g_pGameInstance->getkeyPress(DIK_A) ||
				g_pGameInstance->getkeyPress(DIK_D))
			{
				return ToSprint();
			}
		}
	}

	return _int();
}

const _int CDodgeSlide::Reset()
{
	EDir eDir = EDir::Max;
	if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
			eDir = EDir::LeftForward;
		else if (g_pGameInstance->getkeyPress(DIK_S))
			eDir = EDir::LeftBackward;
		else
			eDir = EDir::Left;
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (g_pGameInstance->getkeyPress(DIK_W))
			eDir = EDir::RightForward;
		else if (g_pGameInstance->getkeyPress(DIK_S))
			eDir = EDir::RightBackward;
		else
			eDir = EDir::Right;
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		eDir = EDir::Backward;
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		eDir = EDir::Forward;
	}
	else
	{
		eDir = EDir::Backward;
	}
	if (eDir != m_eDir)
	{
		switch (eDir)
		{
		case EDir::Forward:
			if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Fwd_Player_NEW", false)))
				return -1;
			break;
		case EDir::Backward:
			if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Back_Player_NEW", false)))
				return -1;
			break;
		case EDir::Left:
		case EDir::LeftForward:
		case EDir::LeftBackward:
			if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Left_Player_NEW", false)))
				return -1;
			break;
		case EDir::Right:
		case EDir::RightForward:
		case EDir::RightBackward:
			if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Dodge_Slide_Right_Player_NEW", false)))
				return -1;
			break;
		}
		m_eDir = eDir;
	}
	else
	{
		m_pAnimationController->Reset_Animation();
	}

	return _int();
}

const _int CDodgeSlide::ToDash()
{
	if (g_pGameInstance->getkeyPress(DIK_A))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SidestepLeft")))
			return -1;
	}
	else if (g_pGameInstance->getkeyPress(DIK_S))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
			return -1;
	}
	else if (g_pGameInstance->getkeyPress(DIK_D))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SidestepRight")))
			return -1;
	}
	else if (g_pGameInstance->getkeyPress(DIK_W))
	{
		if (FAILED(m_pStateController->Change_State(L"1H_DodgeSpin")))
			return -1;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"1H_SidestepBwd")))
			return -1;
	}
	return STATE_CHANGE;
}

CDodgeSlide* CDodgeSlide::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CDodgeSlide* pInstance = new CDodgeSlide(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CDodgeSlide Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDodgeSlide::Free()
{
	__super::Free();
}
