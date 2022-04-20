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
	
	// ���ο� �ֹ� ����
	NvFlexSolverDesc solverDesc;
	NvFlexSetSolverDescDefaults(&solverDesc);
	solverDesc.maxParticles = n;
	solverDesc.maxDiffuseParticles = 0;

	m_NvSolver = NvFlexCreateSolver(m_NvLibrary, &solverDesc);

	m_particlebuffer = NvFlexAllocBuffer(m_NvLibrary, n, sizeof(_float4), eNvFlexBufferHost);
	m_velocitybuffer = NvFlexAllocBuffer(m_NvLibrary, n, sizeof(_float4), eNvFlexBufferHost);
	m_phasebuffer	 = NvFlexAllocBuffer(m_NvLibrary, n, sizeof(_int), eNvFlexBufferHost);

	// eNvFlexBufferHost = 0,	//!< ȣ��Ʈ ���� ���� ����, CUDA�� ���� �޸�, DX�� ������¡ ����
	// eNvFlexBufferDevice = 1,	//!< CUDA�� �����ϴ� ��ġ �޸� ���۴� ��ġ �޸� �����͸� ��ȯ�ϰ� DX�� ���� �����͸� ��ȯ�մϴ�.

	// �Ű����� ����
	NvFlexParams p;

	int numIterations = 2;                  //!< ���� �ܰ�� ������ �ֹ� �ݺ� Ƚ��

	p.gravity[0] = 0;
	p.gravity[1] = -9.8f;
	p.gravity[2] = 0;                       //!< ��� ���ڿ� ������ ���ӵ� ����
	p.radius = 0.01f;                       //!< ������ �ִ� ��ȣ�ۿ� �ݰ�
	p.solidRestDistance = 0.005f;           //!< ��ü�� �ƴ� ���ڰ� ���� �����Ϸ��� �ϴ� �Ÿ��� (0, ������) ������ �־�� �մϴ�.
	p.fluidRestDistance = 0.001f;           //!< ��ü ���ڰ� ������ �е����� ������ �ΰ� �ִ� �Ÿ��� (0, �ݰ�) ������ �־�� �մϴ�. 
											//	 ��ü�� ��� �Ϲ������� mRadius�� 50-70%���� �մϴ�. ��ü�� ��� �ܼ��� ���� �ݰ�� ���� �� �ֽ��ϴ�.

	// ���� �Ű�����
	p.dynamicFriction = 1;              //!< ���� �浹�� �� ���Ǵ� ���� ���
	p.staticFriction = 1;               //!< ���� �浹�� �� ���Ǵ� ���� ���� ���
	p.particleFriction = 1;             //!< ���� �浹 �� ���Ǵ� ���� ���
	p.restitution = 1;                  //!< ���� �浹�� �� ���Ǵ� �ݹ� ���, ���� �浹�� �׻� ��ź�����Դϴ�.
	p.adhesion = 1;                     //!< ���ڰ� �ε��� ǥ�鿡 �󸶳� ���ϰ� �޶�ٴ��� �����մϴ�(�⺻�� 0.0, ���� [0.0, +inf]).
	p.sleepThreshold = 1;               //!< �ӵ� ũ�Ⱑ �� �Ӱ谪���� ���� ���ڴ� ������ ������ ���ֵ˴ϴ�.

	p.maxSpeed = 100;			        //!< ���� �ӵ��� ũ��� �� �ܰ谡 ���� �� �� ������ �����˴ϴ�.
	p.maxAcceleration = 100;	        //!< ���� ���ӵ��� ũ��� �� �ܰ谡 ���� �� �� ������ �����Ǹ�(�ʴ� �ִ� �ӵ� ���� ����) ū ��ȣ ħ���� ���� ������ ���� �����ϴ� �� �����մϴ�.

	p.shockPropagation = 0;             //!< ������ ���������� ���̸� �������� ������ ������ ���������� ���̸� ���ð� ������ �� ���� ���ŵ˴ϴ�.
	p.dissipation = 1;                  //!< ���� ���� ���� ���� ���� �ӵ� ����
	p.damping = 10;                     //!< ���� �׷�, ���� �ӵ��� ����ϰ� �ݴ�Ǵ� ���� ���մϴ�.

	// õ �Ű�����
	p.wind[0] = 0;
	p.wind[0] = 0;
	p.wind[0] = 0;                      //!< ���� �ﰢ���� ���ϴ� ���ڿ� ������ ������ ����Ǹ�, �ٶ��� �ﰢ���� ������ ��ġ���� �巡�װ� > 0�̾�� �մϴ�.
	p.drag = 1;                         //!< ���� �ӵ� ���⿡�� �ӵ�^2*������ ����ϴ� ���� �ﰢ���� ���ϴ� ���ڿ� ����� �巡�� ��
	p.lift = 1;                         //!< ���� �ﰢ���� ���ϴ� ���ڿ� ����� ���, �ӵ�^2*�ӵ��� ������ ������ ���� �� (������ ���) ��� ������ ����

	// ��ü �Ű�����
	p.cohesion = 0.025f;                //!< ���ڰ� ���θ� �󸶳� ���ϰ� �����ϴ��� ����, �⺻��: 0.025, ���� [0.0, +inf]
	p.surfaceTension = 0;               //!< ���ڰ� ǥ������ �ּ�ȭ�Ϸ��� ������ �����մϴ�. �⺻��: 0.0, ����: [0.0, +inf]
	p.viscosity = 1;                    //!< XSPH ������ ����Ͽ� ���� �ӵ��� �ε巴�� �մϴ�.
	p.vorticityConfinement = 0;         //!< ���ڿ� ȸ������ �����Ͽ� �ҿ뵹�̵� ����
	p.anisotropyScale = 0;              //!< �������� ���� ��� Ÿ��ü�� �󸶳� ���� �̹漺�� �ִ��� �����մϴ�. 0�̸� �̹漺�� ������ �ʽ��ϴ�. NvFlexGetAnisotropy() ����
	p.anisotropyMin = 0;                //!< �̹漺 �������� �ݰ��� �� �κ����� �����մϴ�.
	p.anisotropyMax = 0;                //!< �̹漺 �������� �ݰ��� �� �κ����� �����մϴ�.
	p.smoothing = 1;                    //!< �������� ���� ��ƼŬ���� ���ö�þ� �������� ������ �����մϴ�. 0�̸� �������� ��ġ�� ������ �ʽ��ϴ�. NvFlexGetSmoothParticles() ����
	p.solidPressure = 0;                //!< ��ü ǥ�鿡�� ���ڿ� �з� �߰�
	p.freeSurfaceDrag = 0;              //!< ��� ��ü ���ڿ� ����� �׷�
	p.buoyancy = 0;                     //!< �߷��� ��ü ���ڿ� ���� �� ������ �����˴ϴ�.

	// Ȯ�� �Ű�����
	p.diffuseThreshold = 0;             //!< �� �Ӱ谪���� ���� � ������ + �߻��� �ִ� ���ڴ� ���ο� Ȯ�� ���ڸ� �����մϴ�.
	p.diffuseBuoyancy = 0;              //!< Ȯ�� ���ڰ� �޴� �ݴ� �߷��� ��÷� �����մϴ�.
	p.diffuseDrag = 0;                  //!< �������� �л� ���ڰ� �̿� ��ü ������ �������� �޴� ���� ���մϴ�.
	p.diffuseBallistic = 0;             //!< Ȯ�� ���ڰ� ź���� ���ֵǴ� �̿��� ��
	p.diffuseLifetime = 0;              //!< Ȯ�� ���ڰ� ������ �� ���ӵǴ� �ð�(��), ���ڴ� [0, diffuseLifetime] �������� ������ �������� �����˴ϴ�.

	// �浹 �Ű�����
	p.collisionDistance = 0.01f;        //!< �Ÿ� ���ڴ� ��翡 ���� �����˴ϴ�. �ﰢ�� �޽ÿ� ���� ������ �浹�� ��� �� �Ÿ��� 0���� Ŀ�� �մϴ�.
	p.particleCollisionMargin = 1;      //!< ���� �׸��� ã�� ���� ���Ǵ� �ݰ��� �ø��ϴ�. ���ڰ� ���� �ܰ� ���� ũ�� �̵��� ������ ����Ǵ� ��� �ļ� �ݺ����� ������ �������� �ʵ��� �ϴ� �� �����մϴ�.
	p.shapeCollisionMargin = 0;         //!< ����� ���¿� ���� ������ ã�� ���� ���Ǵ� �ݰ��� �ø��ϴ�.

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			p.planes[i][j] = 0;
		}
	}                                       //!< ax + by + cz + d = 0 ������ �浹 ���
	p.numPlanes = 0;                        //!< �浹 ��� ��

	p.relaxationMode = NvFlexRelaxationMode::eNvFlexRelaxationLocal;   //!< �ֹ� ���ο��� �̿��� ����Ǵ� ���
	p.relaxationFactor = 1;                                             //!< ���� �ֹ��� ���� �ӵ� ����, �⺻��: 1, ���� 1���� ũ�� �Ҿ������� �� ����

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
