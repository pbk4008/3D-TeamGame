#include "PhysicsXSystem.h"

/* For.Collision */
#include "contactReportFilterShader.h"
#include "SimulationEventCallback.h"
#include "SimulationFilterCallback.h"

/* For.ControllerManager */
#include "ControllerBehaviorCallback.h"
#include "ControllerHitReport.h"
#include "ContactReportFilterShader.h"

#include "GameObject.h"
#include "Transform.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "NavMeshCollider.h"
#include "CharacterController.h"

CPhysicsXSystem::CPhysicsXSystem()
{
}

HRESULT CPhysicsXSystem::Init_PhysX()
{
	// Foundation
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallBack);
	if (!m_pFoundation)
		return E_FAIL;

	// Pvd + Physics
	m_pPvd = PxCreatePvd(*m_pFoundation);
	m_pPvdTransport = PxDefaultPvdSocketTransportCreate(PVD_HOST, PVD_PORT, PVD_DEFAULT_TIMEOUT);
	if (m_pPvd->connect(*m_pPvdTransport, PxPvdInstrumentationFlag::eALL))
		m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPvd);
	else
		m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true);
	if (!m_pPhysics)
		return E_FAIL;

	// Dispatcher
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	if (!m_pDispatcher)
		return E_FAIL;

	// Scene
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.f, -9.8f, 0.f);
	sceneDesc.cpuDispatcher = m_pDispatcher;

	sceneDesc.filterShader = contactReportFilterShader;
	m_pSimulationEventCallback = new CSimulationEventCallback();
	sceneDesc.simulationEventCallback = m_pSimulationEventCallback;
	m_pSimulationFilterCallback = new CSimulationFilterCallback();
	sceneDesc.filterCallback = m_pSimulationFilterCallback;

	sceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
	sceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKEEP;
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.flags |= PxSceneFlag::eEXCLUDE_KINEMATICS_FROM_ACTIVE_ACTORS;

	m_pScene = m_pPhysics->createScene(sceneDesc);
	if (!m_pScene)
		return E_FAIL;

#ifdef _DEBUG
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.f);
	//m_pScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.f); // ±âÁî¸ð
#endif // _DEBUG

	PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// DefaultMaterial
	m_pDefaultMaterial = m_pPhysics->createMaterial(0.5f, 0.5, 0.6f);

	// Cooking
	PxCookingParams cookingParams(m_pPhysics->getTolerancesScale());
	cookingParams.meshPreprocessParams = PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, cookingParams);
	if (!m_pCooking)
		return E_FAIL;

	// ControllerManager
	m_pControllerManager = PxCreateControllerManager(*m_pScene);
	m_pControllerBehaviorCallback = new CControllerBehaviorCallback();
	m_pControllerHitReport = new CControllerHitReport();

	return S_OK;
}

const _int CPhysicsXSystem::Tick(const _double& _dDeltaTime)
{
	if (!m_pScene)
		return -1;

	m_pScene->simulate((PxReal)_dDeltaTime);
	m_pScene->fetchResults(true);

	return _int();
}

PxMaterial* CPhysicsXSystem::Create_Material(const PxReal _staticFriction, const PxReal _dynamicFriction, const PxReal _restitution)
{
	PxMaterial* pMaterial = m_pPhysics->createMaterial(_staticFriction, _dynamicFriction, _restitution);
	if (!pMaterial)
		return nullptr;
	return pMaterial;
}

PxRigidActor* CPhysicsXSystem::Create_RigidActor(const ERigidType _eRigidType, const _bool _isGravity, const _bool _isKinematic, const _bool _isVisualization, const PxVec3& _pxvPosition)
{
	PxRigidActor* pRigidActor = nullptr;
	PxTransform pxTransform;
	pxTransform.p = _pxvPosition;
	pxTransform.q = PxQuat(0.f, 0.f, 0.f, 1.f);
	switch (_eRigidType)
	{
	case ERigidType::Static:
		pRigidActor = m_pPhysics->createRigidStatic(pxTransform);
		break;
	case ERigidType::Dynamic:
		pRigidActor = m_pPhysics->createRigidDynamic(pxTransform);
		static_cast<PxRigidBody*>(pRigidActor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, _isKinematic);
		break;
	}
	pRigidActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !_isGravity);
	pRigidActor->setActorFlag(PxActorFlag::eVISUALIZATION, _isVisualization);

	return pRigidActor;
}

