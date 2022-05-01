#include "pch.h"
#include "Monster_Crawler.h"

#include "Light.h"

#include "UI_Monster_Panel.h"

#include "Effect_HitParticle.h"
#include "Effect_HitFloating.h"
#include "Effect_DeathParticle.h"

#include "Animation.h"
#include "Crawler_Idle.h"
#include "Crawler_Walk.h"
#include "Crawler_Attack.h"
#include "Crawler_Ricochet.h"
#include "Crawler_Death.h"
#include "Crawler_Flinch_Left.h"

#include "Stage1.h"
#include "Stage2.h"
#include "DamageFont.h"

CMonster_Crawler::CMonster_Crawler(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Crawler::CMonster_Crawler(const CMonster_Crawler& _rhs)
	:CActor(_rhs)
	, m_pStateController(_rhs.m_pStateController)
	, m_pCharacterController(_rhs.m_pCharacterController)
	, m_pPanel(_rhs.m_pPanel)
	, m_pCollider(_rhs.m_pCollider)
{
	Safe_AddRef(m_pStateController);
	Safe_AddRef(m_pCharacterController);
	Safe_AddRef(m_pPanel);
	Safe_AddRef(m_pCollider);
}


HRESULT CMonster_Crawler::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_Crawler::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))	return E_FAIL;
	if (_pArg)
	{
		_float3 vPoint = (*(_float3*)_pArg);
		if (FAILED(CActor::Set_SpawnPosition(vPoint)))
			return E_FAIL;
	}
	else
	{
		_vector Pos = { 0.f, 1.f, 3.f, 1.f };
		m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);
	}
	
	if (FAILED(SetUp_Components())) 
		return E_FAIL;
	if (FAILED(Set_Animation_FSM())) 
		return E_FAIL;
	if (FAILED(Set_State_FSM())) 
		return E_FAIL;
	if (FAILED(Ready_Weapone()))
		return E_FAIL;
	if (FAILED(Set_Panel()))
		return E_FAIL;

	m_bIsFall = true;

	m_iObectTag = (_uint)GAMEOBJECT::MONSTER_CRYSTAL;

	m_fMaxHp = 40.f;
	m_fCurrentHp = m_fMaxHp;

	m_fMaxGroggyGauge = 3.f;
	m_fGroggyGauge = 0.f;

	m_pPanel->Set_HpBar(Get_HpRatio());
	m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());

	m_iObectTag = (_uint)GAMEOBJECT::MONSTER_CRYSTAL;

	m_pCollider->Remove_ActorFromScene();
	setActive(false);

	m_pPanel->setActive(false);
	m_bUIShow = false;

	return S_OK;
}

_int CMonster_Crawler::Tick(_double _dDeltaTime)
{	
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}

	m_pTransform->Set_Velocity(XMVectorZero());
	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	if (m_bIsFall)
		m_pTransform->Fall(_dDeltaTime);

	_int iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (!m_bDead)
	{
		if (0 >= m_fCurrentHp)
		{
			m_bDead = true;
			m_pStateController->Change_State(L"Death");
			m_bLightCheck = true;
			m_pActiveLight->Set_Active(true);
			m_pCollider->Remove_ActorFromScene();

			return 0;
		}
	}
	else
	{
		Set_IsAttack(false);
		if (L"Death" == m_pStateController->Get_CurStateTag())
		{
			if (m_pAnimatorCom->Get_CurrentAnimation()->Is_Finished() && m_lifetime <= 0.f)
			{	
				m_pPanel->Set_UIRemove(true);
				Active_Effect((_uint)EFFECT::DEATH);
				m_bdissolve = true;
			}

			if (m_lifetime >= 1.f)
			{
				CLevel* pLevel = g_pGameInstance->getCurrentLevelScene();
				if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE1)
					static_cast<CStage1*>(pLevel)->Minus_MonsterCount();
				else if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE2)
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

	if (!m_bRemove)
	{
		m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());
		m_pCollider->Tick(_dDeltaTime);
	}

	if (true == m_bUIShow)
	{
		m_pPanel->setActive(true);
		m_pPanel->Set_Show(true);

		m_fUIShowTimeAcc += (_float)_dDeltaTime;
	}
	if(1.f <= m_fUIShowTimeAcc && m_bUIShow)
	{
		m_pPanel->Set_Show(false);
		m_bUIShow = false;
		m_fUIShowTimeAcc = 0.f;
	}

	cout << m_bUIShow << endl;

	CActor::LightOnOff(m_pTransform->Get_State(CTransform::STATE_POSITION), XMVectorSet(0.0f, 1.0f, 0.f,1.f), 4.f);

	return 0;
}

_int CMonster_Crawler::LateTick(_double _dDeltaTime)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	if (!m_bRemove)
	{
		m_pCharacterController->Update_OwnerTransform();
		_int iProgress = m_pStateController->LateTick(_dDeltaTime);
		if (NO_EVENT != iProgress)
		{
			return iProgress;
		}
	}

	return 0;
}

