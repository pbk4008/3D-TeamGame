#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef struct tagColliderDesc
	{
		_float3		vScale;
		_float3		vRotation;
		_float3		vPosition;
	} COLLIDERDESC;

	typedef struct tagOBB
	{
		_float3		vCenter;
		_float3		vAlignAxis[3];
		_float3		vCenterAxis[3];
	}OBB;

public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	_fvector Get_Min(BoundingBox* pBoundBox);
	_fvector Get_Max(BoundingBox* pBoundBox);

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eColliderType);
	virtual HRESULT NativeConstruct(void* pArg);
public:
	void Update(_fmatrix TransformMatrix);
	HRESULT Render();

public:
	_bool Collision_AABB(CCollider* pTargetCollider);
	_bool Collision_OBB(CCollider* pTargetCollider);
	_bool Collision_Sphere(CCollider* pTargetCollider);
private:
	BoundingBox*				m_pOriginAABB = nullptr;
	BoundingBox*				m_pAABB = nullptr;

	BoundingOrientedBox*		m_pOriginOBB = nullptr;
	BoundingOrientedBox*		m_pOBB = nullptr;

	BoundingSphere*				m_pOriginSphere = nullptr;
	BoundingSphere*				m_pSphere = nullptr;
	
private:
	BasicEffect*								m_pEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>*		m_pBatch = nullptr;
	ID3D11InputLayout*							m_pInputLayout = nullptr;
	_float4										m_vColor;

	class CTransform*			m_pTransform = nullptr;
	_float4x4					m_ParentMatrix;




private:
	TYPE			m_eColliderType = TYPE_END;
	_bool			m_isCollision = false;

private:
	_fmatrix Remove_Rotation(_fmatrix TransformMatrix);
	_fmatrix Remove_Scale(_fmatrix TransformMatrix);
	OBB Compute_OBBDesc();

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eColliderType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END