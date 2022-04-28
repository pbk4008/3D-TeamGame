#include "pch.h"
#include "Hud.h"
#include "Loot_Equipment.h"
#include "PlayerData.h"
#include "EquipmentData.h"
#include "UI_Level_UP.h"
#include "UI_EquippedWeapon.h"
#include "UI_EquippedWeapon_Slot_1.h"
#include "UI_EquippedWeapon_Slot_2.h"

CHud::CHud(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{
}

CHud::CHud(const CHud& rhs)
	: CUI(rhs)
{
}

HRESULT CHud::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	/* 임시 위치 */
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, 0.f, 0.f, 1.f });

	m_pPlayerData = g_pDataManager->GET_DATA(CPlayerData, L"PlayerData");
	m_pEquipData = g_pDataManager->GET_DATA(CEquipmentData, L"EquipmentData");

	return S_OK;
}

_int CHud::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;

	if (true == m_bOnLevelUpUI)
	{
		/* 몹을 잡아서 경험치가 계속 들어올 때 */
		m_fOnTime += (_float)dDeltaTime;
		m_bHideLevelUpUI = false;
		FixPos();
		if (4.f <= m_fOnTime) /* 4초내로 추가로 잡은 몹이 없을 경우 */
		{
			m_bOnLevelUpUI = false;
			m_fOnTime = 0.f;
			m_bHideLevelUpUI = true;
		}
		//else
		//	m_bHideLevelUpUI = false; /* 4초가 아직 지나지 않았다면 UI를 끄지 않는다 */
	}

	if (true == m_bHideLevelUpUI)
		m_pLevelUp->Hide();

	if (3 <= g_pGameInstance->getCurrentLevel())
	{
		PullingEquipmentUI();
		CheckEuipWeaponSlot();
		CheckCurEquipSlot();
	}

	if (m_pLevelUp)
	{
		if (m_pLevelUp->getActive())
			m_pLevelUp->Tick(dDeltaTime);
	}
	
	return _int();
}

_int CHud::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;

	if (true == m_bRender)
	{
		if (!g_pInvenUIManager->IsOpenModal() &&
			!g_pGuideManager->IsOpenDeathUI())
		{
			if (nullptr != m_pRenderer)
				m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

			if (0 != m_vecLootEquipment.size())
			{
				for (_int i = 0; i < m_vecLootEquipment.size(); ++i)
				{
					if (true == m_vecLootEquipment[i]->getActive())
					{
						m_vecLootEquipment[i]->LateTick(TimeDelta);
					}
				}
			}
			if (m_pEquipWeapon->getActive())
				m_pEquipWeapon->LateTick(TimeDelta);

			if (m_pEquipWeapon_Slot_1->getActive())
			{
				m_pEquipWeapon_Slot_1->LateTick(TimeDelta);
				m_pEquipWeapon_Slot_2->LateTick(TimeDelta);
			}
		}
		if (m_pLevelUp)
		{
			if (m_pLevelUp->getActive())
				m_pLevelUp->LateTick(TimeDelta);
		}
	}
	return _int();
}

HRESULT CHud::Render()
{
	if (!g_pInvenUIManager->IsOpenModal())
	{
		if (0 != m_vecLootEquipment.size())
		{
			for (_int i = 0; i < m_vecLootEquipment.size(); ++i)
			{
				if (m_vecLootEquipment[i]->getActive())
					m_vecLootEquipment[i]->Render();
			}
		}
	}

	return S_OK;
}


HRESULT CHud::Ready_Component(void)
{
	return S_OK;
}

HRESULT CHud::Ready_UIObject(void)
{
	m_pLevelUp = (CLevel_UP*) static_cast<CHud*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LevelUp"));
	assert(m_pLevelUp);

	m_pEquipWeapon = (CUI_EquippedWeapon*) static_cast<CHud*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_EquipWeapon"));
	assert(m_pEquipWeapon);
	m_pEquipWeapon->setActive(false);

	m_pEquipWeapon_Slot_1 = (CUI_EquippedWeapon_Slot_1*) static_cast<CHud*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_EquipWeapon_Slot_1"));
	assert(m_pEquipWeapon_Slot_1);
	m_pEquipWeapon_Slot_1->setActive(false);

	m_pEquipWeapon_Slot_2 = (CUI_EquippedWeapon_Slot_2*) static_cast<CHud*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_EquipWeapon_Slot_2"));
	assert(m_pEquipWeapon_Slot_2);
	m_pEquipWeapon_Slot_2->setActive(false);

	return S_OK;
}

