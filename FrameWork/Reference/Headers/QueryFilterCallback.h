#pragma once

#include "Base.h"
#include "PxPhysicsAPI.h"

BEGIN(Engine)

class CQueryFilterCallback : public PxQueryFilterCallback
{
public:
	explicit CQueryFilterCallback();
	explicit CQueryFilterCallback(const PxU32 layerMask);
	virtual ~CQueryFilterCallback() = default;
	
	// PxQueryFilterCallback��(��) ���� ��ӵ�
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override;

private:
	PxU32 m_layerMask = 0;
};

END