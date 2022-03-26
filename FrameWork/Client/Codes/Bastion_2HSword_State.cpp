#include "pch.h"
#include "Bastion_2HSword_State.h"
#include "Monster_Bastion_2HSword.h"
#include "Animation.h"
#include "UI_Monster_Panel.h"	

CBastion_2HSword_State::CBastion_2HSword_State(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_State::CBastion_2HSword_State(const CBastion_2HSword_State& _rhs)
	: CMonster_FSM(_rhs)
{
}

HRESULT CBastion_2HSword_State::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_State::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	m_bTargetOn = false;
	m_bAttackOn = false;
	m_bPlayerAttack = false;
	m_bRageOn = false;

	if (3.5f < fDistToPlayer && 10.0f > fDistToPlayer)
	{
		m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
		m_bTargetOn = true;
	}
	
	else if (10.0f < fDistToPlayer && 15.0f > fDistToPlayer)
		m_bRageOn = true;

	if (3.5f > fDistToPlayer)
		m_bAttackOn = true;

	if (m_pMonster->Get_GroggyGauge() >= MAXGROOGUGAGUE)
	{
		//스턴상태일때 스턴state에서 현재 그로기 계속 0으로 고정시켜줌
		CMonster_Bastion_2HSword* pMonster = static_cast<CMonster_Bastion_2HSword*>(m_pMonster);

		pMonster->Groggy_Start();
		//m_pStateController->Change_State(L"Groggy");
	}

	if (0 >= m_pMonster->Get_CurrentHp())
		m_pStateController->Change_State(L"Death");

	return _int();
}

_int CBastion_2HSword_State::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	return _int();
}

HRESULT CBastion_2HSword_State::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	Render_Debug();
#endif

	return S_OK;
}

HRESULT CBastion_2HSword_State::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_State::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CBastion_2HSword_State::Look_Player(void)
{
	
}

void CBastion_2HSword_State::Look_Monster(void)
{
	if (0 >= m_pMonster->Get_CurrentHp())
	{
		dynamic_cast<CMonster_Bastion_2HSword*>(m_pMonster)->Set_Dead();
		m_pStateController->Change_State(L"Death");
	}
}

void CBastion_2HSword_State::OnTriggerEnter(CCollision& collision)
{
	CMonster_Bastion_2HSword* Sword_2H = static_cast<CMonster_Bastion_2HSword*>(m_pMonster);

	Sword_2H->Hit(collision);
	//if (true == g_pObserver->IsAttack()) //플레이어공격일때
	//{
	//	Sword_2H->m_bFirstHit = true; //딱 한번 true로 변경해줌

	//	if (true == Sword_2H->m_bFirstHit)
	//	{
	//		Sword_2H->m_pPanel->Set_BackUIGapY(1.f);
	//	}

	//	if ((_uint)GAMEOBJECT::WEAPON == collision.pGameObject->getTag())
	//	{
	//		Sword_2H->Set_Current_HP(-5.f);
	//		Sword_2H->Set_GroggyGauge(2); //TODO::수치정해서바꿔줘야됨

	//		Sword_2H->m_pPanel->Set_HpBar(Sword_2H->Get_HpRatio());

	//		if (false == Sword_2H->Get_Groggy())
	//		{
	//			//그로기 아닐때만 증가할수있게
	//			Sword_2H->m_pPanel->Set_GroggyBar(Sword_2H->Get_GroggyGaugeRatio());
	//			m_pStateController->Change_State(L"Hit");
	//		}
	//	}
	//	else
	//	{
	//		m_pStateController->Change_State(L"Idle");
	//	}
	//}
}

HRESULT CBastion_2HSword_State::Render_Debug()
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

	return S_OK;
}

CBastion_2HSword_State* CBastion_2HSword_State::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_State* pInstance = new CBastion_2HSword_State(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_State Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_2HSword_State::Free()
{
	__super::Free();
}
