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

#include"UI_Monster_Panel.h"
#include "Stage1.h"

CMonster_Bastion_Shooter::CMonster_Bastion_Shooter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
	, m_pCharacterController(nullptr)
	, m_pModelCom(nullptr)
	, m_pStateController(nullptr)
	, m_pAnimator(nullptr)
{
}

CMonster_Bastion_Shooter::CMonster_Bastion_Shooter(const CMonster_Bastion_Shooter& _rhs)
	: CActor(_rhs)
	, m_pCharacterController(_rhs.m_pCharacterController)
	, m_pModelCom(_rhs.m_pModelCom)
	, m_pStateController(_rhs.m_pStateController)
	, m_pAnimator(_rhs.m_pAnimator)
	, m_pPanel(_rhs.m_pPanel)
{
	Safe_AddRef(m_pCharacterController);
	Safe_AddRef(m_pModelCom);
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

	if (_pArg)
	{
		_float3 vPoint = (*(_float3*)_pArg);
		if (FAILED(Set_SpawnPosition(vPoint)))
			return E_FAIL;
	}
	else
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f, 0.f, 10.f, 1.f));
	}
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_AnimationFSM()))
		return E_FAIL;

	if (FAILED(Ready_StateFSM()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	m_pRenderer->SetRenderButton(CRenderer::PIXEL, true);
	m_pRenderer->SetRenderButton(CRenderer::PBRHDR, true);
	m_pRenderer->SetCameraTag(L"Camera_Silvermane");

	setActive(false);

	return S_OK;
}

_int CMonster_Bastion_Shooter::Tick(_double _dDeltaTime)
{
	if (!m_bFirst)
		m_pPanel->Set_Show(true);

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

			CLevel* pLevel = g_pGameInstance->getCurrentLevelScene();
			if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE1)
				static_cast<CStage1*>(pLevel)->Minus_MonsterCount();
		}
		else
			m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());
	}

	Change_State();

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CMonster_Bastion_Shooter::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	if(!m_bDead)
		m_pCharacterController->Update_OwnerTransform();

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;

	return _int();
}

HRESULT CMonster_Bastion_Shooter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) 
		return E_FAIL;
	if (FAILED(m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) 
		return E_FAIL;
	if (FAILED(m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) 
		return E_FAIL;

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		if (i == 2)
		{
			if (FAILED(m_pModelCom->Render(i, 1))) 	return E_FAIL;
		}
		else
		{
			if (FAILED(m_pModelCom->Render(i, 0))) 	return E_FAIL;
		}
	}

	return S_OK;
}

