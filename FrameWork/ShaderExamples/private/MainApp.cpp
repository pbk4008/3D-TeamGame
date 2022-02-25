#include "..\public\MainApp.h"

#include "GameInstance.h"
//#include "Level_Loading.h"
//#include "BackGround.h"
#include "Floor.h"
#include "DynamicCaemra.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, (_uint)LEVEL::LEVEL_END, CGraphic_Device::MODE_WIN, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;

	//if (FAILED(Ready_Fonts()))
	//	return E_FAIL;

	//if (FAILED(Ready_Gara()))
	//	return E_FAIL;
	if (FAILED(g_pGameInstance->SetUpBaseComponent(m_pDevice, m_pDeviceContext)))
		return E_FAIL;

	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	//if (FAILED(SetUp_StartLevel(LEVEL::LEVEL_LOGO)))
	//	return E_FAIL;

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(1.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	//LightDesc.eType = LIGHTDESC::TYPE_POINT;

	//LightDesc.vPosition = _float3(5.f, 3.f, 5.f);
	//LightDesc.fRange = 7.f;

	//LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.0f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;


	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	//LightDesc.eType = LIGHTDESC::TYPE_POINT;

	//LightDesc.vPosition = _float3(10.f, 3.f, 5.f);
	//LightDesc.fRange = 7.f;

	//LightDesc.vDiffuse = _float4(0.0f, 1.f, 0.0f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;

	//LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	//LightDesc.vDirection = _float3(1.f, -1.f, 1.f);
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;

	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return -1;

	m_TimeAcc += TimeDelta;

	m_pGameInstance->Tick_Engine(TimeDelta);

	return _int();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(XMFLOAT4(0.1f, 0.2f, 0.3f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;

	/* 내 게임을 구성하는객체들의 렌더함수르리 호출하낟. */
	if (FAILED(m_pRenderer->Draw_RenderGroup()))
		return E_FAIL;

	/* 내 게임내의 기타등ㄷ응을 렌더링하낟. */
	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

//#ifdef _DEBUG
//	++m_iNumRender;
//
//	if (m_TimeAcc >= 1.0)
//	{
//		wsprintf(m_szFPS, TEXT("Frame : %d"), m_iNumRender);
//		m_iNumRender = 0;
//		m_TimeAcc = 0.0;
//	}
//
//	if (FAILED(m_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), m_szFPS)))
//		return E_FAIL;
//
//#endif // _DEBUG

	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::SetUp_StartLevel(LEVEL eLevel)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	HRESULT		hr = 0;

	switch (eLevel)
	{
	case LEVEL::LEVEL_LOGO: case LEVEL::LEVEL_GAMEPLAY:
		//hr = m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, eLevel));
		break;

	default:
		return E_FAIL;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Component_Prototype()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)LEVEL::LEVEL_STATIC, TEXT("PrototypeTerrainVIBuffer"), CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../bin/ShaderFiles/Shader_Terrain.hlsl"), TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Loading */
	//if (FAILED(m_pGameInstance->Add_Prototype((_uint)LEVEL::LEVEL_STATIC, TEXT("PrototypeTerrain"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../bin/Resources/Textures/Terrain/Grass_1.tga")))))
	//	return E_FAIL;

	g_pGameInstance->Add_Texture(m_pDevice, L"TerrainBase", L"../bin/Resources/Textures/Terrain/Grass_1.tga");

	//if (FAILED(m_pGameInstance->Add_Prototype((_uint)LEVEL::LEVEL_STATIC, TEXT("PtotocomponentCamera"), CCamera::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	m_pRenderer = g_pGameInstance->Clone_Component<CRenderer>(0, L"Renderer");

	return S_OK;
}

HRESULT CMainApp::Ready_GameObject_Prototype()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_GameObject_BackGround */

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeFloor"), CFloor::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeCam"), CDynamicCaemra::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObjectToLayer((_uint)LEVEL::LEVEL_STATIC, L"Floor", L"PrototypeFloor")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObjectToLayer((_uint)LEVEL::LEVEL_STATIC, L"DyCam", L"PrototypeCam")))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	// MakeSpriteFont "Gulim" /FontSize:16 /characterregion:0xa960 - 0xa97f Gulim.spritefont
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_Arial"), TEXT("../Bin/Fonts/Arial.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_Gulim"), TEXT("../Bin/Fonts/Qulim.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_HY"), TEXT("../Bin/Fonts/hy.spritefont"))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Ready_Gara()
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(TEXT("../Bin/Data/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3];

	ZeroMemory(vPoints, sizeof(_float3) * 3);
	vPoints[0] = _float3(0.f, 0.1f, 5.f);
	vPoints[1] = _float3(5.f, 0.1f, 0.f);
	vPoints[2] = _float3(0.f, 0.1f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoints, sizeof(_float3) * 3);
	vPoints[0] = _float3(0.f, 0.1f, 5.f);
	vPoints[1] = _float3(5.f, 0.1f, 5.f);
	vPoints[2] = _float3(5.f, 0.1f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoints, sizeof(_float3) * 3);
	vPoints[0] = _float3(0.f, 0.1f, 10.f);
	vPoints[1] = _float3(5.f, 0.1f, 5.f);
	vPoints[2] = _float3(0.f, 0.1f, 5.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoints, sizeof(_float3) * 3);
	vPoints[0] = _float3(5.f, 0.1f, 5.f);
	vPoints[1] = _float3(10.f, 0.1f, 0.f);
	vPoints[2] = _float3(5.f, 0.1f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pRenderer);

	CGameInstance::Release_Engine();
}

