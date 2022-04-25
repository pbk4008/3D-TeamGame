#include "pch.h"
#include "GuideUIManager.h"
#include "UI_Guide_Background.h"
#include "UI_Guide_Texture.h"
#include "UI_Fill_Space.h"
#include "UI_Blank_Space.h"
#include "UI_Death.h"

CGuideUIManager::CGuideUIManager(void)
{
}

HRESULT CGuideUIManager::NativeConstruct(void)
{
	Ready_UIObject();

	return S_OK;
}

_int CGuideUIManager::Tick(_double dTimeDelta)
{
	dTimeDelta = g_dImmutableTime;
	ShowDeath();
	ShowNext();

	m_vecGuideTex[m_iCurrentTrigerrIdx]->Tick(dTimeDelta);

	if (m_pInvenGuide->getActive())
	{
		if (g_pGameInstance->getkeyDown(DIK_SPACE))
		{
			Hide();
		}
	}

	return _int();
}

_int CGuideUIManager::Late_Tick(_double dTimeDelta)
{
	dTimeDelta = g_dImmutableTime;

	if (m_vecGuideTex[m_iCurrentTrigerrIdx]->getActive())
	{
		g_pMainApp->Set_DeltaTimeZero(true);
		m_bGuideUIShow = true;
		m_vecGuideTex[m_iCurrentTrigerrIdx]->LateTick(dTimeDelta);
		m_pBg->LateTick(dTimeDelta);
	}

	if (true == m_bInvenRender)
	{
		if (m_pInvenGuide->getActive())
			m_pInvenGuide->LateTick(dTimeDelta);
	}

	if (m_pDeath->getActive())
	{
		m_pDeath->LateTick(dTimeDelta);
	}

	return _int();
}

HRESULT CGuideUIManager::Render(void)
{
	return S_OK;
}

