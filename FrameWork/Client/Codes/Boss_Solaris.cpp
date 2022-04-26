#include "pch.h"
#include "Boss_Solaris.h"
#include "Animation.h"

#include "Boss_Weapon.h"

#include "Boss_Idle.h"
#include "Boss_Walk_Front.h"
#include "Boss_Dash_Back.h"
#include "Boss_BackFlip.h"
#include "Boss_Turn.h"
#include "Boss_Stun.h"
#include "Boss_Flinch_Left.h"
#include "Boss_Flinch_Right.h"
#include "Boss_Stagger_Left.h"
#include "Boss_Stagger_Right.h"
#include "Boss_Taunt_Fist.h"

#include "Boss_Attack_Agg.h"
#include "Boss_Attack_R1.h"
#include "Boss_Attack_R2.h"
#include "Boss_Attack_S2_Variant.h"
#include "Boss_Attack_S3.h"
#include "Boss_Attack_S5_Protocol.h"
#include "Boss_Attack_S6.h"

CBoss_Solaris::CBoss_Solaris(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CActor(pDevice, pDeviceContext)
{
}

CBoss_Solaris::CBoss_Solaris(const CBoss_Solaris& rhs)
	:CActor(rhs)
	, m_pCharacterController(rhs.m_pCharacterController)
	, m_pStateController(rhs.m_pStateController)
	, m_pAnimator(rhs.m_pAnimator)
{
	Safe_AddRef(m_pCharacterController);
	Safe_AddRef(m_pStateController);
	Safe_AddRef(m_pAnimator);
}
HRESULT CBoss_Solaris::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CBoss_Solaris::NativeConstruct(const _uint _iSceneID, void* pArg)
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
	
	m_iObectTag = (_uint)GAMEOBJECT::BOSS;
	m_fMaxHp = 500.f;
	m_fCurrentHp = m_fMaxHp;

	m_fMaxGroggyGauge = 10.f;
	m_fGroggyGauge = m_fMaxGroggyGauge;

	m_tAttackDesc.iLevel = 3;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 1.f, 20.f, 1.f));

	m_pWeapon->setActive(true);
	setActive(true);

	//원래는 active false였다가 시네마틱끝나고 true로 바꿔줘야함 
	return S_OK;
}

_int CBoss_Solaris::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	if (true == m_rimcheck)
	{
		CActor::SetRimIntensity((_float)TimeDelta * -10.f);
	}

	//cout << m_fCurrentHp << endl;

	if (g_pGameInstance->getkeyDown(DIK_NUMPAD2))
	{
		m_pStateController->Change_State(L"Attack_R1");
	}
	if (g_pGameInstance->getkeyDown(DIK_NUMPAD3))
	{
		m_pStateController->Change_State(L"Attack_R2");
	}
	if (g_pGameInstance->getkeyDown(DIK_NUMPAD4))
	{
		m_pStateController->Change_State(L"Attack_S2_Variant");
	}
	if (g_pGameInstance->getkeyDown(DIK_NUMPAD5))
	{
		m_pStateController->Change_State(L"Attack_Agg");
	}
	if (g_pGameInstance->getkeyDown(DIK_NUMPAD6))
	{
		m_pStateController->Change_State(L"Attack_S3");
	}

	if (0 >= m_fCurrentHp)
	{
		//죽는게아니라 시네마틱재생 or 처형 
		m_pWeapon->Set_IsAttack(false);
	}

	m_pTransform->Set_Velocity(XMVectorZero());

	_int iProgress = m_pStateController->Tick(TimeDelta);
	if (NO_EVENT != iProgress)
	{
		return iProgress;
	}

	if (m_bIsFall)
		m_pTransform->Fall(TimeDelta);

	if (nullptr != m_pWeapon)
	{
		if (-1 == m_pWeapon->Tick(TimeDelta))
			return -1;
	}

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (0 == m_fGroggyGauge)
	{
		//스턴상태일때 스턴state에서 현재 그로기 계속 0으로 고정시켜줌
		m_bGroggy = true;
		m_pStateController->Change_State(L"Stun");
	}
	
	if (true == m_bGroggy || true == m_bDead )
	{
		m_fGroggyGauge = m_fMaxGroggyGauge;
	}

	if (STUN_END == m_pAnimator->Get_CurrentAnimNode())
	{
		if (m_pAnimator->Get_AnimController()->Is_Finished())
		{
			m_bGroggy = false;
		}
	}

	m_pCharacterController->Move(TimeDelta, m_pTransform->Get_Velocity());

	//cout << fDistToPlayer << endl;
	return 0;
}

