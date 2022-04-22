#include "pch.h"
#include "Monster_Bastion_Healer.h"
#include "Animation.h"
#include "Staff.h"
#include "UI_Monster_Panel.h"
#include "HierarchyNode.h"

/* for. FSM */
#include "Bastion_Healer_State.h"
#include "Bastion_Healer_Idle.h"
#include "Bastion_Healer_Hit.h"
#include "Bastion_Healer_Death.h"
#include "Bastion_Healer_Run.h"
#include "Bastion_Healer_Attack.h"
#include "Bastion_Healer_Groggy.h"
#include "Bastion_Healer_Groggy_End.h"
#include "Bastion_Healer_CastProtect.h"

#include "Stage1.h"
#include "Stage2.h"

CMonster_Bastion_Healer::CMonster_Bastion_Healer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
	, m_pCharacterController(nullptr)
	, m_pStateController(nullptr)
	, m_pAnimator(nullptr)
{
}

CMonster_Bastion_Healer::CMonster_Bastion_Healer(const CMonster_Bastion_Healer& _rhs)
	: CActor(_rhs)
	, m_pCharacterController(_rhs.m_pCharacterController)
	, m_pStateController(_rhs.m_pStateController)
	, m_pAnimator(_rhs.m_pAnimator)
{
	Safe_AddRef(m_pCharacterController);
	Safe_AddRef(m_pStateController);
	Safe_AddRef(m_pAnimator);
}

HRESULT CMonster_Bastion_Healer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::MONSTER_HEALER;

	return S_OK;
}

HRESULT CMonster_Bastion_Healer::NativeConstruct(const _uint _iSceneID, void* _pArg)
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

	// MonsterBar Panel
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::HEALER;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;

	Safe_AddRef(m_pPanel);

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	m_fMaxHp = 5.f;
	m_fCurrentHp = m_fMaxHp;

	m_fMaxGroggyGauge = 10.f;
	m_fGroggyGauge = 0.f;

	m_pPanel->Set_HpBar(Get_HpRatio());
	m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());

	m_isFall = true;

	m_pWeapon->setActive(false);
	m_pPanel->setActive(false);
	setActive(false);
	return S_OK;
}

_int CMonster_Bastion_Healer::Tick(_double _dDeltaTime)
{
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
	if (!m_bDead)
		m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());
	else
	{
		if (L"Death" == m_pStateController->Get_CurStateTag())
		{
			if (m_pAnimator->Get_CurrentAnimation()->Is_Finished() && m_lifetime <= 0.f)
			{
				//Set_Remove(true);
				m_bdissolve = true;
				m_pPanel->Set_UIRemove(true);
			}

			if (m_lifetime >= 1.f)
				Set_Remove(true);

			if (1 == m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex())
			{
				Active_Effect((_uint)EFFECT::DEATH);
			}
		}
		else
		{
			Set_Remove(true);
			m_pPanel->Set_UIRemove(true);
		}
	}

	m_pWeapon->Tick(_dDeltaTime);
	if (true == m_bUIShow)
		m_pPanel->Set_Show(true);
	else
		m_pPanel->Set_Show(false);

	if (m_fGroggyGauge >= m_fMaxGroggyGauge)
	{
		//���ϻ����϶� ����state���� ���� �׷α� ��� 0���� ����������
		m_bGroggy = true;
		m_fGroggyGauge = 0.f;
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

	return _int();
}

_int CMonster_Bastion_Healer::LateTick(_double _dDeltaTime)
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

HRESULT CMonster_Bastion_Healer::Render()
{
	if (m_bdissolve == true)
		CActor::DissolveOn(0.5f);

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_bdissolve", &m_bdissolve, sizeof(_bool)))) MSGBOX("Failed to Apply dissolvetime");

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		SCB desc;
		ZeroMemory(&desc, sizeof(SCB));

		switch (i)
		{
		case 2:
			CActor::BindConstantBuffer(wstrCamTag, &desc);
			if (FAILED(m_pModel->Render(i, 2))) MSGBOX("Failed To Rendering Healer");
			break;
		default:
			CActor::BindConstantBuffer(wstrCamTag, &desc);
			if (FAILED(m_pModel->Render(i, 0))) MSGBOX("Failed To Rendering Healer");
			break;
		}
	}


#ifdef _DEBUG
	//Render_Debug();
#endif
	return S_OK;
}

HRESULT CMonster_Bastion_Healer::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CActor::BindConstantBuffer(wstrCamTag);
	CActor::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 3);

	return S_OK;
}

void CMonster_Bastion_Healer::Groggy_Start()
{
	Set_Groggy(true);
	Set_GroggyGauge(0);
	m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
}

