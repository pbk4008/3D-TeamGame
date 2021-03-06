#include "framework.h"
#include "pch.h"
#include "UI_Monster_Panel.h"
#include "GameInstance.h"

#include "Camera_Silvermane.h"
#include "Camera.h"

#include "UI_Monster_Back.h"
#include "UI_Monster_Level.h"
#include "UI_Monster_LevelNum.h"
#include "UI_Monster_HpBar.h"
#include "UI_Monster_BackHpBar.h"
#include "UI_Monster_Name.h"
#include "UI_Monster_GroggyBar.h"

CUI_Monster_Panel::CUI_Monster_Panel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
	ZeroMemory(&m_PanelDesc, sizeof(m_PanelDesc));
	ZeroMemory(&m_TargetMatrix, sizeof(_float4x4));

}

CUI_Monster_Panel::CUI_Monster_Panel(const CUI_Monster_Panel& rhs)
	: CUI(rhs)
	, m_PanelDesc(rhs.m_PanelDesc)
	, m_TargetMatrix(rhs.m_TargetMatrix)
	, m_pUIBack(rhs.m_pUIBack)
	, m_pUIHpBar(rhs.m_pUIHpBar)
	, m_pUIBackHpBar(rhs.m_pUIBackHpBar)
	, m_pUILevel(rhs.m_pUILevel)
	, m_pUILevelNum(rhs.m_pUILevelNum)
	, m_pUIName(rhs.m_pUIName)
	, m_pUIGroggyBar(rhs.m_pUIGroggyBar)
{
	Safe_AddRef(m_pUIBack);
	Safe_AddRef(m_pUIHpBar);
	Safe_AddRef(m_pUIBackHpBar);
	Safe_AddRef(m_pUILevel);
	Safe_AddRef(m_pUILevelNum);
	Safe_AddRef(m_pUIName);
	Safe_AddRef(m_pUIGroggyBar);
}

HRESULT CUI_Monster_Panel::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Monster_Panel::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_PanelDesc, pArg, sizeof(PANELDESC));
	}

	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	m_EnemyTag = m_PanelDesc.iEnemyTag; //태그값 받아온걸로 어떤거 생성할지 정해줌 

	if (FAILED(Panel_Setting())) //생성 
		return E_FAIL;

	Safe_AddRef(m_pUIBack);
	Safe_AddRef(m_pUILevel);
	Safe_AddRef(m_pUILevelNum);
	Safe_AddRef(m_pUIHpBar);
	Safe_AddRef(m_pUIBackHpBar);
	Safe_AddRef(m_pUIName);
	Safe_AddRef(m_pUIGroggyBar);

	m_bShow = false;

	m_pUIBack->setActive(false);
	m_pUILevel->setActive(false);
	m_pUILevelNum->setActive(false);
	m_pUIName->setActive(false);
	m_pUIHpBar->setActive(false);
	m_pUIBackHpBar->setActive(false);
	m_pUIGroggyBar->setActive(false);

	setActive(false);

	return S_OK;
}

_int CUI_Monster_Panel::Tick(_double TimeDelta)
{
	if (FAILED(__super::Tick(TimeDelta)))
		return -1;

	if (true == g_pObserver->Get_Player()->Get_Dead())
	{
		m_bShow = false;
	}
	Update_Panel(TimeDelta);


	return 0;
}

_int CUI_Monster_Panel::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHANB, this);
	}
	return _int();
}

HRESULT CUI_Monster_Panel::Render()
{
	if (m_bShow)
	{
		m_pUIBack->setActive(true);
		m_pUILevel->setActive(true);
		m_pUILevelNum->setActive(true);
		m_pUIName->setActive(true);
		m_pUIHpBar->setActive(true);
		m_pUIBackHpBar->setActive(true);
		m_pUIGroggyBar->setActive(true);

		m_pUIBack->Set_Show(true);
		m_pUILevel->Set_Show(true);
		m_pUILevelNum->Set_Show(true);
		m_pUIName->Set_Show(true);
		m_pUIHpBar->Set_Show(true);
		m_pUIBackHpBar->Set_Show(true);
		m_pUIGroggyBar->Set_Show(true);
	}
	if (!m_bShow)
	{
		m_pUIBack->Set_Show(false);
		m_pUILevel->Set_Show(false);
		m_pUILevelNum->Set_Show(false);
		m_pUIName->Set_Show(false);
		m_pUIHpBar->Set_Show(false);
		m_pUIBackHpBar->Set_Show(false);
		m_pUIGroggyBar->Set_Show(false);
	}
	return S_OK;
}

void CUI_Monster_Panel::Set_HpBar(_float fRatio)
{
	m_pUIHpBar->Set_HpRatio(fRatio);
	m_pUIBackHpBar->Set_HpRatio(fRatio);
}

void CUI_Monster_Panel::Set_GroggyBar(_float fRatio)
{
	m_pUIGroggyBar->Set_GroggyRatio(fRatio);
}

void CUI_Monster_Panel::Set_BackUIGapY(_float GapY)
{
	m_pUIBack->Set_GapY(GapY);
}

