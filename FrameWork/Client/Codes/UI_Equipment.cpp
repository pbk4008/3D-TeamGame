#include "pch.h"
#include "UI_Equipment.h"
#include "SingleImage.h"
#include "InventoryData.h"
#include "EquipmentData.h"
#include "UI_ItemSlot.h"

CUI_Equipment::CUI_Equipment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Equipment::CUI_Equipment(const CUI_Equipment& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Equipment::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Equipment::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	m_pTransform->Scaling(_vector{ 1280.f, 720.f });
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ HALF_WINCX, HALF_WINCY });

	m_pInventoryData = g_pDataManager->GET_DATA(CInventoryData, L"InventoryData");
	m_pEquipData     = g_pDataManager->GET_DATA(CEquipmentData, L"EquipmentData");

	setActive(true);

	return S_OK;
}

_int CUI_Equipment::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	MouseOnSlot();
	ClickSlot();

	return _int();
}

_int CUI_Equipment::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (this->getActive())
	{
		for (auto iter : m_vecSlots)
			iter->LateTick(TimeDelta);
	}

	return _int();
}

HRESULT CUI_Equipment::Render()
{
	if (this->getActive())
	{
		for (auto iter : m_vecSlots)
			iter->Render();
	}
	return S_OK;
}

HRESULT CUI_Equipment::Ready_Component(void)
{
	return S_OK;
}

HRESULT CUI_Equipment::Ready_UIObject(void)
{
	_float2 fInitPos   = { 50.f, 140.f };
	_float2 fResultPos = { 50.f, 140.f };

	for (_int i = 0; i < 8 ; ++i)
	{
		CUI_ItemSlot::ItemSlotDesc slotDesc;

		if (0 != i)
		{
			if (1 == i % 2)
			{
				fResultPos.x += 100.f;
			}
			else
			{
				fResultPos.x = fInitPos.x;
				fResultPos.y -= 100.f;
			}
		}
		slotDesc.fPos = fResultPos;
		slotDesc.fScale = { 90, 90.f };
		auto itemSlot = static_cast<CUI_ItemSlot*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_ItemSlot", &slotDesc));
		InsertSlotVector(itemSlot);
		SetResourceCount();
	}



	return S_OK;
}

void CUI_Equipment::Show(void)
{
	this->setActive(true);
	
	UpdateSlots();
	UpdateResourceCount();
	
	m_bEquipmentActive = true;
}

void CUI_Equipment::Hide(void)
{
	this->setActive(false);

	for (auto& slot : m_vecSlots)
	{
		slot->SetActiveAll(false);
	}

	m_bEquipmentActive = false;
}

void CUI_Equipment::UpdateSlots(void)
{
	for (_int i = 0; i < m_vecSlots.size(); ++i)
	{
		CItemData data = m_pEquipData->GetEquipment((EEquipSlot)i);

		if(EItemType::NONE == data.ItemType)
		{
			
			m_vecSlots[i]->SetIcon(L"T_Item_Bg_None");
			m_vecSlots[i]->SetActiveAll(false);
			m_vecSlots[i]->SetActiveOnlyBg(true);
		}
		else
		{
			m_vecSlots[i]->SetIcon(data.iconTexName);
			m_vecSlots[i]->SetGrade(data.equipmentGrade);
			m_vecSlots[i]->SetActiveAll(true);
			m_vecSlots[i]->SetActiveEquiped(false);
		}
	}
}

void CUI_Equipment::UpdateSlot(_int _iIndex)
{
	CItemData itemdata = m_pEquipData->GetEquipment((EEquipSlot)_iIndex);

	if (itemdata.ItemType == EItemType::NONE)
	{
		m_vecSlots[_iIndex]->SetIcon(L"T_Item_Bg_None");
		m_vecSlots[_iIndex]->SetGrade(EEquipmentGrade::EQUIPGRADE_END);

		m_vecSlots[_iIndex]->SetActiveAll(false);
		m_vecSlots[_iIndex]->SetActiveOnlyBg(true);
	}
	else
	{
		m_vecSlots[_iIndex]->SetIcon(itemdata.iconTexName);
		m_vecSlots[_iIndex]->SetActiveOnlnyIcon(true);
		m_vecSlots[_iIndex]->SetGrade(itemdata.equipmentGrade);
	}
}

void CUI_Equipment::UpdateResourceCount(void)
{
}

void CUI_Equipment::ClickSlot(void)
{
	for (_int i = 0; i < Max_EquipmentSlot; ++i)
	{
		if (m_vecSlots[i]->IconClicked())
		{
			_ulong	tempID = m_pEquipData->GetEquipment((EEquipSlot)i).uid;
			CItemData* pInvenData = m_pInventoryData->GetItemByID(tempID);
			pInvenData->bEquiped = false;

			CItemData temp;
			temp.bEquiped = false;
			temp.ItemType = EItemType::NONE;
			m_pEquipData->SetEquipment((EEquipSlot)i, temp);
			
			UpdateSlot(i);
		}
	}
}

void CUI_Equipment::MouseOnSlot(void)
{
	for (_int i = 0; i <  Max_EquipmentSlot; ++i)
	{
		if (m_vecSlots[i]->IconMouseOn())
		{
			CItemData itemdata = m_pEquipData->GetEquipment((EEquipSlot)i);
			if (itemdata.ItemType == EItemType::NONE)
			{
				return;
			}
			else
			{
				CItemData itemdata = m_pEquipData->GetEquipment((EEquipSlot)i);
				g_pInvenUIManager->ShowItemStatus(&itemdata);
			}
		return;
		}
	}
	g_pInvenUIManager->HideItemStatus();
}

_bool CUI_Equipment::GetEquipmentActive(void)
{
	return m_bEquipmentActive;
}

void CUI_Equipment::InsertSlotVector(CUI_ItemSlot* _pObj)
{
	m_vecSlots.emplace_back(_pObj);
}

void CUI_Equipment::SetResourceCount()
{
}

CUI_Equipment* CUI_Equipment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Equipment* pInstance = new CUI_Equipment(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Equipment Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Equipment::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Equipment* pInstance = new CUI_Equipment(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Equipment Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Equipment::Free()
{
	for (auto iter : m_vecSlots)
		Safe_Release(iter);
	m_vecSlots.clear();

	__super::Free();
}
