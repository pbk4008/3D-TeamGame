#include "pch.h"
#include "Boss_Bastion_Judicator.h"
#include "Animation.h"
#include "ShieldBreaker.h"
#include "UI_Monster_Panel.h"
#include "Loading.h"

#include "MidBoss_Idle.h"
#include "MidBoss_Run.h"
#include "MidBoss_Rage.h"
#include "MidBoss_BattleCry.h"
#include "MidBoss_Attack.h"
#include "MidBoss_Turn.h"
#include "MidBoss_Death.h"
#include "MidBoss_Execution.h"
#include "MidBoss_Stun.h"
#include "DamageFont.h"

#include "MeshEffect_Boss_Explosion.h"


CBoss_Bastion_Judicator::CBoss_Bastion_Judicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CActor(pDevice, pDeviceContext)
{
}

CBoss_Bastion_Judicator::CBoss_Bastion_Judicator(const CBoss_Bastion_Judicator& rhs)
	:CActor(rhs)
	, m_pCharacterController(rhs.m_pCharacterController)
	, m_pStateController(rhs.m_pStateController)
	, m_pAnimator(rhs.m_pAnimator)
	, m_pPanel(rhs.m_pPanel)
{
	Safe_AddRef(m_pCharacterController);
	Safe_AddRef(m_pStateController);
	Safe_AddRef(m_pAnimator);
	Safe_AddRef(m_pPanel);
}
HRESULT CBoss_Bastion_Judicator::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		_float3 vPoint = (*(_float3*)pArg);

		if (FAILED(Set_SpawnPosition(vPoint)))
			return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(Set_Animation_FSM()))
		return E_FAIL;

	if (FAILED(Set_State_FSM()))
		return E_FAIL;
	
	if (FAILED(Set_Weapon()))
		return E_FAIL;

	if (FAILED(Set_PanelBar()))
		return E_FAIL;

	//boss Explosion
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_MeshEffect_Boss_Explosion", L"Proto_GameObject_MeshEffect_Boss_Explosion",
		nullptr, (CGameObject**)&m_pEff_Explosion)))
	{
		MSGBOX("Failed to Creating MeshEffect_Boss_Explosion in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	m_pEff_Explosion->setActive(false);

	//TODO : ???? ?????? ?? ??????, ?????? UI???? ?????? ??????????
	m_fMaxHp = 7000.f;
	m_fCurrentHp = m_fMaxHp;

	m_fMaxGroggyGauge = 50.f;
	m_fGroggyGauge = 0.f;

	m_pPanel->Set_HpBar(Get_HpRatio());
	m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());

	m_tAttackDesc.iLevel = 2;

	m_pWeapon->setActive(false);
	setActive(false);
	
	m_pPanel->setActive(false);
	m_bUIShow = false;


	return S_OK;
}

