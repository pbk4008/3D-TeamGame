#include "QueryFilterCallback.h"

CQueryFilterCallback::CQueryFilterCallback()
{
}

CQueryFilterCallback::CQueryFilterCallback(const PxU32 layerMask)
{
	m_layerMask = layerMask;
}

PxQueryHitType::Enum CQueryFilterCallback::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	PxFilterData filterData0 = shape->getQueryFilterData();
	if (filterData0.word0 & m_layerMask)
		return PxQueryHitType::eTOUCH;
	else
		return PxQueryHitType::eNONE;
}

PxQueryHitType::Enum CQueryFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
{
	return PxQueryHitType::Enum();
}
