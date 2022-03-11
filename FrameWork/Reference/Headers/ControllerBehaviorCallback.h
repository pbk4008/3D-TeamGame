#pragma once

#include "Base.h"

BEGIN(Engine)

class CControllerBehaviorCallback : public PxControllerBehaviorCallback
{
public:
	explicit CControllerBehaviorCallback();
	virtual ~CControllerBehaviorCallback() = default;

public:
	HRESULT NativeConstruct();

public:
	// PxControllerBehaviorCallback��(��) ���� ��ӵ�
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) override;
};

END