#include "pch.h"
#include "Monster_Bastion_Shooter.h"
#include "Animation.h"
#include "Animator.h"
#include "CharacterController.h"

#include "Shooter_Idle.h"
#include "Shooter_Attack.h"
#include "Shooter_Chase.h"
#include "Shooter_Hit.h"
#include "Shooter_Death.h"
#include "Shooter_Walk.h"
#include "Shooter_Groggy.h"
#include "Shooter_Excution.h"

#include"UI_Monster_Panel.h"

#include "Stage1.h"
#include "Stage2.h"
#include "Light.h"
#include "DamageFont.h"

CMonster_Bastion_Shooter::CMonster_Bastion_Shooter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
	, m_pCharacterController(nullptr)
	, m_pStateController(nullptr)
	, m_pAnimator(nullptr)
	, m_bShot(false)

{
}

CMonster_Bastion_Shooter::CMonster_Bastion_Shooter(const CMonster_Bastion_Shooter& _rhs)
	: CActor(_rhs)
	, m_pCharacterController(_rhs.m_pCharacterController)
	, m_pStateController(_rhs.m_pStateController)
	, m_pAnimator(_rhs.m_pAnimator)
	, m_pPanel(_rhs.m_pPanel)
	, m_bShot(_rhs.m_bShot)
{
	Safe_AddRef(m_pCharacterController);
	Safe_AddRef(m_pStateController);
	Safe_AddRef(m_pAnimator);
	Safe_AddRef(m_pPanel);
}

HRESULT CMonster_Bastion_Shooter::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::MONSTER_SHOOTER;

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	m_iCurScene = _iSceneID;
	if (_pArg)
	{
		_float3 vPoint = (*(_float3*)_pArg);
		if (FAILED(CActor::Set_SpawnPosition(vPoint)))
			return E_FAIL;
	}
	else
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -10.f, 0.f, 1.f));
	}
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_AnimationFSM()))
		return E_FAIL;

	if (FAILED(Ready_StateFSM()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	m_rimtime = 1.f;
	m_rimtimer = 1.f;

	m_wstrCurState = L"";
	m_tAttackDesc.iLevel = 2;

	m_pPanel->setActive(false);
	setActive(false);
	return S_OK;
}

_int CMonster_Bastion_Shooter::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pTransform->Set_Velocity(XMVectorZero());

	m_pTransform->Fall(_dDeltaTime);

	iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (!m_bDead)
	{
		if (m_fCurrentHp <= 0.f)
		{
			m_bDead = true;
			m_pStateController->Change_State(L"Death");
			m_pCharacterController->Remove_CCT();
			m_bLightCheck = true;
			m_pActiveLight->Set_Active(true);
		}
		else
			m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());
	}
	Change_State();

	if (true == m_bUIShow)
	{
		m_pPanel->setActive(true);
		m_pPanel->Set_Show(true);

		m_fUIShowTimeAcc += (_float)_dDeltaTime;
	}
	if (1.f <= m_fUIShowTimeAcc && m_bUIShow)
	{
		m_pPanel->Set_Show(false);
		m_bUIShow = false;
		m_fUIShowTimeAcc = 0.f;
	}

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	CActor::LightOnOff(m_pTransform->Get_State(CTransform::STATE_POSITION), XMVectorSet(0.f, 1.f, 0.f, 1.f), 10.f);

	return _int();
}

_int CMonster_Bastion_Shooter::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	if (!m_bDead)
		m_pCharacterController->Update_OwnerTransform();

	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;

	return _int();
}

