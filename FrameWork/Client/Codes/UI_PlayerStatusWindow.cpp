#include "pch.h"
#include "EquipmentData.h"
#include "UI_PlayerStatusWindow.h"
#include "UI_PlayerStatus_BG.h"
#include "UI_PlayerStatus_Might.h"
#include "UI_PlayerStatus_Spirit.h"
#include "UI_PlayerStatus_Viality.h"

#include "UI_PlayerStatus_DPS_1.h"
#include "UI_PlayerStatus_DPS_2.h"
#include "UI_PlayerStatus_MaxHealth.h"

CUI_PlayerStatusWindow::CUI_PlayerStatusWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_PlayerStatusWindow::CUI_PlayerStatusWindow(const CUI_PlayerStatusWindow& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PlayerStatusWindow::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerStatusWindow::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	m_pEquipData = g_pDataManager->GET_DATA(CEquipmentData, L"EquipmentData");

	setActive(false);

	return S_OK;
}

_int CUI_PlayerStatusWindow::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_PlayerStatusWindow::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	UpdateStatus();
	UpdateDPS();
	UpdateMaxHP();

	if (this->getActive())
	{
		m_pBG->LateTick(TimeDelta);
		m_pMight->LateTick(TimeDelta);
		m_pSpirit->LateTick(TimeDelta);
		m_pViality->LateTick(TimeDelta);
		m_pFirstDPS->LateTick(TimeDelta);
		m_pSecondDPS->LateTick(TimeDelta);
		m_pMaxHp->LateTick(TimeDelta);
	}

	return _int();
}

HRESULT CUI_PlayerStatusWindow::Render()
{
	if (FAILED(CUI::Render()))
		return -1;

	if (this->getActive())
	{
		m_pBG->Render();
		m_pMight->Render();
		m_pSpirit->Render();
		m_pViality->Render();
		m_pFirstDPS->Render();
		m_pSecondDPS->Render();
		m_pMaxHp->Render();
	}

	return S_OK;
}

HRESULT CUI_PlayerStatusWindow::Ready_Component(void)
{
	return S_OK;
}

HRESULT CUI_PlayerStatusWindow::Ready_UIObject(void)
{
	desc.pOwner = this;

	m_pBG      = static_cast<CUI_PlayerStatus_BG*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_PlayerStatus_BG", &desc));
	m_pMight   = static_cast<CUI_PlayerStatus_Might*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_PlayerStatus_Might", &desc));
	m_pSpirit  = static_cast<CUI_PlayerStatus_Spirit*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_PlayerStatus_Spirit", &desc));
	m_pViality = static_cast<CUI_PlayerStatus_Viality*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_PlayerStatus_Viality", &desc));

	m_pFirstDPS  = static_cast<CUI_PlayerStatus_DPS_1*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_PlayerStatus_FirstDPS", &desc));
	m_pSecondDPS = static_cast<CUI_PlayerStatus_DPS_2*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_PlayerStatus_SecondDPS", &desc));
	m_pMaxHp     = static_cast<CUI_PlayerStatus_MAXHP*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_PlayerStatus_MaxHp", &desc));
		
	return S_OK;
}

void CUI_PlayerStatusWindow::UpdateStatus(void)
{
	/* 근력 */
	if (m_pEquipData->IsExistEquip(EEquipSlot::Banner))
	{
		CItemData pIteamData = m_pEquipData->GetEquipment(EEquipSlot::Banner);
		m_pMight->SetIcon(pIteamData.szStatusName);
	}
	else
		m_pMight->SetIcon(L"T_STR_45");


	/* 정신력 */
	if (m_pEquipData->IsExistEquip(EEquipSlot::Ring1))
	{
		CItemData pIteamData = m_pEquipData->GetEquipment(EEquipSlot::Ring1);
		m_pSpirit->SetIcon(pIteamData.szStatusName);
	}
	else
		m_pSpirit->SetIcon(L"T_INT_40");

	/* 활력 */
	if (m_pEquipData->IsExistEquip(EEquipSlot::Charm))
	{
		CItemData pIteamData = m_pEquipData->GetEquipment(EEquipSlot::Charm);
		m_pViality->SetIcon(pIteamData.szStatusName);
	}
	else
		m_pViality->SetIcon(L"T_DEX_45");

}

void CUI_PlayerStatusWindow::UpdateDPS(void)
{
	/* First Weapon */
	if (m_pEquipData->IsExistEquip(EEquipSlot::Weapon1))
	{
		CItemData pIteamData = m_pEquipData->GetEquipment(EEquipSlot::Weapon1);
		m_pFirstDPS->SetIcon(pIteamData.szStatusName);
	}
	else
		m_pFirstDPS->SetIcon(L"T_DPS_0");

	/* First Weapon */
	if (m_pEquipData->IsExistEquip(EEquipSlot::Weapon2))
	{
		CItemData pIteamData = m_pEquipData->GetEquipment(EEquipSlot::Weapon2);
		m_pSecondDPS->SetIcon(pIteamData.szStatusName);
	}
	else
		m_pSecondDPS->SetIcon(L"T_DPS_0");
}

void CUI_PlayerStatusWindow::UpdateMaxHP(void)
{
	/* MAX HP */
	if (m_pEquipData->IsExistEquip(EEquipSlot::LifeStone))
	{
		CItemData pIteamData = m_pEquipData->GetEquipment(EEquipSlot::LifeStone);
		m_pMaxHp->SetIcon(pIteamData.szStatusName);
	}
	else
		m_pMaxHp->SetIcon(L"T_Max_HP_200");
}

void CUI_PlayerStatusWindow::SetActiveAll(_bool _OnOff)
{
	this->setActive(_OnOff);
}

void CUI_PlayerStatusWindow::Show(void)
{
	setActive(true);
}

void CUI_PlayerStatusWindow::Hide(void)
{
	setActive(false);
}

CUI_PlayerStatusWindow* CUI_PlayerStatusWindow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_PlayerStatusWindow* pInstance = new CUI_PlayerStatusWindow(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_PlayerStatusWindow Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerStatusWindow::Clone(const _uint iSceneID, void* pArg)
{
	CUI_PlayerStatusWindow* pInstance = new CUI_PlayerStatusWindow(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_PlayerStatusWindow Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_PlayerStatusWindow::Free()
{
	Safe_Release(m_pBG);
	Safe_Release(m_pMight);
	Safe_Release(m_pSpirit);
	Safe_Release(m_pViality);
	Safe_Release(m_pFirstDPS);
	Safe_Release(m_pSecondDPS);
	Safe_Release(m_pMaxHp);

	__super::Free();
}