void CMonster_Bastion_Healer::Hit(CCollision& pCol)
{
	if (!m_bDead)
	{
		if (true == g_pObserver->IsAttack()) //�÷��̾�����϶�
		{
			if (false == m_bFirstHit)
			{
				m_bFirstHit = true; //�� �ѹ� true�� ��������
				m_pPanel->Set_BackUIGapY(1.f);
			}

			if ((_uint)GAMEOBJECT::WEAPON == pCol.pGameObject->getTag())
			{
				m_pPanel->Set_Show(true);
				Active_Effect((_uint)EFFECT::HIT);
				Active_Effect((_uint)EFFECT::HIT_FLOATING);
				Active_Effect((_uint)EFFECT::HIT_FLOATING_2);
				Active_Effect((_uint)EFFECT::HIT_IMAGE);

				//m_fCurrentHp -= 5.f;
				//m_bGroggy = 2; //TODO::��ġ���ؼ��ٲ���ߵ�

				m_pPanel->Set_HpBar(Get_HpRatio());

				if (false == m_bGroggy)
				{
					//�׷α� �ƴҶ��� �����Ҽ��ְ�
					m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
					m_pStateController->Change_State(L"Hit");
				}
			}
			else
				m_pStateController->Change_State(L"Idle");
		}
	}
}

void CMonster_Bastion_Healer::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_bDead || 0.f >= m_fCurrentHp)
		return;

	m_pPanel->Set_Show(true);

	m_fCurrentHp -= _tAttackDesc.fDamage;
	CCollision collision;
	collision.pGameObject = _tAttackDesc.pHitObject;

	Hit(collision);
}

void CMonster_Bastion_Healer::Parry(const PARRYDESC& _tParryDesc)
{
	m_fGroggyGauge += (m_fMaxGroggyGauge - m_fGroggyGauge);
}

void CMonster_Bastion_Healer::Remove_Collider()
{
	m_pCharacterController->Remove_CCT();
}

HRESULT CMonster_Bastion_Healer::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 2.0f;
	transformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(transformDesc);
	//������ ��
	//_float4 vPosition = { 3.f, 2.f, 3.f, 1.f };

	//m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));


	// ��
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Bastion_Healer", L"Model", (CComponent**)&m_pModel)))
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

	// ������Ʈ ��Ʈ�ѷ�
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

HRESULT CMonster_Bastion_Healer::Ready_Weapon(void)
{
	m_pWeapon = static_cast<CStaff*>(g_pGameInstance->Clone_GameObject(m_iSceneID, L"Proto_GameObject_Weapon_Staff"));

	if (!m_pWeapon)
		return E_FAIL;

	m_pWeapon->Set_Owner(this);

	vector<CHierarchyNode*> vecNode = m_pModel->Get_HierachyNodes();
	CHierarchyNode* pWeaponBone = nullptr;
	for (auto& pNode : vecNode)
	{
		if (!strcmp(pNode->Get_Name(), "weapon_1_end"))
		{
			pWeaponBone = pNode;
			break;
		}
	}
	m_pWeapon->Set_FixedBone(pWeaponBone);
	m_pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	return S_OK;
}

HRESULT CMonster_Bastion_Healer::Ready_AnimFSM(void)
{
#pragma region Default
	CAnimation* pAnimation = m_pModel->Get_Animation("A_Idle");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, TRUE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Death");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_DEATH, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion
#pragma region Attack
	pAnimation = m_pModel->Get_Animation("A_Cast_Protect");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_CAST_PROTECT, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
	pAnimation = m_pModel->Get_Animation("A_Attack_Blind");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ATTACK_BLIND, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion
#pragma region Hit
	pAnimation = m_pModel->Get_Animation("A_Flinch_Left");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_FLINCH_LEFT, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ)))
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
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_DEATH);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_STUN_ST);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_STUN_ED);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_CAST_PROTECT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ATTACK_BLIND);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_FLINCH_LEFT);
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
#pragma endregion

#pragma region  Auto Change Anim
	/* ##1.������ �ִ� ##2.��Ʈ�� �ִ�  */
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ST, (_uint)ANIM_TYPE::A_WALK_FWD);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD, (_uint)ANIM_TYPE::A_WALK_FWD_ED);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ED, (_uint)ANIM_TYPE::A_IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_BWD_ST, (_uint)ANIM_TYPE::A_WALK_BWD);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_BWD, (_uint)ANIM_TYPE::A_WALK_BWD_ED);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_BWD_ED, (_uint)ANIM_TYPE::A_IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_STUN_ST, (_uint)ANIM_TYPE::A_STUN);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_STUN, (_uint)ANIM_TYPE::A_STUN_ED);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_STUN_ED, (_uint)ANIM_TYPE::A_IDLE);
#pragma endregion