HRESULT CUI_Monster_Panel::Panel_Setting()
{
	switch (m_EnemyTag)
	{
	case Enemy::CRAWLER:
		if (FAILED(Setting_Crawler()))
			return E_FAIL;
		break;
	case Enemy::ABERRANT:
		if (FAILED(Setting_Aberrant()))
			return E_FAIL;
		break;
	case Enemy::ANIMUS:
		if (FAILED(Setting_Animus()))
			return E_FAIL;
		break;
	case Enemy::SWORD:
		if (FAILED(Setting_Sword()))
			return E_FAIL;
		break;
	case Enemy::SPEAR:
		if (FAILED(Setting_Spear()))
			return E_FAIL;
		break;
	case Enemy::SHOOTER:
		if (FAILED(Setting_Shooter()))
			return E_FAIL;
		break;
	case Enemy::HEALER:
		if (FAILED(Setting_Healer()))
			return E_FAIL;
		break;
	case Enemy::SWORD2H:
		if (FAILED(Setting_2HSword()))
			return E_FAIL;
		break;
	case Enemy::MIDBOSS:
		if (FAILED(Setting_MidBoss()))
			return E_FAIL;
		break;
	case Enemy::ENDBOSS:
		if (FAILED(Setting_EndBoss()))
			return E_FAIL;
		break;
	case Enemy::ENEMY_END:
		MSGBOX("Failed to Panel Setting In CUI_Monster_Panel::Panel_Setting()");
		break;
	default:
		MSGBOX("Failed to Panel Setting In CUI_Monster_Panel::Panel_Setting()");
		break;
	}
	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_Crawler()
{
	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.38f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };
	
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc,
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar LevelNum
	CUI_Monster_LevelNum::UIACTIVEDESC Desc1;
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Monster_LevelNum");
	Desc1.UIDesc.IDTag = 13;
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc1.UIDesc.fSize = { 1.f, 1.f };
	Desc1.iTextureNum = 1;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_LevelNum", &Desc1,
		(CGameObject**)&m_pUILevelNum)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level_1");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2,
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;


	//MonsterBar Bar
	CUI_Monster_HpBar::UIBARDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.36f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };
	Desc3.iRenderPass = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;

	//Monster BackHpBar
	CUI_Monster_BackHpBar::UIBARDESC Desc3_1;
	_tcscpy_s(Desc3_1.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3_1.UIDesc.IDTag = 14;
	Desc3_1.UIDesc.bMinus = true;
	Desc3_1.UIDesc.fAngle = 0.36f;
	Desc3_1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3_1.UIDesc.fSize = { 1.f, 1.f };
	Desc3_1.iRenderPass = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_BackHpBar", &Desc3_1,
		(CGameObject**)&m_pUIBackHpBar)))
		return E_FAIL;


	//MonsterBar Name
	CUI_Monster_Name::UINAMEDESC Desc4;
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Monster_Name");
	Desc4.UIDesc.IDTag = 13;
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc4.UIDesc.fSize = { 1.f, 1.f };
	Desc4.iTextureNum = 0;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Name", &Desc4,
		(CGameObject**)&m_pUIName)))
		return E_FAIL;

	//MonsterBar GroggyBar
	CUI_Monster_GroggyBar::UIBARDESC Desc5;
	_tcscpy_s(Desc5.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc5.UIDesc.IDTag = 14;
	Desc5.UIDesc.bMinus = true;
	Desc5.UIDesc.fAngle = 0.43f;
	Desc5.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc5.UIDesc.fSize = { 1.f, 1.f };
	Desc5.iRenderPass = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_GroggyBar", &Desc5,
		(CGameObject**)&m_pUIGroggyBar)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_Aberrant()
{
	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.38f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc,
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar LevelNum
	CUI_Monster_LevelNum::UIACTIVEDESC Desc1;
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Monster_LevelNum");
	Desc1.UIDesc.IDTag = 13;
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc1.UIDesc.fSize = { 1.f, 1.f };
	Desc1.iTextureNum = 2;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_LevelNum", &Desc1,
		(CGameObject**)&m_pUILevelNum)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level_1");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2,
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;

	//MonsterBar Bar
	CUI_Monster_HpBar::UIBARDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.36f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };
	Desc3.iRenderPass = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;

	//Monster BackHpBar
	CUI_Monster_BackHpBar::UIBARDESC Desc3_1;
	_tcscpy_s(Desc3_1.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3_1.UIDesc.IDTag = 14;
	Desc3_1.UIDesc.bMinus = true;
	Desc3_1.UIDesc.fAngle = 0.36f;
	Desc3_1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3_1.UIDesc.fSize = { 1.f, 1.f };
	Desc3_1.iRenderPass = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_BackHpBar", &Desc3_1,
		(CGameObject**)&m_pUIBackHpBar)))
		return E_FAIL;

	//MonsterBar Name
	CUI_Monster_Name::UINAMEDESC Desc4;
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Monster_Name");
	Desc4.UIDesc.IDTag = 13;
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc4.UIDesc.fSize = { 1.f, 1.f };
	Desc4.iTextureNum = 1;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Name", &Desc4,
		(CGameObject**)&m_pUIName)))
		return E_FAIL;

	//MonsterBar GroggyBar
	CUI_Monster_GroggyBar::UIBARDESC Desc5;
	_tcscpy_s(Desc5.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc5.UIDesc.IDTag = 14;
	Desc5.UIDesc.bMinus = true;
	Desc5.UIDesc.fAngle = 0.43f;
	Desc5.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc5.UIDesc.fSize = { 1.f, 1.f };
	Desc5.iRenderPass = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_GroggyBar", &Desc5,
		(CGameObject**)&m_pUIGroggyBar)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_Animus()
{
	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.38f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc,
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar LevelNum
	CUI_Monster_LevelNum::UIACTIVEDESC Desc1;
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Monster_LevelNum");
	Desc1.UIDesc.IDTag = 13;
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc1.UIDesc.fSize = { 1.f, 1.f };
	Desc1.iTextureNum = 8;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_LevelNum", &Desc1,
		(CGameObject**)&m_pUILevelNum)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level_2");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2,
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;

	//MonsterBar Bar
	CUI_Monster_HpBar::UIBARDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.36f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };
	Desc3.iRenderPass = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;

	//Monster BackHpBar
	CUI_Monster_BackHpBar::UIBARDESC Desc3_1;
	_tcscpy_s(Desc3_1.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3_1.UIDesc.IDTag = 14;
	Desc3_1.UIDesc.bMinus = true;
	Desc3_1.UIDesc.fAngle = 0.36f;
	Desc3_1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3_1.UIDesc.fSize = { 1.f, 1.f };
	Desc3_1.iRenderPass = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_BackHpBar", &Desc3_1,
		(CGameObject**)&m_pUIBackHpBar)))
		return E_FAIL;

	//MonsterBar Name
	CUI_Monster_Name::UINAMEDESC Desc4;
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Monster_Name");
	Desc4.UIDesc.IDTag = 13;
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc4.UIDesc.fSize = { 1.f, 1.f };
	Desc4.iTextureNum = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Name", &Desc4,
		(CGameObject**)&m_pUIName)))
		return E_FAIL;

	//MonsterBar GroggyBar
	CUI_Monster_GroggyBar::UIBARDESC Desc5;
	_tcscpy_s(Desc5.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc5.UIDesc.IDTag = 14;
	Desc5.UIDesc.bMinus = true;
	Desc5.UIDesc.fAngle = 0.43f;
	Desc5.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc5.UIDesc.fSize = { 1.f, 1.f };
	Desc5.iRenderPass = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_GroggyBar", &Desc5,
		(CGameObject**)&m_pUIGroggyBar)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_Sword()
{
	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.38f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc,
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar LevelNum
	CUI_Monster_LevelNum::UIACTIVEDESC Desc1;
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Monster_LevelNum");
	Desc1.UIDesc.IDTag = 13;
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc1.UIDesc.fSize = { 1.f, 1.f };
	Desc1.iTextureNum = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_LevelNum", &Desc1,
		(CGameObject**)&m_pUILevelNum)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level_2");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2,
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;

	//MonsterBar Bar
	CUI_Monster_HpBar::UIBARDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.36f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };
	Desc3.iRenderPass = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;

	//Monster BackHpBar
	CUI_Monster_BackHpBar::UIBARDESC Desc3_1;
	_tcscpy_s(Desc3_1.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3_1.UIDesc.IDTag = 14;
	Desc3_1.UIDesc.bMinus = true;
	Desc3_1.UIDesc.fAngle = 0.36f;
	Desc3_1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3_1.UIDesc.fSize = { 1.f, 1.f };
	Desc3_1.iRenderPass = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_BackHpBar", &Desc3_1,
		(CGameObject**)&m_pUIBackHpBar)))
		return E_FAIL;


	//MonsterBar Name
	CUI_Monster_Name::UINAMEDESC Desc4;
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Monster_Name");
	Desc4.UIDesc.IDTag = 13;
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc4.UIDesc.fSize = { 1.f, 1.f };
	Desc4.iTextureNum = 2;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Name", &Desc4,
		(CGameObject**)&m_pUIName)))
		return E_FAIL;

	//MonsterBar GroggyBar
	CUI_Monster_GroggyBar::UIBARDESC Desc5;
	_tcscpy_s(Desc5.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc5.UIDesc.IDTag = 14;
	Desc5.UIDesc.bMinus = true;
	Desc5.UIDesc.fAngle = 0.43f;
	Desc5.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc5.UIDesc.fSize = { 1.f, 1.f };
	Desc5.iRenderPass = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_GroggyBar", &Desc5,
		(CGameObject**)&m_pUIGroggyBar)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_Spear()
{
	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.38f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc,
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar LevelNum
	CUI_Monster_LevelNum::UIACTIVEDESC Desc1;
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Monster_LevelNum");
	Desc1.UIDesc.IDTag = 13;
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc1.UIDesc.fSize = { 1.f, 1.f };
	Desc1.iTextureNum = 8;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_LevelNum", &Desc1,
		(CGameObject**)&m_pUILevelNum)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level_2");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2,
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;

	//MonsterBar Bar
	CUI_Monster_HpBar::UIBARDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.36f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };
	Desc3.iRenderPass = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;

	//Monster BackHpBar
	CUI_Monster_BackHpBar::UIBARDESC Desc3_1;
	_tcscpy_s(Desc3_1.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3_1.UIDesc.IDTag = 14;
	Desc3_1.UIDesc.bMinus = true;
	Desc3_1.UIDesc.fAngle = 0.36f;
	Desc3_1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3_1.UIDesc.fSize = { 1.f, 1.f };
	Desc3_1.iRenderPass = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_BackHpBar", &Desc3_1,
		(CGameObject**)&m_pUIBackHpBar)))
		return E_FAIL;


	//MonsterBar Name
	CUI_Monster_Name::UINAMEDESC Desc4;
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Monster_Name");
	Desc4.UIDesc.IDTag = 13;
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc4.UIDesc.fSize = { 1.f, 1.f };
	Desc4.iTextureNum = 7;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Name", &Desc4,
		(CGameObject**)&m_pUIName)))
		return E_FAIL;

	//MonsterBar GroggyBar
	CUI_Monster_GroggyBar::UIBARDESC Desc5;
	_tcscpy_s(Desc5.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc5.UIDesc.IDTag = 14;
	Desc5.UIDesc.bMinus = true;
	Desc5.UIDesc.fAngle = 0.43f;
	Desc5.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc5.UIDesc.fSize = { 1.f, 1.f };
	Desc5.iRenderPass = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_GroggyBar", &Desc5,
		(CGameObject**)&m_pUIGroggyBar)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_Shooter()
{
	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.38f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc,
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar LevelNum
	CUI_Monster_LevelNum::UIACTIVEDESC Desc1;
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Monster_LevelNum");
	Desc1.UIDesc.IDTag = 13;
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc1.UIDesc.fSize = { 1.f, 1.f };
	Desc1.iTextureNum = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_LevelNum", &Desc1,
		(CGameObject**)&m_pUILevelNum)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level_2");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2,
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;

	//MonsterBar Bar
	CUI_Monster_HpBar::UIBARDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.36f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };
	Desc3.iRenderPass = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;
	//Monster BackHpBar
	CUI_Monster_BackHpBar::UIBARDESC Desc3_1;
	_tcscpy_s(Desc3_1.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3_1.UIDesc.IDTag = 14;
	Desc3_1.UIDesc.bMinus = true;
	Desc3_1.UIDesc.fAngle = 0.36f;
	Desc3_1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3_1.UIDesc.fSize = { 1.f, 1.f };
	Desc3_1.iRenderPass = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_BackHpBar", &Desc3_1,
		(CGameObject**)&m_pUIBackHpBar)))
		return E_FAIL;



	//MonsterBar Name
	CUI_Monster_Name::UINAMEDESC Desc4;
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Monster_Name");
	Desc4.UIDesc.IDTag = 13;
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc4.UIDesc.fSize = { 1.f, 1.f };
	Desc4.iTextureNum = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Name", &Desc4,
		(CGameObject**)&m_pUIName)))
		return E_FAIL;

	//MonsterBar GroggyBar
	CUI_Monster_GroggyBar::UIBARDESC Desc5;
	_tcscpy_s(Desc5.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc5.UIDesc.IDTag = 14;
	Desc5.UIDesc.bMinus = true;
	Desc5.UIDesc.fAngle = 0.43f;
	Desc5.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc5.UIDesc.fSize = { 1.f, 1.f };
	Desc5.iRenderPass = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_GroggyBar", &Desc5,
		(CGameObject**)&m_pUIGroggyBar)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_Healer()
{
	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.38f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc,
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar LevelNum
	CUI_Monster_LevelNum::UIACTIVEDESC Desc1;
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Monster_LevelNum");
	Desc1.UIDesc.IDTag = 13;
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc1.UIDesc.fSize = { 1.f, 1.f };
	Desc1.iTextureNum = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_LevelNum", &Desc1,
		(CGameObject**)&m_pUILevelNum)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level_2");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2,
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;

	//MonsterBar Bar
	CUI_Monster_HpBar::UIBARDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.36f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };
	Desc3.iRenderPass = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;

	//Monster BackHpBar
	CUI_Monster_BackHpBar::UIBARDESC Desc3_1;
	_tcscpy_s(Desc3_1.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3_1.UIDesc.IDTag = 14;
	Desc3_1.UIDesc.bMinus = true;
	Desc3_1.UIDesc.fAngle = 0.36f;
	Desc3_1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3_1.UIDesc.fSize = { 1.f, 1.f };
	Desc3_1.iRenderPass = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_BackHpBar", &Desc3_1,
		(CGameObject**)&m_pUIBackHpBar)))
		return E_FAIL;


	//MonsterBar Name
	CUI_Monster_Name::UINAMEDESC Desc4;
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Monster_Name");
	Desc4.UIDesc.IDTag = 13;
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc4.UIDesc.fSize = { 1.f, 1.f };
	Desc4.iTextureNum = 3;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Name", &Desc4,
		(CGameObject**)&m_pUIName)))
		return E_FAIL;

	//MonsterBar GroggyBar
	CUI_Monster_GroggyBar::UIBARDESC Desc5;
	_tcscpy_s(Desc5.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc5.UIDesc.IDTag = 14;
	Desc5.UIDesc.bMinus = true;
	Desc5.UIDesc.fAngle = 0.43f;
	Desc5.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc5.UIDesc.fSize = { 1.f, 1.f };
	Desc5.iRenderPass = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_GroggyBar", &Desc5,
		(CGameObject**)&m_pUIGroggyBar)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_2HSword()
{
	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.38f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc,
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar LevelNum
	CUI_Monster_LevelNum::UIACTIVEDESC Desc1;
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Monster_LevelNum");
	Desc1.UIDesc.IDTag = 13;
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc1.UIDesc.fSize = { 1.f, 1.f };
	Desc1.iTextureNum = 7;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_LevelNum", &Desc1,
		(CGameObject**)&m_pUILevelNum)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level_2");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2,
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;

	//MonsterBar Bar
	CUI_Monster_HpBar::UIBARDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.36f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };
	Desc3.iRenderPass = 5;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;

	//Monster BackHpBar
	CUI_Monster_BackHpBar::UIBARDESC Desc3_1;
	_tcscpy_s(Desc3_1.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3_1.UIDesc.IDTag = 14;
	Desc3_1.UIDesc.bMinus = true;
	Desc3_1.UIDesc.fAngle = 0.36f;
	Desc3_1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3_1.UIDesc.fSize = { 1.f, 1.f };
	Desc3_1.iRenderPass = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_BackHpBar", &Desc3_1,
		(CGameObject**)&m_pUIBackHpBar)))
		return E_FAIL;


	//MonsterBar Name
	CUI_Monster_Name::UINAMEDESC Desc4;
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Monster_Name");
	Desc4.UIDesc.IDTag = 13;
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc4.UIDesc.fSize = { 1.f, 1.f };
	Desc4.iTextureNum = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Name", &Desc4,
		(CGameObject**)&m_pUIName)))
		return E_FAIL;

	//MonsterBar GroggyBar
	CUI_Monster_GroggyBar::UIBARDESC Desc5;
	_tcscpy_s(Desc5.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc5.UIDesc.IDTag = 14;
	Desc5.UIDesc.bMinus = true;
	Desc5.UIDesc.fAngle = 0.43f;
	Desc5.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc5.UIDesc.fSize = { 1.f, 1.f };
	Desc5.iRenderPass = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_GroggyBar", &Desc5,
		(CGameObject**)&m_pUIGroggyBar)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_MidBoss()
{
	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.42f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc,
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar LevelNum
	CUI_Monster_LevelNum::UIACTIVEDESC Desc1;
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Monster_LevelNum");
	Desc1.UIDesc.IDTag = 13;
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc1.UIDesc.fSize = { 1.f, 1.f };
	Desc1.iTextureNum = 9;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_LevelNum", &Desc1,
		(CGameObject**)&m_pUILevelNum)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level_3");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2,
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;

	//MonsterBar HpBar
	CUI_Monster_HpBar::UIBARDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.4f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };
	Desc3.iRenderPass = 3;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;

	//Monster BackHpBar
	CUI_Monster_BackHpBar::UIBARDESC Desc3_1;
	_tcscpy_s(Desc3_1.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3_1.UIDesc.IDTag = 14;
	Desc3_1.UIDesc.bMinus = true;
	Desc3_1.UIDesc.fAngle = 0.4f;
	Desc3_1.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3_1.UIDesc.fSize = { 1.f, 1.f };
	Desc3_1.iRenderPass = 6;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_BackHpBar", &Desc3_1,
		(CGameObject**)&m_pUIBackHpBar)))
		return E_FAIL;

	//MonsterBar Name
	CUI_Monster_Name::UINAMEDESC Desc4;
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Monster_Name");
	Desc4.UIDesc.IDTag = 13;
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc4.UIDesc.fSize = { 1.f, 1.f };
	Desc4.iTextureNum = 8;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_Name", &Desc4,
		(CGameObject**)&m_pUIName)))
		return E_FAIL;

	//MonsterBar GroggyBar
	CUI_Monster_GroggyBar::UIBARDESC Desc5;
	_tcscpy_s(Desc5.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc5.UIDesc.IDTag = 14;
	Desc5.UIDesc.bMinus = true;
	Desc5.UIDesc.fAngle = 0.45f;
	Desc5.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc5.UIDesc.fSize = { 1.f, 1.f };
	Desc5.iRenderPass = 4;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_UI", L"Proto_GameObject_UI_Monster_GroggyBar", &Desc5,
		(CGameObject**)&m_pUIGroggyBar)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Panel::Setting_EndBoss()
{
	return S_OK;
}

