#include "pch.h"
#include "Spear_State.h"
#include "Monster_Bastion_Spear.h"
#include "Animation.h"
#include "UI_Monster_Panel.h"	

#include "Stage2.h"
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

	_float fDist = g_pObserver->Get_Dist(m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_pOwner = static_cast<CMonster_Bastion_Spear*>(m_pMonster);

	if (m_pOwner->Get_HpRatio() < 0.5f)
		m_pOwner->Set_Half(true);

	if (!m_pOwner->Get_Half())
	{
		if (10.f >= fDist && 2.f <= fDist && !m_pOwner->Get_Attack())
		{
			m_pOwner->Set_Target(true);
			m_pOwner->Set_Attack(false);
		}
		else if (2.f >= fDist)
		{
			if (g_pObserver->Get_PlayerAttackAnimStart() && m_pOwner->Get_GuardCount() > 0)
				m_pOwner->Set_Guard(true);

			m_pOwner->Set_Target(false);
			m_pOwner->Set_Attack(true);
		}
		else if (10.f < fDist)
		{
			m_pOwner->Set_Target(false);
			m_pOwner->Set_Attack(false);
		}
	}
	else
	{
		if (!m_pOwner->Get_ChargeOn())
		{
			if (fDist < 7.f)
			{
				m_pOwner->Set_Target(false);
				m_pOwner->Set_Attack(true);
			}
			else
			{
				m_pOwner->Set_Target(false);
				m_pOwner->Set_Attack(false);
			}
		}
		else
		{
			if (10.f >= fDist && 2.f <= fDist && !m_pOwner->Get_Attack())
			{
				m_pOwner->Set_Target(true);
				m_pOwner->Set_Attack(false);
			}
			else if (2.f >= fDist)
			{
				if (g_pObserver->Get_PlayerAttackAnimStart() && m_pOwner->Get_GuardCount() > 0)
					m_pOwner->Set_Guard(true);

				m_pOwner->Set_Target(false);
				m_pOwner->Set_Attack(true);
			}
			else if (10.f < fDist)
			{
				m_pOwner->Set_Target(false);
				m_pOwner->Set_Attack(false);
			}
		}
	}

	if (FAILED(Check_State()))
		return -1;

	if (!m_pOwner->Get_Dead())
	{
		if (m_pOwner->Get_GroggyGauge() >= m_pOwner->Get_MaxGroggyGauge())
		{
			//���ϻ����϶� ����state���� ���� �׷α� ��� 0���� ����������
			m_pOwner->Groggy_Start();
		}
		if (0 >= m_pOwner->Get_CurrentHp())
		{
			m_pOwner->Set_Dead();
			m_pOwner->Remove_Collider();

			m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.f);
			m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.f);

			m_pStateController->Change_State(L"Death");

			m_pOwner->Set_LightCheck(true);
		}
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


	return S_OK;
}

HRESULT CSpear_State::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_State::Check_State()
{
	if (!m_pOwner->Get_Dead())
	{
		if(!m_pOwner->Get_Groggy()&& !m_pOwner->Get_Half()
			&& m_pStateController->Get_CurStateTag() != L"Hit")
		{
			if (m_pOwner->Get_Guard())
			{
				m_pStateController->Change_State(L"Guard");
				return S_OK;
			}
			if (m_pOwner->Get_Target())
				m_pStateController->Change_State(L"Chaser");
			else if (m_pOwner->Get_Attack())
			{
				_uint iRand = rand() % 2;
				m_pStateController->Change_State(L"Attack", &iRand);
			}
			else
				m_pStateController->Change_State(L"Idle");
			}
		else if (m_pOwner->Get_Half())
		{
			if (!m_pOwner->Get_ChargeOn() && m_pStateController->Get_CurStateTag() != L"Charge_Attack")
			{
				m_pStateController->Change_State(L"Bwd_Dash");
				m_pOwner->Set_GuardCount(5);
			}
			else
			{
				if (m_pOwner->Get_Guard())
				{
					m_pStateController->Change_State(L"Guard");
					return S_OK;
				}
				if (m_pOwner->Get_Target())
					m_pStateController->Change_State(L"Chaser");
				else if (m_pOwner->Get_Attack() && m_pStateController->Get_CurStateTag() != L"Charge_Attack")
				{
					_uint iRand = rand() % 2;
					m_pStateController->Change_State(L"Attack", &iRand);
				}
				else
				{
					if (m_pStateController->Get_CurStateTag() != L"Charge_Attack")
						m_pStateController->Change_State(L"Idle");
				}
			}
		}
	}

	return S_OK;
}

