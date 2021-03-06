#include "pch.h"
#include "UI_ModalWindow.h"
#include "SingleImage.h"
#include "UI_Indexes.h"
#include "UI_Background.h"
#include "UI_Equipment.h"
#include "UI_Armory.h"

CUI_ModalWindow::CUI_ModalWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{
}

CUI_ModalWindow::CUI_ModalWindow(const CUI_ModalWindow& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ModalWindow::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ModalWindow::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if(FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;
	
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

_int CUI_ModalWindow::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	if (m_pIndex->Click_Equipment())
	{
		if (false == m_pEquipment->GetEquipmentActive())
		{
			ShutTheUI();
			m_pEquipment->Show();
		}
	}

	if (m_pIndex->Click_Armory())
	{
		if (false == m_pArmory->GetArmoryActive())
		{
			ShutTheUI();
			m_pArmory->Show();
		}
	}

	if (m_pEquipment->getActive())
		m_pEquipment->Tick(dDeltaTime);

	if (m_pArmory->getActive())
		m_pArmory->Tick(dDeltaTime);

	return _int();
}

_int CUI_ModalWindow::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	if (m_pIndex->getActive())
		m_pIndex->LateTick(TimeDelta);

	if(m_pBG->getActive())
		m_pBG->LateTick(TimeDelta);

	if (m_pEquipment->getActive())
		m_pEquipment->LateTick(TimeDelta);

	if (m_pArmory->getActive())
		m_pArmory->LateTick(TimeDelta);

	return _int();
}

HRESULT CUI_ModalWindow::Render()
{
	if (this->getActive())
	{
		if (m_pIndex->getActive())
			m_pIndex->Render();

		if (m_pBG->getActive())
			m_pBG->Render();

		if (m_pEquipment->getActive())
			m_pEquipment->Render();

		if (m_pArmory->getActive())
			m_pArmory->Render();
	}
	return S_OK;
}

HRESULT CUI_ModalWindow::Ready_Component(void)
{
	return S_OK;
}

HRESULT CUI_ModalWindow::Ready_UIObject(void)
{
	m_pBG		 = static_cast<CUI_Background*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Background"));
	m_pIndex	 = static_cast<CUI_Indexes*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Indexes"));
	m_pEquipment = static_cast<CUI_Equipment*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Equipment"));
	m_pArmory	 = static_cast<CUI_Armory*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Armory"));

	assert("Failed to Create UI Object in Modal Window" && (m_pBG || m_pIndex || m_pEquipment || m_pArmory));

	return S_OK;
}

void CUI_ModalWindow::Show(void)
{
	this->setActive(true);

	m_pIndex->Show();			 /* ???? ?????? */
	m_pBG->setActive(true);		 /* ???? ???? */

	m_pEquipment->Show();		 /* ???? ??  */
}

void CUI_ModalWindow::Hide(void)
{
	this->setActive(false);

	m_pIndex->Hide();			
	m_pBG->setActive(false);	

	m_pEquipment->Hide();		
	m_pArmory->Hide();		
}	

void CUI_ModalWindow::ShutTheUI(void)
{
	m_pEquipment->Hide();
	m_pArmory->Hide();
	//m_pSkills->Hide();
}

_bool CUI_ModalWindow::OpenArmory(void)
{
	return m_pArmory->GetArmoryActive();
}

_bool CUI_ModalWindow::OpenEquipment(void)
{
	return m_pEquipment->GetEquipmentActive();
}

CUI_Indexes* CUI_ModalWindow::GetIndex(void)
{
	return nullptr;
}

CUI_Equipment* CUI_ModalWindow::GeEquipment(void)
{
	return nullptr;
}

CUI_Armory* CUI_ModalWindow::GetArmory(void)
{
	return nullptr;
}

void CUI_ModalWindow::SetIndex(CUI_Indexes* _pIndex)
{
}

void CUI_ModalWindow::SetEquipment(CUI_Equipment* _pEquipment)
{
}

void CUI_ModalWindow::SetArmory(CUI_Armory* _pArmory)
{
}

CUI_ModalWindow* CUI_ModalWindow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_ModalWindow* pInstance = new CUI_ModalWindow(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_ModalWindow Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ModalWindow::Clone(const _uint iSceneID, void* pArg)
{
	CUI_ModalWindow* pInstance = new CUI_ModalWindow(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_ModalWindow Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ModalWindow::Free()
{
	Safe_Release(m_pEquipment);
	Safe_Release(m_pArmory);
	Safe_Release(m_pBG);
	Safe_Release(m_pIndex);

	__super::Free();
}
