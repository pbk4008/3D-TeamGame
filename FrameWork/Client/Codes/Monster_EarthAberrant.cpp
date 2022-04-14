#include "pch.h"
#include "Monster_EarthAberrant.h"

#include "Animation.h"
#include "UI_Monster_Panel.h"
#include "EarthAberrant_Pick.h"

#include "Aberrant_Idle.h"
#include "Aberrant_Walk.h"
#include "Aberrant_Run.h"
#include "Aberrant_Attack.h"
#include "Aberrant_Flinch_Left.h"
#include "Aberrant_Dash_Bwd.h"
#include "Aberrant_Stun.h"
#include "Aberrant_Death.h"

#include "Stage1.h"

CMonster_EarthAberrant::CMonster_EarthAberrant(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CActor(_pDevice, _pDeviceContext)
{
}

CMonster_EarthAberrant::CMonster_EarthAberrant(const CMonster_EarthAberrant& _rhs)
	:CActor(_rhs)
	, m_pCharacterController(_rhs.m_pCharacterController)
	, m_pStateController(_rhs.m_pStateController)
	, m_pAnimatorCom(_rhs.m_pAnimatorCom)
	, m_pPanel(_rhs.m_pPanel)
{
	Safe_AddRef(m_pPanel);
	Safe_AddRef(m_pCharacterController);
	Safe_AddRef(m_pStateController);
	Safe_AddRef(m_pAnimatorCom);
}

HRESULT CMonster_EarthAberrant::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_EarthAberrant::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
	{
		return E_FAIL;
	}
	
	if (_pArg)
	{
		_float3 vPoint = (*(_float3*)_pArg);
		if (FAILED(Set_SpawnPosition(vPoint)))
			return E_FAIL;
	}
	else
	{
		_vector Pos = { 0.f, 1.f, 5.f, 1.f };
		m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);
	}
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	if (FAILED(Set_Animation_FSM()))
	{
		return E_FAIL;
	}

	if (FAILED(Set_State_FSM()))
	{
		return E_FAIL;
	}

	if (FAILED(Set_Weapon()))
	{
		return E_FAIL;
	}

	if (FAILED(Set_Panel()))
	{
		return E_FAIL;
	}

	m_bIsFall = true;
	m_iObectTag = (_uint)GAMEOBJECT::MONSTER_ABERRANT;

	m_fMaxHp = 5.f;
	m_fCurrentHp = m_fMaxHp;

	m_fMaxGroggyGauge = 100.f;
	m_fGroggyGauge = 0.f;

	m_pPanel->Set_HpBar(Get_HpRatio());
	m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());

	m_tAttackDesc.iLevel = 1;
	m_tAttackDesc.fDamage = 3.f;

	setActive(false);

	return S_OK;
}

_int CMonster_EarthAberrant::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}
	//string str;
	//str.assign(m_pStateController->Get_CurStateTag().begin(), m_pStateController->Get_CurStateTag().end());
	//cout << str << endl;
	m_pTransform->Set_Velocity(XMVectorZero());
	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	if (m_bIsFall)
		m_pTransform->Fall(_dDeltaTime);

	_int iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (nullptr != m_pWeapon)
	{
		if (-1 == m_pWeapon->Tick(_dDeltaTime))
		{
			return -1;
		}
	}
	if (!m_bDead)
	{
		if (0 >= m_fCurrentHp)
		{
			CLevel* pLevel = g_pGameInstance->getCurrentLevelScene();
			if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE1)
				static_cast<CStage1*>(pLevel)->Minus_MonsterCount();

			m_bDead = true;
			m_IsAttack = false;
			m_pWeapon->Set_IsAttack(false);
			m_pStateController->Change_State(L"Death");
			m_pCharacterController->Remove_CCT();
		}
		else
			m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());
	}
	else
	{
		if (L"Death" == m_pStateController->Get_CurStateTag())
		{
			if (m_pAnimatorCom->Get_CurrentAnimation()->Is_Finished() && 
				!m_pAnimatorCom->Get_IsLerp())
			{
				Set_Remove(true);
				m_pPanel->Set_UIRemove(true);
			}
			if (9 <= m_pAnimatorCom->Get_AnimController()->Get_CurKeyFrameIndex() 
				&& 10 > m_pAnimatorCom->Get_AnimController()->Get_CurKeyFrameIndex())
			{
				Active_Effect((_uint)EFFECT::DEATH);
			}
		}
		else
		{
			Set_Remove(true);
			m_pPanel->Set_UIRemove(true);
			Active_Effect((_uint)EFFECT::DEATH);
		}
	}

	if (true == m_bUIShow)
		m_pPanel->Set_Show(true);

	if (false == m_bUIShow)
		m_pPanel->Set_Show(false);

	if (m_fGroggyGauge >= m_fMaxGroggyGauge)
	{
		//스턴상태일때 스턴state에서 현재 그로기 계속 0으로 고정시켜줌
		m_bGroggy = true;
		m_pStateController->Change_State(L"Stun");
		m_fGroggyGauge = 0.f;
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}

	if (true == m_bGroggy || true == m_bDead)
	{
		m_fGroggyGauge = 0.f;
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}
	if (STUN_END == m_pAnimatorCom->Get_CurrentAnimNode())
	{
		if (m_pAnimatorCom->Get_AnimController()->Is_Finished())
		{
			m_bGroggy = false;
		}
	}

	return 0;
}

