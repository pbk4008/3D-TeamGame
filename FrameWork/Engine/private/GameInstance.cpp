#include "..\public\GameInstance.h"
#include "GameObject.h"

ENGINE_DLL CGameInstance* g_pGameInstance = nullptr;
ENGINE_DLL mt19937 g_random;		// mt19937 난수 엔진
_float g_fDeltaTime;
_double g_dDeltaTime;
_float g_fImmutableTime;
_double g_dImmutableTime;

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pInput_Device(CInputDev::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pFrustum(CFrustum::GetInstance())
	, m_pFont_Manager(CFont_Manager::GetInstance())
	, m_pTextureManager(CTextureManager::GetInstance())
	, m_pMaterial_Manager(CMaterial_Manager::GetInstance())
	, m_pSaveManager(CSaveManager::GetInstance())
	, m_pSoundManager(CSoundMgr::GetInstance())
	, m_pPhysicSystem(CPhysicsXSystem::GetInstance())
	, m_pEffectManager(CEffectManager::GetInstance())
	, m_pResourceManager(CResourceManager::GetInstance())
{
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pTextureManager);
	Safe_AddRef(m_pMaterial_Manager);
	Safe_AddRef(m_pSaveManager);
	Safe_AddRef(m_pSoundManager);
	Safe_AddRef(m_pPhysicSystem);
	Safe_AddRef(m_pEffectManager);
	Safe_AddRef(m_pResourceManager);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, HWND hWnd, _uint iNumLevel, CGraphic_Device::WINMODE eWinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pObject_Manager || 
		nullptr == m_pFrustum)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(hWnd, eWinMode, iWinCX, iWinCY, ppDeviceOut, ppDeviceContextOut)))
		return E_FAIL;	

	if (FAILED(m_pInput_Device->Init_InputDevice(hInst, hWnd)))
		return E_FAIL;

	if (FAILED(m_pPhysicSystem->Init_PhysX()))
		return E_FAIL;

	if (FAILED(m_pMaterial_Manager->NativeConstruct(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pEffectManager->Ready_EffectManager(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevel)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevel)))
		return E_FAIL;	

	if (FAILED(m_pFrustum->Ready_FrustumInProjSpace()))
		return E_FAIL;

	if (FAILED(m_pResourceManager->NativeConstruct(TRUE, m_ResourcePath.engineResourcePath, m_ResourcePath.clientResourcePath)))
		return E_FAIL;

	g_pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region 난수엔진 초기화후 클라에서 사용할 수 있게 전역변수에 넣어주기~
	random_device rd;
	mt19937 random(rd());
	g_random = random;
#pragma endregion

	return S_OK;
}

_int CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager || 
		nullptr == m_pPipeLine ||
		nullptr == m_pInput_Device || 
		nullptr == m_pFrustum)
		return -1;

	g_fDeltaTime = (_float)TimeDelta;
	g_dDeltaTime = TimeDelta;

	_int	iProgress = 0;

	//m_pInput_Device->Update_InputDev();

	iProgress = m_pLevel_Manager->Tick(TimeDelta);
	if (0 > iProgress)
		return -1;

	iProgress = m_pObject_Manager->Tick(TimeDelta);
	if (0 > iProgress)
		return -1;

	m_pPipeLine->Update_PipeLine();

	if (m_pPipeLine->getCameraCount())
		m_pFrustum->Transform_ToWorldSpace(m_pPipeLine->getBaseCamera());	
	
	m_pPhysicSystem->Tick(TimeDelta);

	iProgress = m_pObject_Manager->LateTick(TimeDelta);
	if (0 > iProgress)
		return -1;
	iProgress = m_pLevel_Manager->LateTick(TimeDelta);

	if (0 > iProgress)
		return -1;

	return _int();
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pLevel_Manager->Render();	
	return S_OK;
}

HRESULT CGameInstance::Clear_Engine(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pObject_Manager->Clear_Object_Manager(iLevelIndex);

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(XMFLOAT4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);;
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

ID3D11Device* CGameInstance::Get_Device()
{
	if(nullptr == m_pGraphic_Device)
		return nullptr;
	return m_pGraphic_Device->Get_Device();
}

ID3D11DeviceContext* CGameInstance::Get_DeviceContext()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;
	return m_pGraphic_Device->Get_DeviceContext();
}

