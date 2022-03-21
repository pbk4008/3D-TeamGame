#include "pch.h"
#include "Monster_Bastion_Healer.h"
#include "Animation.h"
#include "Staff.h"
/* for. FSM */
#include "Bastion_Healer_State.h"
#include "Bastion_Healer_Idle.h"
#include "Bastion_Healer_Chaser.h"
#include "Bastion_Healer_Chaser_End.h"
#include "Bastion_Healer_Attack.h"

CMonster_Bastion_Healer::CMonster_Bastion_Healer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Bastion_Healer::CMonster_Bastion_Healer(const CMonster_Bastion_Healer& _rhs)
	: CActor(_rhs)
{
}

HRESULT CMonster_Bastion_Healer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_Healer::NativeConstruct(const _uint _iSceneID, void* _pArg)
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

_int CMonster_Bastion_Healer::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

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

_int CMonster_Bastion_Healer::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	/* State FSM Late Update */
	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;

	// 무기 레잇업뎃
	if (m_pCurWeapon)
	{
		iProgress = m_pCurWeapon->LateTick(_dDeltaTime);
		if (NO_EVENT != iProgress)
			return iProgress;
	}

	return _int();
}

HRESULT CMonster_Bastion_Healer::Render()
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
		//if (FAILED(m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE))) return E_FAIL;

		if (FAILED(m_pModel->Render(i, 0)))
			return E_FAIL;
	}
#ifdef _DEBUG
	Render_Debug();
	//m_pColliderCom->Render(L"Camera_Silvermane");
#endif
	return S_OK;
}

HRESULT CMonster_Bastion_Healer::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.8f;
	transformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(transformDesc);
	_float4 vPosition = { 3.f, 0.f, 3.f, 1.f };

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));


	// 모델
	if (FAILED(SetUp_Components(m_iSceneID, L"Model_Bastion_Healer_Bin", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	/* for. Collider */
	//CCapsuleCollider::CAPSULEDESC CapDesc;
	//XMStoreFloat4x4(&CapDesc.matTransform, XMMatrixIdentity());
	//CapDesc.pParent = this;

	//CPhysicsXSystem::COLDESC PhyDesc;
	//PhyDesc.bGravity = false;
	//PhyDesc.bKinematic = false;
	//PhyDesc.eType = CPhysicsXSystem::ACTORTYPE::ACTOR_DYNAMIC;

	//CapDesc.tColDesc = PhyDesc;
	//if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_TEST_YM, L"Proto_Component_CapsuleCollider", L"Com_CapsuleCollider", (CComponent**)&m_pColliderCom, &CapDesc)))
	//{
	//	return E_FAIL;
	//}

	// 스테이트 컨트롤러
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	m_AanimDesc.pModel = m_pModel;
	m_AanimDesc.pTransform = m_pTransform;

	//Anim FSM
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Animator", (CComponent**)&m_pAnimator, &m_AanimDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_Healer::Ready_Weapon(void)
{
	CHierarchyNode* pWeaponBone = m_pModel->Get_BoneMatrix("weapon_1_end");
	CWeapon* pWeapon = nullptr;

	//RetributionBlade
	pWeapon = CStaff::Create(m_pDevice, m_pDeviceContext);
	pWeapon->NativeConstruct(m_iSceneID, pWeaponBone);
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	m_umapWeapons.emplace(L"Staff", pWeapon);
	m_pCurWeapon = pWeapon;

	return S_OK;
}

HRESULT CMonster_Bastion_Healer::Ready_AnimFSM(void)
{
#pragma region Default
	CAnimation* pAnimation = m_pModel->Get_Animation("A_Idle");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, TRUE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Death");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_DEATH, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion
#pragma region Attack
	pAnimation = m_pModel->Get_Animation("A_Cast_Protect");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_CAST_PROTECT, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
	pAnimation = m_pModel->Get_Animation("A_Attack_Blind");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_ATTACK_BLIND, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion
#pragma region Hit
	pAnimation = m_pModel->Get_Animation("A_Flinch_Left");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_FLINCH_LEFT, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ)))
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
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_STUN_ST, (_uint)ANIM_TYPE::A_HEAD, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Stun");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_STUN, (_uint)ANIM_TYPE::A_STUN_ST, pAnimation, TRUE, TRUE, TRUE, ERootOption::XYZ)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Stun_End");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_STUN_ED, (_uint)ANIM_TYPE::A_STUN_ST, pAnimation, TRUE, TRUE, FALSE, ERootOption::XYZ)))
		return E_FAIL;
#pragma endregion
#pragma region Turn
	pAnimation = m_pModel->Get_Animation("A_Trun_45_Right");
	pAnimation = m_pModel->Get_Animation("A_Turn_45_Left");
	pAnimation = m_pModel->Get_Animation("A_Turn_90_Left");
	pAnimation = m_pModel->Get_Animation("A_Turn_90_Right");
	pAnimation = m_pModel->Get_Animation("A_Turn_135_Left");
	pAnimation = m_pModel->Get_Animation("A_Turn_135_Right");
	pAnimation = m_pModel->Get_Animation("A_Turn_180_Left");
	pAnimation = m_pModel->Get_Animation("A_Turn_180_Right");
#pragma endregion

#pragma region  Set Any Entry Animation
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_IDLE);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_DEATH);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_CAST_PROTECT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_ATTACK_BLIND);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_FLINCH_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ST);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ED);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_BWD_ST);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_WALK_BWD_ED);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_STUN_ST);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_STUN_ED);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_135_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_135_RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_180_LEFT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_180_RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_90_RIGHT);
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_TURN_90_RIGHT);
#pragma endregion

#pragma region  Auto Change Anim
	/* ##1.끝나는 애님 ##2.루트할 애님  */
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ST, (_uint)ANIM_TYPE::A_WALK_FWD);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD, (_uint)ANIM_TYPE::A_WALK_FWD_ED);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_FWD_ED, (_uint)ANIM_TYPE::A_IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_BWD_ST, (_uint)ANIM_TYPE::A_WALK_BWD);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_BWD, (_uint)ANIM_TYPE::A_WALK_BWD_ED);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_WALK_BWD_ED, (_uint)ANIM_TYPE::A_IDLE);
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
	if (FAILED(m_pStateController->Add_State(L"Chaser", CBastion_Healer_Chaser::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Player Chaser End */
	if (FAILED(m_pStateController->Add_State(L"Chaser_End", CBastion_Healer_Chaser_End::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	/* for. Attack*/
	if (FAILED(m_pStateController->Add_State(L"Attack", CBastion_Healer_Attack::Create(m_pDevice, m_pDeviceContext))))
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
	__super::Free();

	Safe_Release(m_pStateController);
	Safe_Release(m_pModel);
	Safe_Release(m_pAnimator);
}
