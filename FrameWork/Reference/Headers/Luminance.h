#pragma once

#include "Base.h"

BEGIN(Engine)
class CTarget_Manager;
class CVIBuffer_RectViewPort;

class CLuminance final : public CBase
{
private: explicit CLuminance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CLuminance() = default;

public: HRESULT				InitLuminance();

public: HRESULT				DownSampling(CTarget_Manager* pTargetMgr);

#ifdef _DEBUG
public: HRESULT				DownSamplingDebug(CTarget_Manager* pTargetMgr);
#endif // _DEBUG

private: CVIBuffer_RectViewPort* m_pVIBuffer = nullptr;
private: ID3D11Device* m_pDevice = nullptr;
private: ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:D3D11_VIEWPORT		m_viewport;

public: static CLuminance*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
public: virtual void		Free() override;
};
END	

