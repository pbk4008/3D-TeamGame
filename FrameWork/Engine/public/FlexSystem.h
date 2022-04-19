#pragma once

#include "SingleTon.h"
#include "Flex/NvFlex.h"

BEGIN(Engine)

class CFlexSystem final : public CSingleTon<CFlexSystem>
{
private: friend CSingleTon;

private: NO_COPY(CFlexSystem);
private: explicit CFlexSystem();
private: virtual ~CFlexSystem() = default;

public: HRESULT	Init_Flex();

private: NvFlexLibrary* m_NvLibrary = nullptr;
private: NvFlexSolver*	m_NvSolver = nullptr;

public: virtual void Free() override;
};
END	
