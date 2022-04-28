#include "pch.h"
#include "Monster_BronzeAnimus.h"
#include "Animation.h"
#include "BronzeAnimus_Sword.h"
#include "UI_Monster_Panel.h"
#include "HierarchyNode.h"

/* for. FSM */
#include "BronzeAnimus_State.h"
#include "BronzeAnimus_Idle.h"
#include "BronzeAnimus_Battle_Idle.h"
#include "BronzeAnimus_Chaser.h"
#include "BronzeAnimus_Death.h"
#include "BronzeAnimus_Groggy.h"
#include "BronzeAnimus_Hit.h"
#include "BronzeAnimus_Roar.h"
#include "BronzeAnimus_Attack.h"
#include "BronzeAnimus_Excution.h"

#include "Stage1.h"
#include "Stage2.h"
#include "DamageFont.h"

CMonster_BronzeAnimus::CMonster_BronzeAnimus(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
	, m_pCharacterController(nullptr)
	, m_pStateController(nullptr)
	, m_pAnimator(nullptr)
{
}

CMonster_BronzeAnimus::CMonster_BronzeAnimus(const CMonster_BronzeAnimus& _rhs)
	: CActor(_rhs)
	, m_pCharacterController(_rhs.m_pCharacterController)
	, m_pStateController(_rhs.m_pStateController)
	, m_pAnimator(_rhs.m_pAnimator)
{
	Safe_AddRef(m_pCharacterController);
	Safe_AddRef(m_pStateController);
	Safe_AddRef(m_pAnimator);
}

HRESULT CMonster_BronzeAnimus::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::MONSTER_ANIMUS;

	return S_OK;
}

HRESULT CMonster_BronzeAnimus::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg))) 
		return E_FAIL;

	if (_pArg)
	{
		_float3 vPoint = (*(_float3*)_pArg);
		if (FAILED(Set_SpawnPosition(vPoint)))
			return E_FAIL;
	}
	else
	{
		_float4 vPosition = { 3.f, 2.f, 3.f, 1.f };
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;
	if (FAILED(Ready_Weapon()))
		return E_FAIL;
	if (FAILED(Ready_AnimFSM()))
		return E_FAIL;
	if (FAILED(Ready_StateFSM()))
		return E_FAIL;

	m_fMaxGroggyGauge = 10.f;
	m_fGroggyGauge = 0.f;
	m_fMaxHp = 30.f;
	m_fCurrentHp = m_fMaxHp;
	m_isFall = true;
	m_tAttackDesc.iLevel = 1;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	m_pWeapon->setActive(false);
	m_pPanel->setActive(false);
	setActive(false);
	return S_OK;
}

_int CMonster_BronzeAnimus::Tick(_double _dDeltaTime)
{
	if (true == m_bUIShow)
	{
		m_pPanel->Set_Show(true);
	}

	if (false == m_bUIShow)
	{
		m_pPanel->Set_Show(false);
	}

	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;
	m_pTransform->Set_Velocity(XMVectorZero());

	/* State FSM Update */
	iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_isFall)
		m_pTransform->Fall(_dDeltaTime);

	/* Weapon Bone Update */
	m_pWeapon->Tick(_dDeltaTime);

	if(!m_bDead)
		m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());
	else
	{
		if (m_pStateController->Get_CurStateTag()== L"Death")
		{
			if (1 < m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex() && 2 >= m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex())
				Active_Effect((_uint)EFFECT::DEATH);

			if (m_pAnimator->Get_CurrentAnimation()->Is_Finished() && m_lifetime <= 0.f)
			{
				m_bdissolve = true;
				m_pPanel->Set_UIRemove(true);
			}

			if (m_lifetime >= 1.f)
			{
				CLevel* pLevel = g_pGameInstance->getCurrentLevelScene();
				if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE2)
					static_cast<CStage2*>(pLevel)->Minus_MonsterCount();

				Set_Remove(true);
			}
		}
		else if (m_pStateController->Get_CurStateTag() == L"Excution")
		{
			if (m_pAnimator->Get_CurrentAnimation()->Is_Finished() && m_lifetime <= 0.f)
			{
				m_bdissolve = true;
				m_pPanel->Set_UIRemove(true);
			}

			if (m_lifetime >= 1.f)
			{
				CLevel* pLevel = g_pGameInstance->getCurrentLevelScene();
				if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE2)
					static_cast<CStage2*>(pLevel)->Minus_MonsterCount();

				Set_Remove(true);
			}
		}
		else
		{
			Set_Remove(true);
			m_pPanel->Set_UIRemove(true);
			Active_Effect((_uint)EFFECT::DEATH);
		}
	}

	if (m_fGroggyGauge >= m_fMaxGroggyGauge)
	{
		//스턴상태일때 스턴state에서 현재 그로기 계속 0으로 고정시켜줌
		m_bGroggy = true;
		m_fGroggyGauge = 0.f;
		m_pStateController->Change_State(L"Groggy");
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}

	if (true == m_bGroggy || true == m_bDead)
	{
		m_fGroggyGauge = 0.f;
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}

	if ((_uint)ANIM_TYPE::A_STUN_ED == m_pAnimator->Get_CurrentAnimNode())
	{
		if (m_pAnimator->Get_AnimController()->Is_Finished())
		{
			m_bGroggy = false;
		}
	}

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	CActor::LightOnOff(m_pTransform->Get_State(CTransform::STATE_POSITION), XMVectorSet(0.f, 1.f, 0.f, 1.f), 10.f);

	return _int();
}

