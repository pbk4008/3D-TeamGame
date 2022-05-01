#pragma once
#ifndef __POT_H__
#define __POT_H__

#include "LandScape.h"
BEGIN(Engine)
class CModel;
class CCapsuleCollider;
END
BEGIN(Client)
class CPot final : public CLandScape
{
	enum class POTTYPE {POT_1, POT_2, POT_3, POT_END};
public:
	typedef struct tagPot
	{
		_uint iType;
		_float4x4 matWorld;
	}POTDESC;
public:
	explicit CPot();
	explicit CPot(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CPot(const CPot& rhs);
	virtual ~CPot() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	void OnTriggerEnter(CCollision& collision);
private:
	HRESULT Ready_Component();

public:
	static CPot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg) override;
private:
	virtual void Free() override;
private:
	CModel* m_pStaticModel;
	CModel* m_pAnimModel;
	CCapsuleCollider* m_pCollider;
	_uint m_iPotType;
	_float	m_ftimer = 0.f;
private:
	_bool m_bBroken;
};
END
#endif