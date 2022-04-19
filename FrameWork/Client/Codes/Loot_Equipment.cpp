#include "pch.h"
#include "Loot_Equipment.h"
#include "UI_LootBackground.h"
#include "UI_LootFrame.h"
#include "UI_LootGrade.h"
#include "UI_LootItemIcon.h"
#include "UI_LootItemEffect.h"
#include "UI_LootItemNameText.h"
#include "UI_LootItemNameType.h"

CLoot_Equipment::CLoot_Equipment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CHud(pDevice, pDeviceContext)
{
}

CLoot_Equipment::CLoot_Equipment(const CHud& rhs)
	:CHud(rhs)
{
}

HRESULT CLoot_Equipment::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoot_Equipment::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;
	
	desc = (*(CHud::Desc*)pArg);
	desc.pOwner = this;
	desc.fInitPos = { 0.f, 0.f };

	_float fPosyYOffset = desc.iQueueIndex * 90.f;
	m_fInitPos.y -= fPosyYOffset;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fInitPos));

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CLoot_Equipment::Tick(_double dDeltaTime)
{
	return _int();
}

_int CLoot_Equipment::LateTick(_double TimeDelta)
{
	FadeIn(TimeDelta);

	if (false == m_bAlive)
		Pulling(TimeDelta);

	SetGrade(desc.itemData);

	m_fAliveTime += TimeDelta;

	if (desc.fDisapeatTime <= m_fAliveTime)
		m_bAlive = false;

	m_pBG->LateTick(TimeDelta);
	m_pFrame->LateTick(TimeDelta);
	m_pGrade->LateTick(TimeDelta);
	m_pItemIcon->LateTick(TimeDelta);
	m_pItemEffect->LateTick(TimeDelta);
	m_pNameText->LateTick(TimeDelta);
	m_pNameType->LateTick(TimeDelta);

	return _int();
}

HRESULT CLoot_Equipment::Render()
{
	if (!g_pInvenUIManager->IsOpenModal())
	{
		m_pBG->Render();
		m_pFrame->Render();
		m_pGrade->Render();
		m_pItemIcon->Render();
		m_pItemEffect->Render();
		m_pNameText->Render();
		m_pNameType->Render();
	}
	return S_OK;
}


HRESULT CLoot_Equipment::Ready_Component(void)
{
	return S_OK;
}

