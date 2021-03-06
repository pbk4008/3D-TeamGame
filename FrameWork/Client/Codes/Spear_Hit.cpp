#include "pch.h"
#include "Spear_Hit.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Spear.h"

CSpear_Hit::CSpear_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSpear_State(_pDevice, _pDeviceContext)
{
}

CSpear_Hit::CSpear_Hit(const CSpear_Hit& _rhs)
	: CSpear_State(_rhs)
{
}

HRESULT CSpear_Hit::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSpear_Hit::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);
	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		m_pStateController->Change_State(L"Idle");
	}

	return _int();
}

_int CSpear_Hit::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSpear_Hit::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpear_Hit::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Spear_Hit);
	g_pGameInstance->Play_Shot(L"Monster_Hit_6", CSoundMgr::CHANNELID::Spear_Hit);
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Spear::ANIM_TYPE::A_FLINCH);

	return S_OK;
}

HRESULT CSpear_Hit::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Monster_Hit);

	return S_OK;
}

void CSpear_Hit::Look_Player(void)
{
}

void CSpear_Hit::Look_Monster(void)
{
}

CSpear_Hit* CSpear_Hit::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSpear_Hit* pInstance = new CSpear_Hit(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSpear_Hit Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpear_Hit::Free()
{
	__super::Free();
}
