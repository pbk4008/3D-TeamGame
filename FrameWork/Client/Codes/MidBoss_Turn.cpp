#include "pch.h"
#include "MidBoss_Turn.h"

#include "Animation.h"
#include "Boss_Bastion_Judicator.h"

CMidBoss_Turn::CMidBoss_Turn(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CMidBoss_Turn::CMidBoss_Turn(const CMidBoss_Turn& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CMidBoss_Turn::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMidBoss_Turn::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;
	cout << "Turn tick" << endl;
	m_pAnimator->Tick(TimeDelta);

	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		/*_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
		_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
		_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

		if (10.f > fDistToPlayer)
		{
			m_pStateController->Change_State(L"Attack");
			cout << "turn update" << endl;
		}

		if (10.f <= fDistToPlayer)
		{
			m_pStateController->Change_State(L"Run");
		}*/
		m_pStateController->Change_State(L"Run");
	}

	return _int();
}

_int CMidBoss_Turn::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CMidBoss_Turn::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMidBoss_Turn::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	//플레이어를 향한 방향벡터
	_vector vBossPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vBossPos - g_pObserver->Get_PlayerPos();
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
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_45LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "45" << endl;

			}
			else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_90LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "90" << endl;

			}
			else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_135LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "135" << endl;


			}
			else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_180LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
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
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_45RIGHT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "45" << endl;

			}
			else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_90RIGHT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "90" << endl;

			}
			else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_135RIGHT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "135" << endl;


			}
			else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_180RIGHT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
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
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_45LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "45" << endl;

			}
			else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_90LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "90" << endl;

			}
			else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_135LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "135" << endl;

			}
			else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_180LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
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
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_45RIGHT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "45" << endl;

			}
			else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_90RIGHT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
				cout << "90" << endl;

			}

			//TODO : 이거 애니메이션 RIGHT로 변경해야함
			else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_135LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(1200.f);
				cout << "135" << endl;


			}
			else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
			{
				m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_180LEFT_H);
				m_pAnimator->Get_AnimController()->Set_RotSpeed(700.f);
				//m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
				cout << "180" << endl;
			}
		}
	}

	cout << "=============" << endl;
	return S_OK;
}

HRESULT CMidBoss_Turn::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_fRadian = 0.f;
	m_pAnimator->Get_AnimController()->Set_RotSpeed(500.f);
	m_TurnTime = 0.f;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	cout << "turn end" << endl;

	return S_OK;
}

void CMidBoss_Turn::Look_Player(void)
{

}

CMidBoss_Turn* CMidBoss_Turn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CMidBoss_Turn* pInstance = new CMidBoss_Turn(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMidBoss_Turn Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMidBoss_Turn::Free()
{
	__super::Free();
}
