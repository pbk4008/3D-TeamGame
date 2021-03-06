#include "pch.h"
#include "..\Headers\Silvermane.h"

#include "Camera_Silvermane.h"
#include "Needle.h"
#include "Fury.h"
#include "Shield.h"
#include "FlyingShield.h"
#include "JumpNode.h"
#include "JumpTrigger.h"
#include "JumpBox.h"
#include "DropBox.h"
#include "UI_Blank_CKey.h"
#include "UI_Blank_FKey.h"
#include "UI_Fill_CKey.h"
#include "UI_Fill_CKey2.h"
#include "InventoryData.h"
#include "EquipmentData.h"
#include "PlayerData.h"
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
///////////////////////////////////// Dash
#include "1H_SwordDodgeSpinFwd_V3.h"
#include "1H_SwordNormalSidestepBwd_V3.h"
#include "1H_SwordNormalSidestepLeft_V3.h"
#include "1H_SwordNormalSidestepRight_V3.h"
#include "DodgeSlide.h"
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

#include "1H_SwordAttackNormalR2_Start.h"
#include "1H_SwordAttackNormalR2_Loop.h"
#include "1H_SwordAttackNormalR2_ReleaseStab.h" // 이게 첫번쨰 공격같은데 왜 ReleaseStab?
#include "1H_SwordAttackNormalR2_02.h"
#include "1H_SwordAttackNormalR2_03.h"
#include "1H_SwordAttackNormalR2_04.h"
#include "1H_SwordAttackNormalR2_ReleaseDoubleSwing.h"
// 패링 당함
#include "1H_SwordRicochetReaction.h"
// 스킬
#include "1H_Multishlash.h"
#include "1H_SwordSkill_2.h"
#include "1H_SwordSkill_3.h"
////////////////////////////////////////////////// 2H_Hammer
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
// 패링 당함
#include "2H_HammerRicochetReaction.h"
// Skills
#include "2H_HammerSkill_1.h"
#include "2H_HammerSkill_2.h"
///////////////////////////////////////////// Shield
// 막기
#include "Shield_BlockStart.h"
#include "Shield_BlockLoop.h"
#include "Shield_BlockEnd.h"
#include "Shield_BlockSoft.h"
#include "Shield_BlockHard.h"
#include "Shield_BlockSkid.h"
#include "Shield_BlockSkidStrong.h"
#include "Shield_BlockBreakStart.h"
#include "Shield_BlockBreakEnd.h"
#include "Shield_Ricochet.h"
// 패링
#include "Shield_Parry.h"
#include "Shield_ParryStunback.h"
#include "Shield_ParryStunbackStrong.h"
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
#include "Shield_Throw.h"
//////////////////////////////////////////// Jump
#include "Traverse_Jump400Jog.h"
#include "Traverse_JumpNodeJog.h"
//////////////////////////////////////////// Hit
#include "1H_FlinchLeft.h"
#include "1H_Stagger.h"
#include "1H_KnockBack.h"
#include "Silvermane_KnockBack.h"
#include "Silvermane_Death.h"
///////////////////////////////////////////// Looting
#include "LootingChest.h"
///////////////////////////////////////////// Heal
#include "Silvermane_Heal.h"
///////////////////////////////////////////// Execution
#include "Silvermane_Execution.h"
///////////////////////////////////////////// Emote
#include "Silvermane_Bow.h"
#pragma endregion

#include "Material.h"
#include "MotionTrail.h"
#include "HierarchyNode.h"
#include "Light.h"

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

	m_pPlayerData = g_pDataManager->GET_DATA(CPlayerData, L"PlayerData");
	m_pInventoryData = g_pDataManager->GET_DATA(CInventoryData, L"InventoryData");

	if (_pArg)
	{
		SCENEMOVEDATA tDesc = (*(SCENEMOVEDATA*)_pArg);
		_vector vPos = XMLoadFloat3(&tDesc.vPos);
		vPos = XMVectorSetW(vPos, 1.f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
		m_vRespawnPos = tDesc.vPos;
	}
	else
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(2.f, 1.f, -1.f, 1.f));

	if (FAILED(Ready_Components()))
		return E_FAIL;
	if (FAILED(Ready_States()))
		return E_FAIL;
	if (FAILED(Ready_Weapons(_iSceneID)))
		return E_FAIL;

	/*g_pObserver->Set_PlayerTransCom(m_pTransform);*/
	//생성될때 옵저버에 플레이어 셋팅
	if (FAILED(g_pObserver->Set_Player(this)))
		return E_FAIL;

	m_isFall = true;
	m_fMaxHp = 200.f;
	m_fCurrentHp = m_fMaxHp;

	m_pRenderer->SetRenderButton(CRenderer::PIXEL, true);
	m_pRenderer->SetRenderButton(CRenderer::PBR, true);		
	m_pRenderer->SetRenderButton(CRenderer::HDR, true);
	m_pRenderer->SetRenderButton(CRenderer::OUTLINE, true);
	m_pRenderer->SetRenderButton(CRenderer::VELOCITYBLUR, true);


	//m_pRenderer->SetRenderButton(CRenderer::SHADOW, true);

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f,5.f, 10.f, 1.f));

	m_pExecutionTargetBone = m_pModel->Get_BoneMatrix("utility_01");

	////////////////////////////////////// UI
	CUI_Blank_CKey::UIACTIVEDESC tUIDesc;
	ZeroMemory(&tUIDesc, sizeof(CUI_Blank_CKey::UIACTIVEDESC));
	_tcscpy_s(tUIDesc.UIDesc.TextureTag, L"Texture_Blank_Fkey");
	tUIDesc.UIDesc.bMinus = false;
	tUIDesc.UIDesc.fAngle = 0.f;
	tUIDesc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	tUIDesc.UIDesc.fSize = { 10.f , 10.f};
	tUIDesc.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI_BlankF", L"Proto_GameObject_UI_Blank_FKey", &tUIDesc, (CGameObject**)&m_pBlankFKey)))
		return E_FAIL;
	if (m_pBlankFKey)
		m_pBlankFKey->setActive(false);
	//Fill_Ckey2
	CUI_Fill_Ckey2::UIACTIVEDESC tUIDesc2;
	ZeroMemory(&tUIDesc2, sizeof(CUI_Fill_Ckey2::UIACTIVEDESC));
	_tcscpy_s(tUIDesc2.UIDesc.TextureTag, L"Texture_Fill_Ckey");
	_tcscpy_s(tUIDesc2.TextureTag2, L"Texture_Blank_Ckey");
	tUIDesc2.UIDesc.bMinus = false;
	tUIDesc2.UIDesc.fAngle = 0.f;
	tUIDesc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	tUIDesc2.UIDesc.fSize = { 10.f , 10.f };
	tUIDesc2.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI_FillC2", L"Proto_GameObject_UI_Fill_CKey2", &tUIDesc2, (CGameObject**)&m_pFillCKey2)))
		return E_FAIL;
	if (m_pFillCKey2)
		m_pFillCKey2->setActive(false);

	Set_AttackDesc_Damaga(10.f);
	m_isLootShield = false;
	m_fSkillGuage = 100.f;

	return S_OK;
}

