#include "pch.h"
#include "MainApp.h"
#include "BackGround.h"
#include "MainCamera.h"
#include "MainCamera_Ortho.h"
#include "Loading.h"
#include "MeshLoader.h"

#include "DebugSystem.h"
#include "ShakeManager.h"
#include "DataManager.h"
#include "InteractManager.h"
#include "WeaponGenerator.h"
#include "DropManager.h"
#include "GuideUIManager.h"
#include "ScenematicManager.h"

//Inventory UI Object
#include "Inven_UIManager.h"
#include "UI_ModalWindow.h"
#include "UI_Indexes.h"
#include "UI_Background.h"
#include "UI_Equipment.h"
#include "UI_Armory.h"
#include "UI_ItemSlot.h"
#include "UI_SlotBackground.h"
#include "UI_SlotGrade.h"
#include "UI_SlotItemEffect.h"
#include "UI_EquipedText.h"
#include "UI_SlotItemIcon.h"
#include "UI_ItemStatusWindow.h"
#include "UI_ItemStatusBackground.h"
#include "UI_ItemStatusEffect.h"
#include "Button_Equipment.h"
#include "Button_Armory.h"
#include "Button_Skill.h"
#include "UI_SelectEffect.h"
#include "IndexIndicator.h"
#include "UI_PlayerStatusWindow.h"
#include "UI_PlayerStatus_BG.h"
#include "UI_PlayerStatus_Might.h"
#include "UI_PlayerStatus_Spirit.h"
#include "UI_PlayerStatus_Viality.h"
#include "UI_PlayerStatus_DPS_1.h"
#include "UI_PlayerStatus_DPS_2.h"
#include "UI_PlayerStatus_MaxHealth.h"

//HUD
#include "Hud.h"
#include "Loot_Equipment.h"
#include "UI_LootBackground.h"
#include "UI_LootGrade.h"
#include "UI_LootFrame.h"
#include "UI_LootItemIcon.h"
#include "UI_LootItemEffect.h"
#include "UI_LootItemNameText.h"
#include "UI_LootItemNameType.h"
#include "UI_Level_UP.h"
#include "UI_LevelUP_Background.h"
#include "UI_LevelUP_BG_Left.h"
#include "UI_LevelUP_BG_Right.h"
#include "UI_LevelUP_Fill_Lead_Left.h"
#include "UI_LevelUP_Fill_Lead_Right.h"
#include "UI_LevelUP_Fill_Left.h"
#include "UI_LevelUP_Fill_Right.h"
#include "UI_Death.h"

//Quest
#include "Quest.h"
#include "UI_QuestText.h"
#include "UI_QuestHeadText.h"
#include "UI_QuestClear.h"
//Guide
#include "UI_Guide_Background.h"
#include "UI_Guide_Texture.h"

//Inventory UI Component
#include "SingleImage.h"
#include "UIHelper.h"

CClient_Observer*	g_pObserver = nullptr;
CDebugSystem*		g_pDebugSystem = nullptr;
CShakeManager*		g_pShakeManager = nullptr;
CDataManager*		g_pDataManager = nullptr;
CInven_UIManager*	g_pInvenUIManager = nullptr;
CInteractManager*	g_pInteractManager = nullptr;
CWeaponGenerator*	g_pWeaponGenerator = nullptr;
CDropManager*		g_pDropManager = nullptr;
CQuestManager*		g_pQuestManager = nullptr;
CGuideUIManager*	g_pGuideManager = nullptr;

CMainApp::CMainApp()
{
}

HRESULT CMainApp::NativeConstruct()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pInstance->Initialize_Engine(g_hInst, g_hWnd, (_uint)SCENEID::SCENE_END, CGraphic_Device::MODE_WIN, g_iWinCx, g_iWinCy, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);

	g_pGameInstance->Set_NumLayers((_uint)ELayer::Max);
	// 플레이어
	g_pGameInstance->Set_CollisionLayer((_uint)ELayer::Player, (_uint)ELayer::Enviroment);
	g_pGameInstance->Set_CollisionLayer((_uint)ELayer::Player, (_uint)ELayer::MonsterWeapon);
	g_pGameInstance->Set_CollisionLayer((_uint)ELayer::Player, (_uint)ELayer::JumpTrigger);
	g_pGameInstance->Set_CollisionLayer((_uint)ELayer::Player, (_uint)ELayer::Monster);
	g_pGameInstance->Set_CollisionLayer((_uint)ELayer::Player, (_uint)ELayer::Trigger);
	// 몬스터
	g_pGameInstance->Set_CollisionLayer((_uint)ELayer::Monster, (_uint)ELayer::Enviroment);
	g_pGameInstance->Set_CollisionLayer((_uint)ELayer::Monster, (_uint)ELayer::Weapon);

	g_pGameInstance->Set_CollisionLayer((_uint)ELayer::Weapon, (_uint)ELayer::Pot);

	if (FAILED(Ready_Fonts()))
		return E_FAIL;

	if (FAILED(Load_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Init_Camera()))
		return E_FAIL;

