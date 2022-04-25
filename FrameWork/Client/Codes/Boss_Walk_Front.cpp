#include "pch.h"
#include "Boss_Walk_Front.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Walk_Front::CBoss_Walk_Front(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Walk_Front::CBoss_Walk_Front(const CBoss_Walk_Front& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Walk_Front::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Walk_Front::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);


	_vector vBossPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vDist = vBossPos - g_pObserver->Get_PlayerPos();
	_vector vBossToPlayerLook = XMVector3Normalize(vDist);

	_vector vMyLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector vMyUp = m_pTransform->Get_State(CTransform::STATE_UP);

	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	_vector vecDot = XMVector3Dot(vMyLook, vBossToPlayerLook);
	_float CheckFWD = XMVectorGetX(vecDot); //음수 = 플레이어가 앞에있다 

	vDist = XMVector3Normalize(XMVectorSetY(vDist, 0.f));
	vMyLook = XMVector3Normalize(XMVectorSetY(vMyLook, 0.f));

	_vector vAngle = XMVector3AngleBetweenVectors(vMyLook, vDist);
	_float fRadian = 0.f;
	XMStoreFloat(&fRadian, vAngle);
	_vector vCross = XMVector3Cross(vMyLook, vDist);


	if (0 >= CheckFWD)
	{
		if (0.f > XMVectorGetY(vCross))
		{
			fRadian = XMConvertToRadians(XMConvertToRadians(180.f) - fRadian);

			CheckFWD = 1.f - CheckFWD;
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian * CheckFWD);
			//cout << "1 : 여기" << endl;
		}
		else if(0.f <= XMVectorGetY(vCross))
		{
			fRadian = XMConvertToRadians(XMConvertToRadians(180.f) - fRadian);

			//CheckFWD = CheckFWD;
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian * CheckFWD * 2.f);
			//cout << "1 : 아님" << endl;
		}
	}
	else if (0 < CheckFWD)
	{
		if (0.f > XMVectorGetY(vCross))
		{
			//fRadian = XMConvertToRadians(XMConvertToRadians(180.f) - fRadian);
			//
			//CheckFWD = 1.f - CheckFWD;
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian * CheckFWD);
			//cout << "2 : 여기" << endl;
		}
		else if (0.f <= XMVectorGetY(vCross))
		{
			fRadian = XMConvertToRadians(XMConvertToRadians(180.f) - fRadian);

			//CheckFWD = CheckFWD;
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian * CheckFWD);
			//cout << "2 : 아님" << endl;
		}
	}

	if ((_uint)CBoss_Solaris::M_BossAnimState::WALK_FWD_STOP == m_pAnimator->Get_CurrentAnimNode())
	{
		if (m_pAnimator->Get_AnimController()->Is_Finished())
		{
			if (15.f > fDistToPlayer)
			{
				//다른어택상태로 바꿔줌
				_int iRandom = rand() % 5;

				switch (iRandom)
				{
				case 0:
					m_pStateController->Change_State(L"Attack_Agg");
					break;
				case 1:
					m_pStateController->Change_State(L"Attack_R2");
					break;
				case 2:
					m_pStateController->Change_State(L"Attack_S3");
					break;
				case 3:
					m_pStateController->Change_State(L"Attack_R1");
					break;
				case 4:
					m_pStateController->Change_State(L"Attack_S5_Protocol");
					break;
				}
			}
			else if (15.f <= fDistToPlayer)
			{
				m_pStateController->Change_State(L"Turn");
			}
		}
	}


	return _int();
}

_int CBoss_Walk_Front::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Walk_Front::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Walk_Front::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	cout << "Walk Front" << endl;

	m_vLook = { 0.f, 0.f, 0.f,0.f };
	m_vRight = { 0.f, 0.f, 0.f,0.f };

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(true);
	m_pAnimator->Get_AnimController()->Set_MoveSpeed(20.0f);
	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::WALK_FWD_START);
	
	//_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	//_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	//_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	return S_OK;
}

HRESULT CBoss_Walk_Front::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.0f);

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CBoss_Walk_Front::Look_Player(void)
{
	
}

CBoss_Walk_Front* CBoss_Walk_Front::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Walk_Front* pInstance = new CBoss_Walk_Front(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Walk_Front Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Walk_Front::Free()
{
	__super::Free();
}
