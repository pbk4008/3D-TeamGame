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
}

void CControllerHitReport::onControllerHit(const PxControllersHit& hit)
{
}

void CControllerHitReport::onObstacleHit(const PxControllerObstacleHit& hit)
{
}
