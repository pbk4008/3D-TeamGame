#include "pch.h"
#include "Monster_Bastion_2HSword.h"
#include "Animation.h"

/* for. Weapon */
#include "RetributionBlade.h"

/* for. FSM */
#include "Bastion_2HSword_State.h"
#include "Bastion_2HSword_Idle.h"
#include "Bastion_2HSword_Hit.h"
#include "Bastion_2HSword_Death.h"
#include "Bastion_2HSword_Chaser.h"
#include "Bastion_2HSword_Dash.h"
#include "Bastion_2HSword_Attack.h"
#include "Bastion_2HSword_Rage.h"
#include "Bastion_2HSword_Attack_Rage.h"

CMonster_Bastion_2HSword::CMonster_Bastion_2HSword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Bastion_2HSword::CMonster_Bastion_2HSword(const CMonster_Bastion_2HSword& _rhs)
	: CActor(_rhs)
{
}

HRESULT CMonster_Bastion_2HSword::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_2HSword::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;
	if (FAILED(Ready_Components())) 
		return E_FAIL;
	if (FAILED(Ready_Weapon())) 
		return E_FAIL;
	if (FAILED(Ready_AnimFSM())) 
		return E_FAIL;
	if (FAILED(Ready_StateFSM()))
		return E_FAIL;

	return S_OK;
}

_int CMonster_Bastion_2HSword::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	/* Collider Update */
	//m_pColliderCom->Update(m_pTransform->Get_WorldMatrix());

	/* State FSM Update */
	iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	// 무기 업뎃
	if (m_pCurWeapon)
	{
		iProgress = m_pCurWeapon->Tick(_dDeltaTime);
		if (NO_EVENT != iProgress) 
			return iProgress;
	}
	return _int();
}

_int CMonster_Bastion_2HSword::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	/* State FSM Late Update */
	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	m_pCharacterController->Update_OwnerTransform();

	// 무기 레잇업뎃
	if (m_pCurWeapon)
	{
		iProgress = m_pCurWeapon->LateTick(_dDeltaTime);
		if (NO_EVENT != iProgress) 
			return iProgress;
	}

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;
	return _int();
}

HRESULT CMonster_Bastion_2HSword::Render()
{
	if (m_bRender)
	{
		if (FAILED(__super::Render()))
			return E_FAIL;

		_matrix smatWorld, smatView, smatProj;
		smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
		smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
		smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

		if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
			return E_FAIL;

		for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pModel->Render(i, 0))) return E_FAIL;
		}
	}
	else
	{
		m_pCurWeapon = nullptr;
	}
#ifdef _DEBUG
		Render_Debug();
#endif
	return S_OK;
}

