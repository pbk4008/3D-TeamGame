#include "pch.h"
#include "UI_ItemStatusWindow.h"
#include "SingleImage.h"
#include "ItemData.h"
#include "UI_ItemStatusBackground.h"

CUI_ItemStatusWindow::CUI_ItemStatusWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{
}

CUI_ItemStatusWindow::CUI_ItemStatusWindow(const CUI_ItemStatusWindow& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ItemStatusWindow::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ItemStatusWindow::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CUI_ItemStatusWindow::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_ItemStatusWindow::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	if (this->getActive())
		m_pBg->LateTick(TimeDelta);

	for (auto iter : m_mapDefaultUIs)
	{
		if (iter.second->getActive())
		{
			iter.second->LateTick(TimeDelta);
		}
	}

	return _int();
}

HRESULT CUI_ItemStatusWindow::Render()
{

	if (this->getActive())
		m_pBg->Render();

	for (auto iter : m_mapDefaultUIs)
	{
		if (iter.second->getActive())
		{
			iter.second->Render();
		}
	}

	return S_OK;
}

HRESULT CUI_ItemStatusWindow::Ready_Component(void)
{
	return S_OK;
}

HRESULT CUI_ItemStatusWindow::Ready_UIObject(void)
{
	m_pBg = static_cast<UI_ItemStatusBackground*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_ItemStatus_BG"));
	m_pBgSprite = m_pBg->GetSingleImage();
	InsertDefaultUIs(L"Bg", m_pBg);

	return S_OK;
}

void CUI_ItemStatusWindow::Show(CItemData* _pItem)
{
	setActive(true);

	if (!m_bIsOpen)
	{
		for (auto& obj : m_mapDefaultUIs)
		{
			obj.second->setActive(true);
		}
	}
	m_pSelectItem = _pItem;
	SettingItemStatus();
	m_bIsOpen = true;
}

void CUI_ItemStatusWindow::Hide()
{
	setActive(false);

	if (m_bIsOpen)
	{
		for (auto& obj : m_mapDefaultUIs)
		{
			obj.second->setActive(false);
		}

		m_pSelectItem = nullptr;
		m_bIsOpen = false;
	}
}

void CUI_ItemStatusWindow::SettingItemStatus()
{
	assert(L"Item Status Data is null" && m_pSelectItem);
	
	/* for. Background */
	m_pBgSprite->SetTexture(g_arrGradeStatusBgName[(_int)m_pSelectItem->equipmentGrade]);

	/* for. Level */
	/* for. Name */
	/* for. Grade& Kind */
	/* for.MainStat */
	/* for. MainStatKind */
}

wstring CUI_ItemStatusWindow::GradeString(EEquipmentGrade _eGrade)
{
	return wstring();
}

wstring CUI_ItemStatusWindow::TypeString(EEquipmentType _eType)
{
	return wstring();
}

CGameObject* CUI_ItemStatusWindow::GetUI(const wstring& _szUIName)
{
	return nullptr;
}

void CUI_ItemStatusWindow::InsertDefaultUIs(const wstring& _szName, CGameObject* _pObj)
{
	m_mapDefaultUIs.emplace(_szName, _pObj);
}

CUI_ItemStatusWindow* CUI_ItemStatusWindow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_ItemStatusWindow* pInstance = new CUI_ItemStatusWindow(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_ItemStatusWindow Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ItemStatusWindow::Clone(const _uint iSceneID, void* pArg)
{
	CUI_ItemStatusWindow* pInstance = new CUI_ItemStatusWindow(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_ItemStatusWindow Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ItemStatusWindow::Free()
{
	Safe_Release(m_pBg);

	__super::Free();
}
