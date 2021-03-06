#pragma once
#ifndef __BULLET_H__
#define __BULLET_H__

BEGIN(Engine)
class CModel;
class CSphereCollider;
END

#include "MeshEffect.h"

BEGIN(Client)

class CBullet final : public CMeshEffect
{
private:
	explicit CBullet(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBullet(const CBullet& _rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
private:
	HRESULT Ready_Component(const _uint iSceneID);
	HRESULT Set_Spawn();

public:
	CActor* Get_Owner() const;
	void Set_Owner(CActor* _pOwner);

private:
	virtual void OnTriggerEnter(CCollision& collision);
private:
	_uint Move(_double dDeltaTime);
public:
	static CBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg = nullptr) override;
private:
	virtual void Free()override;
private:
	CSphereCollider* m_pCollider = nullptr;
	CActor* m_pOwner = nullptr;
private:
	_float4x4 m_matBulletPosMatrix;
	_float m_fSpawnTime;
	_float4 m_fDir;
	_float m_fSpeed;
	_float m_fDamage = 0.f;
};
END
#endif
