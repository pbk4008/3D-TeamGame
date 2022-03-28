#include "ControllerHitReport.h"
#include "GameObject.h"
#include "PhysicsXSystem.h"

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
	CGameObject* pShapeOBject = static_cast<CGameObject*>(hit.actor->userData);
	CGameObject* pControllerObject = static_cast<CGameObject*>(hit.controller->getActor()->userData);
	if (!pShapeOBject || !pControllerObject)
		return;

	CCollision shapeCollision;
	shapeCollision.pGameObject = pControllerObject;
	shapeCollision.vPos = FromPxextendedVec3(hit.worldPos);
	CCollision controllerCollision;
	controllerCollision.pGameObject = pShapeOBject;
	controllerCollision.vPos = FromPxextendedVec3(hit.worldPos);

	pShapeOBject->OnControllerColliderHit(shapeCollision);
	pControllerObject->OnControllerColliderHit(controllerCollision);
}

void CControllerHitReport::onControllerHit(const PxControllersHit& hit)
{
	int a = 0;
}

void CControllerHitReport::onObstacleHit(const PxControllerObstacleHit& hit)
{
	int a = 0;
}