_int CMonster_EarthAberrant::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
	}

	_int iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
	{
		return iProgress;
	}
	if(!m_bDead)
		m_pCharacterController->Update_OwnerTransform();

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	if (nullptr != m_pWeapon)
	{
		if (-1 == m_pWeapon->LateTick(_dDeltaTime))
		{
			return E_FAIL;
		}
	}

	return 0;
}

HRESULT CMonster_EarthAberrant::Render()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		SCB desc;
		ZeroMemory(&desc, sizeof(SCB));
		switch (i)
		{
		case 0: // crystal
			desc.color = _float4(0.831f, 0.43f, 0.643f, 1.f);
			desc.empower = 0.7f;
			CActor::BindConstantBuffer(wstrCamTag, &desc);
			m_pModel->Render(i, 5);
			break;
		case 1: // body
			CActor::BindConstantBuffer(wstrCamTag, &desc);
			m_pModel->Render(i, 4);
			break;
		}
	}

	return S_OK;
}

HRESULT CMonster_EarthAberrant::Render_Shadow()
{
	CActor::BindConstantBuffer(L"Camera_Silvermane");
	CActor::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 3);

	return S_OK;
}

HRESULT CMonster_EarthAberrant::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_EarthAberrant", L"Com_Model", (CComponent**)&m_pModel)))
	{
		return E_FAIL;
	}

	_matrix matPivot = XMMatrixIdentity();
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModel;
	tDesc.pTransform = m_pTransform;
	tDesc.eType = CAnimationController::EType::CharacterController;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimatorCom, &tDesc)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"Com_StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);



	/* for.Character Controller */
	CCharacterController::DESC tCharacterControllerDesc;
	tCharacterControllerDesc.fHeight = 1.f;
	tCharacterControllerDesc.fRadius = 0.5f;
	tCharacterControllerDesc.fContactOffset = tCharacterControllerDesc.fRadius * 0.1f;
	tCharacterControllerDesc.fStaticFriction = 0.5f;
	tCharacterControllerDesc.fDynamicFriction = 0.5f;
	tCharacterControllerDesc.fRestitution = 0.f;
	tCharacterControllerDesc.pGameObject = this;
	tCharacterControllerDesc.vPosition = { 0.f, 0.f, 0.f };

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCharacterControllerDesc)))
		return E_FAIL;
	m_pCharacterController->setOwnerTransform(m_pTransform);
	m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);

	return S_OK;
}

