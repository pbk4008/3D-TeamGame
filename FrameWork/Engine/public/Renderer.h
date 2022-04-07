#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

class CTarget_Manager;
class CVIBuffer_RectViewPort;
class CRendererAssit;
class CLuminance;
class CHDR;
class CPostProcess;
class CTonemapping;

class ENGINE_DLL CRenderer final : public CComponent
{
public: enum RENDERBUTTON
{
	SHADOW,PBRHDR,PIXEL,HDR,DBG,PARTICLE,OUTLINE,RADIAL, RENDERBUTTON_END
};

public: enum RENDER { RENDER_PRIORITY, RENDER_SKYBOX,RENDER_SHADOW, RENDER_PBR, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_UI_ACTIVE, RENDER_END };

private: explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CRenderer() = default;

public: _bool	Get_Shadow() { return m_bShadow; }
public: _bool	Get_PBR() { return m_bPBR; }

public: void	SetRenderButton(RENDERBUTTON ebutton, _bool check);
public: void	SetCameraTag(const wstring& CameraTag) { lstrcpy(m_CameraTag,CameraTag.c_str());  }
public: void	SetRadialCnt(_int RadialCnt) { m_RadialCnt = RadialCnt; }

public: virtual HRESULT NativeConstruct_Prototype();
public: virtual HRESULT NativeConstruct(void* pArg);

public: HRESULT	CreateShadowDepthStencilview(_uint iWidth, _uint iHeight, ID3D11DepthStencilView** ppDepthStencilView);
public: HRESULT Add_RenderGroup(RENDER eRenderID, class CGameObject* pGameObject);
public: HRESULT Draw_RenderGroup();
public: HRESULT Remove_RenderGroup();

private: list<class CGameObject*>			m_RenderGroup[RENDER_END];
private: typedef list<class CGameObject*>	RENDERGROUP;

private: CVIBuffer_RectViewPort*			m_pVIBuffer = nullptr;
private: CTarget_Manager*					m_pTargetMgr = nullptr;

private: ID3D11DepthStencilView*			m_pShadowMap = nullptr;

private: _bool								m_bShadow = false;
private: _bool								m_bPBR = false;
private: _bool								m_bPixel = false;
private: _bool								m_bHDR = false;
private: _bool								m_bDBG = false;
private: _bool								m_bParticle = false;
private: _bool								m_boutline = false;
private: _bool								m_bradial = false;
private: RENDERBUTTON						m_eRenderButton;
private: _tchar								m_CameraTag[128];
private: _int								m_RadialCnt = 6;

private: CRendererAssit*					m_pRenderAssit = nullptr;
private: CLuminance*						m_pLuminance = nullptr;
private: CHDR*								m_pHDR = nullptr;
private: CPostProcess*						m_pPostProcess = nullptr;
private: CTonemapping*						m_pTonemapping = nullptr;

private: HRESULT Render_Priority();
private: HRESULT Render_SkyBox();
private: HRESULT Render_NonAlpha();
private: HRESULT Render_Alpha();
private: HRESULT Render_UI();
private: HRESULT Render_UI_Active();

private: HRESULT Render_Shadow();
private: HRESULT ShadowPass();
private: HRESULT Render_Final(_bool outline,_bool radial);

private: /* For.PhysX */
	_bool m_isPhysXRender = false;
	BasicEffect* m_pEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	HRESULT Render_PhysX();

public: static CRenderer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
public: virtual CComponent* Clone(void* pArg) override;
public: virtual void Free() override;
};

END