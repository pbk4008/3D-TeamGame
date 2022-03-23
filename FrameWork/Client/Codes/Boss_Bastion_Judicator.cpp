#include "pch.h"
#include "Boss_Bastion_Judicator.h"
#include "Animation.h"
#include "ShieldBreaker.h"
#include "UI_Monster_Panel.h"

#include "MidBoss_Rage.h"
#include "MidBoss_BattleCry.h"
#include "MidBoss_Attack.h"
#include "MidBoss_Turn.h"
#include "MidBoss_Death.h"
#include "MidBoss_Stun.h"

CBoss_Bastion_Judicator::CBoss_Bastion_Judicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CActor(pDevice, pDeviceContext)
{
}

CBoss_Bastion_Judicator::CBoss_Bastion_Judicator(const CBoss_Bastion_Judicator& rhs)
	:CActor(rhs)
{
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

	_vector Pos = { 0.f, 12.f, 20.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(Set_Animation_FSM()))
		return E_FAIL;

	if (FAILED(Set_State_FSM()))
		return E_FAIL;

	CHierarchyNode* pBone = m_pModelCom->Get_BoneMatrix("weapon_r_end");
	CShieldBreaker* pWeapon = CShieldBreaker::Create(m_pDevice, m_pDeviceContext);
	pWeapon->NativeConstruct(m_iSceneID, pBone);
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModelCom->Get_PivotMatrix());
	m_pWeapon = pWeapon;

	
	//MidBossBar Panel
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::MIDBOSS;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	m_isFall = true;
	m_iObectTag = (_uint)GAMEOBJECT::MIDDLE_BOSS;

	m_fMaxHp = 10.f;
	m_fCurrentHp = m_fMaxHp;

	m_fMaxGroggy = 10.f;
	m_fGroggy = 0;

	m_pPanel->Set_GroggyBar(m_fMaxGroggy, m_fGroggy);

	return S_OK;
}

_int CBoss_Bastion_Judicator::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
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
	
	if (0 >= m_fCurrentHp)
	{
		m_bDead = true;
		m_pStateController->Change_State(L"Death");
	}

	if (m_fGroggy >= m_fMaxGroggy)
	{
		//스턴상태일때 스턴state에서 현재 그로기 계속 0으로 고정시켜줌
		m_bGroggy = true;
		m_pStateController->Change_State(L"Stun");
		m_fGroggy = 0.f;
		m_pPanel->Set_GroggyBar(m_fMaxGroggy, m_fGroggy);
	}
	
	if (true == m_bGroggy || true == m_bDead )
	{
		m_fGroggy = 0.f;
		m_pPanel->Set_GroggyBar(m_fMaxGroggy, m_fGroggy);
	}

	if (STUN_END == m_pAnimator->Get_CurrentAnimNode())
	{
		if (m_pAnimator->Get_AnimController()->Is_Finished())
		{
			m_bGroggy = false;
		}
	}

	Fall(TimeDelta);
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
		{
			return E_FAIL;
		}
	}

	

	return 0;
}