HRESULT CMonster_EarthAberrant::Set_Animation_FSM()
{
	CAnimation* pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Idle_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(IDLE, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	//hit
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Attack_Execution_Mook");
	if (FAILED(m_pAnimatorCom->Insert_Animation(ATTACK_EXECUTION, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Flinch_Left_1_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(FLINCH_LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Flinch_Right_1_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(FLINCH_RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Ricochet_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(RICOCHET, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Stagger_Left_1_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(STAGGER_LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Stagger_Right_1_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(STAGGER_RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	//attack
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Attack_R1_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(ATTACK_R1, HEAD, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Attack_R2_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(ATTACK_R2, HEAD, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;

	//stun
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Stun_Start_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(STUN_START, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Stun_Loop_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(STUN_LOOP, STUN_START, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Stun_End_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(STUN_END, STUN_LOOP, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	//if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, STUN_END, false)))
	//	return E_FAIL;

	//dash
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Dash_Bwd_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(DASH_BWD, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Dash_Left_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(DASH_LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Dash_Right_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(DASH_RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	//death
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Death_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(DEATH, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	//run
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Run_Fwd_Start_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(RUN_FWD_START, HEAD, pAnim, true, false, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Run_Fwd_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(RUN_FWD, RUN_FWD_START, pAnim, true, false, true, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Run_Fwd_Stop_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(RUN_FWD_STOP, RUN_FWD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	/*if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, RUN_FWD, false)))
		return E_FAIL;*/


	//walk
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Bwd_Start_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_BWD_START, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Bwd_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_BWD, WALK_BWD_START, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Bwd_Stop_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_BWD_STOP, WALK_BWD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	/*if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, WALK_BWD_STOP, false)))
		return E_FAIL;*/


	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Fwd_Start_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_FWD_START, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Fwd_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_FWD, WALK_FWD_START, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Fwd_Stop_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_FWD_STOP, WALK_FWD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	/*if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, WALK_FWD_STOP, false)))
		return E_FAIL;*/


	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Left_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_LEFT_START, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Fwd_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_LEFT, WALK_LEFT_START, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Left_Start_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_LEFT_STOP, WALK_LEFT, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	/*if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, WALK_LEFT_STOP, false)))
		return E_FAIL;*/


	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Right_Start_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_RIGHT_START, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Right_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_RIGHT, WALK_RIGHT_START, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Right_Stop_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_RIGHT_STOP, WALK_RIGHT, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	/*if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, WALK_RIGHT_STOP, false)))
		return E_FAIL;*/


	//넉백
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_Walk_Left_Start_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(KNOCKBACK, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("SK_Earth_Aberrant_B.ao|A_GetUp_Aberrant");
	if (FAILED(m_pAnimatorCom->Insert_Animation(GET_UP, KNOCKBACK, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;
	/*if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, GET_UP, false)))
		return E_FAIL;*/


	
	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, RUN_FWD_STOP, false)))
		return E_FAIL;
	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, WALK_BWD_STOP, false)))
		return E_FAIL;
	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, WALK_FWD_STOP, false)))
		return E_FAIL;
	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, WALK_LEFT_STOP, false)))
		return E_FAIL;
	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, WALK_RIGHT_STOP, false)))
		return E_FAIL;
	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, GET_UP, false)))
		return E_FAIL;
	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, STUN_END, false)))
		return E_FAIL;


	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_BWD_START, WALK_BWD);
	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_BWD, WALK_BWD_STOP);
	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_BWD_STOP, IDLE);

	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_FWD_START, WALK_FWD);
	//m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_FWD, WALK_FWD_STOP);
	//m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_FWD_STOP, IDLE);

	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_LEFT_START, WALK_LEFT);
	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_LEFT, WALK_LEFT_STOP);
	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_LEFT_STOP, IDLE);

	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_RIGHT_START, WALK_RIGHT);
	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_RIGHT, WALK_RIGHT_STOP);
	m_pAnimatorCom->Set_UpAutoChangeAnimation(WALK_RIGHT_STOP, IDLE);

	m_pAnimatorCom->Set_UpAutoChangeAnimation(STUN_START, STUN_LOOP);
	m_pAnimatorCom->Set_UpAutoChangeAnimation(STUN_LOOP, STUN_END);
	m_pAnimatorCom->Set_UpAutoChangeAnimation(STUN_END, IDLE);

	m_pAnimatorCom->Set_UpAutoChangeAnimation(RUN_FWD_START, RUN_FWD);
	//m_pAnimatorCom->Set_UpAutoChangeAnimation(RUN_FWD, RUN_FWD_STOP);
	//m_pAnimatorCom->Set_UpAutoChangeAnimation(RUN_FWD_STOP, IDLE);

	m_pAnimatorCom->Set_UpAutoChangeAnimation(KNOCKBACK, GET_UP);
	m_pAnimatorCom->Set_UpAutoChangeAnimation(GET_UP, IDLE);

	//Insert
	m_pAnimatorCom->Insert_AnyEntryAnimation(ATTACK_EXECUTION);
	m_pAnimatorCom->Insert_AnyEntryAnimation(FLINCH_LEFT);
	m_pAnimatorCom->Insert_AnyEntryAnimation(FLINCH_RIGHT);
	m_pAnimatorCom->Insert_AnyEntryAnimation(RICOCHET);
	m_pAnimatorCom->Insert_AnyEntryAnimation(STAGGER_LEFT);
	m_pAnimatorCom->Insert_AnyEntryAnimation(STAGGER_RIGHT);

	m_pAnimatorCom->Insert_AnyEntryAnimation(STUN_START);

	m_pAnimatorCom->Insert_AnyEntryAnimation(ATTACK_R1);
	m_pAnimatorCom->Insert_AnyEntryAnimation(ATTACK_R2);

	m_pAnimatorCom->Insert_AnyEntryAnimation(DASH_BWD);
	m_pAnimatorCom->Insert_AnyEntryAnimation(DASH_LEFT);
	m_pAnimatorCom->Insert_AnyEntryAnimation(DASH_RIGHT);
	
	m_pAnimatorCom->Insert_AnyEntryAnimation(DEATH);

	m_pAnimatorCom->Insert_AnyEntryAnimation(RUN_FWD_START);
	m_pAnimatorCom->Insert_AnyEntryAnimation(WALK_BWD_START);
	m_pAnimatorCom->Insert_AnyEntryAnimation(WALK_FWD_START);
	m_pAnimatorCom->Insert_AnyEntryAnimation(WALK_LEFT_START);
	m_pAnimatorCom->Insert_AnyEntryAnimation(WALK_RIGHT_START);

	m_pAnimatorCom->Insert_AnyEntryAnimation(KNOCKBACK);


	m_pAnimatorCom->Change_Animation(IDLE);

	_uint iRand = rand() % 15;
	m_pAnimatorCom->Add_AnimFrame(IDLE, iRand);

	return S_OK;
}

