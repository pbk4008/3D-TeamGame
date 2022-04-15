#include "ControllerFilterCallback.h"

CControllerFilterCallback::CControllerFilterCallback()
	: PxControllerFilterCallback()
{
}

HRESULT CControllerFilterCallback::NativeConstruct()
{
	return S_OK;
}

bool CControllerFilterCallback::filter(const PxController& a, const PxController& b)
{
	const PxU32 numShapes0 = a.getActor()->getNbShapes();
	PxShape** ppShape0 = (PxShape**)m_Allocator.allocate(sizeof(PxShape*) * numShapes0, 0, __FILE__, __LINE__);
	a.getActor()->getShapes(ppShape0, numShapes0);
	const PxU32 numShapes1 = b.getActor()->getNbShapes();
	PxShape** ppShape1 = (PxShape**)m_Allocator.allocate(sizeof(PxShape*) * numShapes1, 0, __FILE__, __LINE__);
	b.getActor()->getShapes(ppShape1, numShapes1);

	if (ppShape0[0] && ppShape1[0])
	{
		PxFilterData filterData0 = ppShape0[0]->getSimulationFilterData();
		PxFilterData filterData1 = ppShape1[0]->getSimulationFilterData();

		_bool isCollision = false;
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
			isCollision = true;
		else
			isCollision = false;

		if (ppShape0)
		{
			m_Allocator.deallocate(ppShape0);
			ppShape0 = NULL;
		}
		if (ppShape1)
		{
			m_Allocator.deallocate(ppShape1);
			ppShape1 = NULL;
		}

		return isCollision;
	}

	return false;
}