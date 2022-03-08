#pragma once
#ifndef __PHYSICSXSYSTEM__
#define __PHYSICSXSYSTEM__
#include "SingleTon.h"

BEGIN(Engine)
class ContactReportCallback;
class CPhysicsXSystem final : public CSingleTon<CPhysicsXSystem>
{
	friend CSingleTon;
public:
	enum class COLLIDERTYPE { COL_BOX, COL_CAP, COL_SPHERE, COL_MESH, COL_END };
	enum class ACTORTYPE {ACTOR_STATIC, ACTOR_DYNAMIC, ACTOR_END};
	typedef struct tagColDesc
	{
		ACTORTYPE eType;
		_float3 fPos;
		_bool bKinematic;
		_bool bGravity;
	}COLDESC;
private:
	NO_COPY(CPhysicsXSystem);
	explicit CPhysicsXSystem();
	virtual ~CPhysicsXSystem() = default;
public:
	HRESULT Init_PhysicsX();
	HRESULT Init_RigidActor(PxShape* pShape, const COLDESC& tDesc, PxRigidActor** ppRigidActor);
	HRESULT UpDate_Collision(_double DeltaTime);
	PxShape* Init_Shape(COLLIDERTYPE eType, const PxVec3 ShapeInfo);
	PxShape* Init_Mesh(const PxTriangleMeshDesc& tDesc);
private:
	HRESULT Intit_Scene();
private:
	virtual void Free() override;
private:
	PxDefaultAllocator m_Allocator;
	PxDefaultErrorCallback m_ErrorCallBack;
	PxFoundation* m_pFoundation;
	PxPhysics* m_pPhysics;
	PxScene* m_pScene;
	PxCooking* m_pCooking;
	PxDefaultCpuDispatcher* m_pDispatcher;

public:
	class ContactReportCallback : public PxSimulationEventCallback
	{
	public:
		virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {}
		virtual void onWake(PxActor** actors, PxU32 count) {}
		virtual void onSleep(PxActor** actors, PxU32 count) {}
		virtual void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) {}
		virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)override;
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count)override;
	};

private: ContactReportCallback	m_pContactRePort;
};
#endif
END