void CGuideUIManager::Ready_UIObject(void)
{
	//Blank_SpaceKey
	g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Blank_SpaceKey"), CUI_Blank_Space::Create(g_pGameInstance->Get_Device(), g_pGameInstance->Get_DeviceContext()));
	g_pGameInstance->Add_Texture(g_pGameInstance->Get_Device(), L"Texture_Blank_Spacekey", L"../bin/Resources/Texture/UI/Static/Active/T_Keyboard_Dark_Key_Space.dds");

	//Fill_SpaceKey
	g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Fill_SpaceKey"), CUI_Fill_Space::Create(g_pGameInstance->Get_Device(), g_pGameInstance->Get_DeviceContext()));
	g_pGameInstance->Add_Texture(g_pGameInstance->Get_Device(), L"Texture_Fill_Spacekey", L"../bin/Resources/Texture/UI/Static/Active/T_Keyboard_Light_Key_Space.dds");

	//Blank_Spacekey
	{
		CUI_Blank_Space::UIACTIVEDESC Desc;
		ZeroMemory(&Desc, sizeof(CUI_Blank_Space::UIACTIVEDESC));
		_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Blank_Spacekey");
		Desc.UIDesc.bMinus = false;
		Desc.UIDesc.fAngle = 0.f;
		Desc.UIDesc.fPos = { 50.f, 0.f, 0.1f };
		Desc.UIDesc.fSize = { 40.f , 40.f };
		Desc.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

		g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_UI_BlankSpace", L"Proto_GameObject_UI_Blank_SpaceKey", &Desc);
	}
	//Fill_Spacekey
	{
		CUI_Fill_Space::UIACTIVEDESC Desc;
		ZeroMemory(&Desc, sizeof(CUI_Fill_Space::UIACTIVEDESC));
		_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Fill_Spacekey");
		Desc.UIDesc.bMinus = false;
		Desc.UIDesc.fAngle = 0.f;
		Desc.UIDesc.fPos = { 50.f, 0.f, 0.09f };
		Desc.UIDesc.fSize = { 40.f , 40.f };
		Desc.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

		g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_UI_FillSpace", L"Proto_GameObject_UI_Fill_SpaceKey", &Desc);
	}

	m_pBg = (CUI_Guide_Background*)static_cast<CUI*>(
		g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Guide_Background"));
	assert(m_pBg);

	vector<TRIGGER> vecPonit;
	if (FAILED(g_pGameInstance->LoadFile<TRIGGER>(vecPonit, L"../bin/SaveData/Trigger/Stage1_GuideUITrigger.dat")))
		MSGBOX(L"가이드트리거 데이터 못읽음");
	if(0 == vecPonit.size())
		MSGBOX(L"가이드트리거 데이터 못읽음");
	CUI_Guide_Texture* pGuideTex = nullptr;

	m_vecGuideTex.reserve(vecPonit.size());

	for (_int i = 0; i < vecPonit.size(); ++i)
	{
		CUI_Guide_Texture::Desc desc;
		desc.fPos = vecPonit[i].fTrigger_Point;
		desc.iNumTex = m_arrStage1[i]; /* 가이드별로 가지고 있을 텍스쳐의 갯수 */

		pGuideTex = (CUI_Guide_Texture*)static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Guide_Texture", &desc));
		assert(pGuideTex);
		pGuideTex->setActive(false);

		m_vecGuideTex.push_back(pGuideTex);
	}

	{
		CUI_Guide_Texture::Desc desc;
		desc.ERenderGroup = CRenderer::RENDER::RENDER_UI_ACTIVE;

		m_pInvenGuide = (CUI_Guide_Texture*)static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Guide_Texture", &desc));
		assert(m_pInvenGuide);
		m_pInvenGuide->SetTexture(L"T_HUD_Guide_Inven");
	}

	{
		m_pDeath = (CUI_Death*)static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Death"));
		assert(m_pDeath);
		m_pDeath->setActive(false);
	}

	
}

void CGuideUIManager::Show()
{
	g_pMainApp->Set_DeltaTimeZero(true);
	m_pInvenGuide->setActive(true);
	m_bGuideUIShow = true;
}
 
void CGuideUIManager::Hide(void)
{
	g_pMainApp->Set_DeltaTimeZero(false);

	if (m_pInvenGuide->getActive())
	{
		m_bGuideUIShow = false;
	}

	m_pInvenGuide->setActive(false);
	m_bInvenRender = false;
}

void CGuideUIManager::ShowDeath(void)
{
	m_pPlayer = (CActor*)*g_pGameInstance->getObjectList(g_pGameInstance->getCurrentLevel(), L"Layer_Silvermane")->begin();

	if (m_pPlayer->Get_Dead())
	{
		m_pDeath->setActive(true);
	}
}

void CGuideUIManager::Respawn(void)
{
	m_pDeath->setActive(false);
	m_pDeath->HideSpaceKey();
	g_pMainApp->Set_DeltaTimeZero(false);
}

_bool CGuideUIManager::IsOpenGuideUI(void)
{
	return m_bGuideUIShow;
}

_bool CGuideUIManager::IsOpenDeathUI(void)
{
	return m_pDeath->getActive();
}

void CGuideUIManager::ShowNext(void)
{

	switch (m_iCurrentTrigerrIdx)
	{
	case 0:
		m_szTextureName = arrFirst[m_iTexIdx];
		break;
	case 1:
		m_szTextureName = arrSecond[m_iTexIdx];
		break;
	case 2:
		m_szTextureName = arrThird[m_iTexIdx];
		break;
	case 3:
		m_szTextureName = arrFourth[m_iTexIdx];
		break;
	}

	m_vecGuideTex[m_iCurrentTrigerrIdx]->SetTexture(m_szTextureName);

	if (true == m_bGuideUIShow && false == m_bInvenRender)
	{
		if (g_pGameInstance->getkeyDown(DIK_SPACE))
		{
			if (m_iTexIdx + 1 < m_vecGuideTex[m_iCurrentTrigerrIdx]->GetNumTex())
			{
				++m_iTexIdx;

				switch (m_iCurrentTrigerrIdx)
				{
				case 0:
					m_szTextureName = arrFirst[m_iTexIdx];
					break;
				case 1:
					m_szTextureName = arrSecond[m_iTexIdx];
					break;
				case 2:
					break;
					m_szTextureName = arrThird[m_iTexIdx];
				case 3:
					m_szTextureName = arrFourth[m_iTexIdx];
					break;
				}
				m_vecGuideTex[m_iCurrentTrigerrIdx]->SetTexture(m_szTextureName);
			}
			else
			{
				g_pMainApp->Set_DeltaTimeZero(false);

				m_vecGuideTex[m_iCurrentTrigerrIdx]->setActive(false);
				m_iTexIdx = 0;
				if(m_iCurrentTrigerrIdx < 4)
					++m_iCurrentTrigerrIdx;

				if (4 == m_iCurrentTrigerrIdx)
					m_iCurrentTrigerrIdx = 0;

				m_bGuideUIShow = false;
			}
		}
	}
}

void CGuideUIManager::Free()
{
	for (auto iter : m_vecGuideTex)
		Safe_Release(iter);
	m_vecGuideTex.clear();

	Safe_Release(m_pBg);
	Safe_Release(m_pInvenGuide);
	Safe_Release(m_pDeath);
}