HRESULT CMonster_Bastion_2HSword::Ready_Components()
{
	/* for. Transform Com */
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.7f;
	transformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(transformDesc);
	_float4 vPosition = { 0.f, 2.f, 3.f, 1.f };

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));

	/* for. Model Com */
	if (FAILED(SetUp_Components(m_iSceneID, L"Model_Bastion_2HSword_Bin", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	/* for. State Controller */
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	m_AanimDesc.pModel = m_pModel;
	m_AanimDesc.pTransform = m_pTransform;

	/* for.Anim FSM */
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Animator", (CComponent**)&m_pAnimator, &m_AanimDesc)))
		return E_FAIL;

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

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCharacterControllerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_2HSword::Ready_Weapon()
{
	CHierarchyNode* pWeaponBone = m_pModel->Get_BoneMatrix("weapon_r");
	CWeapon* pWeapon = nullptr;

	//RetributionBlade
	pWeapon = CRetributionBlade::Create(m_pDevice, m_pDeviceContext);
	pWeapon->NativeConstruct(m_iSceneID, pWeaponBone);
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	m_umapWeapons.emplace(L"RetributionBlade", pWeapon);
	m_pCurWeapon = pWeapon;

	return S_OK;
}

HRESULT CMonster_Bastion_2HSword::Ready_AnimFSM(void)
{
	/* 	##1.추가하는 애니메이션 태그
		##2.연결하고자 하는 애니메이션 태그
		##3.애니메이션
		##4.루트 애님
		##5.루트 애님시 트랜스폼이 따라가게 할 것인지(제자리)
		##6.루프설정
		##7.루트애님 옵션, 막고자 하는 축설정
		##8.쌍방연결 default : false
	*/
#pragma region Anim List
	//pAnimation = m_pModel->Get_Animation("A_Death");
	//pAnimation = m_pModel->Get_Animation("A_Ricochet");
	//pAnimation = m_pModel->Get_Animation("A_Flinch_Left");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Fwd_Start");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Fwd");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Fwd_Stop");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Bwd_Start");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Bwd");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Bwd_Stop"); 
	//pAnimation = m_pModel->Get_Animation("A_Walk_Left_Start");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Left");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Left_Stop");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Right_Start");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Right");
	//pAnimation = m_pModel->Get_Animation("A_Walk_Right_Stop");
	//pAnimation = m_pModel->Get_Animation("A_Dash_Bwd");
	//pAnimation = m_pModel->Get_Animation("A_Dash_Left");
	//pAnimation = m_pModel->Get_Animation("A_Dash_Right");
	//pAnimation = m_pModel->Get_Animation("A_Attack_R1");
	//pAnimation = m_pModel->Get_Animation("A_Attack_R2");
	//pAnimation = m_pModel->Get_Animation("A_Attack_S1");
	//pAnimation = m_pModel->Get_Animation("A_Attack_S3");
	//pAnimation = m_pModel->Get_Animation("A_BattleCry_Start");
	//pAnimation = m_pModel->Get_Animation("A_BattleCry_Loop");
	//pAnimation = m_pModel->Get_Animation("A_BattleCry_End");
	//pAnimation = m_pModel->Get_Animation("A_Taunt_Roar");
	//pAnimation = m_pModel->Get_Animation("A_Kneel_Start");
	//pAnimation = m_pModel->Get_Animation("A_Kneel_Loop");
	//pAnimation = m_pModel->Get_Animation("A_Kneel_End");
	//pAnimation = m_pModel->Get_Animation("A_Stun_Start");
	//pAnimation = m_pModel->Get_Animation("A_Stun_Loop");
	//pAnimation = m_pModel->Get_Animation("A_Stun_End");
	//pAnimation = m_pModel->Get_Animation("A_Turn_45_Left");
	//pAnimation = m_pModel->Get_Animation("A_Turn_45_Right");
	//pAnimation = m_pModel->Get_Animation("A_Turn_90_Left");
	//pAnimation = m_pModel->Get_Animation("A_Turn_90_Right");
	//pAnimation = m_pModel->Get_Animation("A_Turn_135_Left");
	//pAnimation = m_pModel->Get_Animation("A_Turn_135_Right");
	//pAnimation = m_pModel->Get_Animation("A_Turn_180_Left");
	//pAnimation = m_pModel->Get_Animation("A_Turn_180_Right");
#pragma endregion

#pragma region Default
	CAnimation* pAnimation = m_pModel->Get_Animation("A_Idle");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, TRUE, ERootOption::XYZ)))
		return E_FAIL;

	//Hit
	pAnimation = m_pModel->Get_Animation("A_Stagger_Left");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_FLINCH_LEFT, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
	//A_Death
	pAnimation = m_pModel->Get_Animation("A_Death");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_DEATH, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion

#pragma region Walk_Fwd
	//A_Walk_Fwd
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

#pragma region Dash
	//A_Dash_Bwd
	pAnimation = m_pModel->Get_Animation("A_Dash_Bwd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_DASH_BWD, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Dash_Right");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_DASH_RIGHT, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Dash_Left");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_DASH_LEFT, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;
#pragma  endregion

#pragma region Attack
	//A_Attack
	pAnimation = m_pModel->Get_Animation("A_Attack_R1");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ATTACK_R1, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Attack_R2");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ATTACK_R2, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Attack_S1");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ATTACK_S1, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Attack_S3");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ATTACK_S3, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;
#pragma endregion

#pragma region Rage
	//Rage
	pAnimation = m_pModel->Get_Animation("A_Taunt_Roar");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_TAUNT_ROAR, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_BattleCry_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_BATTLECRY_ST, (_uint)ANIM_TYPE::A_TAUNT_ROAR, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_BattleCry");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_BATTLECRY, (_uint)ANIM_TYPE::A_BATTLECRY_ST, pAnimation, TRUE, TRUE, TRUE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_BattleCry_End");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_BATTLECRY_ED, (_uint)ANIM_TYPE::A_BATTLECRY, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;
