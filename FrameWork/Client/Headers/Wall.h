#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CWall final : public CGameObject
{
private: explicit CWall(ID3D11Device* pdevice, ID3D11DeviceContext* pdevicecontext);
private: explicit CWall(const CWall& rhs);
private: virtual ~CWall() = default;

public: virtual HRESULT		NativeConstruct_Prototype() override;
public: virtual HRESULT		NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
public: virtual _int		Tick(_double _dDeltaTime) override;
public: virtual _int		LateTick(_double _dDeltaTime) override;
public: virtual HRESULT		Render() override;

private: HRESULT Ready_Component();
private: HRESULT DissolveOn(_float dissolveSpeed = 0.5f);

private: CVIBuffer_Rect*	m_pbuffer = nullptr;
private: CTexture*			m_pdiffusetex = nullptr;
private: _float				m_delta = 0.f;
private: WALLDESC			m_desc;
	   
private: _bool				m_bdissolve = false;
private: _float				m_lifetime = 0.f;
private: CTexture*			m_dissolveTex = nullptr;

public: static CWall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContex);
public: virtual CGameObject* Clone(_uint iSeneid, void* pArg) override;
public: virtual void Free() override;
};
END