_double CGameInstance::Get_TimeDelta(const wstring& pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0;

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

void CGameInstance::Update_TimeDelta(const wstring& pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	return m_pTimer_Manager->Update_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Ready_Timer(const wstring& pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Ready_Timer(pTimerTag);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pOpenLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pOpenLevel);
}

HRESULT CGameInstance::Delete_GameObject()
{
	if (!m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Delete_GameObject();
}

_uint CGameInstance::getCurrentLevel()
{
	if (!m_pLevel_Manager)
		return 99;

	return m_pLevel_Manager->getCurrentLevel();
}

CLevel* CGameInstance::getCurrentLevelScene()
{
	if (!m_pLevel_Manager)
		return nullptr;

	return m_pLevel_Manager->getCurrentLevelScene();
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(const wstring& pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObjectToLayer(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pPrototypeTag, void * pArg, CGameObject** ppOut /* = nullptr */)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObjectToLayer(iLevelIndex, pLayerTag, pPrototypeTag, pArg, ppOut);
}

HRESULT CGameInstance::Add_GameObjectToLayer(_uint iLevelIndex, const wstring& pLayerTag, CGameObject* pGameObject)
{
	if (!m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObjectToLayer(iLevelIndex, pLayerTag, pGameObject);
}

list<CGameObject*>* CGameInstance::getObjectList(_uint iLevelIndex, const wstring& pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->getObjectList(iLevelIndex, pLayerTag);
}

list<CGameObject*> CGameInstance::getAllObjectList()
{
	return m_pObject_Manager->getAllObjectList();
}

void CGameInstance::Clear_Object_List(void)
{
	if (nullptr == m_pObject_Manager)
		return;

	return m_pObject_Manager->Clear_Object_List();
}

CGameObject* CGameInstance::Clone_GameObject(_uint iLevelIndex, const wstring& pPrototypeTag, void* pArg)
{
	if (!m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Clone_Gameobject(iLevelIndex, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring& pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::SetUpBaseComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->SetUpBaseComponent(pDevice, pDeviceContext);
}

void CGameInstance::Clear_Component(void)
{
	if (nullptr == m_pComponent_Manager)
		return;

	return m_pComponent_Manager->Clear_Component();
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring& pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Camera(const wstring& pCameraTag)
{
	if (!m_pPipeLine)
		return E_FAIL;

	return m_pPipeLine->Add_Camera(pCameraTag);
}

void CGameInstance::Update_PipeLine()
{
	if (!m_pPipeLine)
		return;

	return m_pPipeLine->Update_PipeLine();
}

void CGameInstance::Delete_Camera()
{
	if (!m_pPipeLine)
		return;

	return m_pPipeLine->Delete_Camera();
}

_fmatrix CGameInstance::Get_Transform(const wstring& pCameraTag, TRANSFORMSTATEMATRIX eType)
{
	if (!m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform(pCameraTag,eType);
}

_fvector CGameInstance::Get_CamPosition(const wstring& pCameraTag)
{
	if (!m_pPipeLine)
		return XMVectorZero();

	return m_pPipeLine->Get_CamPosition(pCameraTag);
}

void CGameInstance::Set_Transform(const wstring& pCameraTag, TRANSFORMSTATEMATRIX eType, _fmatrix TransformMatrix)
{
	if (!m_pPipeLine)
		return;

	return m_pPipeLine->Set_Transform(pCameraTag, eType, TransformMatrix);
}

HRESULT CGameInstance::Change_BaseCamera(const wstring& pCameraTag)
{
	if (!m_pPipeLine)
		return E_FAIL;

	return m_pPipeLine->Change_BaseCamera(pCameraTag);
}

const wstring& CGameInstance::Get_BaseCameraTag()
{
	if (!m_pPipeLine)
		MSGBOX("GameInstance PiepeLine null");

	return m_pPipeLine->getBaseCamera();
}

HRESULT CGameInstance::SetPreViewProtj(const wstring& cametag)
{
	if (!m_pPipeLine)
		MSGBOX("pipeline is null");

	return m_pPipeLine->SetPreViewProtj(cametag);
}

_fmatrix CGameInstance::GetPreViewProtj(_fmatrix world)
{
	if (!m_pPipeLine)
		MSGBOX("pipeline is null");

	return m_pPipeLine->GetPreViewProtj(world);
}

_matrix CGameInstance::Get_ViewMatrix()
{
	return m_pPipeLine->Get_ViewMatrix();
}

void CGameInstance::Update_InputDev()
{
	if (!m_pInput_Device)
		return;

	m_pInput_Device->Update_InputDev();
}

_bool CGameInstance::getkeyPress(_ubyte bykeyID)
{
	if (!m_pInput_Device)
		return false;

	return m_pInput_Device->getkeyPress(bykeyID);
}

_bool CGameInstance::getkeyDown(_ubyte bykeyID)
{
	if (!m_pInput_Device)
		return false;

	return m_pInput_Device->getkeyDown(bykeyID);
}

_bool CGameInstance::getkeyUp(_ubyte bykeyID)
{
	if (!m_pInput_Device)
		return false;

	return m_pInput_Device->getkeyUp(bykeyID);
}

_bool CGameInstance::getMousePress(CInputDev::MOUSESTATE eMouse)
{
	if (!m_pInput_Device)
		return false;

	return m_pInput_Device->getMousePress(eMouse);

}

_bool CGameInstance::getMouseKeyDown(CInputDev::MOUSESTATE eMouse)
{
	if (!m_pInput_Device)
		return false;

	return m_pInput_Device->getMouseKeyDown(eMouse);
}

_bool CGameInstance::getMouseKeyUp(CInputDev::MOUSESTATE eMouse)
{
	if (!m_pInput_Device)
		return false;

	return m_pInput_Device->getMouseKeyUp(eMouse);
}

_long CGameInstance::getMouseMoveState(CInputDev::MOUSEMOVESTATE eMouse)
{
	if (!m_pInput_Device)
		return -1;

	return m_pInput_Device->getMouseMoveState(eMouse);
}

_bool CGameInstance::getKeyboardNoKey()
{
	if (!m_pInput_Device)
		return false;

	return m_pInput_Device->getKeyboardNoKey();
}

_fvector CGameInstance::getMousePostion()
{
	if (!m_pInput_Device)
		return _fvector();

	return m_pInput_Device->getMousePostion();
}


const LIGHTDESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::CreateLightCam(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LIGHTDESC& desc)
{
	if (m_pLight_Manager == nullptr)
		return E_FAIL;

	m_pLight_Manager->CreateLightCam(pDevice, pDeviceContext, desc);

	return S_OK;
}

HRESULT CGameInstance::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;
	
	return m_pLight_Manager->Add_Light(pDevice, pDeviceContext, LightDesc);	
}

HRESULT CGameInstance::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc, CLight** ppOutLight)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pDeviceContext, LightDesc, ppOutLight);
}

void CGameInstance::UpdateLightCam(_uint LightIndx, _fvector playerpos)
{
	if (!m_pLight_Manager)
		return;

	m_pLight_Manager->UpdateLightCam(LightIndx, playerpos);
}

void CGameInstance::RemoveLight()
{
	if (m_pLight_Manager == nullptr)
		return;

	m_pLight_Manager->Free();
}

ID3D11ShaderResourceView* CGameInstance::Get_SRV(const wstring& target)
{
	if(m_pTarget_Manager == nullptr)
		return nullptr;

	return m_pTarget_Manager->Get_SRV(target.c_str());
}

HRESULT CGameInstance::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const wstring& pFontTag, const wstring& pFontPath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(pDevice, pDeviceContext, pFontTag, pFontPath);
}

HRESULT CGameInstance::Render_Font(const wstring& pFontTag, _fvector vColor, const wstring& pString, const _float2& _vPos, const _float2& _vScale)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render_Font(pFontTag, vColor, pString, _vPos, _vScale);
}

_bool CGameInstance::isIn_WorldFrustum(_fvector vPosition, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isInWorld(vPosition, fRange);
}

_bool CGameInstance::isIn_WorldFrustum(_float4* vPoints, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isInWorld(vPoints, fRange);
}


_bool CGameInstance::isIn_LocalFrustum(_fvector vPosition, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isInLocal(vPosition, fRange);
}

_float4* CGameInstance::Get_FrustumPlane()
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->Get_FrustumPlane();
}

HRESULT CGameInstance::Add_Texture(ID3D11Device* pDevice, const wstring& pTextureTag, const wstring& pFilePath, _uint iTextureCnt)
{
	if (!m_pTextureManager)
		return E_FAIL;
	return m_pTextureManager->Add_Texture(pDevice, pTextureTag, pFilePath, iTextureCnt);
}

HRESULT CGameInstance::Delete_Texture()
{
	if (!m_pTextureManager)
		return E_FAIL;
	return m_pTextureManager->Delete_Texture();
}

vector<ID3D11ShaderResourceView*>* CGameInstance::Get_Texture(const wstring& pTextureTag)
{
	if (!m_pTextureManager)
		return nullptr;
	return m_pTextureManager->Get_Texture(pTextureTag);
}

HRESULT CGameInstance::Add_Material(const wstring& _wstrMtrlTag, CMaterial* _pMtrl)
{
	if (!m_pMaterial_Manager)
		return E_FAIL;
		
	return m_pMaterial_Manager->Add_Material(_wstrMtrlTag, _pMtrl);
}

CMaterial* CGameInstance::Get_Material(const wstring& _wstrMtrlTag)
{
	if (!m_pMaterial_Manager)
		return nullptr;

	return m_pMaterial_Manager->Get_Material(_wstrMtrlTag);
}

HRESULT CGameInstance::Add_Effect(_uint iSceneID, const wstring& pLayerTag, CEffect* pEffect, _uint iCount)
{
	if (!m_pEffectManager)
		return E_FAIL;
	return m_pEffectManager->Add_Effect(iSceneID, pLayerTag, pEffect, iCount);
}

CEffect* CGameInstance::Get_Effect(_uint iEffectIndex)
{
	if (!m_pEffectManager)
		return nullptr;

	return m_pEffectManager->Get_Effect(iEffectIndex);
}

const _bool CGameInstance::Raycast(RAYCASTDESC & _desc)
{
	if (!m_pPhysicSystem)
		return false;
	return m_pPhysicSystem->Raycast(_desc);
}

const _bool CGameInstance::Sweep(SWEEPDESC& _desc)
{
	if (!m_pPhysicSystem)
		return false;
	return m_pPhysicSystem->Sweep(_desc);
}

const _bool CGameInstance::Overlap(OVERLAPDESC& _desc)
{
	if (!m_pPhysicSystem)
		return false;
	return m_pPhysicSystem->Overlap(_desc);
}

void CGameInstance::Set_NumLayers(const _uint _iNumLayers)
{
	if (!m_pPhysicSystem)
		return;
	m_pPhysicSystem->Set_NumLayers(_iNumLayers);
}

void CGameInstance::Set_CollisionLayer(const _uint _iLayer1, const _uint _iLayer2)
{
	if (!m_pPhysicSystem)
		return;
	m_pPhysicSystem->Set_CollisionLayer(_iLayer1, _iLayer2);
}

HRESULT CGameInstance::Init_SoundManager()
{
	if (!m_pSoundManager)
		return E_FAIL;

	return m_pSoundManager->Init_SoundManager();
}

void CGameInstance::Play_Shot(const wstring& pSoundKey, CSoundMgr::CHANNELID eID)
{
	if (!m_pSoundManager)
		return;
	m_pSoundManager->Play_Shot(pSoundKey, eID);
}

void CGameInstance::PlayBGM(const wstring& pSoundKey)
{
	if (!m_pSoundManager)
		return;
	m_pSoundManager->PlayBGM(pSoundKey);
}

void CGameInstance::StopSound(CSoundMgr::CHANNELID eID)
{
	if (!m_pSoundManager)
		return;
	m_pSoundManager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (!m_pSoundManager)
		return;
	m_pSoundManager->StopAll();
}

void CGameInstance::VolumeChange(CSoundMgr::CHANNELID eID, _float fVolume)
{
	if (!m_pSoundManager)
		return;

	m_pSoundManager->VolumeChange(eID, fVolume);
}

void CGameInstance::BlendSound(const std::wstring& pStartSoundKey, const std::wstring& pEndSoundKey, CSoundMgr::CHANNELID eStartID, CSoundMgr::CHANNELID eEndID, _float fBlendTiming)
{
	if (!m_pSoundManager)
		return;

	m_pSoundManager->BlendSound(pStartSoundKey, pEndSoundKey, eStartID, eEndID, fBlendTiming);
}

_bool CGameInstance::IsPlaying(CSoundMgr::CHANNELID eID)
{
	if (!m_pSoundManager)
		return false;

	return m_pSoundManager->IsPlaying(eID);
}

void CGameInstance::Release_Engine()
{
	RELEASE_INSTANCE(CGameInstance);

	if (0 != CGameInstance::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CGameInstance");	

	if (0 != CLevel_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CLevel_Manager");

	if(0 != CEffectManager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CEffectManager");

	if (0 != CObject_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CObject_Manager");

	if (0 != CComponent_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CComponent_Manager");

	if (0 != CMaterial_Manager::DestroyInstance())
		MSGBOX("Failed to Release CMaterial_Manager");

	if (0 != CTextureManager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CTextureManager");

	if (0 != CResourceManager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CResourceManager");

	if (0 != CPipeLine::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CPipeLine");

	if (0 != CTimer_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CTimer_Manager");

	if (0 != CInputDev::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CInput_Device");

	if (0 != CLight_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CLight_Manager");

	if (0 != CTarget_Manager::GetInstance()->DestroyInstance())
 		MSGBOX("Failed to Release CTarget_Manager");

	if (0 != CFrustum::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CFrustum");

	if (0 != CPhysicsXSystem::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CPhysicsXSystem");

	if (0 != CSaveManager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CSaveManager");

	if (0 != CSoundMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CSoundMgr");

	if (0 != CFont_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CFont_Manager");

	if (0 != CGraphic_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Release CGraphic_Device");
}

void CGameInstance::Free()
{
	Safe_Release(m_pEffectManager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pMaterial_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pTextureManager);
	Safe_Release(m_pResourceManager);
	Safe_Release(m_pSaveManager);
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pPhysicSystem);
}


