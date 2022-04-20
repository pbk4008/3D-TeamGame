#include "FlexSystem.h"

#include <iostream>
#include "Graphic_Device.h"

#define NV_FLEX_VERSION 120

CFlexSystem::CFlexSystem()
{
}
 static bool	g_Error = false;
 static bool	g_extensions = true;
 static int		g_device = -1;

 void ErrorCallback(NvFlexErrorSeverity severity, const char* msg, const char* file, int line)
 {
	 printf("Flex: %s - %s:%d\n", msg, file, line);
	 g_Error = (severity == eNvFlexLogError);
	 //assert(0); asserts are bad for TeamCity
 }

HRESULT CFlexSystem::Init_Flex()
{
	CGraphic_Device* pdevice = GET_INSTANCE(CGraphic_Device);

	NvFlexInitDesc desc;
	desc.deviceIndex = g_device;
	desc.enableExtensions = g_extensions;
	desc.renderDevice = pdevice->Get_Device();
	desc.renderContext = pdevice->Get_DeviceContext();
	desc.computeContext = 0;
	desc.computeType = eNvFlexD3D11;
	desc.runOnRenderContext = false;

	m_NvLibrary = NvFlexInit(NV_FLEX_VERSION, ErrorCallback,&desc);

	if (m_NvLibrary == nullptr)
		MSGBOX("Failed to Creating NvFlex Library");
	
	// 새로운 솔버 생성
	NvFlexSolverDesc solverDesc;
	NvFlexSetSolverDescDefaults(&solverDesc);
	solverDesc.maxParticles = n;
	solverDesc.maxDiffuseParticles = 0;

	m_NvSolver = NvFlexCreateSolver(m_NvLibrary, &solverDesc);

	m_particlebuffer = NvFlexAllocBuffer(m_NvLibrary, n, sizeof(_float4), eNvFlexBufferHost);
	m_velocitybuffer = NvFlexAllocBuffer(m_NvLibrary, n, sizeof(_float4), eNvFlexBufferHost);
	m_phasebuffer	 = NvFlexAllocBuffer(m_NvLibrary, n, sizeof(_int), eNvFlexBufferHost);

	// eNvFlexBufferHost = 0,	//!< 호스트 매핑 가능 버퍼, CUDA의 고정 메모리, DX의 스테이징 버퍼
	// eNvFlexBufferDevice = 1,	//!< CUDA에 매핑하는 장치 메모리 버퍼는 장치 메모리 포인터를 반환하고 DX에 버퍼 포인터를 반환합니다.

	// 매개변수 설정
	NvFlexParams p;

	int numIterations = 2;                  //!< 하위 단계당 수행할 솔버 반복 횟수

	p.gravity[0] = 0;
	p.gravity[1] = -9.8f;
	p.gravity[2] = 0;                       //!< 모든 입자에 일정한 가속도 적용
	p.radius = 0.01f;                       //!< 입자의 최대 상호작용 반경
	p.solidRestDistance = 0.005f;           //!< 유체가 아닌 입자가 서로 유지하려고 하는 거리는 (0, 반지름) 범위에 있어야 합니다.
	p.fluidRestDistance = 0.001f;           //!< 유체 입자가 나머지 밀도에서 간격을 두고 있는 거리는 (0, 반경) 범위에 있어야 합니다. 
											//	 유체의 경우 일반적으로 mRadius의 50-70%여야 합니다. 강체의 경우 단순히 입자 반경과 같을 수 있습니다.

	// 공통 매개변수
	p.dynamicFriction = 1;              //!< 모양과 충돌할 때 사용되는 마찰 계수
	p.staticFriction = 1;               //!< 모양과 충돌할 때 사용되는 정지 마찰 계수
	p.particleFriction = 1;             //!< 입자 충돌 시 사용되는 마찰 계수
	p.restitution = 1;                  //!< 모양과 충돌할 때 사용되는 반발 계수, 입자 충돌은 항상 비탄성적입니다.
	p.adhesion = 1;                     //!< 입자가 부딪힌 표면에 얼마나 강하게 달라붙는지 제어합니다(기본값 0.0, 범위 [0.0, +inf]).
	p.sleepThreshold = 1;               //!< 속도 크기가 이 임계값보다 작은 입자는 고정된 것으로 간주됩니다.

	p.maxSpeed = 100;			        //!< 입자 속도의 크기는 각 단계가 끝날 때 이 값으로 고정됩니다.
	p.maxAcceleration = 100;	        //!< 입자 가속도의 크기는 각 단계가 끝날 때 이 값으로 고정되며(초당 최대 속도 변경 제한) 큰 상호 침투로 인한 터지는 것을 방지하는 데 유용합니다.

	p.shockPropagation = 0;             //!< 고정된 기준점에서 높이를 기준으로 입자의 질량을 인위적으로 줄이면 스택과 파일이 더 빨리 수렴됩니다.
	p.dissipation = 1;                  //!< 입자 접촉 수에 따라 입자 속도 감소
	p.damping = 10;                     //!< 점성 항력, 입자 속도에 비례하고 반대되는 힘을 가합니다.

	// 천 매개변수
	p.wind[0] = 0;
	p.wind[0] = 0;
	p.wind[0] = 0;                      //!< 동적 삼각형에 속하는 입자에 일정한 가속이 적용되며, 바람이 삼각형에 영향을 미치려면 드래그가 > 0이어야 합니다.
	p.drag = 1;                         //!< 음의 속도 방향에서 속도^2*면적에 비례하는 동적 삼각형에 속하는 입자에 적용된 드래그 힘
	p.lift = 1;                         //!< 동적 삼각형에 속하는 입자에 적용된 양력, 속도^2*속도에 수직인 방향의 면적 및 (가능한 경우) 평면 법선에 평행

	// 유체 매개변수
	p.cohesion = 0.025f;                //!< 입자가 서로를 얼마나 강하게 유지하는지 제어, 기본값: 0.025, 범위 [0.0, +inf]
	p.surfaceTension = 0;               //!< 입자가 표면적을 최소화하려는 강도를 제어합니다. 기본값: 0.0, 범위: [0.0, +inf]
	p.viscosity = 1;                    //!< XSPH 점도를 사용하여 입자 속도를 부드럽게 합니다.
	p.vorticityConfinement = 0;         //!< 입자에 회전력을 적용하여 소용돌이도 증가
	p.anisotropyScale = 0;              //!< 렌더링을 위해 결과 타원체에 얼마나 많은 이방성이 있는지 제어합니다. 0이면 이방성이 계산되지 않습니다. NvFlexGetAnisotropy() 참조
	p.anisotropyMin = 0;                //!< 이방성 스케일을 반경의 이 부분으로 고정합니다.
	p.anisotropyMax = 0;                //!< 이방성 스케일을 반경의 이 부분으로 고정합니다.
	p.smoothing = 1;                    //!< 렌더링을 위해 파티클에서 라플라시안 스무딩의 강도를 제어합니다. 0이면 스무딩된 위치가 계산되지 않습니다. NvFlexGetSmoothParticles() 참조
	p.solidPressure = 0;                //!< 고체 표면에서 입자에 압력 추가
	p.freeSurfaceDrag = 0;              //!< 경계 유체 입자에 적용된 항력
	p.buoyancy = 0;                     //!< 중력은 유체 입자에 대한 이 값으로 조정됩니다.

	// 확산 매개변수
	p.diffuseThreshold = 0;             //!< 이 임계값보다 높은 운동 에너지 + 발산이 있는 입자는 새로운 확산 입자를 생성합니다.
	p.diffuseBuoyancy = 0;              //!< 확산 입자가 받는 반대 중력을 비늘로 조정합니다.
	p.diffuseDrag = 0;                  //!< 스케일은 분산 입자가 이웃 유체 입자의 방향으로 받는 힘을 가합니다.
	p.diffuseBallistic = 0;             //!< 확산 입자가 탄도로 간주되는 이웃의 수
	p.diffuseLifetime = 0;              //!< 확산 입자가 생성된 후 지속되는 시간(초), 입자는 [0, diffuseLifetime] 범위에서 임의의 수명으로 생성됩니다.

	// 충돌 매개변수
	p.collisionDistance = 0.01f;        //!< 거리 입자는 모양에 대해 유지됩니다. 삼각형 메시에 대한 강력한 충돌의 경우 이 거리는 0보다 커야 합니다.
	p.particleCollisionMargin = 1;      //!< 인접 항목을 찾는 동안 사용되는 반경을 늘립니다. 입자가 단일 단계 동안 크게 이동할 것으로 예상되는 경우 후속 반복에서 접점이 누락되지 않도록 하는 데 유용합니다.
	p.shapeCollisionMargin = 0;         //!< 운동학적 형태에 대한 접촉을 찾는 동안 사용되는 반경을 늘립니다.

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			p.planes[i][j] = 0;
		}
	}                                       //!< ax + by + cz + d = 0 형식의 충돌 평면
	p.numPlanes = 0;                        //!< 충돌 평면 수

	p.relaxationMode = NvFlexRelaxationMode::eNvFlexRelaxationLocal;   //!< 솔버 내부에서 이완이 적용되는 방식
	p.relaxationFactor = 1;                                             //!< 병렬 솔버의 수렴 속도 제어, 기본값: 1, 값이 1보다 크면 불안정해질 수 있음

	NvFlexSetParams(m_NvSolver, &p);

	//map buffers
	_float4* particles = (_float4*)NvFlexMap(m_particlebuffer, eNvFlexMapWait);
	_float4* velocities = (_float4*)NvFlexMap(m_velocitybuffer, eNvFlexMapWait);
	int* phases = (int*)NvFlexMap(m_phasebuffer, eNvFlexMapWait);

	for (int i = 0; i < m_particle_count; ++i)
	{
		particles[i] = { i * 5.0f,0,0, 1.0f / m_particle_mass };
		velocities[i] = { 0.0f,0.0f,0.0f,0.0f };
		phases[i] = NvFlexMakePhase(0, 1);
	}

	//unmap buffers
	NvFlexUnmap(m_particlebuffer);
	NvFlexUnmap(m_velocitybuffer);
	NvFlexUnmap(m_phasebuffer);

	NvFlexSetParticles(m_NvSolver, m_particlebuffer, NULL);
	NvFlexSetVelocities(m_NvSolver, m_velocitybuffer, NULL);
	NvFlexSetPhases(m_NvSolver, m_phasebuffer, NULL);

	return S_OK;
}

