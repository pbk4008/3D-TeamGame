#pragma once

#include "Component.h"

BEGIN(Engine)

class CTarget_Manager;

class ENGINE_DLL CRenderer final : public CComponent
{
public: enum RENDERBUTTON
{
	SHADOW,PBRHDR,BLUR,DEFERRED, RENDERBUTTON_END
};

public: enum RENDER { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_UI_ACTIVE, RENDER_END };

private: explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CRenderer() = default;

public: ID3D11ShaderResourceView* Get_SRV(const wstring& pTargetTag);

public: void	SetRenderButton(RENDERBUTTON ebutton, _bool check);
public: void	SetCameraTag(const wstring& CameraTag) { lstrcpy(m_CameraTag,CameraTag.c_str());  }

public: virtual HRESULT NativeConstruct_Prototype();
public: virtual HRESULT NativeConstruct(void* pArg);


public: HRESULT Add_RenderGroup(RENDER eRenderID, class CGameObject* pGameObject);
public: HRESULT Draw_RenderGroup();

private: list<class CGameObject*>			m_RenderGroup[RENDER_END];
private: typedef list<class CGameObject*>	RENDERGROUP;

private: class CVIBuffer_RectViewPort*		m_pVIBuffer = nullptr;
private: CTarget_Manager*					m_pTargetMgr = nullptr;

private: _bool								m_bShadow = false;
private: _bool								m_bPBRHDR = false;
private: _bool								m_bBlur = false;
private: _bool								m_bDeferred = false;
private: RENDERBUTTON						m_eRenderButton;
private: _tchar								m_CameraTag[128];

private: HRESULT Render_Priority();
private: HRESULT Render_NonAlpha();
private: HRESULT Render_Alpha();
private: HRESULT Render_UI();
private: HRESULT Render_UI_Active();

private: HRESULT Render_Shadow();
private: HRESULT Render_ShadeShadow();
private: HRESULT Render_PBR();
private: HRESULT Render_LightAcc();
private: HRESULT Render_PostProcessing();
private: HRESULT Render_Blend();

private: HRESULT SetUp_RenderTarget();


public: static CRenderer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
public: virtual CComponent* Clone(void* pArg) override;
public: virtual void Free() override;
};

END