/* �÷��̾� ���� ���� */
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

	//if (m_bTargetOn)
	if (m_pOwner->Get_Target())
		wstrIsChaser = L"TRUE";
	else
		wstrIsChaser = L"FALSE";

	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrChaserOn + wstrIsChaser).c_str(), _float2(950.f, 120.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	//////////////////

	wstring wstrAttackOn = L"Attack On : ";
	wstring wstrIsAttack;

	//if (m_bAttackOn)
	if (m_pOwner->Get_Attack())
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

//void CSpear_State::Check_Attack(const _double& _dDeltaTime)
//{
//	/* ������ ���� Look ���� ���� */
//	_vector vecMonsterLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
//	/* ���Ͱ� �÷��̾�� ���ϴ� ���� ���� */
//	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
//	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
//	/* ���Ϳ� �÷��̾� ������ �Ÿ� */
//	m_fDistance = XMVectorGetX(XMVector3Length(vDist));
//
//	/* �÷��̾ ������ �տ� �ִ��� �ڿ��ִ��� �Ǵ� */
//	_vector vecMonsterToPlayer = XMVector3Normalize(vDist);
//	_vector dotVec = XMVector3Dot(vecMonsterLook, vecMonsterToPlayer);
//	m_CheckFWD = XMVectorGetX(dotVec); /* ����-> �÷��̾ �տ� �ִ�*/
//
//	/* �÷��̾�,���� ��ġ �� ���� */
//	_vector vPlayerToMonster = g_pObserver->Get_PlayerPos() - vMonsterPos;
//
//	/* ��ġ���̿� ���� Y �� 0���� ���� */
//	vecMonsterLook = XMVectorSetY(vecMonsterLook, 0.f);
//	vPlayerToMonster = XMVectorSetY(vPlayerToMonster, 0.f);
//
//	/* �� ������ ���� �� */
//	_vector svAngle = XMVector3AngleBetweenVectors(vecMonsterLook, vPlayerToMonster);
//	XMStoreFloat(&m_fRadian, svAngle);
//	m_fRadian = XMConvertToDegrees(m_fRadian);
//
//	if (!m_bChargeOn)
//	{
//		if (0 > m_CheckFWD && (3.0f < m_fDistance && 10.0f > m_fDistance))
//		{
//			m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
//			m_bTargetOn = true;
//			m_bBattleOn = true;
//		}
//		CMonster_Bastion_Spear* pSpear = static_cast<CMonster_Bastion_Spear*>(m_pMonster);
//
//		if((0 > m_CheckFWD  && !pSpear->m_bGroggy) && g_pObserver->IsAttack())
//			m_pStateController->Change_State(L"Guard");
//		else if (0 > m_CheckFWD && 5.0f > m_fDistance)
//		{
//			if (30.0f > m_fRadian)
//			{
//				m_fAttackTime += (_float)_dDeltaTime;
//				if (m_fAttackTime > 0.5f)
//				{
//					m_fAttackTime = 0.0f;
//					if (!m_bChargeOn && !pSpear->m_bGroggy)
//						m_pStateController->Change_State(L"Attack");
//				}
//			}
//		}
//	}
//}

void CSpear_State::OnTriggerEnter(CCollision& collision)
{
	//if(m_pOwner)
	//	m_pOwner->Hit(collision);
	//if(!m_pOwner->Get_Dead())
	//{
	//	if (true == g_pObserver->IsAttack()) //�÷��̾�����϶�
	//	{
	//		m_pOwner->m_bFirstHit = true; //�� �ѹ� true�� ��������

	//		if (true == m_pOwner->m_bFirstHit)
	//		{
	//			m_pOwner->m_pPanel->Set_BackUIGapY(1.f);
	//		}

	//		if ((_uint)GAMEOBJECT::WEAPON == collision.pGameObject->getTag())
	//		{
	//			if (m_pAnimator->Get_CurrentAnimNode() != (_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_GUARD)
	//			{
	//				m_pOwner->m_pPanel->Set_Show(true);
	//				m_pOwner->Active_Effect((_uint)EFFECT::HIT);
	//				m_pOwner->Active_Effect((_uint)EFFECT::FLOATING);

	//				m_pOwner->Set_Current_HP(-5);
	//				m_pOwner->Set_GroggyGauge(2); //TODO::��ġ���ؼ��ٲ���ߵ�
	//			}
	//			m_pOwner->m_pPanel->Set_HpBar(m_pOwner->Get_HpRatio());

	//			if (false == pSpear->m_bGroggy)
	//			{
	//				//�׷α� �ƴҶ��� �����Ҽ��ְ�
	//				pSpear->m_pPanel->Set_GroggyBar(pSpear->Get_GroggyGaugeRatio());
	//				if (m_pAnimator->Get_CurrentAnimNode() != (_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_GUARD)
	//					m_pStateController->Change_State(L"Hit");
	//			}
	//		}
	//		else
	//		{
	//			m_pStateController->Change_State(L"A_Idle");
	//		}
	//	}
	//}
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