_int CSilvermane::Tick(_double _dDeltaTime)
{
	if(g_pGameInstance->getCurrentLevel() != 4)
		m_pRenderer->SetRenderButton(CRenderer::SHADOW, true);

	if (g_pMainApp->IsDeltaTimeZero())
		return NO_EVENT;

	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	m_pTransform->Set_Velocity(XMVectorZero());

	if (!m_isSkill)
	{
		m_fSkillGuage += (_float)_dDeltaTime * 2.f;
		if (100.f < m_fSkillGuage)
			m_fSkillGuage = 100.f;
	}

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
	Raycast_DropBox(_dDeltaTime);

	// 무기 업뎃
	if (m_pCurWeapon && m_pCurWeapon->getActive())
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

	if (g_pGameInstance->getkeyDown(DIK_O))
	{
		m_pPlayerData->SetExp(5);
	}

	if (g_pGameInstance->getCurrentLevel() != 3)
		m_isLootShield = true;

	//CActor::Set_RimInten(5.f);
	//CActor::Set_Rimtimer(3.f);
	CActor::LightOnOff(m_pTransform->Get_State(CTransform::STATE_POSITION), m_lightcolor, 15.f);

	return _int();
}

_int CSilvermane::LateTick(_double _dDeltaTime)
{
	if (g_pMainApp->IsDeltaTimeZero())
		return NO_EVENT;

	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pCharacterController->Update_OwnerTransform();

	/* 장비창 실시간 변경 */
	if (g_pInvenUIManager->IsOpenModal())
		Change_WeaponOnInventory();

	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;

	if (m_pRenderer->Get_RenderButton(CRenderer::SHADOW) == true)
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_SHADOW, this))) return -1;
	}

	if (m_pRenderer->Get_RenderButton(CRenderer::VELOCITYBLUR) == true)
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_VELOCITY, this)))
			return -1;
	}

	// 무기 레잇업뎃
	if (m_pCurWeapon && m_pCurWeapon->getActive())
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

	if(g_pGameInstance->getCurrentLevel() == 3)
		g_pGameInstance->UpdateLightCam(0, m_pTransform->Get_State(CTransform::STATE_POSITION));


	return _int();
}