HRESULT CFlexSystem::Ready_Particles(_uint particleCnt, _uint diffuseParticleCnt)
{
	return S_OK;
}

HRESULT CFlexSystem::Tick_Flex(_double time)
{

	//_float4* particles = (_float4*)NvFlexMap(m_particlebuffer, eNvFlexMapWait);
	//_float3* velocity = (_float3*)NvFlexMap(m_velocitybuffer, eNvFlexMapWait);
	//_int* phases = (_int*)NvFlexMap(m_phasebuffer, eNvFlexMapWait);

	/*std::cout << "x : " << particles[0].x << " / y : " << particles[0].y << " / z : " << particles[0].z << std::endl;*/

	//NvFlexUnmap(m_particlebuffer);
	//NvFlexUnmap(m_velocitybuffer);
	//NvFlexUnmap(m_phasebuffer);

	//NvFlexSetActiveCount(m_NvSolver, m_particle_count);

	NvFlexUpdateSolver(m_NvSolver, (_float)time, 1, false);

	/*NvFlexSetParticles(m_NvSolver, m_particlebuffer, nullptr);
	NvFlexSetParticles(m_NvSolver, m_velocitybuffer, nullptr);
	NvFlexSetParticles(m_NvSolver, m_phasebuffer, nullptr);*/

	return S_OK;
}

void CFlexSystem::Free()
{
	NvFlexFreeBuffer(m_particlebuffer);
	NvFlexFreeBuffer(m_velocitybuffer);
	NvFlexFreeBuffer(m_phasebuffer);

	NvFlexDestroySolver(m_NvSolver);
	NvFlexShutdown(m_NvLibrary);
}
