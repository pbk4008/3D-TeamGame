#pragma once
#ifndef __PHYSICSXSYSTEM__
#define __PHYSICSXSYSTEM__

#include "SingleTon.h"

BEGIN(Engine)
class CPhysicsXSystem final : public CSingleTon<CPhysicsXSystem>
{
friend CSingleTon;
private:
	NO_COPY(CPhysicsXSystem);
	explicit CPhysicsXSystem();
	virtual ~CPhysicsXSystem() = default;
public:
	HRESULT Init_PhysicsX();
	HRESULT Intit_Scene();
private:
	virtual void Free() override;
private:
	PxDefaultAllocator m_Allocator;
	PxDefaultErrorCallback m_ErrorCallBack;
	PxFoundation* m_pFoundation;
	PxPhysics* m_pPhysics;
	PxScene* m_pScene;
	PxDefaultCpuDispatcher* m_pDispatcher;
};
#endif
END