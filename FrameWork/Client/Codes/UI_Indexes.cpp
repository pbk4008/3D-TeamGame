#include "pch.h"
#include "UI_Indexes.h"

#include "Button_Equipment.h"
#include "Button_Armory.h"
#include "Button_Skill.h"

CUI_Indexes::CUI_Indexes(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{
}

CUI_Indexes::CUI_Indexes(const CUI_Indexes& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Indexes::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Indexes::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	setActive(true);

	return S_OK;
}

_int CUI_Indexes::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_Indexes::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (m_pEquipmentBtn->getActive())
		m_pEquipmentBtn->LateTick(TimeDelta);

	/*if (m_pArmoryBtn->getActive())
		m_pArmoryBtn->LateTick(TimeDelta);

	if (m_pSkillBtn->getActive())
		m_pSkillBtn->LateTick(TimeDelta);*/

	return _int();
}

HRESULT CUI_Indexes::Render(void)
{
	if (m_pEquipmentBtn->getActive())
		m_pEquipmentBtn->Render();

	/*if (m_pArmoryBtn->getActive())
		m_pArmoryBtn->Render();

	if (m_pSkillBtn->getActive())
		m_pSkillBtn->Render();*/

	return S_OK;
}

HRESULT CUI_Indexes::Ready_UIObject(void)
{
	CButton_Equipment::Desc Equipment_Btn;
	{
		Equipment_Btn.fPos = { 100.f, 50.f };
		Equipment_Btn.fScale = { 100.f, 50.f };

		m_pEquipmentBtn = static_cast<CButton_Equipment*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Button_Equipment", &Equipment_Btn));
	}
	/*CButton_Armory::Desc Armory_Btn;
	{
		Armory_Btn.fPos = { 0.f, 0.f};
		Armory_Btn.fScale = { 100.f, 50.f };

		m_pArmoryBtn = static_cast<CButton_Armory*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Button_Armory", &Armory_Btn));
	}
	CButton_Skill::Desc Skill_Btn;
	{
		Skill_Btn.fPos = { 0.f, 0.f };
		Skill_Btn.fScale = { 100.f, 50.f };
		m_pSkillBtn = static_cast<CButton_Skill*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Button_Skill", &Skill_Btn));
	}

	assert(L"Failed to Create CUI_Indexes" && (m_pEquipmentBtn || m_pArmoryBtn || m_pSkillBtn));*/
	return S_OK;
}

void CUI_Indexes::Show(void)
{
	/* Show Menu Button*/
	m_pEquipmentBtn->setActive(true);
	/*m_pArmoryBtn->setActive(true);
	m_pSkillBtn->setActive(true);*/
}

void CUI_Indexes::Hide(void)
{
	/* Hide Menu Button*/
	m_pEquipmentBtn->setActive(false);
	//m_pArmoryBtn->setActive(false);
	//m_pSkillBtn->setActive(false);
}

void CUI_Indexes::MenuAllOff(void)
{
}

_bool CUI_Indexes::Click_Equipment(void)
{
	return m_pEquipmentBtn->ButtonClicked();
}

_bool CUI_Indexes::Click_Armory(void)
{
	return true;
}

_bool CUI_Indexes::Click_Skill(void)
{
	return _bool();
}

void CUI_Indexes::SetMother(CGameObject* _pObj)
{
}


CUI_Indexes* CUI_Indexes::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Indexes* pInstance = new CUI_Indexes(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Indexes Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Indexes::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Indexes* pInstance = new CUI_Indexes(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Indexes Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Indexes::Free()
{
	Safe_Release(m_pEquipmentBtn);
	//Safe_Release(m_pArmoryBtn);
	//Safe_Release(m_pSkillBtn);

	__super::Free();
}
