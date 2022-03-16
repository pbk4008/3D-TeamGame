#pragma once

#include "Base.h"

BEGIN(Engine)
class CTarget_Manager;
class CVIBuffer_RectViewPort;

class CHDR final : public CBase
{
private: explicit CHDR(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CHDR() = default;

public: HRESULT				InitHDR();

public: HRESULT				Render_HDRBase(CTarget_Manager * pTargetMgr, _bool isShadow);

private: CVIBuffer_RectViewPort* m_pVIBuffer = nullptr;
private: ID3D11Device* m_pDevice = nullptr;
private: ID3D11DeviceContext* m_pDeviceContext = nullptr;

public: static CHDR*		Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
public: virtual void		Free() override;
};
END