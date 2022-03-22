#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CBoxCollider final : public CCollider
{
public:
	typedef struct tagDesc
	{
		CCollider::DESC tColliderDesc;
		_float3 vScale = { 1.f, 1.f, 1.f };
	}DESC;
private:
	explicit CBoxCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBoxCollider(const CBoxCollider& _rhs);
	virtual ~CBoxCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual const _int Tick(const _double& _dDeltaTime) override;
	virtual const _int LateTick(const _double& _dDeltaTime) override;

	const DESC& getDesc() const;

private:
	DESC m_tDesc;

public:
	static CBoxCollider* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CComponent* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END