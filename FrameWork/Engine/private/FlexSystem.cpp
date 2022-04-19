#include "FlexSystem.h"

CFlexSystem::CFlexSystem()
{
}

HRESULT CFlexSystem::Init_Flex()
{
	m_NvLibrary = NvFlexInit();

	NvFlexSolverDesc desc;
	desc.featureMode = NvFlexFeatureMode::eNvFlexFeatureModeDefault;
	desc.maxContactsPerParticle = 10;
	desc.maxDiffuseParticles = 10;
	desc.maxNeighborsPerParticle = 10;
	desc.maxParticles = 10;

	m_NvSolver = NvFlexCreateSolver(m_NvLibrary,&desc);

	//NvFlexUpdateSolver(solver,)

	return S_OK;
}

void CFlexSystem::Free()
{

	NvFlexDestroySolver(m_NvSolver);
	NvFlexShutdown(m_NvLibrary);
}
