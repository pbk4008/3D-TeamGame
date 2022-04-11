#pragma once

#include "SingleTon.h"

#include "Graphic_Device.h"
#include "InputDev.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "PipeLine.h"
#include "Light_Manager.h"
#include "Target_Manager.h"
#include "Font_Manager.h"
#include "Frustum.h"
#include "SaveManager.h"
#include "SoundMgr.h"
#include "TextureManager.h"
#include "Material_Manager.h"
#include "PhysicsXSystem.h"
#include "EffectManager.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameInstance final : public CSingleTon<CGameInstance>
{
	friend CSingleTon;
public:
	NO_COPY(CGameInstance);
	explicit CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(HINSTANCE hInst, HWND hWnd, _uint iNumLevel, CGraphic_Device::WINMODE eWinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	_int Tick_Engine(_double TimeDelta);

	/* 엔진에 정의되어있는 매니져객체들을 통해 디버깅적 요소들을 출력한다.  */
	/* 게임의 구성요소들은 렌더러를 통해 렌더하낟. */
	HRESULT Render_Engine(); 
	HRESULT Clear_Engine(_uint iLevelIndex);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(XMFLOAT4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();
	ID3D11Device* Get_Device();
	ID3D11DeviceContext* Get_DeviceContext();

public: /* For.Timer_Manager */ 
	_double Get_TimeDelta(const wstring& pTimerTag);
	void Update_TimeDelta(const wstring& pTimerTag);
	HRESULT Ready_Timer(const wstring& pTimerTag);

public: /* For.Level_Manager*/
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pOpenLevel);
	HRESULT Delete_GameObject();
	_uint getCurrentLevel();
	CLevel* getCurrentLevelScene();

public: /* For.Object_Manager*/
	class CComponent* Get_Component(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const wstring& pPrototypeTag, CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pPrototypeTag, void* pArg = nullptr, class CGameObject** ppOut = nullptr);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const wstring& pLayerTag, CGameObject* pGameObject);
	CGameObject* Clone_GameObject(_uint iLevelIndex, const wstring& pPrototypeTag, void* pArg = nullptr);
	list<CGameObject*>* getObjectList(_uint iLevelIndex, const wstring & pLayerTag);
	list<CGameObject*> getAllObjectList();
	void			  Clear_Object_List(void);
	template<typename T>
	T* Clone_GameObject(_uint iLevelIndex, const wstring& pPrototypeTag, void* pArg = nullptr)
	{
		CGameObject* pObj = Clone_GameObject(iLevelIndex, pPrototypeTag, pArg);

		if (!pObj)
			return nullptr;
		
		return static_cast<T*>(pObj);
	}

	template<typename T>
	T* Get_Component(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pComponentTag, _uint iIndex = 0)
	{
		CComponent* pCom = Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);

		if (!pCom)
			return nullptr;

		return static_cast<T*>(pCom);
	}
public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& pPrototypeTag, class CComponent* pPrototype);
	HRESULT SetUpBaseComponent(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	CComponent* Clone_Component(_uint iLevelIndex, const wstring& pPrototypeTag, void* pArg = nullptr);
	void	Clear_Component(void);
	template<typename T>
	T* Clone_Component(_uint iLevelIndex, const wstring& pPrototypeTag, void* pArg = nullptr)
	{
		CComponent* pCom = Clone_Component(iLevelIndex, pPrototypeTag, pArg);
		if (!pCom)
			return nullptr;

		return static_cast<T*>(pCom);
	}

public: /* For.PipeLine */
	HRESULT Add_Camera(const wstring& pCameraTag);
	void Update_PipeLine();
	void Delete_Camera();
	_fmatrix Get_Transform(const wstring& pCameraTag, TRANSFORMSTATEMATRIX eType);
	_fvector Get_CamPosition(const wstring& pCameraTag);
	void Set_Transform(const wstring& pCameraTag, TRANSFORMSTATEMATRIX eType, _fmatrix TransformMatrix);
	HRESULT Change_BaseCamera(const wstring& pCameraTag);
	const wstring& Get_BaseCameraTag();

public: /* for.Input_Device */
	void Update_InputDev();
	_bool getkeyPress(_ubyte bykeyID);
	_bool getkeyDown(_ubyte bykeyID);
	_bool getkeyUp(_ubyte bykeyID);
	_bool getMousePress(CInputDev::MOUSESTATE eMouse);
	_bool getMouseKeyDown(CInputDev::MOUSESTATE eMouse);
	_bool getMouseKeyUp(CInputDev::MOUSESTATE eMouse);
	_long getMouseMoveState(CInputDev::MOUSEMOVESTATE eMouse);
	_bool getKeyboardNoKey();

public: /* For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex = 0);
	HRESULT	CreateLightCam(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LIGHTDESC& desc);
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc, CLight** ppOutLight);
	void UpdateLightCam(_uint LightIndx, _fvector playerpos);

public: /* Target Mgr*/
	ID3D11ShaderResourceView* Get_SRV(const wstring& target);

public: /* For.Font_Manager */
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pFontTag, const wstring& pFontPath);
	HRESULT Render_Font(const wstring& pFontTag, _fvector vColor, const wstring& pString, const _float2& _vPos = { 0.f, 0.f }, const _float2& _vScale = { 1.f, 1.f });