HRESULT CMonster_Crawler::Render()
{
	if (m_bdissolve == true)
		CActor::DissolveOn(0.7f);

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_bdissolve", &m_bdissolve, sizeof(_bool)))) MSGBOX("Failed to Apply dissolvetime");

	SCB desc;
	ZeroMemory(&desc, sizeof(SCB));
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();

	CActor::BindConstantBuffer(wstrCamTag,&desc);

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CMonster_Crawler::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CActor::BindConstantBuffer(wstrCamTag);
	CActor::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 3);

	return S_OK;
}

void CMonster_Crawler::OnTriggerEnter(CCollision& collision)
{
	if (!m_bDead)
	{
		// 플레이어를 공격할 때
		if ((_uint)GAMEOBJECT::PLAYER == collision.pGameObject->getTag())
		{
			if (!m_IsAttack)
				return;

			m_tAttackDesc.fDamage = 3;
			m_tAttackDesc.iLevel = 1;
			static_cast<CActor*>(collision.pGameObject)->Hit(m_tAttackDesc);
			return;
		}
	}
}

void CMonster_Crawler::OnTriggerExit(CCollision& collision)
{
}

void CMonster_Crawler::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_bDead || 0.f >= m_fCurrentHp)
		return;

	//m_pPanel->Set_Show(true);

	if (false == m_bFirstHit)
	{
		m_bFirstHit = true; //딱 한번 true로 변경해줌
		m_pPanel->Set_BackUIGapY(1.f);
	}

	g_pGameInstance->Play_Shot(L"Monster_Hit", CSoundMgr::CHANNELID::Monster_Hit);
	g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Monster_Hit, 0.5f);
	m_fCurrentHp -= _tAttackDesc.fDamage;
	m_fGroggyGauge += 2; //TODO::수치정해서바꿔줘야됨

	m_pPanel->Set_HpBar(Get_HpRatio());

	if (false == m_bGroggy)
	{
		//그로기 아닐때만 증가할수있게
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
		m_pStateController->Change_State(L"Flinch_Left");
	}

	Active_Effect((_uint)EFFECT::HIT);
	Active_Effect((_uint)EFFECT::HIT_FLOATING);
	Active_Effect((_uint)EFFECT::HIT_FLOATING_2);
	Active_Effect((_uint)EFFECT::HIT_IMAGE);
	//Active_Effect((_uint)EFFECT::DEAD_SMOKE);

	CTransform* pOtherTransform = _tAttackDesc.pOwner->Get_Transform();
	_vector svOtherLook = XMVector3Normalize(pOtherTransform->Get_State(CTransform::STATE_LOOK));
	_vector svOtherRight = XMVector3Normalize(pOtherTransform->Get_State(CTransform::STATE_RIGHT));

	uniform_real_distribution<_float> fRange(-0.4f, 0.4f);
	uniform_real_distribution<_float> fRange2(-0.2f, 0.2f);
	uniform_int_distribution<_int> iRange(-5, 5);
	CDamageFont::DESC tDamageDesc;
	_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 1.5f + fRange2(g_random), 0.f, 0.f };
	svPos += svOtherRight * fRange(g_random) - svOtherLook * 0.5f;
	XMStoreFloat3(&tDamageDesc.vPos, svPos);
	tDamageDesc.fDamage = _tAttackDesc.fDamage + (_float)iRange(g_random);
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_DamageFont", L"Proto_GameObject_DamageFont", &tDamageDesc)))
		MSGBOX(L"데미지 폰트 생성 실패");
}

void CMonster_Crawler::Parry(const PARRYDESC& _tParryDesc)
{
	// 그로기 처리를 해야한다.
	m_fGroggyGauge += (m_fMaxGroggyGauge - m_fGroggyGauge);
}

HRESULT CMonster_Crawler::Set_SpawnPosition(_fvector vPos)
{
	CActor::Set_SpawnPosition(vPos);
	_float3 tmpPos;
	XMStoreFloat3(&tmpPos, vPos);
	m_pCharacterController->setFootPosition(tmpPos);

	return S_OK;
}

void CMonster_Crawler::Set_IsAttack(const _bool _isAttack)
{
	m_IsAttack = _isAttack;
	m_tAttackDesc.fDamage = 3;
	m_tAttackDesc.iLevel = 1;
}

void CMonster_Crawler::setActive(_bool bActive)
{
	CGameObject::setActive(bActive);

	if (bActive)
	{
		if (!m_pCharacterController)
		{
			//Ch_controller
			CCharacterController::DESC tCCTDesc;
			tCCTDesc.fHeight = 0.6f;
			tCCTDesc.fRadius = 0.8f;
			tCCTDesc.fContactOffset = tCCTDesc.fRadius * 0.1f;
			tCCTDesc.fStaticFriction = 0.5f;
			tCCTDesc.fDynamicFriction = 0.5f;
			tCCTDesc.fRestitution = 0.f;
			tCCTDesc.pGameObject = this;
			tCCTDesc.vPosition = { 0.f, 0.f, 0.f };

			if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCCTDesc)))
				MSGBOX(L"땅강아지 cct 생성 실패");
			m_pCharacterController->setOwnerTransform(m_pTransform);
			m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);
		}
		if (m_pCollider)
			m_pCollider->Add_ActorToScene();
		if (m_pPanel)
			m_pPanel->setActive(true);
	}
}

