#include "pch.h"
#include "..\Headers\Silvermane.h"

#include "Camera_Silvermane.h"
#include "Needle.h"
#include "Fury.h"
#include "Shield.h"

#pragma region 스테이트들
#include "Silvermane_Idle.h"

//////////////////// Jog
#include "Silvermane_JogBwd.h"
#include "Silvermane_JogBwdStart.h"
#include "Silvermane_JogBwdPivot180.h"

#include "Silvermane_JogFwd.h"
#include "Silvermane_JogFwdPivot180.h"
#include "Silvermane_JogFwdStart.h"
#include "Silvermane_JogFwdStop.h"

#include "Silvermane_JogLeft.h"
#include "Silvermane_JogLeftPivot180.h"
#include "Silvermane_JogLeftStart.h"

#include "Silvermane_JogRight.h"
#include "Silvermane_JogRightPivot180.h"
#include "Silvermane_JogRightStart.h"

///////////////////////////////////// Sprint
#include "Silvermane_SprintFwd.h"
#include "Silvermane_SprintFwdStart.h"
#include "Silvermane_SprintFwdStop.h"

//////////////////////////////////// 1H_Sword
#include "1H_SwordEquipOff.h"
#include "1H_SwordEquipOn.h"
#include "1H_SwordIdle.h"
// Jog
#include "1H_SwordJogBwd.h"
#include "1H_SwordJogBwdPivot180.h"
#include "1H_SwordJogBwdStart.h"
#include "1H_SwordJogFwd.h"
#include "1H_SwordJogFwdPivot180.h"
#include "1H_SwordJogFwdStart.h"
#include "1H_SwordJogLeft.h"
#include "1H_SwordJogLeftPivot180.h"
#include "1H_SwordJogLeftStart.h"
#include "1H_SwordJogRight.h"
#include "1H_SwordJogRightPivot180.h"
#include "1H_SwordJogRightStart.h"

// Attack
#include "1H_SwordAttackNormalR1_01.h"
#include "1H_SwordAttackNormalR1_02.h"
#include "1H_SwordAttackNormalR1_03.h"
#include "1H_SwordAttackNormalR1_04.h"
#include "1H_SwordAttackNormalR1_04Swap.h"

#include "1H_SwordJogAttack.h"
#include "1H_SwordSupermanStab.h"

#include "1H_SwordDodgeSpinFwd_V3.h"
#include "1H_SwordNormalSidestepBwd_V3.h"
#include "1H_SwordNormalSidestepLeft_V3.h"
#include "1H_SwordNormalSidestepRight_V3.h"

#include "1H_SwordAttackNormalR2_Start.h"
#include "1H_SwordAttackNormalR2_Loop.h"
#include "1H_SwordAttackNormalR2_ReleaseStab.h" // 이게 첫번쨰 공격같은데 왜 ReleaseStab?
#include "1H_SwordAttackNormalR2_02.h"
#include "1H_SwordAttackNormalR2_03.h"
#include "1H_SwordAttackNormalR2_04.h"
#include "1H_SwordAttackNormalR2_ReleaseDoubleSwing.h"

//////////////////////// 2H_Hammer
#include "2H_HammerEquipOff.h"
#include "2H_HammerEquipOn.h"
#include "2H_HammerIdle.h"

// Jog
#include "2H_HammerJogBwd.h"
#include "2H_HammerJogBwdPivot180.h"
#include "2H_HammerJogBwdStart.h"
#include "2H_HammerJogFwd.h"
#include "2H_HammerJogFwdPivot180.h"
#include "2H_HammerJogFwdStart.h"
#include "2H_HammerJogLeft.h"
#include "2H_HammerJogLeftPivot180.h"
#include "2H_HammerJogLeftStart.h"
#include "2H_HammerJogRight.h"
#include "2H_HammerJogRightPivot180.h"
#include "2H_HammerJogRightStart.h"

