#include "pch.h"
#include "UI_Armory.h"
#include "SingleImage.h"
#include "InventoryData.h"
#include "EquipmentData.h"
#include "UI_ItemSlot.h"

CUI_Armory::CUI_Armory(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Armory::CUI_Armory(const CUI_Armory& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Armory::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Armory::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	m_pTransform->Scaling(_vector{ 1920.f, 1080.f });
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ HALF_WINCX, HALF_WINCY });

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_UIObject()))
		return E_FAIL;
	
	m_pInventoryData = g_pDataManager->GET_DATA(CInventoryData, L"InventoryData");
	m_pEquipData = g_pDataManager->GET_DATA(CEquipmentData, L"EquipmentData");

	setActive(false);

	return S_OK;
}

_int CUI_Armory::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	MouseOnSlot();
	ClickSlot();

	return _int();
}

_int CUI_Armory::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;

	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (this->getActive())
	{
		for (auto iter : m_vecSlots)
			iter->LateTick(TimeDelta);
	}

	return _int();
}

HRESULT CUI_Armory::Render()
{
	if (this->getActive())
	{
		for (auto iter : m_vecSlots)
			iter->Render();
	}
	return S_OK;
}

HRESULT CUI_Armory::Ready_Component(void)
{
	return S_OK;
}

HRESULT CUI_Armory::Ready_UIObject(void)
{
	_float2 fInitPos = { -750.f, 140.f };
	_float2 fResultPos = { -750.f, 140.f };

	for (_int i = 0; i < 32; ++i)
	{
		CUI_ItemSlot::ItemSlotDesc slotDesc;

		if (0 != i)
		{
			if (0 == i % 8)
			{
				fResultPos.x = fInitPos.x;
				fResultPos.y -= 130.f;
			}
			else
			{
				fResultPos.x += 130.f;
			}
		}
		slotDesc.fPos = fResultPos;
		slotDesc.fScale = { 120.f, 120.f };
		auto itemSlot = static_cast<CUI_ItemSlot*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_ItemSlot", &slotDesc));
		InsertSlotVector(itemSlot);
		SetResourceCount();
	}

	return S_OK;
}

void CUI_Armory::Show(void)
{
	this->setActive(true);

	UpdateSlots();
	UpdateResourceCount();

	m_bArmoryActive = true;
}

void CUI_Armory::Hide(void)
{
	this->setActive(false);

	for (auto& slot : m_vecSlots)
	{
		slot->SetActiveAll(false);
	}

	m_bArmoryActive = false;
}

void CUI_Armory::UpdateSlots(void)
{
	for (_int i = 0 ; i < Max_InventorySlot; ++i)
	{
		if (i < m_pInventoryData->GetCount())
		{
			m_vecSlots[i]->SetActiveAll(true);

			m_vecSlots[i]->SetIcon(m_pInventoryData->GetItem(i).iconTexName);
			m_vecSlots[i]->SetGrade(m_pInventoryData->GetItem(i).equipmentGrade);

			if (false == m_pInventoryData->GetItem(i).bEquiped)
			{
				//창착중인 아이템이 아닌 경우
				m_vecSlots[i]->SetActiveEquiped(false);
			}
		}
		else
		{
			m_vecSlots[i]->SetActiveAll(false);
			m_vecSlots[i]->SetActiveOnlyBg(true);
		}
	}
}

void CUI_Armory::UpdateSlot(_int _iIndex)
{
	m_vecSlots[_iIndex]->SetActiveAll(true);

	if (m_pInventoryData->GetItem(_iIndex).bEquiped == false)
	{
		m_vecSlots[_iIndex]->SetActiveEquiped(false);
	}
}

void CUI_Armory::UpdateResourceCount(void)
{
}

