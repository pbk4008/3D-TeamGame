#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

class ENGINE_DLL CRay_Collider final : public CComponent
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
	CRay_Collider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CRay_Collider(const CRay_Collider& rhs);
	virtual ~CRay_Collider() = default;

public:
	_fvector Get_Min(BoundingBox* pBoundBox);
	_fvector Get_Max(BoundingBox* pBoundBox);

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eColliderType);
	virtual HRESULT NativeConstruct(void* pArg);
public:
	void Update(_fmatrix TransformMatrix);
	HRESULT Render(const wstring& pCameraTag);

public:
	_bool Collision_AABB(CRay_Collider* pTargetCollider);
	_bool Collision_OBB(CRay_Collider* pTargetCollider);
	_bool Collision_Sphere(CRay_Collider* pTargetCollider);
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
	static CRay_Collider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eColliderType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END