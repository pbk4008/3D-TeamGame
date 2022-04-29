#include "pch.h"
#include "Boss_Dash_Back.h"

#include "Animation.h"
#include "Boss_Solaris.h"

CBoss_Dash_Back::CBoss_Dash_Back(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CMonster_FSM(pDevice, pDeviceContext)
{
}

CBoss_Dash_Back::CBoss_Dash_Back(const CBoss_Dash_Back& rhs)
	: CMonster_FSM(rhs)
{
}

HRESULT CBoss_Dash_Back::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBoss_Dash_Back::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		static_cast<CBoss_Solaris*>(m_pMonster)->Set_Random_AttackAnim();
	}

	//어택 체크
	_uint iCurKeyFrameIndex = m_pAnimator->Get_AnimController()->Get_CurKeyFrameIndex();

	cout << iCurKeyFrameIndex << endl;

	if (25 <= iCurKeyFrameIndex && 27 >= iCurKeyFrameIndex && false == m_bOnEffect)
	{
		_matrix matPos = m_pTransform->Get_WorldMatrix();
		matPos.r[3] = XMVectorSetY(matPos.r[3], XMVectorGetY(matPos.r[3]) - 1.0f);
		m_pMonster->Active_Effect_Target((_uint)EFFECT::HIT_GROUND_SMOKE, matPos);
		m_bOnEffect = true;
	}

	return _int();
}

_int CBoss_Dash_Back::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoss_Dash_Back::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Dash_Back::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	cout << "Dash_Back" << endl;

	m_bOnEffect = false;
	m_pAnimator->Get_AnimController()->Set_MoveSpeed(35.0f);
	m_pAnimator->Get_AnimController()->Set_PlaySpeed(1.5f);

	static_cast<CBoss_Solaris*>(m_pMonster)->Set_HitMotion(true);

	//g_pGameInstance->StopSound(CSoundMgr::CHANNELID::MidBoss);
	//g_pGameInstance->Play_Shot(L"MidBoss_Roar", CSoundMgr::CHANNELID::MidBoss);
	m_pAnimator->Change_AnyEntryAnimation((_uint)CBoss_Solaris::M_BossAnimState::DASH_BWD);

	return S_OK;
}

HRESULT CBoss_Dash_Back::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_bOnEffect = false;

	//_vector vec = { 0.f, 1.f, 0.f,0.f };
	//m_pTransform->SetUp_Rotation(vec, (XMConvertToRadians(0.f)));

	return S_OK;
}

void CBoss_Dash_Back::Look_Player(void)
{

}

CBoss_Dash_Back* CBoss_Dash_Back::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CBoss_Dash_Back* pInstance = new CBoss_Dash_Back(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoss_Dash_Back Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Dash_Back::Free()
{
	__super::Free();
}