HRESULT CPhysicsXSystem::Create_Box(CBoxCollider* _pCollider)
{
	if (!m_pPhysics)
		return E_FAIL;

	CBoxCollider::DESC tBoxColliderDesc = _pCollider->getDesc();
	CCollider::DESC tColliderDesc = tBoxColliderDesc.tColliderDesc;

	PxMaterial* pMaterial = Create_Material(tColliderDesc.fStaticFriction, tColliderDesc.fDynamicFriction, tColliderDesc.fRestitution);
	if (!pMaterial)
		return E_FAIL;

	PxRigidActor* pRigidActor = Create_RigidActor(tColliderDesc.eRigidType, tColliderDesc.isGravity, tColliderDesc.isKinematic, tColliderDesc.isVisualization);
	pRigidActor->userData = tColliderDesc.pGameObject;

	// Shape
	PxVec3 pxvHalfExtents = ToPxVec3(tBoxColliderDesc.vScale);
	pxvHalfExtents *= 0.5f;

	PxShape* pShape = m_pPhysics->createShape(PxBoxGeometry(pxvHalfExtents), *pMaterial, true);
	pShape->setFlag(PxShapeFlag::eVISUALIZATION, tColliderDesc.isVisualization);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, tColliderDesc.isSceneQuery);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !tColliderDesc.isTrigger);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, tColliderDesc.isTrigger);

	//PxFilterData filterData;
	//filterData.word0 = 1;
	//filterData.word1 = 1 << 1;
	//pShape->setSimulationFilterData(filterData);

	pRigidActor->attachShape(*pShape);
	m_pScene->addActor(*pRigidActor);

	_pCollider->setMaterial(pMaterial);
	_pCollider->setShape(pShape);
	_pCollider->setRigidActor(pRigidActor);
	
	return S_OK;
}

HRESULT CPhysicsXSystem::Create_Sphere(CSphereCollider* _pCollider)
{
	if (!m_pPhysics)
		return E_FAIL;

	CSphereCollider::DESC tSphereCollideDesc = _pCollider->getDesc();
	CCollider::DESC tColliderDesc = tSphereCollideDesc.tColliderDesc;

	PxMaterial* pMaterial = Create_Material(tColliderDesc.fStaticFriction, tColliderDesc.fDynamicFriction, tColliderDesc.fRestitution);
	if (!pMaterial)
		return E_FAIL;

	PxRigidActor* pRigidActor = Create_RigidActor(tColliderDesc.eRigidType, tColliderDesc.isGravity, tColliderDesc.isKinematic, tColliderDesc.isVisualization);
	pRigidActor->userData = tColliderDesc.pGameObject;

	// Shape
	PxShape* pShape = m_pPhysics->createShape(PxSphereGeometry(tSphereCollideDesc.fRadius), *pMaterial, true);
	pShape->setFlag(PxShapeFlag::eVISUALIZATION, tColliderDesc.isVisualization);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, tColliderDesc.isSceneQuery);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !tColliderDesc.isTrigger);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, tColliderDesc.isTrigger);

	//PxFilterData filterData;
	//filterData.word0 = 1;
	//filterData.word1 = 1 << 1;
	//pShape->setSimulationFilterData(filterData);

	pRigidActor->attachShape(*pShape);
	m_pScene->addActor(*pRigidActor);

	_pCollider->setMaterial(pMaterial);
	_pCollider->setShape(pShape);
	_pCollider->setRigidActor(pRigidActor);

	return S_OK;
}

HRESULT CPhysicsXSystem::Create_Capsule(CCapsuleCollider* _pCollider)
{
	if (!m_pPhysics)
		return E_FAIL;

	CCapsuleCollider::DESC tCapsuleCollideDesc = _pCollider->getDesc();
	CCollider::DESC tColliderDesc = tCapsuleCollideDesc.tColliderDesc;

	PxMaterial* pMaterial = Create_Material(tColliderDesc.fStaticFriction, tColliderDesc.fDynamicFriction, tColliderDesc.fRestitution);
	if (!pMaterial)
		return E_FAIL;

	PxRigidActor* pRigidActor = Create_RigidActor(tColliderDesc.eRigidType, tColliderDesc.isGravity, tColliderDesc.isKinematic, tColliderDesc.isVisualization);
	pRigidActor->userData = tColliderDesc.pGameObject;

	// Shape
	PxShape* pShape = m_pPhysics->createShape(PxCapsuleGeometry(tCapsuleCollideDesc.fRadius, tCapsuleCollideDesc.fHeight * 0.5f), *pMaterial, true);
	pShape->setFlag(PxShapeFlag::eVISUALIZATION, tColliderDesc.isVisualization);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, tColliderDesc.isSceneQuery);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !tColliderDesc.isTrigger);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, tColliderDesc.isTrigger);

	//PxFilterData filterData;
	//filterData.word0 = 1;
	//filterData.word1 = 1 << 1;
	//pShape->setSimulationFilterData(filterData);

	pRigidActor->attachShape(*pShape);
	m_pScene->addActor(*pRigidActor);

	_pCollider->setMaterial(pMaterial);
	_pCollider->setShape(pShape);
	_pCollider->setRigidActor(pRigidActor);

	return S_OK;
}

