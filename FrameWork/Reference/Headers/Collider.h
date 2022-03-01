#pragma once

#include "Component.h"
#include "DebugDraw.h"
#include "PhysicsXSystem.h"

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
protected:
	CCollider() = default;
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	void Update(_fmatrix TransformMatrix);
	virtual HRESULT Render(const wstring& pCameraTag);
public:
	void Collider();
protected:
	HRESULT Render_Begin(const wstring& pCameraTag);
	HRESULT Render_End();
protected:
	HRESULT Init_Collider(const CPhysicsXSystem::COLDESC& tDesc);
	_fvector getQuaternion(_fmatrix matTransform);
	_fvector ToXMVector3(const PxVec3 pxvec);
	_fvector ToXMVector4(const PxQuat pxquat);
	const PxVec3 ToPxVector(_fvector xmvec);
	const PxQuat ToQuat(_fvector xmvec);
	const _float3 QuaternionToEuler(const _float4& _q);
private:
	void Update_Scale(_fmatrix matTransform);
	void Update_Rotate(_fmatrix matTransform);
	void Update_Position(PxVec3 vPos);
protected:
	BasicEffect*											m_pEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>*		m_pBatch = nullptr;
	ID3D11InputLayout*								m_pInputLayout = nullptr;
	_float4												m_vColor;
protected:
	_bool			m_isCollision = false;
	PxRigidActor* m_pRigidBody = nullptr;
	PxShape* m_pShape = nullptr;
	CPhysicsXSystem::ACTORTYPE m_eType = CPhysicsXSystem::ACTORTYPE::ACTOR_END;
public:
	virtual void Free() override;
};

END