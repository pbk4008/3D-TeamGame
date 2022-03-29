#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCapsuleCollider final : public CCollider
{
public:
	typedef struct tagDesc
	{
		CCollider::DESC tColliderDesc;
		_float fHeight = 1.f;
		_float fRadius = 0.5f;
	}DESC;
private:
	explicit CCapsuleCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCapsuleCollider(const CCapsuleCollider& _rhs);
	virtual ~CCapsuleCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual const _int Tick(const _double& _dDeltaTime) override;
	virtual const _int LateTick(const _double& _dDeltaTime) override;

	const _float getHeight() const;
	const _float getRadius() const;

private:
	_float m_fHeight = 1.f;
	_float m_fRadius = 0.5f;

public:
	static CCapsuleCollider* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CComponent* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END