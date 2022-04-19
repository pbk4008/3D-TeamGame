#pragma once

#include "Actor.h"

BEGIN(Client)

class CMotionTrail : public CActor
{
private: explicit CMotionTrail(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
private: explicit CMotionTrail(const CMotionTrail& _rhs);
private: virtual ~CMotionTrail() = default;

public: virtual HRESULT NativeConstruct_Prototype() override;
public: virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
public: virtual _int	Tick(_double _dDeltaTime) override;
public: virtual _int	LateTick(_double _dDeltaTime) override;
public: virtual HRESULT	Render()			 override;
public: virtual HRESULT	Render_MotionTrail() override;

public: _float	Get_LifeTime() { return m_lifetime; }
public: void	Set_Owner(CActor* _pOwner) { m_pOwner = _pOwner; }
public: void	Set_BoneMat(_fmatrix* bone);
public: void	Set_Info(_fmatrix world, _vector position, _vector campostion);
public: void	Set_Model(CModel* pModel);

private: _matrix	m_bonematrix[256];
private: _matrix	m_worldamt = XMMatrixIdentity();
private: _matrix	m_viewmat = XMMatrixIdentity();
private: _matrix	m_projmat = XMMatrixIdentity();
private: _matrix	m_WVP = XMMatrixIdentity();

private: _vector	m_position = XMVectorZero();
private: _vector	m_camposition = XMVectorZero();

private: CActor*	m_pOwner = nullptr;
private: _float		m_lifetime = 0.f;

private: HRESULT	Ready_Component();

public: static CMotionTrail* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
public: virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
public: virtual void Free() override;
};
END	