void CHud::PullingEquipmentUI(void)
{
	if (0 == m_vecLootEquipment.size())
		return;
	else
	{
		auto iter = m_vecLootEquipment.begin();

		if (false == (*iter)->getActive())
		{
			Safe_Release(*iter);
			m_vecLootEquipment.erase(iter);
		}
		else
			return;
	}
}

void CHud::ShowLevelUp_HUD(void)
{
	m_pLevelUp->Show(m_pPlayerData);
	m_bOnLevelUpUI = true;
}

void CHud::OnLootEquipment(void* pItemData)
{
	CItemData itemData = *static_cast<CItemData*>(pItemData);

	CHud::Desc desc;
	desc.pOwner = this;
	desc.itemData = itemData;
	desc.iQueueIndex = (_int)m_vecLootEquipment.size();
	desc.fDisapeatTime = 3.0f;

	CLoot_Equipment* LootEquipUI = static_cast<CLoot_Equipment*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LootEquipment", &desc));
	assert("LootEquip is nullptr" && LootEquipUI);
	LootEquipUI->SetActiveAll(true);

	m_vecLootEquipment.emplace_back(LootEquipUI);
}

void CHud::CheckEuipWeaponSlot(void)
{
	if (m_pEquipData->IsExistEquip(EEquipSlot::Weapon1))
	{
		/* 1번무기는 있지만 2번 무기는 없는 경우*/
		if (!m_pEquipData->IsExistEquip(EEquipSlot::Weapon2))
		{
			switch (m_pEquipData->GetEquipment(EEquipSlot::Weapon1).weaponType)
			{
			case EWeaponType::LongSword:
				m_pEquipWeapon->SetImage(L"T_HUD_EquippedWeapon_Border_Longsword");
				break;
			case EWeaponType::Hammer:
				m_pEquipWeapon->SetImage(L"T_HUD_EquippedWeapon_Border_Hammer");
				break;
			}
			SetActiveOnlyDefault();
		}
		else /* 1번 무기도 있고 2번 무기도 있는 경우 */
		{
			/* 1번 슬롯 텍스쳐 설정 */
			switch (m_pEquipData->GetEquipment(EEquipSlot::Weapon1).weaponType)
			{
			case EWeaponType::LongSword:
				m_pEquipWeapon_Slot_1->SetImage(L"T_HUD_EquippedWeapon_Border_Longsword");
				break;
			case EWeaponType::Hammer:
				m_pEquipWeapon_Slot_1->SetImage(L"T_HUD_EquippedWeapon_Border_Hammer");
				break;
			}
			/* 2번 슬롯 텍스쳐 설정 */
			switch (m_pEquipData->GetEquipment(EEquipSlot::Weapon2).weaponType)
			{
			case EWeaponType::LongSword:
				m_pEquipWeapon_Slot_2->SetImage(L"T_HUD_EquippedWeapon_Border_Longsword");
				break;
			case EWeaponType::Hammer:
				m_pEquipWeapon_Slot_2->SetImage(L"T_HUD_EquippedWeapon_Border_Hammer");
				break;
			}
			SetActiveOnlySlots();
		}
	}
	else if (m_pEquipData->IsExistEquip(EEquipSlot::Weapon2)) /* 1번 무기는 없지만 2번 무기는 있는 경우*/
	{
		switch (m_pEquipData->GetEquipment(EEquipSlot::Weapon2).weaponType)
		{
		case EWeaponType::LongSword:
			m_pEquipWeapon->SetImage(L"T_HUD_EquippedWeapon_Border_Longsword");
			break;
		case EWeaponType::Hammer:
			m_pEquipWeapon->SetImage(L"T_HUD_EquippedWeapon_Border_Hammer");
			break;
		}
		SetActiveOnlyDefault();
	}
	else /* 1번 2번 무기 모두 없는 경우 */
	{

	}
}