void CMonster_Bastion_Shooter::OnTriggerEnter(CCollision& collision)
{
	if (!m_bDead)
	{
		if (m_fCurrentHp >= 0.f)
		{
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

	m_fCurrentHp--;
	m_pPanel->Set_HpBar(Get_HpRatio());
	if (!m_bFirstHit)
	{
		m_bFirstHit = true; //딱 한번 true로 변경해줌
		m_pPanel->Set_BackUIGapY(1.f);
	}
	if (m_wstrCurState != L"Hit" && !m_bGroggy)
	{
		tData.fCurHp = m_fCurrentHp;
		tData.iHitType = (_uint)m_eHitType;
		m_wstrCurState = L"Hit";
		m_pStateController->Change_State(L"Hit", &tData);
	}

	if (!m_bGroggy)
	{
		//그로기 아닐때만 증가할수있게
		m_fGroggyGauge += 2; //TODO::수치정해서바꿔줘야됨
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}
}

HRESULT CMonster_Bastion_Shooter::Ready_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Bastion_Shooter", L"Model", (CComponent**)&m_pModelCom)))
		return E_FAIL;

	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModelCom->Set_PivotMatrix(matPivot);

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModelCom;
	tDesc.pTransform = m_pTransform;
	tDesc.eType = CAnimationController::EType::CharacterController;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimator, &tDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;

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
		return E_FAIL;

	m_pCharacterController->setOwnerTransform(m_pTransform);

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Ready_AnimationFSM()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation("Idle");

	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("RunStart");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("RunLoop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_START, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("RunEnd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::RUN_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Attack");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Hit1");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::HIT1, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Hit2");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::HIT2, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Hit3");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::HIT3, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Hit4");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::HIT4, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Death");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::DEATH, (_uint)ANIM_TYPE::HIT1, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("StunStart");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::GROGGY_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("StunLoop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::GROGGY_LOOP, (_uint)ANIM_TYPE::GROGGY_START, pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("StunEnd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::GROGGY_END, (_uint)ANIM_TYPE::GROGGY_LOOP, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("BwdWalkStart");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::BACKWARD_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("BwdWalkLoop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::BACKWARD_LOOP, (_uint)ANIM_TYPE::BACKWARD_START, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("BwdWalkEnd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::BACKWARD_END, (_uint)ANIM_TYPE::BACKWARD_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("FwdWalkStart");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::FORWARD_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("FwdWalkLoop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::FORWARD_LOOP, (_uint)ANIM_TYPE::FORWARD_START, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("FwdWalkStop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::FORWARD_END, (_uint)ANIM_TYPE::FORWARD_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("LeftWalkStart");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::LEFTWALK_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("LeftWalkLoop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::LEFTWALK_LOOP, (_uint)ANIM_TYPE::LEFTWALK_START, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("LeftWalkEnd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::LEFTWALK_END, (_uint)ANIM_TYPE::LEFTWALK_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("RightWalkStart");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RIGHTWALK_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("RightWalkLoop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RIGHTWALK_LOOP, (_uint)ANIM_TYPE::RIGHTWALK_START, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("RightWalkEnd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RIGHTWALK_END, (_uint)ANIM_TYPE::RIGHTWALK_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::RUN_END, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::ATTACK, false)))
		return E_FAIL;

	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HIT1, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HIT2, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HIT3, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HIT4, false)))
		return E_FAIL;

	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::DEATH, (_uint)ANIM_TYPE::HIT2, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::DEATH, (_uint)ANIM_TYPE::HIT3, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::DEATH, (_uint)ANIM_TYPE::HIT4, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::GROGGY_END, false)))
		return E_FAIL;

	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::BACKWARD_END, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::FORWARD_END, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::LEFTWALK_END, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::RIGHTWALK_END, false)))
		return E_FAIL;

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::RUN_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::HIT1, (_uint)ANIM_TYPE::IDLE);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::HIT2, (_uint)ANIM_TYPE::IDLE);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::HIT3, (_uint)ANIM_TYPE::IDLE);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::HIT4, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::GROGGY_START, (_uint)ANIM_TYPE::GROGGY_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::GROGGY_LOOP, (_uint)ANIM_TYPE::GROGGY_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::GROGGY_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::BACKWARD_START, (_uint)ANIM_TYPE::BACKWARD_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::BACKWARD_LOOP, (_uint)ANIM_TYPE::BACKWARD_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::BACKWARD_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::FORWARD_START, (_uint)ANIM_TYPE::FORWARD_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::FORWARD_LOOP, (_uint)ANIM_TYPE::FORWARD_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::FORWARD_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::LEFTWALK_START, (_uint)ANIM_TYPE::LEFTWALK_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::LEFTWALK_LOOP, (_uint)ANIM_TYPE::LEFTWALK_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::LEFTWALK_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RIGHTWALK_START, (_uint)ANIM_TYPE::RIGHTWALK_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RIGHTWALK_LOOP, (_uint)ANIM_TYPE::RIGHTWALK_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RIGHTWALK_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::RUN_START);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::ATTACK);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::HIT1);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::HIT2);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::HIT3);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::HIT4);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::DEATH);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::GROGGY_START);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::BACKWARD_START);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::FORWARD_START);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::LEFTWALK_START);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::RIGHTWALK_START);

	m_pAnimator->Change_Animation((_uint)ANIM_TYPE::IDLE);

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

	CMonster_FSM::FSMDESC tFSMDesc;
	ZeroMemory(&tFSMDesc, sizeof(tFSMDesc));
	tFSMDesc.pAnimator = m_pAnimator;
	tFSMDesc.pController = m_pStateController;

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

	lstrcpy(tFSMDesc.pName, L"Hit");
	if (FAILED(m_pStateController->Add_State(L"Hit", CShooter_Hit::Create(m_pDevice, m_pDeviceContext, &tFSMDesc))))
		return E_FAIL;

	lstrcpy(tFSMDesc.pName, L"Death");
	if (FAILED(m_pStateController->Add_State(L"Death", CShooter_Death::Create(m_pDevice, m_pDeviceContext, &tFSMDesc))))
		return E_FAIL;

	lstrcpy(tActorDesc.pName, L"Groggy");
	if (FAILED(m_pStateController->Add_State(L"Groggy", CShooter_Groggy::Create(m_pDevice, m_pDeviceContext, &tActorDesc))))
		return E_FAIL;

	lstrcpy(tMoveDesc.pName, L"Walk");
	if (FAILED(m_pStateController->Add_State(L"Walk", CShooter_Walk::Create(m_pDevice, m_pDeviceContext, &tMoveDesc))))
		return E_FAIL;

	m_pStateController->Change_State(L"Idle");

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Ready_UI()
{
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::SHOOTER;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;

	Safe_AddRef(m_pPanel);

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	m_fMaxHp = 2.f;
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

	if (tmpState != m_wstrCurState)
	{
		if (tmpState == L"Idle")
			Chase();
	}
	if (tmpState == L"Death")
	{
		if(m_pAnimator->Get_CurrentAnimNode() == (_uint)ANIM_TYPE::DEATH
			&& m_pAnimator->Get_CurrentAnimation()->Is_Finished())
		{
			setActive(true);
			m_bRemove = true;
			m_pPanel->Set_Show(false);
		}
		else
		{
			m_fGroggyGauge = 0.f;
			m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
		}
	}
	if (m_fGroggyGauge >= m_fMaxGroggyGauge)
	{
		//스턴상태일때 스턴state에서 현재 그로기 계속 0으로 고정시켜줌
		m_bGroggy = true;
		m_pStateController->Change_State(L"Groggy");
		m_fGroggyGauge = 0.f;
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
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
	__super::Free();
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pCharacterController);
	Safe_Release(m_pPanel);
}
