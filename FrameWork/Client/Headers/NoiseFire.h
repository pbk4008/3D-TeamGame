#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_RectInstance;
class CTexture;
END	

BEGIN(Client)
class CNoiseFire final : public CGameObject
{
private: explicit CNoiseFire(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: explicit CNoiseFire(CGameObject& rhs);
private: virtual ~CNoiseFire() = default;

public: virtual HRESULT NativeConstruct_Prototype() override;
public: virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
public: virtual _int	Tick(_double deltatime) override;
public: virtual _int	LateTick(_double deltatime) override;
public: virtual HRESULT	Render() override;

private: HRESULT	BindConstBuffer();
private: void		UpdateBillboard();
private: HRESULT	ReadyComponent();

private: std::vector<CTexture*>		m_vectexture;
private: CVIBuffer_RectInstance*	m_pbuffer = nullptr;
private: _float						m_deltatime = 0.f;

public: static CNoiseFire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
public: virtual CNoiseFire* Clone(const _uint iSceneID, void* pArg) override;
public: virtual void Free() override;
};

END	