void CHud::CheckCurEquipSlot(void)
{
	if(1 == m_pPlayerData->EquipedSlot)
	{ 
		m_pEquipWeapon_Slot_1->SetMainSlot(true);

		switch (m_pEquipData->GetEquipment(EEquipSlot::Weapon1).weaponType)
		{
		case EWeaponType::LongSword:
			m_pEquipWeapon_Slot_1->SetImage(L"T_HUD_EquippedWeapon_Border_Longsword");
			break;
		case EWeaponType::Hammer:
			m_pEquipWeapon_Slot_1->SetImage(L"T_HUD_EquippedWeapon_Border_Hammer");
			break;
		}

		switch (m_pEquipData->GetEquipment(EEquipSlot::Weapon2).weaponType)
		{
		case EWeaponType::LongSword:
			m_pEquipWeapon_Slot_2->SetImage(L"T_HUD_EquippedWeapon_Border_Longsword");
			break;
		case EWeaponType::Hammer:
			m_pEquipWeapon_Slot_2->SetImage(L"T_HUD_EquippedWeapon_Border_Hammer");
			break;
		}

	}
	else if (2 == m_pPlayerData->EquipedSlot)
	{
		m_pEquipWeapon_Slot_1->SetMainSlot(false);

		switch (m_pEquipData->GetEquipment(EEquipSlot::Weapon2).weaponType)
		{
		case EWeaponType::LongSword:
			m_pEquipWeapon_Slot_1->SetImage(L"T_HUD_EquippedWeapon_Border_Longsword");
			break;
		case EWeaponType::Hammer:
			m_pEquipWeapon_Slot_1->SetImage(L"T_HUD_EquippedWeapon_Border_Hammer");
			break;
		}
		switch (m_pEquipData->GetEquipment(EEquipSlot::Weapon1).weaponType)
		{
		case EWeaponType::LongSword:
			m_pEquipWeapon_Slot_2->SetImage(L"T_HUD_EquippedWeapon_Border_Longsword");
			break;
		case EWeaponType::Hammer:
			m_pEquipWeapon_Slot_2->SetImage(L"T_HUD_EquippedWeapon_Border_Hammer");
			break;
		}
	}
}

void CHud::SetActiveOnlyDefault(void)
{
	m_pEquipWeapon->setActive(true);

	m_pEquipWeapon_Slot_1->setActive(false);
	m_pEquipWeapon_Slot_2->setActive(false);
}

void CHud::SetActiveOnlySlots(void)
{
	m_pEquipWeapon->setActive(false);

	m_pEquipWeapon_Slot_1->setActive(true);
	m_pEquipWeapon_Slot_2->setActive(true);
}

void CHud::SetLevelBG(_int PlayerLevel)
{
	if (!m_pLevelUp)
		return;

	m_pLevelUp->SetLevelBG(PlayerLevel);
	
}

void CHud::FixPos(void)
{
	m_pLevelUp->FixPos();
}

void CHud::EquipmentRenderNo(_bool bOnoff)
{
	m_bRender = bOnoff;
	//m_pEquipWeapon->setActive(false);
	//m_pEquipWeapon_Slot_1->setActive(false);
	//m_pEquipWeapon_Slot_2->setActive(false);
}

void CHud::EquipmentRenderYes(void)
{
	m_pEquipWeapon->setActive(true);
	m_pEquipWeapon_Slot_1->setActive(true);
	m_pEquipWeapon_Slot_2->setActive(true);
}

CHud* CHud::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CHud* pInstance = new CHud(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CHud Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHud::Clone(const _uint _iSceneID, void* _pArg)
{
	CHud* pInstance = new CHud(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CHud Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud::Free()
{
	__super::Free();

	for (auto& iter : m_vecLootEquipment)
		Safe_Release(iter);

	Safe_Release(m_pLevelUp);
	Safe_Release(m_pEquipWeapon);
	Safe_Release(m_pEquipWeapon_Slot_1);
	Safe_Release(m_pEquipWeapon_Slot_2);
}
