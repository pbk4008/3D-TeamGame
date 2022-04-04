#include "pch.h"
#include "..\Headers\Silvermane.h"

#include "Camera_Silvermane.h"
#include "Needle.h"
#include "Fury.h"
#include "Shield.h"
#include "JumpNode.h"
#include "JumpTrigger.h"
#include "JumpBox.h"
#include "UI_Blank_CKey.h"
#include "UI_Fill_CKey.h"

#include "MainApp.h"

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
#include "Shield_BlockSkid.h"
#include "Shield_Parry.h"

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

//////////////////////////////////////////// Jump
#include "Traverse_Jump400Jog.h"
#include "Traverse_JumpNodeJog.h"

//////////////////////////////////////////// Hit
#include "1H_FlinchLeft.h"
#include "1H_KnockBack_Land.h"
#include "Silvermane_KnockBackFwd.h"
#include "Silvermane_KnockBackBwd.h"
#pragma endregion

#include "Material.h"

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

	m_iObectTag = (_uint)GAMEOBJECT::PLAYER;

	return S_OK;
}

HRESULT CSilvermane::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (_pArg)
	{
		SCENEMOVEDATA tDesc = (*(SCENEMOVEDATA*)_pArg);
		_vector vPos = XMLoadFloat3(&tDesc.vPos);
		vPos = XMVectorSetW(vPos, 1.f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(2.f, 1.f, -1.f, 1.f));

	if (FAILED(Ready_Components()))
		return E_FAIL;
	if (FAILED(Ready_States()))
		return E_FAIL;
	if (FAILED(Ready_Weapons()))
		return E_FAIL;

	/*g_pObserver->Set_PlayerTransCom(m_pTransform);*/
	//생성될때 옵저버에 플레이어 셋팅
	if (FAILED(g_pObserver->Set_Player(this)))
		return E_FAIL;

	m_isFall = true;
	m_fCurrentHp = 100.f;
	m_fMaxHp = 100.f;

	m_pRenderer->SetRenderButton(CRenderer::PIXEL, true);
	m_pRenderer->SetRenderButton(CRenderer::PBRHDR, true);
	m_pRenderer->SetCameraTag(L"Camera_Silvermane");

	return S_OK;
}

_int CSilvermane::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	m_pTransform->Set_Velocity(XMVectorZero());

	iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress && STATE_CHANGE != iProgress)
		return iProgress;
	g_pObserver->Set_IsAttack(m_IsAttack);

	iProgress = Trace_CameraLook(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	iProgress = m_pAnimationController->Tick(_dDeltaTime, CAnimationController::EType::CharacterController);
	if (NO_EVENT != iProgress)
		return iProgress;

	iProgress = Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_isFall)
		m_pTransform->Fall(_dDeltaTime);
	//m_pCharacterController->Tick(_dDeltaTime);
	if (m_isHighSpeedMode)
	{
		_float3 vVelocity = m_pTransform->Get_Velocity();
		XMStoreFloat3(&vVelocity, XMLoadFloat3(&vVelocity) * 2.f);
		m_pCharacterController->Move(_dDeltaTime, vVelocity);
	}
	else
	{
		m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());
	}



	Raycast_JumpNode(_dDeltaTime);


	// 무기 업뎃
	if (m_pCurWeapon)
	{
		iProgress = m_pCurWeapon->Tick(_dDeltaTime);
		if (NO_EVENT != iProgress)
			return iProgress;
	}
	if (m_pShield && m_pShield->getActive())
	{
		iProgress = m_pShield->Tick(_dDeltaTime);
		if (NO_EVENT != iProgress)
			return iProgress;
	}

	return _int();
}

_int CSilvermane::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pCharacterController->Update_OwnerTransform();

	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pRenderer->Get_Shadow() == true)
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_SHADOW, this))) return -1;
	}

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
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


	//Raycast_Camera();

	//g_pObserver->Set_PlayerPos(m_pTransform->Get_State(CTransform::STATE_POSITION));
	g_pGameInstance->UpdateLightCam(0, m_pTransform->Get_State(CTransform::STATE_POSITION));

	return _int();
}