//Attack
#include "2H_HammerAttackR1_01.h"
#include "2H_HammerAttackR1_02.h"
#include "2H_HammerAttackR1_03.h"
#include "2H_HammerAttackR1_04.h"
#include "2H_HammerAttackR1_04Swap.h"
#include "2H_HammerAttackJogR1.h"

#include "2H_HammerChargeStage1_Start.h"
#include "2H_HammerChargeStage1_Loop.h"
#include "2H_HammerAttackR2_01.h"
#include "2H_HammerAttackR2_02.h"
#include "2H_HammerAttackR2_03.h"
#include "2H_HammerAttackR2_Charge.h"

#include "2H_HammerAttackDodgeR1.h"
#include "2H_HammerAttackSprintR1.h"

///////////////////////////////////////////// Shield
#include "Shield_BlockStart.h"
#include "Shield_BlockLoop.h"
#include "Shield_BlockEnd.h"

// Walk
#include "Shield_WalkBwd.h"
#include "Shield_WalkBwdStart.h"
#include "Shield_WalkBwdStop.h"
#include "Shield_WalkFwd.h"
#include "Shield_WalkFwdStart.h"
#include "Shield_WalkFwdStop.h"
#include "Shield_WalkLeft.h"
#include "Shield_WalkLeftStart.h"
#include "Shield_WalkLeftStop.h"
#include "Shield_WalkRight.h"
#include "Shield_WalkRightStart.h"
#include "Shield_WalkRightStop.h"

// Attack
#include "Shield_SupermanPunchStraight.h"
#pragma endregion


CSilvermane::CSilvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
{
}

CSilvermane::CSilvermane(const CSilvermane& _rhs)
	: CActor(_rhs)
{
}

HRESULT CSilvermane::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg))) 
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	if (FAILED(Ready_States())) 
		return E_FAIL;
	if (FAILED(Ready_Weapons())) 
		return E_FAIL;

	g_pObserver->Set_PlayerTransCom(m_pTransform);

	return S_OK;
}

_int CSilvermane::Tick(_double _dDeltaTime)
{
	m_pAnimationController->Set_MoveSpeed(4.f);
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	m_pTransform->Set_Velocity(XMVectorZero());

	iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress && STATE_CHANGE != iProgress) 
		return iProgress;

	iProgress = Trace_CameraLook(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	iProgress = m_pAnimationController->Tick(_dDeltaTime, CAnimationController::EType::Transform);
	if (NO_EVENT != iProgress) 
		return iProgress;

	m_pCharacterController->Tick(_dDeltaTime);
	//m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());

	// 무기 업뎃
	if (m_pCurWeapon)
	{
		iProgress = m_pCurWeapon->Tick(_dDeltaTime);
		if (NO_EVENT != iProgress) 
			return iProgress;
	}

	/*if (m_pShield && m_pShield->getActive())
	{
		iProgress = m_pShield->Tick(_dDeltaTime);
		if (NO_EVENT != iProgress)
			return iProgress;
	}*/

	return _int();
}

_int CSilvermane::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	//m_pCharacterController->Update_OwnerTransform();

	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	if(FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;

	// 무기 레잇업뎃
	if (m_pCurWeapon)
	{
		iProgress = m_pCurWeapon->LateTick(_dDeltaTime);
		if (NO_EVENT != iProgress) 
			return iProgress;
	}
	if (m_pShield && m_pShield->getActive())
	{
		iProgress = m_pShield->LateTick(_dDeltaTime);
		if (NO_EVENT != iProgress) 
			return iProgress;
	}

	g_pObserver->Set_PlayerPos(m_pTransform->Get_State(CTransform::STATE_POSITION));


	return _int();
}

HRESULT CSilvermane::Render()
{
	if (FAILED(__super::Render())) 
		return E_FAIL;

#ifdef _DEBUG
	m_pCharacterController->Render();
#endif // _DEBUG

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_CombinedMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if(FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) 
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) 
		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		//if (FAILED(m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE))) return E_FAIL;

		if (FAILED(m_pModel->Render(i, 0)))
			return E_FAIL;
	}

