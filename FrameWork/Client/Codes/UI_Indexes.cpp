#include "pch.h"
#include "UI_Indexes.h"

#include "Button_Equipment.h"
#include "Button_Armory.h"
#include "Button_Skill.h"
#include "IndexIndicator.h"

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
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_Indexes::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (m_fInitPos > m_fEndPos)
	{
		m_fInitPos -= (_float)TimeDelta * 100.f;
		if (m_fInitPos < m_fEndPos)
		{
			m_fInitPos = m_fEndPos;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ m_fInitPos, 0.f, 0.f, 1.f });
	}

	if (m_bClickArmory)
		m_pIndexIndicator->SetMove(true);
	if (m_bClickEquipment)
		m_pIndexIndicator->SetMove(false);

	if (m_pEquipmentBtn->getActive())
		m_pEquipmentBtn->LateTick(TimeDelta);

	if (m_pArmoryBtn->getActive())
		m_pArmoryBtn->LateTick(TimeDelta);

	/*if (m_pSkillBtn->getActive())
		m_pSkillBtn->LateTick(TimeDelta);*/

	if (m_pIndexIndicator->getActive())
		m_pIndexIndicator->LateTick(TimeDelta);


	return _int();
}

HRESULT CUI_Indexes::Render(void)
{
	if (m_pEquipmentBtn->getActive())
		m_pEquipmentBtn->Render();

	if (m_pArmoryBtn->getActive())
		m_pArmoryBtn->Render();

	/*if (m_pSkillBtn->getActive())
		m_pSkillBtn->Render();*/

	if (m_pIndexIndicator->getActive())
		m_pIndexIndicator->Render();

	return S_OK;
}

void CUI_Indexes::setActive(_bool bActive)
{
	this->m_bActive = bActive;

	if (false == bActive)
		m_fInitPos = 10.f;
}

HRESULT CUI_Indexes::Ready_UIObject(void)
{
	CButton_Equipment::Desc Equipment_Btn;
	{
		Equipment_Btn.fPos = { -820.f, 450.f };
		Equipment_Btn.fScale = { 90.f, 45.f };
		Equipment_Btn.pOwner = this;

		m_pEquipmentBtn = static_cast<CButton_Equipment*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Button_Equipment", &Equipment_Btn));
		assert("Failed to Create Proto_GameObject_UI_Button_Equipment" && m_pEquipmentBtn);
	}
	CButton_Armory::Desc Armory_Btn;
	{
		Armory_Btn.fPos = { -680.f, 450.f };
		Armory_Btn.fScale = { 90.f, 45.f };
		Armory_Btn.pOwner = this;

		m_pArmoryBtn = static_cast<CButton_Armory*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Button_Armory", &Armory_Btn));
		assert("Failed to Create Proto_GameObject_UI_Button_Armory" && m_pArmoryBtn);
	}
	//CButton_Skill::Desc Skill_Btn;
	//{
	//	Skill_Btn.fPos = { 0.f, 0.f };
	//	Skill_Btn.fScale = { 100.f, 50.f };
	//	m_pSkillBtn = static_cast<CButton_Skill*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Button_Skill", &Skill_Btn));
	//}
	CIndexIndicator::Desc desc;
	{
		desc.fPos = { -450.f, 300.f };
		desc.fScale = { 124.f, 124.f };
		desc.pOwner = this;

		m_pIndexIndicator = static_cast<CIndexIndicator*>(g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_IndexIndicator", &desc));
		assert("Failed to Create Proto_GameObject_UI_IndexIndicator" && m_pIndexIndicator);
	}

	return S_OK;
}

void CUI_Indexes::Show(void)
{
	setActive(true);
	/* Show Menu Button*/
	m_pEquipmentBtn->setActive(true);
	m_pArmoryBtn->setActive(true);
	//m_pSkillBtn->setActive(true);
	m_pIndexIndicator->setActive(true);
}

void CUI_Indexes::Hide(void)
{
	setActive(false);
	/* Hide Menu Button*/
	m_pEquipmentBtn->setActive(false);
	m_pArmoryBtn->setActive(false);
	//m_pSkillBtn->setActive(false);

	m_bClickArmory = false;
	m_bClickEquipment = false;

	m_pIndexIndicator->setActive(false);
	m_pIndexIndicator->ResetXPos();
}

void CUI_Indexes::MenuAllOff(void)
{
}

_bool CUI_Indexes::Click_Equipment(void)
{
	if (false == m_bClickEquipment) /* 클릭 중복 이벤트 방지 */
	{
		if (m_pEquipmentBtn->ButtonClicked())
		{
			STOP_SOUND(CHANNEL::EFFECT);
			PLAY_SOUND(L"UI_tab_over", CHANNEL::EFFECT);
			m_pIndexIndicator->SetInitEquipXPos(-820.f);
			m_bClickEquipment = true;
			m_bClickArmory = false;
			return true;
		}
	}
	return false;
}

_bool CUI_Indexes::Click_Armory(void)
{
	if (false == m_bClickArmory) /* 클릭 중복 이벤트 방지 */
	{
		if (m_pArmoryBtn->ButtonClicked())
		{
			STOP_SOUND(CHANNEL::EFFECT);
			PLAY_SOUND(L"UI_tab_over", CHANNEL::EFFECT);
			m_pIndexIndicator->SetInitArmoryXPos(-680.f);
			m_bClickEquipment = false;
			m_bClickArmory = true;
			return true;
		}
	}
	return false;
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
	Safe_Release(m_pArmoryBtn);
	//Safe_Release(m_pSkillBtn);
	Safe_Release(m_pIndexIndicator);

	__super::Free();
}
