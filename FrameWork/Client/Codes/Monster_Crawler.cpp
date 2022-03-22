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
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	if (FAILED(Set_Animation_FSM()))
	{
		return E_FAIL;
	}

	if (FAILED(Set_State_FSM()))
	{
		return E_FAIL;
	}

	_vector Pos = { 0.f, 0.f, 9.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);


	//MonsterBar Panel
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;
	Desc.iEnemyTag = CUI_Monster_Panel::Enemy::CRAWLER;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Monster_Panel", &Desc,
		(CGameObject**)&m_pPanel)))
		return E_FAIL;

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	return S_OK;
}

_int CMonster_Crawler::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}
	
	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());

	m_pModelCom->Update_CombinedTransformationMatrix(_dDeltaTime);
	//m_pAnimControllerCom->Tick(_dDeltaTime);
	_int iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
	{
		return iProgress;
	}

	if (g_pGameInstance->getkeyDown(DIK_NUMPAD5))
	{
		m_pStateController->Change_State(L"Flinch_Left");
	}

	if (g_pGameInstance->getkeyDown(DIK_NUMPAD6))
	{
		m_pStateController->Change_State(L"Death");
	}

	m_pPanel->Set_TargetWorldMatrix(m_pTransform->Get_WorldMatrix());
	
	return 0;
}

_int CMonster_Crawler::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
	}
	
	_int iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
	{
		return iProgress;
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

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
		//m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModelCom->Render(i, 0);
	}

	return S_OK;
}

HRESULT CMonster_Crawler::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);
	
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_Crawler", L"Com_Model", (CComponent**)&m_pModelCom)))
	{
		return E_FAIL;
	}

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModelCom;
	tDesc.pTransform = m_pTransform;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimatorCom, &tDesc)))
	{
		return E_FAIL;
	}

	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Crystal_Crawler"), 0);
	
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_StateController", L"Com_StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	return S_OK;
}

HRESULT CMonster_Crawler::Set_Animation_FSM()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Idle_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(IDLE, HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Walk_Fwd_Stop_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(WALK_FWD, HEAD, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Attack_R1_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(ATTACK_R1, HEAD, pAnim, true, true, true, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Death_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(DEATH, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Ricochet_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(RICOCHET, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Flinch_Right_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(FLINCH_RIGHT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Flinch_Left_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(FLINCH_LEFT, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	//³Ë¹é
	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Knockback_Start_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(KNOCKBACK_START, HEAD, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Knockback_End_CrystalCrawler");
	if (FAILED(m_pAnimatorCom->Insert_Animation(KNOCKBACK_END, KNOCKBACK_START, pAnim, true, true, false, ERootOption::XYZ, true)))
		return E_FAIL;

	if (FAILED(m_pAnimatorCom->Connect_Animation(IDLE, KNOCKBACK_END, false)))
		return E_FAIL;


	m_pAnimatorCom->Set_UpAutoChangeAnimation(KNOCKBACK_START, KNOCKBACK_END);


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
	Safe_Release(m_pPanel);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimatorCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
