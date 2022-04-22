#include "pch.h"
#include "UI_ItemSlot.h"
#include "SingleImage.h"
#include "UI_SlotBackground.h"
#include "UI_SlotItemIcon.h"
#include "UI_SlotGrade.h"
#include "UI_SlotItemEffect.h"
#include "UI_EquipedText.h"
#include "UI_SelectEffect.h"

CUI_ItemSlot::CUI_ItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_ItemSlot::CUI_ItemSlot(const CUI_ItemSlot& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ItemSlot::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ItemSlot::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(ItemSlotDesc*)pArg);
	desc.pOwner = this;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CUI_ItemSlot::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_ItemSlot::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	if (m_fInitPos > m_fEndPos)
	{
		m_fInitPos -= (_float)TimeDelta * 100.f;
		if (m_fInitPos < m_fEndPos)
		{
			m_fInitPos = m_fEndPos;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ m_fInitPos, 0.f, 0.f, 1.f });
	}

	IconMouseOn();

	if(m_pBG->getActive())
		m_pBG->LateTick(TimeDelta);

	if (m_pItemIcon->getActive())
		m_pItemIcon->LateTick(TimeDelta);

	if (m_pGrade->getActive())
		m_pGrade->LateTick(TimeDelta);

	if (m_pItemEffect->getActive())
		m_pItemEffect->LateTick(TimeDelta);

	if (m_pEquipedText->getActive())
		m_pEquipedText->LateTick(TimeDelta);

	if (m_pSelectEffect->getActive())
		m_pSelectEffect->LateTick(TimeDelta);


	return _int();
}

HRESULT CUI_ItemSlot::Render()
{
	if (FAILED(CUI::Render()))
		return -1;

	if (m_pBG->getActive())
		m_pBG->Render();

	if (m_pItemIcon->getActive())
		m_pItemIcon->Render();

	if (m_pGrade->getActive())
		m_pGrade->Render();

	if (m_pItemEffect->getActive())
		m_pItemEffect->Render();

	if (m_pEquipedText->getActive())
		m_pEquipedText->Render();

	if (m_pSelectEffect->Render())
		m_pSelectEffect->Render();

	return S_OK;
}

void CUI_ItemSlot::setActive(_bool bActive)
{
	this->m_bActive = bActive;

	if (false == bActive)
		m_fInitPos = 10.f;
}

HRESULT CUI_ItemSlot::Ready_Component(void)
{
	return S_OK;
}

HRESULT CUI_ItemSlot::Ready_UIObject(void)
{
	m_pBG		    = static_cast<CUI_SlotBackground*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_ItemSlot_BG", &desc));
	m_pItemIcon	    = static_cast<CUI_SlotItemIcon*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_ItemIcon", &desc));
	m_pGrade	    = static_cast<CUI_SlotGrade*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_ItemGrade", &desc));
	m_pItemEffect   = static_cast<CUI_SlotItemEffect*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_ItemEffect", &desc));
	m_pEquipedText  = static_cast<CUI_EquipedText*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_ItemEquipedText", &desc));
	m_pSelectEffect = static_cast<CUI_SelectEffect*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_SelectEffect", &desc));

	assert("Failed to Create UI Object" && (m_pBG || m_pItemIcon || m_pGrade || m_pEquipedText));

	return S_OK;
}

void CUI_ItemSlot::SetActiveAll(_bool _OnOff)
{
	this->setActive(_OnOff);

	m_pBG->setActive(_OnOff);
	m_pItemIcon->setActive(_OnOff);
	m_pGrade->setActive(_OnOff);
	m_pItemEffect->setActive(_OnOff);
	m_pEquipedText->setActive(_OnOff);
	m_pSelectEffect->setActive(_OnOff);
}

void CUI_ItemSlot::SetActiveExceptBg(_bool _OnOff)
{
	m_pGrade->setActive(_OnOff);
	m_pItemIcon->setActive(_OnOff);
	m_pItemEffect->setActive(_OnOff);
	m_pSelectEffect->setActive(_OnOff);
}