void CUI_Monster_Panel::Update_Panel(_double TimeDelta)
{
	switch (m_EnemyTag)
	{
	case Client::CUI_Monster_Panel::Enemy::CRAWLER:
		Update_Setting_Crawler(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::ABERRANT:
		Update_Setting_Aberrant(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::ANIMUS:
		Update_Setting_Animus(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::SWORD:
		Update_Setting_Sword(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::SPEAR:
		Update_Setting_Spear(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::SHOOTER:
		Update_Setting_Shooter(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::HEALER:
		Update_Setting_Healer(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::SWORD2H:
		Update_Setting_2HSword(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::MIDBOSS:
		Update_Setting_MidBoss(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::ENDBOSS:
		Update_Setting_EndBoss(TimeDelta);
		break;
	case Client::CUI_Monster_Panel::Enemy::ENEMY_END:
		MSGBOX("Failed In CUI_Monster_Panel::Update_Panel()");
		break;
	default:
		MSGBOX("Failed In CUI_Monster_Panel::Update_Panel()");
		break;
	}
}

void CUI_Monster_Panel::Update_Setting_Crawler(_double TimeDelta)
{
	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 1.5f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치

	//_vector temp = { 0.f, 0.f, 0.f, 0.f };
	//temp = XMVector3TransformCoord(TargetSettinMat.r[3], g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	//temp = XMVector3TransformCoord(temp, g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	//
	//CCamera_Silvermane* pCamera = (CCamera_Silvermane*)g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Camera")->front();
	//CCamera* pCameraCom = (CCamera*)pCamera->Get_Component(L"Com_Camera");
	//_float zFar = pCameraCom->GetDesc().fFar;
	//_float zNear = pCameraCom->GetDesc().fNear;

	//D3D11_VIEWPORT		ViewportDesc;
	//_uint		iNumViewports = 1;
	//m_pDeviceContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	//_matrix matViewPort =
	//{
	//	ViewportDesc.Width * 0.5f, 0.f, 0.f, 0.f,
	//	0.f, -(ViewportDesc.Height * 0.5f), 0.f, 0.f,
	//	0.f, 0.f, zFar - zNear, 0.f,
	//	ViewportDesc.Width * 0.5f , ViewportDesc.Height * 0.5f, zNear , 1.f
	//};

	//temp = XMVector3TransformCoord(temp, matViewPort);

	//_vector newPos = { XMVectorGetX(temp), XMVectorGetY(temp), 0.1f };
	//m_pTransform->Set_State(CTransform::STATE_POSITION, newPos);
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_UP, ViewMatrix.r[1]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.12f,fDistToUI * 0.12f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 3.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.4f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());

	//UI Level Num
	CTransform* LevelNumTransform = (CTransform*)m_pUILevelNum->Get_Component(L"Com_Transform");
	_matrix LevelNummat = XMMatrixIdentity();
	LevelNummat.r[0] = XMVectorSetX(LevelNummat.r[0], 0.55f);
	LevelNummat.r[1] = XMVectorSetY(LevelNummat.r[1], 0.55f);
	LevelNummat.r[3] = { -1.55f, 0.13f, -0.02f, 1.f };
	LevelNumTransform->Set_WorldMatrix(LevelNummat * m_pTransform->Get_WorldMatrix());

	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -1.55f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 2.75f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	//UI HpBarBack
	CTransform* HpBarBackTransform = (CTransform*)m_pUIBackHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarBackmat = XMMatrixIdentity();
	HpBarBackmat.r[0] = XMVectorSetX(HpBarBackmat.r[0], 2.75f);
	HpBarBackmat.r[1] = XMVectorSetY(HpBarBackmat.r[1], 0.40f);
	HpBarBackmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarBackTransform->Set_WorldMatrix(HpBarBackmat * m_pTransform->Get_WorldMatrix());

	//UI Name
	CTransform* NameTransform = (CTransform*)m_pUIName->Get_Component(L"Com_Transform");
	_matrix Namemat = XMMatrixIdentity();
	Namemat.r[0] = XMVectorSetX(Namemat.r[0], 1.28f);
	Namemat.r[1] = XMVectorSetY(Namemat.r[1], 0.64f);
	Namemat.r[3] = { -0.7f, 0.35f, -0.005f, 1.f };
	NameTransform->Set_WorldMatrix(Namemat * m_pTransform->Get_WorldMatrix());

	//UI GroggyBar
	CTransform* GroggyBarTransform = (CTransform*)m_pUIGroggyBar->Get_Component(L"Com_Transform");
	_matrix GroggyBarmat = XMMatrixIdentity();
	GroggyBarmat.r[0] = XMVectorSetX(GroggyBarmat.r[0], 2.75f);
	GroggyBarmat.r[1] = XMVectorSetY(GroggyBarmat.r[1], 0.2f);
	GroggyBarmat.r[3] = { -0.02f, -0.1f, -0.005f, 1.f };
	GroggyBarTransform->Set_WorldMatrix(GroggyBarmat * m_pTransform->Get_WorldMatrix());
}

void CUI_Monster_Panel::Update_Setting_Aberrant(_double TimeDelta)
{
	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 2.f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//Panel Size
	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.12f,fDistToUI * 0.12f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 3.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.40f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());

	//UI Level Num
	CTransform* LevelNumTransform = (CTransform*)m_pUILevelNum->Get_Component(L"Com_Transform");
	_matrix LevelNummat = XMMatrixIdentity();
	LevelNummat.r[0] = XMVectorSetX(LevelNummat.r[0], 0.55f);
	LevelNummat.r[1] = XMVectorSetY(LevelNummat.r[1], 0.55f);
	LevelNummat.r[3] = { -1.55f, 0.13f, -0.02f, 1.f };
	LevelNumTransform->Set_WorldMatrix(LevelNummat * m_pTransform->Get_WorldMatrix());

	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -1.55f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 2.75f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	//UI HpBarBack
	CTransform* HpBarBackTransform = (CTransform*)m_pUIBackHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarBackmat = XMMatrixIdentity();
	HpBarBackmat.r[0] = XMVectorSetX(HpBarBackmat.r[0], 2.75f);
	HpBarBackmat.r[1] = XMVectorSetY(HpBarBackmat.r[1], 0.40f);
	HpBarBackmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarBackTransform->Set_WorldMatrix(HpBarBackmat * m_pTransform->Get_WorldMatrix());

	//UI Name
	CTransform* NameTransform = (CTransform*)m_pUIName->Get_Component(L"Com_Transform");
	_matrix Namemat = XMMatrixIdentity();
	Namemat.r[0] = XMVectorSetX(Namemat.r[0], 1.28f);
	Namemat.r[1] = XMVectorSetY(Namemat.r[1], 0.64f);
	Namemat.r[3] = { -0.5f, 0.35f, -0.005f, 1.f };
	NameTransform->Set_WorldMatrix(Namemat * m_pTransform->Get_WorldMatrix());

	//UI GroggyBar
	CTransform* GroggyBarTransform = (CTransform*)m_pUIGroggyBar->Get_Component(L"Com_Transform");
	_matrix GroggyBarmat = XMMatrixIdentity();
	GroggyBarmat.r[0] = XMVectorSetX(GroggyBarmat.r[0], 2.75f);
	GroggyBarmat.r[1] = XMVectorSetY(GroggyBarmat.r[1], 0.2f);
	GroggyBarmat.r[3] = { -0.02f, -0.1f, -0.005f, 1.f };
	GroggyBarTransform->Set_WorldMatrix(GroggyBarmat * m_pTransform->Get_WorldMatrix());
}

void CUI_Monster_Panel::Update_Setting_Animus(_double TimeDelta)
{
	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 3.3f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//Panel Size
	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.12f,fDistToUI * 0.12f,1.f,1.f };
	m_pTransform->Scaling(vScale);;

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 3.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.40f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());

	//UI Level Num
	CTransform* LevelNumTransform = (CTransform*)m_pUILevelNum->Get_Component(L"Com_Transform");
	_matrix LevelNummat = XMMatrixIdentity();
	LevelNummat.r[0] = XMVectorSetX(LevelNummat.r[0], 0.55f);
	LevelNummat.r[1] = XMVectorSetY(LevelNummat.r[1], 0.55f);
	LevelNummat.r[3] = { -1.55f, 0.13f, -0.02f, 1.f };
	LevelNumTransform->Set_WorldMatrix(LevelNummat * m_pTransform->Get_WorldMatrix());

	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -1.55f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 2.75f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	//UI HpBarBack
	CTransform* HpBarBackTransform = (CTransform*)m_pUIBackHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarBackmat = XMMatrixIdentity();
	HpBarBackmat.r[0] = XMVectorSetX(HpBarBackmat.r[0], 2.75f);
	HpBarBackmat.r[1] = XMVectorSetY(HpBarBackmat.r[1], 0.40f);
	HpBarBackmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarBackTransform->Set_WorldMatrix(HpBarBackmat * m_pTransform->Get_WorldMatrix());

	//UI Name
	CTransform* NameTransform = (CTransform*)m_pUIName->Get_Component(L"Com_Transform");
	_matrix Namemat = XMMatrixIdentity();
	Namemat.r[0] = XMVectorSetX(Namemat.r[0], 1.76f);
	Namemat.r[1] = XMVectorSetY(Namemat.r[1], 0.64f);
	Namemat.r[3] = { -0.4f, 0.35f, -0.005f, 1.f };
	NameTransform->Set_WorldMatrix(Namemat * m_pTransform->Get_WorldMatrix());

	//UI GroggyBar
	CTransform* GroggyBarTransform = (CTransform*)m_pUIGroggyBar->Get_Component(L"Com_Transform");
	_matrix GroggyBarmat = XMMatrixIdentity();
	GroggyBarmat.r[0] = XMVectorSetX(GroggyBarmat.r[0], 2.75f);
	GroggyBarmat.r[1] = XMVectorSetY(GroggyBarmat.r[1], 0.2f);
	GroggyBarmat.r[3] = { -0.02f, -0.1f, -0.005f, 1.f };
	GroggyBarTransform->Set_WorldMatrix(GroggyBarmat * m_pTransform->Get_WorldMatrix());
}

void CUI_Monster_Panel::Update_Setting_Sword(_double TimeDelta)
{
	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 2.5f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//Panel Size
	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.12f,fDistToUI * 0.12f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 3.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.40f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());

	//UI Level Num
	CTransform* LevelNumTransform = (CTransform*)m_pUILevelNum->Get_Component(L"Com_Transform");
	_matrix LevelNummat = XMMatrixIdentity();
	LevelNummat.r[0] = XMVectorSetX(LevelNummat.r[0], 0.55f);
	LevelNummat.r[1] = XMVectorSetY(LevelNummat.r[1], 0.55f);
	LevelNummat.r[3] = { -1.55f, 0.13f, -0.02f, 1.f };
	LevelNumTransform->Set_WorldMatrix(LevelNummat * m_pTransform->Get_WorldMatrix());

	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -1.55f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 2.75f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	//UI HpBarBack
	CTransform* HpBarBackTransform = (CTransform*)m_pUIBackHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarBackmat = XMMatrixIdentity();
	HpBarBackmat.r[0] = XMVectorSetX(HpBarBackmat.r[0], 2.75f);
	HpBarBackmat.r[1] = XMVectorSetY(HpBarBackmat.r[1], 0.40f);
	HpBarBackmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarBackTransform->Set_WorldMatrix(HpBarBackmat * m_pTransform->Get_WorldMatrix());

	//UI Name
	CTransform* NameTransform = (CTransform*)m_pUIName->Get_Component(L"Com_Transform");
	_matrix Namemat = XMMatrixIdentity();
	Namemat.r[0] = XMVectorSetX(Namemat.r[0], 1.76f);
	Namemat.r[1] = XMVectorSetY(Namemat.r[1], 0.64f);
	Namemat.r[3] = { -0.4f, 0.35f, -0.005f, 1.f };
	NameTransform->Set_WorldMatrix(Namemat * m_pTransform->Get_WorldMatrix());

	//UI GroggyBar
	CTransform* GroggyBarTransform = (CTransform*)m_pUIGroggyBar->Get_Component(L"Com_Transform");
	_matrix GroggyBarmat = XMMatrixIdentity();
	GroggyBarmat.r[0] = XMVectorSetX(GroggyBarmat.r[0], 2.75f);
	GroggyBarmat.r[1] = XMVectorSetY(GroggyBarmat.r[1], 0.2f);
	GroggyBarmat.r[3] = { -0.02f, -0.1f, -0.005f, 1.f };
	GroggyBarTransform->Set_WorldMatrix(GroggyBarmat * m_pTransform->Get_WorldMatrix());
}

void CUI_Monster_Panel::Update_Setting_Spear(_double TimeDelta)
{
	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 2.8f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//Panel Size
	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.12f,fDistToUI * 0.12f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 3.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.40f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());

	//UI Level Num
	CTransform* LevelNumTransform = (CTransform*)m_pUILevelNum->Get_Component(L"Com_Transform");
	_matrix LevelNummat = XMMatrixIdentity();
	LevelNummat.r[0] = XMVectorSetX(LevelNummat.r[0], 0.55f);
	LevelNummat.r[1] = XMVectorSetY(LevelNummat.r[1], 0.55f);
	LevelNummat.r[3] = { -1.55f, 0.13f, -0.02f, 1.f };
	LevelNumTransform->Set_WorldMatrix(LevelNummat * m_pTransform->Get_WorldMatrix());

	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -1.55f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 2.75f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	//UI HpBarBack
	CTransform* HpBarBackTransform = (CTransform*)m_pUIBackHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarBackmat = XMMatrixIdentity();
	HpBarBackmat.r[0] = XMVectorSetX(HpBarBackmat.r[0], 2.75f);
	HpBarBackmat.r[1] = XMVectorSetY(HpBarBackmat.r[1], 0.40f);
	HpBarBackmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarBackTransform->Set_WorldMatrix(HpBarBackmat * m_pTransform->Get_WorldMatrix());

	//UI Name
	CTransform* NameTransform = (CTransform*)m_pUIName->Get_Component(L"Com_Transform");
	_matrix Namemat = XMMatrixIdentity();
	Namemat.r[0] = XMVectorSetX(Namemat.r[0], 1.76f);
	Namemat.r[1] = XMVectorSetY(Namemat.r[1], 0.64f);
	Namemat.r[3] = { -0.4f, 0.35f, -0.005f, 1.f };
	NameTransform->Set_WorldMatrix(Namemat * m_pTransform->Get_WorldMatrix());

	//UI GroggyBar
	CTransform* GroggyBarTransform = (CTransform*)m_pUIGroggyBar->Get_Component(L"Com_Transform");
	_matrix GroggyBarmat = XMMatrixIdentity();
	GroggyBarmat.r[0] = XMVectorSetX(GroggyBarmat.r[0], 2.75f);
	GroggyBarmat.r[1] = XMVectorSetY(GroggyBarmat.r[1], 0.2f);
	GroggyBarmat.r[3] = { -0.02f, -0.1f, -0.005f, 1.f };
	GroggyBarTransform->Set_WorldMatrix(GroggyBarmat * m_pTransform->Get_WorldMatrix());
}

void CUI_Monster_Panel::Update_Setting_Shooter(_double TimeDelta)
{
	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 2.5f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//Panel Size
	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.12f,fDistToUI * 0.12f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 3.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.40f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());

	//UI Level Num
	CTransform* LevelNumTransform = (CTransform*)m_pUILevelNum->Get_Component(L"Com_Transform");
	_matrix LevelNummat = XMMatrixIdentity();
	LevelNummat.r[0] = XMVectorSetX(LevelNummat.r[0], 0.55f);
	LevelNummat.r[1] = XMVectorSetY(LevelNummat.r[1], 0.55f);
	LevelNummat.r[3] = { -1.55f, 0.13f, -0.02f, 1.f };
	LevelNumTransform->Set_WorldMatrix(LevelNummat * m_pTransform->Get_WorldMatrix());

	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -1.55f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 2.75f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	//UI HpBarBack
	CTransform* HpBarBackTransform = (CTransform*)m_pUIBackHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarBackmat = XMMatrixIdentity();
	HpBarBackmat.r[0] = XMVectorSetX(HpBarBackmat.r[0], 2.75f);
	HpBarBackmat.r[1] = XMVectorSetY(HpBarBackmat.r[1], 0.40f);
	HpBarBackmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarBackTransform->Set_WorldMatrix(HpBarBackmat * m_pTransform->Get_WorldMatrix());

	//UI Name
	CTransform* NameTransform = (CTransform*)m_pUIName->Get_Component(L"Com_Transform");
	_matrix Namemat = XMMatrixIdentity();
	Namemat.r[0] = XMVectorSetX(Namemat.r[0], 1.76f);
	Namemat.r[1] = XMVectorSetY(Namemat.r[1], 0.64f);
	Namemat.r[3] = { -0.4f, 0.35f, -0.005f, 1.f };
	NameTransform->Set_WorldMatrix(Namemat * m_pTransform->Get_WorldMatrix());

	//UI GroggyBar
	CTransform* GroggyBarTransform = (CTransform*)m_pUIGroggyBar->Get_Component(L"Com_Transform");
	_matrix GroggyBarmat = XMMatrixIdentity();
	GroggyBarmat.r[0] = XMVectorSetX(GroggyBarmat.r[0], 2.75f);
	GroggyBarmat.r[1] = XMVectorSetY(GroggyBarmat.r[1], 0.2f);
	GroggyBarmat.r[3] = { -0.02f, -0.1f, -0.005f, 1.f };
	GroggyBarTransform->Set_WorldMatrix(GroggyBarmat * m_pTransform->Get_WorldMatrix());
}

void CUI_Monster_Panel::Update_Setting_Healer(_double TimeDelta)
{
	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 2.8f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//Panel Size
	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.12f,fDistToUI * 0.12f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 3.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.40f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());

	//UI Level Num
	CTransform* LevelNumTransform = (CTransform*)m_pUILevelNum->Get_Component(L"Com_Transform");
	_matrix LevelNummat = XMMatrixIdentity();
	LevelNummat.r[0] = XMVectorSetX(LevelNummat.r[0], 0.55f);
	LevelNummat.r[1] = XMVectorSetY(LevelNummat.r[1], 0.55f);
	LevelNummat.r[3] = { -1.55f, 0.13f, -0.02f, 1.f };
	LevelNumTransform->Set_WorldMatrix(LevelNummat * m_pTransform->Get_WorldMatrix());

	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -1.55f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 2.75f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	//UI HpBarBack
	CTransform* HpBarBackTransform = (CTransform*)m_pUIBackHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarBackmat = XMMatrixIdentity();
	HpBarBackmat.r[0] = XMVectorSetX(HpBarBackmat.r[0], 2.75f);
	HpBarBackmat.r[1] = XMVectorSetY(HpBarBackmat.r[1], 0.40f);
	HpBarBackmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarBackTransform->Set_WorldMatrix(HpBarBackmat * m_pTransform->Get_WorldMatrix());

	//UI Name
	CTransform* NameTransform = (CTransform*)m_pUIName->Get_Component(L"Com_Transform");
	_matrix Namemat = XMMatrixIdentity();
	Namemat.r[0] = XMVectorSetX(Namemat.r[0], 1.76f);
	Namemat.r[1] = XMVectorSetY(Namemat.r[1], 0.64f);
	Namemat.r[3] = { -0.4f, 0.35f, -0.005f, 1.f };
	NameTransform->Set_WorldMatrix(Namemat * m_pTransform->Get_WorldMatrix());

	//UI GroggyBar
	CTransform* GroggyBarTransform = (CTransform*)m_pUIGroggyBar->Get_Component(L"Com_Transform");
	_matrix GroggyBarmat = XMMatrixIdentity();
	GroggyBarmat.r[0] = XMVectorSetX(GroggyBarmat.r[0], 2.75f);
	GroggyBarmat.r[1] = XMVectorSetY(GroggyBarmat.r[1], 0.2f);
	GroggyBarmat.r[3] = { -0.02f, -0.1f, -0.005f, 1.f };
	GroggyBarTransform->Set_WorldMatrix(GroggyBarmat * m_pTransform->Get_WorldMatrix());
}

void CUI_Monster_Panel::Update_Setting_2HSword(_double TimeDelta)
{
	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 2.8f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//Panel Size
	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.12f,fDistToUI * 0.12f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 3.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.40f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());

	//UI Level Num
	CTransform* LevelNumTransform = (CTransform*)m_pUILevelNum->Get_Component(L"Com_Transform");
	_matrix LevelNummat = XMMatrixIdentity();
	LevelNummat.r[0] = XMVectorSetX(LevelNummat.r[0], 0.55f);
	LevelNummat.r[1] = XMVectorSetY(LevelNummat.r[1], 0.55f);
	LevelNummat.r[3] = { -1.55f, 0.13f, -0.02f, 1.f };
	LevelNumTransform->Set_WorldMatrix(LevelNummat * m_pTransform->Get_WorldMatrix());


	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -1.55f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 2.75f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	//UI HpBarBack
	CTransform* HpBarBackTransform = (CTransform*)m_pUIBackHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarBackmat = XMMatrixIdentity();
	HpBarBackmat.r[0] = XMVectorSetX(HpBarBackmat.r[0], 2.75f);
	HpBarBackmat.r[1] = XMVectorSetY(HpBarBackmat.r[1], 0.40f);
	HpBarBackmat.r[3] = { -0.03f, 0.0f, -0.005f, 1.f };
	HpBarBackTransform->Set_WorldMatrix(HpBarBackmat * m_pTransform->Get_WorldMatrix());

	//UI Name
	CTransform* NameTransform = (CTransform*)m_pUIName->Get_Component(L"Com_Transform");
	_matrix Namemat = XMMatrixIdentity();
	Namemat.r[0] = XMVectorSetX(Namemat.r[0], 1.76f);
	Namemat.r[1] = XMVectorSetY(Namemat.r[1], 0.64f);
	Namemat.r[3] = { -0.4f, 0.35f, -0.005f, 1.f };
	NameTransform->Set_WorldMatrix(Namemat * m_pTransform->Get_WorldMatrix());

	//UI GroggyBar
	CTransform* GroggyBarTransform = (CTransform*)m_pUIGroggyBar->Get_Component(L"Com_Transform");
	_matrix GroggyBarmat = XMMatrixIdentity();
	GroggyBarmat.r[0] = XMVectorSetX(GroggyBarmat.r[0], 2.75f);
	GroggyBarmat.r[1] = XMVectorSetY(GroggyBarmat.r[1], 0.2f);
	GroggyBarmat.r[3] = { -0.02f, -0.1f, -0.005f, 1.f };
	GroggyBarTransform->Set_WorldMatrix(GroggyBarmat * m_pTransform->Get_WorldMatrix());
}