_int CBoss_Solaris::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	_int iProgress = m_pStateController->LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
	{
		return iProgress;
	}

	m_pCharacterController->Update_OwnerTransform();

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	if (nullptr != m_pWeapon)
	{
		if (-1 == m_pWeapon->LateTick(TimeDelta))
			return E_FAIL;
	}

	return 0;
}

HRESULT CBoss_Solaris::Render()
{
	if (m_bdissolve == true)
		CActor::DissolveOn(0.5f);

	RIM rimdesc;
	ZeroMemory(&rimdesc, sizeof(RIM));

	if (m_rimcheck == true)
	{
		rimdesc.rimcheck = m_rimcheck;
		rimdesc.rimcol = m_rimcol;
		rimdesc.rimintensity = 50.f; // intensity 낮을 수록 과하게 빛남
		XMStoreFloat4(&rimdesc.camdir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_POSITION) - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane")));
		CActor::SetRimIntensity(g_fDeltaTime * -10.f);
	}


	if (FAILED(m_pModel->SetUp_ValueOnShader("g_bdissolve", &m_bdissolve, sizeof(_bool)))) MSGBOX("Failed to Apply dissolvetime");

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		SCB desc;
		ZeroMemory(&desc, sizeof(SCB));
		switch (i)
		{
		case 0 : case 1: case 2:  // body
			desc.metalic = 0.3f;
			desc.roughness = -0.1f;
			desc.color = _float4(0.811f, 1.f, 0.898f, 1.f);
			desc.empower = 1.0f;
			CActor::BindConstantBuffer(wstrCamTag,&desc, &rimdesc);
			m_pModel->Render(i, 0);
			break;
		//case 1 : // fur
		//	desc.color = _float4(1.f, 0.466f, 0.901f, 1.f);
		//	desc.empower = 0.01f;
		//	CActor::BindConstantBuffer(wstrCamTag,&desc);
		//	m_pModel->Render(i, 1);
		//	break;
		case 3 :  // cloak
			CActor::BindConstantBuffer(wstrCamTag,&desc);
			m_pModel->Render(i, 2);
			break;
		}
	}

	return S_OK;
}

HRESULT CBoss_Solaris::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CActor::BindConstantBuffer(wstrCamTag);
	CActor::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 3);

	return S_OK;
}

void CBoss_Solaris::setActive(_bool bActive)
{
	CGameObject::setActive(bActive);

	if (bActive)
	{
		if (!m_pCharacterController)
		{
			/* for.Character Controller */
			CCharacterController::DESC tCharacterControllerDesc;
			tCharacterControllerDesc.fHeight = 2.f;
			tCharacterControllerDesc.fRadius = 1.5f;
			tCharacterControllerDesc.fContactOffset = tCharacterControllerDesc.fRadius * 0.1f;
			tCharacterControllerDesc.fStaticFriction = 0.5f;
			tCharacterControllerDesc.fDynamicFriction = 0.5f;
			tCharacterControllerDesc.fRestitution = 0.f;
			tCharacterControllerDesc.pGameObject = this;
			tCharacterControllerDesc.vPosition = { 0.f, 0.f, 0.f };

			if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCharacterControllerDesc)))
				MSGBOX(L"fail to creating boss cct");
			m_pCharacterController->setOwnerTransform(m_pTransform);
			m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);

			if (m_pWeapon)
				m_pWeapon->setActive(true);
		}
	}
}

HRESULT CBoss_Solaris::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components(m_iSceneID, L"Model_Boss_Solaris", L"Model", (CComponent**)&m_pModel)))
	{
		return E_FAIL;
	}

	_matrix matPivot = XMMatrixIdentity();

	matPivot = XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(180.f));
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

	return S_OK;
}