HRESULT CSilvermane::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, i)))	return E_FAIL;
	}

#ifdef _DEBUG
	Render_Debug();
#endif

	return S_OK;
}

HRESULT CSilvermane::Render_Shadow()
{
	_matrix world, lightview, lightproj;
	_float3 lightpos = m_Lightdesc->vPosition;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	lightview = XMMatrixTranspose(m_Lightdesc->mLightView);
	lightproj = XMMatrixTranspose(m_Lightdesc->mLightProj);

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_LightView", &lightview, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_LightProj", &lightproj, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_LightPos", &lightpos, sizeof(_float3));


	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 4);

	return S_OK;
}

HRESULT CSilvermane::Render_ShadeShadow(ID3D11ShaderResourceView* pshadow)
{
	_matrix world, view, proj, lightview, lightproj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	lightview = XMMatrixTranspose(m_Lightdesc->mLightView);
	lightproj = XMMatrixTranspose(m_Lightdesc->mLightProj);

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_LightView", &lightview, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_LightProj", &lightproj, sizeof(_matrix));


	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_ShadowTexture", pshadow);
		m_pModel->Render(i, 5);
	}

	return S_OK;
}

HRESULT CSilvermane::Render_Debug()
{
	if (FAILED(m_pAnimationController->Render()))
		return E_FAIL;
	if (FAILED(m_pStateController->Render()))
		return E_FAIL;

	// FSM
	wstring wstrCurStateTag = m_pStateController->Get_CurStateTag();
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrCurStateTag.c_str(), _float2(0.f, 40.f), _float2(0.6f, 0.6f))))
		return E_FAIL;


	// 애니메이션 상태들
	wstring wstrCurKeyFrameIndex = to_wstring(m_pAnimationController->Get_CurKeyFrameIndex());
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrCurKeyFrameIndex.c_str(), _float2(0.f, 60.f), _float2(0.6f, 0.6f))))
		return E_FAIL;
	wstring wstrCurIndex = to_wstring(m_pAnimationController->Get_CurFixedBoneKeyFrameIndex());
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrCurIndex.c_str(), _float2(0.f, 80.f), _float2(0.6f, 0.6f))))
		return E_FAIL;
	string strCurAnimTag = m_pAnimationController->Get_CurAnimTag();
	wstring wstrCurAnimTag;
	wstrCurAnimTag.assign(strCurAnimTag.begin(), strCurAnimTag.end());
	wstrCurAnimTag = wstrCurAnimTag.substr(wstrCurAnimTag.find_last_of(L"|") + 1);
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrCurAnimTag.c_str(), _float2(0.f, 100.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	wstring wstrAnimFinished = L"AnimFinished : ";
	m_pAnimationController->Is_Finished() == true ? wstrAnimFinished += L"true" : wstrAnimFinished += L"false";
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrAnimFinished.c_str(), _float2(0.f, 120.f), _float2(0.6f, 0.6f))))
		return E_FAIL;


	// 카메라와의 각도
	wstring wstrAngle = L"Angle : " + to_wstring(m_fAngle);
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrAngle.c_str(), _float2(0.f, 140.f), _float2(0.6f, 0.6f))))
		return E_FAIL;
	// 플러스 각도
	wstring wstrPlusAngle = L"Plus Angle : " + to_wstring(m_fPlusAngle);
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrPlusAngle.c_str(), _float2(0.f, 160.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	// 위치
	_float3 vPosition;
	XMStoreFloat3(&vPosition, m_pTransform->Get_State(CTransform::STATE_POSITION));
	wstring wstrPosition = L"Position : " + to_wstring(vPosition.x) + L", " + to_wstring(vPosition.y) + L", " + to_wstring(vPosition.z);
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrPosition.c_str(), _float2(0.f, 180.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	// 공격 상태
	wstring wstrIsAttack = L"IsAttack : ";
	m_IsAttack == true ? wstrIsAttack += L"true" : wstrIsAttack += L"false";
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrIsAttack.c_str(), _float2(0.f, 200.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane::Ready_Components()
{
	// 트랜스폼 설정
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 10.f;
	tTransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(tTransformDesc);
	//m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	//위치 셋팅은 위에서 해줬음

	// 모델
	// 어심프용
	//if (FAILED(SetUp_Components(m_iSceneID, L"Model_Silvermane", L"Model", (CComponent**)&m_pModel)))
	//	return E_FAIL;
	//m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Top"), 0);
	//m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Down"), 1);
	//m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Cloak"), 2);
	//m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Hair"), 3);
	// 바이너리용
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Silvermane_Bin", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);


	// 에니메이션 컨트롤러
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);
	m_pAnimationController->Set_MoveSpeed(36.f);
	m_fMoveSpeed = 3.6f;
	m_fCurrentHp = 100.f;

	// 스테이트 컨트롤러
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	// 캐릭터 컨트롤러
	CCharacterController::DESC tCharacterControllerDesc;
	tCharacterControllerDesc.fHeight = 1.f;
	tCharacterControllerDesc.fRadius = 0.5f;
	tCharacterControllerDesc.fContactOffset = tCharacterControllerDesc.fRadius * 0.1f;
	tCharacterControllerDesc.fStaticFriction = 0.5f;
	tCharacterControllerDesc.fDynamicFriction = 0.5f;
	tCharacterControllerDesc.fRestitution = 0.f;
	tCharacterControllerDesc.vPosition = { 0.f, 0.f, 0.f };
	//tCharacterControllerDesc.fSlopeLimit = 45.f;
	tCharacterControllerDesc.pGameObject = this;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCharacterControllerDesc)))
		return E_FAIL;
	m_pCharacterController->setOwnerTransform(m_pTransform);

	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pTexture->Change_Texture(L"Texture_SilvermeanNewHair");

	m_pModel->Get_Materials()[3]->Set_Texture("g_NewHairTexture", TEXTURETYPE::TEX_TINT, m_pTexture);

	m_Lightdesc = g_pGameInstance->Get_LightDesc(0);

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
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockSkid", CShield_BlockSkid::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_Parry", CShield_Parry::Create(m_pDevice, m_pDeviceContext))))
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
	// 짬푸
	if (FAILED(m_pStateController->Add_State(L"Traverse_Jump400Jog", CTraverse_Jump400Jog::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Traverse_JumpNodeJog", CTraverse_JumpNodeJog::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// 쳐맞음
	if (FAILED(m_pStateController->Add_State(L"1H_FlinchLeft", C1H_FlinchLeft::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_KnockBack_Land", C1H_KnockBack_Land::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"KnockBackBwd", CSilvermane_KnockBackBwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"KnockBackFwd", CSilvermane_KnockBackFwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

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
	if (FAILED(pWeapon->NativeConstruct(m_iSceneID, pWeaponBone)))
	{
		Safe_Release(pWeapon);
		return E_FAIL;
	}
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	m_umapWeapons.emplace(L"Needle", pWeapon);
	m_pCurWeapon = pWeapon;
	// 해머
	pWeapon = CFury::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pWeapon->NativeConstruct(m_iSceneID, pWeaponBone)))
	{
		Safe_Release(pWeapon);
		return E_FAIL;
	}
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	pWeapon->Set_Equip(false);
	m_umapWeapons.emplace(L"Fury", pWeapon);
	// 방패
	pWeaponBone = m_pModel->Get_BoneMatrix("weapon_l");
	m_pShield = CShield::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(m_pShield->NativeConstruct(m_iSceneID, pWeaponBone)))
	{
		Safe_Release(m_pShield);
		return E_FAIL;
	}
	m_pShield->Set_Owner(this);
	m_pShield->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	Set_EquipShield(false);

	return S_OK;
}

void CSilvermane::OnCollisionEnter(CCollision& collision)
{
}

void CSilvermane::OnCollisionStay(CCollision& collision)
{
}

void CSilvermane::OnCollisionExit(CCollision& collision)
{
}

void CSilvermane::OnTriggerEnter(CCollision& collision)
{
	m_pStateController->OnTriggerEnter(collision);
}

void CSilvermane::OnTriggerExit(CCollision& collision)
{
	m_pStateController->OnTriggerExit(collision);
}	

// 점프노드와 같은 오브젝트 충돌용 함수입네다. 
// 무기에 쳐맞는건 State_Silvermane.cpp 로 가주세용
void CSilvermane::OnControllerColliderHit(CCollision& collision)
{
	CGameObject* pHitObject = collision.pGameObject;
	_uint iTag = pHitObject->getTag();

	//점프ui관련
	m_pFillCKey = (CUI_Fill_Ckey*)g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_FillC")->front();

	if (m_pTargetJumpBox)
	{
		_fvector vBoxPos = m_pTargetJumpBox->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		_fvector vDist = vBoxPos - m_pTransform->Get_State(CTransform::STATE_POSITION);
		_float fBoxToPlayer = XMVectorGetX(XMVector3Length(vDist));

		if (5.f < fBoxToPlayer)
		{
			m_pFillCKey->Set_GapX(0.f);
			m_pFillCKey->setActive(false);
			//m_pTargetJumpBox = nullptr;
		}
	}
	if (iTag == (_uint)GAMEOBJECT::JUMP_BOX)
	{
		CJumpBox* pJumpBox = static_cast<CJumpBox*>(pHitObject);
		m_pTargetJumpBox = pJumpBox;

		if (g_pGameInstance->getkeyDown(DIK_C))
		{
			m_pTargetJumpBox->DisableCollision();
			m_pStateController->Change_State(L"Traverse_Jump400Jog");
			m_pTargetJumpBox = nullptr;
		}

		if (nullptr != m_pFillCKey)
			m_pFillCKey->Set_JumpNode(false);

		if (nullptr != m_pTargetJumpBox)
		{
			_fvector vBoxPos = m_pTargetJumpBox->Get_Transform()->Get_State(CTransform::STATE_POSITION);
			_fvector vDist = vBoxPos - m_pTransform->Get_State(CTransform::STATE_POSITION);
			_float fBoxToPlayer = XMVectorGetX(XMVector3Length(vDist));

			if (5.f >= fBoxToPlayer)
			{
				if (nullptr != m_pFillCKey)
				{
					m_pFillCKey->Set_GapX(1.f);
					m_pFillCKey->setActive(true);
				}
			}
		}
	}
}

void CSilvermane::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_isBlock)
		static_cast<CState_Silvermane*>(m_pStateController->Get_CurState())->Block(_tAttackDesc);
	else
		static_cast<CState_Silvermane*>(m_pStateController->Get_CurState())->Hit(_tAttackDesc);
}

