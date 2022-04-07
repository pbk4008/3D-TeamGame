#pragma once
#ifndef __PHYSICSXSYSTEM__
#define __PHYSICSXSYSTEM__
#include "SingleTon.h"

#define PVD_HOST "127.0.0.1"
#define PVD_PORT 5425
#define PVD_DEFAULT_TIMEOUT 10//ms

#define ToPxVec3(v)					PxVec3((v).x, (v).y, (v).z)
#define FromPxVec3(v)				_float3((v).x, (v).y, (v).z)
#define ToPxExtendedVec3(v)			PxExtendedVec3((v).x, (v).y, (v).z)
#define FromPxextendedVec3(v)		_float3((_float)(v).x, (_float)(v).y, (_float)(v).z)
#define ToPxQuat(v)					PxQuat((v).x, (v).y, (v).z, (v).w)
#define FromPxQuat(v)				_float4((v).x, (v).y, (v).z, (v).w)

enum class EShape { Box, Capsule, Sphere, Mesh, Max };
enum class ERigidType { Static, Dynamic, Max };

BEGIN(Engine)

class CSimulationEventCallback;
class CSimulationFilterCallback;
class CControllerBehaviorCallback;
class CControllerHitReport;

class CCollider;
class CBoxCollider;
class CSphereCollider;
class CCapsuleCollider;
class CMeshCollider;
class CNavMeshCollider;
class CCharacterController;

class CGameObject;

typedef struct tagRaycastDesc
{
	// in
	_float3 vOrigin = { 0.f, 0.f, 0.f };
	_float3 vDir = { 0.f, 0.f, 0.f };
	_float fMaxDistance = 0.f;
	PxHitFlags hitFlags = PxHitFlag::eDEFAULT;
	PxQueryFilterData filterData;
	// out
	_float3 vHitPos = { 0.f, 0.f, 0.f };
	CGameObject** ppOutHitObject = nullptr;
	vector<CGameObject*> vecHitObjects;
}RAYCASTDESC;
typedef struct tagSweepDesc
{
	// in
	PxGeometryHolder geometry;
	_float3 vOrigin = { 0.f, 0.f, 0.f };
	_float4 vQuat = { 0.f, 0.f, 0.f, 1.f };
	_float3 vDir = { 0.f, 0.f, 0.f };
	_float fMaxDistance = 0.f;
	PxHitFlags hitFlags = PxHitFlag::eDEFAULT;
	PxQueryFilterData filterData;
	// out
	_float3 vHitPos = { 0.f, 0.f, 0.f };
	CGameObject** ppOutHitObject = nullptr;
	vector<CGameObject*> vecHitObjects;
}SWEEPDESC;
typedef struct tagOverlapDesc
{
	// in
	PxGeometryHolder geometry;
	_float3 vOrigin = { 0.f, 0.f, 0.f };
	_float4 vQuat = { 0.f, 0.f, 0.f, 1.f };
	PxQueryFilterData filterData;
	// out
	CGameObject** ppOutHitObject = nullptr;
	vector<CGameObject*> vecHitObjects;
}OVERLAPDESC;

class CPhysicsXSystem final : public CSingleTon<CPhysicsXSystem>
{
	friend CSingleTon;
private:
	NO_COPY(CPhysicsXSystem);
	explicit CPhysicsXSystem();
	virtual ~CPhysicsXSystem() = default;

public:
	HRESULT Init_PhysX();
	const _int Tick(const _double& _dDeltaTime);

private:
	PxMaterial* Create_Material(const PxReal _staticFriction, const PxReal _dynamicFriction, const PxReal _restitution);
	PxRigidActor* Create_RigidActor(const ERigidType _eRigidType, const _bool isGravity, const _bool _isKinematic, const _bool _isVisualization = true
		, const _float _fMass = 1.f, const PxVec3& _pxvPosition = { 0.f, 0.f, 0.f });
	PxRigidActor* Create_RigidActor(CCollider* _pCollider);

public:
	HRESULT Create_Box(CBoxCollider* _pCollider);
	HRESULT Create_Sphere(CSphereCollider* _pCollider);
	HRESULT Create_Capsule(CCapsuleCollider* _pCollider);
	HRESULT Create_Mesh(CMeshCollider* _pCollider);
	HRESULT Create_NavMesh(CNavMeshCollider* _pCollider);
	HRESULT Create_CharacterController(CCharacterController* _pController);
	const PxRenderBuffer& Get_RenderBuffer();

	void Remove_Actor(PxActor* _pActor);
	void Add_Actor(PxActor* _pActor);

public:
	const _bool Raycast(RAYCASTDESC& _desc);
	const _bool Sweep(SWEEPDESC& _desc);
	const _bool Overlap(OVERLAPDESC& _desc);

private:
	PxDefaultAllocator m_Allocator;
	PxDefaultErrorCallback m_ErrorCallBack;

	PxFoundation* m_pFoundation = nullptr;
	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
	PxPhysics* m_pPhysics = nullptr;
	PxScene* m_pScene = nullptr;
	PxCooking* m_pCooking = nullptr;

	PxMaterial* m_pDefaultMaterial = nullptr;

private: /* For.Pvd */
	PxPvdTransport* m_pPvdTransport = nullptr;
	PxPvd* m_pPvd = nullptr;

private: /* For.Collision */

	CSimulationEventCallback* m_pSimulationEventCallback = nullptr;
	CSimulationFilterCallback* m_pSimulationFilterCallback = nullptr;

private: /* For.ControllerManager */
	PxControllerManager* m_pControllerManager = nullptr;
	CControllerBehaviorCallback* m_pControllerBehaviorCallback = nullptr;
	CControllerHitReport* m_pControllerHitReport = nullptr;

public:
	virtual void Free() override;
};

END

#endif