_int CBoss_Bastion_Judicator::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	if (this->Get_HpRatio()<0.3f && g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE1)
	{
		m_bDead = true;
		m_pWeapon->setActive(false);
		m_bActive = false;
		m_bUIShow = false;
		m_pPanel->Set_Show(false);
		m_pPanel->setActive(false);
		setActive(false);
		
		return 0;
	}

	m_pTransform->Set_Velocity(XMVectorZero());

	_int iProgress = m_pStateController->Tick(TimeDelta);
	if (NO_EVENT != iProgress)
	{
		return iProgress;
	}

	if (nullptr != m_pWeapon)
	{
		if (-1 == m_pWeapon->Tick(TimeDelta))
		{
			return -1;
		}
	}

	if (m_bIsFall)
		m_pTransform->Fall(TimeDelta);

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (20.f >= fDistToPlayer)
	{
		m_bUIShow = true;
	}


	if (true == m_bUIShow)
	{
		m_pPanel->setActive(true);
		m_pPanel->Set_Show(true);

		m_fUIShowTimeAcc += (_float)TimeDelta;
	}

	if (1.f <= m_fUIShowTimeAcc && m_bUIShow)
	{
		m_pPanel->Set_Show(false);
		m_bUIShow = false;
		m_fUIShowTimeAcc = 0.f;
	}

	if (m_fGroggyGauge >= m_fMaxGroggyGauge)
	{
		//???????????? ????state???? ???? ?????? ???? 0???? ??????????
		m_bGroggy = true;
		m_pStateController->Change_State(L"Stun");
		m_fGroggyGauge = 0.f;
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}
	
	if (true == m_bGroggy || true == m_bDead )
	{
		Set_IsAttack(false);
		m_fGroggyGauge = 0.f;
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}

	if (STUN_END == m_pAnimator->Get_CurrentAnimNode())
	{
		if (m_pAnimator->Get_AnimController()->Is_Finished())
		{
			m_bGroggy = false;
		}
	}

	if (m_fCurrentHp <= 0.f && m_bDead == false && g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE2)
	{
		m_bDead = true;
		m_pStateController->Change_State(L"Death");
	}

	if (DEATH == m_pAnimator->Get_CurrentAnimNode())
	{
		if (L"Death" == m_pStateController->Get_CurStateTag())
		{
			if (m_pAnimator->Get_CurrentAnimation()->Is_Finished() && m_lifetime <= 0.f)
			{
				if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE2)
					m_pRenderer->SetRenderButton(CRenderer::FADEIN, true);

				m_bDead = true;
				//Set_Remove(true);
				m_pPanel->Set_Show(false);
				m_pPanel->Set_UIRemove(true);
				m_bdissolve = true;

				return 0;
			}

			if (1 <= m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex() && 2 > m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex())
				Active_Effect((_uint)EFFECT::DEATH);

			if (m_lifetime >= 1.f)
			{
				m_bchanglevel = true;
				g_pQuestManager->SetRender(false);
				g_pInvenUIManager->SetRender(false);
			}
		}
		else
		{
			//Set_Remove(true);
			m_pPanel->Set_UIRemove(true);
			Active_Effect((_uint)EFFECT::DEATH);
		}
		
	}
	else if (EXECUTION == m_pAnimator->Get_CurrentAnimNode())
	{
		if (L"Execution" == m_pStateController->Get_CurStateTag())
		{
			m_pPanel->Set_Show(false);
			m_pPanel->Set_UIRemove(true);

			if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
			{
				m_bDead = true;
				m_bdissolve = true;
				
				if (m_lifetime >= 1.f)
				{
					m_bchanglevel = true;
					g_pQuestManager->SetRender(false);
					g_pInvenUIManager->SetRender(false);
				}

				return 0;
			}
		}
	}

	if (0.5f >= Get_HpRatio())
	{
		m_pWeapon->setActive(false);
	}

	m_pCharacterController->Move(TimeDelta, m_pTransform->Get_Velocity());

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	return 0;
}

_int CBoss_Bastion_Judicator::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	if (!m_bDead)
		m_pCharacterController->Update_OwnerTransform();

	m_pStateController->LateTick(TimeDelta);

	if (!g_pGameInstance->isIn_WorldFrustum(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		return 0;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	if (nullptr != m_pWeapon)
	{
		if (-1 == m_pWeapon->LateTick(TimeDelta))
		{
			return E_FAIL;
		}
	}

	return 0;
}

HRESULT CBoss_Bastion_Judicator::Render()
{
	if (m_bdissolve == true)
		CActor::DissolveOn(0.5f);

	RIM rimdesc;
	ZeroMemory(&rimdesc, sizeof(RIM));

	if (m_rimcheck == true)
	{
		CActor::RimIntensity(g_fDeltaTime * -1.f);

		rimdesc.rimcheck = m_rimcheck;
		rimdesc.rimcol = m_rimcol;
		rimdesc.rimintensity = 3.f; // intensity ???? ???? ?????? ????
		XMStoreFloat4(&rimdesc.camdir, XMVector3Normalize(g_pGameInstance->Get_CamPosition(L"Camera_Silvermane") - m_pTransform->Get_State(CTransform::STATE_POSITION)));
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimtimer", &m_rimtime, sizeof(_float)))) MSGBOX("Failed to Apply RimTime Value");
	}


	if (FAILED(m_pModel->SetUp_ValueOnShader("g_bdissolve", &m_bdissolve, sizeof(_bool)))) MSGBOX("Failed to Apply dissolvetime");

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		SCB desc;
		ZeroMemory(&desc, sizeof(SCB));
		switch (i)
		{
		case 0 : case 3:  // body
			desc.metalic = 0.5f;
			desc.roughness = 0.f;
			desc.ao = 0.5f;
			desc.color = _float4(0.811f, 1.f, 0.898f, 1.f);
			desc.empower = 0.7f;
			CActor::BindConstantBuffer(wstrCamTag,&desc, &rimdesc);
			m_pModel->Render(i, 0);
			break;
		case 1 : // fur
			desc.color = _float4(1.f, 0.466f, 0.901f, 1.f);
			desc.empower = 0.01f;
			CActor::BindConstantBuffer(wstrCamTag,&desc);
			m_pModel->Render(i, 1);
			break;
		case 2 :  // cloak
			CActor::BindConstantBuffer(wstrCamTag,&desc);
			m_pModel->Render(i, 2);
			break;
		}
	}

	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CActor::BindConstantBuffer(wstrCamTag);
	CActor::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 3);

	return S_OK;
}

