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
public: virtual HRESULT	Render_MotionTrail() override;

public: _float	Get_LifeTime() { return m_lifetime; }
public: void	Set_BoneMat(_fmatrix* bone);
public: void	Set_Info(_fmatrix world, _fmatrix weapon = XMMatrixIdentity(), _fmatrix shield = XMMatrixIdentity(), _float UVdvid = 0.f,CModel* pCurWeapon = nullptr);
public: void	Set_Model(CModel* pModel = nullptr, CModel* pWeapon = nullptr, CModel* pShield = nullptr);
public: void	Set_RunCheck(_bool check) { m_runcheck = check; }
public: void	Set_ThrowCheck(_bool check) { m_throwchck = check; }

private: HRESULT	Set_ContantBuffer(CModel* pmodel, _fmatrix worldmat,RIM& rimdesc);

private: _matrix	m_bonematrix[256];
private: _matrix	m_worldamt = XMMatrixIdentity();
private: _matrix	m_weaponworldmat = XMMatrixIdentity();
private: _matrix	m_shieldworldmat = XMMatrixIdentity();
private: _matrix	m_WVP = XMMatrixIdentity();

private: _float		m_lifetime = 1.f;
private: _float		m_UVdvid = 0.f;

private: CModel*	m_pWeapon = nullptr;
private: CModel*	m_pShield = nullptr;
private: CTexture*	m_pGradientTex = nullptr;

private: _bool		m_runcheck = false;
private: _bool		m_throwchck = false;

public: static CMotionTrail* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
public: virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
public: virtual void Free() override;
};
END	
