#include "pch.h"
#include "Monster_Bastion_Sword.h"
#include "Animation.h"
#include "Stargazer.h"
#include "HierarchyNode.h"
#include "Animator.h"
#include "CharacterController.h"

#include "UI_Monster_Panel.h"

#include "Bastion_Sword_Idle.h"
#include "Bastion_Sword_Chase.h"
#include "Bastion_Sword_Attack.h"
#include "Bastion_Sword_Hit.h"
#include "Bastion_Sword_Death.h"
#include "Bastion_Sword_Groggy.h"
#include "Bastion_Sword_Paring.h"
#include "Bastion_Sword_Turn.h"
#include "Bastion_Sword_Walk.h"

#include "Stage1.h"
#include "Stage2.h"

CMonster_Bastion_Sword::CMonster_Bastion_Sword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CActor(_pDevice, _pDeviceContext)
	, m_pCharacterController(nullptr)
	, m_pModelCom(nullptr)
	, m_pStateController(nullptr)
	, m_pAnimator(nullptr)
{
}

CMonster_Bastion_Sword::CMonster_Bastion_Sword(const CMonster_Bastion_Sword& _rhs)
	:CActor(_rhs)
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
}

HRESULT CMonster_Bastion_Sword::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::MONSTER_1H;

	return S_OK;
}

HRESULT CMonster_Bastion_Sword::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;
	m_iCurScene = _iSceneID;

	if (_pArg)
	{
		_float3 tPos = (*(_float3*)_pArg);
		if (FAILED(Set_SpawnPosition(tPos)))
			return E_FAIL;
	}
	else
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f, 0.f, 10.f, 1.f));
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(Set_Animation_FSM()))
		return E_FAIL;

	if (FAILED(Set_Weapon()))
		return E_FAIL;

	if (FAILED(Set_State_FSM()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.5f);

	//MonsterBar Panel
	m_pPanel->Set_Show(false);
	m_bUIShow = false;
	//setActive(false);

	m_tAttackDesc.iLevel = 2;
	m_tAttackDesc.fDamage = 5.f;

	return S_OK;
}

_int CMonster_Bastion_Sword::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}
	m_pTransform->Set_Velocity(XMVectorZero());

	m_pTransform->Fall(_dDeltaTime);
	//상태 컨트롤러 돌리기->애니메이터 자동으로 돌아감
	m_pStateController->Tick(_dDeltaTime);

	//무기 뼈 업데이트
	if (nullptr != m_pWeapon)
	{
		m_pWeapon->Tick(_dDeltaTime);
	}

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

			else if (g_pGameInstance->getCurrentLevel() == (_uint)SCENEID::SCENE_STAGE2)
				static_cast<CStage2*>(pLevel)->Minus_MonsterCount();
		}
		else
		{
			m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());
		}
	}
	if (true == m_bUIShow)
		m_pPanel->Set_Show(true);
	else if (false == m_bUIShow)
		m_pPanel->Set_Show(false);

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());
	Dead_Check();
	return 0;
}

_int CMonster_Bastion_Sword::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
	}
	if (!m_bDead)
	{
		m_pCharacterController->Update_OwnerTransform();
		m_pWeapon->LateTick(_dDeltaTime);
	}
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	
	return 0;
}

HRESULT CMonster_Bastion_Sword::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
		m_pModelCom->Render(i, 0);

	return S_OK;
}

void CMonster_Bastion_Sword::Set_Remove(_bool bCheck)
{
	m_bRemove = bCheck;
	m_pPanel->Set_UIRemove(bCheck);
}

void CMonster_Bastion_Sword::Set_IsAttack(const _bool _isAttack)
{
	m_IsAttack = _isAttack;
	if (m_pWeapon)
		m_pWeapon->Set_IsAttack(_isAttack);
}