_int CMonster_BronzeAnimus::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;

	if(!m_bDead)
		m_pCharacterController->Update_OwnerTransform();

	m_pWeapon->LateTick(_dDeltaTime);

	/* State FSM Late Update */
	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMonster_BronzeAnimus::Render()
{
	if (m_bdissolve == true)
		CActor::DissolveOn(1.2f);

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_bdissolve", &m_bdissolve, sizeof(_bool)))) MSGBOX("Failed to Apply dissolvetime");

	SCB desc;
	ZeroMemory(&desc, sizeof(SCB));

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CActor::BindConstantBuffer(wstrCamTag, &desc);

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 6);

#ifdef _DEBUG
	//Render_Debug();
#endif
	return S_OK;
}

HRESULT CMonster_BronzeAnimus::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CActor::BindConstantBuffer(wstrCamTag);
	CActor::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 3);

	return S_OK;
}

void CMonster_BronzeAnimus::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_bDead || 0.f >= m_fCurrentHp)
		return;

	m_pPanel->Set_Show(true);
	m_fCurrentHp -= _tAttackDesc.fDamage;
	CCollision collision;
	collision.pGameObject = _tAttackDesc.pHitObject;

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
		MSGBOX(L"데미지 폰트 생성 실패");

	Hit(collision);
}

void CMonster_BronzeAnimus::Parry(const PARRYDESC& _tParryDesc)
{
	m_fGroggyGauge += (m_fMaxGroggyGauge - m_fGroggyGauge);
	GroggyStart();
}

void CMonster_BronzeAnimus::Execution()
{
	CLevel* pLevel = g_pGameInstance->getCurrentLevelScene();
	if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE2)
		static_cast<CStage2*>(pLevel)->Minus_MonsterCount();

	Set_Dead();
	Remove_Collider();
	Set_IsAttack(false);
	m_pStateController->Change_State(L"Excution");
}

void CMonster_BronzeAnimus::setActive(_bool bActive)
{
	CGameObject::setActive(bActive);

	if (bActive)
	{
		if (!m_pCharacterController)
		{
			//* for.Character Controller */
			CCharacterController::DESC tCharacterControllerDesc;
			tCharacterControllerDesc.fHeight = 1.f;
			tCharacterControllerDesc.fRadius = 0.5f;
			tCharacterControllerDesc.fContactOffset = tCharacterControllerDesc.fRadius * 0.1f;
			tCharacterControllerDesc.fStaticFriction = 0.5f;
			tCharacterControllerDesc.fDynamicFriction = 0.5f;
			tCharacterControllerDesc.fRestitution = 0.f;
			tCharacterControllerDesc.pGameObject = this;
			tCharacterControllerDesc.vPosition = { 0.f, 0.f, 0.f };

			if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCharacterControllerDesc)))
				MSGBOX(L"브론즈 cct 생성 실패");
			m_pCharacterController->setOwnerTransform(m_pTransform);
			m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);
		}
		if (m_pWeapon)
			m_pWeapon->setActive(true);
		if (m_pPanel)
			m_pPanel->setActive(true);
	}
}

