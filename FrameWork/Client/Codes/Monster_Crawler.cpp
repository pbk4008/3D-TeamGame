#include "pch.h"
#include "Monster_Crawler.h"

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

HRESULT CMonster_Crawler::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	_vector Pos = { -3.f, 0.f, 0.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);


	//MonsterBar Panel
	CUI_Monster_Panel::PANELDESC Desc;
	Desc.pTargetTransform = m_pTransform;

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
	
	m_pColliderCom->Update(m_pTransform->Get_WorldMatrix());

	//m_pAnimControllerCom->Tick(_dDeltaTime);

	/*if (g_pGameInstance->getkeyDown(DIK_NUMPAD9))
	{
		++itest;
	}*/

	//m_pAnimControllerCom->SetUp_NextAnimation(itest, true);
	//m_pAnimControllerCom->Set_RootMotion(true, false);

	return 0;
}

_int CMonster_Crawler::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
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

#ifdef _DEBUG
	m_pColliderCom->Render(L"MainCamera");
#endif // _DEBUG

	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"MainCamera");

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

	/*if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_AnimationController", L"Com_AnimationController", (CComponent**)&m_pAnimControllerCom)))
	{
		return E_FAIL;
	}*/

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimatorCom)))
	{
		return E_FAIL;
	}

	CCapsuleCollider::CAPSULEDESC CapDesc;
	XMStoreFloat4x4(&CapDesc.matTransform, XMMatrixIdentity());
	CapDesc.pParent = this;

	CPhysicsXSystem::COLDESC PhyDesc;
	PhyDesc.bGravity = false;
	PhyDesc.bKinematic = false;
	PhyDesc.eType = CPhysicsXSystem::ACTORTYPE::ACTOR_DYNAMIC;
	CapDesc.tColDesc = PhyDesc;
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_CapsuleCollider", L"Com_CapsuleCollider", (CComponent**)&m_pColliderCom, &CapDesc)))
	{
		return E_FAIL;
	}

	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Crystal_Crawler"), 0);
	
	//m_pAnimControllerCom->Set_GameObject(this);
	//m_pAnimControllerCom->Set_Model(m_pModelCom);
	//m_pAnimControllerCom->Set_Transform(m_pTransform);

	return S_OK;
}

HRESULT CMonster_Crawler::Animation_Setting()
{
	//CAnimation* pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Idle_CrystalCrawler");
	//if (FAILED(m_pAnimatorCom->Insert_Animation(L"Idle", L"Head", pAnim, false, false, true, ERootOption::XYZ)))
	//	return E_FAIL;

	//pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Walk_Fwd_Stop_CrystalCrawler");
	//if (FAILED(m_pAnimatorCom->Insert_Animation(L"Walk_Fwd", L"Idle", pAnim, true, false, true, ERootOption::XYZ, true)))
	//	return E_FAIL;

	//pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Walk_Fwd_Stop_CrystalCrawler");
	//if (FAILED(m_pAnimatorCom->Insert_Animation(L"Walk_Bwd", L"Idle", pAnim, true, false, true, ERootOption::XYZ, true)))
	//	return E_FAIL;

	//pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Walk_Fwd_Stop_CrystalCrawler");
	//if (FAILED(m_pAnimatorCom->Insert_Animation(L"Walk_Left", L"Idle", pAnim, true, false, true, ERootOption::XYZ, true)))
	//	return E_FAIL;

	//pAnim = m_pModelCom->Get_Animation("SK_Crystal_Crawler_v3.ao|A_Walk_Fwd_Stop_CrystalCrawler");
	//if (FAILED(m_pAnimatorCom->Insert_Animation(L"Walk_Right", L"Idle", pAnim, true, false, true, ERootOption::XYZ, true)))
	//	return E_FAIL;


	//m_pAnimatorCom->Set_UpAutoChangeAnimation(L"RunStart", L"RunLoop");
	//m_pAnimatorCom->Set_UpAutoChangeAnimation(L"RunLoop", L"RunStop");

	//m_pAnimatorCom->Change_Animation(L"RunStart");

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

CGameObject* CMonster_Crawler::Clone(void* _pArg)
{
	CMonster_Crawler* pInstance = new CMonster_Crawler(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("Failed to Creating Clone CMonster_Crawler");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Crawler::Free()
{
	Safe_Release(m_pModelCom);
	__super::Free();
}
