#include "pch.h"
#include "Hud.h"
#include "Loot_Equipment.h"
#include "UI_Level_UP.h"
#include "PlayerData.h"

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

	return S_OK;
}

_int CHud::Tick(_double dDeltaTime)
{
	PullingEquipmentUI();
	ShowLevelUp_HUD();

	if (m_pLevelUp)
	{
		if (m_pLevelUp->getActive())
			m_pLevelUp->Tick(dDeltaTime);
	}
	
	return _int();
}

_int CHud::LateTick(_double TimeDelta)
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

	//if (m_pLevelUp)
	//{
	//	if (m_pLevelUp->getActive())
	//		m_pLevelUp->LateTick(TimeDelta);
	//}

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

	//if (m_pLevelUp)
	//{
	//	if (m_pLevelUp->getActive())
	//		m_pLevelUp->Render();
	//}

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
	if (true)
	{
		if (m_pLevelUp)
		{
			//m_pLevelUp->Show(m_pPlayerData);
		}
	}
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

}
