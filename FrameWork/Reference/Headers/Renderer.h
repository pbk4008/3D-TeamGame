#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public: enum RENDERBUTTON
{
	SHADOW,PBRHDR, RENDERBUTTON_END
};

public:
	enum RENDER { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
private:
	explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderer() = default;

public: void	SetRenderButton(RENDERBUTTON ebutton, _bool check);
public: void	SetCameraTag(const wstring& CameraTag) { m_CameraTag = CameraTag;  }

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

public: ID3D11ShaderResourceView* Get_SRV(const wstring& pTargetTag);

public:
	HRESULT Add_RenderGroup(RENDER eRenderID, class CGameObject* pGameObject);
	HRESULT Draw_RenderGroup();
private:
	list<class CGameObject*>			m_RenderGroup[RENDER_END];
	typedef list<class CGameObject*>	RENDERGROUP;

private:
	class CTarget_Manager* m_pTarget_Manager = nullptr;
	class CVIBuffer_RectViewPort* m_pVIBuffer = nullptr;

private: _bool								m_bShadow = false;
private: _bool								m_bPBRHDR = false;
private: RENDERBUTTON						m_eRenderButton;
private: wstring							m_CameraTag;

private: HRESULT Render_Priority();
private: HRESULT Render_NonAlpha();
private: HRESULT Render_Alpha();
private: HRESULT Render_UI();

private: HRESULT Render_Shadow();
private: HRESULT Render_ShadeShadow();
private: HRESULT Render_PBR();
private: HRESULT Render_LightAcc();
private: HRESULT Render_Blend();

public:
	static CRenderer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END