#pragma region Anim to Anim Connect
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_WALK_FWD_ED, FALSE)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_WALK_BWD_ED, FALSE)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_WALK_FWD_ED, (_uint)ANIM_TYPE::A_WALK_FWD, TRUE)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_WALK_FWD_ED, (_uint)ANIM_TYPE::A_WALK_FWD_ST, TRUE)))
		return E_FAIL;
#pragma endregion
	return S_OK;
}

HRESULT CMonster_Bastion_Healer::Ready_StateFSM(void)
{
	/* for. Idle */
	if (FAILED(m_pStateController->Add_State(L"Idle", CBastion_Healer_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Player Chaser */
	if (FAILED(m_pStateController->Add_State(L"Run", CBastion_Healer_Run::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Attack*/
	if (FAILED(m_pStateController->Add_State(L"Attack", CBastion_Healer_Attack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Death */
	if (FAILED(m_pStateController->Add_State(L"Death", CBastion_Healer_Death::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Hit */
	if (FAILED(m_pStateController->Add_State(L"Hit", CBastion_Healer_Hit::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Groggy */
	if (FAILED(m_pStateController->Add_State(L"Groggy", CBastion_Healer_Groggy::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Groggy End */
	if (FAILED(m_pStateController->Add_State(L"Groggy_End", CBastion_Healer_Groggy_End::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Cast Protect */
	if (FAILED(m_pStateController->Add_State(L"Cast_Protect", CBastion_Healer_CastProtect::Create(m_pDevice, m_pDeviceContext))))
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

HRESULT CMonster_Bastion_Healer::Render_Debug(void)
{
	if (FAILED(m_pStateController->Render()))
		return E_FAIL;

	//Hp
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), L"HP : " + to_wstring(m_iHp), _float2(950.f, 20.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	// FSM
	wstring wstrCurStateTag = m_pStateController->Get_CurStateTag();
	wstring wstrState = L"Cur State : ";

	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrState + wstrCurStateTag).c_str(), _float2(950.f, 40.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	m_pAnimation = m_pAnimator->Get_CurrentAnimation();

	// �ִϸ��̼� �̸�
	string CurAnimName = m_pAnimation->Get_Name();
	wstring wstrCurAnimTag;
	wstring wstrAnimname = L"Cur Anim Tag : ";
	wstrCurAnimTag.assign(CurAnimName.begin(), CurAnimName.end());
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrAnimname + wstrCurAnimTag).c_str(), _float2(950.f, 60.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	// �ִϸ��̼� ����
	wstring wstrCurKeyFrameIndex = to_wstring(m_pAnimation->Get_CurrentKeyFrameIndex());
	wstring wstrKeyFrame = L"Key Frame : ";
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrKeyFrame + wstrCurKeyFrameIndex).c_str(), _float2(950.f, 80.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	wstring wstrAnimFinished = L"";
	if (m_pAnimation->Is_Finished())
		wstrAnimFinished = L"AnimFinished : TRUE";
	else
		wstrAnimFinished = L"AnimFinished : FALSE";
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), wstrAnimFinished.c_str(), _float2(950.f, 100.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	return S_OK;
}

void CMonster_Bastion_Healer::OnTriggerEnter(CCollision& collision)
{
	m_pStateController->OnTriggerEnter(collision);
}

void CMonster_Bastion_Healer::OnTriggerExit(CCollision& collision)
{
	m_pStateController->OnTriggerExit(collision);
}

void CMonster_Bastion_Healer::Set_IsAttack(const _bool _isAttack)
{
	m_IsAttack = _isAttack;
	if (m_pWeapon)
		m_pWeapon->Set_IsAttack(_isAttack);
}

void CMonster_Bastion_Healer::Set_Remove(_bool bCheck)
{
	m_bRemove = bCheck;
	m_pPanel->Set_UIRemove(bCheck);
}

void CMonster_Bastion_Healer::setActive(_bool bActive)
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
				MSGBOX(L"���� cct ���� ����");
			m_pCharacterController->setOwnerTransform(m_pTransform);
			m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);
		}
		if (m_pWeapon)
			m_pWeapon->setActive(true);
		if (m_pPanel)
			m_pPanel->setActive(true);
	}
}

CMonster_Bastion_Healer* CMonster_Bastion_Healer::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_Bastion_Healer* pInstance = new CMonster_Bastion_Healer(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMonster_Bastion_Healer Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_Bastion_Healer::Clone(const _uint _iSceneID, void* _pArg)
{
	CMonster_Bastion_Healer* pInstance = new CMonster_Bastion_Healer(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CMonster_Bastion_Healer Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Bastion_Healer::Free()
{
	Safe_Release(m_pAnimator);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pStateController);
	Safe_Release(m_pCharacterController);
	Safe_Release(m_pPanel);

	__super::Free();
}
