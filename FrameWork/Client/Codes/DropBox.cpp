#include "pch.h"
#include "DropBox.h"

#include "DropManager.h"

#include "DropBox_State.h"
#include "DropBox_Idle.h"
#include "DropBox_Open.h"

CDropBox::CDropBox(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CInteractableObject(_pDevice, _pDeviceContext)
	, m_pAnimationController(nullptr)
	, m_pAnimator(nullptr)
	, m_pStateController(nullptr)
	, m_openElapsed(0.0f)
	, m_interactDist(5.f)
	, m_bBoxOpened(false)
	, m_bOpenAnimEnd(false)
	, m_bFocus(false)
{
}

CDropBox::CDropBox(const CDropBox& _rhs)
	: CInteractableObject(_rhs)
	, m_openElapsed(_rhs.m_openElapsed)
	, m_interactDist(_rhs.m_interactDist)
	, m_bBoxOpened(_rhs.m_bBoxOpened)
	, m_bOpenAnimEnd(_rhs.m_bOpenAnimEnd)
	, m_bFocus(_rhs.m_bFocus)
{
}

HRESULT CDropBox::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropBox::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Set_Animation_FSM()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_tDesc = (*(MABOBJECT*)_pArg);
	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_tDesc.WorldMat));

	m_pPlayer = m_pPlayer = *g_pGameInstance->getObjectList(_iSceneID, L"Layer_Silvermane")->begin();
	assert(m_pPlayer);

	m_iObectTag = (_uint)GAMEOBJECT::DROP_BOX;

	setActive(true);

	//박스위치에 파티클생성
	_matrix matPos = m_pTransform->Get_WorldMatrix();
	matPos.r[3] = XMVectorSetY(matPos.r[3], XMVectorGetY(matPos.r[3]) - 0.5f);
	Active_Effect_Target((_uint)EFFECT::BOX , matPos);

	return S_OK;
}

_int CDropBox::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pCollider->Tick(_dDeltaTime);

	/* State FSM Update */
	iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (true == m_bBoxOpened && 0 < m_dropList.size())
	{
		m_dropElapsed += (_float)_dDeltaTime; /* drop time */
		CLEAR_QUEST(L"T_HUD_Find_DropBox");
		if (m_dropElapsed >= m_dropDelay) 
		{
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Item_Drop);
			g_pGameInstance->Play_Shot(L"Drop_Item_2", CSoundMgr::CHANNELID::Item_Drop);
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Item_Drop, 5.5f);
			m_dropElapsed = 0.f;
			m_pCollider->Remove_ActorFromScene();
			m_dropDelay = (_float)MathUtils::ReliableRandom(m_dropDelayMin, m_dropDelayMax);
			CDropManager::GetInstance()->DropItem(m_dropList.back(), m_pTransform->Get_State(CTransform::STATE_POSITION), EScatterType::Cone, Get_Transform());
			m_dropList.pop_back();
		}
	}

	return _int();
}

_int CDropBox::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;

	/* State FSM Late Update */
	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CDropBox::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_CombinedMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 0)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CDropBox::Ready_Components()
{
	/* for. Transform Com */
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_DropBox", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	/* for. Collider Com */
	CCollider::DESC tColliderDesc;
	tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.isSceneQuery = true;
	tColliderDesc.isTrigger = false;
	tColliderDesc.pGameObject = this;
	tColliderDesc.isGravity = false;

	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { 2.0f, 1.f, 1.f };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;
	_matrix smatPivot = XMMatrixTranslation(0.f, tBoxColliderDesc.vScale.y * 0.5f, 0.f);
	m_pCollider->setPivotMatrix(smatPivot);
	m_pCollider->setShapeLayer((_uint)ELayer::ItemBox);

	/* for. Animator Com */
	matPivot = XMMatrixIdentity();
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModel;
	tDesc.pTransform = m_pTransform;
	tDesc.eType = CAnimationController::EType::CharacterController;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimator, &tDesc)))
		return E_FAIL;

	/* for. State Controller */
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	/* for. Anim Controller */
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);
	m_pAnimationController->Set_MoveSpeed(1.f);

	return S_OK;
}

