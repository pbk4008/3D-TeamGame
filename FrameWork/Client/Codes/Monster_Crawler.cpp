#include "pch.h"
#include "Monster_Crawler.h"

#include "UI_Monster_Panel.h"

#include "Effect_HitParticle.h"
#include "Effect_HitFloating.h"

#include "Animation.h"
#include "Crawler_Idle.h"
#include "Crawler_Walk.h"
#include "Crawler_Attack.h"
#include "Crawler_Ricochet.h"
#include "Crawler_Death.h"
#include "Crawler_Flinch_Left.h"

#include "Stage1.h"
#include "Stage2.h"

#include "MainApp.h"

CMonster_Crawler::CMonster_Crawler(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Crawler::CMonster_Crawler(const CMonster_Crawler& _rhs)
	:CActor(_rhs)
	, m_pModelCom(_rhs.m_pModelCom)
	, m_pStateController(_rhs.m_pStateController)
	, m_pCharacterController(_rhs.m_pCharacterController)
	, m_pPanel(_rhs.m_pPanel)
	, m_pCollider(_rhs.m_pCollider)
{
	Safe_AddRef(m_pModelCom);
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
		if (FAILED(Set_SpawnPosition(vPoint)))
			return E_FAIL;
	}
	if (FAILED(SetUp_Components())) return E_FAIL;
	if (FAILED(Set_Animation_FSM())) return E_FAIL;
	if (FAILED(Set_State_FSM())) return E_FAIL;
	if (FAILED(Ready_Weapone())) return E_FAIL;

	/*_vector Pos = { 0.f, 1.f, 3.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);*/

	if (nullptr != _pArg)
	{
		_float3 vPoint = (*(_float3*)_pArg);

		if (FAILED(Set_SpawnPosition(vPoint)))
			return E_FAIL;
	}
	else
	{
		_vector Pos = { 0.f, 1.f, 3.f, 1.f };
		m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);
	}
	//MonsterBar Panel
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::CRAWLER;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;
	//레이어에도 넣어주고 변수도 가지고 있기때문에 Add_Ref필수!!
	Safe_AddRef(m_pPanel);

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	m_bIsFall = true;

	m_iObectTag = (_uint)GAMEOBJECT::MONSTER_CRYSTAL;

	m_fMaxHp = 2.f;
	m_fCurrentHp = m_fMaxHp;

	m_fMaxGroggyGauge = 3.f;
	m_fGroggyGauge = 0.f;

	m_pPanel->Set_HpBar(Get_HpRatio());
	m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());

	m_iObectTag = (_uint)GAMEOBJECT::MONSTER_CRYSTAL;
	setActive(false);

	return S_OK;
}

_int CMonster_Crawler::Tick(_double _dDeltaTime)
{	
	m_pCollider;
	int a = 10;
	//나중에지울코드
	if (!m_bFirst)
	{
		m_pPanel->Set_Show(true);
	}
	m_pTransform->Set_Velocity(XMVectorZero());

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	_int iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (0 >= m_fCurrentHp)
	{
		m_bDead = true;
		m_pStateController->Change_State(L"Death");
	}

	
	/*if (g_pGameInstance->getkeyDown(DIK_NUMPAD5))
	{
		--m_fHp;
		m_pPanel->Set_HpBar(m_fMaxHp, m_fHp);
		cout << m_fHp << endl;

		m_pStateController->Change_State(L"Flinch_Left");
	}*/
	
	m_pCollider->Tick(_dDeltaTime);

	if (m_bIsFall)
		m_pTransform->Fall(_dDeltaTime);

	if (DEATH == m_pAnimatorCom->Get_CurrentAnimNode())
	{
		if (m_pAnimatorCom->Get_CurrentAnimation()->Is_Finished())
		{
			Set_Remove(true);
		}
	}

	m_pCharacterController->Move(_dDeltaTime, m_pTransform->Get_Velocity());

	return 0;
}

_int CMonster_Crawler::LateTick(_double _dDeltaTime)
{
	m_pCollider;
	int a = 10;
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
	m_pCollider;
	int a = 10;
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
	if (true == g_pObserver->IsAttack()) //플레이어공격일때
	{
		m_bFirstHit = true; //딱 한번 true로 변경해줌

		if (true == m_bFirstHit)
		{
			m_pPanel->Set_BackUIGapY(1.f);
		}

		if ((_uint)GAMEOBJECT::WEAPON == collision.pGameObject->getTag())
		{

			m_fCurrentHp -= 2;
			m_fGroggyGauge += 2; //TODO::수치정해서바꿔줘야됨

			m_pPanel->Set_HpBar(Get_HpRatio());

			if (false == m_bGroggy)
			{
				//그로기 아닐때만 증가할수있게
				m_pPanel->Set_GroggyBar(Get_GroggyGaugeRatio());
				m_pStateController->Change_State(L"Flinch_Left");
			}

			CEffect_HitParticle* pEffect = (CEffect_HitParticle*)g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hit")->front();
			_vector Mypos = m_pTransform->Get_State(CTransform::STATE_POSITION);
			Mypos = XMVectorSetY(Mypos, XMVectorGetY(Mypos) + 1.f);
			pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, Mypos);
			pEffect->setActive(true);
			pEffect->Set_Reset(true);

			CEffect_HitFloating* pEffect1 = (CEffect_HitFloating*)g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Floating")->front();
			_vector Mypos1 = m_pTransform->Get_State(CTransform::STATE_POSITION);
			Mypos1 = XMVectorSetY(Mypos1, XMVectorGetY(Mypos1) + 1.f);
			pEffect1->Get_Transform()->Set_State(CTransform::STATE_POSITION, Mypos1);
			pEffect1->setActive(true);
			pEffect1->Set_Reset(true);
		}

		else
		{

		}
	}

}

void CMonster_Crawler::OnTriggerExit(CCollision& collision)
{
	if (true == g_pObserver->IsAttack()) //플레이어공격일때
	{
		if ((_uint)GAMEOBJECT::WEAPON == collision.pGameObject->getTag())
		{
			if(m_bDead)
				g_pMainApp->FreezeTime();
		}
	}
}

void CMonster_Crawler::Set_IsAttack(const _bool _isAttack)
{
	m_IsAttack = _isAttack;
}

HRESULT CMonster_Crawler::SetUp_Components()
{
	//_float x = _float(rand()% 3);
	_float z = _float(rand()% 10) + 3.f;

	//_vector Pos = { 0.f, 10.f, z, 1.f };
	//m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);

	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);
	
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Crawler", L"Com_Model", (CComponent**)&m_pModelCom)))
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

	//m_pCollider->setActive(true);

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

	//넉백
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
	m_pCollider;
	__super::Free();
	if (m_pCollider != nullptr && m_pCharacterController != nullptr)
	{
		Safe_Release(m_pCollider);
		Safe_Release(m_pCharacterController);
	}

	Safe_Release(m_pPanel);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimatorCom);
	Safe_Release(m_pModelCom);


}
