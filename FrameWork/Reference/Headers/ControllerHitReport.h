#pragma once

#include "Base.h"

BEGIN(Engine)

class CControllerHitReport : public PxUserControllerHitReport
{
public:
	explicit CControllerHitReport();
	virtual ~CControllerHitReport() = default;

public:
	HRESULT NativeConstruct();

public:
	// PxUserControllerHitReport��(��) ���� ��ӵ�
	virtual void onShapeHit(const PxControllerShapeHit& hit) override;
	virtual void onControllerHit(const PxControllersHit& hit) override;
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) override;
};

END