#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "PipeLine.h"
#include "Light_Manager.h"
#include "Target_Manager.h"
#include "Font_Manager.h"
#include "Frustum.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
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

public: /* For.Timer_Manager */ 
	_double Get_TimeDelta(const _tchar* pTimerTag);
	void Update_TimeDelta(const _tchar* pTimerTag);
	HRESULT Ready_Timer(const _tchar* pTimerTag);

public: /* For.Level_Manager*/
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pOpenLevel);

public: /* For.Object_Manager*/
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	
public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);

public: /* For.PipeLine */
	_fmatrix Get_Transform(CPipeLine::TRANSFORMSTATEMATRIX eType);
	_fvector Get_CamPosition();
	void Set_Transform(CPipeLine::TRANSFORMSTATEMATRIX eType, _fmatrix TransformMatrix);

public: /* for.Input_Device */
	_byte Get_DIKeyState(_ubyte byKeyID) const;
	_long Get_MouseMoveState(CInput_Device::MOUSEMOVESTATE eMoveState) const;
	_byte Get_MouseButtonState(CInput_Device::MOUSEBUTTONSTATE eButtonState) const;

public: /* For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex = 0);
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);

public: /* For.Font_Manager */
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontPath);
	HRESULT Render_Font(const _tchar* pFontTag, _fvector vColor, const _tchar* pString);


public: /* For.Frustum */
	_bool isIn_WorldFrustum(_fvector vPosition, _float fRange = 0.f);
	_bool isIn_LocalFrustum(_fvector vPosition, _float fRange = 0.f);
	
private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;		
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CPipeLine*					m_pPipeLine = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CLight_Manager*				m_pLight_Manager = nullptr;
	CTarget_Manager*			m_pTarget_Manager = nullptr;
	CFrustum*					m_pFrustum = nullptr;
	CFont_Manager*				m_pFont_Manager = nullptr;
public:

	static void Release_Engine();
	virtual void Free() override;
};

END

