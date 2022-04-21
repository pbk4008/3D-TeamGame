#pragma once

#include "SingleTon.h"
#include "Flex/NvFlex.h"
#include "Flex/NvFlexExt.h"

BEGIN(Engine)

class CFlexSystem final : public CSingleTon<CFlexSystem>
{

private: friend CSingleTon;

private: NO_COPY(CFlexSystem);
private: explicit CFlexSystem();
private: virtual ~CFlexSystem() = default;

public: HRESULT	Init_Flex();
public: HRESULT	Ready_Particles(_uint particleCnt, _uint diffuseParticleCnt);
public: HRESULT Tick_Flex(_double time);

private: NvFlexLibrary*	m_NvLibrary		 = nullptr;
private: NvFlexSolver*	m_NvSolver		 = nullptr;
private: NvFlexBuffer*	m_particlebuffer = nullptr;
private: NvFlexBuffer*	m_phasebuffer	 = nullptr;
private: NvFlexBuffer*	m_velocitybuffer = nullptr;
private: _int			n				 = 100;
private: _int			m_particle_count = 6;
private: _float			m_particle_mass	 = 100;

public: virtual void Free() override;
};
END	