HRESULT CMonster_EarthAberrant::Set_State_FSM()
{
	if (FAILED(m_pStateController->Add_State(L"Idle", CAberrant_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Walk", CAberrant_Walk::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Run", CAberrant_Run::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Attack", CAberrant_Attack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Flinch_Left", CAberrant_Flinch_Left::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Dash_Bwd", CAberrant_Dash_Bwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Stun", CAberrant_Stun::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Death", CAberrant_Death::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CMonster_FSM*>(pair.second)->Set_Monster(this);
		static_cast<CMonster_FSM*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CMonster_FSM*>(pair.second)->Set_Model(m_pModel);
		static_cast<CMonster_FSM*>(pair.second)->Set_Animator(m_pAnimatorCom);
	}

	m_pStateController->Change_State(L"Idle");

	return S_OK;
}

HRESULT CMonster_EarthAberrant::Set_Weapon()
{
	CHierarchyNode* pBone = m_pModel->Get_BoneMatrix("weapon_r_end");
	//CEarthAberrant_Pick* pWeapon = CEarthAberrant_Pick::Create(m_pDevice, m_pDeviceContext);
	m_pWeapon = g_pGameInstance->Clone_GameObject<CEarthAberrant_Pick>(m_iSceneID, L"Proto_GameObject_Weapon_EarthAberrant_Pick");

	if (!m_pWeapon)
	{
		MSGBOX("Earth Weapon Clone Fail");
		return E_FAIL;
	}

	m_pWeapon->Set_Owner(this);
	m_pWeapon->Set_FixedBone(pBone);
	m_pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());

	return S_OK;
}

HRESULT CMonster_EarthAberrant::Set_Panel()
{
	//MonsterBar Panel
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::ABERRANT;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;

	Safe_AddRef(m_pPanel);

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	return S_OK;
}

void CMonster_EarthAberrant::OnTriggerEnter(CCollision& collision)
{
}

void CMonster_EarthAberrant::OnTriggerExit(CCollision& collision)
{
	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Monster_Hit);
}

void CMonster_EarthAberrant::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_bDead || 0.f >= m_fCurrentHp)
		return;

	m_pPanel->Set_Show(true);


	if (false == m_bFirstHit)
	{
		m_bFirstHit = true; //딱 한번 true로 변경해줌
		m_pPanel->Set_BackUIGapY(1.f);
	}

	g_pGameInstance->Play_Shot(L"Monster_Hit_2", CSoundMgr::CHANNELID::Earth_Hit);

	m_fCurrentHp -= _tAttackDesc.fDamage;
	m_fGroggyGauge += 2; //TODO::수치정해서바꿔줘야됨

	m_pPanel->Set_HpBar(Get_HpRatio());

	if (false == m_bGroggy)
	{
		//그로기 아닐때만 증가할수있게
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
		m_pStateController->Change_State(L"Flinch_Left");
	}

	
}

void CMonster_EarthAberrant::Parry(const PARRYDESC& _tParryDesc)
{
	m_fGroggyGauge += (m_fMaxGroggyGauge - m_fGroggyGauge);
}

void CMonster_EarthAberrant::Set_IsAttack(const _bool _isAttack)
{
	m_IsAttack = _isAttack;
	if (m_pWeapon)
		m_pWeapon->Set_IsAttack(_isAttack);
}

void CMonster_EarthAberrant::Set_Remove(_bool bCheck)
{
	m_bRemove = bCheck;
	m_pPanel->Set_UIRemove(bCheck);
}

CMonster_EarthAberrant* CMonster_EarthAberrant::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_EarthAberrant* pInstance = new CMonster_EarthAberrant(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMonster_EarthAberrant");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_EarthAberrant::Clone(const _uint _iSceneID, void* _pArg)
{
	CMonster_EarthAberrant* pInstance = new CMonster_EarthAberrant(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CMonster_EarthAberrant");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_EarthAberrant::Free()
{
	Safe_Release(m_pPanel);
	Safe_Release(m_pCharacterController);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimatorCom);

	__super::Free();
}
