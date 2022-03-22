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
	return true;
}