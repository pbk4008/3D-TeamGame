#pragma once

#include "Base.h"

BEGIN(Engine)
class CTarget_Manager;
class CVIBuffer_RectViewPort;

class CPostProcess final : public CBase
{
private: explicit CPostProcess(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CPostProcess() = default;

public: HRESULT				InitPostProcess();

public: HRESULT				PostProcessing(CTarget_Manager* pTargetMgr);
public: HRESULT				DownSampling(CTarget_Manager * pTargetMgr);
public: HRESULT				HorizontalBlur(CTarget_Manager* pTargetMgr);
public: HRESULT				VerticalBlur(CTarget_Manager* pTargetMgr);

private: CVIBuffer_RectViewPort*	m_pVIBuffer = nullptr;
private: ID3D11Device*				m_pDevice = nullptr;
private: ID3D11DeviceContext*		m_pDeviceContext = nullptr;

private: float						ComputeGaussianValue(_float x, _float mean, _float std_deviation);

private: _float						m_BrightThreshold = 0.8f; // 밝기 한계점 조절
private: _float						m_GaussMultipliter = 1.5f;
private: _float						m_GaussMean = 0.0f;
private: _float						m_GaussStdDev = 0.8f;

private: D3D11_VIEWPORT				m_viewport;

public: static CPostProcess*	Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
public: virtual void			Free() override;
};
END