void CMonster_Bastion_Sword::OnTriggerEnter(CCollision& collision)
{
	if (!m_bDead)
	{
		if (m_fCurrentHp >= 0.f && g_pObserver->IsAttack())
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

void CMonster_Bastion_Sword::OnTriggerExit(CCollision& collision)
{
}

HRESULT CMonster_Bastion_Sword::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Bastion_Sword", L"Model", (CComponent**)&m_pModelCom)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModelCom->Set_PivotMatrix(matPivot);

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModelCom;
	tDesc.pTransform = m_pTransform;
	tDesc.eType = CAnimationController::EType::CharacterController;//애니메이션 돌릴 타입 선택(캐컨으로 움직일지 / Transform으로 움직일지)

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimator, &tDesc)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"Com_StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	CCharacterController::DESC tController;

	tController.fHeight = 1.f;
	tController.fRadius = 1.f;
	tController.fContactOffset = tController.fRadius*0.1f;
	tController.fStaticFriction = 0.5f;
	tController.fDynamicFriction = 0.5f;
	tController.fRestitution = 0.f;
	
	tController.vPosition = _float3(0.f, 0.f, 0.f);
	tController.pGameObject = this;

	if(FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, 
		L"Proto_Component_CharacterController", L"Com_CharacterController", (CComponent**)&m_pCharacterController,&tController)))
		return E_FAIL;

	m_pCharacterController->setOwnerTransform(m_pTransform);
	return S_OK;
}

HRESULT CMonster_Bastion_Sword::Set_Animation_FSM()
{
	//애니메이션 생성
	CAnimation* pAnim = m_pModelCom->Get_Animation("Idle");
	//생성 하면서 연결(연결 할애, 연결 당할애, 애니메이션, 루트 애님, 트랜스폼(루트애니메이션할때 찐으로 따라감), 루프, 옵션)
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;

	////////////////////Run
	pAnim = m_pModelCom->Get_Animation("Run_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Run_Loop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_START, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Run_Stop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::RUN_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	/////////////////////JumpAttack
	pAnim = m_pModelCom->Get_Animation("Attack_JumpStart");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_JUMPSTART, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Attack_JumpEnd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_JUMPEND, (_uint)ANIM_TYPE::ATTACK_JUMPSTART, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	////////////////////Attack
	pAnim = m_pModelCom->Get_Animation("Attack_Double 1");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_DOUBLE, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Attack_Single");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_SINGLE, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	//////////////////////Hit&Death
	pAnim = m_pModelCom->Get_Animation("Hit1");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::HIT1, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Hit2");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::HIT2, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Hit4");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::HIT3, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Hit5");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::HIT4, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Death");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::DEATH, (_uint)ANIM_TYPE::HIT1, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	//////////////////////////Groggy
	pAnim = m_pModelCom->Get_Animation("Groggy_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::GROGGY_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Groggy_Loop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::GROGGY_LOOP, (_uint)ANIM_TYPE::GROGGY_START, pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Groggy_End");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::GROGGY_END, (_uint)ANIM_TYPE::GROGGY_LOOP, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	///////////////////Paring
	pAnim = m_pModelCom->Get_Animation("Paring");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::PARING, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	////////////////Walk_BackWard
	pAnim = m_pModelCom->Get_Animation("Walk_BackWard_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::BACKWARD_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Walk_BackWard_Loop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::BACKWARD_LOOP, (_uint)ANIM_TYPE::BACKWARD_START, pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Walk_BackWard_Stop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::BACKWARD_END, (_uint)ANIM_TYPE::BACKWARD_LOOP, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	//////////////Walk_Forward
	pAnim = m_pModelCom->Get_Animation("Walk_Forward_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::FORWARD_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Walk_Forward_Loop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::FORWARD_LOOP, (_uint)ANIM_TYPE::FORWARD_START, pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Walk_Forward_Stop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::FORWARD_END, (_uint)ANIM_TYPE::FORWARD_LOOP, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	//////////////Walk_Left
	pAnim = m_pModelCom->Get_Animation("Walk_Left_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::LEFTWALK_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Walk_Left_Loop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::LEFTWALK_LOOP, (_uint)ANIM_TYPE::LEFTWALK_START, pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Walk_Forward_Stop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::LEFTWALK_END, (_uint)ANIM_TYPE::LEFTWALK_LOOP, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	///////////////Walk_Right
	pAnim = m_pModelCom->Get_Animation("Walk_Right_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RIGHTWALK_START, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Walk_Right_Loop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RIGHTWALK_LOOP, (_uint)ANIM_TYPE::RIGHTWALK_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("Walk_Right_Stop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RIGHTWALK_END, (_uint)ANIM_TYPE::RIGHTWALK_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Turn");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::TURN, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	//애니메이션 연결(연결 당할 애, 연결할 애, 쌍방으로 연결할지 안할지)
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::DEATH, (_uint)ANIM_TYPE::HIT2, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::DEATH, (_uint)ANIM_TYPE::HIT3, false)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::DEATH, (_uint)ANIM_TYPE::HIT4, false)))
		return E_FAIL;

	//자동으로 돌릴 애들(끝나는애, 끝나고 시작할 애)
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::RUN_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_JUMPSTART, (_uint)ANIM_TYPE::ATTACK_JUMPEND);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_JUMPEND, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_SINGLE, (_uint)ANIM_TYPE::IDLE);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_DOUBLE, (_uint)ANIM_TYPE::IDLE);

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
	
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::TURN, (_uint)ANIM_TYPE::IDLE);

	//언제든지 바꿀 수 있는애들 생성
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::RUN_START);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::ATTACK_JUMPSTART);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::ATTACK_DOUBLE);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::ATTACK_SINGLE);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::HIT1);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::HIT2);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::HIT3);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::HIT4);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::DEATH);
	

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::GROGGY_START);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::PARING);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::BACKWARD_START);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::FORWARD_START);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::LEFTWALK_START);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::RIGHTWALK_START);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::TURN);


	//애니메이션 체인지(바꿀 애)
	_uint iRand = rand()%15;
	m_pAnimator->Change_Animation((_uint)ANIM_TYPE::IDLE);
	m_pAnimator->Add_AnimFrame((_uint)ANIM_TYPE::IDLE, iRand);

	return S_OK;
}

