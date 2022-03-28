#include "pch.h"
#include "Spear_State.h"
#include "Monster_Bastion_Spear.h"
#include "Animation.h"
#include "UI_Monster_Panel.h"	

CSpear_State::CSpear_State(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

CSpear_State::CSpear_State(const CSpear_State& _rhs)
	: CMonster_FSM(_rhs)
{
}

HRESULT CSpear_State::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_State::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	m_bTargetOn = false;
	m_bAttackOn = false;
	m_bPlayerAttack = false;


	Check_Attack(_dDeltaTime);

	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (2.0f >= fDistToPlayer && false == m_bChargeOn)
		m_bAttackOn = true;

	if (m_pMonster->Get_GroggyGauge() >= MAXGROOGUGAGUE)
	{
		//스턴상태일때 스턴state에서 현재 그로기 계속 0으로 고정시켜줌
		CMonster_Bastion_Spear* pMonster = static_cast<CMonster_Bastion_Spear*>(m_pMonster);

		pMonster->m_bGroggy = true;
		pMonster->Set_GroggyGauge(0.f);
		pMonster->m_pPanel->Set_GroggyBar(pMonster->Get_GroggyGaugeRatio());
		m_pStateController->Change_State(L"Groggy");
	}
	return _int();
}

_int CSpear_State::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	return _int();
}

HRESULT CSpear_State::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	Render_Debug();
#endif

	return S_OK;
}

HRESULT CSpear_State::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;


	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_State::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CSpear_State::Look_Player(void)
{
	
}

void CSpear_State::Look_Monster(void)
{

}

HRESULT CSpear_State::Render_Debug()
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

void CSpear_State::Check_Attack(const _double& _dDeltaTime)
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

	/* 위치차이에 따른 Y 값 0으로 셋팅 */
	vecMonsterLook = XMVectorSetY(vecMonsterLook, 0.f);
	vPlayerToMonster = XMVectorSetY(vPlayerToMonster, 0.f);

	/* 두 벡터의 사이 각 */
	_vector svAngle = XMVector3AngleBetweenVectors(vecMonsterLook, vPlayerToMonster);
	XMStoreFloat(&m_fRadian, svAngle);
	m_fRadian = XMConvertToDegrees(m_fRadian);

	if (!m_bChargeOn)
	{
		if (0 > m_CheckFWD && (3.0f < m_fDistance && 10.0f > m_fDistance))
		{
			m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
			m_bTargetOn = true;
			m_bBattleOn = true;
		}

		if (0 > m_CheckFWD && 5.0f > m_fDistance)
		{
			if (20.0f > m_fRadian)
			{
				m_fAttackTime += _dDeltaTime;
				if (m_fAttackTime > 0.5f)
				{
					m_fAttackTime = 0.0f;
					CMonster_Bastion_Spear* pSpear = static_cast<CMonster_Bastion_Spear*>(m_pMonster);
					if (!pSpear->m_bGroggy && g_pObserver->IsAttack())
						m_pStateController->Change_State(L"Guard");
					else if (!m_bChargeOn && !pSpear->m_bGroggy)
						m_pStateController->Change_State(L"Attack");
				}
			}
		}
	}
}

void CSpear_State::OnTriggerEnter(CCollision& collision)
{
	CMonster_Bastion_Spear* pSpear = static_cast<CMonster_Bastion_Spear*>(m_pMonster);

	if (true == g_pObserver->IsAttack()) //플레이어공격일때
	{
		pSpear->m_bFirstHit = true; //딱 한번 true로 변경해줌

		if (true == pSpear->m_bFirstHit)
		{
			pSpear->m_pPanel->Set_BackUIGapY(1.f);
		}

		if ((_uint)GAMEOBJECT::WEAPON == collision.pGameObject->getTag())
		{
			if (m_pAnimator->Get_CurrentAnimNode() != (_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_GUARD)
			{
				pSpear->Set_Current_HP(-5.f);
				pSpear->Set_GroggyGauge(2); //TODO::수치정해서바꿔줘야됨
			}
			pSpear->m_pPanel->Set_HpBar(pSpear->Get_HpRatio());

			if (false == pSpear->m_bGroggy)
			{
				//그로기 아닐때만 증가할수있게
				pSpear->m_pPanel->Set_GroggyBar(pSpear->Get_GroggyGaugeRatio());
				if (m_pAnimator->Get_CurrentAnimNode() != (_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_GUARD)
					m_pStateController->Change_State(L"Hit");
			}
		}
		else
		{
			m_pStateController->Change_State(L"A_Idle");
		}
	}
}

CSpear_State* CSpear_State::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_State* pInstance = new CSpear_State(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_State Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpear_State::Free()
{
	__super::Free();
}
