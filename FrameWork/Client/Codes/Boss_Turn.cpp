#include "pch.h"
#include "Boss_Turn.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Turn::CBoss_Turn(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Turn::CBoss_Turn(const CBoss_Turn& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Turn::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Turn::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);


	/* 몬스터의 현재 Look 방향 벡터 */
	_vector vecMonsterLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	/* 몬스터가 플레이어에게 향하는 방향 벡터 */
	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	/* 몬스터와 플레이어 사이의 거리 */
	_float tfDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	/* 플레이어가 몬스터의 앞에 있는지 뒤에있는지 판단 */
	_vector vecMonsterToPlayer = XMVector3Normalize(vDist);
	_vector dotVec = XMVector3Dot(vecMonsterLook, vecMonsterToPlayer);
	_float CheckFWD = XMVectorGetX(dotVec); /* 음수-> 플레이어가 앞에 있다*/

	/* 플레이어,몬스터 위치 차 벡터 */
	_vector vPlayerToMonster = g_pObserver->Get_PlayerPos() - vMonsterPos;

	/* 위치차이에 따른 Y 값 0으로 셋팅 */
	vecMonsterLook = XMVectorSetY(vecMonsterLook, 0.f);
	vPlayerToMonster = XMVectorSetY(vPlayerToMonster, 0.f);

	/* 두 벡터의 사이 각 */
	_vector svAngle = XMVector3AngleBetweenVectors(vecMonsterLook, vPlayerToMonster);
	_float LookRad = 0.f;
	XMStoreFloat(&LookRad, svAngle);
	LookRad = XMConvertToDegrees(LookRad);

	if (0 > CheckFWD)
	{
		if (10.0f > LookRad)
		{
			static_cast<CBoss_Solaris*>(m_pMonster)->Set_Random_AttackAnim();
		}
	}


	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		static_cast<CBoss_Solaris*>(m_pMonster)->Set_Random_AttackAnim();
	}

	return _int();
}

_int CBoss_Turn::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Turn::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Turn::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(false);

	//플레이어를 향한 방향벡터
	_vector vBossPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector  vDist = vBossPos - g_pObserver->Get_PlayerPos();
	_vector vBossToPlayerLook = XMVector3Normalize(vDist);

	//보스의 현재Look
	_vector vMyLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector vMyUp = m_pTransform->Get_State(CTransform::STATE_UP);

	//플레이어와의 거리 
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	//플레이어가 몬스터의 앞인지 뒤인지
	_vector vecDot = XMVector3Dot(vMyLook, vBossToPlayerLook);
	 _float CheckFWD = XMVectorGetX(vecDot); //음수 = 플레이어가 앞에있다 

	vDist = XMVector3Normalize(XMVectorSetY(vDist, 0.f));
	vMyLook = XMVector3Normalize(XMVectorSetY(vMyLook, 0.f));

	_vector vAngle = XMVector3AngleBetweenVectors(vMyLook, vDist);
	_float fRadian = 0.f;
	XMStoreFloat(&fRadian, vAngle);
	_vector vCross = XMVector3Cross(vMyLook, vDist);

	m_fRadian = fRadian;

	if (0 > CheckFWD)
	{
		cout << "CheckFWD 음수" << endl;

		if (0.f < XMVectorGetY(vCross)) //몬스터왼쪽회전
		{
			fRadian = XMConvertToRadians(180.f - XMConvertToDegrees(m_fRadian));

			m_bLeft = true;

			cout << "왼쪽회전" << endl;
			cout << "Angle : " << XMConvertToDegrees(fRadian) << endl;

			if (0.f <= XMConvertToDegrees(fRadian) && 45.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_45LEFT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 110.f);
				cout << "45" << endl;

			}
			else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_90LEFT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 15.f);
				cout << "90" << endl;

			}
			else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_135LEFT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 11.f);
				cout << "135" << endl;


			}
			else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_180LEFT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 11.f);
				//m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
				cout << "180" << endl;
			}

		}
		else if (0.f >= XMVectorGetY(vCross)) //몬스터오른쪽회전
		{
			fRadian = XMConvertToRadians(180.f - XMConvertToDegrees(m_fRadian));

			m_bLeft = false;

			cout << "오른쪽회전" << endl;
			cout << "Angle : " << XMConvertToDegrees(fRadian) << endl;

			if (0.f <= XMConvertToDegrees(fRadian) && 45.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_45RIGHT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 110.f);
				cout << "45" << endl;

			}
			else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_90RIGHT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 30.f);
				cout << "90" << endl;

			}
			else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_135RIGHT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 11.f);
				cout << "135" << endl;


			}
			else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_180RIGHT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 11.f);
				//m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
				cout << "180" << endl;

			}
		}

	}
	if (0 < CheckFWD)
	{
		cout << "CheckFWD 양수" << endl;

		if (0.f < XMVectorGetY(vCross)) //몬스터왼쪽회전
		{
			fRadian = XMConvertToRadians(180.f - XMConvertToDegrees(m_fRadian));

			m_bLeft = true;

			cout << "왼쪽회전" << endl;
			cout << "Angle : " << XMConvertToDegrees(fRadian) << endl;

			if (0.f <= XMConvertToDegrees(fRadian) && 45.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_45LEFT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 110.f);
				cout << "45" << endl;

			}
			else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_90LEFT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 30.f);
				cout << "90" << endl;

			}
			else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_135LEFT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 11.f);
				cout << "135" << endl;

			}
			else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_180LEFT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 11.f);
				cout << "180" << endl;

			}
		}
		else if (0.f >= XMVectorGetY(vCross)) //몬스터오른쪽회전
		{
			fRadian = XMConvertToRadians(180.f - XMConvertToDegrees(m_fRadian));

			m_bLeft = false;

			cout << "오른쪽회전" << endl;
			cout << "Angle : " << XMConvertToDegrees(fRadian) << endl;

			if (0.f <= XMConvertToDegrees(fRadian) && 45.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_45RIGHT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 110.f);
				cout << "45" << endl;

			}
			else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_90RIGHT);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 30.f);
				cout << "90" << endl;

			}
			else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 23.f);
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_135LEFT);
				//m_pAnimator->Get_AnimController()->Set_RootMotion(true, false, ERootOption::XYZ);
				cout << "135" << endl;

			}
			else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::TURN_135LEFT);
				//m_pAnimator->Get_AnimController()->Set_RootMotion(true, false, ERootOption::XYZ);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(XMConvertToDegrees(fRadian) * 15.5f);
				cout << "180" << endl;

			}
		}
	}

	cout << "=============" << endl;

	return S_OK;
}

HRESULT CBoss_Turn::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_fRadian = 0.f;
	m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);

	cout << "turn end" << endl;

	return S_OK;
}

void CBoss_Turn::Look_Player(void)
{

}

CBoss_Turn* CBoss_Turn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Turn* pInstance = new CBoss_Turn(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Turn Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Turn::Free()
{
	__super::Free();
}