HRESULT CBoss_Solaris::Set_Animation_FSM()
{
	//시작루프
	//연결할애님, 연결당할애님, 애니메이션, 루트애님, 트랜스폼(루트애니메이션때 따라감), 루프, 옵션
	CAnimation* pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Idle_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(IDLE, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

#pragma region WALK_FRONT
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Walk_Fwd_Start_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(WALK_FWD_START, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Walk_Fwd_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(WALK_FWD, WALK_FWD_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Walk_Fwd_Stop_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(WALK_FWD_STOP, WALK_FWD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region WALK_BACK
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Walk_Bwd_Start_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(WALK_BWD_START, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Walk_Bwd_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(WALK_BWD, WALK_BWD_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Walk_Bwd_Stop_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(WALK_BWD_STOP, WALK_BWD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region RUN
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Run_Start_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(RUN_START, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Run_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(RUN, RUN_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Run_Stop_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(RUN_STOP, RUN, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region DASH
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Dash_Backflip_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(DASH_BACK_FLIP, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Dash_Bwd_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(DASH_BWD, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Dash_Left_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(DASH_LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Dash_Right_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(DASH_RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region STUN
	//짧은스턴
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Stun_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(STUN, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	//긴 루프스턴
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Stun_Start_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(STUN_START, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Stun_Loop_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(STUN_LOOP, STUN_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Stun_End_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(STUN_END, STUN_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region 재공격하기위한 모션
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Taunt_Fist_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TAUNT_FIST, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Taunt_Sword_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TAUNT_SWORD, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region 뭔지모르겠음
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Replication_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(REPLICATION, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Richochet_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(RICHOCHET, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region 맞았을때모션
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Flinch_Left_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(FLINCH_LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Flinch_Right_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(FLINCH_RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Stagger_Left_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(STAGGER_LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Stagger_Right_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(STAGGER_RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region 회전
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Turn_135_Left_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_135LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Turn_135_Right_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_135RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Turn_180_Left_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_180LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Turn_180_Right_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_180RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Turn_45_Left_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_45LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Turn_45_Right_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_45RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Turn_90_Left_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_90LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Turn_90_Right_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_90RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion


#pragma region 상시어택애님
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Attack_Agg_Spin_360_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_AGG_SPIN_360, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Attack_Def_Beam_Center_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_DEF_BEAM_CENTER, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Attack_R1_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_R1, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Attack_R2_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_R2, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Attack_S2_Variant_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_S2_VARIANT, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Attack_S3_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_S3, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Attack_S5_SkewerProtocol_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_S5_SKEWER_PROTOCOL, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Attack_S6_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_S6, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion 

#pragma region 죽음
	//처형
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Attack_Finisher_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(EXECUTION, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	//죽음
	pAnim = m_pModel->Get_Animation("SK_Solaris.ao|A_Death_Solaris");
	if (FAILED(m_pAnimator->Insert_Animation(DEATH, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion 

	//자동으로 돌릴 애들(끝날애님, 끝나고시작할애님)
	m_pAnimator->Set_UpAutoChangeAnimation(WALK_FWD_START, WALK_FWD);
	m_pAnimator->Set_UpAutoChangeAnimation(WALK_FWD, WALK_FWD_STOP);
	m_pAnimator->Set_UpAutoChangeAnimation(WALK_FWD_STOP, IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation(WALK_BWD_START, WALK_BWD);
	m_pAnimator->Set_UpAutoChangeAnimation(WALK_BWD, WALK_BWD_STOP);
	m_pAnimator->Set_UpAutoChangeAnimation(WALK_BWD_STOP, IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation(RUN_START, RUN);
	m_pAnimator->Set_UpAutoChangeAnimation(RUN, RUN_STOP);
	m_pAnimator->Set_UpAutoChangeAnimation(RUN_STOP, IDLE);
	
	m_pAnimator->Set_UpAutoChangeAnimation(STUN_START, STUN_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation(STUN_LOOP, STUN_END);
	m_pAnimator->Set_UpAutoChangeAnimation(STUN_END, IDLE);


	///////////////////////////////////AnyEntry/////////////////////////////////////////
	m_pAnimator->Insert_AnyEntryAnimation(IDLE);
	m_pAnimator->Insert_AnyEntryAnimation(WALK_FWD_START);
	m_pAnimator->Insert_AnyEntryAnimation(WALK_BWD_START);
	m_pAnimator->Insert_AnyEntryAnimation(RUN_START);
	m_pAnimator->Insert_AnyEntryAnimation(STUN_START);

	//Dash
	m_pAnimator->Insert_AnyEntryAnimation(DASH_BACK_FLIP);
	m_pAnimator->Insert_AnyEntryAnimation(DASH_BWD);
	m_pAnimator->Insert_AnyEntryAnimation(DASH_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation(DASH_RIGHT);

	//Hit
	m_pAnimator->Insert_AnyEntryAnimation(FLINCH_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation(FLINCH_RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation(STAGGER_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation(STAGGER_RIGHT);

	//언제써야될지모르겠음
	m_pAnimator->Insert_AnyEntryAnimation(REPLICATION);
	m_pAnimator->Insert_AnyEntryAnimation(RICHOCHET);

	//다시공격할때사용할듯
	m_pAnimator->Insert_AnyEntryAnimation(TAUNT_FIST);
	m_pAnimator->Insert_AnyEntryAnimation(TAUNT_SWORD);

	//Attack
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_AGG_SPIN_360);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_DEF_BEAM_CENTER);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_R1);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_R2);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_S2_VARIANT);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_S3);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_S5_SKEWER_PROTOCOL);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_S6);

	//죽음
	m_pAnimator->Insert_AnyEntryAnimation(DEATH);
	m_pAnimator->Insert_AnyEntryAnimation(EXECUTION);

	//회전
	m_pAnimator->Insert_AnyEntryAnimation(TURN_135LEFT);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_135RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_180LEFT);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_180RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_90LEFT);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_90RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_45LEFT);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_45RIGHT);


	//시작애니메이션 지정
	m_pAnimator->Change_AnyEntryAnimation(IDLE);

	return S_OK;
}

HRESULT CBoss_Solaris::Set_State_FSM()
{
	if (FAILED(m_pStateController->Add_State(L"Idle", CBoss_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Walk_Front", CBoss_Walk_Front::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Dash_Back", CBoss_Dash_Back::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Back_Flip", CBoss_BackFlip::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Turn", CBoss_Turn::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Stun", CBoss_Stun::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Taunt_Fist", CBoss_Taunt_Fist::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Flinch_Left", CBoss_Flinch_Left::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Flinch_Right", CBoss_Flinch_Right::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Stagger_Left", CBoss_Stagger_Left::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Stagger_Right", CBoss_Stagger_Right::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Attack_Agg", CBoss_Attack_Agg::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Attack_R1", CBoss_Attack_R1::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Attack_R2", CBoss_Attack_R2::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Attack_S2_Variant", CBoss_Attack_S2_Variant::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Attack_S3", CBoss_Attack_S3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Attack_S5_Protocol", CBoss_Attack_S5_Protocol::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"Attack_S6", CBoss_Attack_S6::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//데스랑 처형은 안넣을거임 

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

HRESULT CBoss_Solaris::Set_Weapon()
{
	CHierarchyNode* pBone = m_pModel->Get_BoneMatrix("blade_middle");
	m_pWeapon = g_pGameInstance->Clone_GameObject<CBoss_Weapon>(m_iSceneID, L"Proto_GameObject_Weapon_Boss");

	if (!m_pWeapon)
	{
		MSGBOX("Boss Weapon Clone Fail");
		return E_FAIL;
	}

	m_pWeapon->Set_Owner(this);
	m_pWeapon->Set_FixedBone(pBone);
	m_pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());

	return S_OK;
}

void CBoss_Solaris::OnTriggerEnter(CCollision& collision)
{
}

void CBoss_Solaris::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_bDead || 0.f >= m_fCurrentHp)
		return;

	m_fCurrentHp -= _tAttackDesc.fDamage;
	
	if (!m_bGroggy)
	{
		++m_iHitCount;
		m_fGroggyGauge -= 2; //TODO::수치정해서바꿔줘야됨
	}

	if (m_bHitMotion)
	{
		if (EAttackDir::Left == _tAttackDesc.eDir)
		{
			if (1 == _tAttackDesc.iLevel || 2 == _tAttackDesc.iLevel)
			{
				m_pStateController->Change_State(L"Flinch_Left");
			}
			else if (3 == _tAttackDesc.iLevel)
			{
				m_pStateController->Change_State(L"Stagger_Left");
			}
		}
		else if (EAttackDir::Right == _tAttackDesc.eDir)
		{
			if (1 == _tAttackDesc.iLevel || 2 == _tAttackDesc.iLevel)
			{
				m_pStateController->Change_State(L"Flinch_Right");
			}
			else if (3 == _tAttackDesc.iLevel)
			{
				m_pStateController->Change_State(L"Stagger_Right");
			}
		}
	}

	if (3 <= m_iHitCount && ATTACK_S6 != m_pAnimator->Get_CurrentAnimNode())
	{
		uniform_int_distribution<_uint> iRange(0, 1);
		_uint iRandom = iRange(g_random);

		switch (iRandom)
		{
		case 0:
			m_pStateController->Change_State(L"Dash_Back");
			break;
		case 1:
			m_pStateController->Change_State(L"Back_Flip");
			break;
		}

		m_iHitCount = 0;
	}
	Active_Effect((_uint)EFFECT::HIT);
	Active_Effect((_uint)EFFECT::HIT_FLOATING);
	Active_Effect((_uint)EFFECT::HIT_FLOATING_2);
	Active_Effect((_uint)EFFECT::HIT_IMAGE);
}

void CBoss_Solaris::Execution()
{
	m_bDead = true;
	m_IsAttack = false;

	//m_pStateController->Change_State(L"Execution");
	m_pCharacterController->Remove_CCT();
}

void CBoss_Solaris::Set_IsAttack(const _bool _isAttack)
{
	m_IsAttack = _isAttack;
	if (m_pWeapon)
		m_pWeapon->Set_IsAttack(_isAttack);
}

void CBoss_Solaris::OnWeaponEffect()
{
}

void CBoss_Solaris::Set_Random_AttackAnim()
{
	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	cout << fDistToPlayer << endl;
	if (0.6f < Get_HpRatio())
	{
		//레이저없음
		if (8.f > fDistToPlayer)
		{
			uniform_int_distribution<_uint> iRange(0, 2);
			_uint iRandom = iRange(g_random);
			
			while (iRandom == m_iPreAnim)
			{
				iRandom = iRange(g_random);
			}

			switch (iRandom)
			{
			case 0:
				m_pStateController->Change_State(L"Attack_Agg");
				break;
			case 1:
				m_pStateController->Change_State(L"Attack_R2");
				break;
			case 2:
				m_pStateController->Change_State(L"Attack_S3");
				break;
			}
			m_iPreAnim = iRandom;
		}
		else if (8.f <= fDistToPlayer && 10.f >= fDistToPlayer)
		{
			m_pStateController->Change_State(L"Attack_R1");
		}
		else if (10.f <= fDistToPlayer)
		{
			m_pStateController->Change_State(L"Attack_S5_Protocol");
		}
	}
	else if (0.6f >= Get_HpRatio())
	{
		if (8.f > fDistToPlayer)
		{
			uniform_int_distribution<_uint> iRange(0, 3);
			_uint iRandom = iRange(g_random);

			while (iRandom == m_iPreAnim)
			{
				iRandom = iRange(g_random);
			}

			switch (iRandom)
			{
			case 0:
				m_pStateController->Change_State(L"Attack_Agg");
				break;
			case 1:
				m_pStateController->Change_State(L"Attack_R2");
				break;
			case 2:
				m_pStateController->Change_State(L"Attack_S3");
				break;
			case 3:
				m_pStateController->Change_State(L"Attack_S2_Variant");
				break;
			}
			m_iPreAnim = iRandom;
		}
		if (8.f <= fDistToPlayer)
		{
			uniform_int_distribution<_uint> iRange(0, 2);
			_uint iRandom = iRange(g_random);

			while (iRandom == m_iPreAnim)
			{
				iRandom = iRange(g_random);
			}

			switch (iRandom)
			{
			case 0:
				m_pStateController->Change_State(L"Attack_S5_Protocol");
				break;
			case 1:
				m_pStateController->Change_State(L"Attack_S6");
				break;
			case 2:
				m_pStateController->Change_State(L"Attack_R1");
				break;
			}
			m_iPreAnim = iRandom;
		}
	}

	m_pStateController->Change_State(L"Attack_R2");
}


CBoss_Solaris* CBoss_Solaris::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CBoss_Solaris* pInstance = new CBoss_Solaris(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CBoss_Solaris");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBoss_Solaris::Clone(const _uint _iSceneID, void* pArg)
{
	CBoss_Solaris* pInstance = new CBoss_Solaris(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CBoss_Solaris");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Solaris::Free()
{
	__super::Free();

	Safe_Release(m_pCharacterController);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimator);
}