void CUI_Monster_Panel::Update_Setting_MidBoss(_double TimeDelta)
{
	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 3.35f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//Panel Size
	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.09f,fDistToUI * 0.09f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 4.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.40f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());

	//UI Level Num
	CTransform* LevelNumTransform = (CTransform*)m_pUILevelNum->Get_Component(L"Com_Transform");
	_matrix LevelNummat = XMMatrixIdentity();
	LevelNummat.r[0] = XMVectorSetX(LevelNummat.r[0], 0.55f);
	LevelNummat.r[1] = XMVectorSetY(LevelNummat.r[1], 0.55f);
	LevelNummat.r[3] = { -2.1f, 0.13f, -0.02f, 1.f };;
	LevelNumTransform->Set_WorldMatrix(LevelNummat * m_pTransform->Get_WorldMatrix());

	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -2.1f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 3.7f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { -0.05f, 0.0f, -0.005f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	//UI HpBarBack
	CTransform* HpBarBackTransform = (CTransform*)m_pUIBackHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarBackmat = XMMatrixIdentity();
	HpBarBackmat.r[0] = XMVectorSetX(HpBarBackmat.r[0], 3.7f);
	HpBarBackmat.r[1] = XMVectorSetY(HpBarBackmat.r[1], 0.40f);
	HpBarBackmat.r[3] = { -0.05f, 0.0f, -0.005f, 1.f };
	HpBarBackTransform->Set_WorldMatrix(HpBarBackmat * m_pTransform->Get_WorldMatrix());

	//UI Name
	CTransform* NameTransform = (CTransform*)m_pUIName->Get_Component(L"Com_Transform");
	_matrix Namemat = XMMatrixIdentity();
	Namemat.r[0] = XMVectorSetX(Namemat.r[0], 2.56f);
	Namemat.r[1] = XMVectorSetY(Namemat.r[1], 0.64f);
	Namemat.r[3] = { -0.7f, 0.35f, -0.005f, 1.f };
	NameTransform->Set_WorldMatrix(Namemat * m_pTransform->Get_WorldMatrix());

	//UI GroggyBar
	CTransform* GroggyBarTransform = (CTransform*)m_pUIGroggyBar->Get_Component(L"Com_Transform");
	_matrix GroggyBarmat = XMMatrixIdentity();
	GroggyBarmat.r[0] = XMVectorSetX(GroggyBarmat.r[0], 3.7f);
	GroggyBarmat.r[1] = XMVectorSetY(GroggyBarmat.r[1], 0.2f);
	GroggyBarmat.r[3] = { -0.05f, -0.1f, -0.005f, 1.f };
	GroggyBarTransform->Set_WorldMatrix(GroggyBarmat * m_pTransform->Get_WorldMatrix());
}

