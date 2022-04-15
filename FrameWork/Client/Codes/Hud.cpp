#include "pch.h"
#include "Hud.h"
#include "Loot_Equipment.h"
#include "ItemData.h"

CHud::CHud(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
}

CHud::CHud(const CHud& rhs)
{
}

HRESULT CHud::NativeConstruct_Prototype(void)
{
	return S_OK;
}

HRESULT CHud::NativeConstruct(const _uint iSceneID, void* pArg)
{
	return S_OK;
}

_int CHud::Tick(_double dDeltaTime)
{
	PullingEquipmentUI();

	return _int();
}

_int CHud::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CHud::Render()
{
	return S_OK;
}

void CHud::setActive(_bool bActive)
{
}

HRESULT CHud::Ready_Component(void)
{
	return S_OK;
}

HRESULT CHud::Ready_UIObject(void)
{
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

			//for (auto& LootEvent : m_vecLootEquipment)
				//LootEvent->Pulling();
		}
		else
			return;
	}

}

void CHud::OnLootEquipment(void* pItemData)
{
	CItemData itemData = *static_cast<CItemData*>(pItemData);

	CLoot_Equipment::Desc desc;
	desc.itemData = itemData;
	desc.iQueueIndex = (_int)m_vecLootEquipment.size();

	CLoot_Equipment* LootEquipUI = static_cast<CLoot_Equipment*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LootEquipment", &desc));

	m_vecLootEquipment.emplace_back(LootEquipUI);
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
	return nullptr;
}

void CHud::Free()
{
	Safe_Release(m_pLootEquipment);
}
