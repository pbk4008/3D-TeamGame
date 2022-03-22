#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CNavMeshCollider final : public CCollider
{
public:
	typedef struct tagDesc
	{
		CCollider::DESC tColliderDesc;
		vector<_float3*> vecPoints;
	}DESC;
private:
	explicit CNavMeshCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CNavMeshCollider(const CNavMeshCollider& _rhs);
	virtual ~CNavMeshCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual const _int Tick(const _double& _dDeltaTime) override;
	virtual const _int LateTick(const _double& _dDeltaTime) override;

	const DESC& getDesc() const;

private:
	DESC m_tDesc;

public:
	static CNavMeshCollider* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CComponent* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END