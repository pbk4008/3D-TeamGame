#include "pch.h"
#include "Bastion_Healer_CastProtect.h"
#include "Monster_Bastion_Healer.h"
#include "Animation.h"

CBastion_Healer_CastProtect::CBastion_Healer_CastProtect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_Healer_State(_pDevice, _pDeviceContext)
{
}

HRESULT CBastion_Healer_CastProtect::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Healer_CastProtect::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Link();
	m_pAnimator->Tick(_dDeltaTime);
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();
	if (pAnim)
	{
		if (pAnim->Is_Finished() 
			&&m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_CAST_PROTECT)
			m_pStateController->Change_State(L"Idle");
	}
	return _int();
}

_int CBastion_Healer_CastProtect::LateTick(const _double& _dDeltaTime)
{				
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_Healer_CastProtect::Render()
{
	return S_OK;
}

void CBastion_Healer_CastProtect::Look_Player(void)
{
}

void CBastion_Healer_CastProtect::Look_Monster(void)
{
	
}

void CBastion_Healer_CastProtect::OnTriggerEnter(CCollision& collision)
{
}

HRESULT CBastion_Healer_CastProtect::EnterState()
{
	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_CAST_PROTECT);

	if(m_pAnimator->Get_CurrentAnimation())
		m_pAnimator->Get_CurrentAnimation()->Reset_Animation();
	
	return S_OK;
}

HRESULT CBastion_Healer_CastProtect::ExitState()
{
	m_pAnimator->Reset_Animation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_CAST_PROTECT);
	return S_OK;
}

_bool CBastion_Healer_CastProtect::Link()
{
	//프레임 체크 후 링크
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();
	if (pAnim)
	{
		_uint dFrame = pAnim->Get_CurrentKeyFrameIndex();

		if (dFrame >= 100 && dFrame < 101)
			g_pGameInstance->Play_Shot(L"Healer_Reinforce", CHANNEL::Healer_Reinforce);

		if (dFrame > 100)
			m_pOwner->Link();
	}
	return true;
}

CBastion_Healer_CastProtect* CBastion_Healer_CastProtect::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Healer_CastProtect* pInstance = new CBastion_Healer_CastProtect(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Healer_CastProtect Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_Healer_CastProtect::Free()
{
	__super::Free();
}
