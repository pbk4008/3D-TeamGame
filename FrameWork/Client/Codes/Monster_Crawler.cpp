#include "pch.h"
#include "Monster_Crawler.h"

#include "UI_Monster_Panel.h"

#include "Crawler_Idle.h"
#include "Crawler_Walk.h"
#include "Crawler_Attack.h"
#include "Crawler_Ricochet.h"
#include "Crawler_Death.h"
#include "Crawler_Flinch_Left.h"

CMonster_Crawler::CMonster_Crawler(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Crawler::CMonster_Crawler(const CMonster_Crawler& _rhs)
	:CActor(_rhs)
{
}

void CMonster_Crawler::Clear_Physix()
{

	m_pCollider->Remove_Actor();
	m_pCharacterController->Remove_CCT();
	Safe_Release(m_pCollider);
	Safe_Release(m_pCharacterController);

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
	if (FAILED(SetUp_Components())) return E_FAIL;
	if (FAILED(Set_Animation_FSM())) return E_FAIL;
	if (FAILED(Set_State_FSM())) return E_FAIL;
	if (FAILED(Ready_Weapone())) return E_FAIL;

	/*_vector Pos = { 0.f, 1.f, 3.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);*/
	_float3 vPoint = (*(_float3*)_pArg);

	if (FAILED(Set_SpawnPosition(vPoint)))
		return E_FAIL;

	//MonsterBar Panel
	//CUI_Monster_Panel::PANELDESC Desc;
	//Desc.pTargetTransform = m_pTransform;
	//Desc.iEnemyTag = CUI_Monster_Panel::Enemy::CRAWLER;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
	//	(CGameObject**)&m_pPanel)))
	//	return E_FAIL;

	//m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	return S_OK;
}

_int CMonster_Crawler::Tick(_double _dDeltaTime)
{	
	m_pTransform->Set_Velocity(XMVectorZero());

	//m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	//m_pModelCom->Update_CombinedTransformationMatrix(_dDeltaTime);
	//m_pAnimControllerCom->Tick(_dDeltaTime);
	_int iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (g_pGameInstance->getkeyDown(DIK_NUMPAD5))
	{
		--m_fHp;
		//m_pPanel->Set_HpBar(m_fMaxHp, m_fHp);
		cout << m_fHp << endl;

		m_pStateController->Change_State(L"Flinch_Left");
	}
	
	m_pCollider->Tick(_dDeltaTime);

	Fall(_dDeltaTime);
	m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());


	//if (0.f >= m_fHp)
	//{
	//	m_pStateController->Change_State(L"Death");
	//}

	//m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	return 0;
}

_int CMonster_Crawler::LateTick(_double _dDeltaTime)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	m_pCharacterController->Update_OwnerTransform();

	_int iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
	{
		return iProgress;
	}


	return 0;
}

HRESULT CMonster_Crawler::Render()
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

void CMonster_Crawler::OnTriggerEnter(CCollision& collision)
{
	if ((_uint)GAMEOBJECT::WEAPON == collision.pGameObject->getTag() && g_pObserver->IsAttack() && m_fHp > 0)
	{
		m_fHp -= 2;
		m_pStateController->Change_State(L"Flinch_Left");
		cout << "È÷Æ®ÇØºÎ·¶À¸" << endl;
	}

}

const _int CMonster_Crawler::Fall(const _double& _dDeltaTime)
{
	if (m_isFall)
	{
		_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		if (-10.f < XMVectorGetY(svPos))
		{
			m_pTransform->Add_Velocity(XMVectorSet(0.f, -9.8f * (_float)_dDeltaTime, 0.f, 0.f));
		}
	}
	return _int();
}

HRESULT CMonster_Crawler::SetUp_Components()
{
	//_float x = _float(rand()% 3);
	_float z = _float(rand()% 10) + 3.f;

	_vector Pos = { 0.f, 10.f, z, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);

	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);
	
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_Crawler", L"Com_Model", (CComponent**)&m_pModelCom)))
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

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimatorCom, &tDesc)))
		return E_FAIL;

	CCharacterController::DESC tCCTDesc;
	tCCTDesc.fHeight = 1.f;
	tCCTDesc.fRadius = 0.5f;
	tCCTDesc.fContactOffset = tCCTDesc.fRadius * 0.1f;
	tCCTDesc.fStaticFriction = 0.5f;
	tCCTDesc.fDynamicFriction = 0.5f;
	tCCTDesc.fRestitution = 0.f;
	tCCTDesc.pGameObject = this;
	tCCTDesc.vPosition = { 0.f, 0.f, 0.f };

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", L"CharacterController", (CComponent**)&m_pCharacterController, &tCCTDesc)))
		return E_FAIL;
	m_pCharacterController->setOwnerTransform(m_pTransform);

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
	tSphereCol.fRadius = 0.3f;
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SphereCollider", L"Collider", (CComponent**)&m_pCollider, &tSphereCol)))
		return E_FAIL;

	_matrix smatPviot = XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.5f, 0.5f);
	m_pCollider->setPivotMatrix(smatPviot);

	m_pCollider->setActive(false);

	return S_OK;
}

HRESULT CMonster_Crawler::Set_Animation_FSM()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Idle_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(IDLE, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Walk_Fwd_Stop_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_FWD, HEAD, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Attack_R1_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(ATTACK_R1, HEAD, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Death_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(DEATH, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Ricochet_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(RICOCHET, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Flinch_Right_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(FLINCH_RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Flinch_Left_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(FLINCH_LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	//³Ë¹é
	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Knockback_Start_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(KNOCKBACK_START, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v1.ao|A_Knockback_End_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(KNOCKBACK_END, KNOCKBACK_START, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, KNOCKBACK_END, false)))	return E_FAIL;
	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, ATTACK_R1, true)))	return E_FAIL;


	m_pAnimatorCom->Insert_AnyEntryAnimation(IDLE);
	m_pAnimatorCom->Insert_AnyEntryAnimation(WALK_FWD);
	m_pAnimatorCom->Insert_AnyEntryAnimation(ATTACK_R1);
	m_pAnimatorCom->Insert_AnyEntryAnimation(DEATH);
	m_pAnimatorCom->Insert_AnyEntryAnimation(FLINCH_RIGHT);
	m_pAnimatorCom->Insert_AnyEntryAnimation(FLINCH_LEFT);
	m_pAnimatorCom->Insert_AnyEntryAnimation(KNOCKBACK_START);
	m_pAnimatorCom->Insert_AnyEntryAnimation(RICOCHET);

	m_pAnimatorCom->Change_Animation(IDLE);

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
		static_cast<CMonster_FSM*>(pair.second)->Set_Model(m_pModelCom);
		static_cast<CMonster_FSM*>(pair.second)->Set_Animator(m_pAnimatorCom);
	}

	m_pStateController->Change_State(L"Idle");

	return S_OK;
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
	if (m_pCollider != nullptr && m_pCharacterController != nullptr)
	{
		Safe_Release(m_pCollider);
		Safe_Release(m_pCharacterController);
	}

	Safe_Release(m_pPanel);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimatorCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
