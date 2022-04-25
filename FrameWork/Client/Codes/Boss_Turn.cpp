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


	/* ������ ���� Look ���� ���� */
	_vector vecMonsterLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	/* ���Ͱ� �÷��̾�� ���ϴ� ���� ���� */
	_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	/* ���Ϳ� �÷��̾� ������ �Ÿ� */
	_float tfDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	/* �÷��̾ ������ �տ� �ִ��� �ڿ��ִ��� �Ǵ� */
	_vector vecMonsterToPlayer = XMVector3Normalize(vDist);
	_vector dotVec = XMVector3Dot(vecMonsterLook, vecMonsterToPlayer);
	_float CheckFWD = XMVectorGetX(dotVec); /* ����-> �÷��̾ �տ� �ִ�*/

	/* �÷��̾�,���� ��ġ �� ���� */
	_vector vPlayerToMonster = g_pObserver->Get_PlayerPos() - vMonsterPos;

	/* ��ġ���̿� ���� Y �� 0���� ���� */
	vecMonsterLook = XMVectorSetY(vecMonsterLook, 0.f);
	vPlayerToMonster = XMVectorSetY(vPlayerToMonster, 0.f);

	/* �� ������ ���� �� */
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

	//�÷��̾ ���� ���⺤��
	_vector vBossPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector  vDist = vBossPos - g_pObserver->Get_PlayerPos();
	_vector vBossToPlayerLook = XMVector3Normalize(vDist);

	//������ ����Look
	_vector vMyLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector vMyUp = m_pTransform->Get_State(CTransform::STATE_UP);

	//�÷��̾���� �Ÿ� 
	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	//�÷��̾ ������ ������ ������
	_vector vecDot = XMVector3Dot(vMyLook, vBossToPlayerLook);
	 _float CheckFWD = XMVectorGetX(vecDot); //���� = �÷��̾ �տ��ִ� 

	vDist = XMVector3Normalize(XMVectorSetY(vDist, 0.f));
	vMyLook = XMVector3Normalize(XMVectorSetY(vMyLook, 0.f));

	_vector vAngle = XMVector3AngleBetweenVectors(vMyLook, vDist);
	_float fRadian = 0.f;
	XMStoreFloat(&fRadian, vAngle);
	_vector vCross = XMVector3Cross(vMyLook, vDist);

	m_fRadian = fRadian;

	if (0 > CheckFWD)
	{
		cout << "CheckFWD ����" << endl;

		if (0.f < XMVectorGetY(vCross)) //���Ϳ���ȸ��
		{
			fRadian = XMConvertToRadians(180.f - XMConvertToDegrees(m_fRadian));

			m_bLeft = true;

			cout << "����ȸ��" << endl;
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
		else if (0.f >= XMVectorGetY(vCross)) //���Ϳ�����ȸ��
		{
			fRadian = XMConvertToRadians(180.f - XMConvertToDegrees(m_fRadian));

			m_bLeft = false;

			cout << "������ȸ��" << endl;
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
		cout << "CheckFWD ���" << endl;

		if (0.f < XMVectorGetY(vCross)) //���Ϳ���ȸ��
		{
			fRadian = XMConvertToRadians(180.f - XMConvertToDegrees(m_fRadian));

			m_bLeft = true;

			cout << "����ȸ��" << endl;
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
		else if (0.f >= XMVectorGetY(vCross)) //���Ϳ�����ȸ��
		{
			fRadian = XMConvertToRadians(180.f - XMConvertToDegrees(m_fRadian));

			m_bLeft = false;

			cout << "������ȸ��" << endl;
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