HRESULT CMonster_BronzeAnimus::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 2.0f;
	transformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(transformDesc);
	//밖으로 뺌
	//_float4 vPosition = { 3.f, 2.f, 3.f, 1.f };

	//m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));


	// 모델
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_BronzeAnimus", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	////* for.Character Controller */
	//CCharacterController::DESC tCharacterControllerDesc;
	//tCharacterControllerDesc.fHeight = 1.f;
	//tCharacterControllerDesc.fRadius = 0.5f;
	//tCharacterControllerDesc.fContactOffset = tCharacterControllerDesc.fRadius * 0.1f;
	//tCharacterControllerDesc.fStaticFriction = 0.5f;
	//tCharacterControllerDesc.fDynamicFriction = 0.5f;
	//tCharacterControllerDesc.fRestitution = 0.f;
	//tCharacterControllerDesc.pGameObject = this;
	//tCharacterControllerDesc.vPosition = { 0.f, 0.f, 0.f };

	//if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCharacterControllerDesc)))
	//	return E_FAIL;
	//m_pCharacterController->setOwnerTransform(m_pTransform);
	//m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);

	// 스테이트 컨트롤러
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	m_AanimDesc.pModel = m_pModel;
	m_AanimDesc.pTransform = m_pTransform;
	m_AanimDesc.eType = CAnimationController::EType::CharacterController;

	//Anim FSM
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Animator", (CComponent**)&m_pAnimator, &m_AanimDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_BronzeAnimus::Ready_Weapon(void)
{
	m_pWeapon = static_cast<CBronzeAnimus_Sword*>(g_pGameInstance->Clone_GameObject(m_iSceneID, L"Proto_GameObject_Weapon_BronzeAnimus_Sword"));

	if (!m_pWeapon)
		return E_FAIL;

	m_pWeapon->Set_Owner(this);

	vector<CHierarchyNode*> vecNode = m_pModel->Get_HierachyNodes();
	CHierarchyNode* pWeaponBone = nullptr;
	for (auto& pNode : vecNode)
	{
		if (!strcmp(pNode->Get_Name(), "weapon_r_end"))
		{
			pWeaponBone = pNode;
			break;
		}
	}
	m_pWeapon->Set_FixedBone(pWeaponBone);
	m_pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	return S_OK;
}

HRESULT CMonster_BronzeAnimus::Ready_UI()
{	
	// MonsterBar Panel
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::ANIMUS;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;
	Safe_AddRef(m_pPanel);
	m_pPanel->Set_HpBar(Get_HpRatio());
	m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	return S_OK;
}

HRESULT CMonster_BronzeAnimus::Ready_AnimFSM(void)
{
#pragma region Default
	CAnimation* pAnimation = m_pModel->Get_Animation("A_Idle");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, TRUE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Idle_Battle");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_IDLE_BATTLE, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Death");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_DEATH, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
	
	pAnimation = m_pModel->Get_Animation("A_Ricochet");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_RICOCHET, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Roar");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ROAR, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region Attack
	pAnimation = m_pModel->Get_Animation("A_Attack_R1");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ATTACK_R1, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Attack_R2");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ATTACK_R2, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Attack_S1");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ATTACK_S1, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;
#pragma endregion

#pragma region Hit
 	pAnimation = m_pModel->Get_Animation("A_Flinch");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_FLINCH, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion
#pragma region Walk
	pAnimation = m_pModel->Get_Animation("A_Walk_Bwd_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_WALK_BWD_ST, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Walk_Bwd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_WALK_BWD, (_uint)ANIM_TYPE::A_WALK_BWD_ST, pAnimation, TRUE, TRUE, TRUE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Walk_Bwd_End");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_WALK_BWD_ED, (_uint)ANIM_TYPE::A_WALK_BWD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Walk_Fwd_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_WALK_FWD_ST, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Walk_Fwd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_WALK_FWD, (_uint)ANIM_TYPE::A_WALK_FWD_ST, pAnimation, TRUE, TRUE, TRUE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Walk_Fwd_End");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_WALK_FWD_ED, (_uint)ANIM_TYPE::A_WALK_FWD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion
