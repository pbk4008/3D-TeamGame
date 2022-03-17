#include "pch.h"
#include "Monster_Bastion_Sword.h"
#include "Animation.h"
#include "Stargazer.h"
#include "ShieldBreaker.h"
#include "HierarchyNode.h"

CMonster_Bastion_Sword::CMonster_Bastion_Sword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Bastion_Sword::CMonster_Bastion_Sword(const CMonster_Bastion_Sword& _rhs)
	:CActor(_rhs)
{
}

HRESULT CMonster_Bastion_Sword::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_Sword::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(Set_Animation_FSM()))
		return E_FAIL;

	if (FAILED(Set_Weapon()))
		return E_FAIL;


	_vector Pos = { 0.f, 0.f, 10.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);
	return S_OK;
}

_int CMonster_Bastion_Sword::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}

	m_pAnimator->Tick(_dDeltaTime);

	if (g_pGameInstance->getkeyDown(DIK_SPACE))
	{
		if (FAILED(m_pAnimator->Change_Animation((_uint)ANIM_TYPE::RUN_START)))
			return -1;
	}
	if (g_pGameInstance->getkeyDown(DIK_RETURN))
	{
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)ANIM_TYPE::ATTACK_JUMPSTART)))
			return -1;
	}

	m_pWeapon->Tick(_dDeltaTime);

	return 0;
}

_int CMonster_Bastion_Sword::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	m_pWeapon->LateTick(_dDeltaTime);
	return 0;
}

HRESULT CMonster_Bastion_Sword::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"MainCamera");

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
		m_pModelCom->Render(i, 0);

	return S_OK;
}

HRESULT CMonster_Bastion_Sword::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_Bastion_Sword", L"Model", (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModelCom;
	tDesc.pTransform = m_pTransform;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)& m_pAnimator, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_Sword::Set_Animation_FSM()
{
	//애니메이션 생성
	CAnimation* pAnim = m_pModelCom->Get_Animation("Idle");
	//생성 하면서 연결(연결 할애, 연결 당할애, 애니메이션, 루트 애님, 트랜스폼(루트애니메이션할때 찐으로 따라감), 루프, 옵션)
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;

	////////////////////
	pAnim = m_pModelCom->Get_Animation("Run_Start");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::IDLE, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Run_Loop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_START, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Run_Stop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::RUN_LOOP, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	/////////////////////////////////////////////
	pAnim = m_pModelCom->Get_Animation("Attack_JumpStart");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_JUMPSTART, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Attack_JumpLoop");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_JUMPLOOP, (_uint)ANIM_TYPE::ATTACK_JUMPSTART, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("Attack_JumpEnd");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_JUMPEND, (_uint)ANIM_TYPE::ATTACK_JUMPLOOP, pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	//////////////////////////////////
	//애니메이션 연결(연결 당할 애, 연결할 애, 쌍방으로 연결할지 안할지)
	if(FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE , (_uint)ANIM_TYPE::RUN_END,false)))
		return E_FAIL;

	if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::ATTACK_JUMPEND, false)))
		return E_FAIL;

	//자동으로 돌릴 애들(끝나는애, 끝나고 시작할 애)
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::RUN_LOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_END);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_JUMPSTART, (_uint)ANIM_TYPE::ATTACK_JUMPLOOP);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_JUMPLOOP, (_uint)ANIM_TYPE::ATTACK_JUMPEND);
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_JUMPEND, (_uint)ANIM_TYPE::IDLE);

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::ATTACK_JUMPSTART);


	//애니메이션 체인지(바꿀 애)
	m_pAnimator->Change_Animation((_uint)ANIM_TYPE::IDLE);

	return S_OK;
}

HRESULT CMonster_Bastion_Sword::Set_Weapon()
{
	m_pWeapon = static_cast<CStargazer*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_Weapon_Stargazer"));

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
	m_pWeapon->Set_OwnerPivotMatrix(m_pModelCom->Get_PivotMatrix());

	return S_OK;
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
	Safe_Release(m_pModelCom);
	Safe_Release(m_pAnimator);

	Safe_Release(m_pWeapon);
}