const _bool CSilvermane::IsHit() const
{
	return m_isHit;
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

const CSilvermane::SCENEMOVEDATA CSilvermane::Get_SceneMoveData() const
{
	SCENEMOVEDATA tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));


	//현재 체력 및 기타 추가해야할 변수
	tDesc.iMaxHp = 100;
	tDesc.iCurHp = 100;

	return tDesc;
}

void CSilvermane::Set_IsHit(const _bool _isHit)
{
	m_isHit = _isHit;
}

void CSilvermane::Set_IsFall(const _bool _isFall)
{
	m_isFall = _isFall;
}

void CSilvermane::Set_IsMove(const _bool _isMove)
{
	m_isMove = _isMove;
}

void CSilvermane::Set_IsTrasceCamera(const _bool _isTraceCamera)
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
	if (pFixedBone)
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

const _bool CSilvermane::IsEquipWeapon() const
{
	return m_isEquipWeapon;
}

const _bool CSilvermane::IsEquipShield() const
{
	return m_isEquipShield;
}

const CWeapon::EType CSilvermane::Get_WeaponType() const
{
	return m_pCurWeapon->Get_Type();
}

void CSilvermane::Set_IsAttack(const _bool _isAttack)
{
	m_IsAttack = _isAttack;
	if (m_pCurWeapon)
		m_pCurWeapon->Set_IsAttack(_isAttack);
}