public: /* For.Frustum */
	_bool isIn_WorldFrustum(_fvector vPosition, _float fRange = 0.f);
	_bool isIn_WorldFrustum(_float4* vPoints, _float fRange = 0.f);
	_bool isIn_LocalFrustum(_fvector vPosition, _float fRange = 0.f);

public:/* For.TextureManager*/
	HRESULT Add_Texture(ID3D11Device* pDevice, const wstring& pTextureTag, const wstring& pFilePath, _uint iTextureCnt = 1);
	HRESULT Delete_Texture();
	vector<ID3D11ShaderResourceView*>* Get_Texture(const wstring& pTextureTag);

public: /* For.Material_Manager */
	HRESULT Add_Material(const wstring& _wstrMtrlTag, CMaterial* _pMtrl);
	CMaterial* Get_Material(const wstring& _wstrMtrlTag = L"Default");

public:/* For. Effect_Manager*/
	HRESULT Add_Effect(_uint iSceneID, const wstring& pLayerTag, CEffect* pEffect, _uint iCount);
	CEffect* Get_Effect(_uint iEffectIndex);

public:/* For.SaveManager*/
	template<typename T>
	HRESULT SaveFile(void* pSaveData, const wstring& pFilePath)
	{
		if (!m_pSaveManager)
			return E_FAIL;

		return m_pSaveManager->SaveFile<T>(pSaveData, pFilePath);
	}
	template<typename T>
	HRESULT LoadFile(vector<T>& pLoadData, const wstring& pFilePath)
	{
		if (!m_pSaveManager)
			return E_FAIL;

		return m_pSaveManager->LoadFile<T>(pLoadData, pFilePath);
	}

public:/* For.PhysX */
	const _bool Raycast(RAYCASTDESC& _desc);
	const _bool Sweep(SWEEPDESC& _desc);
	const _bool Overlap(OVERLAPDESC& _desc);

public: /* For.SoundMgr */
	HRESULT Init_SoundManager();
	void Play_Shot(const wstring& pSoundKey, CSoundMgr::CHANNELID eID);
	void PlayBGM(const wstring& pSoundKey);
	void StopSound(CSoundMgr::CHANNELID eID);
	void StopAll();
	void VolumeChange(CSoundMgr::CHANNELID eID, _float fVolume);
	void BlendSound(const std::wstring& pStartSoundKey,
		const std::wstring& pEndSoundKey,
		CSoundMgr::CHANNELID eStartID,
		CSoundMgr::CHANNELID  eEndID,
		_float fBlendTiming = 0.0f);

private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;		
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CPipeLine*					m_pPipeLine = nullptr;
	CInputDev*					m_pInput_Device = nullptr;
	CLight_Manager*				m_pLight_Manager = nullptr;
	CTarget_Manager*			m_pTarget_Manager = nullptr;
	CFrustum*					m_pFrustum = nullptr;
	CFont_Manager*				m_pFont_Manager = nullptr;
	CTextureManager*			m_pTextureManager = nullptr;
	CMaterial_Manager*			m_pMaterial_Manager = nullptr;
	CSaveManager*				m_pSaveManager = nullptr;
	CSoundMgr*					m_pSoundManager = nullptr;
	CPhysicsXSystem*			m_pPhysicSystem = nullptr;
	CEffectManager*			m_pEffectManager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END

ENGINE_DLL extern CGameInstance* g_pGameInstance;
ENGINE_DLL extern mt19937 g_random;
ENGINE_DLL extern _float g_fDeltaTime;
ENGINE_DLL extern _double g_dDeltaTime;
ENGINE_DLL extern _float g_fImmutableTime;
ENGINE_DLL extern _double g_dImmutableTime;