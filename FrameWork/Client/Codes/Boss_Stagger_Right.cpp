#include "pch.h"
#include "Boss_Stagger_Right.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Stagger_Right::CBoss_Stagger_Right(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Stagger_Right::CBoss_Stagger_Right(const CBoss_Stagger_Right& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Stagger_Right::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Stagger_Right::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	cout << "Stagger_Right" << endl;
	
	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		_vector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
		_vector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
		_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

		if (0.6f < m_pMonster->Get_HpRatio())
		{
			//레이저없음

			if (30.f > fDistToPlayer)
			{
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
			else if (30.f <= fDistToPlayer)
			{
				m_pStateController->Change_State(L"Attack_S5_Protocol");
			}
		}
		else if (0.6f >= m_pMonster->Get_HpRatio())
		{
			if (30.f > fDistToPlayer)
			{
				_int iRandom = rand() % 7;

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
				case 5:
					m_pStateController->Change_State(L"Attack_S6");
					break;
				case 6:
					m_pStateController->Change_State(L"Attack_S2_Variant");
					break;
				}
			}
			else if (30.f <= fDistToPlayer)
			{
				_int iRandom = rand() % 2;

				switch (iRandom)
				{
				case 0:
					m_pStateController->Change_State(L"Attack_S6");
					break;
				case 1:
					m_pStateController->Change_State(L"Attack_S5_Protocol");
					break;
				}
			}
		}
	}

	return _int();
}

_int CBoss_Stagger_Right::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Stagger_Right::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Stagger_Right::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Get_AnimController()->Set_MoveSpeed(40.0f);

	//g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//g_pGameInstance->Play_Shot(L"MidBoss_Roar", CSoundMgr::CHANNELID::MidBoss);
	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::STAGGER_RIGHT);

	return S_OK;
}

HRESULT CBoss_Stagger_Right::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CBoss_Stagger_Right::Look_Player(void)
{

}

CBoss_Stagger_Right* CBoss_Stagger_Right::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Stagger_Right* pInstance = new CBoss_Stagger_Right(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Stagger_Right Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Stagger_Right::Free()
{
	__super::Free();
}
