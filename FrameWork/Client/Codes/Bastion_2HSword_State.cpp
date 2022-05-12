#include "pch.h"
#include "Bastion_2HSword_State.h"
#include "Monster_Bastion_2HSword.h"
#include "Animation.h"
#include "UI_Monster_Panel.h"	

#include "Stage1.h"
#include "Stage2.h"

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

	//_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	//_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	//_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));
	
	_float fDist = g_pObserver->Get_Dist(m_pTransform->Get_State(CTransform::STATE::STATE_POSITION));

	m_pOwner = static_cast<CMonster_Bastion_2HSword*>(m_pMonster);

	if (10.f <= fDist && 15.f >= fDist && m_pOwner->get_FirstAttack()&&!m_pOwner->get_Attack())
		m_pOwner->set_RageOn(true);

	if(!m_pOwner->get_RageOn())
	{
		if (3.5f <= fDist && 10.0f > fDist && !m_pOwner->get_Attack())
		{
			//m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
			//m_bTargetOn = true;
			//m_bAttackOn = false;
			m_pOwner->set_Target(true);
			m_pOwner->set_Attack(false);
		}
		else if (3.5f > fDist)
		{
			if (m_pOwner->get_RandAttack() == -1)
			{
				_int iRand = rand() % 4;
				m_pOwner->set_RandAttack(iRand);
				if (iRand == 0)
				{
					if (!m_pOwner->get_FirstAttack())
						m_pOwner->set_FirstAttack(true);

					m_pOwner->set_Target(false);
					m_pOwner->set_Attack(true);
				}
				else
				{
					if (1.5f < fDist)
					{
						m_pOwner->set_RandAttack(-1);
						m_pOwner->set_Target(true);
						m_pOwner->set_Attack(false);
					}
					else
					{
						if (!m_pOwner->get_FirstAttack())
							m_pOwner->set_FirstAttack(true);

						m_pOwner->set_Target(false);
						m_pOwner->set_Attack(true);
					}
				}
			}
		}
		//else if (1.5f >fDist)
		//{
		//	//m_bTargetOn = false;
		//	//m_bAttackOn = true;
		//	if (!m_pOwner->get_FirstAttack())
		//		m_pOwner->set_FirstAttack(true);

		//	m_pOwner->set_Target(false);
		//	m_pOwner->set_Attack(true);
		//}
		else if(10.0f < fDist)
		{
			m_pOwner->set_Target(false);
			m_pOwner->set_Attack(false);
		}
	}
	else
	{
		if (3.f <= fDist && !m_pOwner->get_Attack())
		{
			//m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
			//m_bTargetOn = true;
			//m_bAttackOn = false;
			if (m_pOwner->get_FirstAttack())
				m_pOwner->set_FirstAttack(false);

			m_pOwner->set_Target(true);
			m_pOwner->set_Attack(false);
			m_pOwner->set_RandAttack(-1);
		}
		else
		{
			//m_bTargetOn = false;
			//m_bAttackOn = true;
			if (m_pOwner->get_RandAttack() == -1)
			{
				m_pOwner->set_Target(false);
				m_pOwner->set_Attack(true);
			}
		}
	}

	if (FAILED(Check_State()))
		return -1;

	//if (m_pMonster->Get_GroggyGauge() >= MAXGROOGUGAGUE)
	//{
	//	//스턴상태일때 스턴state에서 현재 그로기 계속 0으로 고정시켜줌
	//	m_pOwner->Groggy_Start();
	//}

	if (0 >= m_pMonster->Get_CurrentHp() && !m_pMonster->Get_Dead())
	{
		m_pOwner->Set_Dead();
		m_pOwner->Remove_Collider();

		m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.f);
		m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);

		m_pStateController->Change_State(L"Death");

		m_pOwner->Set_LightCheck(true);
	}

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
}

void CBastion_2HSword_State::OnTriggerEnter(CCollision& collision)
{
	//if(nullptr != m_pOwner)
	//	m_pOwner->Hit(collision);
}

HRESULT CBastion_2HSword_State::Render_Debug()
{
	wstring wstrChaserOn = L"Target On : ";
	wstring wstrIsChaser;

	if(m_pOwner->get_Target())
		wstrIsChaser = L"TRUE";
	else
		wstrIsChaser = L"FALSE";

	//if (m_bTargetOn)
	//	wstrIsChaser = L"TRUE";
	//else
	//	wstrIsChaser = L"FALSE";

	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrChaserOn + wstrIsChaser).c_str(), _float2(950.f, 120.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	//////////////////

	wstring wstrAttackOn = L"Attack On : ";
	wstring wstrIsAttack;

	if (m_pOwner->get_Attack())
		wstrIsAttack = L"TRUE";
	else
		wstrIsAttack = L"FALSE";

	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrAttackOn + wstrIsAttack).c_str(), _float2(950.f, 140.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_State::Check_State()
{
	if (!m_pOwner->Get_Dead()&&!m_pOwner->Get_Groggy())
	{
		if (!m_pOwner->get_RageOn())
		{
			if (m_pOwner->get_Attack())
			{
				_uint iRand = m_pOwner->get_RandAttack();
				m_pStateController->Change_State(L"Attack",&iRand);
			}
			else if (m_pOwner->get_Target())
				m_pStateController->Change_State(L"Chaser");
			else
				m_pStateController->Change_State(L"Idle");
		}
		else
		{
			m_pStateController->Change_State(L"Rage");
		}
	}
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