void CUI_ItemSlot::SetActiveOnlyBg(_bool _OnOff)
{
	m_pBG->setActive(true);
}

void CUI_ItemSlot::SetActiveOnlnyIcon(_bool _OnOff)
{
	m_pItemIcon->setActive(true);
}

void CUI_ItemSlot::SetActiveEquiped(_bool _IsEquiped)
{
	m_pEquipedText->setActive(_IsEquiped);
}

_bool CUI_ItemSlot::BgMouseOn(void)
{
	return _bool();
}

_bool CUI_ItemSlot::BgClicked(_int _KeyState)
{
	return _bool();
}

_bool CUI_ItemSlot::IconMouseOn(void)
{
	if (m_pItemIcon->IconMouseOn())
	{
		m_pSelectEffect->setActive(true);
		return true;
	}
	else
	{
		m_pSelectEffect->setActive(false);
		return false;
	}
}

_bool CUI_ItemSlot::IconClicked()
{
	if (m_pItemIcon->getActive())
	{
		if (m_pItemIcon->IconClicked())
			return true;
	}
	return false;
}

_bool CUI_ItemSlot::ItemClicked(void)
{
	if (m_pItemIcon->getActive())
	{
		if (m_pItemIcon->ItemClicked())
			return true;
	}
	return false;
}

void CUI_ItemSlot::SetIcon(const std::wstring& _szFileName)
{
	m_pItemIcon->SetIcon(_szFileName);
}

void CUI_ItemSlot::SetGrade(EEquipmentGrade _eGrade)
{
	EEquipmentGrade tempEnum = _eGrade;

	switch (tempEnum)
	{
	case EEquipmentGrade::Common:
	{//grey
		m_pGrade->SetGrade(L"T_Weapon_Sash_Common");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Grey");
		m_pSelectEffect->SetIcon(L"T_Item_Bg_Blue_Primal");
	}
	break;
	case EEquipmentGrade::Uncommon:
	{//blue
		m_pGrade->SetGrade(L"T_Weapon_Sash_Rare");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Blue");
		m_pSelectEffect->SetIcon(L"T_Item_Bg_Blue_Primal");
	}
	break;
	case EEquipmentGrade::Rare:
	{//green
		m_pGrade->SetGrade(L"T_Weapon_Sash_Common");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Green");
		m_pSelectEffect->SetIcon(L"T_Item_Bg_Green_Primal");
	}
	break;
	case EEquipmentGrade::Epic:
	{//purple
		m_pGrade->SetGrade(L"T_Weapon_Sash_Legendary");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Orange");
		m_pSelectEffect->SetIcon(L"T_Item_Bg_Orange_Primal");
	}
	break; 
	case EEquipmentGrade::Legendary:
	{//orange
		m_pGrade->SetGrade(L"T_Weapon_Sash_Mythic"); 
		m_pItemEffect->SetIcon(L"T_Item_Glow_Purple");
		m_pSelectEffect->SetIcon(L"T_Item_Bg_Purple_Primal");
	}
	break;
	case EEquipmentGrade::EQUIPGRADE_END:
	{
		m_pGrade->SetGrade(L"T_Weapon_Sash_Common");
		m_pItemEffect->SetIcon(L"T_Item_Glow_Grey");
		m_pSelectEffect->SetIcon(L"T_Item_Bg_Blue_Primal");
	}
	break;
	default:
		break;
	}
}

CUI_ItemSlot* CUI_ItemSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_ItemSlot* pInstance = new CUI_ItemSlot(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_ItemSlot Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ItemSlot::Clone(const _uint iSceneID, void* pArg)
{
	CUI_ItemSlot* pInstance = new CUI_ItemSlot(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_ItemSlot Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_ItemSlot::Free()
{
	Safe_Release(m_pBG);
	Safe_Release(m_pItemIcon);
	Safe_Release(m_pGrade);
	Safe_Release(m_pItemEffect);
	Safe_Release(m_pEquipedText);
	Safe_Release(m_pSelectEffect);

	__super::Free();
}