HRESULT CLoot_Equipment::Ready_UIObject(void)
{
	 m_pBG = (CUI_LootBackground*) static_cast<CHud*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LootBackground", &desc));

	 m_pFrame = (CUI_LootFrame*) static_cast<CHud*>(
		 g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LootFrame", &desc));

	 m_pGrade = (CUI_LootGrade*) static_cast<CHud*>(
		 g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LootGrade", &desc));

	 m_pItemIcon = (CUI_LootItemIcon*) static_cast<CHud*>(
		 g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LootItemIcon", &desc));

	 m_pItemEffect = (CUI_LootItemEffect*) static_cast<CHud*>(
		 g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LootItemEffect", &desc));

	 m_pNameText = (CUI_LootItemNameText*) static_cast<CHud*>(
		 g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LootItemNameText", &desc));

	 m_pNameType = (CUI_LootItemNameType*) static_cast<CHud*>(
		 g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LootItemNameType", &desc));

	return S_OK;
}

void CLoot_Equipment::FadeIn(_double TimeDela)
{
	if (desc.fInitScale.x > desc.fEndScale.x)
	{
		desc.fInitScale.y = desc.fInitScale.x -= TimeDela;

		if (desc.fInitScale.x <= desc.fEndScale.x)
		{
			desc.fInitScale.x = desc.fEndScale.x;
			m_bByeBye = true;
		}
		m_pTransform->Scaling(_vector{ desc.fInitScale.x , desc.fInitScale.y, 1.f, 0.f });
	}
}

void CLoot_Equipment::Pulling(_double TimeDelta)
{

	if (m_fInitPos.y < m_fEndPos.y)
	{
		m_fInitPos.y += TimeDelta * 150.f;

		if (m_fInitPos.y >= m_fEndPos.y)
		{
			m_fInitPos.y = m_fEndPos.y;
			m_bBye = true;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ m_fInitPos.x, m_fInitPos.y, 0.0f, 1.f });
	}

	if (m_bBye)
	{
		if (m_fDisaperXInitPos < m_fDisaperXEndPos)
		{
			m_fDisaperXInitPos += TimeDelta * 150.f;

			if (m_fDisaperXInitPos > m_fDisaperXEndPos)
			{
				setActive(false);
			}
			m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ m_fDisaperXInitPos, m_fInitPos.y, 0.0f, 1.f });
		}
	}
}

void CLoot_Equipment::SetActiveAll(_bool _OnOff)
{
	this->setActive(_OnOff);

	m_pBG->setActive(_OnOff);
	m_pFrame->setActive(_OnOff);
	m_pGrade->setActive(_OnOff);
	m_pItemIcon->setActive(_OnOff);
	m_pItemEffect->setActive(_OnOff);
	m_pNameText->setActive(_OnOff);
	m_pNameType->setActive(_OnOff);
}

void CLoot_Equipment::SetGrade(CItemData itemData)
{
	CItemData Itemdata = itemData;

	EEquipmentGrade tempEnum = Itemdata.equipmentGrade;

	switch (tempEnum)
	{
	case EEquipmentGrade::Common:
	{//grey 
		m_pBG->SetBg(L"T_Item_Bg_Grey");
		m_pGrade->SetGrade(L"T_Weapon_Sash_Common");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Grey");
		m_pItemIcon->SetIcon(Itemdata.iconTexName);
		m_pNameText->SetIcon(g_arrHUDItemTypeIcon[(_int)Itemdata.equipmentType]); /* 템 종류 */
		m_pNameType->SetIcon(g_arrHUDItemIcon[(_int)Itemdata.equipmentName]); /* 템 이름 */
	}
	break;
	case EEquipmentGrade::Uncommon:
	{//blue
		m_pBG->SetBg(L"T_Item_Bg_Blue");
		m_pGrade->SetGrade(L"T_Weapon_Sash_Rare");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Blue");
		m_pItemIcon->SetIcon(Itemdata.iconTexName);
 		m_pNameText->SetIcon(g_arrHUDItemTypeIcon[(_int)Itemdata.equipmentType]); /* 템 종류 */
		m_pNameType->SetIcon(g_arrHUDItemIcon[(_int)Itemdata.equipmentName]); /* 템 이름 */
	}
	break;
	case EEquipmentGrade::Rare:
	{//green
		m_pBG->SetBg(L"T_Item_Bg_Green");
		m_pGrade->SetGrade(L"T_Weapon_Sash_Common");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Green");
		m_pItemIcon->SetIcon(Itemdata.iconTexName);
		m_pNameText->SetIcon(g_arrHUDItemTypeIcon[(_int)Itemdata.equipmentType]); /* 템 종류 */
		m_pNameType->SetIcon(g_arrHUDItemIcon[(_int)Itemdata.equipmentName]); /* 템 이름 */
	}
	break;
	case EEquipmentGrade::Epic:
	{//purple
		m_pBG->SetBg(L"T_Item_Bg_Purple");
		m_pGrade->SetGrade(L"T_Weapon_Sash_Legendary");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Purple");
		m_pItemIcon->SetIcon(Itemdata.iconTexName);
		m_pNameText->SetIcon(g_arrHUDItemTypeIcon[(_int)Itemdata.equipmentType]); /* 템 종류 */
		m_pNameType->SetIcon(g_arrHUDItemIcon[(_int)Itemdata.equipmentName]); /* 템 이름 */

	}
	break;
	case EEquipmentGrade::Legendary:
	{//orange
		m_pBG->SetBg(L"T_Item_Bg_Orange");
		m_pGrade->SetGrade(L"T_Weapon_Sash_Mythic");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Orange");
		m_pItemIcon->SetIcon(Itemdata.iconTexName);
		m_pNameText->SetIcon(g_arrHUDItemTypeIcon[(_int)Itemdata.equipmentType]); /* 템 종류 */
		m_pNameType->SetIcon(g_arrHUDItemIcon[(_int)Itemdata.equipmentName]);     /* 템 이름 */

	}
	break;
	case EEquipmentGrade::EQUIPGRADE_END:
	{
		m_pBG->SetBg(L"T_Item_Bg_Grey");
		m_pGrade->SetGrade(L"T_Weapon_Sash_Common");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Grey");
		m_pItemIcon->SetIcon(Itemdata.iconTexName);
		m_pNameText->SetIcon(g_arrHUDItemTypeIcon[(_int)Itemdata.equipmentType]); /* 템 종류 */
		m_pNameType->SetIcon(g_arrHUDItemIcon[(_int)Itemdata.equipmentName]); /* 템 이름 */

	}
	break;
	default:
		break;

	}
}

CLoot_Equipment* CLoot_Equipment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLoot_Equipment* pInstance = new CLoot_Equipment(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CLoot_Equipment Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoot_Equipment::Clone(const _uint iSceneID, void* pArg)
{
	CLoot_Equipment* pInstance = new CLoot_Equipment(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CLoot_Equipment Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoot_Equipment::Free()
{
	Safe_Release(m_pBG);
	Safe_Release(m_pGrade);
	Safe_Release(m_pFrame);
	Safe_Release(m_pItemIcon);
	Safe_Release(m_pItemEffect);
	Safe_Release(m_pNameText);
	Safe_Release(m_pNameType);

	__super::Free();
}
