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

		//CCollision collision0;
		//collision0.pGameObject = pObj1;
		//CCollision collision1;
		//collision1.pGameObject = pObj0;

		COLLISIONDESC tCollisionDesc;
		tCollisionDesc.collision0.pGameObject = pObj1;
		tCollisionDesc.collision1.pGameObject = pObj0;

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//pObj0->OnCollisionEnter(collision0);
			//pObj1->OnCollisionEnter(collision1);
			tCollisionDesc.eFlag = PxPairFlag::eNOTIFY_TOUCH_FOUND;
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			//pObj0->OnCollisionStay(collision0);
			//pObj1->OnCollisionStay(collision1);
			tCollisionDesc.eFlag = PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//pObj0->OnCollisionExit(collision0);
			//pObj1->OnCollisionExit(collision1);
			tCollisionDesc.eFlag = PxPairFlag::eNOTIFY_TOUCH_LOST;
		}

		m_listCollision.emplace_back(tCollisionDesc);
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

		//CCollision collision0;
		//collision0.pGameObject = pObj1;
		//CCollision collision1;
		//collision1.pGameObject = pObj0;

		COLLISIONDESC tCollisionDesc;
		tCollisionDesc.collision0.pGameObject = pObj1;
		tCollisionDesc.collision1.pGameObject = pObj0;

		if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//pObj0->OnTriggerEnter(collision0);
			//pObj1->OnTriggerEnter(collision1);
			tCollisionDesc.eFlag = PxPairFlag::eNOTIFY_TOUCH_FOUND;
		}
		//if (tp.status & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		//{
		//	pObj0->OnTriggerStay(collision0);
		//	pObj1->OnTriggerStay(collision1);
		//}
		if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//pObj0->OnTriggerExit(collision0);
			//pObj1->OnTriggerExit(collision1);
			tCollisionDesc.eFlag = PxPairFlag::eNOTIFY_TOUCH_LOST;
		}

		m_listTrigger.emplace_back(tCollisionDesc);
	}
}

void CSimulationEventCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}

void CSimulationEventCallback::FetchResult()
{
	for (auto& tCollisionDesc : m_listCollision)
	{
		switch (tCollisionDesc.eFlag)
		{
		case PxPairFlag::eNOTIFY_TOUCH_FOUND:
			tCollisionDesc.collision0.pGameObject->OnCollisionEnter(tCollisionDesc.collision1);
			tCollisionDesc.collision1.pGameObject->OnCollisionEnter(tCollisionDesc.collision0);
			break;
		case PxPairFlag::eNOTIFY_TOUCH_PERSISTS:
			tCollisionDesc.collision0.pGameObject->OnCollisionStay(tCollisionDesc.collision1);
			tCollisionDesc.collision1.pGameObject->OnCollisionStay(tCollisionDesc.collision0);
			break;
		case PxPairFlag::eNOTIFY_TOUCH_LOST:
			tCollisionDesc.collision0.pGameObject->OnCollisionExit(tCollisionDesc.collision1);
			tCollisionDesc.collision1.pGameObject->OnCollisionExit(tCollisionDesc.collision0);
			break;
		}
	}
	m_listCollision.clear();
	// 트리거
	for (auto& tCollisionDesc : m_listTrigger)
	{
		switch (tCollisionDesc.eFlag)
		{
		case PxPairFlag::eNOTIFY_TOUCH_FOUND:
			tCollisionDesc.collision0.pGameObject->OnTriggerEnter(tCollisionDesc.collision1);
			tCollisionDesc.collision1.pGameObject->OnTriggerEnter(tCollisionDesc.collision0);
			break;
		case PxPairFlag::eNOTIFY_TOUCH_LOST:
			tCollisionDesc.collision0.pGameObject->OnTriggerExit(tCollisionDesc.collision1);
			tCollisionDesc.collision1.pGameObject->OnTriggerExit(tCollisionDesc.collision0);
			break;
		}
	}
	m_listTrigger.clear();
}