void CBoss_Bastion_Judicator::setActive(_bool bActive)
{
	CGameObject::setActive(bActive);

	if (bActive)
	{
		if (!m_pCharacterController)
		{
			/* for.Character Controller */
			CCharacterController::DESC tCharacterControllerDesc;
			tCharacterControllerDesc.fHeight = 1.f;
			tCharacterControllerDesc.fRadius = 1.f;
			tCharacterControllerDesc.fContactOffset = tCharacterControllerDesc.fRadius * 0.1f;
			tCharacterControllerDesc.fStaticFriction = 0.5f;
			tCharacterControllerDesc.fDynamicFriction = 0.5f;
			tCharacterControllerDesc.fRestitution = 0.f;
			tCharacterControllerDesc.pGameObject = this;
			tCharacterControllerDesc.vPosition = { 0.f, 0.f, 0.f };

			if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCharacterControllerDesc)))
				MSGBOX(L"???????? cct ???? ????");
			m_pCharacterController->setOwnerTransform(m_pTransform);
			m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);
		}
		if (m_pWeapon)
			m_pWeapon->setActive(true);
		if (m_pPanel)
			m_pPanel->setActive(true);
	}
}

HRESULT CBoss_Bastion_Judicator::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Boss_Bastion_Tier4", L"Model", (CComponent**)&m_pModel)))
	{
		return E_FAIL;
	}
	_matrix matPivot = XMMatrixIdentity();
	matPivot = XMMatrixScaling(0.014f, 0.014f, 0.014f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModel;
	tDesc.pTransform = m_pTransform;
	tDesc.eType = CAnimationController::EType::CharacterController;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)& m_pAnimator, &tDesc)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"Com_StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);


	///* for.Character Controller */
	//CCharacterController::DESC tCharacterControllerDesc;
	//tCharacterControllerDesc.fHeight = 1.f;
	//tCharacterControllerDesc.fRadius = 0.5f;
	//tCharacterControllerDesc.fContactOffset = tCharacterControllerDesc.fRadius * 0.1f;
	//tCharacterControllerDesc.fStaticFriction = 0.5f;
	//tCharacterControllerDesc.fDynamicFriction = 0.5f;
	//tCharacterControllerDesc.fRestitution = 0.f;
	//tCharacterControllerDesc.pGameObject = this;
	//tCharacterControllerDesc.vPosition = { 0.f, 0.f, 0.f };

	//if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCharacterControllerDesc)))
	//	return E_FAIL;
	//m_pCharacterController->setOwnerTransform(m_pTransform);
	//m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);

	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::Set_Animation_FSM()
{
	//????????
	//??????????, ????????????, ??????????, ????????, ????????(???????????????? ??????), ????, ????
	CAnimation* pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Menu_Idle_Start");
	if (FAILED(m_pAnimator->Insert_Animation(IDLE_START_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Menu_Idle_Loop");
	if (FAILED(m_pAnimator->Insert_Animation(IDLE_LOOP_H, IDLE_START_H, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	//Rage
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_Rage_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(RAGE, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	//Run
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Loco_Jog_Fwd_Start_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(JOG_FWD_START_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Loco_Jog_Fwd_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(JOG_FWD_H, JOG_FWD_START_H, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation(RAGE, JOG_FWD_H, false)))
		return E_FAIL;

#pragma region ??????????????
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_BattleCry_Start_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(BATTLECRY_START, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_BattleCry_Loop_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(BATTLECRY_LOOP, BATTLECRY_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_BattleCry_End_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(BATTLECRY_END, BATTLECRY_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	//??????????????(????????????, ??????????, ??????????????????????)
	if (FAILED(m_pAnimator->Connect_Animation(RAGE, BATTLECRY_END, false)))
		return E_FAIL;
#pragma endregion

#pragma region ????????
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_Stun_Start_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(STUN_START, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_Stun_Loop_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(STUN_LOOP, STUN_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_Stun_End_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(STUN_END, STUN_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation(RAGE, STUN_END, false)))
		return E_FAIL;
#pragma endregion

#pragma region ????
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_135_Left_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_135LEFT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_135_Right_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_135RIGHT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_180_Left_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_180LEFT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_180_Right_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_180RIGHT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_45_Left_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_45LEFT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_45_Right_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_45RIGHT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_90_Left_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_90LEFT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_90_Right_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_90RIGHT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_Attack_Finisher_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(EXECUTION, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

#pragma region ????????????
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Attack_JogR1");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_JOG_H, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Attack_L1_R2_Legacy");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_LEGACY_H, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Attack_R1_01");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_R1_H, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Sprint_Attack");
	if (FAILED(m_pAnimator->Insert_Animation(SPRINT_ATTACK_H, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_Attack_R1_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_R1, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_Attack_S1_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_S1, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_Attack_S2_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_S2, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion 

#pragma region ????
	pAnim = m_pModel->Get_Animation("SK_Bastion_Tier4.ao|A_Death_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(DEATH, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion 

	//???????? ???? ????(????????, ????????????????)
	//m_pAnimator->Set_UpAutoChangeAnimation(RAGE, BATTLECRY_START);
	m_pAnimator->Set_UpAutoChangeAnimation(IDLE_START_H, IDLE_LOOP_H);

	m_pAnimator->Set_UpAutoChangeAnimation(BATTLECRY_START, BATTLECRY_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation(BATTLECRY_LOOP, BATTLECRY_END);
	m_pAnimator->Set_UpAutoChangeAnimation(BATTLECRY_END, RAGE);

	m_pAnimator->Set_UpAutoChangeAnimation(STUN_START, STUN_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation(STUN_LOOP, STUN_END);
	m_pAnimator->Set_UpAutoChangeAnimation(STUN_END, RAGE);

	m_pAnimator->Set_UpAutoChangeAnimation(JOG_FWD_START_H, JOG_FWD_H);

	m_pAnimator->Insert_AnyEntryAnimation(IDLE_START_H);
	m_pAnimator->Insert_AnyEntryAnimation(RAGE);
	m_pAnimator->Insert_AnyEntryAnimation(JOG_FWD_START_H);

	m_pAnimator->Insert_AnyEntryAnimation(BATTLECRY_START);
	
	m_pAnimator->Insert_AnyEntryAnimation(STUN_START);

	m_pAnimator->Insert_AnyEntryAnimation(EXECUTION);

	//????????
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_JOG_H);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_LEGACY_H);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_R1_H);
	m_pAnimator->Insert_AnyEntryAnimation(SPRINT_ATTACK_H);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_R1);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_S1);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_S2);

	//????
	m_pAnimator->Insert_AnyEntryAnimation(DEATH);

	//????
	m_pAnimator->Insert_AnyEntryAnimation(TURN_135LEFT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_135RIGHT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_180LEFT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_180RIGHT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_90LEFT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_90RIGHT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_45LEFT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_45RIGHT_H);


	m_pAnimator->Change_Animation(IDLE_START_H);

	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::Set_State_FSM()
{
	if (FAILED(m_pStateController->Add_State(L"Idle", CMidBoss_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Run", CMidBoss_Run::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Rage", CMidBoss_Rage::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"BattleCry", CMidBoss_BattleCry::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Attack", CMidBoss_Attack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Turn", CMidBoss_Turn::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Death", CMidBoss_Death::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Execution", CMidBoss_Execution::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Stun", CMidBoss_Stun::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CMonster_FSM*>(pair.second)->Set_Monster(this);
		static_cast<CMonster_FSM*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CMonster_FSM*>(pair.second)->Set_Model(m_pModel);
		static_cast<CMonster_FSM*>(pair.second)->Set_Animator(m_pAnimator);
	}

	m_pStateController->Change_State(L"Idle");

	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::Set_Weapon()
{
	CHierarchyNode* pBone = m_pModel->Get_BoneMatrix("weapon_r_end");
	CShieldBreaker* pWeapon = static_cast<CShieldBreaker*>(g_pGameInstance->Clone_GameObject(m_iSceneID, L"Proto_GameObject_Weapon_ShieldBreaker"));

	if (nullptr == pWeapon)
		return E_FAIL;

	pWeapon->Set_FixedBone(pBone);
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	m_pWeapon = pWeapon;

	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::Set_PanelBar()
{
	//MidBossBar Panel
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::MIDBOSS;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;

	Safe_AddRef(m_pPanel);

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	m_bIsFall = true;
	m_iObectTag = (_uint)GAMEOBJECT::MIDDLE_BOSS;

	return S_OK;
}

void CBoss_Bastion_Judicator::OnTriggerEnter(CCollision& collision)
{
}

void CBoss_Bastion_Judicator::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_bDead || 0.f >= m_fCurrentHp)
		return;

	m_bFirstHit = true; //?? ???? true?? ????????

	if (true == m_bFirstHit)
	{
		m_pPanel->Set_BackUIGapY(1.f);
	}

	m_fCurrentHp -= _tAttackDesc.fDamage;
	m_fGroggyGauge += 6; //TODO::????????????????????

	m_pPanel->Set_HpBar(Get_HpRatio());

	Active_Effect((_uint)EFFECT::HIT);
	Active_Effect((_uint)EFFECT::HIT_FLOATING);
	Active_Effect((_uint)EFFECT::HIT_FLOATING_2);
	Active_Effect((_uint)EFFECT::HIT_IMAGE);

	if (false == m_bGroggy)
	{
		//?????? ???????? ????????????
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}

	Active_Effect((_uint)EFFECT::HIT);
	Active_Effect((_uint)EFFECT::HIT_FLOATING);
	Active_Effect((_uint)EFFECT::HIT_FLOATING_2);
	Active_Effect((_uint)EFFECT::HIT_IMAGE);

	CTransform* pOtherTransform = _tAttackDesc.pOwner->Get_Transform();
	_vector svOtherLook = XMVector3Normalize(pOtherTransform->Get_State(CTransform::STATE_LOOK));
	_vector svOtherRight = XMVector3Normalize(pOtherTransform->Get_State(CTransform::STATE_RIGHT));

	uniform_real_distribution<_float> fRange(-0.5f, 0.5f);
	uniform_real_distribution<_float> fRange2(-0.2f, 0.2f);
	uniform_int_distribution<_int> iRange(-5, 5);
	CDamageFont::DESC tDamageDesc;
	_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 2.f + fRange2(g_random), 0.f, 0.f };
	svPos += svOtherRight * fRange(g_random) - svOtherLook * 0.5f;
	XMStoreFloat3(&tDamageDesc.vPos, svPos);
	tDamageDesc.fDamage = _tAttackDesc.fDamage + (_float)iRange(g_random);
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_DamageFont", L"Proto_GameObject_DamageFont", &tDamageDesc)))
		MSGBOX(L"?????? ???? ???? ????");
}

void CBoss_Bastion_Judicator::Execution()
{
	//m_bDead = true;
	m_bGroggy = false;
	m_IsAttack = false;
	m_pWeapon->Set_IsAttack(false);

	m_pStateController->Change_State(L"Execution");
	m_pCharacterController->Remove_CCT();
}

void CBoss_Bastion_Judicator::Set_IsAttack(const _bool _isAttack)
{
	m_IsAttack = _isAttack;
	if (m_pWeapon)
	{
		m_pWeapon->Set_IsAttack(_isAttack);
	}
}

void CBoss_Bastion_Judicator::OnWeaponEffect()
{
	m_pWeapon->Set_HitGround(true);
}

void CBoss_Bastion_Judicator::OnEff_MeshExplosion(_bool Active)
{
	m_pEff_Explosion->setActive(Active);
}

CBoss_Bastion_Judicator* CBoss_Bastion_Judicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CBoss_Bastion_Judicator* pInstance = new CBoss_Bastion_Judicator(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CBoss_Bastion_Judicator");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBoss_Bastion_Judicator::Clone(const _uint _iSceneID, void* pArg)
{
	CBoss_Bastion_Judicator* pInstance = new CBoss_Bastion_Judicator(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CBoss_Bastion_Judicator");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Bastion_Judicator::Free()
{
	__super::Free();

	Safe_Release(m_pPanel);
	Safe_Release(m_pCharacterController);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimator);
}
