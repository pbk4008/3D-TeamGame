#pragma once
#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__
#include "LandScape.h"
class CEnvironment final : CLandScape
{
private:
	explicit CEnvironment();
	explicit CEnvironment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEnvironment(const CEnvironment& rhs);
	virtual ~CEnvironment() = default;
public:
	virtual  HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

};
#endif