void CUI_Armory::ClickSlot(void)
{
	if (0 >= m_vecSlots.size())
		return;

	CLEAR_QUEST(L"T_HUD_EquipNewWeapon");

	for (_int i = 0; i < m_pInventoryData->GetCount(); ++i)
	{
		if (m_vecSlots[i]->ItemClicked())
		{
			if (m_pEquipData->ExistItem(m_pInventoryData->GetItem(i)))
			{ /* 이미 장착중인 아이템인 경우 */
				return;
			}
			else
			{
				CItemData SelectedItem = m_pInventoryData->GetItem(i);
				EEquipmentType type = SelectedItem.equipmentType;

				switch (type)
				{
				case Client::EEquipmentType::Weapon:
				{
					if (m_pEquipData->IsExistEquip(EEquipSlot::Weapon1))
					{
						if (m_pEquipData->IsExistEquip(EEquipSlot::Weapon2))
							return; /* 무기 1,2 슬롯 모두 사용중 */
						else
						{
							m_pInventoryData->SetEquiped(i, TRUE);
							m_pEquipData->ChangeEquipment(EEquipSlot::Weapon2, SelectedItem);
							UpdateSlot(i);
							STOP_SOUND(CHANNEL::EFFECT);
							PLAY_SOUND(L"UI_Equip", CHANNEL::EFFECT);
						}
					}
					else
					{
						m_pInventoryData->SetEquiped(i, TRUE);
						m_pEquipData->ChangeEquipment(EEquipSlot::Weapon1, SelectedItem);
						UpdateSlot(i);
						STOP_SOUND(CHANNEL::EFFECT);
						PLAY_SOUND(L"UI_Equip", CHANNEL::EFFECT);
					}
				}
				break;
				case Client::EEquipmentType::Ring:
				{
					if (m_pEquipData->IsExistEquip(EEquipSlot::Ring1))
					{
						if (m_pEquipData->IsExistEquip(EEquipSlot::Ring2))
							return; /* 반지 1,2 슬롯 모두 사용중 */
						else
						{
							m_pInventoryData->SetEquiped(i, TRUE);
							m_pEquipData->ChangeEquipment(EEquipSlot::Ring2, SelectedItem);
							UpdateSlot(i);
							STOP_SOUND(CHANNEL::EFFECT);
							PLAY_SOUND(L"UI_Equip", CHANNEL::EFFECT);
						}
					}
					else
					{
						m_pInventoryData->SetEquiped(i, TRUE);
						m_pEquipData->ChangeEquipment(EEquipSlot::Ring1, SelectedItem);
						UpdateSlot(i);
						STOP_SOUND(CHANNEL::EFFECT);
						PLAY_SOUND(L"UI_Equip", CHANNEL::EFFECT);
					}
				}
				break;
				case Client::EEquipmentType::Amulet:
				{
					if (m_pEquipData->IsExistEquip(EEquipSlot::Amulet))
						return;
					else
					{
						m_pInventoryData->SetEquiped(i, TRUE);
						m_pEquipData->ChangeEquipment(EEquipSlot::Amulet, SelectedItem);
						UpdateSlot(i);
						STOP_SOUND(CHANNEL::EFFECT);
						PLAY_SOUND(L"UI_Equip", CHANNEL::EFFECT);
					}
				}
				break;
				case Client::EEquipmentType::Charm:
				{
					if (m_pEquipData->IsExistEquip(EEquipSlot::Charm))
						return;
					else
					{
						m_pInventoryData->SetEquiped(i, TRUE);
						m_pEquipData->ChangeEquipment(EEquipSlot::Charm, SelectedItem);
						UpdateSlot(i);
						STOP_SOUND(CHANNEL::EFFECT);
						PLAY_SOUND(L"UI_Equip", CHANNEL::EFFECT);
					}
				}
				break;
				case Client::EEquipmentType::LifeStone:
				{
					if (m_pEquipData->IsExistEquip(EEquipSlot::LifeStone))
						return;
					else
					{
						m_pInventoryData->SetEquiped(i, TRUE);
						m_pEquipData->ChangeEquipment(EEquipSlot::LifeStone, SelectedItem);
						UpdateSlot(i);
						STOP_SOUND(CHANNEL::EFFECT);
						PLAY_SOUND(L"UI_Equip", CHANNEL::EFFECT);
					}
				}
				break;
				case Client::EEquipmentType::Banner:
				{
					if (m_pEquipData->IsExistEquip(EEquipSlot::Banner))
						return;
					else
					{
						m_pInventoryData->SetEquiped(i, TRUE);
						m_pEquipData->ChangeEquipment(EEquipSlot::Banner, SelectedItem);
						UpdateSlot(i);
						STOP_SOUND(CHANNEL::EFFECT);
						PLAY_SOUND(L"UI_Equip", CHANNEL::EFFECT);
					}
				}
				break;
				default:
				{
					assert("Not Found Item Type!");
				}
					break;
				}
			}
			break;
		}
	}
}

void CUI_Armory::MouseOnSlot(void)
{
	if (0 >= m_vecSlots.size() ||
		false == m_bArmoryActive )
	{
		return;
	}

	for (_int i = 0; i < m_pInventoryData->GetCount(); ++i)
	{
		if (m_vecSlots[i]->IconMouseOn())
		{
			CItemData temp = m_pInventoryData->GetItem(i);
			g_pInvenUIManager->ShowItemStatus(&temp);
			return;
		}
	}

	g_pInvenUIManager->HideItemStatus();
}

_bool CUI_Armory::GetArmoryActive(void)
{
	return m_bArmoryActive;
}

void CUI_Armory::InsertSlotVector(CUI_ItemSlot* _pObj)
{
	m_vecSlots.emplace_back(_pObj);
}

void CUI_Armory::SetResourceCount()
{
}

CUI_Armory* CUI_Armory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Armory* pInstance = new CUI_Armory(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Armory Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Armory::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Armory* pInstance = new CUI_Armory(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Armory Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Armory::Free()
{
	for (auto iter : m_vecSlots)
		Safe_Release(iter);
	m_vecSlots.clear();

	__super::Free();
}