#ifdef _DEBUG
	if (FAILED(m_pAnimationController->Render())) 
		return E_FAIL;
	if (FAILED(m_pStateController->Render())) 
		return E_FAIL;
	wstring wstrAngle = L"Angle : " + to_wstring(m_fAngle);
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrAngle.c_str(), _float2(0.f, 300.f), _float2(0.8f, 0.8f))))
		return E_FAIL;
	wstring wstrPlusAngle = L"Plus Angle : " + to_wstring(m_fPlusAngle);
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrPlusAngle.c_str(), _float2(0.f, 340.f), _float2(0.8f, 0.8f))))
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CSilvermane::Ready_Components()
{
	// 트랜스폼 설정
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 10.f;
	tTransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(tTransformDesc);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	// 모델
	if (FAILED(SetUp_Components(m_iSceneID, L"Model_Silvermane", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Top"), 0);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Down"), 1);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Cloak"), 2);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Hair"), 3);

	// 에니메이션 컨트롤러
	if (FAILED(SetUp_Components(m_iSceneID, L"Proto_Component_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);
	m_pAnimationController->Set_MoveSpeed(2.f);

	// 스테이트 컨트롤러
	if (FAILED(SetUp_Components(m_iSceneID, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	// 캐릭터 컨트롤러
	CCharacterController::CHARACTERCONTROLLERDESC tCharacterControllerDesc;
	tCharacterControllerDesc.fHeight = 1.6f;
	tCharacterControllerDesc.fRadius = 0.5f;
	tCharacterControllerDesc.fStaticFriction = 0.5f;
	tCharacterControllerDesc.fDynamicFriction = 0.5f;
	tCharacterControllerDesc.fRestitution = 0.f;
	tCharacterControllerDesc.pGameObject = this;
	tCharacterControllerDesc.vPosition = { 0.f, 0.8f, 0.f };

	if (FAILED(SetUp_Components(m_iSceneID, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCharacterControllerDesc)))
		return E_FAIL;
	m_pCharacterController->Set_OwnerTransform(m_pTransform);

	return S_OK;
}

HRESULT CSilvermane::Ready_States()
{
#pragma region Default
	if (FAILED(m_pStateController->Add_State(L"Idle", CSilvermane_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogBwd", CSilvermane_JogBwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogBwdStart", CSilvermane_JogBwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogBwdPivot180", CSilvermane_JogBwdPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogFwd", CSilvermane_JogFwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogFwdPivot180", CSilvermane_JogFwdPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogFwdStart", CSilvermane_JogFwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogFwdStop", CSilvermane_JogFwdStop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogLeft", CSilvermane_JogLeft::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogLeftPivot180", CSilvermane_JogLeftPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogLeftStart", CSilvermane_JogLeftStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogRight", CSilvermane_JogRight::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogRightPivot180", CSilvermane_JogRightPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogRightStart", CSilvermane_JogRightStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"SprintFwd", CSilvermane_SprintFwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"SprintFwdStart", CSilvermane_SprintFwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"SprintFwdStop", CSilvermane_SprintFwdStop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion
#pragma region 1H_Sword
	if (FAILED(m_pStateController->Add_State(L"1H_SwordEquipOn", C1H_SwordEquipOn::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordEquipOff", C1H_SwordEquipOff::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordIdle", C1H_SwordIdle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// Jog
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogBwd", C1H_SwordJogBwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogBwdPivot180", C1H_SwordJogBwdPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogBwdStart", C1H_SwordJogBwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogFwd", C1H_SwordJogFwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogFwdPivot180", C1H_SwordJogFwdPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogFwdStart", C1H_SwordJogFwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogLeft", C1H_SwordJogLeft::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogLeftPivot180", C1H_SwordJogLeftPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogLeftStart", C1H_SwordJogLeftStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogRight", C1H_SwordJogRight::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogRightPivot180", C1H_SwordJogRightPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogRightStart", C1H_SwordJogRightStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// Attack
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR1_01", C1H_SwordAttackNormalR1_01::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR1_02", C1H_SwordAttackNormalR1_02::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR1_03", C1H_SwordAttackNormalR1_03::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR1_04", C1H_SwordAttackNormalR1_04::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR1_04Swap", C1H_SwordAttackNormalR1_04Swap::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordJogAttack", C1H_SwordJogAttack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordSupermanStab", C1H_SwordSupermanStab::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR2_Start", C1H_SwordAttackNormalR2_Start::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR2_Loop", C1H_SwordAttackNormalR2_Loop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR2_ReleaseStab", C1H_SwordAttackNormalR2_ReleaseStab::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR2_02", C1H_SwordAttackNormalR2_02::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR2_03", C1H_SwordAttackNormalR2_03::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR2_04", C1H_SwordAttackNormalR2_04::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR2_ReleaseDoubleSwing", C1H_SwordAttackNormalR2_ReleaseDoubleSwing::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_DodgeSpin", C1H_SwordDodgeSpinFwd_V3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SidestepBwd", C1H_SwordNormalSidestepBwd_V3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SidestepLeft", C1H_SwordNormalSidestepLeft_V3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SidestepRight", C1H_SwordNormalSidestepRight_V3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion
#pragma region 2H_Hammer
	if (FAILED(m_pStateController->Add_State(L"2H_HammerEquipOn", C2H_HammerEquipOn::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerEquipOff", C2H_HammerEquipOff::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerIdle", C2H_HammerIdle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// Jog
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogBwd", C2H_HammerJogBwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogBwdPivot180", C2H_HammerJogBwdPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogBwdStart", C2H_HammerJogBwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogFwd", C2H_HammerJogFwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogFwdPivot180", C2H_HammerJogFwdPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogFwdStart", C2H_HammerJogFwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogLeft", C2H_HammerJogLeft::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogLeftPivot180", C2H_HammerJogLeftPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogLeftStart", C2H_HammerJogLeftStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogRight", C2H_HammerJogRight::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogRightPivot180", C2H_HammerJogRightPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerJogRightStart", C2H_HammerJogRightStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// Attack
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackR1_01", C2H_HammerAttackR1_01::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackR1_02", C2H_HammerAttackR1_02::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackR1_03", C2H_HammerAttackR1_03::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackR1_04", C2H_HammerAttackR1_04::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackR1_04Swap", C2H_HammerAttackR1_04Swap::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackJogR1", C2H_HammerAttackJogR1::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerChargeStage1_Start", C2H_HammerChargeStage1_Start::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerChargeStage1_Loop", C2H_HammerChargeStage1_Loop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackR2_01", C2H_HammerAttackR2_01::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackR2_02", C2H_HammerAttackR2_02::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackR2_03", C2H_HammerAttackR2_03::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackR2_Charge", C2H_HammerAttackR2_Charge::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackDodgeR1", C2H_HammerAttackDodgeR1::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerAttackSprintR1", C2H_HammerAttackSprintR1::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion
#pragma region Shield
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockStart", CShield_BlockStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockLoop", CShield_BlockLoop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockEnd", CShield_BlockEnd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// Walk
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkBwd", CShield_WalkBwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkBwdStart", CShield_WalkBwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkBwdStop", CShield_WalkBwdStop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkFwd", CShield_WalkFwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkFwdStart", CShield_WalkFwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkFwdStop", CShield_WalkFwdStop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkLeft", CShield_WalkLeft::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkLeftStart", CShield_WalkLeftStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkLeftStop", CShield_WalkLeftStop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkRight", CShield_WalkRight::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkRightStart", CShield_WalkRightStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_WalkRightStop", CShield_WalkRightStop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// Attack
	if (FAILED(m_pStateController->Add_State(L"Shield_SupermanPunchStraight", CShield_SupermanPunchStraight::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CState_Silvermane*>(pair.second)->Set_Silvermane(this);
		static_cast<CState_Silvermane*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CState_Silvermane*>(pair.second)->Set_Model(m_pModel);
		static_cast<CState_Silvermane*>(pair.second)->Set_AnimationController(m_pAnimationController);
	}
	m_pStateController->Change_State(L"Idle");
	return S_OK;
}

HRESULT CSilvermane::Ready_Weapons()
{
	CHierarchyNode* pWeaponBone = m_pModel->Get_BoneMatrix("spine_03");
	CWeapon* pWeapon = nullptr;
	// 한손검
	pWeapon = CNeedle::Create(m_pDevice, m_pDeviceContext);
	pWeapon->NativeConstruct(m_iSceneID, pWeaponBone);
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	m_umapWeapons.emplace(L"Needle", pWeapon);
	m_pCurWeapon = pWeapon;
	// 해머
	pWeapon = CFury::Create(m_pDevice, m_pDeviceContext);
	pWeapon->NativeConstruct(m_iSceneID, pWeaponBone);
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	m_umapWeapons.emplace(L"Fury", pWeapon);

	// 방패
	/*pWeaponBone = m_pModel->Get_BoneMatrix("weapon_l");
	m_pShield = CShield::Create(m_pDevice, m_pDeviceContext);
	m_pShield->NativeConstruct(m_iSceneID, pWeaponBone);
	m_pShield->Set_Owner(this);
	m_pShield->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	Set_EquipShield(false);*/
	
	return S_OK;
}

CTransform* CSilvermane::Get_Transform() const
{
	return m_pTransform;
}

CModel* CSilvermane::Get_Model() const
{
	return m_pModel;
}

const _float CSilvermane::Get_PlusAngle() const
{
	return m_fPlusAngle;
}

const _float CSilvermane::Get_Angle() const
{
	return m_fAngle;
}

void CSilvermane::Set_Move(const _bool _isMove)
{
	m_isMove = _isMove;
}

void CSilvermane::Set_TrasceCamera(const _bool _isTraceCamera)
{
	m_isTraceCamera = _isTraceCamera;
}

void CSilvermane::Set_EquipWeapon(const _bool _isEquipWeapon)
{
	m_isEquipWeapon = _isEquipWeapon;
	m_pCurWeapon->Set_Equip(m_isEquipWeapon);
}

void CSilvermane::Set_WeaponFixedBone(const string& _sstrFixedBoneTag)
{
	CHierarchyNode* pFixedBone = m_pModel->Get_BoneMatrix(_sstrFixedBoneTag.c_str());
	if(pFixedBone)
		m_pCurWeapon->Set_FixedBone(pFixedBone);
}

void CSilvermane::Set_WeaponFixedBone(CHierarchyNode* _pFixedBone)
{
	if (m_pCurWeapon)
		m_pCurWeapon->Set_FixedBone(_pFixedBone);
}

void CSilvermane::Set_Camera(CCamera_Silvermane* _pCamera)
{
	m_pCamera = _pCamera;
}

void CSilvermane::Set_PlusAngle(const _float _fAngle)
{
	m_fPlusAngle = _fAngle;
}

const _bool CSilvermane::Is_EquipWeapon() const
{
	return m_isEquipWeapon;
}

const _bool CSilvermane::Is_EquipShield() const
{
	return m_isEquipShield;
}

const CWeapon::EType CSilvermane::Get_WeaponType() const
{
	return m_pCurWeapon->Get_Type();
}

void CSilvermane::Add_PlusAngle(const _float _fDeltaAngle)
{
	//m_fPlusAngle += _fDeltaAngle * 400.f;

	if (0 > _fDeltaAngle)
		m_fPlusAngle -= 2.f;
	else
		m_fPlusAngle += 2.f;

	if (360.f < m_fPlusAngle || -360.f > m_fPlusAngle)
		m_fPlusAngle = fmodf(m_fPlusAngle, 360.f);
}

const _bool CSilvermane::Change_Weapon(const wstring& _name)
{
	CWeapon* pWeapon = m_umapWeapons[_name];
	if (!pWeapon)
		return false;
	if (m_pCurWeapon == pWeapon)
		return false;
	
	if (m_pCurWeapon)
	{
		Set_EquipWeapon(false);
		Set_WeaponFixedBone("spine_03");
	}
	m_pCurWeapon = pWeapon;
	return true;
}

HRESULT CSilvermane::Change_State(const wstring& _wstrStateTag)
{
	return m_pStateController->Change_State(_wstrStateTag);
}

void CSilvermane::Set_EquipShield(const _bool _isEquipShield)
{
	if (m_pShield)
	{
		m_pShield->Set_Equip(_isEquipShield);
		m_isEquipShield = _isEquipShield;
	}
}

_int CSilvermane::Trace_CameraLook(const _double& _dDeltaTime)
{
	_vector svCameraLook = m_pCamera->Get_Look();
	_vector svLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	_vector svUp = m_pTransform->Get_State(CTransform::STATE_UP);

	svCameraLook = XMVector3Normalize(XMVectorSetY(svCameraLook, 0.f));
	svLook = XMVector3Normalize(XMVectorSetY(svLook, 0.f));

	if (m_isMove)
	{
		// 추가로 달리기와 같이 더 회전이 필요한 동작들을 회전시켜주는 부분입네다.
		_float fPlusRadian = XMConvertToRadians(m_fPlusAngle);

		//_matrix smatPlusPivot = XMMatrixRotationAxis(svUp, fPlusRadian);
		_matrix smatPlusPivot = XMMatrixRotationY(fPlusRadian);
		_vector svRotCameraLook = XMVector4Transform(svCameraLook, smatPlusPivot);
		_vector svRotCameraRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), svRotCameraLook);
		_vector svRotCameraUp = XMVector3Cross(svRotCameraLook, svRotCameraRight);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, svRotCameraRight);
		m_pTransform->Set_State(CTransform::STATE_UP, svRotCameraUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, svRotCameraLook);
	}
	else
	{
		_vector svAngle = XMVector3AngleBetweenVectors(svCameraLook, svLook);
		_float fRadian;
		XMStoreFloat(&fRadian, svAngle);
		_vector svCross = XMVector3Cross(svLook, svCameraLook);
		if (0.f < XMVectorGetY(svCross)) // 카메라가 왼쪽
		{
			if(m_isTraceCamera)
				m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), (fRadian)*_dDeltaTime * 5.f);
			m_fAngle = XMConvertToDegrees(fRadian);
		}
		else if (0.f > XMVectorGetY(svCross)) // 카메라가 오른쪽
		{
			if (m_isTraceCamera)
				m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), (fRadian) * -_dDeltaTime * 5.f);
			m_fAngle = -XMConvertToDegrees(fRadian);
		}
		else
			m_fAngle = 0.f;
	}

 	return _int();
}

CSilvermane* CSilvermane::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CSilvermane* pInstance = new CSilvermane(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CSilvermane Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSilvermane::Clone(const _uint _iSceneID, void* _pArg)
{
	CSilvermane* pInstance = new CSilvermane(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CSilvermane Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSilvermane::Free()
{
	Safe_Release(m_pShield);
	for (auto& pair : m_umapWeapons)
		Safe_Release(pair.second);
	m_umapWeapons;

	Safe_Release(m_pCharacterController);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimationController);
	Safe_Release(m_pModel);

	__super::Free();
}