HRESULT CMonster_Crawler::SetUp_Components()
{
	//Model
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);
	
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Crawler", L"Com_Model", (CComponent**)&m_pModel)))
	{
		return E_FAIL;
	}

	//animator
	_matrix matPivot = XMMatrixIdentity();
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModel;
	tDesc.pTransform = m_pTransform;
	tDesc.eType = CAnimationController::EType::CharacterController;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimatorCom, &tDesc)))
		return E_FAIL;

	////Ch_controller
	//CCharacterController::DESC tCCTDesc;
	//tCCTDesc.fHeight = 1.f;
	//tCCTDesc.fRadius = 0.5f;
	//tCCTDesc.fContactOffset = tCCTDesc.fRadius * 0.1f;
	//tCCTDesc.fStaticFriction = 0.5f;
	//tCCTDesc.fDynamicFriction = 0.5f;
	//tCCTDesc.fRestitution = 0.f;
	//tCCTDesc.pGameObject = this;
	//tCCTDesc.vPosition = { 0.f, 0.f, 0.f };

	//if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCCTDesc)))
	//	return E_FAIL;
	//m_pCharacterController->setOwnerTransform(m_pTransform);
	//m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);

	//state controller
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"Com_StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	return S_OK;
}

HRESULT CMonster_Crawler::Ready_Weapone()
{
	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.pGameObject = this;

	CSphereCollider::DESC tSphereCol;
	tSphereCol.tColliderDesc = tColliderDesc;
	tSphereCol.fRadius = 0.7f;
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SphereCollider", L"Collider", (CComponent**)&m_pCollider, &tSphereCol)))
		return E_FAIL;

	_matrix smatPviot = XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.5f, 0.5f);
	m_pCollider->setPivotMatrix(smatPviot);
	m_pCollider->setShapeLayer((_uint)ELayer::MonsterWeapon);

	return S_OK;
}

HRESULT CMonster_Crawler::Set_Animation_FSM()
{
	CAnimation* pAnim = m_pModel->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Idle_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(IDLE, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Walk_Fwd_Stop_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_FWD, HEAD, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Attack_R1_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(ATTACK_R1, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Death_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(DEATH, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Ricochet_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(RICOCHET, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Flinch_Right_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(FLINCH_RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Flinch_Left_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(FLINCH_LEFT, HEAD, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;

	//넉백
	pAnim = m_pModel->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Knockback_Start_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(KNOCKBACK_START, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Knockback_End_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(KNOCKBACK_END, KNOCKBACK_START, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	//if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, KNOCKBACK_END, false)))	return E_FAIL;

	//m_pAnimatorCom->Set_UpAutoChangeAnimation(ATTACK_R1, IDLE);
	//if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, ATTACK_R1, false)))	return E_FAIL;

	m_pAnimatorCom->Insert_AnyEntryAnimation(IDLE);
	m_pAnimatorCom->Insert_AnyEntryAnimation(WALK_FWD);
	m_pAnimatorCom->Insert_AnyEntryAnimation(ATTACK_R1);
	m_pAnimatorCom->Insert_AnyEntryAnimation(DEATH);
	m_pAnimatorCom->Insert_AnyEntryAnimation(FLINCH_RIGHT);
	m_pAnimatorCom->Insert_AnyEntryAnimation(FLINCH_LEFT);
	m_pAnimatorCom->Insert_AnyEntryAnimation(KNOCKBACK_START);
	m_pAnimatorCom->Insert_AnyEntryAnimation(RICOCHET);

	m_pAnimatorCom->Change_Animation(IDLE);

	_uint iRand = rand() % 15;
	m_pAnimatorCom->Add_AnimFrame(IDLE, iRand);

	return S_OK;
}

HRESULT CMonster_Crawler::Set_State_FSM()
{
	if (FAILED(m_pStateController->Add_State(L"Idle", CCrawler_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Walk", CCrawler_Walk::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Attack", CCrawler_Attack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Ricochet", CCrawler_Ricochet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Death", CCrawler_Death::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pStateController->Add_State(L"Flinch_Left", CCrawler_Flinch_Left::Create(m_pDevice, m_pDeviceContext))))
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

HRESULT CMonster_Crawler::Set_Panel()
{
	//MonsterBar Panel
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::CRAWLER;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;

	Safe_AddRef(m_pPanel);
	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	return S_OK;
}


void CMonster_Crawler::Set_Remove(_bool bCheck)
{
	m_bRemove = bCheck;
	m_pPanel->Set_UIRemove(m_bRemove);
}

CMonster_Crawler* CMonster_Crawler::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_Crawler* pInstance = new CMonster_Crawler(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMonster_Crawler");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_Crawler::Clone(const _uint _iSceneID, void* _pArg)
{
	CMonster_Crawler* pInstance = new CMonster_Crawler(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CMonster_Crawler");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Crawler::Free()
{
	Safe_Release(m_pCollider);
	Safe_Release(m_pCharacterController);
	Safe_Release(m_pPanel);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimatorCom);

	__super::Free();
}