HRESULT CMonster_Bastion_Sword::Set_State_FSM()
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


	lstrcpy(tMoveDesc.pName,L"Idle");
	if (FAILED(m_pStateController->Add_State(L"Idle", CBastion_Sword_Idle::Create(m_pDevice, m_pDeviceContext, &tMoveDesc))))
		return E_FAIL;

	lstrcpy(tMoveDesc.pName, L"Chase");
	if (FAILED(m_pStateController->Add_State(L"Chase", CBastion_Sword_Chase::Create(m_pDevice, m_pDeviceContext, &tMoveDesc))))
		return E_FAIL;

	lstrcpy(tActorDesc.pName, L"Attack");
	if (FAILED(m_pStateController->Add_State(L"Attack", CBastion_Sword_Attack::Create(m_pDevice, m_pDeviceContext, &tActorDesc))))
		return E_FAIL;

	lstrcpy(tFSMDesc.pName, L"Hit");
	if (FAILED(m_pStateController->Add_State(L"Hit", CBastion_Sword_Hit::Create(m_pDevice, m_pDeviceContext, &tFSMDesc))))
		return E_FAIL;

	lstrcpy(tMoveDesc.pName, L"Death");
	if (FAILED(m_pStateController->Add_State(L"Death", CBastion_Sword_Death::Create(m_pDevice, m_pDeviceContext, &tMoveDesc))))
		return E_FAIL;

	lstrcpy(tActorDesc.pName, L"Groggy");
	if (FAILED(m_pStateController->Add_State(L"Groggy", CBastion_Sword_Groggy::Create(m_pDevice, m_pDeviceContext, &tActorDesc))))
		return E_FAIL;

	lstrcpy(tFSMDesc.pName, L"Paring");
	if (FAILED(m_pStateController->Add_State(L"Paring", CBastion_Sword_Paring::Create(m_pDevice, m_pDeviceContext, &tFSMDesc))))
		return E_FAIL;

	lstrcpy(tFSMDesc.pName, L"Turn");
	if (FAILED(m_pStateController->Add_State(L"Turn", CBastion_Sword_Turn::Create(m_pDevice, m_pDeviceContext, &tFSMDesc))))
		return E_FAIL;

	lstrcpy(tMoveDesc.pName, L"Walk");
	if (FAILED(m_pStateController->Add_State(L"Walk", CBastion_Sword_Walk::Create(m_pDevice, m_pDeviceContext, &tMoveDesc))))
		return E_FAIL;

	m_pStateController->Change_State(L"Idle");

	return S_OK;
}

