#pragma once

#include "Base.h"

BEGIN(Engine)
class CTarget_Manager;
class CVIBuffer_RectViewPort;

class CTonemapping final : public CBase
{
private: explicit CTonemapping(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CTonemapping() = default;

public: void				Set_HDR(_bool check) { m_bHDR = check; }

public: HRESULT				InitToneMapping();

public: HRESULT				Blend_FinalPass(CTarget_Manager* pTargetMgr, _bool check);
public: HRESULT				ToneMapping(CTarget_Manager* pTargetMgr);

private: CVIBuffer_RectViewPort*	m_pTonmapBuffer = nullptr;
private: CVIBuffer_RectViewPort*	m_pFinalBlend = nullptr;

private: ID3D11Device*				m_pDevice = nullptr;
private: ID3D11DeviceContext*		m_pDeviceContext = nullptr;
private: _bool						m_bHDR = false;

private: D3D11_VIEWPORT				m_viewport;

public: static CTonemapping*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
public: virtual void			Free() override;
};
END