void CUI_Monster_Panel::Update_Setting_EndBoss(_double TimeDelta)
{
}

HRESULT CUI_Monster_Panel::SetUp_Components()
{

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_UI", L"Com_Rect_UI", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	//m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_Rect_UI");

	//if (!m_pBuffer)
	//	return E_FAIL;

	//if (FAILED(CGameObject::SetUp_Components(L"Com_Rect_UI", m_pBuffer)))
	//	return E_FAIL;

	return S_OK;
}

void CUI_Monster_Panel::Set_UIRemove(_bool bCheck)
{
	CGameObject::Set_Remove(true);

	m_pUIBack->Set_Remove(m_bRemove);
	m_pUILevel->Set_Remove(m_bRemove);
	m_pUILevelNum->Set_Remove(m_bRemove);
	m_pUIHpBar->Set_Remove(m_bRemove);
	m_pUIBackHpBar->Set_Remove(m_bRemove);
	m_pUIName->Set_Remove(m_bRemove);
	m_pUIGroggyBar->Set_Remove(m_bRemove);
}

CUI_Monster_Panel* CUI_Monster_Panel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Monster_Panel* pInstance = new CUI_Monster_Panel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Monster_Panel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_Panel::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Monster_Panel* pInstance = new CUI_Monster_Panel(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Monster_Panel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_Panel::Free()
{
	__super::Free();
	Safe_Release(m_pUIBack);//값
	Safe_Release(m_pUILevel);//값
	Safe_Release(m_pUILevelNum);//값
	Safe_Release(m_pUIHpBar);
	Safe_Release(m_pUIBackHpBar);
	Safe_Release(m_pUIName);
	Safe_Release(m_pUIGroggyBar);
}