#pragma endregion

#pragma region  Set Any Entry Animation
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_IDLE);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_DEATH);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_FLINCH_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ST);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ED);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_DASH_BWD);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_DASH_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_DASH_RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ATTACK_R1);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ATTACK_R2);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ATTACK_S1);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ATTACK_S3);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_DASH_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TAUNT_ROAR);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_BATTLECRY_ED);
#pragma endregion

#pragma region  Auto Change Anim
	/* ##1.끝나는 애님 ##2.루트할 애님  */
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ST, (_uint)ANIM_TYPE::A_WALK_FWD);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD, (_uint)ANIM_TYPE::A_WALK_FWD_ED);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ED, (_uint)ANIM_TYPE::A_IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_TAUNT_ROAR, (_uint)ANIM_TYPE::A_BATTLECRY_ST);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_BATTLECRY_ST, (_uint)ANIM_TYPE::A_BATTLECRY);
	//m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_BATTLECRY, (_uint)ANIM_TYPE::A_BATTLECRY_ED);
	//m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_BATTLECRY_ED, (_uint)ANIM_TYPE::A_IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_DASH_BWD, (_uint)ANIM_TYPE::A_WALK_FWD_ST);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_DASH_LEFT, (_uint)ANIM_TYPE::A_WALK_FWD_ST);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_DASH_RIGHT, (_uint)ANIM_TYPE::A_WALK_FWD_ST);
#pragma endregion

#pragma region Anim to Anim Connect
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_WALK_FWD_ED, FALSE)))
		return E_FAIL;
	//if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_DASH_BWD, FALSE)))
	//	return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_DASH_LEFT, FALSE)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_DASH_RIGHT, FALSE)))
		return E_FAIL;
	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_BATTLECRY, (_uint)ANIM_TYPE::A_BATTLECRY_ED, FALSE)))
		return E_FAIL;
#pragma endregion
	return S_OK;
}

HRESULT CMonster_Bastion_2HSword::Ready_StateFSM(void)
{
	/* for. Monster Idle */
	if (FAILED(m_pStateController->Add_State(L"Idle", CBastion_2HSword_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for. Player Chaser */
	if (FAILED(m_pStateController->Add_State(L"Chaser", CBastion_2HSword_Chaser::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;	

	/* for. Dash */
	if (FAILED(m_pStateController->Add_State(L"Dash", CBastion_2HSword_Dash::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for. Attack */
	if (FAILED(m_pStateController->Add_State(L"Attack", CBastion_2HSword_Attack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for. Rage */
	if (FAILED(m_pStateController->Add_State(L"Rage", CBastion_2HSword_Rage::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for. Rage_Attack */
	if (FAILED(m_pStateController->Add_State(L"Rage_Attack", CBastion_2HSword_Attack_Rage::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for. Death */
	if (FAILED(m_pStateController->Add_State(L"Death", CBastion_2HSword_Death::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for. Hit */
	if (FAILED(m_pStateController->Add_State(L"Hit", CBastion_2HSword_Hit::Create(m_pDevice, m_pDeviceContext))))
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

HRESULT CMonster_Bastion_2HSword::Render_Debug(void)
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

	// 애니메이션 이름
	string CurAnimName = m_pAnimation->Get_Name();
	wstring wstrCurAnimTag;
	wstring wstrAnimname = L"Cur Anim Tag : ";
	wstrCurAnimTag.assign(CurAnimName.begin(), CurAnimName.end());
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrAnimname + wstrCurAnimTag).c_str(), _float2(950.f, 60.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	// 애니메이션 상태
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

CMonster_Bastion_2HSword* CMonster_Bastion_2HSword::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_Bastion_2HSword* pInstance = new CMonster_Bastion_2HSword(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMonster_Bastion_2HSword Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_Bastion_2HSword::Clone(const _uint _iSceneID, void* _pArg)
{
	CMonster_Bastion_2HSword* pInstance = new CMonster_Bastion_2HSword(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CMonster_Bastion_2HSword Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Bastion_2HSword::Free()
{
	for (auto& pair : m_umapWeapons)
		Safe_Release(pair.second);
	m_umapWeapons.clear();

	Safe_Release(m_pCharacterController);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pModel);

	__super::Free();
}