HRESULT CMonster_Bastion_Shooter::Render()
{
	if (m_bdissolve == true)
		CActor::DissolveOn(0.7f);

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_bdissolve", &m_bdissolve, sizeof(_bool)))) MSGBOX("Failed to Apply dissolvetime");

	SCB desc;
	ZeroMemory(&desc, sizeof(SCB));

	RIM RimDesc;
	ZeroMemory(&RimDesc, sizeof(RIM));

	if (m_isNoDamage)
	{
		RimDesc.rimcheck = true;
		m_rimcheck = true;
		_float time = 1.f;
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimtimer", &time, sizeof(_float)))) MSGBOX("Failed to Apply RimTime Value");
	}
	else if (m_isNoDamage == false && m_rimcheck == true)
	{
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimtimer", &m_rimtime, sizeof(_float)))) MSGBOX("Failed to Apply RimTime Value");
		CActor::RimIntensity(g_fDeltaTime * -0.5f);
		RimDesc.rimcheck = true;
	}
	else if (m_rimcheck == false)
	{
		RimDesc.rimcheck = false;
	}

	RimDesc.rimcol = _float3(0.f, 0.5f, 0.5f);
	RimDesc.rimintensity = 30.f;
	XMStoreFloat4(&RimDesc.camdir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_POSITION) - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane")));


	CActor::BindConstantBuffer(L"Camera_Silvermane", &desc, &RimDesc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		switch (i)
		{
		case 2:
			if (FAILED(m_pModel->Render(i, 1))) MSGBOX("Failed To Rendering Shooter");
			break;
		default:
			if (FAILED(m_pModel->Render(i, 0))) MSGBOX("Failed To Rendering Shooter");
			break;
		}
	}

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Render_Shadow()
{
	CActor::BindConstantBuffer(L"Camera_Silvermane");
	CActor::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 3);

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Set_SpawnPosition(_fvector vPos)
{
	CActor::Set_SpawnPosition(vPos);
	_float3 tmpPos;
	XMStoreFloat3(&tmpPos, vPos);
	m_pCharacterController->setFootPosition(tmpPos);
	return S_OK;
}

void CMonster_Bastion_Shooter::setActive(_bool bActive)
{
	CGameObject::setActive(bActive);

	if (bActive)
	{
		if (!m_pCharacterController)
		{
			CCharacterController::DESC tController;
			tController.fHeight = 1.f;
			tController.fRadius = 1.f;
			tController.fContactOffset = tController.fRadius * 0.1f;
			tController.fStaticFriction = 0.5f;
			tController.fDynamicFriction = 0.5f;
			tController.fRestitution = 0.f;

			tController.vPosition = _float3(0.f, 0.f, 0.f);
			tController.pGameObject = this;

			if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC,
				L"Proto_Component_CharacterController", L"Com_CharacterController", (CComponent**)&m_pCharacterController, &tController)))
				MSGBOX(L"???? cct ???? ????");
			m_pCharacterController->setOwnerTransform(m_pTransform);
			m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);
		}
		//if (m_pPanel)
		//	m_pPanel->setActive(true);
	}
}

void CMonster_Bastion_Shooter::Set_Remove(_bool bCheck)
{
	m_bRemove = bCheck;
	m_pPanel->Set_UIRemove(bCheck);
}

void CMonster_Bastion_Shooter::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_isNoDamage)
		return;

	if (m_bDead || 0.f >= m_fCurrentHp )
		return;

	m_pPanel->Set_Show(true);

	m_fCurrentHp -= _tAttackDesc.fDamage;

	Active_Effect((_uint)EFFECT::HIT);
	Active_Effect((_uint)EFFECT::HIT_FLOATING);
	Active_Effect((_uint)EFFECT::HIT_FLOATING_2);
	Active_Effect((_uint)EFFECT::HIT_IMAGE);

	CTransform* pOtherTransform = _tAttackDesc.pOwner->Get_Transform();
	_vector svOtherLook = XMVector3Normalize(pOtherTransform->Get_State(CTransform::STATE_LOOK));
	_vector svOtherRight = XMVector3Normalize(pOtherTransform->Get_State(CTransform::STATE_RIGHT));

	uniform_real_distribution<_float> fRange(-0.4f, 0.4f);
	uniform_real_distribution<_float> fRange2(-0.2f, 0.2f);
	uniform_int_distribution<_int> iRange(-5, 5);
	CDamageFont::DESC tDamageDesc;
	_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 2.f + fRange2(g_random), 0.f, 0.f };
	svPos += svOtherRight * fRange(g_random) - svOtherLook * 0.5f;
	XMStoreFloat3(&tDamageDesc.vPos, svPos);
	tDamageDesc.fDamage = _tAttackDesc.fDamage + (_float)iRange(g_random);
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_DamageFont", L"Proto_GameObject_DamageFont", &tDamageDesc)))
		MSGBOX(L"?????? ???? ???? ????");

	Hit();
}