HRESULT CMonster_Bastion_Sword::Set_Weapon()
{
	m_pWeapon = static_cast<CStargazer*>(g_pGameInstance->Clone_GameObject(m_iSceneID, L"Proto_GameObject_Weapon_Stargazer"));

	if (!m_pWeapon)
		return E_FAIL;

	m_pWeapon->Set_Owner(this);

	vector<CHierarchyNode*> vecNode=m_pModelCom->Get_HierachyNodes();
	CHierarchyNode* pWeaponBone = nullptr;
	for (auto& pNode : vecNode)
	{
		if (!strcmp(pNode->Get_Name(), "weapon_r"))
		{
			pWeaponBone = pNode;
			break;
		}
	}
	m_pWeapon->Set_FixedBone(pWeaponBone);
	static_cast<CWeapon*>(m_pWeapon)->Set_OwnerPivotMatrix(m_pModelCom->Get_PivotMatrix());


	return S_OK;
}

HRESULT CMonster_Bastion_Sword::Ready_UI()
{
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::SWORD;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iCurScene, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
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

	return S_OK;
}

_int CMonster_Bastion_Sword::Dead_Check()
{
	if(m_bDead)
	{
		if (m_pStateController->Get_CurStateTag() == L"Death")
		{
			if (m_pAnimator->Get_CurrentAnimNode() == (_uint)ANIM_TYPE::DEATH
				&& m_pAnimator->Get_CurrentAnimation()->Is_Finished())
			{
				m_bRemove = true;
				m_bUIShow = false;
				m_pPanel->Set_Show(false);
				m_pPanel->Set_UIRemove(false);
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
		else
		{
			m_bUIShow = false;
			m_pPanel->Set_Show(false);
			m_bRemove = true;
			m_pPanel->Set_UIRemove(false);
			Active_Effect((_uint)EFFECT::DEATH);
			return 0;
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

void CMonster_Bastion_Sword::Hit()
{
	m_eHitType = ANIM_TYPE::HIT1;
	CBastion_Sword_Hit::HITDATA tData;
	ZeroMemory(&tData, sizeof(tData));

	m_fCurrentHp--;
	m_pPanel->Set_HpBar(Get_HpRatio());

	if (!m_bFirstHit)
	{
		m_bFirstHit = true; //딱 한번 true로 변경해줌
		m_pPanel->Set_BackUIGapY(1.f);
	}
	if (!m_bGroggy)
	{
		g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Sword1H_Hit);
		g_pGameInstance->Play_Shot(L"Monster_Hit_6", CSoundMgr::CHANNELID::Sword1H_Hit);
		tData.fCurHp = m_fCurrentHp;
		tData.iHitType = (_uint)m_eHitType;
		Active_Effect((_uint)EFFECT::HIT);
		Active_Effect((_uint)EFFECT::FLOATING);
		m_pStateController->Change_State(L"Hit", &tData);
		m_fGroggyGauge += 2; //TODO::수치정해서바꿔줘야됨
		m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
	}
	//Active_Effect((_uint)EFFECT::GUARD);
}

CMonster_Bastion_Sword* CMonster_Bastion_Sword::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_Bastion_Sword* pInstance = new CMonster_Bastion_Sword(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMonster_Bastion_Sword");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_Bastion_Sword::Clone(const _uint _iSceneID, void* _pArg)
{
	CMonster_Bastion_Sword* pInstance = new CMonster_Bastion_Sword(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Clone CMonster_Bastion_Sword");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Bastion_Sword::Free()
{
	__super::Free();

	Safe_Release(m_pWeapon);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pStateController);
	Safe_Release(m_pCharacterController);
	Safe_Release(m_pPanel);
}