HRESULT CDropBox::Ready_States()
{
	if(FAILED(m_pStateController->Add_State(L"DropBox_Open", CDropBox_Open::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CDropBox_State*>(pair.second)->Set_Object(this);
		static_cast<CDropBox_State*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CDropBox_State*>(pair.second)->Set_Model(m_pModel);
		static_cast<CDropBox_State*>(pair.second)->Set_AnimationController(m_pAnimationController);
		static_cast<CDropBox_State*>(pair.second)->Set_Animator(m_pAnimator);
	}
	m_pStateController->Change_State(L"DropBox_Open");

	return S_OK;
}

HRESULT CDropBox::Set_Animation_FSM()
{
	CAnimation* pAnimation =  m_pModel->Get_Animation("Take 001");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_OPEN, (_uint)ANIM_TYPE::A_HEAD, pAnimation, FALSE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_OPEN);

	return S_OK;
}

void CDropBox::Open(void)
{
	m_dropList = GetDropList();
}

std::vector<CItemData> CDropBox::GetDropList(void)
{
	std::vector<CItemData> dropList;

	CItemData item1;
	item1.iconTexName = L"T_Weapon_Hammer_2H_Player_Fury";
	item1.equipmentGrade = EEquipmentGrade::Epic;
	item1.ItemType = EItemType::Equipment;
	item1.equipmentType = EEquipmentType::Weapon;
	item1.szItemName = L"JinSung's Sword";
	item1.iMainStat = 52;
	item1.iLevel = 10;
	item1.weaponData.weaponName = L"Fury";
	item1.equipmentName = EEquipmentName::Fury;
	item1.szStatusName = L"T_2H_DPS_147";
	dropList.emplace_back(item1);

	CItemData item2;
	item2.iconTexName = L"T_Weapon_Hammer_2H_Player_Harbinger";
	item2.equipmentGrade = EEquipmentGrade::Rare;
	item2.ItemType = EItemType::Equipment;
	item2.equipmentType = EEquipmentType::Weapon;
	item2.szItemName = L"SuBin's Sword";
	item2.iMainStat = 20;
	item2.iLevel = 16;
	item2.szWeaponName = L"2";
	item2.weaponData.weaponName = L"Harbinger";
	item2.equipmentName = EEquipmentName::Harbinger;
	item2.szStatusName = L"T_2H_DPS_132";
	dropList.emplace_back(item2);

	CItemData item3;
	item3.iconTexName = L"T_Weapon_Sword_1H_Player_Crossblade";
	item3.equipmentGrade = EEquipmentGrade::Rare;
	item3.ItemType = EItemType::Equipment;
	item3.equipmentType = EEquipmentType::Weapon;
	item3.szItemName = L"YoungMo's Sword";
	item3.iMainStat = 20;
	item3.iLevel = 16;
	item3.szWeaponName = L"3";
	item3.weaponData.weaponName = L"Crossblade";
	item3.equipmentName = EEquipmentName::Crossblade;
	item3.szStatusName = L"T_2H_DPS_135";
	dropList.emplace_back(item3);

	CItemData item4;
	item4.iconTexName = L"T_Weapon_Sword_1H_Player_Dragon";
	item4.equipmentGrade = EEquipmentGrade::Epic;
	item4.ItemType = EItemType::Equipment;
	item4.equipmentType = EEquipmentType::Weapon;
	item4.szItemName = L"BeangGyu's Sword";
	item4.iMainStat = 20;
	item4.iLevel = 16;
	item4.szWeaponName = L"4";
	item4.weaponData.weaponName = L"Dragon";
	item4.equipmentName = EEquipmentName::Dragon;
	item4.szStatusName = L"T_1H_DPS_120";
	dropList.emplace_back(item4);

	CItemData item5;
	item5.iconTexName = L"T_Weapon_Hammer_2H_Player_Godhammer";
	item5.equipmentGrade = EEquipmentGrade::Legendary;
	item5.ItemType = EItemType::Equipment;
	item5.equipmentType = EEquipmentType::Weapon;
	item5.szItemName = L"HeeDong's Sword";
	item5.iMainStat = 20;
	item5.iLevel = 16;
	item5.szWeaponName = L"5";
	item5.weaponData.weaponName = L"GodHammer";
	item5.equipmentName = EEquipmentName::GodHammer;
	item5.szStatusName = L"T_2H_DPS_155";
	dropList.emplace_back(item5);

	CItemData item6;
	item6.iconTexName = L"T_Weapon_Hammer_2H_Player_INE";
	item6.equipmentGrade = EEquipmentGrade::Legendary;
	item6.ItemType = EItemType::Equipment;
	item6.equipmentType = EEquipmentType::Weapon;
	item6.szItemName = L"HeeDong's Sword_2";
	item6.iMainStat = 20;
	item6.iLevel = 16;
	item6.szWeaponName = L"6";
	item6.weaponData.weaponName = L"Legend5";
	item6.equipmentName = EEquipmentName::Legend5;
	item6.szStatusName = L"T_2H_DPS_160";
	dropList.emplace_back(item6);

	CItemData item7;
	item7.iconTexName = L"T_Weapon_Sword_1H_Player_Eclipse_Gold";
	item7.equipmentGrade = EEquipmentGrade::Legendary;
	item7.ItemType = EItemType::Equipment;
	item7.equipmentType = EEquipmentType::Weapon;
	item7.szItemName = L"HeeDong's Sword_3";
	item7.iMainStat = 20;
	item7.iLevel = 16;
	item7.szWeaponName = L"7";
	item7.weaponData.weaponName = L"Eclipse";
	item7.equipmentName = EEquipmentName::Eclipse;
	item7.szStatusName = L"T_1H_DPS_145";
	dropList.emplace_back(item7);

	CItemData item8;
	item8.iconTexName = L"T_Weapon_Hammer_2H_Player_Skymourne";
	item8.equipmentGrade = EEquipmentGrade::Rare;
	item8.ItemType = EItemType::Equipment;
	item8.equipmentType = EEquipmentType::Weapon;
	item8.szItemName = L"HeeDong's Sword_4";
	item8.iMainStat = 20;
	item8.iLevel = 16;
	item8.weaponData.weaponName = L"Skymourne";
	item8.equipmentName = EEquipmentName::Skymourne;
	item8.szStatusName = L"T_2H_DPS_135";
	dropList.emplace_back(item8);

	CItemData item9;
	item9.iconTexName = L"T_Weapon_Hammer_2H_Player_Soothsayer";
	item9.equipmentGrade = EEquipmentGrade::Uncommon;
	item9.ItemType = EItemType::Equipment;
	item9.equipmentType = EEquipmentType::Weapon;
	item9.szItemName = L"HeeDong's Sword_5";
	item9.iMainStat = 20;
	item9.iLevel = 16;
	item9.weaponData.weaponName = L"Soothsayer";
	item9.equipmentName = EEquipmentName::Soothsayer;
	item9.szStatusName = L"T_2H_DPS_135";
	dropList.emplace_back(item9);

	CItemData item10;
	item10.iconTexName = L"T_Weapon_Sword_1H_Player_ArgentBlade";
	item10.equipmentGrade = EEquipmentGrade::Rare;
	item10.ItemType = EItemType::Equipment;
	item10.equipmentType = EEquipmentType::Weapon;
	item10.szItemName = L"HeeDong's Sword_6";
	item10.iMainStat = 20;
	item10.iLevel = 16;
	item10.weaponData.weaponName = L"ArgentBlade";
	item10.equipmentName = EEquipmentName::ArgentBlade;
	item10.szStatusName = L"T_1H_DPS_122";
	dropList.emplace_back(item10);

	CItemData item11;
	item11.iconTexName = L"T_Weapon_Hammer_2H_Player_Ironstone";
	item11.equipmentGrade = EEquipmentGrade::Rare;
	item11.ItemType = EItemType::Equipment;
	item11.equipmentType = EEquipmentType::Weapon;
	item11.szItemName = L"HeeDong's Sword_6";
	item11.iMainStat = 20;
	item11.iLevel = 16;
	item11.weaponData.weaponName = L"Ironstone";
	item11.equipmentName = EEquipmentName::Ironstone;
	item11.szStatusName = L"T_2H_DPS_134";
	dropList.emplace_back(item11);

	CItemData item12;
	item12.iconTexName = L"T_Weapon_Sword_1H_Player_Justice";
	item12.equipmentGrade = EEquipmentGrade::Epic;
	item12.ItemType = EItemType::Equipment;
	item12.equipmentType = EEquipmentType::Weapon;
	item12.szItemName = L"HeeDong's Sword_6";
	item12.iMainStat = 20;
	item12.iLevel = 16;
	item12.weaponData.weaponName = L"Justice";
	item12.equipmentName = EEquipmentName::Justice;
	item12.szStatusName = L"T_1H_DPS_135";
	dropList.emplace_back(item12);

	CItemData item13;
	item13.iconTexName = L"T_Weapon_Hammer_2H_Player_Legend1";
	item13.equipmentGrade = EEquipmentGrade::Legendary;
	item13.ItemType = EItemType::Equipment;
	item13.equipmentType = EEquipmentType::Weapon;
	item13.szItemName = L"HeeDong's Sword_6";
	item13.iMainStat = 20;
	item13.iLevel = 16;
	item13.weaponData.weaponName = L"Legend1";
	item13.equipmentName = EEquipmentName::Legend1;
	item13.szStatusName = L"T_2H_DPS_156";
	dropList.emplace_back(item3);

	CItemData item14;
	item14.iconTexName = L"T_Weapon_Hammer_2H_Player_Legend2";
	item14.equipmentGrade = EEquipmentGrade::Uncommon;
	item14.ItemType = EItemType::Equipment;
	item14.equipmentType = EEquipmentType::Weapon;
	item14.szItemName = L"HeeDong's Sword_6";
	item14.iMainStat = 20;
	item14.iLevel = 16;
	item14.weaponData.weaponName = L"Legend2";
	item14.equipmentName = EEquipmentName::Legend2;
	item14.szStatusName = L"T_2H_DPS_125";
	dropList.emplace_back(item14);

	CItemData item15;
	item15.iconTexName = L"T_Weapon_Hammer_2H_Player_Mesa";
	item15.equipmentGrade = EEquipmentGrade::Epic;
	item15.ItemType = EItemType::Equipment;
	item15.equipmentType = EEquipmentType::Weapon;
	item15.szItemName = L"HeeDong's Sword_6";
	item15.iMainStat = 20;
	item15.iLevel = 16;
	item15.weaponData.weaponName = L"Mesa";
	item15.equipmentName = EEquipmentName::Mesa;
	item15.szStatusName = L"T_2H_DPS_144";
	dropList.emplace_back(item15);

	//for (auto& iter : m_tDesc.itemData)
	//{
	//	for (auto& temp : iter)
	//	{
	//		dropList.emplace_back(*((CItemData*)temp));
	//	}
	//}
	return dropList;
}

_bool CDropBox::IsInteractable(void)
{
	if (IsFocused() && m_bFocus && false == m_bBoxOpened)
	{
		_float dist = MathUtils::Length(m_pPlayer, this);
		if (m_interactDist >= dist)
		{
			/* Guide UI Show */
			return true;
		}
	}
	return false;
}

_bool CDropBox::HasInteractionUI(void)
{
	return _bool();
}

_float CDropBox::GetRatioForUI(void)
{
	return _float();
}

_vector CDropBox::GetGuideUIPos(void)
{
	return _vector();
}

void CDropBox::BeginInteraction(void)
{
	__super::BeginInteraction();
	m_openElapsed = 0.f;
}

void CDropBox::UpdateInteraction(_double _dTimeDelta)
{
	__super::UpdateInteraction(_dTimeDelta);

	m_pStateController->Change_State(L"DropBox_Open");
	m_bBoxOpened = true;
	Open();
}

void CDropBox::EndInteraction(void)
{
	__super::EndInteraction();

	m_openElapsed = 0.f;
	HideGuideUI();
}

void CDropBox::FocusEnter()
{
	__super::FocusEnter();
}

void CDropBox::Focus()
{
	__super::Focus();
	Active_Effect((_uint)EFFECT::OPENBOX);

	m_bFocus = true;
}

void CDropBox::FocusExit()
{
	__super::FocusExit();

	m_bFocus = false;
}

CDropBox* CDropBox::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CDropBox* pInstance = new CDropBox(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CDropBox Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDropBox::Clone(const _uint _iSceneID, void* _pArg)
{
	CDropBox* pInstance = new CDropBox(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CDropBox Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDropBox::Free()
{
	Safe_Release(m_pAnimationController);
	Safe_Release(m_pStateController);
	Safe_Release(m_pCollider);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pModel);

	__super::Free();

}