HRESULT CPhysicsXSystem::Create_Mesh(CMeshCollider* _pCollider)
{
	return S_OK;
}

HRESULT CPhysicsXSystem::Create_NavMesh(CNavMeshCollider* _pCollider)
{
	if (!m_pPhysics)
		return E_FAIL;

	CNavMeshCollider::DESC tNavMeshDesc = _pCollider->getDesc();
	CCollider::DESC tColliderDesc = tNavMeshDesc.tColliderDesc;

	PxMaterial* pMaterial = Create_Material(tColliderDesc.fStaticFriction, tColliderDesc.fDynamicFriction, tColliderDesc.fRestitution);
	if (!pMaterial)
		return E_FAIL;

	PxRigidActor* pRigidActor = Create_RigidActor(tColliderDesc.eRigidType, tColliderDesc.isGravity, tColliderDesc.isKinematic, tColliderDesc.isVisualization);
	pRigidActor->userData = tColliderDesc.pGameObject;

	// Shape
	_uint iSize = (_uint)tNavMeshDesc.vecPoints.size();
	_float3* pPoints = new _float3[iSize * 3];
	for (_uint i = 0; i < iSize; i++)
	{
		pPoints[i * 3] = tNavMeshDesc.vecPoints[i][0];
		pPoints[i * 3 + 1] = tNavMeshDesc.vecPoints[i][1];
		pPoints[i * 3 + 2] = tNavMeshDesc.vecPoints[i][2];
	}

	FACEINDICES32* Indices = new FACEINDICES32[iSize];
	for (_uint i = 0; i < iSize; i++)
	{
		Indices[i]._0 = i * 3;
		Indices[i]._1 = i * 3 + 1;
		Indices[i]._2 = i * 3 + 2;
	}


	PxTriangleMeshDesc meshDesc;
	meshDesc.points.data = pPoints;
	meshDesc.points.count = iSize * 3;
	meshDesc.points.stride = sizeof(_float3);
	meshDesc.triangles.count = iSize;
	meshDesc.triangles.data = Indices;
	meshDesc.triangles.stride = sizeof(FACEINDICES32);


	PxTriangleMesh* pTriMesh = m_pCooking->createTriangleMesh(meshDesc, m_pPhysics->getPhysicsInsertionCallback());
	if (!pTriMesh)
		return E_FAIL;

	//PxTriangleMeshGeometry pxMesh = PxTriangleMeshGeometry(pTriMesh);
	PxShape* pShape = m_pPhysics->createShape(PxTriangleMeshGeometry(pTriMesh), *pMaterial, true);
	pShape->setFlag(PxShapeFlag::eVISUALIZATION, tColliderDesc.isVisualization);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, tColliderDesc.isSceneQuery);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !tColliderDesc.isTrigger);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, tColliderDesc.isTrigger);

	pRigidActor->attachShape(*pShape);
	m_pScene->addActor(*pRigidActor);

	_pCollider->setMaterial(pMaterial);
	_pCollider->setShape(pShape);
	_pCollider->setRigidActor(pRigidActor);

	Safe_Delete_Array(pPoints);
	Safe_Delete_Array(Indices);

	return S_OK;
}