void CMonster_Bastion_Shooter::Parry(const PARRYDESC& _tParrykDesc)
{
	m_fGroggyGauge += (m_fMaxGroggyGauge - m_fGroggyGauge);
}

void CMonster_Bastion_Shooter::Execution()
{
	CLevel* pLevel = g_pGameInstance->getCurrentLevelScene();
	if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE1)
		static_cast<CStage1*>(pLevel)->Minus_MonsterCount();

	else if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE2)
		static_cast<CStage2*>(pLevel)->Minus_MonsterCount();

	m_bDead = true;
	m_pStateController->Change_State(L"Excution");
	m_pCharacterController->Remove_CCT();
}

void CMonster_Bastion_Shooter::OnTriggerEnter(CCollision& collision)
{
	if (!m_bDead)
	{
		if (m_fCurrentHp >= 0.f)
		{
			m_pPanel->Set_Show(true);

			if (collision.pGameObject->getTag() == (_uint)GAMEOBJECT::WEAPON)
			{
				CWeapon* pWeapon = static_cast<CWeapon*>(collision.pGameObject);
				if (pWeapon->IsAttack())
					Hit();
			}
		}
	}
}

void CMonster_Bastion_Shooter::Chase()
{
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPlayerPos = g_pObserver->Get_PlayerPos();

	if (XMVector3Equal(vPlayerPos, XMVectorZero()))
		return;

	_float fDist = XMVectorGetX(XMVector3Length(vPos - vPlayerPos));

	if (fDist < 20.f)
	{
		m_wstrCurState = L"Chase";
		m_pStateController->Change_State(L"Chase");
	}
}

void CMonster_Bastion_Shooter::Hit()
{
	m_eHitType = ANIM_TYPE::HIT1;
	CShooter_Hit::HITDATA tData;
	ZeroMemory(&tData, sizeof(tData));

	//m_fCurrentHp--;
	m_pPanel->Set_HpBar(Get_HpRatio());
	if (!m_bFirstHit)
	{
		m_bFirstHit = true; //?? ???? true?? ????????
		m_pPanel->Set_BackUIGapY(1.f);
	}
	if (m_wstrCurState != L"Hit" && !m_bGroggy)
	{
		g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Shooter_Hit);
		g_pGameInstance->Play_Shot(L"Monster_Hit_6", CSoundMgr::CHANNELID::Shooter_Hit);
		tData.fCurHp = m_fCurrentHp;
		tData.iHitType = (_uint)m_eHitType;
		m_wstrCurState = L"Hit";
		m_pStateController->Change_State(L"Hit", &tData);
	}

	if (!m_bGroggy)
	{
		//?????? ???????? ????????????
		m_fGroggyGauge += 2; //TODO::????????????????????
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}

	Active_Effect((_uint)EFFECT::HIT);
	Active_Effect((_uint)EFFECT::HIT_FLOATING);
	Active_Effect((_uint)EFFECT::HIT_FLOATING_2);
	Active_Effect((_uint)EFFECT::HIT_IMAGE);
}

