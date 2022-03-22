#include "SimulationEventCallback.h"

#include "GameObject.h"
#include "Collision.h"

CSimulationEventCallback::CSimulationEventCallback()
{
}

void CSimulationEventCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void CSimulationEventCallback::onWake(PxActor** actors, PxU32 count)
{
}

void CSimulationEventCallback::onSleep(PxActor** actors, PxU32 count)
{
}

void CSimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (_uint i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];

		// 도형이 삭제되었을 때 쌍 무시 
		if (cp.flags & (PxContactPairFlag::eREMOVED_SHAPE_0 |
			PxContactPairFlag::eREMOVED_SHAPE_1))
		{
			continue;
		}

		CGameObject* pObj0 = static_cast<CGameObject*>(pairHeader.actors[0]->userData);
		CGameObject* pObj1 = static_cast<CGameObject*>(pairHeader.actors[1]->userData);

		if (!pObj0 || !pObj1)
			continue;

		CCollision collision0;
		collision0.pGameObject = pObj1;
		CCollision collision1;
		collision1.pGameObject = pObj0;

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pObj0->OnCollisionEnter(collision0);
			pObj1->OnCollisionEnter(collision1);
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			pObj0->OnCollisionStay(collision0);
			pObj1->OnCollisionStay(collision1);
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pObj0->OnCollisionExit(collision0);
			pObj1->OnCollisionExit(collision1);
		}
	}
}

void CSimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (_uint i = 0; i < count; i++)
	{
		const PxTriggerPair& tp = pairs[i];

		// 도형이 삭제되었을 때 쌍 무시 
		if (tp.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
			PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
		{
			continue;
		}

		CGameObject* pObj0 = static_cast<CGameObject*>(tp.triggerActor->userData);
		CGameObject* pObj1 = static_cast<CGameObject*>(tp.otherActor->userData);

		if (!pObj0 || !pObj1)
			continue;

		CCollision collision0;
		collision0.pGameObject = pObj1;
		CCollision collision1;
		collision1.pGameObject = pObj0;


		if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pObj0->OnTriggerEnter(collision0);
			pObj1->OnTriggerEnter(collision1);
		}
		//if (tp.status & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		//{
		//	pObj0->OnTriggerStay(collision0);
		//	pObj1->OnTriggerStay(collision1);
		//}
		if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pObj0->OnTriggerExit(collision0);
			pObj1->OnTriggerExit(collision1);
		}
	}
}

void CSimulationEventCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