void CSilvermane::Add_PlusAngle(const _float _fDeltaAngle)
{
	m_fPlusAngle += _fDeltaAngle * 400.f;

	//if (0 > _fDeltaAngle)
	//	m_fPlusAngle -= 2.f;
	//else
	//	m_fPlusAngle += 2.f;

	if (360.f < m_fPlusAngle || -360.f > m_fPlusAngle)
		m_fPlusAngle = fmodf(m_fPlusAngle, 360.f);
}

void CSilvermane::Add_Velocity(const CTransform::STATE _eState, const _double& _dDeltaTime)
{
	m_pTransform->Add_Velocity(_eState, m_fMoveSpeed * (_float)_dDeltaTime);
}

void CSilvermane::Add_HP(const _float _fValue)
{
	m_fCurrentHp += _fValue;
}

void CSilvermane::Set_Position(const _float3 _vPosition)
{
	m_pCharacterController->setPosition(_vPosition);
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

const _float CSilvermane::Get_BlockTime() const
{
	return m_fBlockTime;
}

void CSilvermane::Set_EquipShield(const _bool _isEquipShield)
{
	if (m_pShield)
	{
		if (m_isEquipShield != _isEquipShield)
		{
			m_pShield->Set_Equip(_isEquipShield);
			m_isEquipShield = _isEquipShield;
			m_isBlock = _isEquipShield;
		}
	}
}

void CSilvermane::Set_EquipShieldAnim(const _bool _isEquipShield)
{
	static_cast<CShield*>(m_pShield)->Set_EquipAnim(_isEquipShield);
}

void CSilvermane::Set_BlockTime(const _float _fValue)
{
	m_fBlockTime = _fValue;
}

void CSilvermane::Set_IsShieldAttack(const _bool _isAttack)
{
	if (m_pShield)
		m_pShield->Set_IsAttack(_isAttack);
}

void CSilvermane::Add_BlockTime(const _float _fValue)
{
	m_fBlockTime += _fValue;
}

const _int CSilvermane::Trace_CameraLook(const _double& _dDeltaTime)
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
			if (m_isTraceCamera)
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

const _int CSilvermane::Input(const _double& _dDeltaTime)
{
	if (g_pGameInstance->getkeyDown(DIK_HOME))
	{
		m_pCharacterController->setFootPosition(_float3(0.f, 2.f, 0.f));
		m_isFall = true;
	}
	if (g_pGameInstance->getkeyDown(DIK_NUMPAD9))
		Set_Position(_float3(-175.f, 53.f, 384.f));

	if (g_pGameInstance->getkeyDown(DIK_MINUS))
		m_isFall = !m_isFall;
	if (g_pGameInstance->getkeyDown(DIK_0))
		m_isHighSpeedMode = !m_isHighSpeedMode;

	if (!m_isFall)
	{
		if (g_pGameInstance->getkeyPress(DIK_UP))
			m_pTransform->Add_Velocity(XMVectorSet(0.f, 40.f * (_float)_dDeltaTime, 0.f, 0.f));
		if (g_pGameInstance->getkeyPress(DIK_DOWN))
			m_pTransform->Add_Velocity(XMVectorSet(0.f, -40.f * (_float)_dDeltaTime, 0.f, 0.f));
	}

	return _int();
}

CJumpNode* CSilvermane::Get_TargetJumpNode() const
{
	return m_pTargetJumpNode;
}

CJumpTrigger* CSilvermane::Get_TargetJumpTrigger() const
{
	return m_pTargetJumpTrigger;
}

CJumpBox* CSilvermane::Get_TargetJumpBox() const
{
	return m_pTargetJumpBox;
}

const _bool CSilvermane::Raycast_JumpNode(const _double& _dDeltaTime)
{
	_matrix smatView;
	smatView = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	smatView = XMMatrixInverse(nullptr, smatView);
	if (XMMatrixIsNaN(smatView))
		return false;

	_vector svRayPos, svRayDir;
	memcpy_s(&svRayPos, sizeof(_vector), &smatView.r[3], sizeof(_vector));
	memcpy_s(&svRayDir, sizeof(_vector), &smatView.r[2], sizeof(_vector));
	svRayDir = XMVector3Normalize(svRayDir);
	_float fOutDist = 0.f;


	_uint iObjectTag = -1;

	svRayPos += svRayDir * 6.f;
	RAYCASTDESC tRaycastDesc;
	XMStoreFloat3(&tRaycastDesc.vOrigin, svRayPos);
	XMStoreFloat3(&tRaycastDesc.vDir, svRayDir);
	tRaycastDesc.fMaxDistance = 50.f;
	tRaycastDesc.filterData.flags = PxQueryFlag::eANY_HIT | PxQueryFlag::eDYNAMIC;
	CGameObject* pHitObject = nullptr;
	tRaycastDesc.ppOutHitObject = &pHitObject;
	if (g_pGameInstance->Raycast(tRaycastDesc))
	{
		if (pHitObject)
		{
			iObjectTag = pHitObject->getTag();
		}
	}

	//점프ui관련
	m_pBlankCKey = (CUI_Blank_CKey*)g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_BlankC")->front();
	m_pFillCKey = (CUI_Fill_Ckey*)g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_FillC")->front();

	if ((_uint)GAMEOBJECT::JUMP_TRIGGER == iObjectTag)
	{
		m_pTargetJumpTrigger = static_cast<CJumpTrigger*>(pHitObject);
		if (g_pGameInstance->getkeyPress(DIK_C))
			m_fJumpTriggerLookTime += (_float)_dDeltaTime;

		if (1.f < m_fJumpTriggerLookTime)
		{
			if (FAILED(m_pStateController->Change_State(L"Traverse_Jump400Jog")))
				return false;
			m_fJumpTriggerLookTime = 0.f;
		}

		return true;
	}
	else if ((_uint)GAMEOBJECT::JUMP_NODE == iObjectTag)
	{
		m_pTargetJumpNode = static_cast<CJumpNode*>(pHitObject);
		m_pTargetJumpNode->setIsPick(true);
		if (g_pGameInstance->getkeyPress(DIK_C))
			m_fJumpNodeLookTime += (_float)_dDeltaTime;

		if (nullptr != m_pFillCKey)
			m_pFillCKey->Set_JumpNode(true);

		if (1.f < m_fJumpNodeLookTime)
		{
			if (FAILED(m_pStateController->Change_State(L"Traverse_JumpNodeJog")))
				return false;
			m_fJumpNodeLookTime = 0.f;
		}
		else if (1.f >= m_fJumpNodeLookTime)
		{
			if (nullptr != m_pBlankCKey && false == m_pBlankCKey->getActive())
			{
				m_pBlankCKey->setActive(true);
				m_pFillCKey->setActive(true);
			}

		}
		return true;
	}
	else
	{
		if (m_pTargetJumpNode)
		{
			m_pTargetJumpNode->setIsPick(false);
			m_pTargetJumpNode = nullptr;
		}
		if (m_pTargetJumpTrigger)
		{
			m_pTargetJumpTrigger = nullptr;
		}
		m_fJumpNodeLookTime = 0.f;
		m_fJumpTriggerLookTime = 0.f;

		if (nullptr != m_pBlankCKey)
		{
			m_pBlankCKey->setActive(false);
		}

		if (nullptr != m_pFillCKey && nullptr == m_pTargetJumpBox)
		{
			m_pFillCKey->Set_GapX(0.f);
			m_pFillCKey->setActive(false);
			m_pFillCKey->Set_JumpNode(false);
		}
	}

	return false;
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
	m_umapWeapons.clear();

	Safe_Release(m_pCharacterController);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimationController);
	Safe_Release(m_pModel);

	__super::Free();
}
