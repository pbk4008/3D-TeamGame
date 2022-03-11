#pragma once

#include "Base.h"

BEGIN(Engine)

class CControllerFilterCallback : public PxControllerFilterCallback
{
public:
	explicit CControllerFilterCallback();
	virtual ~CControllerFilterCallback() = default;

public:
	HRESULT NativeConstruct();
	
public:
	// PxControllerFilterCallback��(��) ���� ��ӵ�
	virtual bool filter(const PxController& a, const PxController& b) override;
};

END