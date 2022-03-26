#include "pch.h"
#include "BronzeAnimus_State.h"
#include "Monster_BronzeAnimus.h"
#include "Animation.h"
#include "UI_Monster_Panel.h"	

CBronzeAnimus_State::CBronzeAnimus_State(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

CBronzeAnimus_State::CBronzeAnimus_State(const CBronzeAnimus_State& _rhs)
	: CMonster_FSM(_rhs)
{
}

HRESULT CBronzeAnimus_State::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_State::Tick(const _double& _dDeltaTime)
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

	if (2.0f >= fDistToPlayer)
		m_bAttackOn = true;

	if (m_pMonster->Get_GroggyGauge() >= MAXGROOGUGAGUE)
	{
		//스턴상태일때 스턴state에서 현재 그로기 계속 0으로 고정시켜줌
		CMonster_BronzeAnimus* pMonster = static_cast<CMonster_BronzeAnimus*>(m_pMonster);

		pMonster->m_bGroggy = true;
		pMonster->Set_GroggyGauge(0.f);
		pMonster->m_pPanel->Set_GroggyBar(pMonster->Get_GroggyGaugeRatio());
		m_pStateController->Change_State(L"Groggy");
	}

	if (0 >= m_pMonster->Get_CurrentHp())
		m_pStateController->Change_State(L"Death");

	return _int();
}

_int CBronzeAnimus_State::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	return _int();
}

HRESULT CBronzeAnimus_State::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	Render_Debug();
#endif

	return S_OK;
}

HRESULT CBronzeAnimus_State::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (!m_bBattleOn)
	{
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_IDLE)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_IDLE_BATTLE)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBronzeAnimus_State::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CBronzeAnimus_State::Look_Player(void)
{
	
}

void CBronzeAnimus_State::Look_Monster(void)
{

}

HRESULT CBronzeAnimus_State::Render_Debug()
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

void CBronzeAnimus_State::Check_Attack(const _double& _dDeltaTime)
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

	if (0 > m_CheckFWD && (3.0f < m_fDistance && 10.0f > m_fDistance))
	{
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
		m_bTargetOn = true;
		m_bBattleOn = true;
	}

	if (0 > m_CheckFWD && 3.0f > m_fDistance)
	{
		if (20.0f > m_fRadian)
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

void CBronzeAnimus_State::OnTriggerEnter(CCollision& collision)
{
	CMonster_BronzeAnimus* pHealer = static_cast<CMonster_BronzeAnimus*>(m_pMonster);

	if (true == g_pObserver->IsAttack()) //플레이어공격일때
	{
		pHealer->m_bFirstHit = true; //딱 한번 true로 변경해줌

		if (true == pHealer->m_bFirstHit)
		{
			pHealer->m_pPanel->Set_BackUIGapY(1.f);
		}

		if ((_uint)GAMEOBJECT::WEAPON == collision.pGameObject->getTag())
		{
			pHealer->Set_Current_HP(-5.f);
			pHealer->Set_GroggyGauge(2); //TODO::수치정해서바꿔줘야됨

			pHealer->m_pPanel->Set_HpBar(pHealer->Get_HpRatio());

			if (false == pHealer->m_bGroggy)
			{
				//그로기 아닐때만 증가할수있게
				pHealer->m_pPanel->Set_GroggyBar(pHealer->Get_GroggyGaugeRatio());
				m_pStateController->Change_State(L"Hit");
			}
		}
		else
		{
			m_pStateController->Change_State(L"A_Idle_Battle");
		}
	}
}

CBronzeAnimus_State* CBronzeAnimus_State::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_State* pInstance = new CBronzeAnimus_State(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_State Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBronzeAnimus_State::Free()
{
	__super::Free();
}
