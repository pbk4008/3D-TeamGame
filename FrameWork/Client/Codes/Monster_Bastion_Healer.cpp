#include "pch.h"
#include "Monster_Bastion_Healer.h"
#include "Animation.h"
#include "Staff.h"
/* for. FSM */
#include "Bastion_Healer_State.h"
#include "Bastion_Healer_Idle.h"

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

	// ¹«±â ¾÷µ«
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

	// ¹«±â ·¹ÀÕ¾÷µ«
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

	return S_OK;
}

HRESULT CMonster_Bastion_Healer::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.7f;
	transformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(transformDesc);
	_float4 vPosition = { 3.f, 0.f, 3.f, 1.f };

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));


	// ¸ðµ¨
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

	// ½ºÅ×ÀÌÆ® ÄÁÆ®·Ñ·¯
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_YM, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	m_AanimDesc.pModel = m_pModel;
	m_AanimDesc.pTransform = m_pTransform;

	//Anim FSM
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_YM, L"Proto_Component_Animator", L"Animator", (CComponent**)&m_pAnimator, &m_AanimDesc)))
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

#pragma endregion
#pragma region  Set Any Entry Animation
	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_IDLE);
#pragma endregion

#pragma region  Auto Change Anim
	/* ##1.³¡³ª´Â ¾Ö´Ô ##2.·çÆ®ÇÒ ¾Ö´Ô  */
	//m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::A_BATTLECRY, (_uint)ANIM_TYPE::A_BATTLECRY_ED);

#pragma endregion

#pragma region Anim to Anim Connect
	//if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::A_IDLE, (_uint)ANIM_TYPE::A_WALK_FWD_ED, FALSE)))
	//	return E_FAIL;
#pragma endregion
	return S_OK;
}

HRESULT CMonster_Bastion_Healer::Ready_StateFSM(void)
{
	/* for. Idle */
	if (FAILED(m_pStateController->Add_State(L"Idle", CBastion_Healer_Idle::Create(m_pDevice, m_pDeviceContext))))
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
	return E_NOTIMPL;
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