HRESULT CPhysicsXSystem::Create_CharacterController(CCharacterController* _pController)
{
	PxCapsuleControllerDesc controllerDesc;
	CCharacterController::DESC tCharacterControllerDesc = _pController->Get_CharacterControllerDesc();

	PxMaterial* pMaterial = Create_Material(tCharacterControllerDesc.fStaticFriction, tCharacterControllerDesc.fDynamicFriction, tCharacterControllerDesc.fRestitution);

	controllerDesc.behaviorCallback = m_pControllerBehaviorCallback;
	controllerDesc.userData = tCharacterControllerDesc.pGameObject;
	controllerDesc.material = pMaterial;
	controllerDesc.height = tCharacterControllerDesc.fHeight;
	controllerDesc.radius = tCharacterControllerDesc.fRadius;
	controllerDesc.climbingMode = tCharacterControllerDesc.eClimbingMode;
	controllerDesc.contactOffset = tCharacterControllerDesc.fContactOffset;
	controllerDesc.stepOffset = tCharacterControllerDesc.fStepOffset;
	controllerDesc.slopeLimit = XMConvertToRadians(tCharacterControllerDesc.fSlopeLimit);
	controllerDesc.upDirection = ToPxVec3(tCharacterControllerDesc.vUpDirection);
	controllerDesc.reportCallback = m_pControllerHitReport;

	_vector svPosition =_pController->m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	svPosition += XMLoadFloat3(&tCharacterControllerDesc.vPosition);
	_float3 vPosition; XMStoreFloat3(&vPosition, svPosition);
	controllerDesc.position = PxExtendedVec3(vPosition.x, vPosition.y, vPosition.z);

	PxController* pPxController = m_pControllerManager->createController(controllerDesc);
	pPxController->setUserData(tCharacterControllerDesc.pGameObject);

	vector<PxShape*> vecShapes;
	const PxU32 numShapes = pPxController->getActor()->getNbShapes();
	vecShapes.resize(numShapes);

	PxShape** ppShape = (PxShape**)m_Allocator.allocate(sizeof(PxShape*) * numShapes, 0, __FILE__, __LINE__);
	pPxController->getActor()->getShapes(ppShape, numShapes);
	pPxController->getActor()->userData = tCharacterControllerDesc.pGameObject;

	for (PxU32 i = 0; i < numShapes; ++i)
	{
		PxShape* pShape = ppShape[i];
		pShape->userData = tCharacterControllerDesc.pGameObject;
		vecShapes[i] = pShape;
	}

	if (ppShape)
	{
		m_Allocator.deallocate(ppShape);
		ppShape = NULL;
	}

	_pController->setPxController(pPxController);
	_pController->setShapes(vecShapes);
	_pController->setMaterial(pMaterial);

	//m_pScene->addActor(*pPxController->getActor());
	return S_OK;
}

const PxRenderBuffer& CPhysicsXSystem::Get_RenderBuffer()
{
	return m_pScene->getRenderBuffer();
}

void CPhysicsXSystem::Remove_Actor(PxActor* _pActor)
{
	if (_pActor)
		m_pScene->removeActor(*_pActor);
}

void CPhysicsXSystem::Add_Actor(PxActor* _pActor)
{
	m_pScene->addActor(*_pActor);
}

const _bool CPhysicsXSystem::Raycast(RAYCASTDESC & _desc)
{
	PxVec3 origin = ToPxVec3(_desc.vOrigin);
	PxVec3 unitDir = ToPxVec3(_desc.vDir);

	PxRaycastBuffer hit;
	if (m_pScene->raycast(origin, unitDir, _desc.fMaxDistance, hit, _desc.hitFlags, _desc.filterData))
	{
		if (hit.hasBlock)
		{
			PxRaycastHit hitInfo = hit.block;
			if (_desc.ppOutHitObject)
			{
				*_desc.ppOutHitObject = static_cast<CGameObject*>(hitInfo.actor->userData);
				_desc.vHitPos = FromPxVec3(hitInfo.position);
			}
			return true;
		}
	}
	return false;
}

void CPhysicsXSystem::Free()
{
	// release
	Safe_PxRelease(m_pControllerManager);
	Safe_PxRelease(m_pCooking);

	Safe_PxRelease(m_pDefaultMaterial);

	Safe_PxRelease(m_pScene);
	Safe_PxRelease(m_pDispatcher);
	Safe_PxRelease(m_pPhysics);
	if (m_pPvd)
		m_pPvd->disconnect();
	Safe_PxRelease(m_pPvd);
	Safe_PxRelease(m_pPvdTransport);
	Safe_PxRelease(m_pFoundation);

	// Delete
	Safe_Delete(m_pControllerHitReport);
	Safe_Delete(m_pControllerBehaviorCallback);
	Safe_Delete(m_pSimulationFilterCallback);
	Safe_Delete(m_pSimulationEventCallback);
}