#pragma region Stun
	pAnimation = m_pModel->Get_Animation("A_Stun_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_STUN_ST, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Stun");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_STUN, (_uint)ANIM_TYPE::A_STUN_ST, pAnimation, TRUE, TRUE, TRUE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Stun_End");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_STUN_ED, (_uint)ANIM_TYPE::A_STUN, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Excution");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_EXCUTION, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;
#pragma endregion
#pragma region Turn
	//pAnimation = m_pModel->Get_Animation("A_Trun_45_Right");
	//pAnimation = m_pModel->Get_Animation("A_Turn_45_Left");
	//pAnimation = m_pModel->Get_Animation("A_Turn_90_Left");
	//pAnimation = m_pModel->Get_Animation("A_Turn_90_Right");
	//pAnimation = m_pModel->Get_Animation("A_Turn_135_Left");
	//pAnimation = m_pModel->Get_Animation("A_Turn_135_Right");
	//pAnimation = m_pModel->Get_Animation("A_Turn_180_Left");
	//pAnimation = m_pModel->Get_Animation("A_Turn_180_Right");
#pragma endregion

#pragma region  Set Any Entry Animation
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_IDLE);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_IDLE_BATTLE);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_DEATH);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ROAR);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_FLINCH);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_STUN_ST);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_STUN_ED);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ATTACK_R1);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ATTACK_R2);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ATTACK_S1);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ST);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ED);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_BWD_ST);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_BWD_ED);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_135_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_135_RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_180_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_180_RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_90_RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_90_RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_EXCUTION);
#pragma endregion

#pragma region  Auto Change Anim
	/* ##1.끝나는 애님 ##2.루트할 애님  */
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ST, (_uint)ANIM_TYPE::A_WALK_FWD);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD, (_uint)ANIM_TYPE::A_WALK_FWD_ED);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_BWD_ST, (_uint)ANIM_TYPE::A_WALK_BWD);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_BWD, (_uint)ANIM_TYPE::A_WALK_BWD_ED);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_STUN_ST, (_uint)ANIM_TYPE::A_STUN);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_STUN, (_uint)ANIM_TYPE::A_STUN_ED);
#pragma endregion

	return S_OK;
}