HRESULT CBoss_Bastion_Judicator::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		m_pModelCom->Render(i, 0);
	}

	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Boss_Bastion_Tier4", L"Model", (CComponent**)&m_pModelCom)))
	{
		return E_FAIL;
	}
	_matrix matPivot = XMMatrixIdentity();
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModelCom->Set_PivotMatrix(matPivot);


	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModelCom;
	tDesc.pTransform = m_pTransform;
	tDesc.eType = CAnimationController::EType::CharacterController;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)& m_pAnimator, &tDesc)))
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

	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::Set_Animation_FSM()
{
	//시작루프
	CAnimation* pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_Rage_Phalanxar");
	//연결할애님, 연결당할애님, 애니메이션, 루트애님, 트랜스폼(루트애니메이션때 따라감), 루프, 옵션
	if (FAILED(m_pAnimator->Insert_Animation(RAGE, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

#pragma region 공격재시작루프
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_BattleCry_Start_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(BATTLECRY_START, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_BattleCry_Loop_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(BATTLECRY_LOOP, BATTLECRY_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_BattleCry_End_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(BATTLECRY_END, BATTLECRY_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	//애니메이션연결(연결당할애님, 연결할애님, 쌍방으로연결할지안할지)
	if (FAILED(m_pAnimator->Connect_Animation(RAGE, BATTLECRY_END, false)))
		return E_FAIL;
#pragma endregion

#pragma region 스턴루프
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_Stun_Start_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(STUN_START, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_Stun_Loop_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(STUN_LOOP, STUN_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_Stun_End_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(STUN_END, STUN_LOOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation(RAGE, STUN_END, false)))
		return E_FAIL;
#pragma endregion

#pragma region 회전
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_135_Left_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_135LEFT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_135_Right_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_135RIGHT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_180_Left_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_180LEFT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_180_Right_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_180RIGHT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_45_Left_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_45LEFT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_45_Right_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_45RIGHT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_90_Left_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_90LEFT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Turn_90_Right_Normal");
	if (FAILED(m_pAnimator->Insert_Animation(TURN_90RIGHT_H, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region 상시어택애님
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Attack_JogR1");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_JOG_H, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Attack_L1_R2_Legacy");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_LEGACY_H, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Attack_R1_01");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_R1_H, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_2H_Hammer_Sprint_Attack");
	if (FAILED(m_pAnimator->Insert_Animation(SPRINT_ATTACK_H, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_Attack_R1_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_R1, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_Attack_S1_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_S1, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_Attack_S2_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(ATTACK_S2, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion 

#pragma region 죽음
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier4.ao|A_Death_Phalanxar");
	if (FAILED(m_pAnimator->Insert_Animation(DEATH, HEAD, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion 

	//자동으로 돌릴 애들(끝날애님, 끝나고시작할애님)
	//m_pAnimator->Set_UpAutoChangeAnimation(RAGE, BATTLECRY_START);
	m_pAnimator->Set_UpAutoChangeAnimation(BATTLECRY_START, BATTLECRY_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation(BATTLECRY_LOOP, BATTLECRY_END);
	m_pAnimator->Set_UpAutoChangeAnimation(BATTLECRY_END, RAGE);

	m_pAnimator->Set_UpAutoChangeAnimation(STUN_START, STUN_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation(STUN_LOOP, STUN_END);
	m_pAnimator->Set_UpAutoChangeAnimation(STUN_END, RAGE);

	m_pAnimator->Insert_AnyEntryAnimation(BATTLECRY_START);

	//스턴루프
	m_pAnimator->Insert_AnyEntryAnimation(STUN_START);

	//어택애님
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_JOG_H);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_LEGACY_H);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_R1_H);
	m_pAnimator->Insert_AnyEntryAnimation(SPRINT_ATTACK_H);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_R1);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_S1);
	m_pAnimator->Insert_AnyEntryAnimation(ATTACK_S2);

	//죽음
	m_pAnimator->Insert_AnyEntryAnimation(DEATH);

	//회전
	m_pAnimator->Insert_AnyEntryAnimation(TURN_135LEFT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_135RIGHT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_180LEFT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_180RIGHT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_90LEFT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_90RIGHT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_45LEFT_H);
	m_pAnimator->Insert_AnyEntryAnimation(TURN_45RIGHT_H);


	m_pAnimator->Change_Animation(RAGE);

	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::Set_State_FSM()
{
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

	if (FAILED(m_pStateController->Add_State(L"Stun", CMidBoss_Stun::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CMonster_FSM*>(pair.second)->Set_Monster(this);
		static_cast<CMonster_FSM*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CMonster_FSM*>(pair.second)->Set_Model(m_pModelCom);
		static_cast<CMonster_FSM*>(pair.second)->Set_Animator(m_pAnimator);
	}

	m_pStateController->Change_State(L"Rage");

	return S_OK;
}

const _int CBoss_Bastion_Judicator::Fall(const _double& TimeDelta)
{
	if (m_isFall)
	{
		_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		if (-10.f < XMVectorGetY(svPos))
		{
			m_pTransform->Add_Velocity(XMVectorSet(0.f, -9.8f * (_float)TimeDelta, 0.f, 0.f));
		}
	}
	return _int();
}

void CBoss_Bastion_Judicator::OnTriggerEnter(CCollision& collision)
{

	if (true == g_pObserver->IsAttack()) //플레이어공격일때
	{
		m_bFirstHit = true; //딱 한번 true로 변경해줌

		if (true == m_bFirstHit)
		{
			m_pPanel->Set_BackUIGapY(1.f);
		}

		if ((_uint)GAMEOBJECT::WEAPON == collision.pGameObject->getTag())
		{
			--m_fCurrentHp;
			m_fGroggy += 2; //TODO::수치정해서바꿔줘야됨

			m_pPanel->Set_HpBar(m_fMaxHp, m_fCurrentHp);

			if (false == m_bGroggy)
			{	
				//그로기 아닐때만 증가할수있게
				m_pPanel->Set_GroggyBar(m_fMaxGroggy, m_fGroggy);
			}
		}
		else
		{

		}
			
	}
	
}

void CBoss_Bastion_Judicator::Set_IsAttack(const _bool _isAttack)
{
	m_bIsAttack = _isAttack;
	if (m_pWeapon)
		m_pWeapon->Set_IsAttack(_isAttack);
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
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pModelCom);

	__super::Free();
}
