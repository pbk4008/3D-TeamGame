#include "ControllerHitReport.h"

CControllerHitReport::CControllerHitReport()
	: PxUserControllerHitReport()
{
}

HRESULT CControllerHitReport::NativeConstruct()
{
	return S_OK;
}

void CControllerHitReport::onShapeHit(const PxControllerShapeHit& hit)
{
	int a = 0;
}

void CControllerHitReport::onControllerHit(const PxControllersHit& hit)
{
	int a = 0;
}

void CControllerHitReport::onObstacleHit(const PxControllerObstacleHit& hit)
{
	int a = 0;
}