#ifdef _DEBUG
	g_pDebugSystem = CDebugSystem::GetInstance();
	if (FAILED(g_pDebugSystem->Init_DebugSystem(m_pDevice, m_pDeviceContext,m_pRenderer)))
		return E_FAIL;
#endif

	if (FAILED(SetUp_StartLevel(SCENEID::SCENE_LOGO)))
		return E_FAIL;

	if (FAILED(Ready_GameManager()))
		return E_FAIL;


	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	m_TimeAcc += TimeDelta;

	g_pGameInstance->Update_InputDev();

	if (m_isDeltaTimeZero)
		TimeDelta = 0.f;

	if (g_pInvenUIManager->IsOpenModal())
		TimeDelta = 0.f;

	if (m_isFreeze)
	{
		m_dFreezeTimeAcc += TimeDelta;
		if (0.25 < m_dFreezeTimeAcc)
		{
			m_isFreeze = false;
			m_dFreezeTimeAcc = 0.0;
		}
		else
			TimeDelta *= 0.1;
	}

	RenderingBnt();

	if (g_pGameInstance->getkeyDown(DIK_P))
		m_isPause = !m_isPause;

	if (!m_isPause)
	{
		g_pGameInstance->Tick_Engine(TimeDelta);
		m_isRender = true;
	}
	else
	{
		if (g_pGameInstance->getkeyDown(DIK_PGUP))
		{
			g_pGameInstance->Tick_Engine(0.04);
			m_isRender = true;
		}
	}

	if(g_pDataManager)
		g_pDataManager->Tick();

	return _int();
}

