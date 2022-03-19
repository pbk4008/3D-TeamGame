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

	m_pAnimator->Tick(TimeDelta);

	//_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	//_vector vDist = vMonsterPos - XMLoadFloat3(&g_pObserver->m_fPos);
	//_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	//vDist = XMVector3Normalize(vDist); //보스가 플레이어를 바라보는 방향벡터
	//_vector vMyLook = m_pTransform->Get_State(CTransform::STATE_LOOK); //몬스터가 실제로 보고있는 방향벡터
	//_vector vMyUp = m_pTransform->Get_State(CTransform::STATE_UP);

	//vDist = XMVector3Normalize(XMVectorSetY(vDist, 0.f));
	//vMyLook = XMVector3Normalize(XMVectorSetY(vMyLook, 0.f));

	//_vector vAngle = XMVector3AngleBetweenVectors(vMyLook, vDist);
	//_float fRadian;
	//XMStoreFloat(&fRadian, vAngle);
	//_vector vCross = XMVector3Cross(vMyLook, vDist);

	//if (45.f > XMConvertToDegrees(m_fRadian))
	//{
	//	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_180RIGHT_H);
	//	//m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_180RIGHT_H);
	//	//m_pStateController->Change_State(L"Attack");
	//	cout << "각이너무 작아서 어택" <<endl;
	//}
	//
	//else
	//{
	//	if (XMConvertToDegrees(m_fRadian) > m_fRadAcc)
	//	{
	//		cout << XMVectorGetY(vCross) << endl;

	//		m_fRadAcc += TimeDelta * 0.1f;

	//		if (m_bLeft) //몬스터왼쪽회전
	//		{
	//			m_pTransform->Rotation_Axis(CTransform::STATE_UP, TimeDelta * -1.f);
	//			//cout << " 1 " << endl;
	//		}

	//		else if (0.f > XMVectorGetY(vCross)) //몬스터오른쪽회전
	//		{
	//			m_pTransform->Rotation_Axis(CTransform::STATE_UP, TimeDelta * 1.f);
	//			//cout << " 2 " << endl;
	//		}
	//	}
	//}
	//
	///*if (XMConvertToDegrees(m_fRadian) <= m_fRadAcc)
	//{
	//	m_pTransform->Rotation_Axis(CTransform::STATE_UP, 0.f);
	//	cout << " 3 " << endl;
	//}*/


	
	if (m_pAnimator->Get_AnimController()->Is_Finished())
	{
		m_pStateController->Change_State(L"Attack");
		cout << "턴->어택" << endl;
	}
	
	m_TurnTime += TimeDelta;
	m_fRadAcc += TimeDelta * 0.1f;

	if (m_TurnTime < 1.f)
	{
		if (5.f < XMConvertToDegrees(m_fRadian))
		{
			if (XMConvertToDegrees(m_fRadian) > m_fRadAcc)
			{
				cout << XMConvertToDegrees(m_fRadian) << endl;

				if (m_bLeft) //몬스터왼쪽회전
				{
					m_pTransform->Rotation_Axis(CTransform::STATE_UP, TimeDelta * -1.f);
				}

				else if (!m_bLeft) //몬스터오른쪽회전
				{
					m_pTransform->Rotation_Axis(CTransform::STATE_UP, TimeDelta * 1.f);
				}
			}
		}
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

	//m_pAnimator->Get_AnimController()->Set_RotSpeed(0.f);

	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - XMLoadFloat3(&g_pObserver->m_fPos);
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	vDist = XMVector3Normalize(vDist); //보스가 플레이어를 바라보는 방향벡터
	_vector vMyLook = m_pTransform->Get_State(CTransform::STATE_LOOK); //몬스터가 실제로 보고있는 방향벡터
	_vector vMyUp = m_pTransform->Get_State(CTransform::STATE_UP);

	vDist = XMVector3Normalize(XMVectorSetY(vDist, 0.f));
	vMyLook = XMVector3Normalize(XMVectorSetY(vMyLook, 0.f));

	_vector vAngle = XMVector3AngleBetweenVectors(vMyLook, vDist);
	_float fRadian;
	XMStoreFloat(&fRadian, vAngle);
	_vector vCross = XMVector3Cross(vMyLook, vDist);

	m_fRadian = fRadian;
	cout << m_fRadian << endl;
	if (0.f < XMVectorGetY(vCross)) //몬스터왼쪽회전
	{
		m_bLeft = true; 

		if (25.f > XMConvertToDegrees(fRadian))
		{
			m_pTransform->Face_Target(XMLoadFloat3(&g_pObserver->m_fPos));
		}
		else if (25.f <= XMConvertToDegrees(fRadian) && 45.f > XMConvertToDegrees(fRadian))
		{
			m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_45LEFT_H);
			m_pAnimator->Get_AnimController()->Set_RotSpeed(1400.f);

		}
		else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
		{
			m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_90LEFT_H);
			m_pAnimator->Get_AnimController()->Set_RotSpeed(700.f);

		}
		else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
		{
			m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_135LEFT_H);
			m_pAnimator->Get_AnimController()->Set_RotSpeed(300.f);

		}
		else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
		{
			m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_180LEFT_H);
			m_pAnimator->Get_AnimController()->Set_RotSpeed(100.f);

		}
	}
	else if (0.f > XMVectorGetY(vCross)) //몬스터오른쪽회전
	{
		m_bLeft = false;

		if (25.f > XMConvertToDegrees(fRadian))
		{
			m_pTransform->Face_Target(XMLoadFloat3(&g_pObserver->m_fPos));
		}
		else if (25.f <= XMConvertToDegrees(fRadian) && 45.f > XMConvertToDegrees(fRadian))
		{
			m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_45RIGHT_H);
			m_pAnimator->Get_AnimController()->Set_RotSpeed(1400.f);

		}
		else if (45.f <= XMConvertToDegrees(fRadian) && 90.f > XMConvertToDegrees(fRadian))
		{
			m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_90RIGHT_H);
			m_pAnimator->Get_AnimController()->Set_RotSpeed(700.f);

		}
		else if (90.f <= XMConvertToDegrees(fRadian) && 135.f > XMConvertToDegrees(fRadian))
		{
			m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_135RIGHT_H);
			m_pAnimator->Get_AnimController()->Set_RotSpeed(300.f);

		}
		else if (135.f <= XMConvertToDegrees(fRadian) && 180.f > XMConvertToDegrees(fRadian))
		{
			m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Bastion_Judicator::M_BossAnimState::TURN_180RIGHT_H);
			m_pAnimator->Get_AnimController()->Set_RotSpeed(100.f);

		}
	}
	else
	{
		cout << "아무거에도 안속함, 어택으로넘어감 " << endl;
		m_pStateController->Change_State(L"Attack");
	}

	return S_OK;
}

HRESULT CMidBoss_Turn::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_fRadian = 0.f;
	m_pAnimator->Get_AnimController()->Set_RotSpeed(700.f);
	m_TurnTime = 0.f;
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