HRESULT CMonster_Bastion_Shooter::Ready_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Bastion_Shooter", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModel;
	tDesc.pTransform = m_pTransform;
	tDesc.eType = CAnimationController::EType::CharacterController;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimator, &tDesc)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;

	m_pStateController->Set_GameObject(this);

	//CCharacterController::DESC tController;
	//tController.fHeight = 1.f;
	//tController.fRadius = 1.f;
	//tController.fContactOffset = tController.fRadius * 0.1f;
	//tController.fStaticFriction = 0.5f;
	//tController.fDynamicFriction = 0.5f;
	//tController.fRestitution = 0.f;

	//tController.vPosition = _float3(0.f, 0.f, 0.f);
	//tController.pGameObject = this;

	//if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC,
	//	L"Proto_Component_CharacterController", L"Com_CharacterController", (CComponent**)&m_pCharacterController, &tController)))
	//	return E_FAIL;
	//m_pCharacterController->setOwnerTransform(m_pTransform);
	//m_pCharacterController->setShapeLayer((_uint)ELayer::Monster);

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Ready_AnimationFSM()
{
	CAnimation* pAnim = m_pModel->Get_Animation("Idle");

	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("RunStart");;
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("RunLoop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_START, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("RunEnd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::RUN_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("Attack");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("Hit1");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::HIT1, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("Death");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::DEATH, (_uint)ANIM_TYPE::HIT1, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModel->Get_Animation("StunStart");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::GROGGY_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("StunLoop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::GROGGY_LOOP, (_uint)ANIM_TYPE::GROGGY_START, pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModel->Get_Animation("StunEnd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::GROGGY_END, (_uint)ANIM_TYPE::GROGGY_LOOP, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;


	pAnim = m_pModel->Get_Animation("Excution");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::EXCUTION, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::RUN_END, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::ATTACK, false)))
		return E_FAIL;

	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HIT1, false)))
		return E_FAIL;

	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::GROGGY_END, false)))
		return E_FAIL;

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::RUN_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::GROGGY_START, (_uint)ANIM_TYPE::GROGGY_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::GROGGY_LOOP, (_uint)ANIM_TYPE::GROGGY_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::GROGGY_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK, (_uint)ANIM_TYPE::IDLE);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::HIT1, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::GROGGY_START, (_uint)ANIM_TYPE::GROGGY_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::GROGGY_LOOP, (_uint)ANIM_TYPE::GROGGY_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::GROGGY_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::IDLE);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::RUN_START);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::ATTACK);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::HIT1);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::DEATH);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::GROGGY_START);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::EXCUTION);

	m_pAnimator->Change_Animation((_uint)ANIM_TYPE::IDLE);
	
	_uint iRand = rand() % 15;
	m_pAnimator->Add_AnimFrame((_uint)ANIM_TYPE::IDLE, iRand);
	
	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Ready_StateFSM()
{
	if (!m_pStateController)
		return E_FAIL;

	CMonster_FSM::FSMMOVEDESC tMoveDesc;
	ZeroMemory(&tMoveDesc, sizeof(tMoveDesc));
	tMoveDesc.pAnimator = m_pAnimator;
	tMoveDesc.pTransform = m_pTransform;
	tMoveDesc.pController = m_pStateController;

	CMonster_FSM::FSMACTORDESC tActorDesc;
	ZeroMemory(&tActorDesc, sizeof(tActorDesc));
	tActorDesc.pAnimator = m_pAnimator;
	tActorDesc.pController = m_pStateController;
	tActorDesc.pActor = this;

	lstrcpy(tMoveDesc.pName, L"Idle");
	if (FAILED(m_pStateController->Add_State(L"Idle", CShooter_Idle::Create(m_pDevice, m_pDeviceContext, &tMoveDesc))))
		return E_FAIL;

	lstrcpy(tMoveDesc.pName, L"Chase");
	if (FAILED(m_pStateController->Add_State(L"Chase", CShooter_Chase::Create(m_pDevice, m_pDeviceContext, &tMoveDesc))))
		return E_FAIL;

	lstrcpy(tActorDesc.pName, L"Attack");
	if (FAILED(m_pStateController->Add_State(L"Attack", CShooter_Attack::Create(m_pDevice, m_pDeviceContext, &tActorDesc))))
		return E_FAIL;

	lstrcpy(tActorDesc.pName, L"Hit");
	if (FAILED(m_pStateController->Add_State(L"Hit", CShooter_Hit::Create(m_pDevice, m_pDeviceContext, &tActorDesc))))
		return E_FAIL;

	lstrcpy(tMoveDesc.pName, L"Death");
	if (FAILED(m_pStateController->Add_State(L"Death", CShooter_Death::Create(m_pDevice, m_pDeviceContext, &tMoveDesc))))
		return E_FAIL;

	lstrcpy(tActorDesc.pName, L"Groggy");
	if (FAILED(m_pStateController->Add_State(L"Groggy", CShooter_Groggy::Create(m_pDevice, m_pDeviceContext, &tActorDesc))))
		return E_FAIL;


	lstrcpy(tActorDesc.pName, L"Excution");
	if (FAILED(m_pStateController->Add_State(L"Excution", CShooter_Excution::Create(m_pDevice, m_pDeviceContext, &tActorDesc))))
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

	//cout << "idle?? ????" << endl;

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Ready_UI()
{
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::SHOOTER;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iCurScene, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;

	Safe_AddRef(m_pPanel);

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	m_fMaxHp = 250.f;
	m_fCurrentHp = m_fMaxHp;

	m_fMaxGroggyGauge = 10.f;
	m_fGroggyGauge = 0.f;

	m_pPanel->Set_HpBar(Get_HpRatio());
	m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());

	return S_OK;
}

