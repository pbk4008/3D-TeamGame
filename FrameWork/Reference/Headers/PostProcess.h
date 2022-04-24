#pragma once

#include "Base.h"

BEGIN(Engine)
class CTarget_Manager;
class CVIBuffer_RectViewPort;
class CTonemapping;

class CPostProcess final : public CBase
{
private: explicit CPostProcess(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CPostProcess() = default;

public: HRESULT				InitPostProcess();

public: HRESULT				AlphaBlur(CTarget_Manager* pTargetMgr, _bool alpha);
public: HRESULT				Shadowblur(CTarget_Manager* pTargetMgr, _bool shadow, _float weight);
public: HRESULT				PossProcessing(CTonemapping* tone,CTarget_Manager* pTargetMgr,_bool hdr);

public: HRESULT				ComputeBrightPass(CTarget_Manager* pTargetMgr, const wstring& in, _float sizex, _float sizey);
public: HRESULT				BloomPass(CTarget_Manager* pTargetMgr, const wstring& target, const wstring& base1, const wstring& base2, const wstring& base3, const wstring& base4, _float weight);
public: HRESULT				BloomPass(CTarget_Manager* pTargetMgr, const wstring& target, const wstring& base, const wstring& base1, const wstring& base2 , _float weight, _bool check);
public: HRESULT				BloomPass(CTarget_Manager* pTargetMgr, const wstring& target, const wstring& base, const wstring& base1);

public: HRESULT				BlurPass(CTarget_Manager* pTargetMgr, const wstring& in, const wstring& middle, const wstring& out, _float sizex, _float sizey);
public: HRESULT				GaussianBlurVertical(CTarget_Manager* pTargetMgr, const wstring& in, const wstring& bind, _float sizex, _float sizey);
public: HRESULT				GaussianblurHorizontal(CTarget_Manager* pTargetMgr, const wstring& in, const wstring& bind, _float sizex, _float sizey);

public: HRESULT				GodRayPass(CTarget_Manager* pTargetMgr, const wstring& cmatag);

private: CVIBuffer_RectViewPort*	m_pVIBuffer = nullptr;
private: CVIBuffer_RectViewPort*	m_pGRBuffer = nullptr;
private: ID3D11Device*				m_pDevice = nullptr;
private: ID3D11DeviceContext*		m_pDeviceContext = nullptr;

private: _float						m_BrightThreshold = 0.7f; // 밝기 한계점 조절
private: _float						m_GaussMultipliter = 1.5f;
private: _float						m_GaussMean = 0.0f;
private: _float						m_GaussStdDev = 0.8f;

private: D3D11_VIEWPORT				m_viewport;

public: static CPostProcess*	Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
public: virtual void			Free() override;
};
END