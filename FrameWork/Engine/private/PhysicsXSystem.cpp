#include "PhysicsXSystem.h"
#include "PipeLine.h"
#include "SaveManager.h"
#include "GameObject.h"

#include "CharacterController.h"
#include "Transform.h"

CPhysicsXSystem::CPhysicsXSystem()
	: m_pFoundation(nullptr)
	, m_pPhysics(nullptr)
	, m_pScene(nullptr)
	, m_pDispatcher(nullptr)
	, m_pCooking(nullptr)
{
}
PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	pairFlags = PxPairFlag::eSOLVE_CONTACT
		| PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| PxPairFlag::eNOTIFY_TOUCH_LOST
		| PxPairFlag::eNOTIFY_CONTACT_POINTS;

	return PxFilterFlag::eDEFAULT;
}

HRESULT CPhysicsXSystem::Init_PhysicsX()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallBack);
	if (!m_pFoundation)
		return E_FAIL;

	//m_pPvd = PxCreatePvd(*m_pFoundation);
	//m_pPvdTransport = PxDefaultPvdSocketTransportCreate(PVD_HOST, PVD_PORT, PVD_DEFAULT_TIMEOUT);
	//if (!m_pPvd->connect(*m_pPvdTransport, PxPvdInstrumentationFlag::eALL))
	//	return E_FAIL;

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true/*, m_pPvd*/);
	if (!m_pPhysics)
		return E_FAIL;

	PxCookingParams cookingParams(m_pPhysics->getTolerancesScale());

	cookingParams.meshPreprocessParams = PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, cookingParams);

	if (!m_pCooking)
		return E_FAIL;

	if (FAILED(Intit_Scene()))
		return E_FAIL;

	if (FAILED(Init_ControllerManager())) return E_FAIL;


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

HRESULT CPhysicsXSystem::Create_Material(const PxReal _staticFriction, const PxReal _dynamicFriction, const PxReal _restitution, PxMaterial** _ppOutMaterial)
{
	PxMaterial* pMaterial = m_pPhysics->createMaterial(_staticFriction, _dynamicFriction, _restitution);

	if (_ppOutMaterial)
	{
		*_ppOutMaterial = pMaterial;
		return S_OK;
	}
	else
	{
		pMaterial->release();
		return E_FAIL;
	}

}

HRESULT CPhysicsXSystem::Create_CharacterController(CCharacterController* _pController, PxController** _ppOutPxController, vector<PxShape*>& _vecShapes)
{
	PxCapsuleControllerDesc controllerDesc;
	CCharacterController::CHARACTERCONTROLLERDESC tCharacterControllerDesc = _pController->Get_CharacterControllerDesc();

	controllerDesc.behaviorCallback = m_pControllerBehaviorCallback;
	controllerDesc.userData = tCharacterControllerDesc.pGameObject;
	controllerDesc.material = _pController->Get_Material();
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

	const PxU32 numShapes = pPxController->getActor()->getNbShapes();
	_vecShapes.resize(numShapes);

	PxShape** ppShape = (PxShape**)m_Allocator.allocate(sizeof(PxShape*) * numShapes, 0, __FILE__, __LINE__);
	pPxController->getActor()->getShapes(ppShape, numShapes);
	pPxController->getActor()->userData = tCharacterControllerDesc.pGameObject;

	for (PxU32 i = 0; i < numShapes; ++i)
	{
		PxShape* pShape = ppShape[i];
		pShape->userData = tCharacterControllerDesc.pGameObject;
		_vecShapes[i] = pShape;
	}

	if (ppShape)
	{
		m_Allocator.deallocate(ppShape);
		ppShape = NULL;
	}

	*_ppOutPxController = pPxController;

	m_pScene->addActor(*pPxController->getActor());
	return S_OK;
}

const PxRenderBuffer& CPhysicsXSystem::Get_RenderBuffer()
{
	return m_pScene->getRenderBuffer();
}

HRESULT CPhysicsXSystem::Intit_Scene()
{
	if (!m_pPhysics)
		return E_FAIL;
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);

	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.f, -9.8f, 0.f);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	/*m_pContactRePort = new ContactReportCallback();*/
	sceneDesc.simulationEventCallback = &m_pContactRePort;

	m_pScene = m_pPhysics->createScene(sceneDesc);

	if (!m_pScene)
		return E_FAIL;

	return S_OK;
}

HRESULT CPhysicsXSystem::Init_ControllerManager()
{
	if (m_pScene == nullptr)
	{
		return E_FAIL;
	}

	m_pControllerManager = PxCreateControllerManager(*m_pScene);

	m_pControllerBehaviorCallback = new CControllerBehaviorCallback();
	m_pControllerHitReport = new CControllerHitReport();

	return S_OK;
}


void CPhysicsXSystem::Free()
{
	//Safe_Delete(m_pContactRePort);

	if (m_pControllerManager)
		m_pControllerManager->release();
	Safe_Delete(m_pControllerHitReport);
	Safe_Delete(m_pControllerBehaviorCallback);


	if (m_pPvd)
		m_pPvd->disconnect();



	m_pScene->release();
	m_pDispatcher->release();
	m_pCooking->release();
	m_pPhysics->release();
	if (m_pPvd)
		m_pPvd->release();
	if (m_pPvdTransport)
		m_pPvdTransport->release();
	m_pFoundation->release();
	//PX_RELEASE(m_pScene);
	//PX_RELEASE(m_pDispatcher);
	//PX_RELEASE(m_pCooking);
	//PX_RELEASE(m_pPhysics);
	//PX_RELEASE(m_pFoundation);
}

void CPhysicsXSystem::ContactReportCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
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

void CPhysicsXSystem::ContactReportCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
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