HRESULT CMainApp::Render()
{
	if (!m_isRender)
		return S_OK;

	if (FAILED(g_pGameInstance->Clear_BackBuffer_View(XMFLOAT4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;	
	
	if (FAILED(m_pRenderer->Draw_RenderGroup()))
		return E_FAIL;

	if (FAILED(g_pGameInstance->SetPreViewProtj(g_pGameInstance->Get_BaseCameraTag())))
		MSGBOX("Failed to Recording PreViewProj");

	if (FAILED(g_pGameInstance->Render_Engine()))
		return E_FAIL;

	++m_iNumRender;

	if (m_TimeAcc >= 1.0)
	{
		wsprintf(m_szFPS, TEXT("Frame : %d"), m_iNumRender);
		m_iNumRender = 0;
		m_TimeAcc = 0.0;	
	}

	SetWindowText(g_hWnd, m_szFPS);

	if (FAILED(g_pGameInstance->Present()))
		return E_FAIL;

	m_isRender = false;

	return S_OK;
}

HRESULT CMainApp::Destroy()
{
	if(FAILED(g_pGameInstance->Delete_GameObject()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_StartLevel(SCENEID eLevel)
{
	HRESULT		hr = 0;

	switch (eLevel)
	{
	case SCENEID::SCENE_LOGO:
		hr = g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext, eLevel));
		break;
	default:
		hr = E_FAIL;
		break;
	}	

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_GameManager(void)
{
	/*CMeshLoader* pMeshLoader = CMeshLoader::GetInstance();
if (FAILED(pMeshLoader->Reserve_MeshLoader(m_pDevice, m_pDeviceContext)))
	return E_FAIL;*/

	g_pObserver = CClient_Observer::GetInstance();
	g_pShakeManager = CShakeManager::GetInstance();

	if (FAILED(g_pShakeManager->NativeConstruct()))
		return E_FAIL;

	g_pDataManager = CDataManager::GetInstance();
	if (FAILED(g_pDataManager->NativeConstruct()))
		return E_FAIL;

	g_pInvenUIManager = CInven_UIManager::GetInstance();
	if (FAILED(g_pInvenUIManager->NativeConstruct()))
		return E_FAIL;

	g_pInteractManager = CInteractManager::GetInstance();
	if (FAILED(g_pInteractManager->NativeConstruct()))
		return E_FAIL;

	g_pQuestManager = CQuestManager::GetInstance();
	if (FAILED(g_pQuestManager->NativeConstruct()))
		return E_FAIL;

	g_pGuideManager = CGuideUIManager::GetInstance();
	if (FAILED(g_pGuideManager->NativeConstruct()))
		return E_FAIL;

	CScenematicManager::GetInstance();
	return S_OK;
}

HRESULT CMainApp::Ready_Component_Prototype()
{
	//Buffer
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Bar", CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_UI_Bar.hlsl"))))
		return E_FAIL;

	if(FAILED(g_pGameInstance->SetUpBaseComponent(m_pDevice, m_pDeviceContext)))
		return E_FAIL;
	m_pRenderer = g_pGameInstance->Clone_Component<CRenderer>((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Renderer");

	if (!m_pRenderer)
		return E_FAIL;
	//////////////////////////////////////////
	/* for. inventory ui component*/
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", CSingleImage::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_UIHelper", CUIHelper::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//////////////////////////////////////////
	return S_OK;
}

HRESULT CMainApp::Ready_GameObject_Prototype()
{
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_MainCamera"), CMainCamera::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_MainOrthoCamera"), CMainCamera_Ortho::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//for. Inventory ////////////////
	//Modal Window
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ModalWindow"), CUI_ModalWindow::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Indexes
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Indexes"), CUI_Indexes::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//BG
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Background"), CUI_Background::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Equip
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Equipment"), CUI_Equipment::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Armory
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Armory"), CUI_Armory::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Item Slot 
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ItemSlot"), CUI_ItemSlot::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Item Slot BG
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ItemSlot_BG"), CUI_SlotBackground::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Item Icon
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ItemIcon"), CUI_SlotItemIcon::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Item Grade
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ItemGrade"), CUI_SlotGrade::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Item Effect
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ItemEffect"), CUI_SlotItemEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Item Equip Text
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ItemEquipedText"), CUI_EquipedText::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//ItemStatusWindow
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ItemStatusWindow"), CUI_ItemStatusWindow::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//ItemStatus BG
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ItemStatus_BG"), UI_ItemStatusBackground::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//ItemStatus Effect
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_ItemStatus_Effect"), UI_ItemStatusEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Button Equip
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Button_Equipment"), CButton_Equipment::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Button Armory
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Button_Armory"), CButton_Armory::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Button Skill
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Button_Skill"), CButton_Skill::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Select Effect
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_SelectEffect"), CUI_SelectEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Index Indicator
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_IndexIndicator"), CIndexIndicator::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Player Status Window
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_PlayerStatus_Window"), CUI_PlayerStatusWindow::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Player Status BG
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_PlayerStatus_BG"), CUI_PlayerStatus_BG::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Player Status Might 
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_PlayerStatus_Might"), CUI_PlayerStatus_Might::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Player Status Spirit
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_PlayerStatus_Spirit"), CUI_PlayerStatus_Spirit::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Player Status Viality
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_PlayerStatus_Viality"), CUI_PlayerStatus_Viality::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Player Status First DPS
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_PlayerStatus_FirstDPS"), CUI_PlayerStatus_DPS_1::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Player Status Second DPS
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_PlayerStatus_SecondDPS"), CUI_PlayerStatus_DPS_2::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Player Status Max Hp
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_PlayerStatus_MaxHp"), CUI_PlayerStatus_MAXHP::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	////////////////////////////////
	/* for. Hud */
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Hud"), CHud::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Loot Equipment
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LootEquipment"), CLoot_Equipment::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Loot Equipment Background
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LootBackground"), CUI_LootBackground::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Loot Equipment Grade
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LootGrade"), CUI_LootGrade::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Loot Equipment Frame
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LootFrame"), CUI_LootFrame::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Loot Equipment Item Icon
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LootItemIcon"), CUI_LootItemIcon::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Loot Equipment Item Effect
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LootItemEffect"), CUI_LootItemEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Loot Equipment Item Name Text
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LootItemNameText"), CUI_LootItemNameText::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Loot Equipment Item Name Type
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LootItemNameType"), CUI_LootItemNameType::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	///////////////////////////////
	//Quest
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Quest"), CQuest::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Quest Text
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_QuestText"), CUI_QuestText::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Quest Head Text
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_QuestHeadText"), CUI_QuestHeadText::Create(m_pDevice, m_pDeviceContext))))
	return E_FAIL;
	//Quest Clear Marker
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_QuestClear"), CUI_QuestClear::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Death
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Death"), CUI_Death::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	///////////////////////////////
	//Level Up
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LevelUp"), CLevel_UP::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Level Up Background
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LevelUp_Background"), CUI_LevelUP_Background::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Level Up Background Left
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LevelUp_BG_Left"), CUI_LevelUP_BG_Left::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Level Up Background Right
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LevelUp_BG_Right"), CUI_LevelUP_BG_Right::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Level Up Fill Lead Left
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LevelUp_Fill_Lead_Left"), CUI_LevelUP_Fill_Lead_Left::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Level Up Fill Lead Right
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LevelUp_Fill_Lead_Right"), CUI_LevelUP_Fill_Lead_Right::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Level Up Fill Left
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LevelUp_Fill_Left"), UI_LevelUP_Fill_Left::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Level Up Fill Right
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_LevelUp_Fill_Right"), UI_LevelUP_Fill_Right::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//////////////////////////////
	//Guide UI
	//Background
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Guide_Background"), CUI_Guide_Background::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//Tex
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Guide_Texture"), CUI_Guide_Texture::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Load_Texture()
{
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, TEXT("Texture_BackGround"), TEXT("../bin/Resources/Texture/Loading/T_LoadScreen_KeyArt_5.dds"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, TEXT("Texture_SilvermeanNewHair"), TEXT("../bin/FBX/Texture/T_Silvermane_Hairs_d_new.dds"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Init_Camera()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"MainCamera";
	tDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	tDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	tDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);
	//tDesc.vEye = _float4(0.f, 2.f, -5.f, 1.f);
	//tDesc.vAt = _float4(0.f, 2.f, 0.f, 1.f);
	//tDesc.vAxisY = _float4(0.f, 1.f, 0.f, 1.f);
	tDesc.fFovy = XMConvertToRadians(90.f);
	tDesc.fFar = 300.f;
	tDesc.fNear = 0.1f;
	tDesc.fAspect = (_float)g_iWinCx/g_iWinCy;

	if(FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Camera", L"Proto_GameObject_MainCamera", &tDesc)))
		return E_FAIL;

	CCamera::CAMERADESC tDesc2;
	tDesc2.eType = CCamera::CAMERATYPE::CAMERA_ORTHO;
	tDesc2.pCameraTag = L"MainOrthoCamera";
	tDesc2.fWinCX = g_iWinCx;
	tDesc2.fWinCY = g_iWinCy;
	tDesc2.fNear = 0.01f;
	tDesc2.fFar = 1.f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Camera", L"Proto_GameObject_MainOrthoCamera", &tDesc2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	// MakeSpriteFont "Gulim" /FontSize:16 /characterregion:0xa960 - 0xa97f Gulim.spritefont
	if (FAILED(g_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_Arial"), TEXT("../bin/Resources/Font/Arial.spritefont"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_Gulim"), TEXT("../bin/Resources/Font/Qulim.spritefont"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_HY"), TEXT("../bin/Resources/Font/hy.spritefont"))))
		return E_FAIL;

	return S_OK;
}

void CMainApp::RenderingBnt()
{
	if (g_pGameInstance->getkeyDown(DIK_F1))
	{
		m_bHDR = !m_bHDR;
		m_pRenderer->SetRenderButton(CRenderer::HDR, m_bHDR);
	}
	if (g_pGameInstance->getkeyDown(DIK_F2))
	{
		m_bDBG = !m_bDBG;
		m_pRenderer->SetRenderButton(CRenderer::DBG, m_bDBG);
	}
	if (g_pGameInstance->getkeyDown(DIK_F3))
	{
		m_bShadow = !m_bShadow;
		m_pRenderer->SetRenderButton(CRenderer::SHADOW, m_bShadow);
	}
	if (g_pGameInstance->getkeyDown(DIK_F4))
	{
		m_bOutline = !m_bOutline;
		m_pRenderer->SetRenderButton(CRenderer::OUTLINE, m_bOutline);
	}
	if (g_pGameInstance->getkeyDown(DIK_F5))
	{
		m_bMotionblur = !m_bMotionblur;
		m_pRenderer->SetRenderButton(CRenderer::VELOCITYBLUR, m_bMotionblur);
	}
	if (g_pGameInstance->getkeyDown(DIK_F6))
	{
		m_bMotiontrail = !m_bMotiontrail;
		m_pRenderer->SetRenderButton(CRenderer::MOTIONTRAIL, m_bMotiontrail);
	}
}

const _bool CMainApp::IsFreeze() const
{
	return m_isFreeze;
}

const _bool CMainApp::IsDeltaTimeZero() const
{
	return m_isDeltaTimeZero;
}

void CMainApp::FreezeTime()
{
	if (!m_isFreeze)
	{
		m_isFreeze = true;
	}
}

void CMainApp::Set_DeltaTimeZero(const _bool _isDetaTimeZero)
{
	m_isDeltaTimeZero = _isDetaTimeZero;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}	

	return pInstance;
}

void CMainApp::Free()
{
#ifdef _DEBUG
	g_pDebugSystem->Stop_DebugSystem();
	Safe_Release(g_pDebugSystem);
#endif
	CShakeManager::DestroyInstance();
	CMeshLoader::DestroyInstance();
	CDataManager::DestroyInstance();
	CInven_UIManager::DestroyInstance();
	CWeaponGenerator::DestroyInstance();
	CInteractManager::DestroyInstance();
	CQuestManager::DestroyInstance();
	CGuideUIManager::DestroyInstance();
	CScenematicManager::DestroyInstance();

	Safe_Release(g_pObserver);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	
	CGameInstance::Release_Engine();
}

