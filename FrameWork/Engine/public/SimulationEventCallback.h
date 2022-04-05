#pragma once

#include "Base.h"
#include "PxSimulationEventCallback.h"
#include "Collision.h"

BEGIN(Engine)

class CSimulationEventCallback : public PxSimulationEventCallback
{
public:
	typedef struct tagTypeCollision
	{
		CCollision collision0;
		CCollision collision1;
		PxPairFlag::Enum eFlag;
	}COLLISIONDESC;
public:
	explicit CSimulationEventCallback();
	virtual ~CSimulationEventCallback() = default;

	// PxSimulationEventCallback을(를) 통해 상속됨
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
	virtual void onWake(PxActor** actors, PxU32 count) override;
	virtual void onSleep(PxActor** actors, PxU32 count) override;
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;

	void FetchResult();

private:
	list<COLLISIONDESC> m_listCollision;
	list<COLLISIONDESC> m_listTrigger;
};

END