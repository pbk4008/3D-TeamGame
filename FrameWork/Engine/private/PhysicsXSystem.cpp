#include "PhysicsXSystem.h"

#define RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
CPhysicsXSystem::CPhysicsXSystem()
	: m_pFoundation(nullptr)
	, m_pPhysics(nullptr)
	, m_pScene(nullptr)
	, m_pDispatcher(nullptr)
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

	if (FAILED(Intit_Scene()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPhysicsXSystem::Intit_Scene()
{
	if (!m_pPhysics)
		return E_FAIL;
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = _pvec3(0.f, -9.8f, 0.f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_pScene = m_pPhysics->createScene(sceneDesc);

	if (!m_pScene)
		return E_FAIL;

	return S_OK;
}

void CPhysicsXSystem::Free()
{
	RELEASE(m_pScene);
	RELEASE(m_pDispatcher);
	RELEASE(m_pPhysics);
	RELEASE(m_pFoundation);
}