HRESULT CMonster_BronzeAnimus::Ready_StateFSM(void)
{
	/* for. Idle */
	if (FAILED(m_pStateController->Add_State(L"Idle", CBronzeAnimus_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Battle Idle */
	if (FAILED(m_pStateController->Add_State(L"A_Idle_Battle", CBronzeAnimus_Battle_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for.Roar */
	if (FAILED(m_pStateController->Add_State(L"Roar", CBronzeAnimus_Roar::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Player Chaser */
	if (FAILED(m_pStateController->Add_State(L"Chaser", CBronzeAnimus_Chaser::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Attack*/
	if (FAILED(m_pStateController->Add_State(L"Attack", CBronzeAnimus_Attack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Death */
	if (FAILED(m_pStateController->Add_State(L"Death", CBronzeAnimus_Death::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Hit */
	if (FAILED(m_pStateController->Add_State(L"Hit", CBronzeAnimus_Hit::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Groggy */
	if (FAILED(m_pStateController->Add_State(L"Groggy", CBronzeAnimus_Groggy::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Excution */
	if(FAILED(m_pStateController->Add_State(L"Excution", CBronzeAnimus_Excution::Create(m_pDevice, m_pDeviceContext))))
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

HRESULT CMonster_BronzeAnimus::Render_Debug(void)
{
	if (FAILED(m_pStateController->Render()))
		return E_FAIL;

	////Hp
	//if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), L"HP : " + to_wstring(m_fCurrentHp), _float2(950.f, 20.f), _float2(0.6f, 0.6f))))
	//	return E_FAIL;

	//// FSM
	//wstring wstrCurStateTag = m_pStateController->Get_CurStateTag();
	//wstring wstrState = L"Cur State : ";

	//if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrState + wstrCurStateTag).c_str(), _float2(950.f, 40.f), _float2(0.6f, 0.6f))))
	//	return E_FAIL;

	//m_pAnimation = m_pAnimator->Get_CurrentAnimation();

	//// 애니메이션 이름
	//string CurAnimName = m_pAnimation->Get_Name();
	//wstring wstrCurAnimTag;
	//wstring wstrAnimname = L"Cur Anim Tag : ";
	//wstrCurAnimTag.assign(CurAnimName.begin(), CurAnimName.end());
	//if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrAnimname + wstrCurAnimTag).c_str(), _float2(950.f, 60.f), _float2(0.6f, 0.6f))))
	//	return E_FAIL;

	//// 애니메이션 상태
	//wstring wstrCurKeyFrameIndex = to_wstring(m_pAnimation->Get_CurrentKeyFrameIndex());
	//wstring wstrKeyFrame = L"Key Frame : ";
	//if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrKeyFrame + wstrCurKeyFrameIndex).c_str(), _float2(950.f, 80.f), _float2(0.6f, 0.6f))))
	//	return E_FAIL;

	//wstring wstrAnimFinished = L"";
	//if (m_pAnimation->Is_Finished())
	//	wstrAnimFinished = L"AnimFinished : TRUE";
	//else
	//	wstrAnimFinished = L"AnimFinished : FALSE";
	//if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), wstrAnimFinished.c_str(), _float2(950.f, 100.f), _float2(0.6f, 0.6f))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CMonster_BronzeAnimus::Set_SpawnPosition(const _float3 vPoint)
{
	if (FAILED(CActor::Set_SpawnPosition(vPoint)))
		return E_FAIL;

	m_pTransform->Rotation_Axis(CTransform::STATE_UP, XMConvertToRadians(-90.f));

	return S_OK;
}

void CMonster_BronzeAnimus::OnTriggerEnter(CCollision& collision)
{
	m_pStateController->OnTriggerEnter(collision);
}

void CMonster_BronzeAnimus::Set_IsAttack(const _bool _isAttack)
{
	m_IsAttack = _isAttack;
	if (m_pWeapon)
		m_pWeapon->Set_IsAttack(_isAttack);
}

void CMonster_BronzeAnimus::Remove_Collider()
{
	m_pCharacterController->Remove_CCT();
}

void CMonster_BronzeAnimus::GroggyStart()
{
	m_bGroggy = true;
	m_fGroggyGauge = 0.f;
	m_pStateController->Change_State(L"Groggy");
	m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
}

void CMonster_BronzeAnimus::Hit(CCollision& collision)
{
	if (!m_bDead)
	{
		if (!m_bFirstHit)
		{
			m_bFirstHit = true; //딱 한번 true로 변경해줌
			m_pPanel->Set_BackUIGapY(1.f);
		}

		_vector MonsterPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector Pos = { XMVectorGetX(MonsterPos), XMVectorGetY(MonsterPos) + 3.f, XMVectorGetZ(MonsterPos), 1.f };
		Active_Effect((_uint)EFFECT::HIT, Pos);
		Active_Effect((_uint)EFFECT::HIT_FLOATING, Pos);
		Active_Effect((_uint)EFFECT::HIT_FLOATING_2, Pos);
		Active_Effect((_uint)EFFECT::HIT_IMAGE, Pos);

		//TODO::수치정해서바꿔줘야됨
		m_fGroggyGauge += 2.f;

		m_pPanel->Set_HpBar(Get_HpRatio());

		if (false == m_bGroggy)
		{
			//그로기 아닐때만 증가할수있게
			m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
			m_pStateController->Change_State(L"Hit");
		}
	}
}

CMonster_BronzeAnimus* CMonster_BronzeAnimus::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_BronzeAnimus* pInstance = new CMonster_BronzeAnimus(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMonster_BronzeAnimus Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_BronzeAnimus::Clone(const _uint _iSceneID, void* _pArg)
{
	CMonster_BronzeAnimus* pInstance = new CMonster_BronzeAnimus(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CMonster_BronzeAnimus Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_BronzeAnimus::Free()
{
	Safe_Release(m_pAnimator);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pStateController);
	Safe_Release(m_pCharacterController);
	Safe_Release(m_pPanel);

	__super::Free();
}
