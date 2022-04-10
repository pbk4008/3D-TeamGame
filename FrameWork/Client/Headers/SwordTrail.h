#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTexture;
class CTrail_VIBuffer;
END

BEGIN(Client)

class CSwordTrail final : public CGameObject
{
private: explicit CSwordTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: explicit CSwordTrail(const CGameObject& rhs);
private: virtual ~CSwordTrail() = default;


public: virtual HRESULT NativeConstruct_Prototype() override;
public: virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
public:  _int Tick(_double TimeDelta);
public:  _int LateTick(_double TimeDelta);
public:  HRESULT Render();

public: void			AddVertex(const _fvector pToppos, const _vector pBottompos);
public: void			Clear_Vertex();

//private: _float				m_Frametime = 0.f;
private: CTrail_VIBuffer*	m_pViBufferCom = nullptr;
private: CTexture*			m_pTexture = nullptr;
private: CTexture*			m_pTexture1 = nullptr;
private: CTexture*			m_pTexture2 = nullptr;

private: list < pair<_fvector, _fvector>>	m_TrailList;
private: _bool								m_bOnOff = false;


private: virtual HRESULT SetUp_Components();

public: static CSwordTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
public: virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
public: virtual void Free() override;
};

END
