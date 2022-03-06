#include "PhysicsXSystem.h"
#include "PipeLine.h"
#include "SaveManager.h"
#include "GameObject.h"

CPhysicsXSystem::CPhysicsXSystem()
	: m_pFoundation(nullptr)
	, m_pPhysics(nullptr)
	, m_pScene(nullptr)
	, m_pDispatcher(nullptr)
	, m_pContactRePort(nullptr)
	, m_pCooking(nullptr)
{
}
HRESULT CPhysicsXSystem::Init_PhysicsX()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallBack);
	if (!m_pFoundation)
		return E_FAIL;

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true);
	if (!m_pPhysics)
		return E_FAIL;

	PxCookingParams cookingParams(m_pPhysics->getTolerancesScale());

	cookingParams.meshPreprocessParams = PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;

	PxCooking* pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, cookingParams);

	if (!pCooking)
		return E_FAIL;

	if (FAILED(Intit_Scene()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPhysicsXSystem::Init_RigidActor(PxShape* pShape , const COLDESC& tDesc, PxRigidActor** ppRigidActor)
{
	if (!m_pPhysics||!m_pScene)
		return E_FAIL;

	PxTransform transform = PxTransform(PxVec3(tDesc.fPos.x, tDesc.fPos.y, tDesc.fPos.z));

	if (tDesc.eType == ACTORTYPE::ACTOR_STATIC)
		*ppRigidActor = m_pPhysics->createRigidStatic(transform);
	else if (tDesc.eType == ACTORTYPE::ACTOR_DYNAMIC)
	{
		PxRigidDynamic* pActor = m_pPhysics->createRigidDynamic(transform);
		pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, tDesc.bKinematic);
		*ppRigidActor = pActor;
	}
	(*ppRigidActor)->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, tDesc.bGravity);
	(*ppRigidActor)->attachShape(*pShape);
	m_pScene->addActor(**ppRigidActor);

	return S_OK;
}

HRESULT CPhysicsXSystem::UpDate_Collision(_double DeltaTime)
{
	if (!m_pScene)
		return E_FAIL;
	m_pScene->simulate((_float)DeltaTime);
	m_pScene->fetchResults(true);
	return S_OK;
}


PxShape* CPhysicsXSystem::Init_Shape(COLLIDERTYPE eType, const PxVec3 ShapeInfo)
{
	if (!m_pPhysics)
		return nullptr;

	PxMaterial* pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	if (!pMaterial)
		return nullptr;

	PxShape* pShape = nullptr;
	switch (eType)
	{
	case Engine::CPhysicsXSystem::COLLIDERTYPE::COL_BOX:
		pShape = m_pPhysics->createShape(PxBoxGeometry(ShapeInfo), *pMaterial, true);
		break;
	case Engine::CPhysicsXSystem::COLLIDERTYPE::COL_CAP:
		pShape = m_pPhysics->createShape(PxCapsuleGeometry(ShapeInfo.x, ShapeInfo.y), *pMaterial, true);
		break;
	case Engine::CPhysicsXSystem::COLLIDERTYPE::COL_SPHERE:
		pShape = m_pPhysics->createShape(PxSphereGeometry(ShapeInfo.x), *pMaterial, true);
		break;
	case Engine::CPhysicsXSystem::COLLIDERTYPE::COL_END:
		break;
	default:
		break;
	}
	pShape->setFlag(PxShapeFlag::eVISUALIZATION, true);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

	return pShape;
}

PxShape* CPhysicsXSystem::Init_Mesh(const PxTriangleMeshDesc& tDesc)
{
	if (!m_pCooking||!m_pPhysics)
		return nullptr;
	PxTriangleMesh* pTriMesh=m_pCooking->createTriangleMesh(tDesc, m_pPhysics->getPhysicsInsertionCallback());
	if (!pTriMesh)
		return nullptr;

	PxMaterial* pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	if (!pMaterial)
		return nullptr;

	PxTriangleMeshGeometry pxMesh = PxTriangleMeshGeometry(pTriMesh);

	PxShape* pShape = nullptr;
	pShape = m_pPhysics->createShape(pxMesh, *pMaterial,true);
	pShape->setFlag(PxShapeFlag::eVISUALIZATION, true);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

	return pShape;
}

HRESULT CPhysicsXSystem::Intit_Scene()
{
	if (!m_pPhysics)
		return E_FAIL;
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.f, -9.8f, 0.f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	m_pContactRePort = new ContactReportCallback();
	sceneDesc.simulationEventCallback = m_pContactRePort;

	m_pScene = m_pPhysics->createScene(sceneDesc);

	if (!m_pScene)
		return E_FAIL;

	return S_OK;
}


void CPhysicsXSystem::Free()
{
	Safe_Delete(m_pContactRePort);
	PX_RELEASE(m_pScene);
	PX_RELEASE(m_pDispatcher);
	PX_RELEASE(m_pPhysics);
	PX_RELEASE(m_pFoundation);
}

void ContactReportCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (_uint i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];
		
		CGameObject* pObj0 = static_cast<CGameObject*>(pairHeader.actors[0]->userData);
		CGameObject* pObj1 = static_cast<CGameObject*>(pairHeader.actors[1]->userData);
		
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pObj0->OnCollisionEnter(pObj1);
			pObj1->OnCollisionEnter(pObj0);
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			pObj0->OnCollisionStay(pObj1);
			pObj1->OnCollisionStay(pObj0);
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pObj0->OnCollisionExit(pObj1);
			pObj1->OnCollisionExit(pObj0);
		}
	}
}

void ContactReportCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (_uint i = 0; i < count; i++)
	{
		const PxTriggerPair& cp = pairs[i];

		CGameObject* pObj0 = static_cast<CGameObject*>(pairs->triggerActor[0].userData);
		CGameObject* pObj1 = static_cast<CGameObject*>(pairs->otherActor[0].userData);

		if (cp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pObj0->OnTriggerEnter(pObj1);
			pObj1->OnTriggerEnter(pObj0);
		}
		if (cp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pObj0->OnTriggerExit(pObj1);
			pObj1->OnTriggerExit(pObj0);
		}
	}
}
