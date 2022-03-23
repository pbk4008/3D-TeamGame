#include "pch.h"
#include "Bastion_Healer_State.h"
#include "Monster_Bastion_Healer.h"
#include "Animation.h"

CBastion_Healer_State::CBastion_Healer_State(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

CBastion_Healer_State::CBastion_Healer_State(const CBastion_Healer_State& _rhs)
	: CMonster_FSM(_rhs)
{
}

HRESULT CBastion_Healer_State::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Healer_State::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_bTargetOn = false;
	m_bAttackOn = false;
	m_bPlayerAttack = false;

	Check_Attack(_dDeltaTime);

	m_pAnimator->Tick(_dDeltaTime);

	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (2.0f >= fDistToPlayer)
		m_bAttackOn = true;

	return _int();
}

_int CBastion_Healer_State::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	return _int();
}

HRESULT CBastion_Healer_State::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	Render_Debug();
#endif

	return S_OK;
}

HRESULT CBastion_Healer_State::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_Healer_State::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CBastion_Healer_State::Look_Player(void)
{
	
}

void CBastion_Healer_State::Look_Monster(void)
{

}

HRESULT CBastion_Healer_State::Render_Debug()
{
	wstring wstrChaserOn = L"Target On : ";
	wstring wstrIsChaser;

	if (m_bTargetOn)
		wstrIsChaser = L"TRUE";
	else
		wstrIsChaser = L"FALSE";

	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrChaserOn + wstrIsChaser).c_str(), _float2(950.f, 120.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	//////////////////

	wstring wstrAttackOn = L"Attack On : ";
	wstring wstrIsAttack;

	if (m_bAttackOn)
		wstrIsAttack = L"TRUE";
	else
		wstrIsAttack = L"FALSE";

	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrAttackOn + wstrIsAttack).c_str(), _float2(950.f, 140.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	wstring wstrFWD;
	if (0 > m_CheckFWD)
		wstrFWD = L"Forward";
	else
		wstrFWD = L"Backward";

	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), L"Check Distance : " + to_wstring(m_fDistance), _float2(950.f, 160.f), _float2(0.6f, 0.6f))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), L"Check FWD : " + wstrFWD, _float2(950.f, 180.f), _float2(0.6f, 0.6f))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), L"Check Radian : " + to_wstring(m_fRadian), _float2(950.f, 200.f), _float2(0.6f, 0.6f))))
		return E_FAIL;


	return S_OK;
}

void CBastion_Healer_State::Check_Attack(const _double& _dDeltaTime)
{
	/* 몬스터의 현재 Look 방향 벡터 */
	_vector vecMonsterLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	/* 몬스터가 플레이어에게 향하는 방향 벡터 */
	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	/* 몬스터와 플레이어 사이의 거리 */
	m_fDistance = XMVectorGetX(XMVector3Length(vDist));

	/* 플레이어가 몬스터의 앞에 있는지 뒤에있는지 판단 */
	_vector vecMonsterToPlayer = XMVector3Normalize(vDist);
	_vector dotVec = XMVector3Dot(vecMonsterLook, vecMonsterToPlayer);
	m_CheckFWD = XMVectorGetX(dotVec); /* 음수-> 플레이어가 앞에 있다*/

	/* 플레이어,몬스터 위치 차 벡터 */
	_vector vPlayerToMonster = g_pObserver->Get_PlayerPos() - vMonsterPos;

	/* 두 벡터의 사이 각 */
	vecMonsterLook = XMVectorSetY(vecMonsterLook, 0.f);
	vPlayerToMonster = XMVectorSetY(vPlayerToMonster, 0.f);

	_vector svAngle = XMVector3AngleBetweenVectors(vecMonsterLook, vPlayerToMonster);
	XMStoreFloat(&m_fRadian, svAngle);
	m_fRadian = XMConvertToDegrees(m_fRadian);

	if (0 > m_CheckFWD && (5.0f < m_fDistance && 10.0f > m_fDistance))
	{
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
		m_bTargetOn = true;
	}

	if (0 > m_CheckFWD && 3.0f > m_fDistance)
	{
		if (10.0f > m_fRadian)
		{
			m_fAttackTime += _dDeltaTime;
			if (m_fAttackTime > 0.5f)
			{
				m_fAttackTime = 0.0f;
				m_pStateController->Change_State(L"Attack");
			}
		}
	}

}

void CBastion_Healer_State::OnTriggerEnter(CCollision& collision)
{
	int a = 10;
}

CBastion_Healer_State* CBastion_Healer_State::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Healer_State* pInstance = new CBastion_Healer_State(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Healer_State Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_Healer_State::Free()
{
	__super::Free();
}
