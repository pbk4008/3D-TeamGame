#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager;
class CVIBuffer_RectViewPort;

class CRendererAssit final : public CBase
{
private: explicit CRendererAssit(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CRendererAssit() = default;

public: HRESULT			InitRenderAssit();
public: HRESULT			Setup_RenderTarget();

public: HRESULT			Render_LightAcc(const wstring& cameraTag,_bool bPBR, _bool shadow);

private: CVIBuffer_RectViewPort*	m_pVIBuffer = nullptr;
private: CTarget_Manager*			m_pTargetMgr = nullptr;
private: ID3D11Device*				m_pDevice = nullptr;
private: ID3D11DeviceContext*		m_pDeviceContext = nullptr;

public: static CRendererAssit*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
public: virtual void			Free() override;
};
END	