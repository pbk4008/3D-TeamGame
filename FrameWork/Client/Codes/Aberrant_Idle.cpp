#include "pch.h"
#include "Aberrant_Idle.h"

#include "Animation.h"
#include "Monster_EarthAberrant.h"

CAberrant_Idle::CAberrant_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
	, m_iRandDist(0)
{
}

CAberrant_Idle::CAberrant_Idle(const CAberrant_Idle& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CAberrant_Idle::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;




	return S_OK;
}

_int CAberrant_Idle::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if ( 5.f + m_iRandDist > fDistToPlayer)
		m_pStateController->Change_State(L"Walk");
		//cout << "?ȱ??? ????" << endl;

	return _int();
}

_int CAberrant_Idle::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CAberrant_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAberrant_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_iRandDist = rand() % (10-5+1)+5;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CAberrant_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CAberrant_Idle::Look_Player(void)
{

}

CAberrant_Idle* CAberrant_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CAberrant_Idle* pInstance = new CAberrant_Idle(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAberrant_Idle Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAberrant_Idle::Free()
{
	__super::Free();
}