HRESULT CSilvermane::Render()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();

	RIM rimdesc;
	ZeroMemory(&rimdesc, sizeof(RIM));
	rimdesc = ColorChange_RimCheck(rimdesc);

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		SCB desc;
		ZeroMemory(&desc, sizeof(SCB));

		if (i == 0 || i == 1)
		{
			desc.metalic = 0.3f;
			desc.roughness = 0.3f;
			desc.ao = 0.8f;
			desc.color = m_color;
			desc.empower = 0.7f;
			CActor::BindConstantBuffer(wstrCamTag, &desc, &rimdesc);
		}
		else
			CActor::BindConstantBuffer(wstrCamTag, &desc, &rimdesc);

		if (FAILED(m_pModel->Render(i, i))) MSGBOX("Fialed To Rendering Silvermane");
	}
	if (m_pRenderer->Get_RenderButton(CRenderer::VELOCITYBLUR) == false)
		m_PreWroldMat = m_pTransform->Get_WorldMatrix();

	if (m_bhealcheck == true)
	{
		_matrix smatWorld, smatView, smatProj;

		smatWorld = XMMatrixTranspose(m_pWeaponBone->Get_CombinedMatrix() * m_pModel->Get_PivotMatrix() * m_pTransform->Get_WorldMatrix());
		smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
		smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

		if (FAILED(m_pHealSphere->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pHealSphere->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pHealSphere->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

		_float4 color = _float4(0, 1, 0, 1);
		_float empower = 0.5f;

		if (FAILED(m_pHealSphere->SetUp_ValueOnShader("g_color", &color, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pHealSphere->SetUp_ValueOnShader("g_empower", &empower, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

		for (_uint i = 0; i < m_pHealSphere->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pHealSphere->Render(i, 2))) MSGBOX("Fialed To Rendering Silver HealSphere");
		}
	}

#ifdef _DEBUG
	//Render_Debug();
#endif

	return S_OK;
}

HRESULT CSilvermane::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CActor::BindConstantBuffer(wstrCamTag);
	CActor::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 6);

	return S_OK;
}

HRESULT CSilvermane::Render_Velocity()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	SCB desc;
	ZeroMemory(&desc, sizeof(SCB));

	RIM rimdesc;
	ZeroMemory(&rimdesc, sizeof(RIM));

	MOTIONBLUR motion;
	ZeroMemory(&motion, sizeof(MOTIONBLUR));
	// velocity desc
	_float4x4 rot;
	XMStoreFloat4x4(&rot, m_pTransform->Get_WorldMatrix()
					* g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW)
					* g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	rot._11 = 1.0f; rot._22 = 1.0f; rot._33 = 1.0f;
	rot._41 = 0.0f; rot._42 = 0.0f; rot._43 = 0.0f;
	motion.RotationMat = rot;
	_matrix prewvp = g_pGameInstance->GetPreViewProtj(m_PreWroldMat);
	XMStoreFloat4x4(&motion.preWorldViewPorjMat, prewvp);
	//----------------------------------------------------

	CActor::BindConstantBuffer(wstrCamTag, &desc, &rimdesc, &motion);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (i != 2)
		{
			if (FAILED(m_pModel->Render(i, 5))) MSGBOX("Fialed To Rendering Silvermane");
		}
	}

	m_PreWroldMat = m_pTransform->Get_WorldMatrix();
	//m_timer += g_fDeltaTime;
	//if (m_timer >= 0.3f)
	//{
	//	m_PreWroldMat = m_pTransform->Get_WorldMatrix();
	//	m_timer = 0.f;
	//}

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
	m_pCharacterController->setShapeLayer((_uint)ELayer::Player);

	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pTexture->Change_Texture(L"Texture_SilvermeanNewHair");

	m_pModel->Get_Materials()[3]->Set_Texture("g_OtherTexture", TEXTURETYPE::TEX_OTHER, m_pTexture);

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
	// Dash
	if (FAILED(m_pStateController->Add_State(L"1H_DodgeSpin", C1H_SwordDodgeSpinFwd_V3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SidestepBwd", C1H_SwordNormalSidestepBwd_V3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SidestepLeft", C1H_SwordNormalSidestepLeft_V3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SidestepRight", C1H_SwordNormalSidestepRight_V3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"DodgeSlide", CDodgeSlide::Create(m_pDevice, m_pDeviceContext))))
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
	// 패랭 당함
	if (FAILED(m_pStateController->Add_State(L"1H_SwordRicochetReaction", C1H_SwordRicochetReaction::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// 스킬
	if (FAILED(m_pStateController->Add_State(L"1H_Multishlash", C1H_Multishlash::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordSkill_2", C1H_SwordSkill_2::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordSkill_3", C1H_SwordSkill_3::Create(m_pDevice, m_pDeviceContext))))
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
	// 패랭 당함
	if (FAILED(m_pStateController->Add_State(L"2H_HammerRicochetReaction", C2H_HammerRicochetReaction::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// Skills
	if (FAILED(m_pStateController->Add_State(L"2H_HammerSkill_1", C2H_HammerSkill_1::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"2H_HammerSkill_2", C2H_HammerSkill_2::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion
#pragma region Shield
	// 막기
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockStart", CShield_BlockStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockLoop", CShield_BlockLoop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockEnd", CShield_BlockEnd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockSoft", CShield_BlockSoft::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockHard", CShield_BlockHard::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockSkid", CShield_BlockSkid::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockSkidStrong", CShield_BlockSkidStrong::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockBreakStart", CShield_BlockBreakStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_BlockBreakEnd", CShield_BlockBreakEnd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_Ricochet", CShield_Ricochet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// 패링
	if (FAILED(m_pStateController->Add_State(L"Shield_Parry", CShield_Parry::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_ParryStunback", CShield_ParryStunback::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Shield_ParryStunbackStrong", CShield_ParryStunbackStrong::Create(m_pDevice, m_pDeviceContext))))
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
	if (FAILED(m_pStateController->Add_State(L"Shield_Throw", CShield_Throw::Create(m_pDevice, m_pDeviceContext))))
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
	if (FAILED(m_pStateController->Add_State(L"1H_Stagger", C1H_Stagger::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_KnockBack", C1H_KnockBack::Create(m_pDevice, m_pDeviceContext))))
 		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"KnockBack", CSilvermane_KnockBack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Death", CSilvermane_Death::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// 루팅
	if (FAILED(m_pStateController->Add_State(L"LootingChest", CLootingChest::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// 힐
	if (FAILED(m_pStateController->Add_State(L"Silvermane_Heal", CSilvermane_Heal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// Execution
	if (FAILED(m_pStateController->Add_State(L"Execution", CSilvermane_Execution::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	// Emote
	if (FAILED(m_pStateController->Add_State(L"Bow", CSilvermane_Bow::Create(m_pDevice, m_pDeviceContext))))
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

HRESULT CSilvermane::Ready_Weapons(const _uint _iSceneID)
{
	CWeapon* pWeapon = nullptr;

	if (FAILED(g_pWeaponGenerator->NativeConstruct(m_pDevice, m_pDeviceContext, _iSceneID, m_pModel)))
		return E_FAIL;

	/// <summary>
	/// Equipment&Inventory Data와 연동하여 장비 Ready함
	/// </summary>
	/// 
	if (g_pDataManager)
	{
		m_pEquipmentData = g_pDataManager->GET_DATA(CEquipmentData, L"EquipmentData");

		if (1 == m_pPlayerData->EquipedSlot)
		{
			pWeapon = m_pEquipmentData->GetEquipment(EEquipSlot::Weapon1).weaponData.Get_Weapon();

			if (pWeapon)
			{
				pWeapon->Set_Owner(this);
				pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
				pWeapon->setActive(true);

				m_pCurWeapon = pWeapon;
			}
		}
		else if (2 == m_pPlayerData->EquipedSlot)
		{
			pWeapon = m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.Get_Weapon();

			if (pWeapon)
			{
				pWeapon->Set_Owner(this);
				pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
				pWeapon->setActive(true);

				m_pCurWeapon = pWeapon;
			}
		}
	}

	///////////////////////////////////////////////////////////////////// 방패
	m_pWeaponBone = m_pModel->Get_BoneMatrix("weapon_l");
	m_pShield = CShield::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(m_pShield->NativeConstruct(m_iSceneID, m_pWeaponBone)))
	{
		Safe_Release(m_pShield);
		return E_FAIL;
	}
	m_pShield->Set_Owner(this);
	m_pShield->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	Set_EquipShield(false);

	CFlyingShield::DESC tDesc;
	tDesc.pOriginTransform = m_pShield->Get_Transform();
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Weapon", L"Proto_GameObject_FlyingShield", &tDesc, (CGameObject**)&m_pFlyingShield)))
		return E_FAIL;
	if (m_pFlyingShield)
	{
		m_pFlyingShield->Set_Owner(this);
		m_pFlyingShield->Set_FixedBone(m_pWeaponBone);
		m_pFlyingShield->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	}

	///////////////////////////////////////////////////////////////////// 모션 트레일
	for (_int i = 0; i < 20; ++i)
	{
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)m_iSceneID, L"Layer_MotionTrail", L"Proto_GameObject_MotionTrail")))
			return E_FAIL;

		list<CGameObject*>* pobjlist = nullptr;
		pobjlist = g_pGameInstance->getObjectList((_uint)m_iSceneID, L"Layer_MotionTrail");
		CGameObject* pobj = pobjlist->back();
		pobj->setActive(false);
		static_cast<CMotionTrail*>(pobj)->Set_Model(m_pModel, m_pCurWeapon->Get_Model(), m_pFlyingShield->Get_Model());

		m_vecMotionTrail.emplace_back(pobj);
	}

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Player_HealEffect", L"Model_HealShphere", (CComponent**)&m_pHealSphere)))	return E_FAIL;

	if (!m_pHealSphere)
		MSGBOX("HealSphere Null!!");

	//HealBase
	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Needle", L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../bin/Resources/Mesh/Bullet/Dungeons_Texture_01.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_HealSphere", pMtrl);
	m_pHealSphere->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_HealSphere"), 0);

	m_pWeaponBone = m_pModel->Get_BoneMatrix("middle_01_l");

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

	_uint iTag = collision.pGameObject->getTag();
	switch (iTag)
	{
	case (_uint)GAMEOBJECT::TRIGGER:
		_vector svOtherPos = collision.pGameObject->Get_Transform()->Get_State(CTransform::STATE_POSITION);

		XMStoreFloat3(&m_vRespawnPos, svOtherPos);
		break;
	}
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
	list<CGameObject*>* listFillKeys = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_FillC");
	if (listFillKeys)
	{
		m_pFillCKey = (CUI_Fill_Ckey*)listFillKeys->front();
	}

	if (m_pTargetJumpBox)
	{
		_float fBoxToPlayer = MathUtils::Length(m_pTargetJumpBox, this);

		if (5.f < fBoxToPlayer)
		{
			if (m_pFillCKey)
			{
				m_pFillCKey->Set_GapX(0.f);
				m_pFillCKey->setActive(false);
			}
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

		if (m_pFillCKey)
			m_pFillCKey->Set_JumpNode(false);

		if (m_pTargetJumpBox)
		{
			_float fBoxToPlayer = MathUtils::Length(m_pTargetJumpBox, this);

			if (5.f >= fBoxToPlayer)
			{
				if (nullptr != m_pFillCKey)
				{
					if (m_pFillCKey)
					{
						m_pFillCKey->Set_GapX(1.f);
						m_pFillCKey->setActive(true);
					}
				}
			}
		}
	}
}

void CSilvermane::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_isBlock && !m_IsAttack)
		static_cast<CState_Silvermane*>(m_pStateController->Get_CurState())->Block(_tAttackDesc);
	else
		static_cast<CState_Silvermane*>(m_pStateController->Get_CurState())->Hit(_tAttackDesc);
}

void CSilvermane::Parry(const PARRYDESC& _tParryDesc)
{
	switch (Get_WeaponType())
	{
	case CWeapon::EType::Sword_1H:
		m_pStateController->Change_State(L"1H_SwordRicochetReaction");
		break;
	case CWeapon::EType::Hammer_2H:
		m_pStateController->Change_State(L"2H_HammerRicochetReaction");
		break;
	}
}

const _bool CSilvermane::IsHit() const
{
	return m_isHit;
}

const _bool CSilvermane::IsDash() const
{
	return m_isDash;
}

const _bool CSilvermane::IsGround() const
{
	return m_pCharacterController->IsDown();
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

const _float CSilvermane::Get_FootStepTime() const
{
	return m_fFootStepTime;
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

void CSilvermane::Set_Radial(_bool check)
{
	m_pRenderer->SetRenderButton(CRenderer::RADIAL, check);
}

void CSilvermane::Set_RadialCnt(_int radialCnt)
{
	m_pRenderer->SetRadialCnt(radialCnt);
}

void CSilvermane::Set_IsHit(const _bool _isHit)
{
	m_isHit = _isHit;
}

void CSilvermane::Set_IsFall(const _bool _isFall)
{
	m_isFall = _isFall;
}

void CSilvermane::Set_IsDash(const _bool _isDash)
{
	m_isDash = _isDash;
}

void CSilvermane::Set_IsMove(const _bool _isMove)
{
	m_isMove = _isMove;
}

void CSilvermane::Set_IsTrasceCamera(const _bool _isTraceCamera)
{
	m_isTraceCamera = _isTraceCamera;
}

void CSilvermane::Set_IsDead(const _bool _isDead)
{
	m_bDead = _isDead;
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

void CSilvermane::Set_WeaponActive(const _bool _isActive)
{
	m_pCurWeapon->setActive(_isActive);
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

const _bool CSilvermane::IsShieldThrow() const
{
	return m_isShieldThrow;
}

const _bool CSilvermane::IsShieldReturn() const
{
	return m_isShieldReturn;
}

const _bool CSilvermane::IsCoolTime_ShieldThrow() const
{
	return m_isCoolTime_ShieldThrow;
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

void CSilvermane::Set_IsTrail(const _bool _isTrail)
{
	if (m_pCurWeapon)
		m_pCurWeapon->Set_IsTrail(_isTrail);
}

void CSilvermane::Add_PlusAngle(const _float _fDeltaAngle)
{
	m_fPlusAngle += _fDeltaAngle * 360.f;

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
	if (m_fCurrentHp > m_fMaxHp)
		m_fCurrentHp = m_fMaxHp;
}

void CSilvermane::Add_FootStepTime(const _float _fValue)
{
	m_fFootStepTime += _fValue;
}

void CSilvermane::Respawn()
{
	m_pCharacterController->setPosition(m_vRespawnPos);
	m_pTransform->SetUp_Rotation(_float3(0.f, 0.f, 0.f));
	m_pTransform->Set_State(CTransform::STATE_POSITION,XMVectorSetW(XMLoadFloat3(&m_vRespawnPos), 1.f));

	if (m_pCamera)
		m_pCamera->Respawn();

	m_fCurrentHp = m_fMaxHp;

	if (!m_isEquipWeapon)
	{
		Set_EquipWeapon(true);
		Set_WeaponFixedBone("weapon_r");
	}
	if (m_isEquipShield)
	{
		Set_EquipShield(false);
		Set_EquipShieldAnim(false);
	}

	if (FAILED(m_pStateController->Change_State(L"Idle")))
		return;

	m_bDead = false;

	g_pMainApp->Set_DeltaTimeZero(true);
}

void CSilvermane::Set_Position(const _float3 _vPosition)
{
	m_pCharacterController->setPosition(_vPosition);
}

void CSilvermane::Set_FootPosition(const _float3& _vPos)
{
	m_pCharacterController->setFootPosition(_vPos);
}

void CSilvermane::Set_FootStepTime(const _float _fValue)
{
	m_fFootStepTime = _fValue;
}


const _bool CSilvermane::Change_Weapon(const wstring& _name)
{
#pragma region Old Change Weapon
	/*
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
*/
#pragma endregion

	return true;
}

const _bool CSilvermane::Change_Weapon()
{
	if (!m_pEquipmentData)
		return false;

	if (1 == m_pPlayerData->EquipedSlot) /* 현재 1번 슬롯상태 */
	{
		if (m_pEquipmentData->IsExistEquip(EEquipSlot::Weapon1)) /* 장비창 첫번째 슬롯의 무기를 장착중인 경우 */
		{
			if (m_pEquipmentData->IsExistEquip(EEquipSlot::Weapon2)) /* 두번째 슬롯에 장착 중인 무기가 있는 경우*/
			{
				if (nullptr != m_pCurWeapon && /* 현재 장착중인 무기가 두번째 슬롯의 무기와 이름이 같은 경우  */
					0 == m_pCurWeapon->Get_Name().compare(m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.weaponName))
				{
					m_pPlayerData->EquipedSlot = 1;
					return false; /* 같은 무기, 변화 없음 */
				}
				else 
				{
					if (nullptr != m_pCurWeapon)
					{
						m_pCurWeapon->setActive(false);
						Set_EquipWeapon(false); /* 현재 착용중인 무기를 해제 */
						Set_WeaponFixedBone("spine_03");
					}
					m_pCurWeapon = m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.Get_Weapon(); /* 2번슬롯의 무기로 바꿔서 든다  */
					m_pCurWeapon->Set_Owner(this);
					m_pCurWeapon->setActive(true);
					m_pPlayerData->EquipedSlot = 2;
					if (Hammer == m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.weaponType)
					{
						STOP_SOUND(CHANNEL::PLAYER3);
						PLAY_SOUND(L"Swap_Hammer", CHANNEL::PLAYER3);
					}
					else if(LongSword == m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.weaponType)
					{
						STOP_SOUND(CHANNEL::PLAYER3);
						PLAY_SOUND(L"Swap_Longsword", CHANNEL::PLAYER3);
					}
					return true;
				}
			}
			else /* 두번째 슬롯에 장착중인 무기가 없는 경우 */
			{
				return false; /* 무기 변경 없음 */
			}
		}
	}

	else if (2 == m_pPlayerData->EquipedSlot) /* 현재 2번 슬롯상태 */
	{
		if (m_pEquipmentData->IsExistEquip(EEquipSlot::Weapon2)) /* 장비창 두번째 슬롯의 무기를 장착중인 경우 */
		{
			if (m_pEquipmentData->IsExistEquip(EEquipSlot::Weapon1)) /* 첫번째 슬롯에 장착 중인 무기가 있는 경우*/
			{
				if (nullptr != m_pCurWeapon && /* 현재 장착중인 무기가 첫번째 슬롯의 무기와 이름이 같은 경우  */
					0 == m_pCurWeapon->Get_Name().compare(m_pEquipmentData->GetEquipment(EEquipSlot::Weapon1).weaponData.weaponName))
				{
					m_pPlayerData->EquipedSlot = 2;
					return false; /* 같은 무기, 변화 없음 */
				}
				else
				{
					if (nullptr != m_pCurWeapon)
					{
						m_pCurWeapon->setActive(false);
						Set_EquipWeapon(false); /* 현재 착용중인 무기를 해제 */
						Set_WeaponFixedBone("spine_03");
					}
					m_pCurWeapon = m_pEquipmentData->GetEquipment(EEquipSlot::Weapon1).weaponData.Get_Weapon(); /* 1번슬롯의 무기로 바꿔서 든다  */
					m_pCurWeapon->Set_Owner(this);
					m_pCurWeapon->setActive(true);
					m_pPlayerData->EquipedSlot = 1;

					if (Hammer == m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.weaponType)
					{
						STOP_SOUND(CHANNEL::PLAYER3);
						PLAY_SOUND(L"Swap_Hammer", CHANNEL::PLAYER3);
					}
					else if (LongSword == m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.weaponType)
					{
						STOP_SOUND(CHANNEL::PLAYER3);
						PLAY_SOUND(L"Swap_Longsword", CHANNEL::PLAYER3);
					}

					return true;
				}
			}
			else /* 두번째 슬롯에 장착중인 무기가 없는 경우 */
			{
				return false; /* 무기 변경 없음 */
			}
		}
	}
	else
		return false;
	

	return false;
}

void CSilvermane::Change_WeaponOnInventory(void)
{
	if (!m_pEquipmentData)
		return;

	if (1 == m_pPlayerData->EquipedSlot)
	{
		if (m_pEquipmentData->IsExistEquip(EEquipSlot::Weapon1)) /* 첫번째 슬롯에 무기가 장착되어있는 경우 */
		{
			if (nullptr != m_pCurWeapon && /* 현재 장착중인 무기가 첫번째 슬롯의 무기와 이름이 같은 경우  */
				0 == m_pCurWeapon->Get_Name().compare(m_pEquipmentData->GetEquipment(EEquipSlot::Weapon1).weaponData.weaponName))
			{
				return; /* 장착 해제 X */
			}
			else /* 인벤에서 1번 슬롯 무기를 변경했다. */
			{
				if (nullptr != m_pCurWeapon)
				{
					Set_EquipWeapon(false);
					Set_WeaponFixedBone("spine_03");
					m_pCurWeapon = m_pEquipmentData->GetEquipment(EEquipSlot::Weapon1).weaponData.Get_Weapon();
					m_pCurWeapon->Set_Owner(this);
					m_pPlayerData->EquipedSlot = 1;
					/* 무기 교체 후 애님 상태 변경 */
					if (CWeapon::EType::Sword_1H == Get_WeaponType())
						m_pStateController->Change_State(L"1H_SwordEquipOn");
					else if (CWeapon::EType::Hammer_2H == Get_WeaponType())
						m_pStateController->Change_State(L"2H_HammerEquipOn");
				}
			}
		}
		else if (m_pEquipmentData->IsExistEquip(EEquipSlot::Weapon2)) /* 두번째 슬롯에 무기가 장착되어있는 경우 */
		{
			/* 인벤에서 1번 슬롯 무기를 장착 해제 했다. 2번 슬롯의 무기로 변경 */
			if (nullptr != m_pCurWeapon)
			{
				Set_EquipWeapon(false);
				Set_WeaponFixedBone("spine_03");
				m_pCurWeapon = m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.Get_Weapon();
				m_pCurWeapon->Set_Owner(this);
				m_pPlayerData->EquipedSlot = 2;
				/* 무기 교체 후 애님 상태 변경 */
				if (CWeapon::EType::Sword_1H == Get_WeaponType())
					m_pStateController->Change_State(L"1H_SwordEquipOn");
				else if (CWeapon::EType::Hammer_2H == Get_WeaponType())
					m_pStateController->Change_State(L"2H_HammerEquipOn");
			}
		}
		else /* 1번과 2번 슬롯 모두 무기가 없다. */
			return;
	}
	else if(2 == m_pPlayerData->EquipedSlot)
	{
		if (m_pEquipmentData->IsExistEquip(EEquipSlot::Weapon2)) /* 두번째 슬롯에 무기가 장착되어있는 경우 */
		{
			if (nullptr != m_pCurWeapon && /* 현재 장착중인 무기가 두번째 슬롯의 무기와 이름이 같은 경우  */
				0 == m_pCurWeapon->Get_Name().compare(m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.weaponName))
			{
				return; /* 장착 해제 X */
			}
			else /* 인벤에서 2번 슬롯 무기를 변경했다. */
			{
				if (nullptr != m_pCurWeapon)
				{
					Set_EquipWeapon(false);
					Set_WeaponFixedBone("spine_03");
					m_pCurWeapon = m_pEquipmentData->GetEquipment(EEquipSlot::Weapon2).weaponData.Get_Weapon();
					m_pCurWeapon->Set_Owner(this);
					m_pPlayerData->EquipedSlot = 2;
					/* 무기 교체 후 애님 상태 변경 */
					if (CWeapon::EType::Sword_1H == Get_WeaponType())
						m_pStateController->Change_State(L"1H_SwordEquipOn");
					else if (CWeapon::EType::Hammer_2H == Get_WeaponType())
						m_pStateController->Change_State(L"2H_HammerEquipOn");
				}
			}
		}
		else if (m_pEquipmentData->IsExistEquip(EEquipSlot::Weapon1)) /* 첫번째 슬롯에 무기가 장착되어있는 경우 */
		{
			/* 인벤에서 2번 슬롯 무기를 장착 해제 했다. 1번 슬롯의 무기로 변경 */
			if (nullptr != m_pCurWeapon)
			{
				Set_EquipWeapon(false);
				Set_WeaponFixedBone("spine_03");
				m_pCurWeapon = m_pEquipmentData->GetEquipment(EEquipSlot::Weapon1).weaponData.Get_Weapon();
				m_pCurWeapon->Set_Owner(this);
				m_pPlayerData->EquipedSlot = 1;
				/* 무기 교체 후 애님 상태 변경 */
				if (CWeapon::EType::Sword_1H == Get_WeaponType())
					m_pStateController->Change_State(L"1H_SwordEquipOn");
				else if (CWeapon::EType::Hammer_2H == Get_WeaponType())
					m_pStateController->Change_State(L"2H_HammerEquipOn");
			}
		}
		else /* 1번과 2번 슬롯 모두 무기가 없다. */
			return;
	}
}

HRESULT CSilvermane::Change_State(const wstring& _wstrStateTag)
{
	return m_pStateController->Change_State(_wstrStateTag);
}

void CSilvermane::RangeAttack(const _float _fForce)
{
	if (m_pCurWeapon)
		m_pCurWeapon->RangeAttack(_fForce);
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

void CSilvermane::Set_IsShieldThrow(const _bool _isShieldThrow)
{
	m_isShieldThrow = _isShieldThrow;
}

void CSilvermane::Set_IsShieldReturn(const _bool _isShieldReturn)
{
	m_isShieldReturn = _isShieldReturn;
}

void CSilvermane::Set_IsShieldCoolTime(const _bool _isCoolTime)
{
	m_isCoolTime_ShieldThrow = _isCoolTime;
}

void CSilvermane::Add_BlockTime(const _float _fValue)
{
	m_fBlockTime += _fValue;
}

HRESULT CSilvermane::ThrowShield(const _fvector& _svTargetPos)
{
	if (!m_pFlyingShield)
		return E_FAIL;

	m_pFlyingShield->Throw(_svTargetPos);
	m_isCoolTime_ShieldThrow = true;

	return S_OK;
}

void CSilvermane::Return_Shield()
{
	m_isShieldReturn = true;
	m_pAnimationController->Set_PlaySpeed(1.f);
}

void CSilvermane::End_ThrowShield()
{
	m_isShieldThrow = false;
	if (g_pObserver->Get_PlayerAttackAnimStart() || m_isBlock || m_isHit)
		return;

	STOP_SOUND(CHANNEL::PLAYER2);
	PLAY_SOUND(L"Shield_Catch", CHANNEL::PLAYER2);

	m_pStateController->Change_State(L"Shield_Throw");
	Set_EquipShield(true);
	Set_EquipShieldAnim(true);
	m_pShield->Set_TrackAcc(6.0); // 방패가 펼쳐진상태로 켜지도록 함

	m_pAnimationController->Set_TrackAcc(73.0);
	m_pAnimationController->Set_PlaySpeed(1.4f);
}

void CSilvermane::Loot_Shield()
{
	m_isLootShield = true;
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
		if (FAILED(m_pStateController->Change_State(L"Bow")))
			return -1;
	}
	if (g_pGameInstance->getkeyDown(DIK_END))
	{
		m_pCharacterController->setFootPosition(m_vRespawnPos);
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

RIM CSilvermane::ColorChange_RimCheck(RIM& rimdesc)
{
	if (g_pObserver->IsAttack())
	{
		m_color = _float4(0.784f, 0.137f, 0.137f, 0.f);
	}
	else
	{
		if (m_color.x <= 0.498f)
			m_color.x = 0.498f;
		else
			m_color.x -= 0.005f;

		if (m_color.y >= 0.9411f)
			m_color.y = 0.9411f;
		else
			m_color.y += 0.005f;

		if (m_color.z >= 0.8196f)
			m_color.z = 0.8196f;
		else
			m_color.z += 0.005f;
	}
	
	if (m_rimcheck == true)
	{
		CActor::RimIntensity(g_fDeltaTime * -1.f);
		rimdesc.rimcheck = m_rimcheck;
		rimdesc.rimcol = m_rimcol;
		rimdesc.rimintensity = m_rimintensity; // intensity 낮을 수록 과하게 빛남
		XMStoreFloat4(&rimdesc.camdir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_POSITION) - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane")));
		if(FAILED(m_pModel->SetUp_ValueOnShader("g_rimtimer", &m_rimtime, sizeof(_float)))) MSGBOX("Failed to Apply RimTime Value");
	}

	return rimdesc;
}

HRESULT CSilvermane::Create_MotionTrail(_int idex, _bool runcheck, _bool throwcheck, _float colorindx)
{
	if (idex <= 19)
	{
		wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();

		_matrix smatWorld;
		
		smatWorld = m_pTransform->Get_WorldMatrix();
		_vector position, camposition;
		position = m_pTransform->Get_State(CTransform::STATE_POSITION);
		camposition = g_pGameInstance->Get_CamPosition(wstrCamTag);

		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->setActive(true);
		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->Set_BoneMat(m_pModel->Get_CurBoneMatrix());
		_float uvdvid = 0.f;
		if (throwcheck == true)
			uvdvid = 0.7f;
		else
		{
			if (m_pCurWeapon->Get_Type() == CWeapon::EType::Sword_1H)
				uvdvid = 0.9f;
			else
				uvdvid = 0.7f;
			//if (idex <= 10)
			//	uvdvid = idex * 0.1f;
			//else
			//	uvdvid = (idex - 10) * 0.1f;
		}

		if (colorindx != 0.f)
		{
			// color idx = 1.f ~ 0.0f
			uvdvid = colorindx;
		}

		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->Set_Info(smatWorld
																	,m_pCurWeapon->Get_Transform()->Get_WorldMatrix()
																	, m_pFlyingShield->Get_Transform()->Get_WorldMatrix()
																	,uvdvid,m_pCurWeapon->Get_Model());

		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->Set_RunCheck(runcheck);
		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->Set_ThrowCheck(throwcheck);
	}

	return S_OK;
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

	_uint iObjectTag = -1;

	svRayPos += svRayDir * 6.f;
	// 레이캐스트
	RAYCASTDESC tRaycastDesc;
	XMStoreFloat3(&tRaycastDesc.vOrigin, svRayPos);
	XMStoreFloat3(&tRaycastDesc.vDir, svRayDir);
	tRaycastDesc.fMaxDistance = 50.f;
	tRaycastDesc.filterData.flags = PxQueryFlag::eANY_HIT | PxQueryFlag::eDYNAMIC;
	tRaycastDesc.layerMask = (1 << (_uint)ELayer::JumpTrigger);
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

	list<CGameObject*>* listBlankCkeys = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_BlankC");
	if (listBlankCkeys)
	{
		m_pBlankCKey = (CUI_Blank_CKey*)listBlankCkeys->front();
	}
	list<CGameObject*>* listFillKeys = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_FillC");
	if (listFillKeys)
	{
		m_pFillCKey = (CUI_Fill_Ckey*)listFillKeys->front();
	}

	if ((_uint)GAMEOBJECT::JUMP_NODE == iObjectTag)
	{
		m_pTargetJumpNode = static_cast<CJumpNode*>(pHitObject);
		m_pTargetJumpNode->setIsPick(true);
		if (g_pGameInstance->getkeyPress(DIK_C))
			m_fJumpNodeLookTime += (_float)_dDeltaTime;
		else
			m_fJumpNodeLookTime = 0.f;

		if (m_pFillCKey)
			m_pFillCKey->Set_JumpNode(true);

		if (1.f < m_fJumpNodeLookTime)
		{
			if (FAILED(m_pStateController->Change_State(L"Traverse_JumpNodeJog")))
				return false;
			m_fJumpNodeLookTime = 0.f;
		}
		else if (1.f >= m_fJumpNodeLookTime)
		{
			if (m_pBlankCKey && !m_pBlankCKey->getActive())
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

		if (m_pBlankCKey)
		{
			m_pBlankCKey->setActive(false);
		}

		if (m_pFillCKey && !m_pTargetJumpBox)
		{
			m_pFillCKey->Set_GapX(0.f);
			m_pFillCKey->setActive(false);
			m_pFillCKey->Set_JumpNode(false);
		}
	}

	return false;
}

CDropBox* CSilvermane::Get_TargetDropBox() const
{
	return m_pTargetDropBox;
}

void CSilvermane::Set_IsBoxOpen(const _bool _isBoxOpen)
{
	m_isBoxOpen = _isBoxOpen;
}

const void CSilvermane::Raycast_DropBox(const _double& _dDeltaTime)
{
	_matrix smatView;
	smatView = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	smatView = XMMatrixInverse(nullptr, smatView);

	if (XMMatrixIsNaN(smatView))
		return;

	_vector svRayPos, svRayDir;

	memcpy_s(&svRayPos, sizeof(_vector), &smatView.r[3], sizeof(_vector));
	memcpy_s(&svRayDir, sizeof(_vector), &smatView.r[2], sizeof(_vector));
	svRayDir = XMVector3Normalize(svRayDir);

	_uint iObjectTag = -1;

	svRayPos += svRayDir * 4.f;
	SWEEPDESC tSweepDesc;
	tSweepDesc.geometry = PxSphereGeometry(1.f);
	XMStoreFloat3(&tSweepDesc.vOrigin, svRayPos);
	XMStoreFloat3(&tSweepDesc.vDir, svRayDir);
	tSweepDesc.fMaxDistance = 2.f;
	tSweepDesc.filterData.flags = PxQueryFlag::eANY_HIT | PxQueryFlag::eDYNAMIC;
	tSweepDesc.layerMask = (1 << (_uint)ELayer::ItemBox) + (1 << (_uint)ELayer::Monster);

	CGameObject* pHitObject = nullptr;
	tSweepDesc.ppOutHitObject = &pHitObject;
	if (g_pGameInstance->Sweep(tSweepDesc))
	{
		if (pHitObject)
			iObjectTag = pHitObject->getTag();
	}

	if ((_uint)GAMEOBJECT::DROP_BOX == iObjectTag)
	{
		m_pTargetDropBox = static_cast<CDropBox*>(pHitObject);
		if (!m_isBoxOpen && !m_pTargetDropBox->IsOpen())
		{
			if (m_pFillCKey2)
			{
				m_pFillCKey2->setActive(true);
				CTransform* pTargetTransform = pHitObject->Get_Transform();
				_vector svTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
				_vector svTargetLook = XMVector3Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK));
				svTargetPos += _vector{ 0.f, 0.8f, 0.f, 0.f } + svTargetLook * 1.f;
				m_pFillCKey2->Set_Position(svTargetPos);
			}

			if (g_pGameInstance->getkeyDown(DIK_C))
			{
				m_pFillCKey2->Set_Press(true);
				if (FAILED(m_pStateController->Change_State(L"LootingChest", nullptr)))
					return;
				return;
			}
		}
	}
	else
	{
		m_pTargetDropBox = nullptr;
		if (m_pFillCKey2)
		{
			m_pFillCKey2->setActive(false);
			m_pFillCKey2->Set_Press(false);
			m_pFillCKey2->Set_GapX(0.f);
		}
	}
	//else
	//	static_cast<CDropBox*>(pHitObject)->FocusExit();

	switch (iObjectTag)
	{
	case (_uint)GAMEOBJECT::MONSTER_ABERRANT:
	case (_uint)GAMEOBJECT::MONSTER_1H:
	case (_uint)GAMEOBJECT::MONSTER_SHOOTER:
	case (_uint)GAMEOBJECT::MONSTER_ANIMUS:
	case (_uint)GAMEOBJECT::MONSTER_HEALER:
		if (static_cast<CActor*>(pHitObject)->Get_Groggy())
		{
			if (!m_isExecution)
			{
				if (m_pBlankFKey)
				{
					m_pBlankFKey->setActive(true);
					CTransform* pTargetTransform = pHitObject->Get_Transform();
					_vector svTargetLook = XMVector3Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK));
					_vector svTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
					svTargetPos += _vector{ 0.f, 1.2f, 0.f, 0.f } + svTargetLook * 1.f;
					m_pBlankFKey->Set_Position(svTargetPos);
				}

				if (g_pGameInstance->getkeyDown(DIK_F))
				{
					m_pTargetExecution = static_cast<CActor*>(pHitObject);
					m_pTargetExecution->Execution();
					Set_Execution(true);
					m_pBlankFKey->setActive(false);
				}
			}
		}
		break;
	case (_uint)GAMEOBJECT::MIDDLE_BOSS:
		if ((_uint)SCENEID::SCENE_STAGE2 == g_pGameInstance->getCurrentLevel())
		{
			if (static_cast<CActor*>(pHitObject)->Get_Groggy())
			{
				if (!m_isExecution)
				{
					if (m_pBlankFKey)
					{
						m_pBlankFKey->setActive(true);
						CTransform* pTargetTransform = pHitObject->Get_Transform();
						_vector svTargetLook = XMVector3Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK));
						_vector svTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
						svTargetPos += _vector{ 0.f, 1.2f, 0.f, 0.f } + svTargetLook * 1.f;
						m_pBlankFKey->Set_Position(svTargetPos);
					}

					if (g_pGameInstance->getkeyDown(DIK_F))
					{
						m_pTargetExecution = static_cast<CActor*>(pHitObject);
						m_pTargetExecution->Execution();
						Set_Execution(true);
						m_pBlankFKey->setActive(false);
					}
				}
			}
		}
		break;
	default:
		if (m_pBlankFKey)
			m_pBlankFKey->setActive(false);
		break;
	}

	tSweepDesc.geometry = PxSphereGeometry(1.f);
	XMStoreFloat3(&tSweepDesc.vOrigin, svRayPos);
	XMStoreFloat3(&tSweepDesc.vDir, svRayDir);
	tSweepDesc.fMaxDistance = 8.f;
	tSweepDesc.filterData.flags = PxQueryFlag::eANY_HIT | PxQueryFlag::eDYNAMIC;
	tSweepDesc.layerMask = (1 << (_uint)ELayer::Monster);

	CGameObject* pHitObject2 = nullptr;
	tSweepDesc.ppOutHitObject = &pHitObject2;
	if (g_pGameInstance->Sweep(tSweepDesc))
	{
		if (g_pGameInstance->getkeyPress(DIK_E))
		{
			if (!m_isSkill && 50.f < m_fSkillGuage)
			{
				if (g_pGameInstance->getMousePress(CInputDev::MOUSESTATE::MB_LBUTTON))
				{
					switch (m_pCurWeapon->Get_Type())
					{
					case CWeapon::EType::Sword_1H:
						if (FAILED(m_pStateController->Change_State(L"1H_Multishlash")))
						{
							MSGBOX(L"멀티슬래시 공격이 실패했어!!!");
						}
						else
						{
							m_pTargetExecution = static_cast<CActor*>(pHitObject2);
						}
						break;
					}
				}
				else if (g_pGameInstance->getMousePress(CInputDev::MOUSESTATE::MB_RBUTTON))
				{
					switch (m_pCurWeapon->Get_Type())
					{
					case CWeapon::EType::Sword_1H:
						if (FAILED(m_pStateController->Change_State(L"1H_SwordSkill_3")))
						{
							MSGBOX(L"소드스킬_3 공격이 실패했어!!!");
						}
						else
						{
							m_pTargetExecution = static_cast<CActor*>(pHitObject2);
						}
						break;
					}
				}
			}
		}
	}
}

void CSilvermane::Set_Execution(const _bool _isExecution, CActor* _pTarget)
{
	if (m_pCamera)
	{
		CHierarchyNode* pEyeBone = nullptr;
		CHierarchyNode* pAtBone = nullptr;
		switch (_isExecution)
		{
		case true:
			pEyeBone = m_pModel->Get_BoneMatrix("camera_location_1");
			pAtBone = m_pModel->Get_BoneMatrix("camera_look_at_1");
			m_pStateController->Change_State(L"Execution");
			break;
		case false:
			m_pTargetExecution = nullptr;
		}
		m_isExecution = _isExecution;
		m_pCamera->Set_Execution(_isExecution, pEyeBone, pAtBone);
	}
}

CActor* CSilvermane::Get_TargetExecution() const
{
	return m_pTargetExecution;
}

void CSilvermane::Set_TargetExecution(CActor* pTarget)
{
	m_pTargetExecution = pTarget;
}

CHierarchyNode* CSilvermane::Get_ExecutionTargetBone() const
{
	return m_pExecutionTargetBone;
}

CUI_Blank_FKey* CSilvermane::Get_Blank_FKey() const
{
	return m_pBlankFKey;
}

void CSilvermane::Set_IsSkill(const _bool _isSkill)
{
	m_isSkill = _isSkill;
}

void CSilvermane::Set_Skill(const _bool _isSkil)
{
	if (m_pCamera)
	{
		CHierarchyNode* pEyeBone = nullptr;
		CHierarchyNode* pAtBone = nullptr;
		switch (_isSkil)
		{
		case true:
			pEyeBone = m_pModel->Get_BoneMatrix("camera_location_2");
			pAtBone = m_pModel->Get_BoneMatrix("camera_look_at_2");
			break;
		}
		m_pCamera->Set_Skill(_isSkil, pEyeBone, pAtBone);
	}
}

void CSilvermane::Add_SkillGuage(const _float _fValue)
{
	if (m_isSkill)
		return;

	m_fSkillGuage += _fValue;
}

const _bool CSilvermane::IsSkill() const
{
	return m_isSkill;
}

const _float CSilvermane::Get_SkillGuage() const
{
	return m_fSkillGuage;
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
	__super::Free();

	Safe_Release(m_pHealSphere);
	Safe_Release(m_pShield);
	Safe_Release(m_pCharacterController);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimationController);
}