_int CMonster_Bastion_Shooter::Change_State()
{
	wstring tmpState = m_pStateController->Get_CurStateTag();

	if (tmpState == L"Idle")
		Chase();

	if (m_bDead)
	{
		Set_IsAttack(false);
		if (tmpState == L"Death")
		{
			if (m_pAnimator->Get_CurrentAnimNode() == (_uint)ANIM_TYPE::DEATH
				&& m_pAnimator->Get_CurrentAnimation()->Is_Finished() && m_lifetime <= 0.f)
			{
				//m_bRemove = true;
				m_pPanel->Set_UIRemove(false);
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
			else if (1 == m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex())
			{
				Active_Effect((_uint)EFFECT::DEATH);
			}
			else
			{
				m_fGroggyGauge = 0.f;
				m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
			}
		}
		else if (tmpState == L"Excution")
		{
			m_pPanel->Set_UIRemove(false);

			if (m_pAnimator->Get_CurrentAnimNode() == (_uint)ANIM_TYPE::EXCUTION
				&& m_pAnimator->Get_CurrentAnimation()->Is_Finished() && m_lifetime <= 0.f)
			{
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
			m_bRemove = true;
			m_pPanel->Set_UIRemove(false);
			Active_Effect((_uint)EFFECT::DEATH);
			return 0;
		}
	}
	if (m_fGroggyGauge >= m_fMaxGroggyGauge)
	{
		//???????????? ????state???? ???? ?????? ???? 0???? ??????????
		m_bGroggy = true;
		m_pStateController->Change_State(L"Groggy");
		m_fGroggyGauge = 0.f;
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}

	if ((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::GROGGY_END == m_pAnimator->Get_CurrentAnimNode())
	{
		if (m_pAnimator->Get_AnimController()->Is_Finished())
		{
			m_bGroggy = false;
		}
	}

	return _int();
}

CMonster_Bastion_Shooter* CMonster_Bastion_Shooter::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_Bastion_Shooter* pInstance = new CMonster_Bastion_Shooter(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMonster_Bastion_Shooter Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_Bastion_Shooter::Clone(const _uint _iSceneID, void* _pArg)
{
	CMonster_Bastion_Shooter* pInstance = new CMonster_Bastion_Shooter(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CMonster_Bastion_Shooter Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Bastion_Shooter::Free()
{
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pCharacterController);
	Safe_Release(m_pPanel);

	__super::Free();
}
