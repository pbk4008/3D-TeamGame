#include "pch.h"
#include "Aberrant_Run.h"

#include "Monster_EarthAberrant.h"
#include "Animation.h"

CAberrant_Run::CAberrant_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CAberrant_Run::CAberrant_Run(const CAberrant_Run& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Run::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAberrant_Run::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	if ((_uint)CMonster_EarthAberrant::RUN_FWD == m_pAnimator->Get_CurrentAnimNode())
	{
		Chase_Player(TimeDelta);
	}

	if ((_uint)CMonster_EarthAberrant::RUN_FWD_START == m_pAnimator->Get_CurrentAnimNode())
	{
		Chase_Player(TimeDelta);
	}

	_float fDistToPlayer = g_pObserver->Get_Dist(m_pTransform->Get_State(CTransform::STATE::STATE_POSITION));
	
	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	
	m_fRunTime += (_float)TimeDelta;
	if (0.5f < m_fRunTime)
	{
		if (m_iAttackRand == 0)
		{
			if (4.f > fDistToPlayer)
			{
				m_pStateController->Change_State(L"Attack", &m_iAttackRand);
				//cout << "공격으로 변경" << endl;
			}
		}
		else
		{
			{
				if (2.f > fDistToPlayer)
				{
					m_pStateController->Change_State(L"Attack", &m_iAttackRand);
					//cout << "공격으로 변경" << endl;
				}
			}
		}
	}

	return _int();
}

_int CAberrant_Run::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Run::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAberrant_Run::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation(CMonster_EarthAberrant::MON_STATE::RUN_FWD_START);
	m_iAttackRand = rand() % 2;

	return S_OK;
}

HRESULT CAberrant_Run::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_fRunTime = 0.f;
	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

HRESULT CAberrant_Run::ExitState(void* pArg)
{
	m_fRunTime = 0.f;
	return S_OK;
}

void CAberrant_Run::Look_Player(void)
{

}

void CAberrant_Run::Chase_Player(_double TimeDelta)
{
	_vector vPlayerPos = g_pObserver->Get_PlayerPos();
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDir = vPlayerPos - vPos;
	vDir = XMVector3Normalize(vDir);

	vDir *= (_float)TimeDelta * 5.f;

	m_pTransform->Add_Velocity(vDir);
}

CAberrant_Run* CAberrant_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Run* pInstance = new CAberrant_Run(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Run Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Run::Free()
{
	__super::Free();
}
